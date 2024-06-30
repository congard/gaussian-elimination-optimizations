#include "benchmark.h"

void ge(double **A, int SIZE) {
    register int i, j, k;
    register double multiplier;

    for (k = 0; k < SIZE; k++) {
        for (i = k + 1; i < SIZE; i++) {
            multiplier = A[i][k] / A[k][k];

            for (j = k + 1; j < SIZE; j++) {
                A[i][j] = A[i][j] - A[k][j] * multiplier;
            }
        }
    }
}

int main() {
    benchmark_2d(ge, MAT_SIZE);
    return 0;
}
