#include <math.h>
#include "benchmark.h"

#define BLKSIZE 8

void ge(double **A, int SIZE) {
    register int i, j, k;
    register double multiplier;

    for (k = 0; k < SIZE; k++) {
        for (i = k + 1; i < SIZE; i++) {
            multiplier = A[i][k] / A[k][k];

            for (j = k + 1; j < SIZE;) {
                if (j < (fmax(SIZE - BLKSIZE, 0))) {
                    #define Aij(_i) \
                        A[i][j + _i] = A[i][j + _i] - A[k][j + _i] * multiplier

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
                    A[i][j] = A[i][j] - A[k][j] * multiplier;
                    j++;
                }
            }
        }
    }
}

int main() {
    benchmark_2d(ge, MAT_SIZE);
    return 0;
}
