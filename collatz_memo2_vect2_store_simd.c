#include <stdio.h>
#include <stdlib.h>
#include <smmintrin.h>

typedef unsigned long long int ULLI;
typedef unsigned int UI;
typedef unsigned short int USI;

#define SIZE 100000000

#define COLLATZ(_a, _cycle_len, _n) {               \
            while (_a > 1 & _a >= (_n)) {           \
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
    UI i;
    USI cycle_len1, cycle_len2, cycle_len3, cycle_len4, cycle_len5, cycle_len6, cycle_len7, cycle_len8;
    for (i = 4; i < SIZE; i+=8) {
        ULLI aux = i;
        cycle_len1 = 0;
        COLLATZ(aux, cycle_len1, i);
        cycle_len1 = cycle_len1 + prec[aux];

        aux = i + 2;
        cycle_len2 = 0;
        COLLATZ(aux, cycle_len2, i);
        cycle_len2 = cycle_len2 + prec[aux];

        aux = i + 3;
        cycle_len3 = 0;
        COLLATZ(aux, cycle_len3, i);
        cycle_len3 = cycle_len3 + prec[aux];

        aux = i + 4;
        cycle_len4 = 0;
        COLLATZ(aux, cycle_len4, i);
        cycle_len4 = cycle_len4 + prec[aux];

        aux = i + 5;
        cycle_len5 = 0;
        COLLATZ(aux, cycle_len5, i);
        cycle_len5 = cycle_len5 + prec[aux];

        aux = i + 6;
        cycle_len6 = 0;
        COLLATZ(aux, cycle_len6, i);
        cycle_len6 = cycle_len6 + prec[aux];

        aux = i + 7;
        cycle_len7 = 0;
        COLLATZ(aux, cycle_len7, i);
        cycle_len7 = cycle_len7 + prec[aux];

        aux = i + 8;
        cycle_len8 = 0;
        COLLATZ(aux, cycle_len8, i);
        cycle_len8 = cycle_len8 + prec[aux];

        __m128i values = _mm_set_epi16(cycle_len8, cycle_len7, cycle_len6, cycle_len5, cycle_len4, cycle_len3, cycle_len2, cycle_len1);
        _mm_store_si128((__m128i *)&prec[i], values);

        i += 8;

        aux = i + 1;
        cycle_len1 = 0;
        COLLATZ(aux, cycle_len1, i);
        cycle_len1 = cycle_len1 + prec[aux];

        aux = i + 2;
        cycle_len2 = 0;
        COLLATZ(aux, cycle_len2, i);
        cycle_len2 = cycle_len2 + prec[aux];

        aux = i + 3;
        cycle_len3 = 0;
        COLLATZ(aux, cycle_len3, i);
        cycle_len3 = cycle_len3 + prec[aux];

        aux = i + 4;
        cycle_len4 = 0;
        COLLATZ(aux, cycle_len4, i);
        cycle_len4 = cycle_len4 + prec[aux];

        aux = i + 5;
        cycle_len5 = 0;
        COLLATZ(aux, cycle_len5, i);
        cycle_len5 = cycle_len5 + prec[aux];

        aux = i + 6;
        cycle_len6 = 0;
        COLLATZ(aux, cycle_len6, i);
        cycle_len6 = cycle_len6 + prec[aux];

        aux = i + 7;
        cycle_len7 = 0;
        COLLATZ(aux, cycle_len7, i);
        cycle_len7 = cycle_len7 + prec[aux];

        aux = i + 8;
        cycle_len8 = 0;
        COLLATZ(aux, cycle_len8, i);
        cycle_len8 = cycle_len8 + prec[aux];

        values = _mm_set_epi16(cycle_len8, cycle_len7, cycle_len6, cycle_len5, cycle_len4, cycle_len3, cycle_len2, cycle_len1);
        _mm_store_si128((__m128i *)&prec[i], values);
    }

    // End of precomputations

    while (a != 0) {
        __m128i maxs = _mm_setzero_si128();
        __m128i maxs_new1, maxs_new2, maxs_new3, maxs_new4;
        USI max_cycle_len = 0;
        a = (a < (b>>1))? b>>1 : a;
        ULLI copy_a;
    
        if (b - a > 7) {
            for (; (b < SIZE) & ((UI)&prec[a] & 15); ++a) {
                cycle_len1 = prec[a];
                max_cycle_len = max_cycle_len < cycle_len1 ? cycle_len1 : max_cycle_len;
            }
            for (; (UI)&prec[a] & 15; ++a) {
                copy_a = a;
                cycle_len1 = 0;
                COLLATZ(copy_a, cycle_len1, SIZE-1);
                cycle_len1 += prec[copy_a];
                max_cycle_len = max_cycle_len < cycle_len1 ? cycle_len1 : max_cycle_len;
            }
        }
        if (b > 63) {
            for (; (a < b - 62) & (a < SIZE-64); a+=64) {
                maxs_new1 = _mm_stream_load_si128((__m128i *)&prec[a]);
                maxs = _mm_max_epi16(maxs, maxs_new1);

                maxs_new2 = _mm_stream_load_si128((__m128i *)&prec[a+8]);
                maxs = _mm_max_epi16(maxs, maxs_new2);

                maxs_new3 = _mm_stream_load_si128((__m128i *)&prec[a+16]);
                maxs = _mm_max_epi16(maxs, maxs_new3);

                maxs_new4 = _mm_stream_load_si128((__m128i *)&prec[a+24]);
                maxs = _mm_max_epi16(maxs, maxs_new4);

                maxs_new1 = _mm_stream_load_si128((__m128i *)&prec[a+32]);
                maxs = _mm_max_epi16(maxs, maxs_new1);

                maxs_new2 = _mm_stream_load_si128((__m128i *)&prec[a+40]);
                maxs = _mm_max_epi16(maxs, maxs_new2);

                maxs_new3 = _mm_stream_load_si128((__m128i *)&prec[a+48]);
                maxs = _mm_max_epi16(maxs, maxs_new3);

                maxs_new4 = _mm_stream_load_si128((__m128i *)&prec[a+56]);
                maxs = _mm_max_epi16(maxs, maxs_new4);
            }

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

                maxs_new1 = _mm_set_epi16(cycle_len8, cycle_len7, cycle_len6, cycle_len5, cycle_len4, cycle_len3, cycle_len2, cycle_len1);
                maxs = _mm_max_epi16(maxs, maxs_new1);                
            }
        }
        for (; a <= b; a++) {
            copy_a = a;
            cycle_len1 = 0;
            COLLATZ(copy_a, cycle_len1, SIZE-1);
            cycle_len1 += prec[copy_a];
            max_cycle_len = max_cycle_len < cycle_len1 ? cycle_len1 : max_cycle_len;
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

/*

for (; (a & 15) & (a < b - 6) & (a < SIZE); ++a) {
                cycle_len1 = prec[a];
                max_cycle_len = max_cycle_len < cycle_len1 ? cycle_len1 : max_cycle_len;
            }

            for (; (a < b - 6) & (a < SIZE); a+=8) {
                maxs_new = _mm_stream_load_si128((__m128i *)&prec[a]);
                maxs = _mm_max_epi16(maxs, maxs_new);
            }

*/

