module BoundaryPositions(	input logic clk,
				input logic [9:0] boundaryInput,
				input logic selectSignal,
				input logic [7:0] address,
				input logic shiftSignal,
				output logic [9:0] boundaryOutput)

	logic[2399:0] MegaReg;



	always @(posedge clk)
		if (shiftSignal == 1) begin
		shiftSignal = 0
			MegaReg >> 10;
			MegaReg[9:0] = boundaryInput;
			
							
		end
		if (selectSignal == 1) begin
			boundaryOutput = megaReg[(10*address):(10 * address - 10)]
		end
		



end module



			
