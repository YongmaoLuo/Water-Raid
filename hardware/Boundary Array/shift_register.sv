`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: Jakob Stiens
// 
// Create Date: 04/04/2022 04:42:56 PM
// Design Name: 
// Module Name: shift_register
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


module shift_register #(parameter MSB = 9)(input clk, input enable, input direction, input reset, 
                                            input logic[MSB:0] data, output logic[MSB:0] outputData);
 
    always @(posedge clk)
        if (!reset)
            outputData <= 0;
        else begin
            if (enable)
                case (direction)
                    0 : outputData <= {outputData[MSB:0], data};
                    1 : outputData <= {data, outputData[MSB:0]};
                endcase
             else
                outputData <= outputData;
             end
endmodule
 