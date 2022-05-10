//Module created by Jakob Stiens
`include "../ROM/explosion_ROM.v"
`include "../ROM/shoot_ROM.v"
module tonegen (
		input clk, // 50MHz
		input reset,
		input logic startAudio,
		input logic selectedAudio,
		input left_chan_ready,
		input right_chan_ready,
		output logic [15:0] sample_data_l,
		output logic [15:0] sample_data_r,
		output logic sample_valid_r,
		output logic sample_valid_l
		
	);

	logic [11:0] counter;      
	logic [13:0] shoot_address;
	logic [13:0] explode_address;
	//input logic [7:0]  address;

	//instantiate memory for shoot sound effect from .v file from MegaWizard and .mif file
	shoot_ROM	shoot_ROM(.address(shoot_address),.clock(clk),.q(shoot_out)); //8105 addresses
	//instantiate memory for explode sound effect from .v file from MegaWizard and .mif file
	explosion_ROM	explode_ROM(.address(explode_address),.clock(clk),.q(explode_out)); //3044 addresses


	always_ff @(startAudio) begin
	   	if (reset) begin
			counter <= 0;
			//address <= 0;
			sample_valid_l <= 0;
			sample_valid_r <= 0;
		end
		if (startAudio == 1) begin

			if (left_chan_ready && right_chan_ready) begin
				sample_valid_l <= 1;
				sample_valid_r <= 1;
				if (selectedAudio == 0) begin
					shoot_address <= counter;
					sample_data_l <= shoot_out;
					sample_data_r <= shoot_out;
				end else if (selectedAudio == 1) begin
					explode_address <= counter;
					sample_data_l <= explode_out;
					sample_data_r <= explode_out;
				end
				counter <= counter + 1;
			end

		end else begin
			counter <= 0;
			sample_valid_l <= 0;
			sample_valid_r <= 0;
		end
		//address <= address + 1; // move to next audio point
		//sample_valid <= 1'b1; // set to valid
	end

endmodule
