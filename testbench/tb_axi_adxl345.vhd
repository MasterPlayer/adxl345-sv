library IEEE;
    use IEEE.STD_LOGIC_1164.ALL;
    use ieee.std_logic_unsigned.all;
    use ieee.std_logic_arith.all;

entity tb_axi_adxl345 is 
end tb_axi_adxl345;



architecture tb_axi_adxl345_arch of tb_axi_adxl345 is 

    constant N_BYTES            :           integer     := 4                                    ;

    constant clock_period   :           time                                          := 10 ns              ;

    signal i                :           integer                                       := 0                  ;

    signal  clk       :       std_logic                        := '0'               ;
    signal  reset     :       std_logic                        := '1'               ;

    constant  S_AXI_LITE_DEV_DATA_WIDTH   :           integer                         := 32                           ;
    constant  S_AXI_LITE_DEV_ADDR_WIDTH   :           integer                         := 6                            ;
    constant  DEFAULT_DEVICE_ADDRESS      :           std_Logic_vector ( 6 downto 0 ) := "1010011"                    ;
    constant  DEFAULT_REQUEST_INTERVAL    :           integer                         :=  1000                        ;
    constant  S_AXI_LITE_CFG_DATA_WIDTH   :           integer                         :=  32                          ;
    constant  S_AXI_LITE_CFG_ADDR_WIDTH   :           integer                         := 6                            ;
    constant  CLK_PERIOD                  :           integer                         := 100000000                    ;

   
    component axi_adxl345 
        generic (
            S_AXI_LITE_DEV_DATA_WIDTH   :           integer                         := 32                           ;
            S_AXI_LITE_DEV_ADDR_WIDTH   :           integer                         := 6                            ;
            DEFAULT_DEVICE_ADDRESS      :           std_Logic_vector ( 6 downto 0 ) := "1010011"                    ;
            DEFAULT_REQUEST_INTERVAL    :           integer                         :=  1000                        ;
            S_AXI_LITE_CFG_DATA_WIDTH   :           integer                         :=  32                          ;
            S_AXI_LITE_CFG_ADDR_WIDTH   :           integer                         := 6                            ;
            CLK_PERIOD                  :           integer                         := 100000000                    ;
            RESET_DURATION              :           integer                         := 1000                         
        ); 
        port (
            CLK                         :   in      std_logic                                                       ;
            RESETN                      :   in      std_logic                                                       ;
            S_AXI_LITE_CFG_AWADDR       :   in      std_logic_vector (    S_AXI_LITE_CFG_ADDR_WIDTH-1 downto 0 )    ;
            S_AXI_LITE_CFG_AWPROT       :   in      std_logic_vector (                              2 downto 0 )    ;
            S_AXI_LITE_CFG_AWVALID      :   in      std_logic                                                       ;
            S_AXI_LITE_CFG_AWREADY      :   out     std_logic                                                       ;
            S_AXI_LITE_CFG_WDATA        :   in      std_logic_vector (    S_AXI_LITE_CFG_DATA_WIDTH-1 downto 0 )    ;
            S_AXI_LITE_CFG_WSTRB        :   in      std_logic_vector ((S_AXI_LITE_CFG_DATA_WIDTH/8)-1 downto 0 )    ;
            S_AXI_LITE_CFG_WVALID       :   in      std_logic                                                       ;
            S_AXI_LITE_CFG_WREADY       :   out     std_logic                                                       ;
            S_AXI_LITE_CFG_BRESP        :   out     std_logic_vector (                              1 downto 0 )    ;
            S_AXI_LITE_CFG_BVALID       :   out     std_logic                                                       ;
            S_AXI_LITE_CFG_BREADY       :   in      std_logic                                                       ;
            S_AXI_LITE_CFG_ARADDR       :   in      std_logic_vector (    S_AXI_LITE_CFG_ADDR_WIDTH-1 downto 0 )    ;
            S_AXI_LITE_CFG_ARPROT       :   in      std_logic_vector (                              2 downto 0 )    ;
            S_AXI_LITE_CFG_ARVALID      :   in      std_logic                                                       ;
            S_AXI_LITE_CFG_ARREADY      :   out     std_logic                                                       ;
            S_AXI_LITE_CFG_RDATA        :   out     std_logic_vector (    S_AXI_LITE_CFG_DATA_WIDTH-1 downto 0 )    ;
            S_AXI_LITE_CFG_RRESP        :   out     std_logic_vector (                              1 downto 0 )    ;
            S_AXI_LITE_CFG_RVALID       :   out     std_logic                                                       ;
            S_AXI_LITE_CFG_RREADY       :   in      std_logic                                                       ;
            S_AXI_LITE_DEV_AWADDR       :   in      std_logic_vector (    S_AXI_LITE_DEV_ADDR_WIDTH-1 downto 0 )    ;
            S_AXI_LITE_DEV_AWPROT       :   in      std_logic_vector (                              2 downto 0 )    ;
            S_AXI_LITE_DEV_AWVALID      :   in      std_logic                                                       ;
            S_AXI_LITE_DEV_AWREADY      :   out     std_logic                                                       ;
            S_AXI_LITE_DEV_WDATA        :   in      std_logic_vector (    S_AXI_LITE_DEV_DATA_WIDTH-1 downto 0 )    ;
            S_AXI_LITE_DEV_WSTRB        :   in      std_logic_vector ((S_AXI_LITE_DEV_DATA_WIDTH/8)-1 downto 0 )    ;
            S_AXI_LITE_DEV_WVALID       :   in      std_logic                                                       ;
            S_AXI_LITE_DEV_WREADY       :   out     std_logic                                                       ;
            S_AXI_LITE_DEV_BRESP        :   out     std_logic_vector (                              1 downto 0 )    ;
            S_AXI_LITE_DEV_BVALID       :   out     std_logic                                                       ;
            S_AXI_LITE_DEV_BREADY       :   in      std_logic                                                       ;
            S_AXI_LITE_DEV_ARADDR       :   in      std_logic_vector (    S_AXI_LITE_DEV_ADDR_WIDTH-1 downto 0 )    ;
            S_AXI_LITE_DEV_ARPROT       :   in      std_logic_vector (                              2 downto 0 )    ;
            S_AXI_LITE_DEV_ARVALID      :   in      std_logic                                                       ;
            S_AXI_LITE_DEV_ARREADY      :   out     std_logic                                                       ;
            S_AXI_LITE_DEV_RDATA        :   out     std_logic_vector (    S_AXI_LITE_DEV_DATA_WIDTH-1 downto 0 )    ;
            S_AXI_LITE_DEV_RRESP        :   out     std_logic_vector (                              1 downto 0 )    ;
            S_AXI_LITE_DEV_RVALID       :   out     std_logic                                                       ;
            S_AXI_LITE_DEV_RREADY       :   in      std_logic                                                       ;
            M_AXIS_TDATA                :   out     std_logic_vector (                              7 downto 0 )    ;
            M_AXIS_TKEEP                :   out     std_logic_vector (                              0 downto 0 )    ;
            M_AXIS_TUSER                :   out     std_logic_vector (                              7 downto 0 )    ;
            M_AXIS_TVALID               :   out     std_logic                                                       ;
            M_AXIS_TLAST                :   out     std_logic                                                       ;
            M_AXIS_TREADY               :   in      std_logic                                                       ;
            S_AXIS_TDATA                :   in      std_logic_vector (                              7 downto 0 )    ;
            S_AXIS_TKEEP                :   in      std_logic_vector (                              0 downto 0 )    ;
            S_AXIS_TUSER                :   in      std_logic_vector (                              7 downto 0 )    ;
            S_AXIS_TVALID               :   in      std_logic                                                       ;
            S_AXIS_TLAST                :   in      std_logic                                                       ;
            S_AXIS_TREADY               :   out     std_logic                                                       ;
            ADXL_INTERRUPT              :   in      std_Logic                                                       ;
            ADXL_IRQ                    :   out     std_logic                                                       
        );
    end component;

    signal  awaddr                      :           std_logic_vector (                              7 downto 0 ) := (others => '0')     ;
    signal  awprot                      :           std_logic_vector (                              2 downto 0 ) := (others => '0')     ;
    signal  awvalid                     :           std_logic                                                    := '0'                 ;
    signal  awready                     :           std_logic                                                                           ;
    signal  wdata                       :           std_logic_vector (    S_AXI_LITE_CFG_DATA_WIDTH-1 downto 0 ) := (others => '0')     ;
    signal  wstrb                       :           std_logic_vector ((S_AXI_LITE_CFG_DATA_WIDTH/8)-1 downto 0 ) := (others => '0')     ;
    signal  wvalid                      :           std_logic                                                    := '0'                 ;
    signal  wready                      :           std_logic                                                                           ;
    signal  bresp                       :           std_logic_vector (                              1 downto 0 )                        ;
    signal  bvalid                      :           std_logic                                                                           ;
    signal  bready                      :           std_logic                                                    := '0'                 ;
    signal  araddr                      :           std_logic_vector (                              7 downto 0 ) := (others => '0')     ;
    signal  arprot                      :           std_logic_vector (                              2 downto 0 ) := (others => '0')     ;
    signal  arvalid                     :           std_logic                                                    := '0'                 ;
    signal  arready                     :           std_logic                                                                           ;
    signal  rdata                       :           std_logic_vector (    S_AXI_LITE_CFG_DATA_WIDTH-1 downto 0 )                        ;
    signal  rresp                       :           std_logic_vector (                              1 downto 0 )                        ;
    signal  rvalid                      :           std_logic                                                                           ;
    signal  rready                      :           std_logic                                                    := '0'                 ;
    signal  DEV_AWADDR                  :           std_logic_vector (                              7 downto 0 ) := (others => '0')     ;
    signal  DEV_AWPROT                  :           std_logic_vector (                              2 downto 0 ) := (others => '0')     ;
    signal  DEV_AWVALID                 :           std_logic                                                    := '0'                 ;
    signal  DEV_AWREADY                 :           std_logic                                                                           ;
    signal  DEV_WDATA                   :           std_logic_vector (    S_AXI_LITE_DEV_DATA_WIDTH-1 downto 0 ) := (others => '0')     ;
    signal  DEV_WSTRB                   :           std_logic_vector ((S_AXI_LITE_DEV_DATA_WIDTH/8)-1 downto 0 ) := (others => '0')     ;
    signal  DEV_WVALID                  :           std_logic                                                    := '0'                 ;
    signal  DEV_WREADY                  :           std_logic                                                                           ;
    signal  DEV_BRESP                   :           std_logic_vector (                              1 downto 0 )                        ;
    signal  DEV_BVALID                  :           std_logic                                                                           ;
    signal  DEV_BREADY                  :           std_logic                                                    := '0'                 ;
    signal  DEV_ARADDR                  :           std_logic_vector (    S_AXI_LITE_DEV_ADDR_WIDTH-1 downto 0 ) := (others => '0')     ;
    signal  DEV_ARPROT                  :           std_logic_vector (                              2 downto 0 ) := (others => '0')     ;
    signal  DEV_ARVALID                 :           std_logic                                                    := '0'                 ;
    signal  DEV_ARREADY                 :           std_logic                                                                           ;
    signal  DEV_RDATA                   :           std_logic_vector (    S_AXI_LITE_DEV_DATA_WIDTH-1 downto 0 )                        ;
    signal  DEV_RRESP                   :           std_logic_vector (                              1 downto 0 )                        ;
    signal  DEV_RVALID                  :           std_logic                                                                           ;
    signal  DEV_RREADY                  :           std_logic                                                    := '0'                 ;
    signal  M_AXIS_TDATA                :           std_logic_vector (                              7 downto 0 )                        ;
    signal  M_AXIS_TKEEP                :           std_logic_vector (                              0 downto 0 )                        ;
    signal  M_AXIS_TUSER                :           std_logic_vector (                              7 downto 0 )                        ;
    signal  M_AXIS_TVALID               :           std_logic                                                                           ;
    signal  M_AXIS_TLAST                :           std_logic                                                                           ;
    signal  M_AXIS_TREADY               :           std_logic                                                    := '0'                 ;
    signal  S_AXIS_TDATA                :           std_logic_vector (                              7 downto 0 ) := (others => '0')     ;
    signal  S_AXIS_TKEEP                :           std_logic_vector (                              0 downto 0 ) := (others => '0')     ;
    signal  S_AXIS_TUSER                :           std_logic_vector (                              7 downto 0 ) := (others => '0')     ;
    signal  S_AXIS_TVALID               :           std_logic                                                    := '0'                 ;
    signal  S_AXIS_TLAST                :           std_logic                                                    := '0'                 ;
    signal  S_AXIS_TREADY               :           std_logic                                                                           ;

    signal  ADXL_INTERRUPT              :           std_Logic                                                    := '0'                 ;

    constant clock_period_time : time := 10 ns;

