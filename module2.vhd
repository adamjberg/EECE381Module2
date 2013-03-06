LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;
USE ieee.std_logic_unsigned.all;
ENTITY module2 IS
PORT (
	CLOCK_50 : IN STD_LOGIC;
	KEY : IN STD_LOGIC_VECTOR(3 DOWNTO 0);
	SW : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
	DRAM_CLK, DRAM_CKE : OUT STD_LOGIC;
	DRAM_ADDR : OUT STD_LOGIC_VECTOR(11 DOWNTO 0);
	DRAM_BA_0, DRAM_BA_1 : BUFFER STD_LOGIC;
	DRAM_CS_N, DRAM_CAS_N, DRAM_RAS_N, DRAM_WE_N : OUT STD_LOGIC;
	DRAM_DQ : INOUT STD_LOGIC_VECTOR(15 DOWNTO 0);
	DRAM_UDQM, DRAM_LDQM : BUFFER STD_LOGIC;
	SRAM_ADDR : OUT STD_LOGIC_VECTOR(17 downto 0);
	SRAM_DQ : INOUT STD_LOGIC_VECTOR(15 downto 0);
	SRAM_LB_N, SRAM_UB_N, SRAM_CE_N, SRAM_OE_N, SRAM_WE_N : OUT STD_LOGIC;
	VGA_R : OUT STD_LOGIC_VECTOR(9 downto 0);
	VGA_G : OUT STD_LOGIC_VECTOR(9 downto 0);
	VGA_B : OUT STD_LOGIC_VECTOR(9 downto 0);
	VGA_CLK, VGA_BLANK, VGA_HS, VGA_VS, VGA_SYNC : OUT STD_LOGIC;
	SD_CLK : OUT STD_LOGIC;
	SD_DAT3, SD_DAT, SD_CMD : INOUT STD_LOGIC;
	I2C_SDAT : INOUT STD_LOGIC;
	I2C_SCLK : OUT STD_LOGIC;
	AUD_XCK : OUT STD_LOGIC;
	CLOCK_27 : IN STD_LOGIC;
	AUD_ADCDAT, AUD_ADCLRCK, AUD_BCLK, AUD_DACLRCK : IN STD_LOGIC;
	AUD_DACDAT : OUT STD_LOGIC;
	LCD_DATA: inout std_LOGIC_VECTOR(7 downto 0);
	LCD_ON, LCD_BLON, LCD_EN,  LCD_RS,  LCD_RW : out std_LOGIC;
	HEX0, HEX1, HEX2 ,HEX3, HEX4 :out std_LOGIC_VECTOR(7 downto 0);
	HEX5,HEX6,HEX7 : out std_LOGIC_VECTOR(7 downto 0) := "11111111";
	LEDG : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);
	LEDR : OUT STD_LOGIC_VECTOR(17 DOWNTO 0);
	PS2_CLK : INOUT STD_LOGIC;
	PS2_DAT : INOUT STD_LOGIC;
	UART_RXD : IN STD_LOGIC;
	UART_TXD : OUT STD_LOGIC
	);
END module2;

