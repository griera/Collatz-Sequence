#include <stdio.h>
#include <stdlib.h>

typedef unsigned long long int ULLI;
typedef unsigned int UI;

#define SIZE 2080769

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
    ULLI a, b;
    scanf("%llu%llu", &a, &b);

    // Precomputation

    UI prec[SIZE];
    prec[1] = 1;
    ULLI i;
    for (i = 2; i < SIZE; ++i) {
        ULLI aux = i;
        UI cycle_len = 0;
        COLLATZ(aux, cycle_len, i);
        prec[i] = cycle_len + prec[aux];
    }

    // End of precomputations

    while (a != 0) {
        UI max_cycle_len = 0;
        a = (a < (b>>1))? b>>1 : a;
        for (; a <= b; a++) {
            ULLI copy_a = a;
            UI cycle_len = 0;
            COLLATZ(copy_a, cycle_len, SIZE-1);
            cycle_len += prec[copy_a];
            if (cycle_len > max_cycle_len) {
                max_cycle_len = cycle_len;
            }
        }
        printf("%u\n", max_cycle_len);
        scanf("%llu%llu", &a, &b);
    }
    return 0;
}

