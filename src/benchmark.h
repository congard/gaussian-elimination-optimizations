#ifndef GAUSSELIMINATION_BENCHMARK_H
#define GAUSSELIMINATION_BENCHMARK_H

void benchmark_2d(void (*ge)(double **A, int size), int size);
void benchmark_1d(void (*ge)(double *A, int size), int size);

#endif //GAUSSELIMINATION_BENCHMARK_H
