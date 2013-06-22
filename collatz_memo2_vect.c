#include <stdio.h>
#include <stdlib.h>
#include <smmintrin.h>

typedef unsigned long long int ULLI;
typedef unsigned int UI;
typedef unsigned short int USI;

#define SIZE 100000000

#define COLLATZ(_a, _cycle_len, _n) {               \
            while (_a > 1 & _a >= _n) {             \
                if (_a & 1) {                       \
                    _a = ((_a<<1) + _a + 1)>>1;     \
                    _cycle_len += 2;                \
                }                                   \
                else {                              \
                    _a = _a>>1;                     \
                    _cycle_len += 1;                \
                }                                   \
            }                                       \
        }                                           \

int main() {
    UI a, b;
    scanf("%u%u", &a, &b);

    // Precomputation

    USI *prec = (USI *)calloc(SIZE, 2);
    prec[1] = 1;
    prec[2] = 2;
    prec[3] = 8;
    prec[4] = 3;
    prec[5] = 6;
    prec[6] = 9;
    prec[7] = 17;
    prec[8] = 4;
    UI i;
    USI cycle_len;
    for (i = 9; i < SIZE; ++i) {
        ULLI aux = i;
        cycle_len = 0;
        COLLATZ(aux, cycle_len, i);
        prec[i] = cycle_len + prec[aux];

        aux = i + 1;
        i += 1;
        cycle_len = 0;
        COLLATZ(aux, cycle_len, i);
        prec[i] = cycle_len + prec[aux];

        aux = i + 1;
        i += 1;
        cycle_len = 0;
        COLLATZ(aux, cycle_len, i);
        prec[i] = cycle_len + prec[aux];

        aux = i + 1;
        i += 1;
        cycle_len = 0;
        COLLATZ(aux, cycle_len, i);
        prec[i] = cycle_len + prec[aux];

        aux = i + 1;
        i += 1;
        cycle_len = 0;
        COLLATZ(aux, cycle_len, i);
        prec[i] = cycle_len + prec[aux];

        aux = i + 1;
        i += 1;
        cycle_len = 0;
        COLLATZ(aux, cycle_len, i);
        prec[i] = cycle_len + prec[aux];

        aux = i + 1;
        i += 1;
        cycle_len = 0;
        COLLATZ(aux, cycle_len, i);
        prec[i] = cycle_len + prec[aux];

        aux = i + 1;
        i += 1;
        cycle_len = 0;
        COLLATZ(aux, cycle_len, i);
        prec[i] = cycle_len + prec[aux];

        aux = i + 1;
        i += 1;
        cycle_len = 0;
        COLLATZ(aux, cycle_len, i);
        prec[i] = cycle_len + prec[aux];

        aux = i + 1;
        i += 1;
        cycle_len = 0;
        COLLATZ(aux, cycle_len, i);
        prec[i] = cycle_len + prec[aux];

        aux = i + 1;
        i += 1;
        cycle_len = 0;
        COLLATZ(aux, cycle_len, i);
        prec[i] = cycle_len + prec[aux];

        aux = i + 1;
        i += 1;
        cycle_len = 0;
        COLLATZ(aux, cycle_len, i);
        prec[i] = cycle_len + prec[aux];

        aux = i + 1;
        i += 1;
        cycle_len = 0;
        COLLATZ(aux, cycle_len, i);
        prec[i] = cycle_len + prec[aux];

        aux = i + 1;
        i += 1;
        cycle_len = 0;
        COLLATZ(aux, cycle_len, i);
        prec[i] = cycle_len + prec[aux];

        aux = i + 1;
        i += 1;
        cycle_len = 0;
        COLLATZ(aux, cycle_len, i);
        prec[i] = cycle_len + prec[aux];

        aux = i + 1;
        i += 1;
        cycle_len = 0;
        COLLATZ(aux, cycle_len, i);
        prec[i] = cycle_len + prec[aux];
    }

    // End of precomputations

    USI cycle_len1, cycle_len2, cycle_len3, cycle_len4, cycle_len5, cycle_len6, cycle_len7, cycle_len8;
    while (a != 0) {
        __m128i maxs = _mm_set1_epi16(0);
        __m128i maxs_new;
        USI max_cycle_len = 0;
        a = (a < (b>>1))? b>>1 : a;
        ULLI copy_a;
        if (b > 7) {
            for (; a < b - 6; a+=8) {
                copy_a = a;
                cycle_len1 = 0;
                COLLATZ(copy_a, cycle_len1, SIZE-1);
                cycle_len1 += prec[copy_a];
                
                copy_a = a + 1;
                cycle_len2 = 0;
                COLLATZ(copy_a, cycle_len2, SIZE-1);
                cycle_len2 += prec[copy_a];

                copy_a = a + 2;
                cycle_len3 = 0;
                COLLATZ(copy_a, cycle_len3, SIZE-1);
                cycle_len3 += prec[copy_a];

                copy_a = a + 3;
                cycle_len4 = 0;
                COLLATZ(copy_a, cycle_len4, SIZE-1);
                cycle_len4 += prec[copy_a];

                copy_a = a + 4;
                cycle_len5 = 0;
                COLLATZ(copy_a, cycle_len5, SIZE-1);
                cycle_len5 += prec[copy_a];

                copy_a = a + 5;
                cycle_len6 = 0;
                COLLATZ(copy_a, cycle_len6, SIZE-1);
                cycle_len6 += prec[copy_a];

                copy_a = a + 6;
                cycle_len7 = 0;
                COLLATZ(copy_a, cycle_len7, SIZE-1);
                cycle_len7 += prec[copy_a];

                copy_a = a + 7;
                cycle_len8 = 0;
                COLLATZ(copy_a, cycle_len8, SIZE-1);
                cycle_len8 += prec[copy_a];

                maxs_new = _mm_set_epi16(cycle_len8, cycle_len7, cycle_len6, cycle_len5, cycle_len4, cycle_len3, cycle_len2, cycle_len1);
                maxs = _mm_max_epi16(maxs, maxs_new);
                
            }
        }

        for (; a <= b; a++) {
            copy_a = a;
            cycle_len1 = 0;
            COLLATZ(copy_a, cycle_len1, SIZE-1);
            cycle_len1 += prec[copy_a];
            if (cycle_len1 > max_cycle_len) {
                max_cycle_len = cycle_len1;
            }
        }
        
        if (a > 8) {
            for (b = 0; b < 8; ++b) {
                cycle_len1 = _mm_extract_epi16(maxs, b);
                max_cycle_len = max_cycle_len < cycle_len1 ? cycle_len1 : max_cycle_len;
            }
        }
        printf("%hu\n", max_cycle_len);
        scanf("%u%u", &a, &b);
    }
    free(prec);
    return 0;
}

