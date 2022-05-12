/*
 * Avalon memory-mapped peripheral that generates VGA
 *
 * Stephen A. Edwards
 * Columbia University
 */
`include "../ROM/plane_ROM.v"
`include "../ROM/planeleft_ROM.v"
`include "../ROM/PlaneRight.v"
`include "../ROM/zero_ROM.v"
`include "../ROM/one_ROM.v"
`include "../ROM/two_ROM.v"
`include "../ROM/three_ROM.v"
`include "../ROM/four_ROM.v"
`include "../ROM/five_ROM.v"
`include "../ROM/six_ROM.v"
`include "../ROM/seven_ROM.v"
`include "../ROM/eight_ROM.v"
`include "../ROM/nine_ROM.v"
`include "../ROM/fuel_ROM.v"
`include "../ROM/chopper_ROM.v"
`include "../ROM/battleship_ROM.v"
`include "../ROM/scoreboard_ROM.v"
`include "../ROM/fuelgauge_ROM.v"
`include "../ROM/indicator_ROM.v"
`include "../ROM/shoot_ROM.v"
`include "../ROM/explosion_ROM.v"
`include "../ROM/hotairballoon_ROM.v"
`include "../ROM/reversebattleship_ROM.v"
`include "../ROM/reversehelicopter_ROM.v"
`include "../BoundaryMemory/boundary_mem.sv"
`include "shoot.v"
`include "bomb.v"
`include "hit.v"



module vga_ball(input logic        clk,
	        input logic 	   reset,
		input logic [15:0] writedata,
		input logic 	   write,
		input 		   chipselect,
		input logic [5:0]  address,


		//input logic [7:0] volume,
		input left_chan_ready,
		input right_chan_ready,
		output logic [15:0] sample_data_l,
		output logic sample_valid_l,
		output logic [15:0] sample_data_r,
		output logic sample_valid_r,

		output logic [7:0] VGA_R, VGA_G, VGA_B,
		output logic 	   VGA_CLK, VGA_HS, VGA_VS,
		                   VGA_BLANK_n,
		output logic 	   VGA_SYNC_n);

   logic [10:0]	   hcount;
   logic [9:0]     vcount;
   
   logic [9:0]	   boundary_1_IN;
   logic [9:0]	   boundary_2_IN;
   logic [9:0]	   boundary_3_IN;
   logic [9:0]	   boundary_4_IN;

   logic [9:0]	   boundary_1;
   logic [9:0]	   boundary_2;
   logic [9:0]	   boundary_3;
   logic [9:0]	   boundary_4;

   logic [9:0]	   boundary_1_LATCHED;
   logic [9:0]	   boundary_2_LATCHED;
   logic [9:0]	   boundary_3_LATCHED;
   logic [9:0]	   boundary_4_LATCHED;

   logic [39:0]    boundary_out;

   assign boundary_1 = boundary_out[39:30];
   assign boundary_2 = boundary_out[29:20];
   assign boundary_3 = boundary_out[19:10];
   assign boundary_4 = boundary_out[9:0];

   logic [3:0]	   current_color; //for sprites
   logic [3:0] 	   current_color_NONSPRITE;
   logic [3:0]	   current_background; //for background

   logic [3:0] 	   current_color_LATCHED;
   logic [3:0] 	   current_color_NONSPRITE_LATCHED;
   logic [3:0]	   current_background_LATCHED;

   logic [3:0] 	   sprite1_color;
   logic [3:0]     sprite2_color;
   logic [3:0]     sprite3_color;
   logic [3:0]     sprite4_color;
   logic [3:0]     sprite5_color;
   logic [3:0] 	   sprite6_color;
   logic [3:0] 	   sprite7_color;
   logic [3:0] 	   sprite8_color;
   logic [3:0] 	   sprite9_color;
   logic [3:0]	   digit1_color;
   logic [3:0]	   digit2_color;
   logic [3:0]	   digit3_color;

   logic [3:0] 	   sprite1_color_LATCHED;
   logic [3:0]     sprite2_color_LATCHED;
   logic [3:0]     sprite3_color_LATCHED;
   logic [3:0]     sprite4_color_LATCHED;
   logic [3:0]	   sprite5_color_LATCHED;
   logic [3:0] 	   sprite6_color_LATCHED;
   logic [3:0] 	   sprite7_color_LATCHED;
   logic [3:0] 	   sprite8_color_LATCHED;
   logic [3:0] 	   sprite9_color_LATCHED;
   logic [3:0]	   digit1_color_LATCHED;
   logic [3:0]	   digit2_color_LATCHED;
   logic [3:0]	   digit3_color_LATCHED;
   // last bit of y positions indicates whether sprite is onscreen
   logic	   shift;
   // logic to determine whether or not to pull reset high

   logic [9:0]	   sprite1_x;
   logic [9:0]	   sprite1_y;
   logic [4:0]	   sprite1_img; //which sprite is this?

   logic [9:0]	   sprite2_x;
   logic [9:0]	   sprite2_y;
   logic [4:0]	   sprite2_img; //which sprite is this?

   logic [9:0]	   sprite3_x;
   logic [9:0]	   sprite3_y;
   logic [4:0]	   sprite3_img; //which sprite is this?

   logic [9:0]	   sprite4_x;
   logic [9:0]	   sprite4_y;
   logic [4:0]	   sprite4_img; //which sprite is this?

   logic [9:0]     sprite5_x;
   logic [9:0]	   sprite5_y;
   logic [4:0]     sprite5_img;

   logic [9:0]	   sprite6_x;
   logic [9:0]	   sprite6_y;
   logic [4:0]	   sprite6_img; //which sprite i.19.0'


   logic [9:0]	   sprite7_x;
   logic [9:0]	   sprite7_y;
   logic [4:0]	   sprite7_img; //which sprite is this?

   logic [9:0]	   sprite8_x;
   logic [9:0]	   sprite8_y;
   logic [4:0]	   sprite8_img; //which sprite is this?

   logic [9:0]	   sprite9_x;
   logic [9:0]	   sprite9_y;
   logic [4:0]	   sprite9_img; //which sprite is this?

   logic [9:0]     digit1_x;
   logic [9:0]	   digit1_y;
   logic [3:0]     digit1_img;

   logic [9:0]     digit2_x;
   logic [9:0]	   digit2_y;
   logic [3:0]     digit2_img;

   logic [9:0]     digit3_x;
   logic [9:0]	   digit3_y;
   logic [3:0]     digit3_img;


   logic [9:0]     scoreboard_x;
   logic [9:0]     scoreboard_y;
   logic [9:0]	   fuelgauge_x;
   logic [9:0]	   fuelgauge_y;
   logic [9:0]     indicator_x;
   logic [9:0]     indicator_y;

   logic	   isSprite;
   logic	   isMusic;  //remove
   logic [1:0]	   whichClip;

