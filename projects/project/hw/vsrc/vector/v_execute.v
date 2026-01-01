module v_execute #(
  parameter VALUOP_DW = 5,
  parameter VREG_DW   = 512
)(
  input                   clk,
  input                   rst,
  input [VALUOP_DW-1:0]   valu_opcode_i,
  input [VREG_DW-1:0]     operand_v1_i,
  input [VREG_DW-1:0]     operand_v2_i,
  output reg [VREG_DW-1:0]     valu_result_o
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

  wire signed [15:0] op1_elements16 [0:31];
  wire signed [15:0] op2_elements16 [0:31];
  reg  signed [15:0] result_elements16 [0:31];
  genvar i;
  generate
    for (i = 0; i < 32; i = i + 1) begin
      assign op1_elements16[i] = operand_v1_i[(i+1)*16-1:i*16];
      assign op2_elements16[i] = operand_v2_i[(i+1)*16-1:i*16];
    end
  endgenerate
  wire signed [31:0] op1_elements32 [0:15];
  wire signed [31:0] op2_elements32 [0:15];
  reg  signed [31:0] result_elements32 [0:15];
  generate
  for (i = 0; i < 16; i = i + 1) begin 
        assign op1_elements32[i] = operand_v1_i[(i+1)*32-1:i*32];
        assign op2_elements32[i] = operand_v2_i[(i+1)*32-1:i*32];
  end
  endgenerate
  
  integer j;
  reg signed [31:0] temp;
  reg signed [15:0] max_val;
  always @(*) begin
    for (j = 0; j < 32; j = j + 1) begin
      result_elements16[j] = 0;
    end
    for (j = 0; j < 16; j = j + 1) begin
      result_elements32[j] = 0;
    end
    valu_result_o=0;
    temp=0;
    max_val=0;

    case (valu_opcode_i)
      VALU_OP_VMUL8to16: begin
        for (j = 0; j < 32; j = j + 1) begin
          result_elements16[j] = op2_elements16[j] * op1_elements16[j];
          valu_result_o[j*16+:16] = result_elements16[j];
        end
      end
      VALU_OP_VADD16: begin
        for (j = 0; j < 32; j = j + 1) begin
          result_elements16[j] = op2_elements16[j] + op1_elements16[j];
          valu_result_o[j*16+:16] = result_elements16[j];
        end
      end
      VALU_OP_VDIV16: begin
        for (j = 0; j < 32; j = j + 1) begin
          result_elements16[j] = op2_elements16[j] / op1_elements16[j];
          valu_result_o[j*16+:16] = result_elements16[j];
        end
      end
      VALU_OP_VMAX16: begin
        for (j = 0; j < 32; j = j + 1) begin
          result_elements16[j] = (op2_elements16[j] > op1_elements16[j]) ? 
                              op2_elements16[j] : op1_elements16[j];
          valu_result_o[j*16+:16] = result_elements16[j];
        end
      end

      VALU_OP_VPOOL16: begin
        integer k;
        for (j = 0; j < 32; j = j + 1) begin
          result_elements16[j] = 0;
          valu_result_o[j*16+:16] = 0;
        end
        for (k = 0; k < 6; k = k + 1) begin
          max_val = op1_elements16[k*2];
          if (op1_elements16[k*2 + 1] > max_val) 
            max_val = op1_elements16[k*2 + 1];
          if (op1_elements16[k*2 + 12] > max_val) 
            max_val = op1_elements16[k*2 + 12];
          if (op1_elements16[k*2 + 13] > max_val) 
            max_val = op1_elements16[k*2 + 13];
          result_elements16[k] = max_val;
          valu_result_o[k*16+:16] = max_val;
        end
      end

      VALU_OP_VMUL16to32: begin
        for (j = 0; j < 16; j = j + 1) begin
          result_elements32[j] = op2_elements32[j] * op1_elements32[j];
          valu_result_o[j*32+:32] = result_elements32[j];
        end
      end
      VALU_OP_VADD32: begin
        for (j = 0; j < 16; j = j + 1) begin
          result_elements32[j] = op2_elements32[j] + op1_elements32[j];
          valu_result_o[j*32+:32] = result_elements32[j];
        end
      end
      VALU_OP_VDIV32: begin
        for (j = 0; j < 16; j = j + 1) begin
          result_elements32[j] = op2_elements32[j] / op1_elements32[j];
          valu_result_o[j*32+:32] = result_elements32[j];
        end
      end
      VALU_OP_VMAX32: begin
        for (j = 0; j < 16; j = j + 1) begin
          result_elements32[j] = (op2_elements32[j] > op1_elements32[j]) ? 
                              op2_elements32[j] : op1_elements32[j];
          valu_result_o[j*32+:32] = result_elements32[j];
        end
      end

      VALU_OP_VMUL32: begin
        for (j = 0; j < 16; j = j + 1) begin
          result_elements32[j] = op2_elements32[j] * op1_elements32[j];
          valu_result_o[j*32+:32] = result_elements32[j];
        end
      end

      VALU_OP_VMIN32: begin
        for (j = 0; j < 16; j = j + 1) begin
          result_elements32[j] = (op2_elements32[j] < op1_elements32[j]) ? 
                              op2_elements32[j] : op1_elements32[j];
          valu_result_o[j*32+:32] = result_elements32[j];
        end
      end
      VALU_OP_VSUB32: begin
        for (j = 0; j < 16; j = j + 1) begin
          result_elements32[j] = op2_elements32[j] - op1_elements32[j];
          valu_result_o[j*32+:32] = result_elements32[j];
        end
      end
      VALU_OP_VRED10MAX32: begin
        integer k;
        temp = op1_elements32[0];
        for (k = 0; k < 10; k = k + 1) begin
          if (op2_elements32[k] > temp) begin
            temp = op2_elements32[k];
          end
        end
        for (j = 0; j < 16; j = j + 1) begin
          result_elements32[j] = temp;
          valu_result_o[j*32+:32] = result_elements32[j];
        end
      end
      VALU_OP_VRED10SUM32: begin
        integer k;
        temp = op1_elements32[0];
        for (k = 0; k < 10; k = k + 1) begin
          temp = temp + op2_elements32[k];
        end
        for (j = 0; j < 16; j = j + 1) begin
          result_elements32[j] = temp;
          valu_result_o[j*32+:32] = result_elements32[j];
        end
      end

      default:;
    endcase
  end

endmodule
