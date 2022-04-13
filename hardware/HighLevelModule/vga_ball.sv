/*
 * Avalon memory-mapped peripheral that generates VGA
 *
 * Stephen A. Edwards
 * Columbia University
 */
`include "../ROM/plane_ROM.v"
`include "../ROM/chopper_ROM.v"


module vga_ball(input logic        clk,
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

   logic [9:0]	   sprite1_x;
   logic [9:0]	   sprite1_y;
   logic [4:0]	   sprite1_img; //which sprite is this?

   logic [9:0]	   sprite2_x;
   logic [9:0]	   sprite2_y;
   logic [4:0]	   sprite2_img; //which sprite is this?

   logic	   isSprite;
   logic	   isMusic;
   logic [1:0]	   whichClip;

	
   vga_counters counters(.clk50(clk), .*);
	
   //ROM Wires

   logic [9:0]	   sprite1_address;
   logic [9:0]	   sprite2_address;
   logic	   isSprite1;
   logic	   isSprite2;


   logic [3:0]     plane_out;
   logic [9:0]     plane_address;


   logic [3:0]     chopper_out;
   logic [9:0] 	   chopper_address;

   plane_ROM 	plane_ROM(.address(plane_address), .clock(clk),.q(plane_out));
   chopper_ROM 	chopper_ROM(.address(chopper_address),.clock(clk),.q(chopper_out));	

   assign isSprite = isSprite1 || isSprite2;

   always_ff @(posedge clk) begin
     if (chipselect && write)
       case (address)

	 6'h0 : boundary_1 <= writedata[9:0];
	 6'h1 : boundary_2 <= writedata[9:0];
	 6'h2 : boundary_3 <= writedata[9:0];
	 6'h3 : boundary_4 <= writedata[9:0];
	 6'h4 : sprite1_x <= writedata[9:0];
	 6'h5 : sprite1_y <= writedata[9:0];
	 6'h6 : sprite1_img <= writedata[4:0];
	 6'h7 : sprite2_x <= writedata[9:0];
	 6'h8 : sprite2_y <= writedata[9:0];
	 6'h9 : sprite2_img <= writedata[4:0];

       endcase
   end

   always_ff @(posedge clk) begin
	if (reset) begin
	   {VGA_R, VGA_G, VGA_B} <= {8'h00, 8'h00, 8'h00}; //Black
        end 
        else if(isSprite && current_color != 0) begin
		case(current_color)

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
		case(current_background)

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

   always begin
      
      isSprite1 = 0;
      isSprite2 = 0;


      if(sprite1_y[0]) begin
	      if((hcount[10:1] < sprite1_x + 16) && (hcount[10:1] > sprite1_x - 16) && (vcount < sprite1_y[9:1]+16) && (vcount  > sprite1_y[9:1]-16)) begin // check sprite1
			//pull its contents from memory
			sprite1_address = 32 * (vcount - (sprite1_y[9:1]-16)) + (hcount[10:1]-(sprite1_x-16));
			case(sprite1_img)
				0: plane_address = sprite1_address;
				1: chopper_address = sprite1_address;
			endcase
			isSprite1 = 1;
	
	      
	      end
      end
      
      if(sprite2_y[0]) begin
	      if((hcount[10:1] < sprite2_x + 16) && (hcount[10:1] > sprite2_x - 16) && (vcount < sprite2_y[9:1]+16) && (vcount  > sprite2_y[9:1]-16)) begin // check sprite2
			//pull its contents from memory
			sprite2_address = 32 * (vcount - (sprite2_y[9:1]-16)) + (hcount[10:1]-(sprite2_x-16));
			case(sprite2_img)
				0: plane_address = sprite2_address;
				1: chopper_address = sprite2_address;
			endcase
			isSprite2 = 1;				
	      
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
      if (isSprite1) begin
		case(sprite1_img) begin
			0: if(plane_out != 0) current_color = plane_out;
			1: if(chopper_out != 0) current_color = chopper_out;
		endcase
      end
      else if(isSprite2) begin
		case(sprite2_img) begin
			0: if(plane_out != 0) current_color = plane_out;
			1: if(chopper_out != 0) current_color = chopper_out;
		endcase
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
