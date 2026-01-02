#include "inst.c"
#include "trap.h"

void conv1vector2(int8_t input[6][14][14], int8_t weight[4][6][3][3], int16_t scale, int16_t output[4][12][12]) {
    for (int f = 0; f < 4; f++) {
        vle_v(1, &weight[f][0][0][0]);
        for (int h = 0; h < 12; h += 4) {
            vadd16_vv(2, 0, 0);
            vadd16_vv(3, 0, 0);
            vleinputconv1mac_vx(2, h, 1, 0b0000000);
            vleinputconv1mac_vx(2, h, 1, 0b0000001);
            vleinputconv1mac_vx(2, h, 1, 0b0000010);
            vleinputconv1mac_vx(2, h, 1, 0b0000100);
            vleinputconv1mac_vx(2, h, 1, 0b0000101);
            vleinputconv1mac_vx(2, h, 1, 0b0000110);
            vleinputconv1mac_vx(2, h, 1, 0b0001000);
            vleinputconv1mac_vx(2, h, 1, 0b0001001);
            vleinputconv1mac_vx(2, h, 1, 0b0001010);

            vleinputconv1mac_vx(2, h, 1, 0b0010000);
            vleinputconv1mac_vx(2, h, 1, 0b0010001);
            vleinputconv1mac_vx(2, h, 1, 0b0010010);
            vleinputconv1mac_vx(2, h, 1, 0b0010100);
            vleinputconv1mac_vx(2, h, 1, 0b0010101);
            vleinputconv1mac_vx(2, h, 1, 0b0010110);
            vleinputconv1mac_vx(2, h, 1, 0b0011000);
            vleinputconv1mac_vx(2, h, 1, 0b0011001);
            vleinputconv1mac_vx(2, h, 1, 0b0011010);

            vleinputconv1mac_vx(2, h, 1, 0b0100000);
            vleinputconv1mac_vx(2, h, 1, 0b0100001);
            vleinputconv1mac_vx(2, h, 1, 0b0100010);
            vleinputconv1mac_vx(2, h, 1, 0b0100100);
            vleinputconv1mac_vx(2, h, 1, 0b0100101);
            vleinputconv1mac_vx(2, h, 1, 0b0100110);
            vleinputconv1mac_vx(2, h, 1, 0b0101000);
            vleinputconv1mac_vx(2, h, 1, 0b0101001);
            vleinputconv1mac_vx(2, h, 1, 0b0101010);

            vleinputconv1mac_vx(2, h, 1, 0b0110000);
            vleinputconv1mac_vx(2, h, 1, 0b0110001);
            vleinputconv1mac_vx(2, h, 1, 0b0110010);
            vleinputconv1mac_vx(2, h, 1, 0b0110100);
            vleinputconv1mac_vx(2, h, 1, 0b0110101);
            vleinputconv1mac_vx(2, h, 1, 0b0110110);
            vleinputconv1mac_vx(2, h, 1, 0b0111000);
            vleinputconv1mac_vx(2, h, 1, 0b0111001);
            vleinputconv1mac_vx(2, h, 1, 0b0111010);

            vleinputconv1mac_vx(2, h, 1, 0b1000000);
            vleinputconv1mac_vx(2, h, 1, 0b1000001);
            vleinputconv1mac_vx(2, h, 1, 0b1000010);
            vleinputconv1mac_vx(2, h, 1, 0b1000100);
            vleinputconv1mac_vx(2, h, 1, 0b1000101);
            vleinputconv1mac_vx(2, h, 1, 0b1000110);
            vleinputconv1mac_vx(2, h, 1, 0b1001000);
            vleinputconv1mac_vx(2, h, 1, 0b1001001);
            vleinputconv1mac_vx(2, h, 1, 0b1001010);

            vleinputconv1mac_vx(2, h, 1, 0b1010000);
            vleinputconv1mac_vx(2, h, 1, 0b1010001);
            vleinputconv1mac_vx(2, h, 1, 0b1010010);
            vleinputconv1mac_vx(2, h, 1, 0b1010100);
            vleinputconv1mac_vx(2, h, 1, 0b1010101);
            vleinputconv1mac_vx(2, h, 1, 0b1010110);
            vleinputconv1mac_vx(2, h, 1, 0b1011000);
            vleinputconv1mac_vx(2, h, 1, 0b1011001);
            vleinputconv1mac_vx(2, h, 1, 0b1011010);

            vdiv16_vx(2, scale, 2);
            vdiv16_vx(3, scale, 3);
            vmax16_vi(2, 0, 2);
            vmax16_vi(3, 0, 3);
            vseoutputconv11_v(2, &output[f][h][0]);
            vseoutputconv12_v(3, &output[f][h + 2][4]);
        }
    }
}

