#include <stdio.h>
#include <stdlib.h>
#include <smmintrin.h>

typedef unsigned long long int ULLI;
typedef unsigned int UI;
typedef unsigned short int USI;

#define SIZE 100000000


#define COLLATZ_SIMD(_k1, _k2) {\
    __m128i cycles = _mm_set_epi64x(1, 1);\
    __m128i x = _mm_set_epi64x(_k2, _k1);\
    __m128i ones = cycles;\
\
    xi[0] = _mm_extract_epi32(x, 0);\
    xi[1] = _mm_extract_epi32(x, 1);\
\
    xj[0] = _mm_extract_epi32(x, 2);\
    xj[1] = _mm_extract_epi32(x, 3);\
\
    long long *i, *j;\
    i = (long long *)&xi;\
    j = (long long *)&xj;\
\
    while (*j > 1 & *i > 1) {\
        xi[0] = _mm_extract_epi32(x, 0);\
        xi[1] = _mm_extract_epi32(x, 1);\
\
        xj[0] = _mm_extract_epi32(x, 2);\
        xj[1] = _mm_extract_epi32(x, 3);\
\
        i = (long long *)&xi;\
        j = (long long *)&xj;\
\
        __m128i mask = _mm_cmpeq_epi64(x, ones);\
        mask = _mm_xor_si128(mask, aux2);\
	    cycles = _mm_sub_epi64(cycles, mask);\
\
        __m128i odd = _mm_slli_epi64(x, 1);\
        odd = _mm_add_epi64(odd, x);\
	    odd = _mm_add_epi64(odd, ones);\
\
	    __m128i even = _mm_srli_epi64(x, 1);\
\
	    mask = _mm_and_si128(x, ones);\
	    mask = _mm_cmpeq_epi64(mask, ones);\
\
	    x = _mm_blendv_epi8(even, odd, mask);\
\
        xi[0] = _mm_extract_epi32(x, 0);\
        xi[1] = _mm_extract_epi32(x, 1);\
\
        xj[0] = _mm_extract_epi32(x, 2);\
        xj[1] = _mm_extract_epi32(x, 3);\
\
        i = (long long *)&xi;\
        j = (long long *)&xj;\
    }\
\
    int cyclei = _mm_extract_epi32(cycles, 0);\
    int cyclej = _mm_extract_epi32(cycles, 2);\
\
    if (*i > 1) {\
        COLLATZ(*i, cyclei, 1);\
    }\
\
    else if (*j > 1) {\
        COLLATZ(*j, cyclej, 1);\
    }\
}\







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

#define UNROLL_PREC(_aux, _cycle_len, _i, _prec) {  \
            _aux = _i + 1;                          \
			_i += 1;                                \
			_cycle_len = 0;                         \
			COLLATZ(aux, cycle_len, _i);            \
			_prec[_i] = _cycle_len + _prec[_aux];   \
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

    __m128i aux = _mm_set_epi64x(-2, -2);
    __m128i aux2 = _mm_set_epi64x(-1, -1);
    
    int xi[2], xj[2];

    for (i = 9; i < SIZE; ++i) {
		_mm_prefetch((char *)&prec[i], 3);
		ULLI aux = i;
        cycle_len = 0;
        COLLATZ(aux, cycle_len, i);
        prec[i] = cycle_len + prec[aux];

		UNROLL_PREC(aux, cycle_len, i, prec);
		UNROLL_PREC(aux, cycle_len, i, prec);
		UNROLL_PREC(aux, cycle_len, i, prec);
		UNROLL_PREC(aux, cycle_len, i, prec);
		UNROLL_PREC(aux, cycle_len, i, prec);
		UNROLL_PREC(aux, cycle_len, i, prec);
		UNROLL_PREC(aux, cycle_len, i, prec);
		UNROLL_PREC(aux, cycle_len, i, prec);
		UNROLL_PREC(aux, cycle_len, i, prec);
		UNROLL_PREC(aux, cycle_len, i, prec);
		UNROLL_PREC(aux, cycle_len, i, prec);
		UNROLL_PREC(aux, cycle_len, i, prec);
		UNROLL_PREC(aux, cycle_len, i, prec);
		UNROLL_PREC(aux, cycle_len, i, prec);
		UNROLL_PREC(aux, cycle_len, i, prec);
        
    }

    // End of precomputations

    USI cycle_len1, cycle_len2, cycle_len3, cycle_len4, cycle_len5, cycle_len6, cycle_len7, cycle_len8;
    while (a != 0) {
        __m128i maxs = _mm_setzero_si128();
        __m128i maxs_new;
        USI max_cycle_len = 0;
        a = (a < (b>>1))? b>>1 : a;
        ULLI copy_a;
        if (b > 7) {
            for (; (a < SIZE-8) & ((((UI)&prec[a]) & 15) != 0) & (a < b - 6); ++a) {
                cycle_len1 = prec[a];
                max_cycle_len = max_cycle_len < cycle_len1 ? cycle_len1 : max_cycle_len;
            }

            for (; (a < SIZE-8) & (a < b - 6); a+=8) {
                maxs_new = _mm_load_si128((__m128i *)&prec[a]);
                maxs = _mm_max_epi16(maxs, maxs_new);
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

                maxs_new = _mm_set_epi16(cycle_len8, cycle_len7, cycle_len6, cycle_len5, cycle_len4, cycle_len3, cycle_len2, cycle_len1);
                maxs = _mm_max_epi16(maxs, maxs_new);
                
            }
        }

        for (; a <= b; a++) {
            copy_a = a;
            cycle_len1 = 0;
            COLLATZ(copy_a, cycle_len1, SIZE-1);
            cycle_len1 += prec[copy_a];
            max_cycle_len = max_cycle_len < cycle_len1 ? cycle_len1 : max_cycle_len;
        }
        
        for (b = 0; b < 8; ++b) {
            cycle_len1 = _mm_extract_epi16(maxs, b);
            max_cycle_len = max_cycle_len < cycle_len1 ? cycle_len1 : max_cycle_len;
        }

        printf("%hu\n", max_cycle_len);
        scanf("%u%u", &a, &b);
    }
    free(prec);
    return 0;
}

