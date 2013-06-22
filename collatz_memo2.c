#include <stdio.h>
#include <stdlib.h>

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
    }

    // End of precomputations

    while (a != 0) {
        USI max_cycle_len = 0;
        a = (a < (b>>1))? b>>1 : a;
        ULLI copy_a;
        for (; a < b; a++) {
            copy_a = a;
            cycle_len = 0;
            COLLATZ(copy_a, cycle_len, SIZE-1);
            cycle_len += prec[copy_a];
            if (cycle_len > max_cycle_len) {
                max_cycle_len = cycle_len;
            }

            a += 1;
            copy_a = a;
            cycle_len = 0;
            COLLATZ(copy_a, cycle_len, SIZE-1);
            cycle_len += prec[copy_a];
            if (cycle_len > max_cycle_len) {
                max_cycle_len = cycle_len;
            }            
        }
        if (a == b) {
            copy_a = a;
            cycle_len = 0;
            COLLATZ(copy_a, cycle_len, SIZE-1);
            cycle_len += prec[copy_a];
            if (cycle_len > max_cycle_len) {
                max_cycle_len = cycle_len;
            }
        }
        printf("%hu\n", max_cycle_len);
        scanf("%u%u", &a, &b);
    }
    free(prec);
    return 0;
}