void fc1vector2(int16_t input[144], int16_t weight[144][60], int32_t scale, int32_t output[60]) {
    for (int i = 0; i < 60; i += 32) {
        vadd32_vv(2, 0, 0);
        vadd32_vv(3, 0, 0);
        vle_v(1, &input[0]);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0000000);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0000000);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0000001);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0000001);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0000010);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0000010);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0000011);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0000011);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0000100);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0000100);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0000101);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0000101);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0000110);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0000110);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0000111);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0000111);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0001000);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0001000);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0001001);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0001001);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0001010);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0001010);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0001011);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0001011);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0001100);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0001100);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0001101);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0001101);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0001110);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0001110);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0001111);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0001111);

        vle_v(1, &input[32]);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0010000);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0010000);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0010001);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0010001);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0010010);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0010010);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0010011);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0010011);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0010100);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0010100);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0010101);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0010101);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0010110);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0010110);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0010111);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0010111);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0011000);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0011000);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0011001);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0011001);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0011010);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0011010);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0011011);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0011011);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0011100);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0011100);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0011101);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0011101);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0011110);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0011110);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0011111);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0011111);

        vle_v(1, &input[64]);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0100000);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0100000);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0100001);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0100001);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0100010);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0100010);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0100011);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0100011);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0100100);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0100100);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0100101);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0100101);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0100110);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0100110);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0100111);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0100111);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0101000);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0101000);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0101001);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0101001);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0101010);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0101010);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0101011);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0101011);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0101100);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0101100);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0101101);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0101101);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0101110);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0101110);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0101111);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0101111);

        vle_v(1, &input[96]);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0110000);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0110000);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0110001);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0110001);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0110010);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0110010);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0110011);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0110011);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0110100);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0110100);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0110101);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0110101);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0110110);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0110110);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0110111);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0110111);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0111000);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0111000);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0111001);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0111001);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0111010);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0111010);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0111011);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0111011);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0111100);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0111100);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0111101);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0111101);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0111110);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0111110);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b0111111);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b0111111);

        vle_v(1, &input[128]);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b1000000);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b1000000);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b1000001);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b1000001);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b1000010);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b1000010);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b1000011);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b1000011);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b1000100);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b1000100);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b1000101);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b1000101);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b1000110);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b1000110);
        vleoutputpool1mac_vx(2, i, 1, 0b011, 0b1000111);
        vleoutputpool1mac_vx(2, i, 1, 0b111, 0b1000111);

        vdiv32_vx(2, scale, 2);
        vdiv32_vx(3, scale, 3);
        vmax32_vi(2, 0, 2);
        vmax32_vi(3, 0, 3);
        vse_v(2, &output[i]);
        vse_v(3, &output[i + 16]);
    }
}

