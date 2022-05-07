`include "../SRAM/SRAM_twoport.v"


module boundary_mem(
	input 		clk,
	input 		shift,
	input		reset,
	input 		[8:0] readaddress,
	input		[39:0] datain,
	output		[39:0] dataout
	);

	logic 		[8:0] readbase = 9'd1;
	logic 		[8:0] writebase = 9'd0;
	logic		[39:0] q_b; //non-necessary	

	logic 		shift_prev = 0;

	logic		[8:0] readaddress_mem;	

	assign readaddress_mem = readaddress + readbase;

	SRAM_twoport 	SRAM_twoport( //port a will be read, port b will be write
		.address_a(readaddress_mem),
		.address_b(writebase),
		.clock(clk),
		.data_a(39'bX),
		.data_b(datain),
		.wren_a(1'b0),
		.wren_b(1'b1),
		.q_a(dataout),
		.q_b(q_b)
	);

	always_ff @(negedge clk) begin

		if(reset) begin
			readbase <= 9'd1;
			writebase <= 9'd0;
			shift_prev <= shift;
		end
		if (shift != shift_prev) begin
			readbase <= readbase-1;
			writebase <= writebase-1;
			shift_prev <= shift;	
		end
	end


endmodule



