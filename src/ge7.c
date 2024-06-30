#include <math.h>
#include <immintrin.h>

#include "benchmark.h"

#define BLKSIZE 8
#define IDX(i, j) ((j) + (i) * SIZE)

void ge(double *A, int SIZE) {
    register int i, j, k;
    register double multiplier;

    register __m256d mm_multiplier;
    register __m256d tmp0, tmp1, tmp2, tmp3;

    for (k = 0; k < SIZE; k++) {
        for (i = k + 1; i < SIZE; i++) {
            multiplier = A[IDX(i, k)] / A[IDX(k, k)];

            mm_multiplier[0] = multiplier;
            mm_multiplier[1] = multiplier;
            mm_multiplier[2] = multiplier;
            mm_multiplier[3] = multiplier;

            for (j = k + 1; j < SIZE;) {
                if (j < (fmax(SIZE - BLKSIZE, 0))) {
                    tmp0 = _mm256_loadu_pd(A + IDX(i, j));
                    tmp2 = _mm256_loadu_pd(A + IDX(i, j + 4));

                    tmp1 = _mm256_loadu_pd(A + IDX(k, j));
                    tmp3 = _mm256_loadu_pd(A + IDX(k, j + 4));

                    tmp1 = _mm256_mul_pd(tmp1, mm_multiplier);
                    tmp3 = _mm256_mul_pd(tmp3, mm_multiplier);

                    tmp0 = _mm256_sub_pd(tmp0, tmp1);
                    tmp2 = _mm256_sub_pd(tmp2, tmp3);

                    _mm256_storeu_pd(A + IDX(i, j), tmp0);
                    _mm256_storeu_pd(A + IDX(i, j + 4), tmp2);

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
