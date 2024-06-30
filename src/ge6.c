#include <math.h>
#include <x86intrin.h>

#include "benchmark.h"

#define BLKSIZE 8
#define IDX(i, j) ((j) + (i) * SIZE)

void ge(double *A, int SIZE) {
    register int i, j, k;
    register double multiplier;

    register __m128d mm_multiplier;
    register __m128d tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;

    for (k = 0; k < SIZE; k++) {
        for (i = k + 1; i < SIZE; i++) {
            multiplier = A[IDX(i, k)] / A[IDX(k, k)];

            mm_multiplier[0] = multiplier;
            mm_multiplier[1] = multiplier;

            for (j = k + 1; j < SIZE;) {
                if (j < (fmax(SIZE - BLKSIZE, 0))) {
                    tmp0 = _mm_loadu_pd(A + IDX(i, j));
                    tmp2 = _mm_loadu_pd(A + IDX(i, j + 2));
                    tmp4 = _mm_loadu_pd(A + IDX(i, j + 4));
                    tmp6 = _mm_loadu_pd(A + IDX(i, j + 6));

                    tmp1 = _mm_loadu_pd(A + IDX(k, j));
                    tmp3 = _mm_loadu_pd(A + IDX(k, j + 2));
                    tmp5 = _mm_loadu_pd(A + IDX(k, j + 4));
                    tmp7 = _mm_loadu_pd(A + IDX(k, j + 6));

                    tmp1 = _mm_mul_pd(tmp1, mm_multiplier);
                    tmp3 = _mm_mul_pd(tmp3, mm_multiplier);
                    tmp5 = _mm_mul_pd(tmp5, mm_multiplier);
                    tmp7 = _mm_mul_pd(tmp7, mm_multiplier);

                    tmp0 = _mm_sub_pd(tmp0, tmp1);
                    tmp2 = _mm_sub_pd(tmp2, tmp3);
                    tmp4 = _mm_sub_pd(tmp4, tmp5);
                    tmp6 = _mm_sub_pd(tmp6, tmp7);

                    _mm_storeu_pd(A + IDX(i, j), tmp0);
                    _mm_storeu_pd(A + IDX(i, j + 2), tmp2);
                    _mm_storeu_pd(A + IDX(i, j + 4), tmp4);
                    _mm_storeu_pd(A + IDX(i, j + 6), tmp6);

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
