module v_inst_decode #(
  parameter VALUOP_DW = 5,
  parameter VMEM_DW   = 512,
  parameter VMEM_AW   = 64,
  parameter VREG_DW   = 512,
  parameter VREG_AW   = 5,
  parameter INST_DW   = 32,
  parameter REG_DW    = 64,
  parameter REG_AW    = 5
) (
  input                   clk,
  input                   rst,

  input   [INST_DW-1:0]   inst_i,

  output                  rs1_en_o,
  output  [REG_AW-1:0]    rs1_addr_o,
  input   [REG_DW-1:0]    rs1_dout_i,

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
  output  [VALUOP_DW-1:0] vmem_opcode_o,
  output  [32-1:0]        vmem_vs2select_o,

  output                  vid_wb_en_o,
  output                  vid_wb_sel_o,
  output  [VREG_AW-1:0]   vid_wb_addr_o,
  output                  vid_wb_double_o

);

  localparam VALU_OP_NOP          = 5'd0  ;
  localparam VALU_OP_VMUL8to16    = 5'd1  ;
  localparam VALU_OP_VADD16       = 5'd2  ;
  localparam VALU_OP_VDIV16       = 5'd3 ; 
  localparam VALU_OP_VMAX16       = 5'd4  ;
  localparam VALU_OP_VPOOL16      = 5'd14  ;
  localparam VALU_OP_VMUL16to32   = 5'd5  ;
  localparam VALU_OP_VADD32       = 5'd6  ;
  localparam VALU_OP_VDIV32       = 5'd7 ; 
  localparam VALU_OP_VMAX32       = 5'd8  ;
  localparam VALU_OP_VMUL32       = 5'd9  ;
  localparam VALU_OP_VMIN32       = 5'd10  ;
  localparam VALU_OP_VSUB32       = 5'd11  ;
  localparam VALU_OP_VRED10MAX32  = 5'd12  ;
  localparam VALU_OP_VRED10SUM32  = 5'd13  ;
  
  localparam VMEM_OP_NOP          = 5'd0  ;
  localparam VMEM_OP_CONV1          = 5'd1  ;

  wire [6:0]  opcode   = inst_i[6:0];
  wire [4:0]  vd       = inst_i[11:7];
  wire [2:0]  funct3   = inst_i[14:12];
  wire [4:0]  vs1      = inst_i[19:15];
  wire [4:0]  vs2      = inst_i[24:20];
  wire [6:0]  funct7   = inst_i[31:25];
  wire [4:0]  rs1      = vs1;       
  wire [4:0]  vs3      = vd;       
  wire [31:0] imm = {{27{inst_i[19]}}, inst_i[19:15]};

  wire [511:0] vs1_dout_i8to16;
  genvar i;
  generate
      for (i = 0; i < 32; i = i + 1) begin
          wire [7:0] byte_in = vs1_dout_i[255 - i*8 : 248 - i*8];
          wire [15:0] extended = {{8{byte_in[7]}}, byte_in};
          assign vs1_dout_i8to16[511 - i*16 : 496 - i*16] = extended;
      end
  endgenerate
  wire [511:0] vs2_dout_i8to16;
  generate
      for (i = 0; i < 32; i = i + 1) begin
          wire [7:0] byte_in = vs2_dout_i[255 - i*8 : 248 - i*8];
          wire [15:0] extended = {{8{byte_in[7]}}, byte_in};
          assign vs2_dout_i8to16[511 - i*16 : 496 - i*16] = extended;
      end
  endgenerate
  wire [511:0] vs1_dout_i16to32;
  generate
      for (i = 0; i < 16; i = i + 1) begin
          wire [15:0] byte_in = vs1_dout_i[255 - i*16 : 240 - i*16];
          wire [31:0] extended = {{16{byte_in[15]}}, byte_in};
          assign vs1_dout_i16to32[511 - i*32 : 480 - i*32] = extended;
      end
  endgenerate
  wire [511:0] vs2_dout_i16to32;
  generate
      for (i = 0; i < 16; i = i + 1) begin
          wire [15:0] byte_in = vs2_dout_i[255 - i*16 : 240 - i*16];
          wire [31:0] extended = {{16{byte_in[15]}}, byte_in};
          assign vs2_dout_i16to32[511 - i*32 : 480 - i*32] = extended;
      end
  endgenerate
  
  reg                  rs1_en;
  reg                  vs1_en;
  reg                  vs2_en;
  reg [VALUOP_DW-1:0]  valu_opcode;
  reg                  vmem_ren;
  reg                  vmem_wen;
  reg                  vid_wb_en;
  reg                  vid_wb_double;
  reg                  vid_wb_sel;
  reg                  wb_from_rs1;
  reg [VMEM_AW-1:0]    vmem_addr;
  reg [VMEM_DW-1:0]    vmem_din;
  reg  [VALUOP_DW-1:0] vmem_opcode;
  reg  [32-1:0]        vmem_vs2select;
  reg [VREG_DW-1:0]    operand_v1;
  reg [VREG_DW-1:0]    operand_v2;
  assign rs1_en_o = rs1_en;
  assign rs1_addr_o = rs1;
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
  assign vmem_opcode_o= vmem_opcode;
  assign vmem_vs2select_o= vmem_vs2select;
  assign vid_wb_en_o = vid_wb_en;
  assign vid_wb_double_o = vid_wb_double;
  assign vid_wb_sel_o = vid_wb_sel;
  assign vid_wb_addr_o = vd;
  
  always @(*) begin
    rs1_en = 1'b0;
    vs1_en = 1'b0;
    vs2_en = 1'b0;
    valu_opcode = VALU_OP_NOP;
    vmem_ren = 1'b0;
    vmem_wen = 1'b0;
    vid_wb_en = 1'b0;
    vid_wb_double = 1'b0;
    vid_wb_sel = 1'b0;
    vmem_addr = 64'b0;
    vmem_din = 512'b0;
    vmem_opcode=VMEM_OP_NOP;
    vmem_vs2select=32'b0;
    operand_v1 = 512'b0;
    operand_v2 = 512'b0;
    
    case (opcode)
      7'b0000111: begin  
          rs1_en = 1'b1;
          vmem_ren = 1'b1;
          vid_wb_en = 1'b1;
          vid_wb_sel = 1'b1;
          case (funct3[1:0])
            2'b00: begin
              vmem_addr = rs1_dout_i; 
            end
            2'b01: begin
              vmem_addr = 64'h80800000+funct7[6:4]*196+(rs1_dout_i+{62'b0,funct7[3:2]})*14+{62'b0,funct7[1:0]};
              vid_wb_double = 1'b1;
              vmem_opcode=VMEM_OP_CONV1;
              vs2_en = 1'b1;
              vmem_vs2select[7:0]=vs2_dout_i[((funct7[6:4] * 9) + (funct7[3:2] * 3) + {2'b0,funct7[1:0]}) * 8 +: 8];
              vmem_vs2select[31:8]={24{vs2_dout_i[((funct7[6:4] * 9) + (funct7[3:2] * 3) + {2'b0,funct7[1:0]}) * 8 +7]}};
            end
            default: ;
          endcase
      end
      7'b0100111: begin 
          rs1_en = 1'b1;
          vs2_en = 1'b1;
          vmem_wen = 1'b1;
          vmem_addr = rs1_dout_i; 
          vmem_din = vs2_dout_i;  
      end

      7'b1010111: begin
        case (funct7)
          7'b000000: begin 
            vs2_en = 1'b1;
            operand_v2 = vs2_dout_i8to16;
            valu_opcode = VALU_OP_VMUL8to16;
            vid_wb_en = 1'b1;
            case (funct3)
              3'b000: begin
                vs1_en = 1'b1;
                operand_v1 = vs1_dout_i8to16;
              end
              3'b100: begin
                rs1_en = 1'b1;
                operand_v1 = {32{{8{rs1_dout_i[7]}}, rs1_dout_i[7:0]}};
              end
              3'b011: begin
                operand_v1 = {32{imm[15:0]}};
              end
              default: ;
            endcase
          end
          7'b000001: begin
            vs2_en = 1'b1;
            operand_v2 = vs2_dout_i;
            valu_opcode = VALU_OP_VADD16;
            vid_wb_en = 1'b1;
            case (funct3)
              3'b000: begin
                vs1_en = 1'b1;
                operand_v1 = vs1_dout_i;
              end
              3'b100: begin
                rs1_en = 1'b1;
                operand_v1 = {32{rs1_dout_i[15:0]}};
              end
              3'b011: begin
                operand_v1 = {32{imm[15:0]}};
              end
              default: ;
            endcase
          end
          7'b000010: begin
            vs2_en = 1'b1;
            operand_v2 = vs2_dout_i;
            valu_opcode = VALU_OP_VDIV16;
            vid_wb_en = 1'b1;
            case (funct3)
              3'b100: begin
                rs1_en = 1'b1;
                operand_v1 = {32{rs1_dout_i[15:0]}};
              end
              default: ;
            endcase
          end
          7'b000011: begin
            vs2_en = 1'b1;
            operand_v2 = vs2_dout_i;
            valu_opcode = VALU_OP_VMAX16;
            vid_wb_en = 1'b1;
            case (funct3)
              3'b011: begin
                operand_v1 = {32{imm[15:0]}};
              end
              default: ;
            endcase
          end

          7'b001101: begin
            vs2_en = 1'b1;
            operand_v2 = vs2_dout_i;
            valu_opcode = VALU_OP_VPOOL16;
            vid_wb_en = 1'b1;
            case (funct3)
              3'b000: begin
                vs1_en = 1'b1;
                operand_v1 = vs1_dout_i;
              end
              default: ;
            endcase
          end

          7'b0000100: begin 
            vs2_en = 1'b1;
            operand_v2 = vs2_dout_i16to32;
            valu_opcode = VALU_OP_VMUL16to32;
            vid_wb_en = 1'b1;
            case (funct3)
              3'b000: begin
                vs1_en = 1'b1;
                operand_v1 = vs1_dout_i16to32;
              end
              3'b100: begin
                rs1_en = 1'b1;
                operand_v1 = {16{{16{rs1_dout_i[15]}}, rs1_dout_i[15:0]}};
              end
              3'b011: begin
                operand_v1 = {16{imm[31:0]}};
              end
              default: ;
            endcase
          end
          7'b000101: begin
            vs2_en = 1'b1;
            operand_v2 = vs2_dout_i;
            valu_opcode = VALU_OP_VADD32;
            vid_wb_en = 1'b1;
            case (funct3)
              3'b000: begin
                vs1_en = 1'b1;
                operand_v1 = vs1_dout_i;
              end
              3'b100: begin
                rs1_en = 1'b1;
                operand_v1 = {16{rs1_dout_i[31:0]}};
              end
              3'b011: begin
                operand_v1 = {16{imm[31:0]}};
              end
              default: ;
            endcase
          end
          7'b000110: begin
            vs2_en = 1'b1;
            operand_v2 = vs2_dout_i;
            valu_opcode = VALU_OP_VDIV32;
            vid_wb_en = 1'b1;
            case (funct3)
              3'b000: begin
                vs1_en = 1'b1;
                operand_v1 = vs1_dout_i;
              end
              3'b100: begin
                rs1_en = 1'b1;
                operand_v1 = {16{rs1_dout_i[31:0]}};
              end
              3'b011: begin
                operand_v1 = {16{imm[31:0]}};
              end
              default: ;
            endcase
          end
          7'b000111: begin
            vs2_en = 1'b1;
            operand_v2 = vs2_dout_i;
            valu_opcode = VALU_OP_VMAX32;
            vid_wb_en = 1'b1;
            case (funct3)
              3'b000: begin
                vs1_en = 1'b1;
                operand_v1 = vs1_dout_i;
              end
              3'b100: begin
                rs1_en = 1'b1;
                operand_v1 = {16{rs1_dout_i[31:0]}};
              end
              3'b011: begin
                operand_v1 = {16{imm[31:0]}};
              end
              default: ;
            endcase
          end

          7'b001000: begin
            vs2_en = 1'b1;
            operand_v2 = vs2_dout_i;
            valu_opcode = VALU_OP_VMUL32;
            vid_wb_en = 1'b1;
            case (funct3)
              3'b000: begin
                vs1_en = 1'b1;
                operand_v1 = vs1_dout_i;
              end
              3'b100: begin
                rs1_en = 1'b1;
                operand_v1 = {16{rs1_dout_i[31:0]}};
              end
              3'b011: begin
                operand_v1 = {16{imm[31:0]}};
              end
              default: ;
            endcase
          end

          7'b001001: begin
            vs2_en = 1'b1;
            operand_v2 = vs2_dout_i;
            valu_opcode = VALU_OP_VMIN32;
            vid_wb_en = 1'b1;
            case (funct3)
              3'b000: begin
                vs1_en = 1'b1;
                operand_v1 = vs1_dout_i;
              end
              3'b100: begin
                rs1_en = 1'b1;
                operand_v1 = {16{rs1_dout_i[31:0]}};
              end
              3'b011: begin
                operand_v1 = {16{imm[31:0]}};
              end
              default: ;
            endcase
          end
          7'b001010: begin
            vs2_en = 1'b1;
            operand_v2 = vs2_dout_i;
            valu_opcode = VALU_OP_VSUB32;
            vid_wb_en = 1'b1;
            case (funct3)
              3'b000: begin
                vs1_en = 1'b1;
                operand_v1 = vs1_dout_i;
              end
              3'b100: begin
                rs1_en = 1'b1;
                operand_v1 = {16{rs1_dout_i[31:0]}};
              end
              3'b011: begin
                operand_v1 = {16{imm[31:0]}};
              end
              default: ;
            endcase
          end
          7'b001011: begin
            vs2_en = 1'b1;
            operand_v2 = vs2_dout_i;
            valu_opcode = VALU_OP_VRED10MAX32;
            vid_wb_en = 1'b1;
            case (funct3)
              3'b000: begin
                vs1_en = 1'b1;
                operand_v1 = vs1_dout_i;
              end
              default: ;
            endcase
          end
          7'b001100: begin
            vs2_en = 1'b1;
            operand_v2 = vs2_dout_i;
            valu_opcode = VALU_OP_VRED10SUM32;
            vid_wb_en = 1'b1;
            case (funct3)
              3'b000: begin
                vs1_en = 1'b1;
                operand_v1 = vs1_dout_i;
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

