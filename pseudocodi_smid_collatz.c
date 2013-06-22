#include <stdio.h>
#include <stdlib.h>
#include <smmintrin.h>


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
    long long a = 1, b = 2;
    while (b <= 50) {
        __m128i cycles = _mm_set_epi64x(1, 1);
        __m128i x = _mm_set_epi64x(b, a);
        __m128i ones = cycles;

        __m128i aux = _mm_set_epi64x(-2, -2);
        __m128i aux2 = _mm_set_epi64x(-1, -1);
        
        int xi[2], xj[2];
        xi[0] = _mm_extract_epi32(x, 0);
        xi[1] = _mm_extract_epi32(x, 1);

        xj[0] = _mm_extract_epi32(x, 2);
        xj[1] = _mm_extract_epi32(x, 3);

        long long *i, *j;
        i = (long long *)&xi;
        j = (long long *)&xj;

        while (*j > 1 & *i > 1) {
            xi[0] = _mm_extract_epi32(x, 0);
            xi[1] = _mm_extract_epi32(x, 1);

            xj[0] = _mm_extract_epi32(x, 2);
            xj[1] = _mm_extract_epi32(x, 3);

            i = (long long *)&xi;
            j = (long long *)&xj;

            __m128i mask = _mm_cmpeq_epi64(x, ones);
            mask = _mm_xor_si128(mask, aux2);
	        cycles = _mm_sub_epi64(cycles, mask);

            __m128i odd = _mm_slli_epi64(x, 1);
            odd = _mm_add_epi64(odd, x);
	        odd = _mm_add_epi64(odd, ones);

	        __m128i even = _mm_srli_epi64(x, 1);

	        mask = _mm_and_si128(x, ones);
	        mask = _mm_cmpeq_epi64(mask, ones);

	        x = _mm_blendv_epi8(even, odd, mask);

            xi[0] = _mm_extract_epi32(x, 0);
            xi[1] = _mm_extract_epi32(x, 1);

            xj[0] = _mm_extract_epi32(x, 2);
            xj[1] = _mm_extract_epi32(x, 3);

            i = (long long *)&xi;
            j = (long long *)&xj;
        }

        int cyclei = _mm_extract_epi32(cycles, 0);
        int cyclej = _mm_extract_epi32(cycles, 2);

        if (*i > 1) {
            COLLATZ(*i, cyclei, 1);
        }

        else if (*j > 1) {
            COLLATZ(*j, cyclej, 1);
        }

        printf("%lli ==> %i\n%lli ==> %i\n", a, cyclei, b, cyclej);
        a += 2;
        b += 2;
    }
}

