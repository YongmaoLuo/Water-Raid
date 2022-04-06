`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: Jakob Stiens
// 
// Create Date: 04/04/2022 04:37:56 PM
// Design Name: 
// Module Name: MegaShiftRegister
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

module BoundaryPositions(	input logic clk,//passed
                input logic enableSig,//just for this block
                input logic operationSig, //0 for read, 1 for write -- for this block  --passed to enable on MegaShiftRegister
				input logic [9:0] boundaryInput, //passed
				input logic direction, //passed
				input logic [7:0] address, //passed
				output logic [9:0] boundaryOutput); //passed

	MegaShiftRegister MegaReg(.clk(clk), .enable(enable), .direction(direction), .address(address),
                                            .shift_in(boundaryInput), .outputData(boundaryOutput));

    logic enable;


	always @(posedge clk) begin
	   if (enableSig == 1) begin
	       // if need to shift in values  / also read out 
            if (operationSig == 1) begin
                //shiftSignalStore <= boundaryInput;
                enable <= 1;
            //if want to read with no shifting    
            end else begin
                enable <= 0; // turn off shift
            end
            
		end
    end


endmodule



			
