#include "trap.h"

// 所有向量寄存器初始化0

// 加载512位
#define vle_v(vd, rs1) asm volatile(".insn r 0x57, 0x4, 0x01,  x" #vd ", %0,  x0" ::"r"(rs1))
// 存储512位
#define vse_v(vs3, rs1) asm volatile(".insn r 0x57, 0x4, 0x02,  x" #vs3 ", %0,  x0" ::"r"(rs1))

// conv1
// 32个8位元素相乘得到32个16位元素，多余元素忽略
#define vmul8to16_vv(vd, vs1, vs2) asm volatile(".insn r 0x57, 0x0, 0x10, x" #vd ", x" #vs1 ", x" #vs2 "")
#define vmul8to16_vx(vd, rs1, vs2) asm volatile(".insn r 0x57, 0x4, 0x10,  x" #vd ", %0,  x" #vs2 "" ::"r"(rs1))
#define vmul8to16_vi(vd, imm, vs2) asm volatile(".insn r 0x57, 0x3, 0x10,  x" #vd ", x" #imm ",  x" #vs2 "")

// 32个16位元素相加得到32个16位元素，多余元素忽略
#define vadd16_vv(vd, vs1, vs2) asm volatile(".insn r 0x57, 0x0, 0x11, x" #vd ", x" #vs1 ", x" #vs2 "")
#define vadd16_vx(vd, rs1, vs2) asm volatile(".insn r 0x57, 0x4, 0x11,  x" #vd ", %0,  x" #vs2 "" ::"r"(rs1))
#define vadd16_vi(vd, imm, vs2) asm volatile(".insn r 0x57, 0x3, 0x11,  x" #vd ", x" #imm ",  x" #vs2 "")

#define vdiv16_vx(vd, rs1, vs2) asm volatile(".insn r 0x57, 0x4, 0x12,  x" #vd ", %0,  x" #vs2 "" ::"r"(rs1))

#define vmax16_vi(vd, imm, vs2) asm volatile(".insn r 0x57, 0x3, 0x13,  x" #vd ", x" #imm ",  x" #vs2 "")

// pool1
// 24个16位元素池化得到6个16位元素，多余元素忽略，多余结果元素置0
#define vpool16_vv(vd, vs1) asm volatile(".insn r 0x57, 0x0, 0x20, x" #vd ", x" #vs1 ", x0")

// fc1
#define vmul16to32_vv(vd, vs1, vs2) asm volatile(".insn r 0x57, 0x0, 0x30, x" #vd ", x" #vs1 ", x" #vs2 "")
#define vmul16to32_vx(vd, rs1, vs2) asm volatile(".insn r 0x57, 0x4, 0x30,  x" #vd ", %0,  x" #vs2 "" ::"r"(rs1))
#define vmul16to32_vi(vd, imm, vs2) asm volatile(".insn r 0x57, 0x3, 0x30,  x" #vd ", x" #imm ",  x" #vs2 "")

#define vadd32_vv(vd, vs1, vs2) asm volatile(".insn r 0x57, 0x0, 0x31, x" #vd ", x" #vs1 ", x" #vs2 "")
#define vadd32_vx(vd, rs1, vs2) asm volatile(".insn r 0x57, 0x4, 0x31,  x" #vd ", %0,  x" #vs2 "" ::"r"(rs1))
#define vadd32_vi(vd, imm, vs2) asm volatile(".insn r 0x57, 0x3, 0x31,  x" #vd ", x" #imm ",  x" #vs2 "")

#define vdiv32_vv(vd, vs1, vs2) asm volatile(".insn r 0x57, 0x0, 0x32, x" #vd ", x" #vs1 ", x" #vs2 "")
#define vdiv32_vx(vd, rs1, vs2) asm volatile(".insn r 0x57, 0x4, 0x32,  x" #vd ", %0,  x" #vs2 "" ::"r"(rs1))
#define vdiv32_vi(vd, imm, vs2) asm volatile(".insn r 0x57, 0x3, 0x32,  x" #vd ", x" #imm ",  x" #vs2 "")

#define vmax32_vv(vd, vs1, vs2) asm volatile(".insn r 0x57, 0x0, 0x33, x" #vd ", x" #vs1 ", x" #vs2 "")
#define vmax32_vx(vd, rs1, vs2) asm volatile(".insn r 0x57, 0x4, 0x33,  x" #vd ", %0,  x" #vs2 "" ::"r"(rs1))
#define vmax32_vi(vd, imm, vs2) asm volatile(".insn r 0x57, 0x3, 0x33,  x" #vd ", x" #imm ",  x" #vs2 "")

// fc2
#define vmul32_vv(vd, vs1, vs2) asm volatile(".insn r 0x57, 0x0, 0x36, x" #vd ", x" #vs1 ", x" #vs2 "")
#define vmul32_vx(vd, rs1, vs2) asm volatile(".insn r 0x57, 0x4, 0x36,  x" #vd ", %0,  x" #vs2 "" ::"r"(rs1))
#define vmul32_vi(vd, imm, vs2) asm volatile(".insn r 0x57, 0x3, 0x36,  x" #vd ", x" #imm ",  x" #vs2 "")

// softmax2
#define vmin32_vv(vd, vs1, vs2) asm volatile(".insn r 0x57, 0x0, 0x34, x" #vd ", x" #vs1 ", x" #vs2 "")
#define vmin32_vx(vd, rs1, vs2) asm volatile(".insn r 0x57, 0x4, 0x34,  x" #vd ", %0,  x" #vs2 "" ::"r"(rs1))
#define vmin32_vi(vd, imm, vs2) asm volatile(".insn r 0x57, 0x3, 0x34,  x" #vd ", x" #imm ",  x" #vs2 "")

#define vsub32_vv(vd, vs1, vs2) asm volatile(".insn r 0x57, 0x0, 0x35, x" #vd ", x" #vs1 ", x" #vs2 "")
#define vsub32_vx(vd, rs1, vs2) asm volatile(".insn r 0x57, 0x4, 0x35,  x" #vd ", %0,  x" #vs2 "" ::"r"(rs1))
#define vsub32_vi(vd, imm, vs2) asm volatile(".insn r 0x57, 0x3, 0x35,  x" #vd ", x" #imm ",  x" #vs2 "")

// 对vs1第1个元素和vs2的前10个元素求最大，将结果写入vd每个元素中
#define vred10max32_vs(vd, vs1, vs2) asm volatile(".insn r 0x57, 0x0, 0x40, x" #vd ", x" #vs1 ", x" #vs2 "")

// 对vs1第1个元素和vs2的前10个元素求和，将结果写入vd每个元素中
#define vred10sum32_vs(vd, vs1, vs2) asm volatile(".insn r 0x57, 0x0, 0x41, x" #vd ", x" #vs1 ", x" #vs2 "")
