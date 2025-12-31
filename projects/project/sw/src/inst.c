#include "trap.h"

#define INST_OPV_BIN(funct6, vm, vs2, vs1, funct3, vd, opcode) \
  (0b##funct6##vm##vs2##vs1##funct3##vd##opcode)
#define WORD(inst) ".word " #inst "\n"
#define OPV_OPCODE 1010111
#define VM_NOMASK 1
#define F6(x) x
// 所有向量寄存器初始化0

// 加载512位
#define vle_v(vd, rs1) asm volatile(".insn r 0x57, 0x4, 0x01,  x" #vd ", %0,  x0" ::"r"(rs1))
// 存储512位
#define vse_v(vs3, rs1) asm volatile(".insn r 0x57, 0x4, 0x02,  x" #vs3 ", %0,  x0" ::"r"(rs1))

// conv1
// 32个8位元素相乘得到32个16位元素，多余元素忽略
#define vmul8to16_vv(vd, vs1, vs2) \
  asm volatile(WORD(INST_OPV_BIN(F6(010000), VM_NOMASK, \
                                 /*vs2*/ vs2, /*vs1*/ vs1, \
                                 /*funct3*/ 000, /*vd*/ vd, OPV_OPCODE)))
#define vmul8to16_vx(vd, rs1, vs2) \
  asm volatile(".insn r 0x57, 0x4, 0x10,  x" #vd ", %0,  x" #vs2 "" ::"r"(rs1))
#define vmul8to16_vi(vd, imm, vs2) \
  asm volatile(WORD(INST_OPV_BIN(F6(010000), VM_NOMASK, \
                                 /*vs2*/ vs2, /*vs1(imm5)*/ imm, \
                                 /*funct3*/ 011, /*vd*/ vd, OPV_OPCODE)))

// 32个16位元素相加得到32个16位元素，多余元素忽略
#define vadd16_vv(vd, vs1, vs2) \
  asm volatile(WORD(INST_OPV_BIN(F6(010001), VM_NOMASK, \
                                 vs2, vs1, 000, vd, OPV_OPCODE)))
#define vadd16_vx(vd, rs1, vs2) \
  asm volatile(".insn r 0x57, 0x4, 0x11,  x" #vd ", %0,  x" #vs2 "" ::"r"(rs1))
#define vadd16_vi(vd, imm, vs2) \
  asm volatile(WORD(INST_OPV_BIN(F6(010001), VM_NOMASK, \
                                 vs2, imm, 011, vd, OPV_OPCODE)))

#define vdiv16_vx(vd, rs1, vs2) \
  asm volatile(".insn r 0x57, 0x4, 0x12,  x" #vd ", %0,  x" #vs2 "" ::"r"(rs1))

#define vmax16_vi(vd, imm, vs2) \
  asm volatile(WORD(INST_OPV_BIN(F6(010011), VM_NOMASK, \
                                 vs2, imm, 011, vd, OPV_OPCODE)))

// pool1
// 24个16位元素池化得到6个16位元素，多余元素忽略，多余结果元素置0
#define vpool16_vv(vd, vs1) \
  asm volatile(WORD(INST_OPV_BIN(F6(100000), VM_NOMASK, \
                                 /*vs2*/ 00000, /*vs1*/ vs1, \
                                 000, vd, OPV_OPCODE)))

// fc1
#define vmul16to32_vv(vd, vs1, vs2) \
  asm volatile(WORD(INST_OPV_BIN(F6(110000), VM_NOMASK, \
                                 vs2, vs1, 000, vd, OPV_OPCODE)))
#define vmul16to32_vx(vd, rs1, vs2) \
  asm volatile(".insn r 0x57, 0x4, 0x30,  x" #vd ", %0,  x" #vs2 "" ::"r"(rs1))
#define vmul16to32_vi(vd, imm, vs2) \
  asm volatile(WORD(INST_OPV_BIN(F6(110000), VM_NOMASK, \
                                 vs2, imm, 011, vd, OPV_OPCODE)))

#define vadd32_vv(vd, vs1, vs2) \
  asm volatile(WORD(INST_OPV_BIN(F6(110001), VM_NOMASK, \
                                 vs2, vs1, 000, vd, OPV_OPCODE)))
#define vadd32_vx(vd, rs1, vs2) \
  asm volatile(".insn r 0x57, 0x4, 0x31,  x" #vd ", %0,  x" #vs2 "" ::"r"(rs1))
#define vadd32_vi(vd, imm, vs2) \
  asm volatile(WORD(INST_OPV_BIN(F6(110001), VM_NOMASK, \
                                 vs2, imm, 011, vd, OPV_OPCODE)))

#define vdiv32_vv(vd, vs1, vs2) \
  asm volatile(WORD(INST_OPV_BIN(F6(110010), VM_NOMASK, \
                                 vs2, vs1, 000, vd, OPV_OPCODE)))
#define vdiv32_vx(vd, rs1, vs2) \
  asm volatile(".insn r 0x57, 0x4, 0x32,  x" #vd ", %0,  x" #vs2 "" ::"r"(rs1))
#define vdiv32_vi(vd, imm, vs2) \
  asm volatile(WORD(INST_OPV_BIN(F6(110010), VM_NOMASK, \
                                 vs2, imm, 011, vd, OPV_OPCODE)))

#define vmax32_vv(vd, vs1, vs2) \
  asm volatile(WORD(INST_OPV_BIN(F6(110011), VM_NOMASK, \
                                 vs2, vs1, 000, vd, OPV_OPCODE)))
#define vmax32_vx(vd, rs1, vs2) \
  asm volatile(".insn r 0x57, 0x4, 0x33,  x" #vd ", %0,  x" #vs2 "" ::"r"(rs1))
#define vmax32_vi(vd, imm, vs2) \
  asm volatile(WORD(INST_OPV_BIN(F6(110011), VM_NOMASK, \
                                 vs2, imm, 011, vd, OPV_OPCODE)))
                                 
// fc2
#define vmul32_vv(vd, vs1, vs2) \
  asm volatile(WORD(INST_OPV_BIN(F6(110110), VM_NOMASK, \
                                 vs2, vs1, 000, vd, OPV_OPCODE)))
#define vmul32_vx(vd, rs1, vs2) \
  asm volatile(".insn r 0x57, 0x4, 0x36,  x" #vd ", %0,  x" #vs2 "" ::"r"(rs1))
#define vmul32_vi(vd, imm, vs2) \
  asm volatile(WORD(INST_OPV_BIN(F6(110110), VM_NOMASK, \
                                 vs2, imm, 011, vd, OPV_OPCODE)))

// softmax2
#define vmin32_vv(vd, vs1, vs2) \
  asm volatile(WORD(INST_OPV_BIN(F6(110100), VM_NOMASK, \
                                 vs2, vs1, 000, vd, OPV_OPCODE)))
#define vmin32_vx(vd, rs1, vs2) \
  asm volatile(".insn r 0x57, 0x4, 0x34,  x" #vd ", %0,  x" #vs2 "" ::"r"(rs1))
#define vmin32_vi(vd, imm, vs2) \
  asm volatile(WORD(INST_OPV_BIN(F6(110100), VM_NOMASK, \
                                 vs2, imm, 011, vd, OPV_OPCODE)))

#define vsub32_vv(vd, vs1, vs2) \
  asm volatile(WORD(INST_OPV_BIN(F6(110101), VM_NOMASK, \
                                 vs2, vs1, 000, vd, OPV_OPCODE)))
#define vsub32_vx(vd, rs1, vs2) \
  asm volatile(".insn r 0x57, 0x4, 0x35,  x" #vd ", %0,  x" #vs2 "" ::"r"(rs1))
#define vsub32_vi(vd, imm, vs2) \
  asm volatile(WORD(INST_OPV_BIN(F6(110101), VM_NOMASK, \
                                 vs2, imm, 011, vd, OPV_OPCODE)))

// 对vs1第1个元素和vs2的前10个元素求最大，将结果写入vd每个元素中
#define vred10max32_vs(vd, vs1, vs2) \
  asm volatile(WORD(INST_OPV_BIN(F6(000000), VM_NOMASK, \
                                 vs2, vs1, 000, vd, OPV_OPCODE)))

// 对vs1第1个元素和vs2的前10个元素求和，将结果写入vd每个元素中
#define vred10sum32_vs(vd, vs1, vs2) \
  asm volatile(WORD(INST_OPV_BIN(F6(000001), VM_NOMASK, \
                                 vs2, vs1, 000, vd, OPV_OPCODE)))