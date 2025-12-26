#include "trap.h"

void vadd8to16_vv(register int *vd, register int *vs1, register int *vs2) {
    asm volatile(".insn r 0x57, 0x0, 0x01, %0, %1, %2" : : "r"(vd), "r"(vs1), "r"(vs2));
}
void vadd8to16_vx(register int *vd, register int *rs1, register int *vs2) {
    asm volatile(".insn r 0x57, 0x4, 0x01, %0, %1, %2" : : "r"(vd), "r"(rs1), "r"(vs2));
}
void vadd8to16_vi(register int *vd, register int *imm, register int *vs2) {
    asm volatile(".insn r 0x57, 0x3, 0x01, %0, %1, %2" : : "r"(vd), "r"(imm), "r"(vs2));
}

void vmul8to16_vv(register int *vd, register int *vs1, register int *vs2) {
    asm volatile(".insn r 0x57, 0x0, 0x01, %0, %1, %2" : : "r"(vd), "r"(vs1), "r"(vs2));
}
void vmul8to16_vx(register int *vd, register int *rs1, register int *vs2) {
    asm volatile(".insn r 0x57, 0x4, 0x01, %0, %1, %2" : : "r"(vd), "r"(rs1), "r"(vs2));
}
void vmul8to16_vi(register int *vd, register int *imm, register int *vs2) {
    asm volatile(".insn r 0x57, 0x3, 0x01, %0, %1, %2" : : "r"(vd), "r"(imm), "r"(vs2));
}

void vdiv16to16_vx(register int *vd, register int *rs1, register int *vs2) {
    asm volatile(".insn r 0x57, 0x0, 0x01, %0, %1, %2" : : "r"(vd), "r"(rs1), "r"(vs2));
}

void vmax16to16_vi(register int *vd, register int *imm, register int *vs2) {
    asm volatile(".insn r 0x57, 0x4, 0x01, %0, %1, %2" : : "r"(vd), "r"(imm), "r"(vs2));
}

void vmaxpool16to16_vv(register int *vd, register int *vs1) {
    asm volatile(".insn r 0x57, 0x4, 0x01, %0, %1, %2" : : "r"(vd), "r"(vs1), "r"(0));
} // 24个元素池化得到6个元素

void vadd16to32_vv(register int *vd, register int *vs1, register int *vs2) {
    asm volatile(".insn r 0x57, 0x0, 0x01, %0, %1, %2" : : "r"(vd), "r"(vs1), "r"(vs2));
}
void vadd16to32_vx(register int *vd, register int *rs1, register int *vs2) {
    asm volatile(".insn r 0x57, 0x4, 0x01, %0, %1, %2" : : "r"(vd), "r"(rs1), "r"(vs2));
}
void vadd16to32_vi(register int *vd, register int *imm, register int *vs2) {
    asm volatile(".insn r 0x57, 0x3, 0x01, %0, %1, %2" : : "r"(vd), "r"(imm), "r"(vs2));
}

void vmul16to32_vv(register int *vd, register int *vs1, register int *vs2) {
    asm volatile(".insn r 0x57, 0x0, 0x01, %0, %1, %2" : : "r"(vd), "r"(vs1), "r"(vs2));
}
void vmul16to32_vx(register int *vd, register int *rs1, register int *vs2) {
    asm volatile(".insn r 0x57, 0x4, 0x01, %0, %1, %2" : : "r"(vd), "r"(rs1), "r"(vs2));
}
void vmul16to32_vi(register int *vd, register int *imm, register int *vs2) {
    asm volatile(".insn r 0x57, 0x3, 0x01, %0, %1, %2" : : "r"(vd), "r"(imm), "r"(vs2));
}

void vdiv32to32_vx(register int *vd, register int *rs1, register int *vs2) {
    asm volatile(".insn r 0x57, 0x0, 0x01, %0, %1, %2" : : "r"(vd), "r"(rs1), "r"(vs2));
}

void vmax32to32_vi(register int *vd, register int *imm, register int *vs2) {
    asm volatile(".insn r 0x57, 0x4, 0x01, %0, %1, %2" : : "r"(vd), "r"(imm), "r"(vs2));
}

