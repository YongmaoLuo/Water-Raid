module testBench(input logic clk)


BoundaryPositions BP1(	.clk(clk),
			.boundaryInput(boundaryInput),
			.selectSignal(selectSignal),
			.address(address),
			.shiftSignal(shiftSignal),
			.boundaryOutput(boundaryOutput))
	int test;
	logic [9:0] boundaryInput,
	logic selectSignal,
	logic [7:0] address,
	logic shiftSignal,
	logic [9:0] boundaryOutput	



	initial begin 
	address = 0;
	selectSignal = 0;
	boundaryInput = 0'
	boundaryOutput = 0'
	shift Signal = 0;
	test = 0;
	
	end 

	always @(posedge clk)
		
		test <= test + 1;
		shiftSignal = 1;
		boundaryInput = test;
		

end module
