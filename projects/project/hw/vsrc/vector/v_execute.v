module v_execute #(
  parameter VALUOP_DW = 5,
  parameter VREG_DW   = 512
)(
  input                   clk,
  input                   rst,
  input [VALUOP_DW-1:0]   valu_opcode_i,
  input [VREG_DW-1:0]     operand_v1_i,
  input [VREG_DW-1:0]     operand_v2_i,
  output[VREG_DW-1:0]     valu_result_o
);

  localparam VALU_OP_NOP        = 5'd0  ;
  localparam VALU_OP_VMUL       = 5'd1  ;
  localparam VALU_OP_VADD       = 5'd2  ;
  localparam VALU_OP_VDIV       = 5'd3 ; 
  localparam VALU_OP_VMAX       = 5'd4  ;

  wire signed [15:0] op1_elements [0:31];
  wire signed [15:0] op2_elements [0:31];
  reg  signed [15:0] result_elements [0:31];
  genvar i;
  generate
    for (i = 0; i < 31; i = i + 1) begin : UNPACK
      assign op1_elements[i] = operand_v1_i[(i+1)*16-1:i*16];
      assign op2_elements[i] = operand_v2_i[(i+1)*16-1:i*16];
      assign valu_result_o[(i+1)*16-1:i*16] = result_elements[i];
    end
  endgenerate
  
  integer j;
  always @(*) begin
    case (valu_opcode_i)
      VALU_OP_VMUL: begin
        for (j = 0; j < 32; j = j + 1) begin
          result_elements[j] = op2_elements[j] * op1_elements[j];
        end
      end
      VALU_OP_VADD: begin
        for (j = 0; j < 32; j = j + 1) begin
          result_elements[j] = op2_elements[j] + op1_elements[j];
        end
      end
      VALU_OP_VDIV: begin
        for (j = 0; j < 32; j = j + 1) begin
          result_elements[j] = op2_elements[j] / op1_elements[j];
        end
      end
      VALU_OP_VMAX: begin
        for (j = 0; j < 32; j = j + 1) begin
          result_elements[j] = (op2_elements[j] > op1_elements[j]) ? 
                              op2_elements[j] : op1_elements[j];
        end
      end
      
      default: begin  
        for (j = 0; j < 32; j = j + 1) begin
          result_elements[j] = 0;
        end
      end
    endcase
  end

endmodule
