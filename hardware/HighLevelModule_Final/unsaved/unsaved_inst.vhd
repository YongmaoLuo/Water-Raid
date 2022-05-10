	component unsaved is
		port (
			vga_ball_0_clock_clk   : in std_logic := 'X'; -- clk
			audio_0_clk_clk        : in std_logic := 'X'; -- clk
			vga_ball_0_reset_reset : in std_logic := 'X'; -- reset
			audio_0_reset_reset    : in std_logic := 'X'  -- reset
		);
	end component unsaved;

	u0 : component unsaved
		port map (
			vga_ball_0_clock_clk   => CONNECTED_TO_vga_ball_0_clock_clk,   -- vga_ball_0_clock.clk
			audio_0_clk_clk        => CONNECTED_TO_audio_0_clk_clk,        --      audio_0_clk.clk
			vga_ball_0_reset_reset => CONNECTED_TO_vga_ball_0_reset_reset, -- vga_ball_0_reset.reset
			audio_0_reset_reset    => CONNECTED_TO_audio_0_reset_reset     --    audio_0_reset.reset
		);

