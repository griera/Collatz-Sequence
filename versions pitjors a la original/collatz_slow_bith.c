#include <stdio.h>
#include <stdlib.h>

typedef unsigned long long int ULLI;

#define COLLATZ(_a, _cycle_len) {                                         \
            while (_a > 1) {                                              \
                ++_cycle_len;                                             \
                char mask = (_a & 1) == 0;                                \
                _a = (((_a<<1) + _a + 1) & (mask-1)) | ((_a>>1) & -mask); \
            }                                                             \
        }                                                                 \

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
