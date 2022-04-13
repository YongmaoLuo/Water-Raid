//`include "../SRAM/SRAM.v"

module boundary_mem(
	input 		clk,
	input 		shift,
	input 		[8:0] readaddress,
	input		[39:0] datain,
	output		[39:0] dataout
	);

	logic 		[8:0] readbase;
	logic 		[8:0] writebase;
		
	logic 		shift_prev;
	logic		wren;

	logic		[8:0] readaddress_mem;

	assign readaddress_mem = (readaddress + readbase)%480;
	
/*	SRAM		SRAM(
				.clock(clk),
				.data(datain),
				.rdaddress(readaddress_mem),
				.wraddress(writebase),
				.wren(wren),
				.q(dataout)
			);
*/
	always_ff @(posedge clk) begin
		if (shift != shift_prev) begin
			readbase <= (readbase+1)%480;
			writebase <= (writebase+1)%480;	
			wren <= 1;
			shift_prev <= shift;		
		end
		if (wren != 0) begin
			wren <= 0;
		end
	end


endmodule
