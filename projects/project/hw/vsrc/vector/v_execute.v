// =======================================
// You need to finish this module
// =======================================

module v_alu #(
  parameter SEW       = 32,
  parameter VLMAX     = 8,
  parameter VALUOP_DW = 5,
  parameter VREG_DW   = 256,
  parameter VREG_AW   = 5
)(
  input                   clk,
  input                   rst,
  input [VALUOP_DW-1:0]   valu_opcode_i,
  input [VREG_DW-1:0]     operand_v1_i,
  input [VREG_DW-1:0]     operand_v2_i,
  output[VREG_DW-1:0]     valu_result_o
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

  wire signed [31:0] op1_elements [0:VLMAX-1];
  wire signed [31:0] op2_elements [0:VLMAX-1];
  reg  signed [31:0] result_elements [0:VLMAX-1];
  genvar i;
  generate
    for (i = 0; i < VLMAX; i = i + 1) begin : UNPACK
      assign op1_elements[i] = operand_v1_i[(i+1)*SEW-1:i*SEW];
      assign op2_elements[i] = operand_v2_i[(i+1)*SEW-1:i*SEW];
      assign valu_result_o[(i+1)*SEW-1:i*SEW] = result_elements[i];
    end
  endgenerate
  
  integer j;
  always @(*) begin
    case (valu_opcode_i)
      VALU_OP_VADD: begin
        for (j = 0; j < VLMAX; j = j + 1) begin
          result_elements[j] = op2_elements[j] + op1_elements[j];
        end
      end
      VALU_OP_VSUB: begin
        for (j = 0; j < VLMAX; j = j + 1) begin
          result_elements[j] = op2_elements[j] - op1_elements[j];
        end
      end
      VALU_OP_VMUL: begin
        for (j = 0; j < VLMAX; j = j + 1) begin
          result_elements[j] = op2_elements[j] * op1_elements[j];
        end
      end
      VALU_OP_VDIV: begin
        for (j = 0; j < VLMAX; j = j + 1) begin
          if (op2_elements[j] != 32'b0) begin
            result_elements[j] = op2_elements[j] / op1_elements[j];
          end else begin
            result_elements[j] = 32'hFFFFFFFF;
          end
        end
      end
      VALU_OP_VMIN: begin
        for (j = 0; j < VLMAX; j = j + 1) begin
          result_elements[j] = (op2_elements[j] < op1_elements[j]) ? 
                              op2_elements[j] : op1_elements[j];
        end
      end
      VALU_OP_VMAX: begin
        for (j = 0; j < VLMAX; j = j + 1) begin
          result_elements[j] = (op2_elements[j] > op1_elements[j]) ? 
                              op2_elements[j] : op1_elements[j];
        end
      end
      VALU_OP_VSRA: begin
        for (j = 0; j < VLMAX; j = j + 1) begin
          result_elements[j] = op2_elements[j] >>> op1_elements[j][4:0];
        end
      end
      
      VALU_OP_VREDSUM_VS: begin
        result_elements[0] = op1_elements[0];
        for (j = 0; j < VLMAX; j = j + 1) begin
          result_elements[0] = result_elements[0] + op2_elements[j];
        end
        for (j = 1; j < VLMAX; j = j + 1) begin
          result_elements[j] = result_elements[0];
        end
      end
      
      VALU_OP_VREDMAX_VS: begin
        result_elements[0] = op1_elements[0];
        for (j = 0; j < VLMAX; j = j + 1) begin
          if (op2_elements[j] > result_elements[0]) begin
            result_elements[0] = op2_elements[j];
          end
        end
        for (j = 1; j < VLMAX; j = j + 1) begin
          result_elements[j] = result_elements[0];
        end
      end
      
      default: begin  
        for (j = 0; j < VLMAX; j = j + 1) begin
          result_elements[j] = 0;
        end
      end
    endcase
  end

endmodule
