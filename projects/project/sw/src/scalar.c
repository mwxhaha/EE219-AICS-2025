#include "trap.h"

void conv1(int8_t input[6][14][14], int8_t weight[4][6][3][3], int16_t scale, int16_t output[4][12][12]) {
    for (int f = 0; f < 4; f++) {
        for (int h = 0; h < 12; h++) {
            for (int w = 0; w < 12; w++) {
                int16_t sum = 0;
                for (int c = 0; c < 6; c++) {
                    for (int kh = 0; kh < 3; kh++) {
                        for (int kw = 0; kw < 3; kw++) {
                            sum += (int16_t)input[c][h + kh][w + kw] * (int16_t)weight[f][c][kh][kw];
                        }
                    }
                }
                output[f][h][w] = (sum > 0) ? sum / scale : 0;
            }
        }
    }
}

void pool1(int16_t input[4][12][12], int16_t output[4][6][6]) {
    for (int c = 0; c < 4; c++) {
        for (int h = 0; h < 6; h++) {
            for (int w = 0; w < 6; w++) {
                int16_t max_val = input[c][h * 2][w * 2];
                for (int ph = 0; ph < 2; ph++) {
                    for (int pw = 0; pw < 2; pw++) {
                        int16_t val = input[c][h * 2 + ph][w * 2 + pw];
                        if (val > max_val) {
                            max_val = val;
                        }
                    }
                }
                output[c][h][w] = max_val;
            }
        }
    }
}

void fc1(int16_t input[144], int16_t weight[144][60], int32_t scale, int32_t output[60]) {
    for (int i = 0; i < 60; i++) {
        int32_t sum = 0;
        for (int j = 0; j < 144; j++) {
            sum += (int32_t)input[j] * (int32_t)weight[j][i];
        }
        output[i] = (sum > 0) ? sum / scale : 0;
    }
}

void fc2(int32_t input[60], int32_t weight[60][10], int32_t bias[10], int32_t output[10]) {
    for (int i = 0; i < 10; i++) {
        int32_t sum = bias[i];
        for (int j = 0; j < 60; j++) {
            sum += (int32_t)input[j] * (int32_t)weight[j][i];
        }
        output[i] = sum;
    }
}

void softmax2(int32_t input[10], int32_t softmaxlut[256], int32_t output[10]) {
    int32_t Q = 16;
    int32_t Q_16 = 1 << Q;

    int32_t x_q16_16[10];
    for (int i = 0; i < 10; i++) {
        int32_t clipped = input[i];
        if (clipped < -32767)
            clipped = -32767;
        if (clipped > 32767)
            clipped = 32767;
        x_q16_16[i] = clipped * Q_16;
    }

    int32_t x_max = x_q16_16[0];
    for (int i = 1; i < 10; i++) {
        if (x_q16_16[i] > x_max) {
            x_max = x_q16_16[i];
        }
    }
    int32_t exp_delta[10];
    int32_t exp_sum = 0;
    for (int i = 0; i < 10; i++) {
        int32_t delta = x_q16_16[i] - x_max;
        if (delta < -8 * Q_16)
            delta = -8 * Q_16;
        if (delta > 0)
            delta = 0;

        int32_t idx_num = (delta + 8 * Q_16) * 255;
        int32_t idx = idx_num / (8 * Q_16);

        exp_delta[i] = softmaxlut[idx];

        exp_sum += exp_delta[i];
    }

    int32_t exp_sum_shr = exp_sum / 4;
    for (int i = 0; i < 10; i++) {
        int32_t exp_delta_shr = exp_delta[i] / 4;

        int32_t num = exp_delta_shr * Q_16;
        int32_t softmax_q16_16 = num / exp_sum_shr;
        output[i] = softmax_q16_16;
    }
}
