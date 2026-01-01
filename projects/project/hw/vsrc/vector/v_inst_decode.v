// =======================================
// You need to finish this module
// =======================================

`include "define_rv32v.v"

module v_id #(
  parameter VLMAX     = 8,
  parameter VALUOP_DW = 5,
  parameter VMEM_DW   = 256,
  parameter VMEM_AW   = 32,
  parameter VREG_DW   = 256,
  parameter VREG_AW   = 5,
  parameter INST_DW   = 32,
  parameter REG_DW    = 32,
  parameter REG_AW    = 5
) (
  input                   clk,
  input                   rst,

  input   [INST_DW-1:0]   inst_i,

  output                  rs1_en_o,
  output  [REG_AW-1:0]    rs1_addr_o,
  input   [REG_DW-1:0]    rs1_dout_i,

  output                  rd_w_en_o,
  output  [REG_AW-1:0]    rd_addr_o,

  output                  vs1_en_o,
  output  [VREG_AW-1:0]   vs1_addr_o,
  input   [VREG_DW-1:0]   vs1_dout_i,

  output                  vs2_en_o,
  output  [VREG_AW-1:0]   vs2_addr_o,
  input   [VREG_DW-1:0]   vs2_dout_i,

  output  [VALUOP_DW-1:0] valu_opcode_o,
  output  [VREG_DW-1:0]   operand_v1_o,
  output  [VREG_DW-1:0]   operand_v2_o,

  output                  vmem_ren_o,
  output                  vmem_wen_o,
  output  [VMEM_AW-1:0]   vmem_addr_o,
  output  [VMEM_DW-1:0]   vmem_din_o,

  output                  vid_wb_en_o,
  output                  vid_wb_sel_o,
  output  [VREG_AW-1:0]   vid_wb_addr_o,

  output                  vid_wb_from_rs1
);

  localparam VALU_OP_NOP        = 5'd0  ;
  localparam VALU_OP_VADD       = 5'd1  ;
  localparam VALU_OP_VMUL       = 5'd2  ;
  localparam VALU_OP_VSUB       = 5'd3  ;
  localparam VALU_OP_VMIN       = 5'd4  ;
  localparam VALU_OP_VMAX       = 5'd5  ;
  localparam VALU_OP_VSRA       = 5'd6  ;
  localparam VALU_OP_VREDSUM_VS = 5'd7  ;
  localparam VALU_OP_VREDMAX_VS = 5'd8  ;
  localparam VALU_OP_VMV_V_X    = 5'd9  ;
  localparam VALU_OP_VMV_X_S    = 5'd10 ;
  localparam VALU_OP_VDIV       = 5'd11 ; 

  wire [6:0]  opcode   = inst_i[6:0];
  wire [4:0]  vd       = inst_i[11:7];
  wire [2:0]  funct3   = inst_i[14:12];
  wire [4:0]  vs1      = inst_i[19:15];
  wire [4:0]  vs2      = inst_i[24:20];
  wire [5:0]  funct6   = inst_i[31:26];
  wire [4:0]  rs1      = vs1;       
  wire [4:0]  vs3      = vd;     
  wire [4:0]  rd       = vd;     
  wire [31:0] imm = {{27{inst_i[19]}}, inst_i[19:15]};
  wire [31:0] uimm = {27'b0, inst_i[19:15]};
  
  reg                  rs1_en;
  reg                  rd_w_en;
  reg                  vs1_en;
  reg                  vs2_en;
  reg [VALUOP_DW-1:0]  valu_opcode;
  reg                  vmem_ren;
  reg                  vmem_wen;
  reg                  vid_wb_en;
  reg                  vid_wb_sel;
  reg                  wb_from_rs1;
  reg [VMEM_AW-1:0]    vmem_addr;
  reg [VMEM_DW-1:0]    vmem_din;
  reg [VREG_DW-1:0]    operand_v1;
  reg [VREG_DW-1:0]    operand_v2;
  assign rs1_en_o = rs1_en;
  assign rs1_addr_o = rs1;
  assign rd_w_en_o = rd_w_en;
  assign rd_addr_o = rd;
  assign vs1_en_o = vs1_en;
  assign vs1_addr_o = vs1;
  assign vs2_en_o = vs2_en;
  assign vs2_addr_o = opcode == 7'b0100111 ? vs3 : vs2;
  assign valu_opcode_o = valu_opcode;
  assign operand_v1_o = operand_v1;
  assign operand_v2_o = operand_v2;
  assign vmem_ren_o = vmem_ren;
  assign vmem_wen_o = vmem_wen;
  assign vmem_addr_o = vmem_addr;
  assign vmem_din_o = vmem_din;
  assign vid_wb_en_o = vid_wb_en;
  assign vid_wb_sel_o = vid_wb_sel;
  assign vid_wb_addr_o = vd;
  assign vid_wb_from_rs1 = wb_from_rs1;
  
  always @(*) begin
    rs1_en = 1'b0;
    rd_w_en = 1'b0;
    vs1_en = 1'b0;
    vs2_en = 1'b0;
    valu_opcode = VALU_OP_NOP;
    vmem_ren = 1'b0;
    vmem_wen = 1'b0;
    vid_wb_en = 1'b0;
    vid_wb_sel = 1'b0;
    wb_from_rs1 = 1'b0;
    vmem_addr = 32'b0;
    vmem_din = 256'b0;
    operand_v1 = 256'b0;
    operand_v2 = 256'b0;
    
    case (opcode)
      7'b0000111: begin  
          rs1_en = 1'b1;
          vmem_ren = 1'b1;
          vmem_addr = rs1_dout_i; 
          vid_wb_en = 1'b1;
          vid_wb_sel = 1'b1;  
      end
      7'b0100111: begin 
          rs1_en = 1'b1;
          vs2_en = 1'b1;
          vmem_wen = 1'b1;
          vmem_addr = rs1_dout_i; 
          vmem_din = vs2_dout_i;  
      end
      7'b1010111: begin
        case (funct6)
          6'b000000: begin 
            vs2_en = 1'b1;
            operand_v2 = vs2_dout_i;
            valu_opcode = VALU_OP_VADD;
            vid_wb_en = 1'b1;
            case (funct3)
              3'b000: begin
                vs1_en = 1'b1;
                operand_v1 = vs1_dout_i;
              end
              3'b100: begin
                rs1_en = 1'b1;
                operand_v1 = {VLMAX{rs1_dout_i}};
              end
              3'b011: begin
                operand_v1 = {VLMAX{imm}};
              end
              3'b010: begin
                vs1_en = 1'b1;
                operand_v1 = vs1_dout_i;
                valu_opcode = VALU_OP_VREDSUM_VS;
              end
              default: ;
            endcase
          end
          
          6'b000010: begin
            vs2_en = 1'b1;
            operand_v2 = vs2_dout_i;
            valu_opcode = VALU_OP_VSUB;
            vid_wb_en = 1'b1;
            case (funct3)
              3'b000: begin
                vs1_en = 1'b1;
                operand_v1 = vs1_dout_i;
              end
              3'b100: begin
                rs1_en = 1'b1;
                operand_v1 = {VLMAX{rs1_dout_i}};
              end
              default: ;
            endcase
          end
          
          6'b100101: begin
            vs2_en = 1'b1;
            operand_v2 = vs2_dout_i;
            valu_opcode = VALU_OP_VMUL;
            vid_wb_en = 1'b1;
            case (funct3)
              3'b010: begin
                vs1_en = 1'b1;
                operand_v1 = vs1_dout_i;
              end
              3'b110: begin
                rs1_en = 1'b1;
                operand_v1 = {VLMAX{rs1_dout_i}};
              end
              default: ;
            endcase
          end
          
          6'b100001: begin
            vs2_en = 1'b1;
            operand_v2 = vs2_dout_i;
            valu_opcode = VALU_OP_VDIV;
            vid_wb_en = 1'b1;
            case (funct3)
              3'b010: begin
                vs1_en = 1'b1;
                operand_v1 = vs1_dout_i;
              end
              3'b110: begin
                rs1_en = 1'b1;
                operand_v1 = {VLMAX{rs1_dout_i}};
              end
              default: ;
            endcase
          end

          6'b010000: begin
            vs2_en = 1'b1;
            rd_w_en = 1'b1;
          end
          6'b010111: begin
            rs1_en = 1'b1;
            vid_wb_en = 1'b1;
            wb_from_rs1 = 1'b1;
          end
          
          6'b000101: begin
            vs2_en = 1'b1;
            operand_v2 = vs2_dout_i;
            valu_opcode = VALU_OP_VMIN;
            vid_wb_en = 1'b1;
            case (funct3)
              3'b000: begin
                vs1_en = 1'b1;
                operand_v1 = vs1_dout_i;
              end
              3'b100: begin
                rs1_en = 1'b1;
                operand_v1 = {VLMAX{rs1_dout_i}};
              end
              default: ;
            endcase
          end

          6'b000111: begin
            vs2_en = 1'b1;
            operand_v2 = vs2_dout_i;
            valu_opcode = VALU_OP_VMAX;
            vid_wb_en = 1'b1;
            case (funct3)
              3'b000: begin
                vs1_en = 1'b1;
                operand_v1 = vs1_dout_i;
              end
              3'b100: begin
                rs1_en = 1'b1;
                operand_v1 = {VLMAX{rs1_dout_i}};
              end
              3'b010: begin
                vs1_en = 1'b1;
                operand_v1 = vs1_dout_i;
                valu_opcode = VALU_OP_VREDMAX_VS;
              end
              default: ;
            endcase
          end

          6'b101001: begin 
            vs2_en = 1'b1;
            operand_v2 = vs2_dout_i;
            valu_opcode = VALU_OP_VSRA;
            vid_wb_en = 1'b1;
            case (funct3)
              3'b000: begin
                vs1_en = 1'b1;
                operand_v1 = vs1_dout_i;
              end
              3'b100: begin
                rs1_en = 1'b1;
                operand_v1 = {VLMAX{rs1_dout_i}};
              end
              3'b011: begin
                operand_v1 = {VLMAX{uimm}};
              end
              default: ;
            endcase
          end


          default: ;
        endcase
      end

      default: ;
    endcase
  end
  

endmodule

