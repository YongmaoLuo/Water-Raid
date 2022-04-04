/*
 * Avalon memory-mapped peripheral that generates VGA
 *
 * Stephen A. Edwards
 * Columbia University
 */
`include "../ROM/plane_ROM.v"

module vga(	input logic        clk,
	        input logic 	   reset,
		input logic [15:0]  writedata,
		input logic 	   write,
		input 		   chipselect,
		input logic [5:0]  address,

		output logic [7:0] VGA_R, VGA_G, VGA_B,
		output logic 	   VGA_CLK, VGA_HS, VGA_VS,
		                   VGA_BLANK_n,
		output logic 	   VGA_SYNC_n);

   logic [10:0]	   hcount;
   logic [9:0]     vcount;
   
   logic [9:0]	   boundary_1;
   logic [9:0]	   boundary_2;
   logic [9:0]	   boundary_3;
   logic [9:0]	   boundary_4;

   logic [3:0]	   current_color; //for sprites
   logic [3:0]	   current_background; //for background

   // last bit of y positions indicates whether sprite is onscreen

   logic [9:0]	   plane_x;
   logic [9:0]	   plane_y;

   logic [9:0]	   enemy_1_x;
   logic [9:0]	   enemy_1_y;

   logic [9:0]	   enemy_2_x;
   logic [9:0]	   enemy_2_y;

   logic [9:0]	   enemy_3_x;
   logic [9:0]	   enemy_3_y;

   logic [9:0]	   enemy_4_x;
   logic [9:0]	   enemy_4_y;

   logic [9:0]	   enemy_5_x;
   logic [9:0]	   enemy_5_y;

   logic [9:0]	   enemy_6_x;
   logic [9:0]	   enemy_6_y;

   logic [9:0]	   enemy_7_x;
   logic [9:0]	   enemy_7_y;

   logic [9:0]	   enemy_8_x;
   logic [9:0]	   enemy_8_y;

   logic [9:0]	   bullet_x;
   logic [9:0]	   bullet_y;

   logic [9:0]	   explosion_x;
   logic [9:0]	   explosion_y;

   logic [3:0]	   score_digit_1;
   logic [3:0]	   score_digit_2;
   logic [3:0]	   score_digit_3;
   logic [3:0]	   score_digit_4;
   logic [3:0]	   score_digit_5;

   logic	   isMusic;
   logic	   isSprite;
   logic [1:0]	   whichClip;

	
   vga_counters counters(.clk50(clk), .*);
	
   //ROM Wires

   logic [9:0]	   plane_address;
   logic [3:0]     plane_out;

   plane_ROM plane_ROM(.address(plane_address), .clock(clk),.q(plane_out));
/*
   logic [9:0]	   enemy1_address;
   logic [3:0]     enemy1_out;

   ROM enemy1(.address(enemy1_address), .clock(clk),.q(enemy1_out));

   logic [9:0]	   enemy2_address;
   logic [3:0]     enemy2_out;

   ROM enemy2(.address(enemy2_address), .clock(clk),.q(enemy2_out));

   logic [9:0]	   enemy3_address;
   logic [3:0]     enemy3_out;

   ROM enemy3(.address(enemy3_address), .clock(clk),.q(enemy3_out));

   logic [9:0]	   enemy4_address;
   logic [3:0]     enemy4_out;

   ROM enemy4(.address(enemy4_address), .clock(clk),.q(enemy4_out));

   logic [9:0]	   enemy5_address;
   logic [3:0]     enemy5_out;

   ROM enemy5(.address(enemy5_address), .clock(clk),.q(enemy5_out));

   logic [9:0]	   enemy6_address;
   logic [3:0]     enemy6_out;

   ROM enemy6(.address(enemy6_address), .clock(clk),.q(enemy6_out));

   logic [9:0]	   enemy7_address;
   logic [3:0]     enemy7_out;

   ROM enemy7(.address(enemy7_address), .clock(clk),.q(enemy7_out));

   logic [9:0]	   enemy8_address;
   logic [3:0]     enemy8_out;

   ROM enemy8(.address(enemy8_address), .clock(clk),.q(enemy8_out));
*/
	

   always_ff @(posedge clk) begin
     if (reset) begin
	{VGA_R, VGA_G, VGA_B} = {8'h00, 8'h00, 8'h00}; //Black
     end else if (chipselect && write)
       case (address)

	 6'h1 : boundary_1 <= writedata[9:0];
	 6'h2 : boundary_2 <= writedata[9:0];
	 6'h3 : boundary_3 <= writedata[9:0];
	 6'h4 : boundary_4 <= writedata[9:0];
	 6'h5 : plane_x <= writedata[9:0]; 
	 6'h6 : plane_y <= writedata[9:0];
   	 6'h7 : enemy_1_x <= writedata[9:0];
   	 6'h8 : enemy_1_y <= writedata[9:0];
         6'h9 : enemy_2_x <= writedata[9:0];
         6'h10 : enemy_2_y <= writedata[9:0];
         6'h11 : enemy_3_x <= writedata[9:0];
         6'h12 : enemy_3_y <= writedata[9:0];
         6'h13 : enemy_4_x <= writedata[9:0];
         6'h14 : enemy_4_y <= writedata[9:0];
         6'h15 : enemy_5_x <= writedata[9:0];
         6'h16 : enemy_5_y <= writedata[9:0];
         6'h17 : enemy_6_x <= writedata[9:0];
         6'h18 : enemy_6_y <= writedata[9:0];
         6'h19 : enemy_7_x <= writedata[9:0];
         6'h20 : enemy_7_y <= writedata[9:0];
         6'h21 : enemy_8_x <= writedata[9:0];
         6'h22 : enemy_8_y <= writedata[9:0];
         6'h23 : bullet_x <= writedata[9:0];
         6'h24 : bullet_y <= writedata[9:0];
         6'h25 : explosion_x <= writedata[9:0];
         6'h26 : explosion_y <= writedata[9:0];
         6'h27 : score_digit_1 <= writedata[3:0];
         6'h28 : score_digit_2 <= writedata[3:0];
         6'h29 : score_digit_3 <= writedata[3:0];
         6'h30 : score_digit_4 <= writedata[3:0];
         6'h31 : score_digit_5 <= writedata[3:0];
         6'h32 : isMusic <= writedata[0];
         6'h33 : whichClip <= writedata[1:0];

       endcase
   end

   always_ff @(posedge clk) begin
	if(isSprite && current_color != 0) begin
		case(current_color)

			1: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'hff, 8'h00}; //Green
			2: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'h00, 8'hff}; //Blue
			3: {VGA_R, VGA_G, VGA_B} = {8'hff, 8'h00, 8'h00}; //Red
			4: {VGA_R, VGA_G, VGA_B} = {8'hff, 8'hff, 8'h00}; //Yellow
			5: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'hff, 8'hff}; //Cyan
			6: {VGA_R, VGA_G, VGA_B} = {8'hff, 8'h00, 8'hff}; //Magenta
			7: {VGA_R, VGA_G, VGA_B} = {8'h80, 8'h80, 8'h80}; //Gray
			8: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'h00, 8'h00}; //Black
			9: {VGA_R, VGA_G, VGA_B} = {8'hff, 8'hff, 8'h00}; //White
			10: {VGA_R, VGA_G, VGA_B} = {8'hff, 8'hff, 8'hff}; //White
			11: {VGA_R, VGA_G, VGA_B} = {8'hff, 8'hff, 8'hff}; //White
			12: {VGA_R, VGA_G, VGA_B} = {8'hff, 8'hff, 8'hff}; //White
			13: {VGA_R, VGA_G, VGA_B} = {8'hff, 8'hff, 8'hff}; //White
			14: {VGA_R, VGA_G, VGA_B} = {8'hff, 8'hff, 8'hff}; //White
			15: {VGA_R, VGA_G, VGA_B} = {8'hff, 8'hff, 8'hff}; //White

		endcase
	end
	else begin
		case(current_background)

			0: {VGA_R, VGA_G, VGA_B} = {8'hff, 8'hff, 8'hff}; //White
			1: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'hff, 8'h00}; //Green
			2: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'h00, 8'hff}; //Blue
			3: {VGA_R, VGA_G, VGA_B} = {8'hff, 8'h00, 8'h00}; //Red
			4: {VGA_R, VGA_G, VGA_B} = {8'hff, 8'hff, 8'h00}; //Yellow
			5: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'hff, 8'hff}; //Cyan
			6: {VGA_R, VGA_G, VGA_B} = {8'hff, 8'h00, 8'hff}; //Magenta
			7: {VGA_R, VGA_G, VGA_B} = {8'h80, 8'h80, 8'h80}; //Gray
			8: {VGA_R, VGA_G, VGA_B} = {8'h00, 8'h00, 8'h00}; //Black
			9: {VGA_R, VGA_G, VGA_B} = {8'hff, 8'hff, 8'h00}; //White
			10: {VGA_R, VGA_G, VGA_B} = {8'hff, 8'hff, 8'hff}; //White
			11: {VGA_R, VGA_G, VGA_B} = {8'hff, 8'hff, 8'hff}; //White
			12: {VGA_R, VGA_G, VGA_B} = {8'hff, 8'hff, 8'hff}; //White
			13: {VGA_R, VGA_G, VGA_B} = {8'hff, 8'hff, 8'hff}; //White
			14: {VGA_R, VGA_G, VGA_B} = {8'hff, 8'hff, 8'hff}; //White
			15: {VGA_R, VGA_G, VGA_B} = {8'hff, 8'hff, 8'hff}; //White

		endcase
	end
   end

   always_comb begin
      if (plane_y[0]) begin
	      if((hcount[10:1] - plane_x < 16) && (hcount[10:1] - plane_x > -16) && (vcount - plane_y[9:1] < 16) && (vcount - plane_y[9:1] > -16)) begin // check plane sprite
			//pull its contents from memory
			plane_address = 32 * (vcount - plane_y[9:1]) + hcount[10:1]-(plane_x-16);
			current_color = plane_out;
			isSprite = 1;					
	      end
      end
/*
      if (enemy_1_y[0]) begin
	      else if((hcount[10:1] - enemy_1_x < 16) && (hcount[10:1] - enemy_1_x > 16) && (vcount[9:1] - enemy_1_y < 16) && (vcount - enemy_1_y[9:1] > 16)) begin // check enemy 1

	      end
      end
      if (enemy_2_y[0]) begin
	      else if((hcount[10:1] - enemy_2_x < 16) && (hcount[10:1] - enemy_2_x > 16) && (vcount[9:1] - enemy_2_y < 16) && (vcount - enemy_2_y[9:1] > 16)) begin // check enemy 2

	      end
      end
      if(enemy_3_y[0]) begin
	      else if((hcount[10:1] - enemy_3_x < 16) && (hcount[10:1] - enemy_3_x > 16) && (vcount[9:1] - enemy_3_y < 16) && (vcount - enemy_3_y[9:1] > 16)) begin // check enemy 3

	      end
      end
      if(enemy_4_y[0]) begin
	      else if((hcount[10:1] - enemy_4_x < 16) && (hcount[10:1] - enemy_4_x > 16) && (vcount[9:1] - enemy_4_y < 16) && (vcount - enemy_4_y[9:1] > 16)) begin // check enemy 4

	      end
      end
      if(enemy_5_y[0]) begin
	      else if((hcount[10:1] - enemy_5_x < 16) && (hcount[10:1] - enemy_5_x > 16) && (vcount[9:1] - enemy_5_y < 16) && (vcount - enemy_5_y[9:1] > 16)) begin // check enemy 5

	      end
      end
      if(enemy_6_y[0]) begin
	      else if((hcount[10:1] - enemy_6_x < 16) && (hcount[10:1] - enemy_6_x > 16) && (vcount[9:1] - enemy_6_y < 16) && (vcount - enemy_6_y[9:1] > 16)) begin // check enemy 6

	      end
      end
      if(enemy_7_y[0]) begin
	      else if((hcount[10:1] - enemy_7_x < 16) && (hcount[10:1] - enemy_7_x > 16) && (vcount[9:1] - enemy_7_y < 16) && (vcount - enemy_7_y[9:1] > 16)) begin // check enemy 7

	      end
      end
      if(enemy_8_y[0]) begin
	      else if((hcount[10:1] - enemy_8_x < 16) && (hcount[10:1] - enemy_8_x > 16) && (vcount[9:1] - enemy_8_y < 16) && (vcount - enemy_8_y[9:1] > 16)) begin // check enemy 8

	      end
      end
*/
      if (boundary_3 == 0 && boundary_4 == 0) begin // 1 River
	 isSprite = 0;					
         if  (hcount[10:1] < boundary_1) begin
            current_background = 1; // green
	 end
         else if (hcount[10:1] < boundary_2) begin
            current_background = 2; // blue
         end
	 else begin
	    current_background = 1; // green
	 end
      end
      else begin // 2 Rivers
	 isSprite = 0;					
         if  (hcount[10:1] < boundary_1) begin
            current_background = 1; // green
	 end
         else if (hcount[10:1] < boundary_2) begin
            current_background = 2; // blue
	 end
         else if (hcount[10:1] < boundary_3) begin
            current_background = 1; // green
	 end
         else if (hcount[10:1] < boundary_4) begin
            current_background = 2; // blue
	 end
	 else begin
	    current_background = 1; // green
	 end
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
