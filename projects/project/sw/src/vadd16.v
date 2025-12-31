module vadd16 (
    input  wire         instr_valid,
    input  wire [31:0]  instr,
    input  wire [511:0] vs1_data,
    input  wire [511:0] vs2_data,
    input  wire [63:0]  xrs1_val,

    output reg          vreg_we,
    output reg  [4:0]   vwd_addr,
    output reg  [511:0] vwd_data,
    output wire         is_vadd16
);

    wire [6:0] opcode = instr[6:0];
    wire [4:0] rd     = instr[11:7];
    wire [2:0] funct3 = instr[14:12];
    wire [4:0] vs1    = instr[19:15];
    wire [4:0] vs2    = instr[24:20];
    wire [5:0] funct6 = instr[31:26];
    wire       vm     = instr[25];
    wire [6:0] funct7 = instr[31:25];

    wire opv_vadd16_vv = (opcode == 7'h57) && (funct6 == 6'b010001) && (vm == 1'b1) && (funct3 == 3'b000);
    wire opv_vadd16_vi = (opcode == 7'h57) && (funct6 == 6'b010001) && (vm == 1'b1) && (funct3 == 3'b011);
    wire r_vadd16_vx   = (opcode == 7'h57) && (funct7 == 7'h11) && (funct3 == 3'b100);

    assign is_vadd16 = instr_valid && (opv_vadd16_vv || opv_vadd16_vi || r_vadd16_vx);

    reg [511:0] a_vec, b_vec;
    reg [511:0] b_broadcast;
    integer k;

    always @(*) begin
        a_vec = 512'b0;
        b_vec = 512'b0;
        b_broadcast = 512'b0;

        if (opv_vadd16_vv) begin
            // vv
            a_vec = vs1_data;
            b_vec = vs2_data;

        end else if (r_vadd16_vx) begin
            // vx
            for (k = 0; k < 32; k = k + 1) begin
                b_broadcast[k*16 +: 16] = xrs1_val[15:0];
            end
            a_vec = vs2_data;
            b_vec = b_broadcast;

        end else if (opv_vadd16_vi) begin
            // vi
            reg signed [4:0]  imm5;
            reg signed [15:0] imm16;
            imm5  = vs1;
            imm16 = {{11{imm5[4]}}, imm5};

            for (k = 0; k < 32; k = k + 1) begin
                b_broadcast[k*16 +: 16] = imm16[15:0];
            end
            a_vec = vs2_data;
            b_vec = b_broadcast;
        end
    end

    wire [511:0] y_vec;

    genvar i;
    generate
        for (i = 0; i < 32; i = i + 1) begin : G_ADD16
            wire signed [15:0] a_i = a_vec[i*16 +: 16];
            wire signed [15:0] b_i = b_vec[i*16 +: 16];
            wire signed [16:0] s_i = a_i + b_i;
            assign y_vec[i*16 +: 16] = s_i[15:0];
        end
    endgenerate

    always @(*) begin
        vreg_we   = 1'b0;
        vwd_addr  = rd;
        vwd_data  = 512'b0;

        if (is_vadd16) begin
            vreg_we  = 1'b1;
            vwd_data = y_vec;
        end
    end

endmodule
