#include "trap.h"

void vle_v(register int *vd, register int *rs1) {
    asm volatile(".insn r 0x57, 0x4, 0x01, %0, %1, %2" : : "r"(vd), "r"(rs1));
} // 加载512位

void vse_v(register int *rs1, register int *vs3) {
    asm volatile(".insn r 0x57, 0x4, 0x01, %0, %1, %2" : : "r"(rs1), "r"(vs3));
} // 存储512位

// 所有向量寄存器初始化0

// conv1
void vmul8to16_vv(register int *vd, register int *vs1, register int *vs2) {
    asm volatile(".insn r 0x57, 0x0, 0x01, %0, %1, %2" : : "r"(vd), "r"(vs1), "r"(vs2));
} // 32个8位元素相乘得到32个16位元素，多余元素忽略
void vmul8to16_vx(register int *vd, register int *rs1, register int *vs2) {
    asm volatile(".insn r 0x57, 0x4, 0x01, %0, %1, %2" : : "r"(vd), "r"(rs1), "r"(vs2));
}
void vmul8to16_vi(register int *vd, register int *imm, register int *vs2) {
    asm volatile(".insn r 0x57, 0x3, 0x01, %0, %1, %2" : : "r"(vd), "r"(imm), "r"(vs2));
}

void vadd16_vv(register int *vd, register int *vs1, register int *vs2) {
    asm volatile(".insn r 0x57, 0x0, 0x01, %0, %1, %2" : : "r"(vd), "r"(vs1), "r"(vs2));
} // 32个16位元素相加得到32个16位元素，多余元素忽略
void vadd16_vx(register int *vd, register int *rs1, register int *vs2) {
    asm volatile(".insn r 0x57, 0x4, 0x01, %0, %1, %2" : : "r"(vd), "r"(rs1), "r"(vs2));
}
void vadd16_vi(register int *vd, register int *imm, register int *vs2) {
    asm volatile(".insn r 0x57, 0x3, 0x01, %0, %1, %2" : : "r"(vd), "r"(imm), "r"(vs2));
}

void vdiv16_vx(register int *vd, register int *rs1, register int *vs2) {
    asm volatile(".insn r 0x57, 0x0, 0x01, %0, %1, %2" : : "r"(vd), "r"(rs1), "r"(vs2));
}

void vmax16_vi(register int *vd, register int *imm, register int *vs2) {
    asm volatile(".insn r 0x57, 0x4, 0x01, %0, %1, %2" : : "r"(vd), "r"(imm), "r"(vs2));
}

// pool1
void vpool16_vv(register int *vd, register int *vs1) {
    asm volatile(".insn r 0x57, 0x4, 0x01, %0, %1, %2" : : "r"(vd), "r"(vs1), "r"(0));
} // 24个16位元素池化得到6个16位元素，多余元素忽略，多余结果元素置0

// fc1
void vmul16to32_vv(register int *vd, register int *vs1, register int *vs2) {
    asm volatile(".insn r 0x57, 0x0, 0x01, %0, %1, %2" : : "r"(vd), "r"(vs1), "r"(vs2));
}
void vmul16to32_vx(register int *vd, register int *rs1, register int *vs2) {
    asm volatile(".insn r 0x57, 0x4, 0x01, %0, %1, %2" : : "r"(vd), "r"(rs1), "r"(vs2));
}
void vmul16to32_vi(register int *vd, register int *imm, register int *vs2) {
    asm volatile(".insn r 0x57, 0x3, 0x01, %0, %1, %2" : : "r"(vd), "r"(imm), "r"(vs2));
}

void vadd32_vv(register int *vd, register int *vs1, register int *vs2) {
    asm volatile(".insn r 0x57, 0x0, 0x01, %0, %1, %2" : : "r"(vd), "r"(vs1), "r"(vs2));
}
void vadd32_vx(register int *vd, register int *rs1, register int *vs2) {
    asm volatile(".insn r 0x57, 0x4, 0x01, %0, %1, %2" : : "r"(vd), "r"(rs1), "r"(vs2));
}
void vadd32_vi(register int *vd, register int *imm, register int *vs2) {
    asm volatile(".insn r 0x57, 0x3, 0x01, %0, %1, %2" : : "r"(vd), "r"(imm), "r"(vs2));
}

