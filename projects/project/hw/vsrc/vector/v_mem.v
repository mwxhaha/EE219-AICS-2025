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
    output  [VMEM_DW-1:0]   vmem_dout_o,

    output                  vram_ren_o,
    output                  vram_wen_o,
    output  [VRAM_AW-1:0]   vram_addr_o,
    output  [VRAM_DW-1:0]   vram_mask_o,
    output  [VRAM_DW-1:0]   vram_din_o,
    input   [VRAM_DW-1:0]   vram_dout_i
);
  
  assign vram_ren_o   = vmem_ren_i ;
  assign vram_wen_o   = vmem_wen_i ;
  assign vram_addr_o  = vmem_addr_i ;
  reg [VMEM_DW-1:0] vram_din;
  always@(*) begin
    case (vmem_addr_i[2:0])
      3'b000: vram_din=vmem_din_i;
      3'b001: vram_din={vmem_din_i[503:0],8'b0};
      3'b010: vram_din={vmem_din_i[495:0],16'b0};
      3'b011: vram_din={vmem_din_i[487:0],24'b0};
      3'b100: vram_din={vmem_din_i[479:0],32'b0};
      3'b101: vram_din={vmem_din_i[471:0],40'b0};
      3'b110: vram_din={vmem_din_i[463:0],48'b0};
      3'b111: vram_din={vmem_din_i[455:0],56'b0};
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
  assign vmem_dout_o  = vmem_dout ;

endmodule
