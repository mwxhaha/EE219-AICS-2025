module v_mem #(
    parameter VMEM_DW   = 512,
    parameter VMEM_AW   = 64,
    parameter VRAM_DW   = 512,
    parameter VRAM_AW   = 64
)(
    input                   clk,
    input                   rst,
    
    input                   vmem_ren_i,
    input                   vmem_wen_i,
    input   [VMEM_AW-1:0]   vmem_addr_i,
    input   [VMEM_DW-1:0]   vmem_din_i,
    output  [1024-1:0]      vmem_dout_o,
    input  [5-1:0]          vmem_opcode_i,
    input  [32-1:0]         vmem_vs2select_i,
    input       [1024-1:0]  vd_data_i,

    output                  vram_ren_o,
    output                  vram_wen_o,
    output  [VRAM_AW-1:0]   vram_addr_o,
    output  [VRAM_DW-1:0]   vram_mask_o,
    output  [VRAM_DW-1:0]   vram_din_o,
    input   [VRAM_DW-1:0]   vram_dout_i
);
  
  localparam VMEM_OP_NOP           = 5'd0  ;
  localparam VMEM_OP_inputconv1    = 5'd1  ;
  localparam VMEM_OP_outputconv11  = 5'd2  ;
  localparam VMEM_OP_outputconv12  = 5'd3  ;

  assign vram_ren_o   = vmem_ren_i ;
  assign vram_wen_o   = vmem_wen_i ;
  assign vram_addr_o  = vmem_addr_i ;

  reg [VMEM_DW-1:0] vmem_din;
  always@(*) begin
    if (vmem_opcode_i == VMEM_OP_outputconv11) begin
      vmem_din[12*16-1:0] = vmem_din_i[12*16-1:0];  
      vmem_din[24*16-1:12*16] = vmem_din_i[26*16-1:14*16]; 
      vmem_din[28*16-1:24*16] = vmem_din_i[32*16-1:28*16]; 
      vmem_din[32*16-1:28*16] = 0; 
    end else if (vmem_opcode_i == VMEM_OP_outputconv12) begin
      vmem_din[8*16-1:0] = vmem_din_i[8*16-1:0];  
      vmem_din[20*16-1:8*16] = vmem_din_i[22*16-1:10*16]; 
      vmem_din[32*16-1:20*16] = 0; 
    end else begin
      vmem_din = vmem_din_i;
    end
  end

  reg [VMEM_DW-1:0] vram_din;
  always@(*) begin
    case (vmem_addr_i[2:0])
      3'b000: vram_din=vmem_din;
      3'b001: vram_din={vmem_din[503:0],8'b0};
      3'b010: vram_din={vmem_din[495:0],16'b0};
      3'b011: vram_din={vmem_din[487:0],24'b0};
      3'b100: vram_din={vmem_din[479:0],32'b0};
      3'b101: vram_din={vmem_din[471:0],40'b0};
      3'b110: vram_din={vmem_din[463:0],48'b0};
      3'b111: vram_din={vmem_din[455:0],56'b0};
      default: ;
    endcase
  end
  assign vram_din_o   = vram_din;
  
  reg [VRAM_DW-1:0] vram_mask;
  always@(*) begin
    case (vmem_addr_i[2:0])
      3'b000: vram_mask= {(VRAM_DW){1'b1}};
      3'b001: vram_mask={{(VRAM_DW-8){1'b1}},8'b0};
      3'b010: vram_mask={{(VRAM_DW-16){1'b1}},16'b0};
      3'b011: vram_mask={{(VRAM_DW-24){1'b1}},24'b0};
      3'b100: vram_mask={{(VRAM_DW-32){1'b1}},32'b0};
      3'b101: vram_mask={{(VRAM_DW-40){1'b1}},40'b0};
      3'b110: vram_mask={{(VRAM_DW-48){1'b1}},48'b0};
      3'b111: vram_mask={{(VRAM_DW-56){1'b1}},56'b0};
      default: ;
    endcase
  end
  assign vram_mask_o  =vram_mask;
  
  reg [VMEM_DW-1:0] vmem_dout;
  always@(*) begin
    case (vmem_addr_i[2:0])
      3'b000: vmem_dout=vram_dout_i;
      3'b001: vmem_dout={8'b0,vram_dout_i[511:8]};
      3'b010: vmem_dout={16'b0,vram_dout_i[511:16]};
      3'b011: vmem_dout={24'b0,vram_dout_i[511:24]};
      3'b100: vmem_dout={32'b0,vram_dout_i[511:32]};
      3'b101: vmem_dout={40'b0,vram_dout_i[511:40]};
      3'b110: vmem_dout={48'b0,vram_dout_i[511:48]};
      3'b111: vmem_dout={56'b0,vram_dout_i[511:56]};
      default: ;
    endcase
  end

  wire [1024-1:0] conv_result;
  generate
    for (genvar i = 0; i < 64; i = i + 1) begin : conv_calc
      wire [7:0]  vmem_8bit;
      wire [15:0] vmem_16bit_sign_ext;
      wire [15:0] vd_16bit;
      wire [15:0] product;
      
      wire [7:0] vmem_8bit = vmem_dout[(i*8)+7 : (i*8)];
      wire signed [15:0] vmem_16bit_sign_ext = {{8{vmem_8bit[7]}}, vmem_8bit};
      wire signed [15:0] vd_16bit = vd_data_i[(i*16)+15 : (i*16)];
      
      assign conv_result[(i*16)+15 : (i*16)]=vd_16bit+$signed(vmem_vs2select_i[15:0]) * vmem_16bit_sign_ext;
    end
  endgenerate
  
  
  assign vmem_dout_o  = vmem_opcode_i==VMEM_OP_NOP ? {512'b0,vmem_dout} : 
                        vmem_opcode_i==VMEM_OP_inputconv1 ? conv_result : 0;
  

endmodule