void vadd32to32_vv(register int *vd, register int *vs1, register int *vs2) {
    asm volatile(".insn r 0x57, 0x0, 0x01, %0, %1, %2" : : "r"(vd), "r"(vs1), "r"(vs2));
}
void vadd32to32_vx(register int *vd, register int *rs1, register int *vs2) {
    asm volatile(".insn r 0x57, 0x4, 0x01, %0, %1, %2" : : "r"(vd), "r"(rs1), "r"(vs2));
}
void vadd32to32_vi(register int *vd, register int *imm, register int *vs2) {
    asm volatile(".insn r 0x57, 0x3, 0x01, %0, %1, %2" : : "r"(vd), "r"(imm), "r"(vs2));
}

void vmul32to32_vv(register int *vd, register int *vs1, register int *vs2) {
    asm volatile(".insn r 0x57, 0x0, 0x01, %0, %1, %2" : : "r"(vd), "r"(vs1), "r"(vs2));
}
void vmul32to32_vx(register int *vd, register int *rs1, register int *vs2) {
    asm volatile(".insn r 0x57, 0x4, 0x01, %0, %1, %2" : : "r"(vd), "r"(rs1), "r"(vs2));
}
void vmul32to32_vi(register int *vd, register int *imm, register int *vs2) {
    asm volatile(".insn r 0x57, 0x3, 0x01, %0, %1, %2" : : "r"(vd), "r"(imm), "r"(vs2));
}

void vmax32to32_vx(register int *vd, register int *rs1, register int *vs2) {
    asm volatile(".insn r 0x57, 0x0, 0x01, %0, %1, %2" : : "r"(vd), "r"(rs1), "r"(vs2));
}

void vmin32to32_vx(register int *vd, register int *rs1, register int *vs2) {
    asm volatile(".insn r 0x57, 0x0, 0x01, %0, %1, %2" : : "r"(vd), "r"(rs1), "r"(vs2));
}

void vsub32to32_vv(register int *vd, register int *vs1, register int *vs2) {
    asm volatile(".insn r 0x57, 0x0, 0x01, %0, %1, %2" : : "r"(vd), "r"(vs1), "r"(vs2));
}
void vsub32to32_vx(register int *vd, register int *rs1, register int *vs2) {
    asm volatile(".insn r 0x57, 0x4, 0x01, %0, %1, %2" : : "r"(vd), "r"(rs1), "r"(vs2));
}
void vsub32to32_vi(register int *vd, register int *imm, register int *vs2) {
    asm volatile(".insn r 0x57, 0x3, 0x01, %0, %1, %2" : : "r"(vd), "r"(imm), "r"(vs2));
}

void vdiv32to32_vv(register int *vd, register int *vs1, register int *vs2) {
    asm volatile(".insn r 0x57, 0x0, 0x01, %0, %1, %2" : : "r"(vd), "r"(vs1), "r"(vs2));
}
void vdiv32to32_vx(register int *vd, register int *rs1, register int *vs2) {
    asm volatile(".insn r 0x57, 0x4, 0x01, %0, %1, %2" : : "r"(vd), "r"(rs1), "r"(vs2));
}
void vdiv32to32_vi(register int *vd, register int *imm, register int *vs2) {
    asm volatile(".insn r 0x57, 0x3, 0x01, %0, %1, %2" : : "r"(vd), "r"(imm), "r"(vs2));
}

void vred10max32to32_vs(register int *vd, register int *vs1, register int *vs2) {
    asm volatile(".insn r 0x57, 0x0, 0x01, %0, %1, %2" : : "r"(vd), "r"(vs1), "r"(vs2));
} // 和标准行为不同，只对vs2的前十个元素求最大，将结果写入vd每个元素中

void vred10sum32to32_vs(register int *vd, register int *vs1, register int *vs2) {
    asm volatile(".insn r 0x57, 0x0, 0x01, %0, %1, %2" : : "r"(vd), "r"(vs1), "r"(vs2));
} // 和标准行为不同，只对vs2的前十个元素求和，将结果写入vd每个元素中