//audio
   logic shootRegister;
   logic hitRegister;
   logic explodeRegister;


   boundary_mem boundary_mem(
	.clk(clk), 
	.shift(shift), 
	.reset(reset), 
	.readaddress(vcount[8:0]), 
	.datain({boundary_1_IN, boundary_2_IN, boundary_3_IN, boundary_4_IN}),
	.dataout(boundary_out)
   );
	


   vga_counters counters(.clk50(clk), .*);
	
   //ROM Wires

   logic [9:0]	   sprite1_address;
   logic [9:0]	   sprite2_address;
   logic [9:0]     sprite3_address;
   logic [9:0]	   sprite4_address;
   logic [9:0] 	   sprite5_address;
   logic [9:0]	   sprite6_address;
   logic [9:0]	   sprite7_address;
   logic [9:0]	   sprite8_address;
   logic [9:0]	   sprite9_address;
   logic [9:0]	   digit1_address;
   logic [9:0]	   digit2_address;
   logic [9:0]	   digit3_address;

   logic	   isSprite1;
   logic	   isSprite2;
   logic 	   isSprite3;
   logic	   isSprite4;
   logic	   isSprite5;
   logic	   isSprite6;
   logic	   isSprite7;
   logic	   isSprite8;
   logic	   isSprite9;
   logic	   isScoreboard;
   logic	   isFuelgauge;
   logic	   isIndicator;
   logic	   isDigit1;
   logic	   isDigit2;
   logic	   isDigit3;

   logic	   isSprite1_LATCHED;
   logic	   isSprite2_LATCHED;
   logic 	   isSprite3_LATCHED;
   logic	   isSprite4_LATCHED;
   logic	   isSprite5_LATCHED;
   logic	   isSprite6_LATCHED;
   logic	   isSprite7_LATCHED;
   logic	   isSprite8_LATCHED;
   logic	   isSprite9_LATCHED;
   logic	   isScoreboard_LATCHED;
   logic	   isFuelgauge_LATCHED;
   logic	   isIndicator_LATCHED;
   logic	   isDigit1_LATCHED;
   logic	   isDigit2_LATCHED;
   logic	   isDigit3_LATCHED;

   logic [3:0]     plane_out;
   logic [9:0]     plane_address;
   logic [9:0]	   plane_address_LATCHED;

   logic [3:0]     planeleft_out;
   logic [9:0]     planeleft_address;
   logic [9:0]	   planeleft_address_LATCHED;

   logic [3:0]     planeright_out;
   logic [9:0]     planeright_address;
   logic [9:0]	   planeright_address_LATCHED;

   logic [3:0]     chopper_out;
   logic [9:0] 	   chopper_address;
   logic [9:0]     chopper_address_LATCHED;

   logic [3:0]     battleship_out;
   logic [9:0]     battleship_address;
   logic [9:0]     battleship_address_LATCHED;

   logic [3:0]     reversebattleship_out;
   logic [9:0]     reversebattleship_address;
   logic [9:0]     reversebattleship_address_LATCHED;

   logic [3:0]     reversechopper_out;
   logic [9:0] 	   reversechopper_address;
   logic [9:0]     reversechopper_address_LATCHED;

   logic [3:0]     fuel_out;
   logic [9:0]     fuel_address;
   logic [9:0]     fuel_address_LATCHED;

   logic [3:0]     shoot_out;
   logic [9:0]     shoot_address;
   logic [9:0]	   shoot_address_LATCHED;

   logic [3:0]     explosion_out;
   logic [9:0]     explosion_address;
   logic [9:0]	   explosion_address_LATCHED;

   logic [3:0]     hotairballoon_out;
   logic [9:0]     hotairballoon_address;
   logic [9:0]	   hotairballoon_address_LATCHED;

   logic [3:0]	   scoreboard_out;
   logic [9:0]     scoreboard_address;
   logic [9:0]     scoreboard_address_LATCHED;

   logic [3:0]     fuelgauge_out;
   logic [11:0]    fuelgauge_address;
   logic [11:0]    fuelgauge_address_LATCHED;

   logic [3:0]     indicator_out;
   logic [9:0]     indicator_address;
   logic [9:0]	   indicator_address_LATCHED;

   plane_ROM 		plane_ROM(.address(plane_address_LATCHED), .clock(clk),.q(plane_out));
   planeleft_ROM 	planeleft_ROM(.address(planeleft_address_LATCHED), .clock(clk),.q(planeleft_out));
   PlaneRight 	planeright_ROM(.address(planeright_address_LATCHED), .clock(clk),.q(planeright_out));
   chopper_ROM 		chopper_ROM(.address(chopper_address_LATCHED),.clock(clk),.q(chopper_out));	
   reversehelicopter_ROM reversehelicopter_ROM(.address(reversechopper_address_LATCHED),.clock(clk),.q(reversechopper_out));	
   reversebattleship_ROM reversebatteship_ROM(.address(reversebattleship_address_LATCHED), .clock(clk), .q(reversebattleship_out));
   battleship_ROM 	batteship_ROM(.address(battleship_address_LATCHED), .clock(clk), .q(battleship_out));
   fuel_ROM 		fuel_ROM(.address(fuel_address_LATCHED), .clock(clk), .q(fuel_out));
   scoreboard_ROM 	scoreboard_ROM(.address(scoreboard_address_LATCHED), .clock(clk), .q(scoreboard_out));
   fuelgauge_ROM	fuelgauge_ROM(.address(fuelgauge_address_LATCHED),.clock(clk),.q(fuelgauge_out));
   indicator_ROM	indicator_ROM(.address(indicator_address_LATCHED),.clock(clk),.q(indicator_out));
   shoot_ROM 		shoot_ROM(.address(shoot_address_LATCHED), .clock(clk),.q(shoot_out));
   explosion_ROM 	explosion_ROM(.address(explosion_address_LATCHED), .clock(clk),.q(explosion_out));
   hotairballoon_ROM 	hotairballoon_ROM(.address(hotairballoon_address_LATCHED), .clock(clk),.q(hotairballoon_out));


   logic [3:0]	   zero_out;
   logic [9:0]	   zero_address;
   logic [9:0] 	   zero_address_LATCHED;

   zero_ROM		zero_ROM(.address(zero_address_LATCHED),.clock(clk), .q(zero_out));

   logic [3:0]	   one_out;
   logic [9:0]	   one_address;
   logic [9:0] 	   one_address_LATCHED;

   one_ROM		one_ROM(.address(one_address_LATCHED),.clock(clk), .q(one_out));

   logic [3:0]	   two_out;
   logic [9:0]	   two_address;
   logic [9:0] 	   two_address_LATCHED;

   two_ROM		two_ROM(.address(two_address_LATCHED),.clock(clk), .q(two_out));

   logic [3:0]	   three_out;
   logic [9:0]	   three_address;
   logic [9:0] 	   three_address_LATCHED;

   three_ROM		three_ROM(.address(three_address_LATCHED),.clock(clk),.q(three_out));

   logic [3:0]	   four_out;
   logic [9:0]	   four_address;
   logic [9:0] 	   four_address_LATCHED;

   four_ROM		four_ROM(.address(four_address_LATCHED),.clock(clk), .q(four_out));

   logic [3:0]	   five_out;
   logic [9:0]	   five_address;
   logic [9:0] 	   five_address_LATCHED;

   five_ROM		five_ROM(.address(five_address_LATCHED),.clock(clk), .q(five_out));

   logic [3:0]	   six_out;
   logic [9:0]	   six_address;
   logic [9:0] 	   six_address_LATCHED;

   six_ROM		six_ROM(.address(six_address_LATCHED),.clock(clk), .q(six_out));

   logic [3:0]	   seven_out;
   logic [9:0]	   seven_address;
   logic [9:0] 	   seven_address_LATCHED;

   seven_ROM		seven_ROM(.address(seven_address_LATCHED),.clock(clk), .q(seven_out));

   logic [3:0]	   eight_out;
   logic [9:0]	   eight_address;
   logic [9:0] 	   eight_address_LATCHED;

   eight_ROM		eight_ROM(.address(eight_address_LATCHED),.clock(clk), .q(eight_out));

   logic [3:0]	   nine_out;
   logic [9:0]	   nine_address;
   logic [9:0] 	   nine_address_LATCHED;

   nine_ROM		nine_ROM(.address(nine_address_LATCHED),.clock(clk), .q(nine_out));


   assign isSprite = isSprite1_LATCHED || isSprite2_LATCHED || isSprite3_LATCHED || isSprite4_LATCHED || isSprite5_LATCHED || isSprite6_LATCHED || isSprite7_LATCHED || isSprite8_LATCHED ||isSprite9_LATCHED ||isScoreboard_LATCHED || isFuelgauge_LATCHED || isIndicator_LATCHED || isDigit1_LATCHED || isDigit2_LATCHED || isDigit3_LATCHED;

   always_ff @(posedge clk) begin
     if (chipselect && write)
       case (address)

	 6'd0 : boundary_1_IN 		<= writedata[9:0];
	 6'd1 : boundary_2_IN 		<= writedata[9:0];
	 6'd2 : boundary_3_IN 		<= writedata[9:0];
	 6'd3 : boundary_4_IN 		<= writedata[9:0];
	 6'd4 : shift 			<= writedata[0];
	 6'd5 : sprite1_x 		<= writedata[9:0];
	 6'd6 : sprite1_y 		<= writedata[9:0];
	 6'd7 : sprite1_img 		<= writedata[4:0];
	 6'd8 : sprite2_x 		<= writedata[9:0];
	 6'd9 : sprite2_y 		<= writedata[9:0];
	 6'd10 : sprite2_img 		<= writedata[4:0];
	 6'd11 : sprite3_x 		<= writedata[9:0];
	 6'd12 : sprite3_y 		<= writedata[9:0];
	 6'd13 : sprite3_img 		<= writedata[4:0];
	 6'd14 : sprite4_x		<= writedata[9:0];
	 6'd15 : sprite4_y		<= writedata[9:0];
	 6'd16 : sprite4_img 		<= writedata[4:0];
	 6'd17 : sprite5_x		<= writedata[9:0];
	 6'd18 : sprite5_y		<= writedata[9:0];
	 6'd19 : sprite5_img 		<= writedata[4:0];
	 6'd20 : sprite6_x		<= writedata[9:0];
	 6'd21 : sprite6_y		<= writedata[9:0];
	 6'd22 : sprite6_img 		<= writedata[4:0];
	 6'd23 : sprite7_x		<= writedata[9:0];
	 6'd24 : sprite7_y		<= writedata[9:0];
	 6'd25 : sprite7_img 		<= writedata[4:0];
	 6'd26 : sprite8_x		<= writedata[9:0];
	 6'd27 : sprite8_y		<= writedata[9:0];
	 6'd28 : sprite8_img 		<= writedata[4:0];
	 6'd29 : sprite9_x		<= writedata[9:0];
	 6'd30 : sprite9_y		<= writedata[9:0];
	 6'd31 : sprite9_img 		<= writedata[4:0];
	 6'd32 : scoreboard_x		<= writedata[9:0];
	 6'd33 : scoreboard_y		<= writedata[9:0];
	 6'd34 : digit1_x		<= writedata[9:0];
	 6'd35 : digit1_y		<= writedata[9:0];
	 6'd36 : digit1_img		<= writedata[3:0];	
	 6'd37 : digit2_x		<= writedata[9:0];
	 6'd38 : digit2_y		<= writedata[9:0];
	 6'd39 : digit2_img		<= writedata[3:0];
	 6'd40 : digit3_x		<= writedata[9:0];
	 6'd41 : digit3_y		<= writedata[9:0];
	 6'd42 : digit3_img		<= writedata[3:0];
	 6'd43 : fuelgauge_x		<= writedata[9:0];
	 6'd44 : fuelgauge_y		<= writedata[9:0];
	 6'd45 : indicator_x		<= writedata[9:0];
	 6'd46 : indicator_y		<= writedata[9:0];
	 6'd47 : shootRegister		<= writedata[0];
	 6'd48 : hitRegister		<= writedata[0];
	 6'd49 : explodeRegister	<= writedata[0];


       endcase
   end

   always_ff @(posedge clk) begin
	if (reset) begin
	   {VGA_R, VGA_G, VGA_B} <= {8'h00, 8'h00, 8'h00}; //Black
        end 
        else if(isSprite && current_color_LATCHED != 0 && vcount >= 80) begin
		case(current_color_LATCHED)

			0: {VGA_R, VGA_G, VGA_B} 	<= {8'hff, 8'hff, 8'hff}; //White
			1: {VGA_R, VGA_G, VGA_B} 	<= {8'h00, 8'hff, 8'h00}; //Green
			2: {VGA_R, VGA_G, VGA_B} 	<= {8'h00, 8'h00, 8'hff}; //Blue
			3: {VGA_R, VGA_G, VGA_B} 	<= {8'hff, 8'h00, 8'h00}; //Red
			4: {VGA_R, VGA_G, VGA_B} 	<= {8'hff, 8'hff, 8'h00}; //Yellow
			5: {VGA_R, VGA_G, VGA_B} 	<= {8'h00, 8'hff, 8'hff}; //Cyan
			6: {VGA_R, VGA_G, VGA_B} 	<= {8'hff, 8'h00, 8'hff}; //Magenta
			7: {VGA_R, VGA_G, VGA_B} 	<= {8'h80, 8'h80, 8'h80}; //Gray
			8: {VGA_R, VGA_G, VGA_B} 	<= {8'h00, 8'h00, 8'h00}; //Black
			9: {VGA_R, VGA_G, VGA_B} 	<= {8'hff, 8'hff, 8'h00}; //White
			10: {VGA_R, VGA_G, VGA_B} 	<= {8'hff, 8'hff, 8'hff}; //White
			11: {VGA_R, VGA_G, VGA_B} 	<= {8'hff, 8'hff, 8'hff}; //White
			12: {VGA_R, VGA_G, VGA_B} 	<= {8'hff, 8'hff, 8'hff}; //White
			13: {VGA_R, VGA_G, VGA_B} 	<= {8'hff, 8'hff, 8'hff}; //White
			14: {VGA_R, VGA_G, VGA_B} 	<= {8'hff, 8'hff, 8'hff}; //White
			15: {VGA_R, VGA_G, VGA_B} 	<= {8'hff, 8'hff, 8'hff}; //White

		endcase
	end
	else if((isScoreboard_LATCHED || isFuelgauge_LATCHED || isIndicator_LATCHED || isDigit1_LATCHED || isDigit2_LATCHED || isDigit3_LATCHED) && current_color_NONSPRITE_LATCHED != 0) begin

		case(current_color_NONSPRITE_LATCHED)

			0: {VGA_R, VGA_G, VGA_B} 	<= {8'hff, 8'hff, 8'hff}; //White
			1: {VGA_R, VGA_G, VGA_B} 	<= {8'h00, 8'hff, 8'h00}; //Green
			2: {VGA_R, VGA_G, VGA_B} 	<= {8'h00, 8'h00, 8'hff}; //Blue
			3: {VGA_R, VGA_G, VGA_B} 	<= {8'hff, 8'h00, 8'h00}; //Red
			4: {VGA_R, VGA_G, VGA_B} 	<= {8'hff, 8'hff, 8'h00}; //Yellow
			5: {VGA_R, VGA_G, VGA_B} 	<= {8'h00, 8'hff, 8'hff}; //Cyan
			6: {VGA_R, VGA_G, VGA_B} 	<= {8'hff, 8'h00, 8'hff}; //Magenta
			7: {VGA_R, VGA_G, VGA_B} 	<= {8'h80, 8'h80, 8'h80}; //Gray
			8: {VGA_R, VGA_G, VGA_B} 	<= {8'h00, 8'h00, 8'h00}; //Black
			9: {VGA_R, VGA_G, VGA_B} 	<= {8'hff, 8'hff, 8'h00}; //White
			10: {VGA_R, VGA_G, VGA_B} 	<= {8'hff, 8'hff, 8'hff}; //White
			11: {VGA_R, VGA_G, VGA_B} 	<= {8'hff, 8'hff, 8'hff}; //White
			12: {VGA_R, VGA_G, VGA_B} 	<= {8'hff, 8'hff, 8'hff}; //White
			13: {VGA_R, VGA_G, VGA_B} 	<= {8'hff, 8'hff, 8'hff}; //White
			14: {VGA_R, VGA_G, VGA_B} 	<= {8'hff, 8'hff, 8'hff}; //White
			15: {VGA_R, VGA_G, VGA_B} 	<= {8'hff, 8'hff, 8'hff}; //White

		endcase


	end
	else begin
		case(current_background_LATCHED)

			0: {VGA_R, VGA_G, VGA_B} 	<= {8'hff, 8'hff, 8'hff}; //White
			1: {VGA_R, VGA_G, VGA_B} 	<= {8'h00, 8'hff, 8'h00}; //Green
			2: {VGA_R, VGA_G, VGA_B} 	<= {8'h00, 8'h00, 8'hff}; //Blue
			3: {VGA_R, VGA_G, VGA_B} 	<= {8'hff, 8'h00, 8'h00}; //Red
			4: {VGA_R, VGA_G, VGA_B} 	<= {8'hff, 8'hff, 8'h00}; //Yellow
			5: {VGA_R, VGA_G, VGA_B} 	<= {8'h00, 8'hff, 8'hff}; //Cyan
			6: {VGA_R, VGA_G, VGA_B} 	<= {8'hff, 8'h00, 8'hff}; //Magenta
			7: {VGA_R, VGA_G, VGA_B} 	<= {8'h80, 8'h80, 8'h80}; //Gray
			8: {VGA_R, VGA_G, VGA_B} 	<= {8'h00, 8'h00, 8'h00}; //Black
			9: {VGA_R, VGA_G, VGA_B} 	<= {8'hff, 8'hff, 8'h00}; //White
			10: {VGA_R, VGA_G, VGA_B} 	<= {8'hff, 8'hff, 8'hff}; //White
			11: {VGA_R, VGA_G, VGA_B} 	<= {8'hff, 8'hff, 8'hff}; //White
			12: {VGA_R, VGA_G, VGA_B} 	<= {8'hff, 8'hff, 8'hff}; //White
			13: {VGA_R, VGA_G, VGA_B} 	<= {8'hff, 8'hff, 8'hff}; //White
			14: {VGA_R, VGA_G, VGA_B} 	<= {8'hff, 8'hff, 8'hff}; //White
			15: {VGA_R, VGA_G, VGA_B} 	<= {8'hff, 8'hff, 8'hff}; //White

		endcase
	end
   end
   
   always @(posedge clk) begin //latching of some values
	current_color_LATCHED 		<= current_color;
	current_background_LATCHED 	<= current_background;
	current_color_NONSPRITE_LATCHED <= current_color_NONSPRITE;
	boundary_1_LATCHED 		<= boundary_1;
	boundary_2_LATCHED 		<= boundary_2;
	boundary_3_LATCHED 		<= boundary_3;
	boundary_4_LATCHED 		<= boundary_4;
	plane_address_LATCHED		<= plane_address;
	planeleft_address_LATCHED	<= planeleft_address;
	planeright_address_LATCHED	<= planeright_address;
	shoot_address_LATCHED		<= shoot_address;
	explosion_address_LATCHED	<= explosion_address;
	hotairballoon_address_LATCHED	<= hotairballoon_address;
	chopper_address_LATCHED		<= chopper_address;
	reversechopper_address_LATCHED		<= reversechopper_address;
	battleship_address_LATCHED	<= battleship_address;
	reversebattleship_address_LATCHED	<= reversebattleship_address;
	fuel_address_LATCHED 		<= fuel_address;
	scoreboard_address_LATCHED	<= scoreboard_address;
	fuelgauge_address_LATCHED	<= fuelgauge_address;
	indicator_address_LATCHED	<= indicator_address;
	zero_address_LATCHED		<= zero_address;
	one_address_LATCHED		<= one_address;
	two_address_LATCHED		<= two_address;
	three_address_LATCHED		<= three_address;
	four_address_LATCHED		<= four_address;
	five_address_LATCHED		<= five_address;
	six_address_LATCHED		<= six_address;
	seven_address_LATCHED		<= seven_address;
	eight_address_LATCHED		<= eight_address;
	nine_address_LATCHED		<= nine_address;
   	sprite1_color_LATCHED		<= sprite1_color;
   	sprite2_color_LATCHED		<= sprite2_color;
     	sprite3_color_LATCHED		<= sprite3_color;
     	sprite4_color_LATCHED		<= sprite4_color;
	sprite5_color_LATCHED		<= sprite5_color;
   	sprite6_color_LATCHED		<= sprite6_color;
   	sprite7_color_LATCHED		<= sprite7_color;
   	sprite8_color_LATCHED		<= sprite8_color;
   	sprite9_color_LATCHED		<= sprite9_color;
	digit1_color_LATCHED		<= digit1_color;
	digit2_color_LATCHED		<= digit2_color;
	digit3_color_LATCHED		<= digit3_color;
   	isSprite1_LATCHED 		<= isSprite1;
   	isSprite2_LATCHED		<= isSprite2;
   	isSprite3_LATCHED		<= isSprite3;
	isSprite4_LATCHED		<= isSprite4;
	isSprite5_LATCHED		<= isSprite5;
   	isSprite6_LATCHED 		<= isSprite6;
   	isSprite7_LATCHED 		<= isSprite7;
   	isSprite8_LATCHED 		<= isSprite8;
   	isSprite9_LATCHED 		<= isSprite9;
	isDigit1_LATCHED		<= isDigit1;
	isDigit2_LATCHED		<= isDigit2;
	isDigit3_LATCHED		<= isDigit3;
	isScoreboard_LATCHED		<= isScoreboard;
	isFuelgauge_LATCHED		<= isFuelgauge;
	isIndicator_LATCHED		<= isIndicator;
   end

   always begin
      
      isSprite1 = 0;
      isSprite2 = 0;
      isSprite3 = 0;
      isSprite4 = 0;
      isSprite5 = 0;
      isSprite6 = 0;
      isSprite7 = 0;
      isSprite8 = 0;
      isSprite9 = 0;
      isScoreboard = 0;
      isFuelgauge = 0;
      isIndicator = 0;
      isDigit1 = 0;
      isDigit2 = 0;
      isDigit3 = 0;

      zero_address = 10'd0;
      one_address = 10'd0;
      two_address = 10'd0;
      three_address = 10'd0;
      four_address = 10'd0;
      five_address = 10'd0;
      six_address = 10'd0;
      seven_address = 10'd0;
      eight_address = 10'd0;
      nine_address = 10'd0;
      plane_address = 10'd0;
      planeleft_address = 10'd0;
      planeright_address = 10'd0;
      explosion_address = 10'd0;
      hotairballoon_address = 10'd0;
      shoot_address = 10'd0;
      chopper_address = 10'd0;
      reversechopper_address = 10'd0;
      battleship_address = 10'd0;
      reversebattleship_address = 10'd0;
      fuel_address = 10'd0;

      digit1_color = 4'b0;
      digit2_color = 4'b0;
      digit3_color = 4'b0;
      sprite1_color = 4'b0;
      sprite2_color = 4'b0;
      sprite3_color = 4'b0;
      sprite4_color = 4'b0;
      sprite5_color = 4'b0;
      sprite6_color = 4'b0;
      sprite7_color = 4'b0;
      sprite8_color = 4'b0;
      sprite9_color = 4'b0;
			
      sprite1_address = ((vcount - (sprite1_y[9:1]-16)) << 5) + (hcount[10:1] - (sprite1_x-16));
      sprite2_address = ((vcount - (sprite2_y[9:1]-16)) << 5) + (hcount[10:1] - (sprite2_x-16));
      sprite3_address = ((vcount - (sprite3_y[9:1]-16)) << 5) + (hcount[10:1] - (sprite3_x-16));
      sprite4_address = ((vcount - (sprite4_y[9:1]-16)) << 5) + (hcount[10:1] - (sprite4_x-16));
      sprite5_address = ((vcount - (sprite5_y[9:1]-16)) << 5) + (hcount[10:1] - (sprite5_x-16));
      sprite6_address = ((vcount - (sprite6_y[9:1]-16)) << 5) + (hcount[10:1] - (sprite6_x-16));
      sprite7_address = ((vcount - (sprite7_y[9:1]-16)) << 5) + (hcount[10:1] - (sprite7_x-16));
      sprite8_address = ((vcount - (sprite8_y[9:1]-16)) << 5) + (hcount[10:1] - (sprite8_x-16));
      sprite9_address = ((vcount - (sprite9_y[9:1]-16)) << 5) + (hcount[10:1] - (sprite9_x-16));
      scoreboard_address = ((vcount - (scoreboard_y[9:1]-16)) * 40) + (hcount[10:1] - (scoreboard_x-20));
      fuelgauge_address = (({2'b00,vcount} - ({3'b000, fuelgauge_y[9:1]}-20)) * 80) + ({2'b00,hcount[10:1]} - ({2'b00,fuelgauge_x}-40));
      indicator_address = ((vcount - (indicator_y[9:1]-16)) << 5) + (hcount[10:1] - (indicator_x - 16));
      digit1_address = ((vcount - (digit1_y[9:1]-16)) * 20) + (hcount[10:1] - (digit1_x-10));
      digit2_address = ((vcount - (digit2_y[9:1]-16)) * 20) + (hcount[10:1] - (digit2_x-10));
      digit3_address = ((vcount - (digit3_y[9:1]-16)) * 20) + (hcount[10:1] - (digit3_x-10));

      //assuming none of the images will be the same
      if(isSprite1_LATCHED) begin
	      case(sprite1_img)
			0: begin 
				plane_address = sprite1_address;
				sprite1_color = plane_out; //maybe latch the sprite colors
			   end
			1: begin
				chopper_address = sprite1_address;
				sprite1_color = chopper_out;
			   end
			2: begin
				battleship_address = sprite1_address;
				sprite1_color = battleship_out;
	   		   end
			3: begin
				fuel_address = sprite1_address;
				sprite1_color = fuel_out;
			   end
			4: begin
				shoot_address = sprite1_address;
				sprite1_color = shoot_out;
			   end
			5: begin 
				explosion_address = sprite1_address;
				sprite1_color = explosion_out; //maybe latch the sprite colors
			   end
			6: begin 
				hotairballoon_address = sprite1_address;
				sprite1_color = hotairballoon_out; //maybe latch the sprite colors
			   end
			7: begin 
				planeleft_address = sprite1_address;
				sprite1_color = planeleft_out; 
			   end			
			8: begin 
				planeright_address = sprite1_address;
				sprite1_color = planeright_out;
			   end 
			9: begin
				reversebattleship_address = sprite1_address;
				sprite1_color = reversebattleship_out;
	   		   end
			10: begin
				reversechopper_address = sprite1_address;
				sprite1_color = reversechopper_out;
			   end

	      endcase
      end

      if(isSprite2_LATCHED) begin
	      case(sprite2_img)
			0: begin 
				plane_address = sprite2_address;
				sprite2_color = plane_out;
			   end
			1: begin
				chopper_address = sprite2_address;
				sprite2_color = chopper_out;
			   end
			2: begin
				battleship_address = sprite2_address;
				sprite2_color = battleship_out;
			   end
			3: begin
				fuel_address = sprite2_address;
				sprite2_color = fuel_out;
			   end
			4: begin
				shoot_address = sprite2_address;
				sprite2_color = shoot_out;
			   end
			5: begin 
				explosion_address = sprite2_address;
				sprite2_color = explosion_out; //maybe latch the sprite colors
			   end
			6: begin 
				hotairballoon_address = sprite2_address;
				sprite2_color = hotairballoon_out; //maybe latch the sprite colors
			   end
			7: begin 
				planeleft_address = sprite2_address;
				sprite2_color = planeleft_out; 
			   end			
			8: begin 
				planeright_address = sprite2_address;
				sprite2_color = planeright_out; 
			   end
			9: begin
				reversebattleship_address = sprite2_address;
				sprite2_color = reversebattleship_out;
	   		   end
			10: begin
				reversechopper_address = sprite2_address;
				sprite2_color = reversechopper_out;
			   end
	      endcase
      end

      if(isSprite3_LATCHED) begin
	      case(sprite3_img)
			0: begin 
				plane_address = sprite3_address;
				sprite3_color = plane_out;
			   end
			1: begin
				chopper_address = sprite3_address;
				sprite3_color = chopper_out;
			   end
			2: begin
				battleship_address = sprite3_address;
				sprite3_color = battleship_out;
			   end
			3: begin
				fuel_address = sprite3_address;
				sprite3_color = fuel_out;
			   end
			4: begin
				shoot_address = sprite3_address;
				sprite3_color = shoot_out;
			   end
			5: begin 
				explosion_address = sprite3_address;
				sprite3_color = explosion_out; //maybe latch the sprite colors
			   end
			6: begin 
				hotairballoon_address = sprite3_address;
				sprite3_color = hotairballoon_out; //maybe latch the sprite colors
			   end
			7: begin 
				planeleft_address = sprite3_address;
				sprite3_color = planeleft_out; 
			   end			
			8: begin 
				planeright_address = sprite3_address;
				sprite3_color = planeright_out; 
 			   end
			9: begin
				reversebattleship_address = sprite3_address;
				sprite3_color = reversebattleship_out;
	   		   end
			10: begin
				reversechopper_address = sprite3_address;
				sprite3_color = reversechopper_out;
			   end
	      endcase
      end

      if(isSprite4_LATCHED) begin
	      case(sprite4_img)
			0: begin 
				plane_address = sprite4_address;
				sprite4_color = plane_out;
			   end
			1: begin
				chopper_address = sprite4_address;
				sprite4_color = chopper_out;
			   end
			2: begin
				battleship_address = sprite4_address;
				sprite4_color = battleship_out;
			   end
			3: begin
				fuel_address = sprite4_address;
				sprite4_color = fuel_out;
			   end
			4: begin
				shoot_address = sprite4_address;
				sprite4_color = shoot_out;
			   end
			5: begin 
				explosion_address = sprite4_address;
				sprite4_color = explosion_out; //maybe latch the sprite colors
			   end
			6: begin 
				hotairballoon_address = sprite4_address;
				sprite4_color = hotairballoon_out; //maybe latch the sprite colors
			   end
			7: begin 
				planeleft_address = sprite4_address;
				sprite4_color = planeleft_out; 
			   end			
			8: begin 
				planeright_address = sprite4_address;
				sprite4_color = planeright_out; 
			   end
			9: begin
				reversebattleship_address = sprite4_address;
				sprite4_color = reversebattleship_out;
	   		   end
			10: begin
				reversechopper_address = sprite4_address;
				sprite4_color = reversechopper_out;
			   end
	      endcase
      end

      if(isSprite5_LATCHED) begin
	      case(sprite5_img)
			0: begin 
				plane_address = sprite5_address;
				sprite5_color = plane_out;
			   end
			1: begin
				chopper_address = sprite5_address;
				sprite5_color = chopper_out;
			   end
			2: begin
				battleship_address = sprite5_address;
				sprite5_color = battleship_out;
			   end
			3: begin
				fuel_address = sprite5_address;
				sprite5_color = fuel_out;
			   end
			4: begin
				shoot_address = sprite5_address;
				sprite5_color = shoot_out;
			   end
			5: begin 
				explosion_address = sprite5_address;
				sprite5_color = explosion_out; //maybe latch the sprite colors
			   end
			6: begin 
				hotairballoon_address = sprite5_address;
				sprite5_color = hotairballoon_out; //maybe latch the sprite colors
			   end
			7: begin 
				planeleft_address = sprite5_address;
				sprite5_color = planeleft_out; 
			   end			
			8: begin 
				planeright_address = sprite5_address;
				sprite5_color = planeright_out; 
			   end
			9: begin
				reversebattleship_address = sprite5_address;
				sprite5_color = reversebattleship_out;
	   		   end
			10: begin
				reversechopper_address = sprite5_address;
				sprite5_color = reversechopper_out;
			   end
	      endcase
      end

      if(isSprite6_LATCHED) begin
	      case(sprite6_img)
			0: begin 
				plane_address = sprite6_address;
				sprite6_color = plane_out; //maybe latch the sprite colors
			   end
			1: begin
				chopper_address = sprite6_address;
				sprite6_color = chopper_out;
			   end
			2: begin
				battleship_address = sprite6_address;
				sprite6_color = battleship_out;
	   		   end
			3: begin
				fuel_address = sprite6_address;
				sprite6_color = fuel_out;
			   end
			4: begin
				shoot_address = sprite6_address;
				sprite6_color = shoot_out;
			   end
			5: begin 
				explosion_address = sprite6_address;
				sprite6_color = explosion_out; //maybe latch the sprite colors
			   end
			6: begin 
				hotairballoon_address = sprite6_address;
				sprite6_color = hotairballoon_out; //maybe latch the sprite colors
			   end
			7: begin 
				planeleft_address = sprite6_address;
				sprite6_color = planeleft_out; 
			   end			
			8: begin 
				planeright_address = sprite6_address;
				sprite6_color = planeright_out; 
			   end
			9: begin
				reversebattleship_address = sprite6_address;
				sprite6_color = reversebattleship_out;
	   		   end
			10: begin
				reversechopper_address = sprite6_address;
				sprite6_color = reversechopper_out;
			   end
	      endcase
      end

      if(isSprite7_LATCHED) begin
	      case(sprite7_img)
			0: begin 
				plane_address = sprite7_address;
				sprite7_color = plane_out; //maybe latch the sprite colors
			   end
			1: begin
				chopper_address = sprite7_address;
				sprite7_color = chopper_out;
			   end
			2: begin
				battleship_address = sprite7_address;
				sprite7_color = battleship_out;
	   		   end
			3: begin
				fuel_address = sprite7_address;
				sprite7_color = fuel_out;
			   end
			4: begin
				shoot_address = sprite7_address;
				sprite7_color = shoot_out;
			   end
			5: begin 
				explosion_address = sprite7_address;
				sprite7_color = explosion_out; //maybe latch the sprite colors
			   end
			6: begin 
				hotairballoon_address = sprite7_address;
				sprite7_color = hotairballoon_out; //maybe latch the sprite colors
			   end
			7: begin 
				planeleft_address = sprite7_address;
				sprite7_color = planeleft_out; 
			   end			
			8: begin 
				planeright_address = sprite7_address;
				sprite7_color = planeright_out; 
			   end
			9: begin
				reversebattleship_address = sprite7_address;
				sprite7_color = reversebattleship_out;
	   		   end
			10: begin
				reversechopper_address = sprite7_address;
				sprite7_color = reversechopper_out;
			   end
	      endcase
      end

      if(isSprite8_LATCHED) begin
	      case(sprite8_img)
			0: begin 
				plane_address = sprite8_address;
				sprite8_color = plane_out; //maybe latch the sprite colors
			   end
			1: begin
				chopper_address = sprite8_address;
				sprite8_color = chopper_out;
			   end
			2: begin
				battleship_address = sprite8_address;
				sprite8_color = battleship_out;
	   		   end
			3: begin
				fuel_address = sprite8_address;
				sprite8_color = fuel_out;
			   end
			4: begin
				shoot_address = sprite8_address;
				sprite8_color = shoot_out;
			   end
			5: begin 
				explosion_address = sprite8_address;
				sprite8_color = explosion_out; //maybe latch the sprite colors
			   end
			6: begin 
				hotairballoon_address = sprite8_address;
				sprite8_color = hotairballoon_out; //maybe latch the sprite colors
			   end
			7: begin 
				planeleft_address = sprite8_address;
				sprite8_color = planeleft_out; 
			   end			
			8: begin 
				planeright_address = sprite8_address;
				sprite8_color = planeright_out; 
			   end
			9: begin
				reversebattleship_address = sprite8_address;
				sprite8_color = reversebattleship_out;
	   		   end
			10: begin
				reversechopper_address = sprite8_address;
				sprite8_color = reversechopper_out;
			   end
	      endcase
      end

      if(isSprite9_LATCHED) begin
	      case(sprite9_img)
			0: begin 
				plane_address = sprite9_address;
				sprite9_color = plane_out; //maybe latch the sprite colors
			   end
			1: begin
				chopper_address = sprite9_address;
				sprite9_color = chopper_out;
			   end
			2: begin
				battleship_address = sprite9_address;
				sprite9_color = battleship_out;
	   		   end
			3: begin
				fuel_address = sprite9_address;
				sprite9_color = fuel_out;
			   end
			4: begin
				shoot_address = sprite9_address;
				sprite9_color = shoot_out;
			   end
			5: begin 
				explosion_address = sprite9_address;
				sprite9_color = explosion_out; //maybe latch the sprite colors
			   end
			6: begin 
				hotairballoon_address = sprite9_address;
				sprite9_color = hotairballoon_out; //maybe latch the sprite colors
			   end
			7: begin 
				planeleft_address = sprite9_address;
				sprite9_color = planeleft_out; 
			   end
			8: begin 
				planeright_address = sprite9_address;
				sprite9_color = planeright_out; 
			   end
			9: begin
				reversebattleship_address = sprite9_address;
				sprite9_color = reversebattleship_out;
	   		   end
			10: begin
				reversechopper_address = sprite9_address;
				sprite9_color = reversechopper_out;
			   end
	      endcase
      end

      if(isDigit1_LATCHED) begin
	      case(digit1_img)
			0: begin
				zero_address = digit1_address;
				digit1_color = zero_out;	
			end
			1: begin
				one_address = digit1_address;
				digit1_color = one_out;	
			end
			2: begin
				two_address = digit1_address;
				digit1_color = two_out;	
			end
			3: begin
				three_address = digit1_address;
				digit1_color = three_out;	
			end
			4: begin
				four_address = digit1_address;
				digit1_color = four_out;	
			end
			5: begin
				five_address = digit1_address;
				digit1_color = five_out;	
			end
			6: begin
				six_address = digit1_address;
				digit1_color = six_out;	
			end
			7: begin
				seven_address = digit1_address;
				digit1_color = seven_out;	
			end
			8: begin
				eight_address = digit1_address;
				digit1_color = eight_out;	
			end
			9: begin
				nine_address = digit1_address;
				digit1_color = nine_out;	
			end
	      endcase
      end
    
      if (isDigit2_LATCHED) begin
	      case(digit2_img)
			0: begin
				zero_address = digit2_address;
				digit2_color = zero_out;	
			end
			1: begin
				one_address = digit2_address;
				digit2_color = one_out;	
			end
			2: begin
				two_address = digit2_address;
				digit2_color = two_out;	
			end
			3: begin
				three_address = digit2_address;
				digit2_color = three_out;	
			end
			4: begin
				four_address = digit2_address;
				digit2_color = four_out;	
			end
			5: begin
				five_address = digit2_address;
				digit2_color = five_out;	
			end
			6: begin
				six_address = digit2_address;
				digit2_color = six_out;	
			end
			7: begin
				seven_address = digit2_address;
				digit2_color = seven_out;	
			end
			8: begin
				eight_address = digit2_address;
				digit2_color = eight_out;	
			end
			9: begin
				nine_address = digit2_address;
				digit2_color = nine_out;	
			end
	      endcase
      end

      if(isDigit3_LATCHED) begin
	      case(digit3_img)
			0: begin
				zero_address = digit3_address;
				digit3_color = zero_out;	
			end
			1: begin
				one_address = digit3_address;
				digit3_color = one_out;	
			end
			2: begin
				two_address = digit3_address;
				digit3_color = two_out;	
			end
			3: begin
				three_address = digit3_address;
				digit3_color = three_out;	
			end
			4: begin
				four_address = digit3_address;
				digit3_color = four_out;	
			end
			5: begin
				five_address = digit3_address;
				digit3_color = five_out;	
			end
			6: begin
				six_address = digit3_address;
				digit3_color = six_out;	
			end
			7: begin
				seven_address = digit3_address;
				digit3_color = seven_out;	
			end
			8: begin
				eight_address = digit3_address;
				digit3_color = eight_out;	
			end
			9: begin
				nine_address = digit3_address;
				digit3_color = nine_out;	
			end
	      endcase
      end

      if(sprite1_y[0]) begin
	      if((hcount[10:1] < sprite1_x + 16) && (hcount[10:1] >= sprite1_x - 16) && (vcount < sprite1_y[9:1]+16) && (vcount  >= sprite1_y[9:1]-16)) begin // check sprite1
			//pull its contents from memory
			isSprite1 = 1;
	      end
      end
      
      if(sprite2_y[0]) begin
	      if((hcount[10:1] < sprite2_x + 16) && (hcount[10:1] >= sprite2_x - 16) && (vcount < sprite2_y[9:1]+16) && (vcount  >= sprite2_y[9:1]-16)) begin // check sprite2
			//pull its contents from memory
			isSprite2 = 1;				
	      end
      end

      if(sprite3_y[0]) begin
	      if((hcount[10:1] < sprite3_x + 16) && (hcount[10:1] >= sprite3_x - 16) && (vcount < sprite3_y[9:1] + 16) && (vcount  >= sprite3_y[9:1] - 16)) begin // check sprite3
			//pull its contents from memory
			isSprite3 = 1;		
	      end
      end

      if(sprite4_y[0]) begin
	      if((hcount[10:1] < sprite4_x + 16) && (hcount[10:1] >= sprite4_x - 16) && (vcount < sprite4_y[9:1] + 16) && (vcount  >= sprite4_y[9:1] - 16)) begin // check sprite4
			//pull its contents from memory
			isSprite4 = 1;		
	      end
      end

      if(sprite5_y[0]) begin
	      if((hcount[10:1] < sprite5_x + 16) && (hcount[10:1] >= sprite5_x - 16) && (vcount < sprite5_y[9:1] + 16) && (vcount  >= sprite5_y[9:1] - 16)) begin // check sprite5
			//pull its contents from memory
			isSprite5 = 1;		
	      end
      end
      
      if(sprite6_y[0]) begin
	      if((hcount[10:1] < sprite6_x + 16) && (hcount[10:1] >= sprite6_x - 16) && (vcount < sprite6_y[9:1]+16) && (vcount  >= sprite6_y[9:1]-16)) begin // check sprite6
			//pull its contents from memory
			isSprite6 = 1;
	      end
      end

      if(sprite7_y[0]) begin
	      if((hcount[10:1] < sprite7_x + 16) && (hcount[10:1] >= sprite7_x - 16) && (vcount < sprite7_y[9:1]+16) && (vcount  >= sprite7_y[9:1]-16)) begin // check sprite7
			//pull its contents from memory
			isSprite7 = 1;
	      end
      end

      if(sprite8_y[0]) begin
	      if((hcount[10:1] < sprite8_x + 16) && (hcount[10:1] >= sprite8_x - 16) && (vcount < sprite8_y[9:1]+16) && (vcount  >= sprite8_y[9:1]-16)) begin // check sprite8
			//pull its contents from memory
			isSprite8 = 1;
	      end
      end

      if(sprite9_y[0]) begin
	      if((hcount[10:1] < sprite9_x + 16) && (hcount[10:1] >= sprite9_x - 16) && (vcount < sprite9_y[9:1]+16) && (vcount  >= sprite9_y[9:1]-16)) begin // check sprite9
			//pull its contents from memory
			isSprite9 = 1;
	      end
      end

      if(scoreboard_y[0]) begin
	      if((hcount[10:1] < scoreboard_x + 20) && (hcount[10:1] >= scoreboard_x - 20) && (vcount < scoreboard_y[9:1] + 16) && (vcount  >= scoreboard_y[9:1] - 16)) begin // check scoreboard
			//pull its contents from memory
			isScoreboard = 1;		
	      end
      end

      if(fuelgauge_y[0]) begin
	      if((hcount[10:1] < fuelgauge_x + 40) && (hcount[10:1] >= fuelgauge_x - 40) && (vcount < fuelgauge_y[9:1] + 20) && (vcount  >= fuelgauge_y[9:1] - 20)) begin // check fuelgauge
			//pull its contents from memory
			isFuelgauge = 1;		
	      end
      end

      if(indicator_y[0]) begin
	      if((hcount[10:1] < indicator_x + 16) && (hcount[10:1] >= indicator_x - 16) && (vcount < indicator_y[9:1] + 16) && (vcount  >= indicator_y[9:1] - 16)) begin // check indicator
			//pull its contents from memory
			isIndicator = 1;		
	      end
      end

      if(digit1_y[0]) begin
	      if((hcount[10:1] < digit1_x + 10) && (hcount[10:1] >= digit1_x - 10) && (vcount < digit1_y[9:1] + 16) && (vcount  >= digit1_y[9:1] - 16)) begin // check digit1
			//pull its contents from memory
			isDigit1 = 1;		
	      end
      end

      if(digit2_y[0]) begin
	      if((hcount[10:1] < digit2_x + 10) && (hcount[10:1] >= digit2_x - 10) && (vcount < digit2_y[9:1] + 16) && (vcount  >= digit2_y[9:1] - 16)) begin // check digit2
			//pull its contents from memory
			isDigit2 = 1;		
	      end
      end

      if(digit3_y[0]) begin
	      if((hcount[10:1] < digit3_x + 10) && (hcount[10:1] >= digit3_x - 10) && (vcount < digit3_y[9:1] + 16) && (vcount  >= digit3_y[9:1] - 16)) begin // check digit3
			//pull its contents from memory
			isDigit3 = 1;		
	      end
      end

      if(vcount < 80) begin
            current_background = 7; //gray
      end
      else if (boundary_3_LATCHED == 0 && boundary_4_LATCHED == 0) begin // 1 River
         if  (hcount[10:1] < boundary_1_LATCHED) begin
            current_background = 1; // green
	 end
         else if (hcount[10:1] < boundary_2_LATCHED) begin
            current_background = 2; // blue
         end
	 else begin
	    current_background = 1; // green
	 end
      end
      else begin // 2 Rivers
         if  (hcount[10:1] < boundary_1_LATCHED) begin
            current_background = 1; // green
	 end
         else if (hcount[10:1] < boundary_2_LATCHED) begin
            current_background = 2; // blue
	 end
         else if (hcount[10:1] < boundary_3_LATCHED) begin
            current_background = 1; // green
	 end
         else if (hcount[10:1] < boundary_4_LATCHED) begin
            current_background = 2; // blue
	 end
	 else begin
	    current_background = 1; // green
	 end
      end

      //priority encoding of sprites
      if (isScoreboard) begin
		current_color_NONSPRITE = scoreboard_out;
      end
      else if (isFuelgauge && fuelgauge_out != 0) begin
		current_color_NONSPRITE = fuelgauge_out;
      end
      else if (isIndicator && indicator_out != 0) begin
		current_color_NONSPRITE = indicator_out;
      end
      else if (isDigit1 && digit1_color_LATCHED != 0) begin
		current_color_NONSPRITE = digit1_color_LATCHED;
      end
      else if (isDigit2 && digit2_color_LATCHED != 0) begin
		current_color_NONSPRITE = digit2_color_LATCHED;
      end
      else if (isDigit3 && digit3_color_LATCHED != 0) begin
		current_color_NONSPRITE = digit3_color_LATCHED;
      end
      else begin
		current_color_NONSPRITE = 0;
      end


      if (isSprite1 && sprite1_color_LATCHED != 0) begin
		current_color = sprite1_color_LATCHED;
      end
      else if(isSprite2 && sprite2_color_LATCHED != 0) begin
		current_color = sprite2_color_LATCHED;
      end
      else if(isSprite3 && sprite3_color_LATCHED != 0) begin
		current_color = sprite3_color_LATCHED;
      end
      else if(isSprite4 && sprite4_color_LATCHED != 0) begin
		current_color = sprite4_color_LATCHED;
      end
      else if(isSprite5 && sprite5_color_LATCHED != 0) begin
		current_color = sprite5_color_LATCHED;
      end
      else if(isSprite6 && sprite6_color_LATCHED != 0) begin
		current_color = sprite6_color_LATCHED;
      end
      else if(isSprite7 && sprite7_color_LATCHED != 0) begin
		current_color = sprite7_color_LATCHED;
      end
      else if(isSprite8 && sprite8_color_LATCHED != 0) begin
		current_color = sprite8_color_LATCHED;
      end
      else if(isSprite9 && sprite9_color_LATCHED != 0) begin
		current_color = sprite9_color_LATCHED;
      end
      else begin
		current_color = 0;
      end
   end


	reg [11:0] counter;

	reg	[12:0]  address1;
	wire	[15:0]  q1;

	shoot audio1(.address(address1), .clock(clk), .q(q1)); //1653

	
	reg	[12:0]  address2;
	wire	[15:0]  q2;
	hit  audio2(.address(address2), .clock(clk), .q(q2));//1235

	reg	[12:0]  address3;
	wire	[15:0]  q3;
	bomb audio3(.address(address3), .clock(clk), .q(q3));//5832


	logic shootRegisterPrev;
	logic hitRegisterPrev;
	logic explodeRegisterPrev;

	logic playShoot;
	logic playHit;
	logic playExplode;

	
   	logic	[15:0]  q1_intermediate;
   	logic	[15:0]  q2_intermediate;
   	logic	[15:0]  q3_intermediate;

	always_ff @(posedge clk) begin

		sample_data_l = q1_intermediate + q2_intermediate + q3_intermediate;
		sample_data_r = q1_intermediate + q2_intermediate + q3_intermediate;

		if(reset) begin
			sample_valid_l <= 0; 
			sample_valid_r <= 0;
			
			address3 <= 0;
			address2 <= 0;
			address1 <= 0;
	
			shootRegisterPrev <= 0;
			hitRegisterPrev <= 0;
			explodeRegisterPrev <= 0;
		
		end

		else if(left_chan_ready == 1 && right_chan_ready == 1) begin 

			if(!shootRegister) shootRegisterPrev <= 0;
			if(!hitRegister) hitRegisterPrev <= 0;
			if(!explodeRegister) explodeRegisterPrev <= 0;
			
			//shoot logic
			if(shootRegister == 1 && shootRegisterPrev == 0) begin
				playShoot <= 1;
				shootRegisterPrev <= 1;
			end			

			if(playShoot) begin
				address1 <= address1 + 1;
				sample_valid_l <= 1;
				sample_valid_r <= 1;
				q1_intermediate <= q1;				
			end
			
			if(address1 >= 1650) begin
				address1 <= 0;
				playShoot <= 0;
			end


			//hit logic
			if(hitRegister == 1 && hitRegisterPrev == 0) begin
				playHit <= 1;
				hitRegisterPrev <= 1;
			end			
			
			if(playHit) begin
				address2 <= address2 + 1;
				sample_valid_l <= 1;
				sample_valid_r <= 1;
				q2_intermediate <= q2;				
			end
			
			if(address2 >= 1230) begin
				address2 <= 0;
				playHit <= 0;
			end

			//explode logic
			if(explodeRegister == 1 && explodeRegisterPrev == 0) begin
				playExplode <= 1;
				explodeRegisterPrev <= 1;
			end			
			
			if(playExplode) begin
				address3 <= address3 + 1;
				sample_valid_l <= 1;
				sample_valid_r <= 1;
				q3_intermediate <= q3;				
			end
			
			if(address3 >= 5830) begin
				address3 <= 0;
				playExplode <= 0;
			end

		end
		else begin
			sample_valid_l <= 0;
			sample_valid_r <= 0;
		end
	end
	       
endmodule



module vga_counters(
 input logic 	     clk50, reset,
 output logic [10:0] hcount,  // hcount[10:1] is pixel column
 output logic [9:0]  vcount,  // vcount[9:0] is pixel row
 output logic 	     VGA_CLK, VGA_HS, VGA_VS, VGA_BLANK_n, VGA_SYNC_n);

/*
 * 640 X 480 VGA timing for a 50 MHz clock: one pixel every other cycle
 * 
 * HCOUNT 1599 0             1279       1599 0
 *             _______________              ________
 * ___________|    Video      |____________|  Video
 * 
 * 
 * |SYNC| BP |<-- HACTIVE -->|FP|SYNC| BP |<-- HACTIVE
 *       _______________________      _____________
 * |____|       VGA_HS          |____|
 */
   // Parameters for hcount
   parameter HACTIVE      = 11'd 1280,
             HFRONT_PORCH = 11'd 32,
             HSYNC        = 11'd 192,
             HBACK_PORCH  = 11'd 96,   
             HTOTAL       = HACTIVE + HFRONT_PORCH + HSYNC +
                            HBACK_PORCH; // 1600
   
   // Parameters for vcount
   parameter VACTIVE      = 10'd 480,
             VFRONT_PORCH = 10'd 10,
             VSYNC        = 10'd 2,
             VBACK_PORCH  = 10'd 33,
             VTOTAL       = VACTIVE + VFRONT_PORCH + VSYNC +
                            VBACK_PORCH; // 525

   logic endOfLine;
   
   always_ff @(posedge clk50 or posedge reset)
     if (reset)          hcount <= 0;
     else if (endOfLine) hcount <= 0;
     else  	         hcount <= hcount + 11'd 1;

   assign endOfLine = hcount == HTOTAL - 1;
       
   logic endOfField;
   
   always_ff @(posedge clk50 or posedge reset)
     if (reset)          vcount <= 0;
     else if (endOfLine)
       if (endOfField)   vcount <= 0;
       else              vcount <= vcount + 10'd 1;

   assign endOfField = vcount == VTOTAL - 1;

   // Horizontal sync: from 0x520 to 0x5DF (0x57F)
   // 101 0010 0000 to 101 1101 1111
   assign VGA_HS = !( (hcount[10:8] == 3'b101) &
		      !(hcount[7:5] == 3'b111));
   assign VGA_VS = !( vcount[9:1] == (VACTIVE + VFRONT_PORCH) / 2);

   assign VGA_SYNC_n = 1'b0; // For putting sync on the green signal; unused
   
   // Horizontal active: 0 to 1279     Vertical active: 0 to 479
   // 101 0000 0000  1280	       01 1110 0000  480
   // 110 0011 1111  1599	       10 0000 1100  524
   assign VGA_BLANK_n = !( hcount[10] & (hcount[9] | hcount[8]) ) &
			!( vcount[9] | (vcount[8:5] == 4'b1111) );

   /* VGA_CLK is 25 MHz
    *             __    __    __
    * clk50    __|  |__|  |__|
    *        
    *             _____       __
    * hcount[0]__|     |_____|
    */
   assign VGA_CLK = hcount[0]; // 25 MHz clock: rising edge sensitive
   
endmodule