void fc2vector2(int32_t input[60], int32_t weight[60][10], int32_t bias[10], int32_t output[10]) {
    vle_v(2, &bias[0]);
    vle_v(1, &input[0]);
    vleoutputfc1mac_vx(2, 0, 1, 0b0000000);
    vleoutputfc1mac_vx(2, 0, 1, 0b0000001);
    vleoutputfc1mac_vx(2, 0, 1, 0b0000010);
    vleoutputfc1mac_vx(2, 0, 1, 0b0000011);
    vleoutputfc1mac_vx(2, 0, 1, 0b0000100);
    vleoutputfc1mac_vx(2, 0, 1, 0b0000101);
    vleoutputfc1mac_vx(2, 0, 1, 0b0000110);
    vleoutputfc1mac_vx(2, 0, 1, 0b0000111);
    vleoutputfc1mac_vx(2, 0, 1, 0b0001000);
    vleoutputfc1mac_vx(2, 0, 1, 0b0001001);
    vleoutputfc1mac_vx(2, 0, 1, 0b0001010);
    vleoutputfc1mac_vx(2, 0, 1, 0b0001011);
    vleoutputfc1mac_vx(2, 0, 1, 0b0001100);
    vleoutputfc1mac_vx(2, 0, 1, 0b0001101);
    vleoutputfc1mac_vx(2, 0, 1, 0b0001110);
    vleoutputfc1mac_vx(2, 0, 1, 0b0001111);

    vle_v(1, &input[16]);
    vleoutputfc1mac_vx(2, 0, 1, 0b0010000);
    vleoutputfc1mac_vx(2, 0, 1, 0b0010001);
    vleoutputfc1mac_vx(2, 0, 1, 0b0010010);
    vleoutputfc1mac_vx(2, 0, 1, 0b0010011);
    vleoutputfc1mac_vx(2, 0, 1, 0b0010100);
    vleoutputfc1mac_vx(2, 0, 1, 0b0010101);
    vleoutputfc1mac_vx(2, 0, 1, 0b0010110);
    vleoutputfc1mac_vx(2, 0, 1, 0b0010111);
    vleoutputfc1mac_vx(2, 0, 1, 0b0011000);
    vleoutputfc1mac_vx(2, 0, 1, 0b0011001);
    vleoutputfc1mac_vx(2, 0, 1, 0b0011010);
    vleoutputfc1mac_vx(2, 0, 1, 0b0011011);
    vleoutputfc1mac_vx(2, 0, 1, 0b0011100);
    vleoutputfc1mac_vx(2, 0, 1, 0b0011101);
    vleoutputfc1mac_vx(2, 0, 1, 0b0011110);
    vleoutputfc1mac_vx(2, 0, 1, 0b0011111);

    vle_v(1, &input[32]);
    vleoutputfc1mac_vx(2, 0, 1, 0b0100000);
    vleoutputfc1mac_vx(2, 0, 1, 0b0100001);
    vleoutputfc1mac_vx(2, 0, 1, 0b0100010);
    vleoutputfc1mac_vx(2, 0, 1, 0b0100011);
    vleoutputfc1mac_vx(2, 0, 1, 0b0100100);
    vleoutputfc1mac_vx(2, 0, 1, 0b0100101);
    vleoutputfc1mac_vx(2, 0, 1, 0b0100110);
    vleoutputfc1mac_vx(2, 0, 1, 0b0100111);
    vleoutputfc1mac_vx(2, 0, 1, 0b0101000);
    vleoutputfc1mac_vx(2, 0, 1, 0b0101001);
    vleoutputfc1mac_vx(2, 0, 1, 0b0101010);
    vleoutputfc1mac_vx(2, 0, 1, 0b0101011);
    vleoutputfc1mac_vx(2, 0, 1, 0b0101100);
    vleoutputfc1mac_vx(2, 0, 1, 0b0101101);
    vleoutputfc1mac_vx(2, 0, 1, 0b0101110);
    vleoutputfc1mac_vx(2, 0, 1, 0b0101111);

    vle_v(1, &input[48]);
    vleoutputfc1mac_vx(2, 0, 1, 0b0110000);
    vleoutputfc1mac_vx(2, 0, 1, 0b0110001);
    vleoutputfc1mac_vx(2, 0, 1, 0b0110010);
    vleoutputfc1mac_vx(2, 0, 1, 0b0110011);
    vleoutputfc1mac_vx(2, 0, 1, 0b0110100);
    vleoutputfc1mac_vx(2, 0, 1, 0b0110101);
    vleoutputfc1mac_vx(2, 0, 1, 0b0110110);
    vleoutputfc1mac_vx(2, 0, 1, 0b0110111);
    vleoutputfc1mac_vx(2, 0, 1, 0b0111000);
    vleoutputfc1mac_vx(2, 0, 1, 0b0111001);
    vleoutputfc1mac_vx(2, 0, 1, 0b0111010);
    vleoutputfc1mac_vx(2, 0, 1, 0b0111011);

    vse_v(1, &output[0]);
}
