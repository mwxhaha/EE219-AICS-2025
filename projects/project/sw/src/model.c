#include "model.h"
#include "inst.c"
#include "scalar.c"
#include "trap.h"
#include "vector.c"
#include "vector2.c"

int main() {
    int8_t (*input)[14][14] = (int8_t (*)[14][14])ADDR_INPUT;

    int8_t (*weightconv1)[6][3][3] = (int8_t (*)[6][3][3])ADDR_WCONV1;
    int16_t scaleconv1 = *((int16_t *)ADDR_SCONV1);
    int16_t (*outputconv1)[12][12] = (int16_t (*)[12][12])ADDR_OUTCONV1;
    // conv1(input, weightconv1, scaleconv1, outputconv1);
    // conv1vector(input, weightconv1, scaleconv1, outputconv1);
    conv1vector2(input, weightconv1, scaleconv1, outputconv1);

    // for (int f = 0; f < 4; f++) {
    //     for (int h = 0; h < 12; h++) {
    //         for (int w = 0; w < 12; w++) {
    //             printf("%d ", outputconv1[f][h][w]);
    //         }
    //         printf("\n");
    //     }
    //     printf("\n");
    // }
    // printf("\n");

    int16_t (*outputpool1)[6][6] = (int16_t (*)[6][6])ADDR_OUTPOOL1;
    // pool1(outputconv1, outputpool1);
    pool1vector(outputconv1, outputpool1);

    // for (int c = 0; c < 4; c++) {
    //     for (int h = 0; h < 6; h++) {
    //         for (int w = 0; w < 6; w++) {
    //             printf("%d ", outputpool1[c][h][w]);
    //         }
    //         printf("\n");
    //     }
    //     printf("\n");
    // }
    // printf("\n");

    int16_t (*weightfc1)[60] = (int16_t (*)[60])ADDR_WFC1;
    int32_t scalefc1 = *((int32_t *)ADDR_SFC1);
    int32_t *outputfc1 = (int32_t *)ADDR_OUTFC1;
    // fc1((int16_t *)outputpool1, weightfc1, scalefc1, outputfc1);
    // fc1vector((int16_t *)outputpool1, weightfc1, scalefc1, outputfc1);
    fc1vector2((int16_t *)outputpool1, weightfc1, scalefc1, outputfc1);

    // for (int i = 0; i < 60; i++) {
    //     printf("%d ", outputfc1[i]);
    // }
    // printf("\n");

    int32_t (*weightfc2)[10] = (int32_t (*)[10])ADDR_WFC2;
    int32_t *biasfc2 = (int32_t *)ADDR_BFC2;
    int32_t *outputfc2 = (int32_t *)ADDR_OUTFC2;
    // fc2(outputfc1, weightfc2, biasfc2, outputfc2);
    // fc2vector(outputfc1, weightfc2, biasfc2, outputfc2);
    fc2vector2(outputfc1, weightfc2, biasfc2, outputfc2);

    // for (int i = 0; i < 10; i++) {
    //     printf("%d ", outputfc2[i]);
    // }
    // printf("\n");

    int32_t *softmaxlut = (int32_t *)ADDR_SOFTMAX_LUT;
    int32_t *outputsoftmax2 = (int32_t *)ADDR_OUTSM2;
    // softmax2(outputfc2, softmaxlut, outputsoftmax2);
    softmax2vector(outputfc2, softmaxlut, outputsoftmax2);

    // for (int i = 0; i < 10; i++) {
    //     printf("%d ", outputsoftmax2[i]);
    // }
    // printf("\n");

    return 0;
}