ARCHITECTURE struct OF module2 IS
component nios_system is
port (
            clk_clk                     : in    std_logic                     := 'X';             -- clk
            reset_reset_n               : in    std_logic                     := 'X';             -- reset_n
            sdram_wire_addr             : out   std_logic_vector(11 downto 0);                    -- addr
            sdram_wire_ba               : out   std_logic_vector(1 downto 0);                     -- ba
            sdram_wire_cas_n            : out   std_logic;                                        -- cas_n
            sdram_wire_cke              : out   std_logic;                                        -- cke
            sdram_wire_cs_n             : out   std_logic;                                        -- cs_n
            sdram_wire_dq               : inout std_logic_vector(15 downto 0) := (others => 'X'); -- dq
            sdram_wire_dqm              : out   std_logic_vector(1 downto 0);                     -- dqm
            sdram_wire_ras_n            : out   std_logic;                                        -- ras_n
            sdram_wire_we_n             : out   std_logic;                                        -- we_n
            sram_interface_DQ           : inout std_logic_vector(15 downto 0) := (others => 'X'); -- DQ
            sram_interface_ADDR         : out   std_logic_vector(17 downto 0);                    -- ADDR
            sram_interface_LB_N         : out   std_logic;                                        -- LB_N
            sram_interface_UB_N         : out   std_logic;                                        -- UB_N
            sram_interface_CE_N         : out   std_logic;                                        -- CE_N
            sram_interface_OE_N         : out   std_logic;                                        -- OE_N
            sram_interface_WE_N         : out   std_logic;                                        -- WE_N
            vga_extern_CLK              : out   std_logic;                                        -- CLK
            vga_extern_HS               : out   std_logic;                                        -- HS
            vga_extern_VS               : out   std_logic;                                        -- VS
            vga_extern_BLANK            : out   std_logic;                                        -- BLANK
            vga_extern_SYNC             : out   std_logic;                                        -- SYNC
            vga_extern_R                : out   std_logic_vector(9 downto 0);                     -- R
            vga_extern_G                : out   std_logic_vector(9 downto 0);                     -- G
            vga_extern_B                : out   std_logic_vector(9 downto 0);                     -- B
            sdram_clk_clk               : out   std_logic;                                        -- clk
            leds_export                 : out   std_logic_vector(7 downto 0);                     -- export
            sd_card_b_SD_cmd            : inout std_logic                     := 'X';             -- b_SD_cmd
            sd_card_b_SD_dat            : inout std_logic                     := 'X';             -- b_SD_dat
            sd_card_b_SD_dat3           : inout std_logic                     := 'X';             -- b_SD_dat3
            sd_card_o_SD_clock          : out   std_logic;                                        -- o_SD_clock
            keys_export                 : in    std_logic_vector(3 downto 0)  := (others => 'X'); -- export
            switches_export             : in    std_logic_vector(7 downto 0)  := (others => 'X'); -- export
            audio_and_video_config_SDAT : inout std_logic                     := 'X';             -- SDAT
            audio_and_video_config_SCLK : out   std_logic;                                        -- SCLK
            audio_clk_1_clk               : out   std_logic;                                        -- clk
            clk_in_secondary_1_clk        : in    std_logic                     := 'X';             -- clk
            audio_ADCDAT                : in    std_logic                     := 'X';             -- ADCDAT
            audio_ADCLRCK               : in    std_logic                     := 'X';             -- ADCLRCK
            audio_BCLK                  : in    std_logic                     := 'X';             -- BCLK
            audio_DACDAT                : out   std_logic;                                        -- DACDAT
            audio_DACLRCK               : in    std_logic                     := 'X';              -- DACLRCK
				ledrs_export                 : out   std_logic_vector(17 downto 0);
				character_lcd_0_external_interface_DATA : inout std_LOGIC_VECTOR(7 dowNTO 0);
				character_lcd_0_external_interface_ON: out std_LOGIC;
				character_lcd_0_external_interface_BLON: out std_LOGIC;
			   character_lcd_0_external_interface_EN: out std_LOGIC;
				character_lcd_0_external_interface_RS: out std_LOGIC;
				character_lcd_0_external_interface_RW: out std_LOGIC;
				hex0_ex_export, hex1_ex_export, hex2_ex_export, hex3_ex_export, hex4_ex_export  :out std_LOGIC_VECTOR(7 downto 0);
				ps2_0_external_interface_CLK : inout std_logic;
				ps2_0_external_interface_DAT : inout std_logic;
            rs232_RXD                               : in    std_logic                     := 'X';             -- RXD
            rs232_TXD                               : out   std_logic                                         -- TXD		
				);
