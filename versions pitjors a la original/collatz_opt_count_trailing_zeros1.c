#include <stdio.h>
#include <stdlib.h>

typedef unsigned long long int ULLI;
typedef unsigned int UI;

static const UI MultiplyDeBruijnBitPosition[32] = 
{
  0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8, 
  31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
};

#define COUNT_TRAILING_ZEROS(_a, _p, _r) {\
            UI _x1 = MultiplyDeBruijnBitPosition[((((*(_p)) & -(*(_p))) * 0x077CB531)) >> 27];\
            UI _x2 = MultiplyDeBruijnBitPosition[((((*(_p+1)) & -(*(_p+1))) * 0x077CB531)) >> 27];\
            UI _x = _x1 + ((_x2 + 32) & -(_x1 == 0));\
            _a = _a>>_x;\
            _r += _x;\
        }\

#define COLLATZ(_a, _cycle_len) {                          \
            UI *_p = (UI *)&(_a);                          \
            while (_a > 1) {                               \
                _cycle_len += (_a & 1);                    \
                _a += (((_a<<1) + 1) & -(_a & 1));         \
                COUNT_TRAILING_ZEROS(_a, _p, _cycle_len);  \
            }                                              \
        }                                                  \

int main() {
    ULLI a, b;
    scanf("%llu%llu", &a, &b);
    while (a != 0) {
        ULLI max_cycle_len = 0;
        for (; a <= b; a++) {
            ULLI copy_a = a;
            ULLI cycle_len = 1;
            COLLATZ(copy_a, cycle_len);
            if (cycle_len > max_cycle_len) {
                max_cycle_len = cycle_len;
            }
        }
        printf("%llu\n", max_cycle_len);
        scanf("%llu%llu", &a, &b);
    }
    return 0;
}