void vdiv32_vx(register int *vd, register int *rs1, register int *vs2) {
    asm volatile(".insn r 0x57, 0x0, 0x01, %0, %1, %2" : : "r"(vd), "r"(rs1), "r"(vs2));
}

void vmax32_vi(register int *vd, register int *imm, register int *vs2) {
    asm volatile(".insn r 0x57, 0x4, 0x01, %0, %1, %2" : : "r"(vd), "r"(imm), "r"(vs2));
}

// fc2
void vmul32_vv(register int *vd, register int *vs1, register int *vs2) {
    asm volatile(".insn r 0x57, 0x0, 0x01, %0, %1, %2" : : "r"(vd), "r"(vs1), "r"(vs2));
}
void vmul32_vx(register int *vd, register int *rs1, register int *vs2) {
    asm volatile(".insn r 0x57, 0x4, 0x01, %0, %1, %2" : : "r"(vd), "r"(rs1), "r"(vs2));
}
void vmul32_vi(register int *vd, register int *imm, register int *vs2) {
    asm volatile(".insn r 0x57, 0x3, 0x01, %0, %1, %2" : : "r"(vd), "r"(imm), "r"(vs2));
}

// softmax2
void vmax32_vx(register int *vd, register int *rs1, register int *vs2) {
    asm volatile(".insn r 0x57, 0x0, 0x01, %0, %1, %2" : : "r"(vd), "r"(rs1), "r"(vs2));
}

void vmin32_vx(register int *vd, register int *rs1, register int *vs2) {
    asm volatile(".insn r 0x57, 0x0, 0x01, %0, %1, %2" : : "r"(vd), "r"(rs1), "r"(vs2));
}

void vsub32_vv(register int *vd, register int *vs1, register int *vs2) {
    asm volatile(".insn r 0x57, 0x0, 0x01, %0, %1, %2" : : "r"(vd), "r"(vs1), "r"(vs2));
}
void vsub32_vx(register int *vd, register int *rs1, register int *vs2) {
    asm volatile(".insn r 0x57, 0x4, 0x01, %0, %1, %2" : : "r"(vd), "r"(rs1), "r"(vs2));
}
void vsub32_vi(register int *vd, register int *imm, register int *vs2) {
    asm volatile(".insn r 0x57, 0x3, 0x01, %0, %1, %2" : : "r"(vd), "r"(imm), "r"(vs2));
}

void vdiv32_vv(register int *vd, register int *vs1, register int *vs2) {
    asm volatile(".insn r 0x57, 0x0, 0x01, %0, %1, %2" : : "r"(vd), "r"(vs1), "r"(vs2));
}
void vdiv32_vx(register int *vd, register int *rs1, register int *vs2) {
    asm volatile(".insn r 0x57, 0x4, 0x01, %0, %1, %2" : : "r"(vd), "r"(rs1), "r"(vs2));
}
void vdiv32_vi(register int *vd, register int *imm, register int *vs2) {
    asm volatile(".insn r 0x57, 0x3, 0x01, %0, %1, %2" : : "r"(vd), "r"(imm), "r"(vs2));
}

void vred10max32_vs(register int *vd, register int *vs1, register int *vs2) {
    asm volatile(".insn r 0x57, 0x0, 0x01, %0, %1, %2" : : "r"(vd), "r"(vs1), "r"(vs2));
} // 对vs1第1个元素和vs2的前10个元素求最大，将结果写入vd每个元素中

void vred10sum32_vs(register int *vd, register int *vs1, register int *vs2) {
    asm volatile(".insn r 0x57, 0x0, 0x01, %0, %1, %2" : : "r"(vd), "r"(vs1), "r"(vs2));
} // 对vs1第1个元素和vs2的前10个元素求和，将结果写入vd每个元素中