end component nios_system;
SIGNAL DQM : STD_LOGIC_VECTOR(1 DOWNTO 0);
SIGNAL BA : STD_LOGIC_VECTOR(1 DOWNTO 0);
BEGIN
DRAM_BA_0 <= BA(0);
DRAM_BA_1 <= BA(1);
DRAM_UDQM <= DQM(1);
DRAM_LDQM <= DQM(0);
niosII : component nios_system
port map (
            clk_clk          => CLOCK_50,          --        clk.clk
            reset_reset_n    => SW(7),    --      reset.reset_n
				sdram_clk_clk => DRAM_CLK,
            sdram_wire_addr  => DRAM_ADDR,  -- sdram_wire.addr
            sdram_wire_ba    => BA,    --           .ba
            sdram_wire_cas_n => DRAM_CAS_N, --           .cas_n
            sdram_wire_cke   => DRAM_CKE,   --           .cke
            sdram_wire_cs_n  => DRAM_CS_N,  --           .cs_n
            sdram_wire_dq    => DRAM_DQ,    --           .dq
            sdram_wire_dqm   => DQM,   --           .dqm
            sdram_wire_ras_n => DRAM_RAS_N, --           .ras_n
            sdram_wire_we_n  => DRAM_WE_N,   --           .we_n
				sram_interface_DQ   => SRAM_DQ,   -- sram_interface.DQ
            sram_interface_ADDR => SRAM_ADDR, --               .ADDR
            sram_interface_LB_N => SRAM_LB_N, --               .LB_N
            sram_interface_UB_N => SRAM_UB_N, --               .UB_N
            sram_interface_CE_N => SRAM_CE_N, --               .CE_N
            sram_interface_OE_N => SRAM_OE_N, --               .OE_N
            sram_interface_WE_N => SRAM_WE_N, --               .WE_N
            vga_extern_CLK      => VGA_CLK,      --     vga_extern.CLK
            vga_extern_HS       => VGA_HS,       --               .HS
            vga_extern_VS       => VGA_VS,       --               .VS
            vga_extern_BLANK    => VGA_BLANK,    --               .BLANK
            vga_extern_SYNC     => VGA_SYNC,     --               .SYNC
            vga_extern_R        => VGA_R,        --               .R
            vga_extern_G        => VGA_G,        --               .G
            vga_extern_B        => VGA_B,         --               .B
				sd_card_b_SD_cmd    => SD_CMD,    --        sd_card.b_SD_cmd
            sd_card_b_SD_dat    => SD_DAT,    --               .b_SD_dat
            sd_card_b_SD_dat3   => SD_DAT3,   --               .b_SD_dat3
            sd_card_o_SD_clock  => SD_CLK,   --               .o_SD_clock
				keys_export         => KEY,         --           keys.export
            switches_export     => SW,      --       switches.export
				audio_and_video_config_SDAT => I2C_SDAT, -- audio_and_video_config.SDAT
            audio_and_video_config_SCLK => I2C_SCLK, --                       .SCLK
            audio_clk_1_clk               => AUD_XCK,               --              audio_clk.clk
            clk_in_secondary_1_clk        => CLOCK_27,        --       clk_in_secondary.clk
            audio_ADCDAT                => AUD_ADCDAT,                --                  audio.ADCDAT
            audio_ADCLRCK               => AUD_ADCLRCK,               --                       .ADCLRCK
            audio_BCLK                  => AUD_BCLK,                  --                       .BCLK
            audio_DACDAT                => AUD_DACDAT,                --                       .DACDAT
            audio_DACLRCK               => AUD_DACLRCK,                --                       .DACLRCK	
				character_lcd_0_external_interface_DATA => LCD_DATA,
				character_lcd_0_external_interface_BLON => LCD_BLON,
				character_lcd_0_external_interface_ON => LCD_ON,
				character_lcd_0_external_interface_EN => LCD_EN,
				character_lcd_0_external_interface_RS => LCD_RS,
				character_lcd_0_external_interface_RW => LCD_RW,
				leds_export			 => LEDG(7 DOWNTO 0),
				hex0_ex_export => HEX0 (7 DOWNTO 0),
				hex1_ex_export => HEX1 (7 DOWNTO 0),
				hex2_ex_export => HEX2 (7 DOWNTO 0),
				hex3_ex_export => HEX3 (7 DOWNTO 0),
				hex4_ex_export => HEX4 (7 DOWNTO 0),
				ledrs_export => LEDR(17 downto 0),
				ps2_0_external_interface_CLK => PS2_CLK,
				ps2_0_external_interface_DAT => PS2_DAT,
            rs232_RXD => UART_RXD,        --                              rs232.RXD
            rs232_TXD => UART_TXD         --                                   .TXD
        );
end struct;
