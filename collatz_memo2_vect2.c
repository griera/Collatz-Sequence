#include <stdio.h>
#include <stdlib.h>
#include <smmintrin.h>

typedef unsigned long long int ULLI;
typedef unsigned int UI;
typedef unsigned short int USI;

#define SIZE 100000000

#define COLLATZ(_a, _cycle_len, _n) {            \
            while (_a > 1 & _a >= _n) {          \
                if (!(_a & 1)) {                 \
                    _a = _a>>1;                  \
                    _cycle_len += 1;             \
                }                                \
                else {                           \
                    _a = ((_a<<1) + _a + 1)>>1;  \
                    _cycle_len += 2;             \
                }                                \
            }                                    \
        }

#define UNROLL_PREC() {                       \
            aux = i + 1;                      \
	        i += 1;                           \
	        cycle_len = 0;                    \
	        COLLATZ(aux, cycle_len, i);       \
	        prec[i] = cycle_len + prec[aux];  \
	    }
	
#define UNROLL_MAX(_pos, _inc) {                           \
            copy_a = a + _inc;                             \
	        cycle_len_vec[_pos] = 0;                       \
	        COLLATZ(copy_a, cycle_len_vec[_pos], SIZE-1);  \
	        cycle_len_vec[_pos] += prec[copy_a];           \
	    }

#define UNROLL_MAX_VEC(_a) {                        \
            maxs_new = (__m128i *)&prec[(_a)];      \
            maxs = _mm_max_epu16(maxs, *maxs_new);  \
        }
        