begin 

    CLK <= not CLK after clock_period_time/2;

    i_processing : process(CLK)
    begin
        if CLK'event AND CLK = '1' then 
            i <= i + 1;
        end if;
    end process;

    reset <= '1' when i < 5 else '0';

    axi_adxl345_inst : axi_adxl345 
        generic map (
            S_AXI_LITE_DEV_DATA_WIDTH   =>  32                                  ,
            S_AXI_LITE_DEV_ADDR_WIDTH   =>  6                                   ,
            DEFAULT_DEVICE_ADDRESS      =>  "1010011"                           ,
            DEFAULT_REQUEST_INTERVAL    =>  1000                                ,
            S_AXI_LITE_CFG_DATA_WIDTH   =>  32                                  ,
            S_AXI_LITE_CFG_ADDR_WIDTH   =>  6                                   ,
            CLK_PERIOD                  =>  100000000                           ,
            RESET_DURATION              =>  50                                 
        )
        port map (
            CLK                         =>  CLK                                 ,
            RESETN                      =>  not(RESET)                          ,
            S_AXI_LITE_CFG_AWADDR       =>  awaddr( 5 downto 0 )                ,
            S_AXI_LITE_CFG_AWPROT       =>  awprot                              ,
            S_AXI_LITE_CFG_AWVALID      =>  awvalid                             ,
            S_AXI_LITE_CFG_AWREADY      =>  awready                             ,
            S_AXI_LITE_CFG_WDATA        =>  wdata                               ,
            S_AXI_LITE_CFG_WSTRB        =>  wstrb                               ,
            S_AXI_LITE_CFG_WVALID       =>  wvalid                              ,
            S_AXI_LITE_CFG_WREADY       =>  wready                              ,
            S_AXI_LITE_CFG_BRESP        =>  bresp                               ,
            S_AXI_LITE_CFG_BVALID       =>  bvalid                              ,
            S_AXI_LITE_CFG_BREADY       =>  bready                              ,
            S_AXI_LITE_CFG_ARADDR       =>  araddr( 5 downto 0 )                ,
            S_AXI_LITE_CFG_ARPROT       =>  arprot                              ,
            S_AXI_LITE_CFG_ARVALID      =>  arvalid                             ,
            S_AXI_LITE_CFG_ARREADY      =>  arready                             ,
            S_AXI_LITE_CFG_RDATA        =>  rdata                               ,
            S_AXI_LITE_CFG_RRESP        =>  rresp                               ,
            S_AXI_LITE_CFG_RVALID       =>  rvalid                              ,
            S_AXI_LITE_CFG_RREADY       =>  rready                              ,
            S_AXI_LITE_DEV_AWADDR       =>  DEV_AWADDR( 5 downto 0 )            ,
            S_AXI_LITE_DEV_AWPROT       =>  DEV_AWPROT                          ,
            S_AXI_LITE_DEV_AWVALID      =>  DEV_AWVALID                         ,
            S_AXI_LITE_DEV_AWREADY      =>  DEV_AWREADY                         ,
            S_AXI_LITE_DEV_WDATA        =>  DEV_WDATA                           ,
            S_AXI_LITE_DEV_WSTRB        =>  DEV_WSTRB                           ,
            S_AXI_LITE_DEV_WVALID       =>  DEV_WVALID                          ,
            S_AXI_LITE_DEV_WREADY       =>  DEV_WREADY                          ,
            S_AXI_LITE_DEV_BRESP        =>  DEV_BRESP                           ,
            S_AXI_LITE_DEV_BVALID       =>  DEV_BVALID                          ,
            S_AXI_LITE_DEV_BREADY       =>  DEV_BREADY                          ,
            S_AXI_LITE_DEV_ARADDR       =>  DEV_ARADDR                          ,
            S_AXI_LITE_DEV_ARPROT       =>  DEV_ARPROT                          ,
            S_AXI_LITE_DEV_ARVALID      =>  DEV_ARVALID                         ,
            S_AXI_LITE_DEV_ARREADY      =>  DEV_ARREADY                         ,
            S_AXI_LITE_DEV_RDATA        =>  DEV_RDATA                           ,
            S_AXI_LITE_DEV_RRESP        =>  DEV_RRESP                           ,
            S_AXI_LITE_DEV_RVALID       =>  DEV_RVALID                          ,
            S_AXI_LITE_DEV_RREADY       =>  DEV_RREADY                          ,
            M_AXIS_TDATA                =>  M_AXIS_TDATA                        ,
            M_AXIS_TKEEP                =>  M_AXIS_TKEEP                        ,
            M_AXIS_TUSER                =>  M_AXIS_TUSER                        ,
            M_AXIS_TVALID               =>  M_AXIS_TVALID                       ,
            M_AXIS_TLAST                =>  M_AXIS_TLAST                        ,
            M_AXIS_TREADY               =>  M_AXIS_TREADY                       ,
            S_AXIS_TDATA                =>  S_AXIS_TDATA                        ,
            S_AXIS_TKEEP                =>  S_AXIS_TKEEP                        ,
            S_AXIS_TUSER                =>  S_AXIS_TUSER                        ,
            S_AXIS_TVALID               =>  S_AXIS_TVALID                       ,
            S_AXIS_TLAST                =>  S_AXIS_TLAST                        ,
            S_AXIS_TREADY               =>  S_AXIS_TREADY                       ,
            ADXL_INTERRUPT              =>  ADXL_INTERRUPT                      ,
            ADXL_IRQ                    =>  open                                 
        );
    
    M_AXIS_TREADY <= '1';
    

    s_axis_processing : process(CLK)
    begin
        if CLK'event aND CLK = '1' then 
            case i is 

                when 1100   => S_AXIS_TDATA <= x"40"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '1';

                when 2000   => S_AXIS_TDATA <= x"12"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 2100   => S_AXIS_TDATA <= x"23"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 2200   => S_AXIS_TDATA <= x"34"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 2300   => S_AXIS_TDATA <= x"45"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 2400   => S_AXIS_TDATA <= x"56"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 2500   => S_AXIS_TDATA <= x"67"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '1';

                when others => S_AXIS_TDATA <= S_AXIS_TDATA; S_AXIS_TKEEP <= S_AXIS_TKEEP; S_AXIS_TUSER <= S_AXIS_TUSER; S_AXIS_TVALID <= '0'; S_AXIS_TLAST <= S_AXIS_TLAST;

            end case;
        end if;
    end process;


    write_cfg_processing : process(CLK)
    begin
        if CLK'event AND CLK = '1' then 
            case i is 

                when 200   => awaddr <= x"00"; awprot <= "000"; awvalid <= '1'; wdata <= x"00005304"; wstrb <= x"F"; wvalid <= '1'; bready <= '1';
                when 201   => awaddr <= x"00"; awprot <= "000"; awvalid <= '1'; wdata <= x"00005304"; wstrb <= x"F"; wvalid <= '1'; bready <= '1';
                when 202   => awaddr <= x"00"; awprot <= "000"; awvalid <= '0'; wdata <= x"00005304"; wstrb <= x"F"; wvalid <= '0'; bready <= '1';

                when 4000  => awaddr <= x"00"; awprot <= "000"; awvalid <= '1'; wdata <= x"00005304"; wstrb <= x"F"; wvalid <= '1'; bready <= '1';
                when 4001  => awaddr <= x"00"; awprot <= "000"; awvalid <= '1'; wdata <= x"00005304"; wstrb <= x"F"; wvalid <= '1'; bready <= '1';
                when 4002  => awaddr <= x"00"; awprot <= "000"; awvalid <= '0'; wdata <= x"00005304"; wstrb <= x"F"; wvalid <= '0'; bready <= '1';


                --when 210   => awaddr <= x"04"; awprot <= "000"; awvalid <= '1'; wdata <= x"00000010"; wstrb <= x"F"; wvalid <= '1'; bready <= '1';
                --when 211   => awaddr <= x"04"; awprot <= "000"; awvalid <= '1'; wdata <= x"00000010"; wstrb <= x"F"; wvalid <= '1'; bready <= '1';
                --when 212   => awaddr <= x"04"; awprot <= "000"; awvalid <= '0'; wdata <= x"00000010"; wstrb <= x"F"; wvalid <= '0'; bready <= '1';

                --when 1000   => awaddr <= x"00"; awprot <= "000"; awvalid <= '1'; wdata <= x"00000001"; wstrb <= x"F"; wvalid <= '1'; bready <= '1';
                --when 1001   => awaddr <= x"00"; awprot <= "000"; awvalid <= '1'; wdata <= x"00000001"; wstrb <= x"F"; wvalid <= '1'; bready <= '1';
                --when 1002   => awaddr <= x"00"; awprot <= "000"; awvalid <= '0'; wdata <= x"00000001"; wstrb <= x"F"; wvalid <= '0'; bready <= '1';

                when others => awaddr <= awaddr; awprot <= awprot; awvalid <= '0'; wdata <= wdata; wstrb <= wstrb; wvalid <= '0'; bready <= '0';
            end case;
        end if;
    end process;


    write_dev_processing : process(CLK)
    begin
        if CLK'event AND CLK = '1' then 
            case i is 

                when 200   => DEV_AWADDR <= x"2E"; DEV_AWPROT <= "000"; DEV_AWVALID <= '1'; DEV_WDATA <= x"FF40FFFF"; DEV_WSTRB <= x"4"; DEV_WVALID <= '1'; DEV_BREADY <= '1';
                when 201   => DEV_AWADDR <= x"2E"; DEV_AWPROT <= "000"; DEV_AWVALID <= '1'; DEV_WDATA <= x"FF40FFFF"; DEV_WSTRB <= x"4"; DEV_WVALID <= '1'; DEV_BREADY <= '1';
                when 202   => DEV_AWADDR <= x"2E"; DEV_AWPROT <= "000"; DEV_AWVALID <= '0'; DEV_WDATA <= x"FF40FFFF"; DEV_WSTRB <= x"4"; DEV_WVALID <= '0'; DEV_BREADY <= '1';

                --when 510   => DEV_AWADDR <= x"1F"; DEV_AWPROT <= "000"; DEV_AWVALID <= '1'; DEV_WDATA <= x"FF80FFFF"; DEV_WSTRB <= x"8"; DEV_WVALID <= '1'; DEV_BREADY <= '1';
                --when 511   => DEV_AWADDR <= x"1F"; DEV_AWPROT <= "000"; DEV_AWVALID <= '1'; DEV_WDATA <= x"FF80FFFF"; DEV_WSTRB <= x"8"; DEV_WVALID <= '1'; DEV_BREADY <= '1';
                --when 512   => DEV_AWADDR <= x"1F"; DEV_AWPROT <= "000"; DEV_AWVALID <= '0'; DEV_WDATA <= x"FF80FFFF"; DEV_WSTRB <= x"8"; DEV_WVALID <= '0'; DEV_BREADY <= '1';

                --when 810   => DEV_AWADDR <= x"00"; DEV_AWPROT <= "000"; DEV_AWVALID <= '1'; DEV_WDATA <= x"FFFFFFFE"; DEV_WSTRB <= x"1"; DEV_WVALID <= '1'; DEV_BREADY <= '1';
                --when 811   => DEV_AWADDR <= x"00"; DEV_AWPROT <= "000"; DEV_AWVALID <= '1'; DEV_WDATA <= x"FFFFFFFE"; DEV_WSTRB <= x"1"; DEV_WVALID <= '1'; DEV_BREADY <= '1';
                --when 812   => DEV_AWADDR <= x"00"; DEV_AWPROT <= "000"; DEV_AWVALID <= '0'; DEV_WDATA <= x"FFFFFFFE"; DEV_WSTRB <= x"1"; DEV_WVALID <= '0'; DEV_BREADY <= '1';

                --when 200   => awaddr <= x"00"; awprot <= "000"; awvalid <= '1'; wdata <= x"00005306"; wstrb <= x"F"; wvalid <= '1'; bready <= '1';
                --when 201   => awaddr <= x"00"; awprot <= "000"; awvalid <= '1'; wdata <= x"00005306"; wstrb <= x"F"; wvalid <= '1'; bready <= '1';
                --when 202   => awaddr <= x"00"; awprot <= "000"; awvalid <= '0'; wdata <= x"00005306"; wstrb <= x"F"; wvalid <= '0'; bready <= '1';

                --when 210   => awaddr <= x"04"; awprot <= "000"; awvalid <= '1'; wdata <= x"00000010"; wstrb <= x"F"; wvalid <= '1'; bready <= '1';
                --when 211   => awaddr <= x"04"; awprot <= "000"; awvalid <= '1'; wdata <= x"00000010"; wstrb <= x"F"; wvalid <= '1'; bready <= '1';
                --when 212   => awaddr <= x"04"; awprot <= "000"; awvalid <= '0'; wdata <= x"00000010"; wstrb <= x"F"; wvalid <= '0'; bready <= '1';

                --when 1000   => awaddr <= x"00"; awprot <= "000"; awvalid <= '1'; wdata <= x"00000001"; wstrb <= x"F"; wvalid <= '1'; bready <= '1';
                --when 1001   => awaddr <= x"00"; awprot <= "000"; awvalid <= '1'; wdata <= x"00000001"; wstrb <= x"F"; wvalid <= '1'; bready <= '1';
                --when 1002   => awaddr <= x"00"; awprot <= "000"; awvalid <= '0'; wdata <= x"00000001"; wstrb <= x"F"; wvalid <= '0'; bready <= '1';

                when others => DEV_AWADDR <= DEV_AWADDR; DEV_AWPROT <= DEV_AWPROT; DEV_AWVALID <= '0'; DEV_WDATA <= DEV_WDATA; DEV_WSTRB <= DEV_WSTRB; DEV_WVALID <= '0'; DEV_BREADY <= '0';
            end case;
        end if;
    end process;


    read_processing : process(CLK)
    begin
        if CLK'event AND CLK = '1' then 
            case i is 
                --when 300   => araddr <= x"00"; arprot <= "000"; arvalid <= '1'; rready <= '1';
                --when 301   => araddr <= x"00"; arprot <= "000"; arvalid <= '1'; rready <= '1';
                --when 302   => araddr <= x"00"; arprot <= "000"; arvalid <= '0'; rready <= '1';

                --when 310   => araddr <= x"04"; arprot <= "000"; arvalid <= '1'; rready <= '1';
                --when 311   => araddr <= x"04"; arprot <= "000"; arvalid <= '1'; rready <= '1';
                --when 312   => araddr <= x"04"; arprot <= "000"; arvalid <= '0'; rready <= '1';

                --when 320   => araddr <= x"08"; arprot <= "000"; arvalid <= '1'; rready <= '1';
                --when 321   => araddr <= x"08"; arprot <= "000"; arvalid <= '1'; rready <= '1';
                --when 322   => araddr <= x"08"; arprot <= "000"; arvalid <= '0'; rready <= '1';

                --when 330   => araddr <= x"0C"; arprot <= "000"; arvalid <= '1'; rready <= '1';
                --when 331   => araddr <= x"0C"; arprot <= "000"; arvalid <= '1'; rready <= '1';
                --when 332   => araddr <= x"0C"; arprot <= "000"; arvalid <= '0'; rready <= '1';

                --when 340   => araddr <= x"10"; arprot <= "000"; arvalid <= '1'; rready <= '1';
                --when 341   => araddr <= x"10"; arprot <= "000"; arvalid <= '1'; rready <= '1';
                --when 342   => araddr <= x"10"; arprot <= "000"; arvalid <= '0'; rready <= '1';

                --when 350   => araddr <= x"14"; arprot <= "000"; arvalid <= '1'; rready <= '1';
                --when 351   => araddr <= x"14"; arprot <= "000"; arvalid <= '1'; rready <= '1';
                --when 352   => araddr <= x"14"; arprot <= "000"; arvalid <= '0'; rready <= '1';

                --when 360   => araddr <= x"18"; arprot <= "000"; arvalid <= '1'; rready <= '1';
                --when 361   => araddr <= x"18"; arprot <= "000"; arvalid <= '1'; rready <= '1';
                --when 362   => araddr <= x"18"; arprot <= "000"; arvalid <= '0'; rready <= '1';

                --when 370   => araddr <= x"1C"; arprot <= "000"; arvalid <= '1'; rready <= '1';
                --when 371   => araddr <= x"1C"; arprot <= "000"; arvalid <= '1'; rready <= '1';
                --when 372   => araddr <= x"1C"; arprot <= "000"; arvalid <= '0'; rready <= '1';

                --when 1020   => araddr <= x"00"; arprot <= "000"; arvalid <= '1'; rready <= '1';
                --when 1021   => araddr <= x"00"; arprot <= "000"; arvalid <= '1'; rready <= '1';
                --when 1022   => araddr <= x"00"; arprot <= "000"; arvalid <= '0'; rready <= '1';


                when others => araddr <= araddr; arprot <= arprot; arvalid <= '0'; rready <= '0';
            end case;
        end if;
    end process;

    ADXL_INTERRUPT_processing : process(CLK)
    begin
        if CLK'event aND CLK = '1' then 
            case i is
                when 1000   => ADXL_INTERRUPT <= '1'; 
                when 2500   => ADXL_INTERRUPT <= '0';
                when others => ADXL_INTERRUPT <= ADXL_INTERRUPT;

            end case;
        end if;
    end process;        






end architecture;