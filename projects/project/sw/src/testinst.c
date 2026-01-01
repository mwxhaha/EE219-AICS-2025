#include "inst.c"
#include "trap.h"

int16_t a[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
int16_t b[10];

int main() {
    vle_v(1, a);
    vadd16_vi(2, 2, 1);
    vse_v(2, b);
    for (int i = 0; i < 10; i++) {
        printf("%d ", b[i]);
    }
}
