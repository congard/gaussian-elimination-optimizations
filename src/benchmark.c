#include "benchmark.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#include <papi.h>

#define ERROR_RETURN(retval) do { \
    fprintf(stderr, "Error %d %s:line %d: \n", retval, __FILE__, __LINE__); \
    exit(retval); \
} while (0)

static double gtod_ref_time_sec = 0.0;

/* Adapted from the bl2_clock() routine in the BLIS library */

static double dclock() {
    double the_time;
    double norm_sec;

    struct timeval tv;
    gettimeofday(&tv, NULL);

    if (gtod_ref_time_sec == 0.0)
        gtod_ref_time_sec = (double) tv.tv_sec;

    norm_sec = (double) tv.tv_sec - gtod_ref_time_sec;
    the_time = norm_sec + (double) tv.tv_usec * 1.0e-6;

    return the_time;
}

typedef enum MatDim {
    D1, D2
} MatDim_t;

typedef void (*ge_t)(void *mat, int size);
typedef void (*benchmark_t)(ge_t ge, void *mat, int size);

static void benchmark_time(ge_t ge, void *mat, int size) {
    printf("Benchmarking time...\n");

    double dtime = dclock();
    ge(mat, size);
    dtime = dclock() - dtime;

    printf("Time: %le sec\n", dtime);
}

static void benchmark_flops(ge_t ge, void *mat, int size) {
    printf("Benchmarking flops...\n");

    float real_time;
    float proc_time;
    float mflops;

    long long flpops;

    float ireal_time;
    float iproc_time;
    float imflops;

    long long iflpops;
    int retval;

    if ((retval = PAPI_flops_rate(PAPI_FP_OPS, &ireal_time, &iproc_time, &iflpops, &imflops)) < PAPI_OK){
        printf("Could not initialise PAPI_flops \n");
        printf("Your platform may not support floating point operation event.\n");
        ERROR_RETURN(retval);
    }

    ge(mat, size);

    if ((retval = PAPI_flops_rate(PAPI_FP_OPS, &real_time, &proc_time, &flpops, &mflops)) < PAPI_OK) {
        ERROR_RETURN(retval);
    }

    if ((retval = PAPI_rate_stop()) < PAPI_OK) {
        ERROR_RETURN(retval);
    }

    printf("Real_time: %f\nProc_time: %f\nflpops: %lld\nMFLOPS: %f\n", real_time, proc_time, flpops, mflops);
}

static void papi_run(ge_t ge, void *mat, int size, int *event_codes, int event_count, long long *result) {
    int retval;

    /* PAPI counters variables */
    int eventSet = PAPI_NULL;
    /*must be initialized to PAPI_NULL before calling PAPI_create_event*/

    /* initializing library */
    if ((retval = PAPI_library_init(PAPI_VER_CURRENT)) != PAPI_VER_CURRENT) {
        ERROR_RETURN(retval);
    }

    /* Creating event set   */
    if ((retval = PAPI_create_eventset(&eventSet)) != PAPI_OK){
        ERROR_RETURN(retval);
    }

    /* Add the array of events PAPI_TOT_INS and PAPI_TOT_CYC to the eventset*/
    if ((retval = PAPI_add_events(eventSet, event_codes, event_count)) != PAPI_OK){
        ERROR_RETURN(retval);
    }

    /* Start counting */
    if ((retval = PAPI_start(eventSet)) != PAPI_OK){
        ERROR_RETURN(retval);
    }

    ge(mat, size);

    /* Stop counting, this reads from the counter as well as stop it. */
    if ((retval = PAPI_stop(eventSet, result)) != PAPI_OK){
        ERROR_RETURN(retval);
    }
}

static void benchmark_ins_cyc(ge_t ge, void *mat, int size) {
    int event_codes[2] = {PAPI_TOT_INS, PAPI_TOT_CYC};
    long long values[2];

    papi_run(ge, mat, size, event_codes, 2, values);

    printf("Total instructions executed: %lld\nTotal cycles: %lld\n", values[0], values[1]);
}

static void benchmark_cond_taken_mispred(ge_t ge, void *mat, int size) {
    int event_codes[2] = {PAPI_BR_TKN, PAPI_BR_MSP};
    long long values[2];

    papi_run(ge, mat, size, event_codes, 2, values);

    printf(
        "Conditional branch instructions taken: %lld\nConditional branch instructions mispredicted: %lld\n",
        values[0], values[1]
    );
}

static void run_bench(benchmark_t benchmark, ge_t ge, int size, MatDim_t dim) {
    double *matrix_1d = malloc(sizeof(double) * size * size);
    double **matrix_2d = malloc(sizeof(double*) * size);

    for (int i = 0; i < size; ++i)
        matrix_2d[i] = &matrix_1d[i * size];

    srand(1);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix_2d[i][j] = rand();
        }
    }

    void *mat;

    switch (dim) {
        case D1:
            mat = matrix_1d;
            break;
        case D2:
            mat = matrix_2d;
            break;
    }

    printf("Starting benchmarking...\n");

    benchmark(ge, mat, size);

    double check = 0.0;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            check = check + matrix_2d[i][j];
        }
    }

    printf("Check: %le \n", check);

    fflush(stdout);

    free(matrix_1d);
    free(matrix_2d);
}

#define RUN_BENCH(name) do { \
    printf("Starting benchmark: " #name "\n"); \
    run_bench(name, ge, size, dim);            \
    printf("\n"); \
} while (0)

static void benchmark(ge_t ge, int size, MatDim_t dim) {
    RUN_BENCH(benchmark_time);
    RUN_BENCH(benchmark_flops);
    RUN_BENCH(benchmark_ins_cyc);
    RUN_BENCH(benchmark_cond_taken_mispred);
}

void benchmark_2d(void (*ge)(double **A, int size), int size) {
    benchmark((ge_t) ge, size, D2);
}

void benchmark_1d(void (*ge)(double *A, int size), int size) {
    benchmark((ge_t) ge, size, D1);
}
