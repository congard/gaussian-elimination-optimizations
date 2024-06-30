#include <math.h>
#include "benchmark.h"

#define BLKSIZE 8
#define IDX(i, j) ((j) + (i) * SIZE)

void ge(double *A, int SIZE) {
    register int i, j, k;
    register double multiplier;

    for (k = 0; k < SIZE; k++) {
        for (i = k + 1; i < SIZE; i++) {
            multiplier = A[IDX(i, k)] / A[IDX(k, k)];

            for (j = k + 1; j < SIZE;) {
                if (j < (fmax(SIZE - BLKSIZE, 0))) {
                    #define Aij(_i) \
                        A[IDX(i, j + _i)] = A[IDX(i, j + _i)] - A[IDX(k, j + _i)] * multiplier

                    Aij(0);
                    Aij(1);
                    Aij(2);
                    Aij(3);
                    Aij(4);
                    Aij(5);
                    Aij(6);
                    Aij(7);
                    
                    j += BLKSIZE;
                } else {
                    A[IDX(i, j)] = A[IDX(i, j)] - A[IDX(k, j)] * multiplier;
                    j++;
                }
            }
        }
    }
}

int main() {
    benchmark_1d(ge, MAT_SIZE);
    return 0;
}
