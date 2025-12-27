#include "inst.c"
#include "trap.h"

void conv1vector(int8_t input[6][14][14], int8_t weight[4][6][3][3], int16_t scale, int16_t output[4][12][12]) {
    for (int f = 0; f < 4; f++) {
        for (int h = 0; h < 12; h++) {
            vadd16_vv(1, 0, 0);
            for (int c = 0; c < 6; c++) {
                for (int kh = 0; kh < 3; kh++) {
                    for (int kw = 0; kw < 3; kw++) {
                        vle_v(2, &input[c][h + kh][kw]);
                        vmul8to16_vx(2, weight[f][c][kh][kw], 2);
                        vadd16_vv(1, 1, 2);
                    }
                }
            }
            vdiv16_vx(1, scale, 1);
            vmax16_vi(1, 0, 1);
            vse_v(1, &output[f][h][0]);
        }
    }
}

void pool1vector(int16_t input[4][12][12], int16_t output[4][6][6]) {
    for (int c = 0; c < 4; c++) {
        for (int h = 0; h < 6; h++) {
            vle_v(1, &input[c][h * 2][0]);
            vpool16_vv(1, 1);
            vse_v(1, &output[c][h][0]);
        }
    }
}

void fc1vector(int16_t input[144], int16_t weight[144][60], int32_t scale, int32_t output[60]) {
    for (int i = 0; i < 60; i += 16) {
        vadd32_vv(1, 0, 0);
        for (int j = 0; j < 144; j++) {
            vle_v(2, &weight[j][i]);
            vmul16to32_vx(2, input[j], 2);
            vadd32_vv(1, 1, 2);
        }
        vdiv32_vx(1, scale, 1);
        vmax32_vi(1, 0, 1);
        vse_v(1, &output[i]);
    }
}

void fc2vector(int32_t input[60], int32_t weight[60][10], int32_t bias[10], int32_t output[10]) {
    vle_v(1, &bias[0]);
    for (int j = 0; j < 60; j++) {
        vle_v(2, &weight[j][0]);
        vmul32_vx(2, input[j], 2);
        vadd32_vv(1, 1, 2);
    }
    vse_v(1, &output[0]);
}

void softmax2vector(int32_t input[10], int32_t softmaxlut[256], int32_t output[10]) {
    int32_t Q = 16;
    int32_t Q_16 = 1 << Q;

    vle_v(1, &input[0]);
    vmax32_vx(1, -32767, 1);
    vmin32_vx(1, 32767, 1);
    vmul32_vx(1, Q_16, 1);

    vred10max32_vs(2, 0, 1);
    vsub32_vv(1, 2, 1);
    vmax32_vx(1, -8 * Q_16, 1);
    vmin32_vx(1, 0, 1);

    vadd32_vx(1, 8 * Q_16, 1);
    vmul32_vx(1, 255, 1);
    vdiv32_vx(1, 8 * Q_16, 1);

    vse_v(1, &output[0]);
    for (int i = 0; i < 10; i++) {
        output[i] = softmaxlut[output[i]];
    }
    vle_v(1, &output[0]);

    vred10sum32_vs(2, 0, 1);

    vdiv32_vi(2, 4, 2);
    vdiv32_vi(1, 4, 1);

    vmul32_vx(1, Q_16, 1);
    vdiv32_vv(1, 2, 1);
    vse_v(1, &output[0]);
}
