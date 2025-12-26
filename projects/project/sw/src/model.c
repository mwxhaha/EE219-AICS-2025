#include "model.h"
#include "scalar.c"
#include "trap.h"

int main() {
    int8_t (*input)[14][14] = (int8_t (*)[14][14])ADDR_INPUT;

    int8_t (*weightconv1)[6][3][3] = (int8_t (*)[6][3][3])ADDR_WCONV1;
    int16_t scaleconv1 = *((int16_t *)ADDR_SCONV1);
    int16_t (*outputconv1)[12][12] = (int16_t (*)[12][12])ADDR_OUTCONV1;
    conv(input, weightconv1, outputconv1, scaleconv1);

    int16_t (*outputpool1)[6][6] = (int16_t (*)[6][6])ADDR_OUTPOOL1;
    maxpool(outputconv1, outputpool1);

    int16_t (*weightfc1)[144] = (int16_t (*)[144])ADDR_WFC1;
    int32_t scalefc1 = *((int32_t *)ADDR_SFC1);
    int32_t *outputfc1 = (int32_t *)ADDR_OUTFC1;
    fc1(outputpool1, weightfc1, outputfc1, scalefc1);

    int32_t (*weightfc2)[60] = (int32_t (*)[60])ADDR_WFC2;
    int32_t *biasfc2 = (int32_t *)ADDR_BFC2;
    int32_t *outputfc2 = (int32_t *)ADDR_OUTFC2;
    fc2(outputfc1, weightfc2, biasfc2, outputfc2);

    int32_t *softmaxlut = (int32_t *)ADDR_SOFTMAX_LUT;
    int32_t *outputsoftmax = (int32_t *)ADDR_OUTSM2;
    softmax(outputfc2, softmaxlut, outputsoftmax);

    // for (int i = 0; i < 10; i++) {
    //     printf("%d ", outputsoftmax[i]);
    // }

    return 0;
}
