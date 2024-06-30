#include "benchmark.h"

void ge(double **A, int SIZE) {
    int i, j, k;

    for (k = 0; k < SIZE; k++) {
        for (i = k + 1; i < SIZE; i++) {
            for (j = k + 1; j < SIZE; j++) {
                A[i][j] = A[i][j] - A[k][j] * (A[i][k] / A[k][k]);
            }
        }
    }
}

int main() {
    benchmark_2d(ge, MAT_SIZE);
    return 0;
}
