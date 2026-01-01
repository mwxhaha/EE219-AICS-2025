#include "inst.c"
#include "trap.h"

int8_t a[10] = {1 ,-1, 6 ,-1 ,-2, 0,-2 ,6,2, -4};
int16_t b[10];

int main() {
    vle_v(2, a);
    // int8_t c = -1;
    vmul8to16_vx(2, a[1], 2);
    vse_v(2, b);
    for (int i = 0; i < 10; i++) {
        printf("%d ", b[i]);
    }
}