int main() {
    UI a, b;
    scanf("%u%u", &a, &b);

    // Precomputations

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
	    _mm_prefetch((char *)&prec[i], 3);
	    ULLI aux = i;
        cycle_len = 0;
        COLLATZ(aux, cycle_len, i);
        prec[i] = cycle_len + prec[aux];

	    UNROLL_PREC();
	    UNROLL_PREC();
	    UNROLL_PREC();
	    UNROLL_PREC();
	    UNROLL_PREC();
	    UNROLL_PREC();
	    UNROLL_PREC();
	    UNROLL_PREC();
	    UNROLL_PREC();
	    UNROLL_PREC();
	    UNROLL_PREC();
	    UNROLL_PREC();
	    UNROLL_PREC();
	    UNROLL_PREC();
	    UNROLL_PREC();
	
	    UNROLL_PREC();
	    UNROLL_PREC();
	    UNROLL_PREC();
	    UNROLL_PREC();
	    UNROLL_PREC();
	    UNROLL_PREC();
	    UNROLL_PREC();
	    UNROLL_PREC();
	    UNROLL_PREC();
	    UNROLL_PREC();
	    UNROLL_PREC();
	    UNROLL_PREC();
	    UNROLL_PREC();
	    UNROLL_PREC();
	    UNROLL_PREC();
	    UNROLL_PREC();
    }

    // End of precomputations

    USI *cycle_len_vec;
    posix_memalign((void **)&cycle_len_vec, 16, 16);
    while (a != 0) {
        __m128i maxs = _mm_setzero_si128();
        __m128i *maxs_new;
        USI max_cycle_len = 0;
        a = (a < (b>>1))? (b>>1) + 1 : a;
        ULLI copy_a;
        if (b > 255) {
            for (; (a < SIZE-256) & ((((UI)&prec[a]) & 15) != 0) & (a < b - 254); ++a) {
                cycle_len = prec[a];
                max_cycle_len = max_cycle_len < cycle_len ? cycle_len : max_cycle_len;
            }

            for (; (a < SIZE-256) & (a < b - 254); a+=256) {
                UNROLL_MAX_VEC(a);
		        UNROLL_MAX_VEC(a+8);
		        UNROLL_MAX_VEC(a+16);
		        UNROLL_MAX_VEC(a+24);
		        UNROLL_MAX_VEC(a+32);
		        UNROLL_MAX_VEC(a+40);
		        UNROLL_MAX_VEC(a+48);
		        UNROLL_MAX_VEC(a+56);
		        UNROLL_MAX_VEC(a+64);
		        UNROLL_MAX_VEC(a+72);
		        UNROLL_MAX_VEC(a+80);
		        UNROLL_MAX_VEC(a+88);
		        UNROLL_MAX_VEC(a+96);
		        UNROLL_MAX_VEC(a+104);
		        UNROLL_MAX_VEC(a+112);
		        UNROLL_MAX_VEC(a+120);

                UNROLL_MAX_VEC(a+128);
		        UNROLL_MAX_VEC(a+136);
		        UNROLL_MAX_VEC(a+144);
		        UNROLL_MAX_VEC(a+152);
		        UNROLL_MAX_VEC(a+160);
		        UNROLL_MAX_VEC(a+168);
		        UNROLL_MAX_VEC(a+176);
		        UNROLL_MAX_VEC(a+184);
		        UNROLL_MAX_VEC(a+192);
		        UNROLL_MAX_VEC(a+200);
		        UNROLL_MAX_VEC(a+208);
		        UNROLL_MAX_VEC(a+216);
		        UNROLL_MAX_VEC(a+224);
		        UNROLL_MAX_VEC(a+232);
		        UNROLL_MAX_VEC(a+240);
		        UNROLL_MAX_VEC(a+248);
            }

            for (; a < b - 62; a+=64) {
		        UNROLL_MAX(0, 0);
		        UNROLL_MAX(1, 1);
		        UNROLL_MAX(2, 2);
		        UNROLL_MAX(3, 3);
		        UNROLL_MAX(4, 4);
		        UNROLL_MAX(5, 5);
		        UNROLL_MAX(6, 6);
		        UNROLL_MAX(7, 7);

                maxs_new = (__m128i *)&cycle_len_vec[0];
                maxs = _mm_max_epu16(maxs, *maxs_new);
		
		        UNROLL_MAX(0, 8);
		        UNROLL_MAX(1, 9);
		        UNROLL_MAX(2, 10);
		        UNROLL_MAX(3, 11);
		        UNROLL_MAX(4, 12);
		        UNROLL_MAX(5, 13);
		        UNROLL_MAX(6, 14);
		        UNROLL_MAX(7, 15);

                maxs_new = (__m128i *)&cycle_len_vec[0];
                maxs = _mm_max_epu16(maxs, *maxs_new);
		
		        UNROLL_MAX(0, 16);
		        UNROLL_MAX(1, 17);
		        UNROLL_MAX(2, 18);
		        UNROLL_MAX(3, 19);
		        UNROLL_MAX(4, 20);
		        UNROLL_MAX(5, 21);
		        UNROLL_MAX(6, 22);
		        UNROLL_MAX(7, 23);

                maxs_new = (__m128i *)&cycle_len_vec[0];
                maxs = _mm_max_epu16(maxs, *maxs_new);
		
		        UNROLL_MAX(0, 24);
		        UNROLL_MAX(1, 25);
		        UNROLL_MAX(2, 26);
		        UNROLL_MAX(3, 27);
		        UNROLL_MAX(4, 28);
		        UNROLL_MAX(5, 29);
		        UNROLL_MAX(6, 30);
		        UNROLL_MAX(7, 31);

                maxs_new = (__m128i *)&cycle_len_vec[0];
                maxs = _mm_max_epu16(maxs, *maxs_new);
		
				UNROLL_MAX(0, 32);
		        UNROLL_MAX(1, 33);
		        UNROLL_MAX(2, 34);
		        UNROLL_MAX(3, 35);
		        UNROLL_MAX(4, 36);
		        UNROLL_MAX(5, 37);
		        UNROLL_MAX(6, 38);
		        UNROLL_MAX(7, 39);

                maxs_new = (__m128i *)&cycle_len_vec[0];
                maxs = _mm_max_epu16(maxs, *maxs_new);
		
		        UNROLL_MAX(0, 40);
		        UNROLL_MAX(1, 41);
		        UNROLL_MAX(2, 42);
		        UNROLL_MAX(3, 43);
		        UNROLL_MAX(4, 44);
		        UNROLL_MAX(5, 45);
		        UNROLL_MAX(6, 46);
		        UNROLL_MAX(7, 47);

                maxs_new = (__m128i *)&cycle_len_vec[0];
                maxs = _mm_max_epu16(maxs, *maxs_new);
		
		        UNROLL_MAX(0, 48);
		        UNROLL_MAX(1, 49);
		        UNROLL_MAX(2, 50);
		        UNROLL_MAX(3, 51);
		        UNROLL_MAX(4, 52);
		        UNROLL_MAX(5, 53);
		        UNROLL_MAX(6, 54);
		        UNROLL_MAX(7, 55);

                maxs_new = (__m128i *)&cycle_len_vec[0];
                maxs = _mm_max_epu16(maxs, *maxs_new);
		
		        UNROLL_MAX(0, 56);
		        UNROLL_MAX(1, 57);
		        UNROLL_MAX(2, 58);
		        UNROLL_MAX(3, 59);
		        UNROLL_MAX(4, 60);
		        UNROLL_MAX(5, 61);
		        UNROLL_MAX(6, 62);
		        UNROLL_MAX(7, 63);

                maxs_new = (__m128i *)&cycle_len_vec[0];
                maxs = _mm_max_epu16(maxs, *maxs_new);
            }
        }

        for (; a <= b; a++) {
            copy_a = a;
            cycle_len = 0;
            COLLATZ(copy_a, cycle_len, SIZE-1);
            cycle_len += prec[copy_a];
            max_cycle_len = max_cycle_len < cycle_len ? cycle_len : max_cycle_len;
        }
        
        // Horitzontally compute the maximum value of maxs SIMD register
        // Then compute the maximum cycle length
        
        __m128i mask = _mm_cmpeq_epi16(maxs, maxs);
        __m128i neg = _mm_sign_epi16(maxs, mask);
	    __m128i max_neg = _mm_minpos_epu16(neg);
	    cycle_len = -(_mm_extract_epi16(max_neg, 0));
	    max_cycle_len = max_cycle_len < cycle_len ? cycle_len : max_cycle_len;
	
	    printf("%hu\n", max_cycle_len);
        scanf("%u%u", &a, &b);
    }

    free(prec);
    return 0;
}

