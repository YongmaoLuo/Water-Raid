`timescale 1ns / 1ps
`define HALF_CLOCK_PERIOD #.5

module testBench();

    logic clk;
	logic [9:0] test;
	logic enableSig;
	logic operationSig;
	logic [9:0] boundaryInput;
	logic direction;
	logic [7:0] address;
	logic [9:0] boundaryOutput;	

BoundaryPositions BP1(	.clk(clk),
            .enableSig(enableSig),
            .operationSig(operationSig),
			.boundaryInput(boundaryInput),
			.direction(direction),
			.address(address),
			.boundaryOutput(boundaryOutput));


	initial begin 
	//address = 0;
//	selectSignal = 0;
//	boundaryInput = 0;
	//boundaryOutput = 0;
	test = 0;
	address  = 8'b00000000;
	clk = 0;
	
	end 

	always begin
		#1
		clk = ~clk; // clock
	end


	always @(posedge clk) begin
		address <= 0;
		enableSig <= 1;
		test <= test + 1;
		operationSig <= 1; // write
		boundaryInput <= test;
		direction <= 0; //move to right
	end

endmodule





