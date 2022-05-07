/*
 * Avalon memory-mapped peripheral that generates VGA
 *
 * Stephen A. Edwards
 * Columbia University
 */
`include "../ROM/plane_ROM.v"
`include "../ROM/chopper_ROM.v"
`include "../ROM/battleship_ROM.v"

module vga_ball(input logic        clk,
	        input logic 	   reset,
		input logic [15:0] writedata,
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

   logic [3:0] 	   current_color_LATCHED;
   logic [3:0]	   current_background_LATCHED;

   logic [3:0] 	   sprite1_color;
   logic [3:0]     sprite2_color;
   logic [3:0]     sprite3_color;

   // last bit of y positions indicates whether sprite is onscreen

   logic [9:0]	   sprite1_x;
   logic [9:0]	   sprite1_y;
   logic [4:0]	   sprite1_img; //which sprite is this?

   logic [9:0]	   sprite2_x;
   logic [9:0]	   sprite2_y;
   logic [4:0]	   sprite2_img; //which sprite is this?

   logic [9:0]	   sprite3_x;
   logic [9:0]	   sprite3_y;
   logic [4:0]	   sprite3_img; //which sprite is this?

   logic	   isSprite;
   logic	   isMusic;
   logic [1:0]	   whichClip;

	
   vga_counters counters(.clk50(clk), .*);
	
   //ROM Wires

   logic [9:0]	   sprite1_address;
   logic [9:0]	   sprite2_address;
   logic [9:0]     sprite3_address;
   logic	   isSprite1;
   logic	   isSprite2;
   logic 	   isSprite3;

   logic [3:0]     plane_out;
   logic [9:0]     plane_address;

   logic [3:0]     chopper_out;
   logic [9:0] 	   chopper_address;

   logic [3:0]     battleship_out;
   logic [9:0]     battleship_address;

   plane_ROM 	plane_ROM(.address(plane_address), .clock(clk),.q(plane_out));
   chopper_ROM 	chopper_ROM(.address(chopper_address),.clock(clk),.q(chopper_out));	
   battleship_ROM batteship_ROM(.address(battleship_address), .clock(clk), .q(battleship_out));

   assign isSprite = isSprite1 || isSprite2 || isSprite3;

   always_ff @(posedge clk) begin
     if (chipselect && write)
       case (address)

	 0 : boundary_1 <= writedata[9:0];
	 1 : boundary_2 <= writedata[9:0];
	 2 : boundary_3 <= writedata[9:0];
	 3 : boundary_4 <= writedata[9:0];
	 4 : sprite1_x <= writedata[9:0];
	 5 : sprite1_y <= writedata[9:0];
	 6 : sprite1_img <= writedata[4:0];
	 7 : sprite2_x <= writedata[9:0];
	 8 : sprite2_y <= writedata[9:0];
	 9 : sprite2_img <= writedata[4:0];
	 10 : sprite3_x <= writedata[9:0]; 
	 11 : sprite3_y <= writedata[9:0];
	 12 : sprite3_img <= writedata[4:0];

       endcase
   end

   always_ff @(posedge clk) begin
	if (reset) begin
	   {VGA_R, VGA_G, VGA_B} <= {8'h00, 8'h00, 8'h00}; //Black
        end 
        else if(isSprite && current_color_LATCHED != 0) begin
		case(current_color_LATCHED)

			0: {VGA_R, VGA_G, VGA_B} <= {8'hff, 8'hff, 8'hff}; //White
			1: {VGA_R, VGA_G, VGA_B} <= {8'h00, 8'hff, 8'h00}; //Green
			2: {VGA_R, VGA_G, VGA_B} <= {8'h00, 8'h00, 8'hff}; //Blue
			3: {VGA_R, VGA_G, VGA_B} <= {8'hff, 8'h00, 8'h00}; //Red
			4: {VGA_R, VGA_G, VGA_B} <= {8'hff, 8'hff, 8'h00}; //Yellow
			5: {VGA_R, VGA_G, VGA_B} <= {8'h00, 8'hff, 8'hff}; //Cyan
			6: {VGA_R, VGA_G, VGA_B} <= {8'hff, 8'h00, 8'hff}; //Magenta
			7: {VGA_R, VGA_G, VGA_B} <= {8'h80, 8'h80, 8'h80}; //Gray
			8: {VGA_R, VGA_G, VGA_B} <= {8'h00, 8'h00, 8'h00}; //Black
			9: {VGA_R, VGA_G, VGA_B} <= {8'hff, 8'hff, 8'h00}; //White
			10: {VGA_R, VGA_G, VGA_B} <= {8'hff, 8'hff, 8'hff}; //White
			11: {VGA_R, VGA_G, VGA_B} <= {8'hff, 8'hff, 8'hff}; //White
			12: {VGA_R, VGA_G, VGA_B} <= {8'hff, 8'hff, 8'hff}; //White
			13: {VGA_R, VGA_G, VGA_B} <= {8'hff, 8'hff, 8'hff}; //White
			14: {VGA_R, VGA_G, VGA_B} <= {8'hff, 8'hff, 8'hff}; //White
			15: {VGA_R, VGA_G, VGA_B} <= {8'hff, 8'hff, 8'hff}; //White

		endcase
	end
	else begin
		case(current_background_LATCHED)

			0: {VGA_R, VGA_G, VGA_B} <= {8'hff, 8'hff, 8'hff}; //White
			1: {VGA_R, VGA_G, VGA_B} <= {8'h00, 8'hff, 8'h00}; //Green
			2: {VGA_R, VGA_G, VGA_B} <= {8'h00, 8'h00, 8'hff}; //Blue
			3: {VGA_R, VGA_G, VGA_B} <= {8'hff, 8'h00, 8'h00}; //Red
			4: {VGA_R, VGA_G, VGA_B} <= {8'hff, 8'hff, 8'h00}; //Yellow
			5: {VGA_R, VGA_G, VGA_B} <= {8'h00, 8'hff, 8'hff}; //Cyan
			6: {VGA_R, VGA_G, VGA_B} <= {8'hff, 8'h00, 8'hff}; //Magenta
			7: {VGA_R, VGA_G, VGA_B} <= {8'h80, 8'h80, 8'h80}; //Gray
			8: {VGA_R, VGA_G, VGA_B} <= {8'h00, 8'h00, 8'h00}; //Black
			9: {VGA_R, VGA_G, VGA_B} <= {8'hff, 8'hff, 8'h00}; //White
			10: {VGA_R, VGA_G, VGA_B} <= {8'hff, 8'hff, 8'hff}; //White
			11: {VGA_R, VGA_G, VGA_B} <= {8'hff, 8'hff, 8'hff}; //White
			12: {VGA_R, VGA_G, VGA_B} <= {8'hff, 8'hff, 8'hff}; //White
			13: {VGA_R, VGA_G, VGA_B} <= {8'hff, 8'hff, 8'hff}; //White
			14: {VGA_R, VGA_G, VGA_B} <= {8'hff, 8'hff, 8'hff}; //White
			15: {VGA_R, VGA_G, VGA_B} <= {8'hff, 8'hff, 8'hff}; //White

		endcase
	end
   end
   
   always @(posedge clk) begin
	current_color_LATCHED <= current_color;
	current_background_LATCHED <= current_background;
   end

   always begin
      
      isSprite1 = 0;
      isSprite2 = 0;
      isSprite3 = 0;

      if(sprite1_y[0]) begin //whether the sprite is onscreen
	      if((hcount[10:1] < sprite1_x + 16) && (hcount[10:1] >= sprite1_x - 16) && (vcount < sprite1_y[9:1]+16) && (vcount  >= sprite1_y[9:1]-16)) begin // check sprite1
			//pull its contents from memory
			sprite1_address = 32 * (vcount - (sprite1_y[9:1]-16)) + (hcount[10:1]-(sprite1_x-16));
			case(sprite1_img)
				0: begin 
					plane_address = sprite1_address;
					sprite1_color = plane_out;
				   end
				1: begin
					chopper_address = sprite1_address;
					sprite1_color = chopper_out;
				   end
				2: begin
					battleship_address = sprite1_address;
					sprite1_color = battleship_out;
			   	   end
			endcase
			isSprite1 = 1; // we are in range of sprite 1
	
	      end
      end
      
      if(sprite2_y[0]) begin
	      if((hcount[10:1] < sprite2_x + 16) && (hcount[10:1] >= sprite2_x - 16) && (vcount < sprite2_y[9:1]+16) && (vcount  >= sprite2_y[9:1]-16)) begin // check sprite2
			//pull its contents from memory
			sprite2_address = 32 * (vcount - (sprite2_y[9:1]-16)) + (hcount[10:1]-(sprite2_x-16));
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
			endcase
			isSprite2 = 1;				
	      
	      end
      end

      if(sprite3_y[0]) begin
	      if((hcount[10:1] < sprite3_x + 16) && (hcount[10:1] >= sprite3_x - 16) && (vcount < sprite3_y[9:1] + 16) && (vcount  >= sprite3_y[9:1] - 16)) begin // check sprite3
			//pull its contents from memory
			sprite3_address = 32 * (vcount - (sprite3_y[9:1]-16)) + (hcount[10:1] - (sprite3_x-16));
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
			endcase
			isSprite3 = 1;				
	      
	      end
      end

      if (boundary_3 == 0 && boundary_4 == 0) begin // 1 River
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

      //priority encoding of sprites
      if (isSprite1 && sprite1_color != 0) begin
		current_color = sprite1_color;
      end
      else if(isSprite2 && sprite2_color != 0) begin
		current_color = sprite2_color;
      end
      else if(isSprite3 && sprite3_color != 0) begin
		current_color = sprite3_color;
      end
      else begin
		current_color = 0;
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
