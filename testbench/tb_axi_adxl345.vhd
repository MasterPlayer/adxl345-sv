library IEEE;
    use IEEE.STD_LOGIC_1164.ALL;
    use ieee.std_logic_unsigned.all;
    use ieee.std_logic_arith.all;

entity tb_axi_adxl345 is 
end tb_axi_adxl345;



architecture tb_axi_adxl345_arch of tb_axi_adxl345 is 


    constant   S_AXI_LITE_DEV_DATA_WIDTH  : integer                         := 32                           ;
    constant   S_AXI_LITE_DEV_ADDR_WIDTH  : integer                         := 8                            ;
    constant   DEFAULT_DEVICE_ADDRESS     : std_logic_Vector ( 6 downto 0 ) := "1010011"                    ;
    constant   DEFAULT_REQUEST_INTERVAL   : integer                         := 1000                         ;
    constant   DEFAULT_CALIBRATION_LIMIT  : integer                         := 8                            ;
    constant   S_AXI_LITE_CFG_DATA_WIDTH  : integer                         := 32                           ;
    constant   S_AXI_LITE_CFG_ADDR_WIDTH  : integer                         := 8                            ;
    constant   CLK_PERIOD                 : integer                         := 100000000                    ;
    constant   RESET_DURATION             : integer                         := 100                          ;

    component axi_adxl345 
        generic (
            S_AXI_LITE_DEV_DATA_WIDTH   :           integer                         := 32                           ;
            S_AXI_LITE_DEV_ADDR_WIDTH   :           integer                         := 6                            ;
            DEFAULT_DEVICE_ADDRESS      :           std_logic_Vector ( 6 downto 0 ) := "1010011"                    ;
            DEFAULT_REQUEST_INTERVAL    :           integer                         := 1000                         ;
            DEFAULT_CALIBRATION_LIMIT   :           integer                         := 8                            ;
            S_AXI_LITE_CFG_DATA_WIDTH   :           integer                         := 32                           ;
            S_AXI_LITE_CFG_ADDR_WIDTH   :           integer                         := 8                            ;
            CLK_PERIOD                  :           integer                         := 100000000                    ;
            RESET_DURATION              :           integer                         := 1000                          
        );
        port (
            CLK                         :   in      std_logic                                                                          ;
            RESETN                      :   in      std_logic                                                                          ;
            S_AXI_LITE_CFG_AWADDR       :   in      std_logic_vector (     S_AXI_LITE_CFG_ADDR_WIDTH-1 downto 0 )                      ;
            S_AXI_LITE_CFG_AWPROT       :   in      std_logic_vector (                               2 downto 0 )                      ;
            S_AXI_LITE_CFG_AWVALID      :   in      std_logic                                                                          ;
            S_AXI_LITE_CFG_AWREADY      :   out     std_logic                                                                          ;
            S_AXI_LITE_CFG_WDATA        :   in      std_logic_vector (     S_AXI_LITE_CFG_DATA_WIDTH-1 downto 0 )                      ;
            S_AXI_LITE_CFG_WSTRB        :   in      std_logic_vector ( (S_AXI_LITE_CFG_DATA_WIDTH/8)-1 downto 0 )                      ;
            S_AXI_LITE_CFG_WVALID       :   in      std_logic                                                                          ;
            S_AXI_LITE_CFG_WREADY       :   out     std_logic                                                                          ;
            S_AXI_LITE_CFG_BRESP        :   out     std_logic_vector (                               1 downto 0 )                      ;
            S_AXI_LITE_CFG_BVALID       :   out     std_logic                                                                          ;
            S_AXI_LITE_CFG_BREADY       :   in      std_logic                                                                          ;
            S_AXI_LITE_CFG_ARADDR       :   in      std_logic_vector (     S_AXI_LITE_CFG_ADDR_WIDTH-1 downto 0 )                      ;
            S_AXI_LITE_CFG_ARPROT       :   in      std_logic_vector (                               2 downto 0 )                      ;
            S_AXI_LITE_CFG_ARVALID      :   in      std_logic                                                                          ;
            S_AXI_LITE_CFG_ARREADY      :   out     std_logic                                                                          ;
            S_AXI_LITE_CFG_RDATA        :   out     std_logic_vector (     S_AXI_LITE_CFG_DATA_WIDTH-1 downto 0 )                      ;
            S_AXI_LITE_CFG_RRESP        :   out     std_logic_vector (                               1 downto 0 )                      ;
            S_AXI_LITE_CFG_RVALID       :   out     std_logic                                                                          ;
            S_AXI_LITE_CFG_RREADY       :   in      std_logic                                                                          ;
            S_AXI_LITE_DEV_AWADDR       :   in      std_logic_vector (     S_AXI_LITE_DEV_ADDR_WIDTH-1 downto 0 )                      ;
            S_AXI_LITE_DEV_AWPROT       :   in      std_logic_vector (                               2 downto 0 )                      ;
            S_AXI_LITE_DEV_AWVALID      :   in      std_logic                                                                          ;
            S_AXI_LITE_DEV_AWREADY      :   out     std_logic                                                                          ;
            S_AXI_LITE_DEV_WDATA        :   in      std_logic_vector (     S_AXI_LITE_DEV_DATA_WIDTH-1 downto 0 )                      ;
            S_AXI_LITE_DEV_WSTRB        :   in      std_logic_vector ( (S_AXI_LITE_DEV_DATA_WIDTH/8)-1 downto 0 )                      ;
            S_AXI_LITE_DEV_WVALID       :   in      std_logic                                                                          ;
            S_AXI_LITE_DEV_WREADY       :   out     std_logic                                                                          ;
            S_AXI_LITE_DEV_BRESP        :   out     std_logic_vector (                               1 downto 0 )                      ;
            S_AXI_LITE_DEV_BVALID       :   out     std_logic                                                                          ;
            S_AXI_LITE_DEV_BREADY       :   in      std_logic                                                                          ;
            S_AXI_LITE_DEV_ARADDR       :   in      std_logic_vector (     S_AXI_LITE_DEV_ADDR_WIDTH-1 downto 0 )                      ;
            S_AXI_LITE_DEV_ARPROT       :   in      std_logic_vector (                               2 downto 0 )                      ;
            S_AXI_LITE_DEV_ARVALID      :   in      std_logic                                                                          ;
            S_AXI_LITE_DEV_ARREADY      :   out     std_logic                                                                          ;
            S_AXI_LITE_DEV_RDATA        :   out     std_logic_vector (     S_AXI_LITE_DEV_DATA_WIDTH-1 downto 0 )                      ;
            S_AXI_LITE_DEV_RRESP        :   out     std_logic_vector (                               1 downto 0 )                      ;
            S_AXI_LITE_DEV_RVALID       :   out     std_logic                                                                          ;
            S_AXI_LITE_DEV_RREADY       :   in      std_logic                                                                          ;
            M_AXIS_TDATA                :   out     std_logic_vector (                               7 downto 0 )                      ;
            M_AXIS_TKEEP                :   out     std_logic_vector (                               0 downto 0 )                      ;
            M_AXIS_TUSER                :   out     std_logic_vector (                               7 downto 0 )                      ;
            M_AXIS_TVALID               :   out     std_logic                                                                          ;
            M_AXIS_TLAST                :   out     std_logic                                                                          ;
            M_AXIS_TREADY               :   in      std_logic                                                                          ;
            S_AXIS_TDATA                :   in      std_logic_vector (                               7 downto 0 )                      ;
            S_AXIS_TKEEP                :   in      std_logic_vector (                               0 downto 0 )                      ;
            S_AXIS_TUSER                :   in      std_logic_vector (                               7 downto 0 )                      ;
            S_AXIS_TVALID               :   in      std_logic                                                                          ;
            S_AXIS_TLAST                :   in      std_logic                                                                          ;
            S_AXIS_TREADY               :   out     std_logic                                                                          ;
            ADXL_INTERRUPT              :   in      std_logic                                                                          ;
            ADXL_IRQ                    :   out     std_logic                                                                           
        );
    end component; 

    signal  clk                         :           std_logic                                                     := '0'               ;
    signal  resetn                      :           std_logic                                                     := '0'               ;
    signal  cfg_awaddr                  :           std_logic_vector (     S_AXI_LITE_CFG_ADDR_WIDTH-1 downto 0 ) := (others => '0')   ;
    signal  cfg_awprot                  :           std_logic_vector (                               2 downto 0 ) := (others => '0')   ;
    signal  cfg_awvalid                 :           std_logic                                                     := '0'               ;
    signal  cfg_awready                 :           std_logic                                                                          ;
    signal  cfg_wdata                   :           std_logic_vector (     S_AXI_LITE_CFG_DATA_WIDTH-1 downto 0 ) := (others => '0')   ;
    signal  cfg_wstrb                   :           std_logic_vector ( (S_AXI_LITE_CFG_DATA_WIDTH/8)-1 downto 0 ) := (others => '0')   ;
    signal  cfg_wvalid                  :           std_logic                                                     := '0'               ;
    signal  cfg_wready                  :           std_logic                                                                          ;
    signal  cfg_bresp                   :           std_logic_vector (                               1 downto 0 )                      ;
    signal  cfg_bvalid                  :           std_logic                                                                          ;
    signal  cfg_bready                  :           std_logic                                                     := '0'               ;
    signal  cfg_araddr                  :           std_logic_vector (     S_AXI_LITE_CFG_ADDR_WIDTH-1 downto 0 ) := (others => '0')   ;
    signal  cfg_arprot                  :           std_logic_vector (                               2 downto 0 ) := (others => '0')   ;
    signal  cfg_arvalid                 :           std_logic                                                     := '0'               ;
    signal  cfg_arready                 :           std_logic                                                                          ;
    signal  cfg_rdata                   :           std_logic_vector (     S_AXI_LITE_CFG_DATA_WIDTH-1 downto 0 )                      ;
    signal  cfg_rresp                   :           std_logic_vector (                               1 downto 0 )                      ;
    signal  cfg_rvalid                  :           std_logic                                                                          ;
    signal  cfg_rready                  :           std_logic                                                     := '0'               ;
    signal  dev_awaddr                  :           std_logic_vector (     S_AXI_LITE_DEV_ADDR_WIDTH-1 downto 0 ) := (others => '0')   ;
    signal  dev_awprot                  :           std_logic_vector (                               2 downto 0 ) := (others => '0')   ;
    signal  dev_awvalid                 :           std_logic                                                     := '0'               ;
    signal  dev_awready                 :           std_logic                                                                          ;
    signal  dev_wdata                   :           std_logic_vector (     S_AXI_LITE_DEV_DATA_WIDTH-1 downto 0 ) := (others => '0')   ;
    signal  dev_wstrb                   :           std_logic_vector ( (S_AXI_LITE_DEV_DATA_WIDTH/8)-1 downto 0 ) := (others => '0')   ;
    signal  dev_wvalid                  :           std_logic                                                     := '0'               ;
    signal  dev_wready                  :           std_logic                                                                          ;
    signal  dev_bresp                   :           std_logic_vector (                               1 downto 0 )                      ;
    signal  dev_bvalid                  :           std_logic                                                                          ;
    signal  dev_bready                  :           std_logic                                                     := '0'               ;
    signal  dev_araddr                  :           std_logic_vector (     S_AXI_LITE_DEV_ADDR_WIDTH-1 downto 0 ) := (others => '0')   ;
    signal  dev_arprot                  :           std_logic_vector (                               2 downto 0 ) := (others => '0')   ;
    signal  dev_arvalid                 :           std_logic                                                     := '0'               ;
    signal  dev_arready                 :           std_logic                                                                          ;
    signal  dev_rdata                   :           std_logic_vector (     S_AXI_LITE_DEV_DATA_WIDTH-1 downto 0 )                      ;
    signal  dev_rresp                   :           std_logic_vector (                               1 downto 0 )                      ;
    signal  dev_rvalid                  :           std_logic                                                                          ;
    signal  dev_rready                  :           std_logic                                                     := '0'               ;
    signal  m_axis_tdata                :           std_logic_vector (                               7 downto 0 )                      ;
    signal  m_axis_tkeep                :           std_logic_vector (                               0 downto 0 )                      ;
    signal  m_axis_tuser                :           std_logic_vector (                               7 downto 0 )                      ;
    signal  m_axis_tvalid               :           std_logic                                                                          ;
    signal  m_axis_tlast                :           std_logic                                                                          ;
    signal  m_axis_tready               :           std_logic                                                     := '0'               ;
    signal  s_axis_tdata                :           std_logic_vector (                               7 downto 0 ) := (others => '0')   ;
    signal  s_axis_tkeep                :           std_logic_vector (                               0 downto 0 ) := (others => '0')   ;
    signal  s_axis_tuser                :           std_logic_vector (                               7 downto 0 ) := (others => '0')   ;
    signal  s_axis_tvalid               :           std_logic                                                     := '0'               ;
    signal  s_axis_tlast                :           std_logic                                                     := '0'               ;
    signal  s_axis_tready               :           std_logic                                                                          ;
    signal  adxl_interrupt              :           std_logic                                                     := '0'               ;
    signal  adxl_irq                    :           std_logic                                                                          ;

    constant clock_period               :           time                                                          := 10 ns             ;
    signal  i                           :           integer                                                       := 0                 ;



begin 

    CLK <= not CLK after clock_period/2;

    i_processing : process(CLK)
    begin
        if CLK'event AND CLK = '1' then 
            i <= i + 1;
        end if;
    end process;

    resetn <= '0' when i < 5 else '1';

    axi_adxl345_inst : axi_adxl345 
        generic map (
            S_AXI_LITE_DEV_DATA_WIDTH   =>  S_AXI_LITE_DEV_DATA_WIDTH                       ,
            S_AXI_LITE_DEV_ADDR_WIDTH   =>  S_AXI_LITE_DEV_ADDR_WIDTH                       ,
            DEFAULT_DEVICE_ADDRESS      =>  DEFAULT_DEVICE_ADDRESS                          ,
            DEFAULT_REQUEST_INTERVAL    =>  DEFAULT_REQUEST_INTERVAL                        ,
            DEFAULT_CALIBRATION_LIMIT   =>  DEFAULT_CALIBRATION_LIMIT                       ,
            S_AXI_LITE_CFG_DATA_WIDTH   =>  S_AXI_LITE_CFG_DATA_WIDTH                       ,
            S_AXI_LITE_CFG_ADDR_WIDTH   =>  S_AXI_LITE_CFG_ADDR_WIDTH                       ,
            CLK_PERIOD                  =>  CLK_PERIOD                                      ,
            RESET_DURATION              =>  RESET_DURATION                                   
        )
        port map (
            CLK                         =>  CLK                                             ,
            RESETN                      =>  RESETN                                          ,
            S_AXI_LITE_CFG_AWADDR       =>  CFG_AWADDR                                      ,
            S_AXI_LITE_CFG_AWPROT       =>  CFG_AWPROT                                      ,
            S_AXI_LITE_CFG_AWVALID      =>  CFG_AWVALID                                     ,
            S_AXI_LITE_CFG_AWREADY      =>  CFG_AWREADY                                     ,
            S_AXI_LITE_CFG_WDATA        =>  CFG_WDATA                                       ,
            S_AXI_LITE_CFG_WSTRB        =>  CFG_WSTRB                                       ,
            S_AXI_LITE_CFG_WVALID       =>  CFG_WVALID                                      ,
            S_AXI_LITE_CFG_WREADY       =>  CFG_WREADY                                      ,
            S_AXI_LITE_CFG_BRESP        =>  CFG_BRESP                                       ,
            S_AXI_LITE_CFG_BVALID       =>  CFG_BVALID                                      ,
            S_AXI_LITE_CFG_BREADY       =>  CFG_BREADY                                      ,
            S_AXI_LITE_CFG_ARADDR       =>  CFG_ARADDR                                      ,
            S_AXI_LITE_CFG_ARPROT       =>  CFG_ARPROT                                      ,
            S_AXI_LITE_CFG_ARVALID      =>  CFG_ARVALID                                     ,
            S_AXI_LITE_CFG_ARREADY      =>  CFG_ARREADY                                     ,
            S_AXI_LITE_CFG_RDATA        =>  CFG_RDATA                                       ,
            S_AXI_LITE_CFG_RRESP        =>  CFG_RRESP                                       ,
            S_AXI_LITE_CFG_RVALID       =>  CFG_RVALID                                      ,
            S_AXI_LITE_CFG_RREADY       =>  CFG_RREADY                                      ,
            S_AXI_LITE_DEV_AWADDR       =>  DEV_AWADDR                                      ,
            S_AXI_LITE_DEV_AWPROT       =>  DEV_AWPROT                                      ,
            S_AXI_LITE_DEV_AWVALID      =>  DEV_AWVALID                                     ,
            S_AXI_LITE_DEV_AWREADY      =>  DEV_AWREADY                                     ,
            S_AXI_LITE_DEV_WDATA        =>  DEV_WDATA                                       ,
            S_AXI_LITE_DEV_WSTRB        =>  DEV_WSTRB                                       ,
            S_AXI_LITE_DEV_WVALID       =>  DEV_WVALID                                      ,
            S_AXI_LITE_DEV_WREADY       =>  DEV_WREADY                                      ,
            S_AXI_LITE_DEV_BRESP        =>  DEV_BRESP                                       ,
            S_AXI_LITE_DEV_BVALID       =>  DEV_BVALID                                      ,
            S_AXI_LITE_DEV_BREADY       =>  DEV_BREADY                                      ,
            S_AXI_LITE_DEV_ARADDR       =>  DEV_ARADDR                                      ,
            S_AXI_LITE_DEV_ARPROT       =>  DEV_ARPROT                                      ,
            S_AXI_LITE_DEV_ARVALID      =>  DEV_ARVALID                                     ,
            S_AXI_LITE_DEV_ARREADY      =>  DEV_ARREADY                                     ,
            S_AXI_LITE_DEV_RDATA        =>  DEV_RDATA                                       ,
            S_AXI_LITE_DEV_RRESP        =>  DEV_RRESP                                       ,
            S_AXI_LITE_DEV_RVALID       =>  DEV_RVALID                                      ,
            S_AXI_LITE_DEV_RREADY       =>  DEV_RREADY                                      ,
            M_AXIS_TDATA                =>  M_AXIS_TDATA                                    ,
            M_AXIS_TKEEP                =>  M_AXIS_TKEEP                                    ,
            M_AXIS_TUSER                =>  M_AXIS_TUSER                                    ,
            M_AXIS_TVALID               =>  M_AXIS_TVALID                                   ,
            M_AXIS_TLAST                =>  M_AXIS_TLAST                                    ,
            M_AXIS_TREADY               =>  M_AXIS_TREADY                                   ,
            S_AXIS_TDATA                =>  S_AXIS_TDATA                                    ,
            S_AXIS_TKEEP                =>  S_AXIS_TKEEP                                    ,
            S_AXIS_TUSER                =>  S_AXIS_TUSER                                    ,
            S_AXIS_TVALID               =>  S_AXIS_TVALID                                   ,
            S_AXIS_TLAST                =>  S_AXIS_TLAST                                    ,
            S_AXIS_TREADY               =>  S_AXIS_TREADY                                   ,
            ADXL_INTERRUPT              =>  ADXL_INTERRUPT                                  ,
            ADXL_IRQ                    =>  ADXL_IRQ                                         
        );
    
    M_AXIS_TREADY <= '1';



    configuration_write_processing : process(CLK)
    begin
        if CLK'event AND CLK = '1' then 
            case i is 

                when    300     => cfg_awaddr <= x"00"; cfg_awprot <= "000"; cfg_awvalid <= '1'; cfg_wdata <= x"00005308"; cfg_wstrb <= x"F"; cfg_wvalid <= '1'; cfg_bready <= '1';
                when    301     => cfg_awaddr <= x"00"; cfg_awprot <= "000"; cfg_awvalid <= '1'; cfg_wdata <= x"00005308"; cfg_wstrb <= x"F"; cfg_wvalid <= '1'; cfg_bready <= '1';
                when    302     => cfg_awaddr <= x"00"; cfg_awprot <= "000"; cfg_awvalid <= '0'; cfg_wdata <= x"00005308"; cfg_wstrb <= x"F"; cfg_wvalid <= '0'; cfg_bready <= '1';

                when   2300     => cfg_awaddr <= x"00"; cfg_awprot <= "000"; cfg_awvalid <= '1'; cfg_wdata <= x"00005308"; cfg_wstrb <= x"F"; cfg_wvalid <= '1'; cfg_bready <= '1';
                when   2301     => cfg_awaddr <= x"00"; cfg_awprot <= "000"; cfg_awvalid <= '1'; cfg_wdata <= x"00005308"; cfg_wstrb <= x"F"; cfg_wvalid <= '1'; cfg_bready <= '1';
                when   2302     => cfg_awaddr <= x"00"; cfg_awprot <= "000"; cfg_awvalid <= '0'; cfg_wdata <= x"00005308"; cfg_wstrb <= x"F"; cfg_wvalid <= '0'; cfg_bready <= '1';


                when others     => cfg_awaddr <= cfg_awaddr; cfg_awprot <= cfg_awprot; cfg_awvalid <= '0'; cfg_wdata <= cfg_wdata; cfg_wstrb <= cfg_wstrb; cfg_wvalid <= '0'; cfg_bready <= '0';
            end case;
        end if;
    end process;




    devoce_write_processing : process(CLK)
    begin
        if CLK'event AND CLK = '1' then 
            case i is 

                when   5000     => dev_awaddr <= x"1C"; dev_awvalid <= '1'; dev_wdata <= x"000000F0"; dev_wstrb <= x"1"; dev_wvalid <= '1'; dev_bready <= '1';
                when   5001     => dev_awaddr <= x"1C"; dev_awvalid <= '1'; dev_wdata <= x"000000F0"; dev_wstrb <= x"1"; dev_wvalid <= '1'; dev_bready <= '1';
                when   5002     => dev_awaddr <= x"1C"; dev_awvalid <= '0'; dev_wdata <= x"000000F0"; dev_wstrb <= x"1"; dev_wvalid <= '0'; dev_bready <= '1';

                when   5010     => dev_awaddr <= x"1C"; dev_awvalid <= '1'; dev_wdata <= x"0000F100"; dev_wstrb <= x"2"; dev_wvalid <= '1'; dev_bready <= '1';
                when   5011     => dev_awaddr <= x"1C"; dev_awvalid <= '1'; dev_wdata <= x"0000F100"; dev_wstrb <= x"2"; dev_wvalid <= '1'; dev_bready <= '1';
                when   5012     => dev_awaddr <= x"1C"; dev_awvalid <= '0'; dev_wdata <= x"0000F100"; dev_wstrb <= x"2"; dev_wvalid <= '0'; dev_bready <= '1';

                when   5020     => dev_awaddr <= x"1C"; dev_awvalid <= '1'; dev_wdata <= x"00F20000"; dev_wstrb <= x"4"; dev_wvalid <= '1'; dev_bready <= '1';
                when   5021     => dev_awaddr <= x"1C"; dev_awvalid <= '1'; dev_wdata <= x"00F20000"; dev_wstrb <= x"4"; dev_wvalid <= '1'; dev_bready <= '1';
                when   5022     => dev_awaddr <= x"1C"; dev_awvalid <= '0'; dev_wdata <= x"00F20000"; dev_wstrb <= x"4"; dev_wvalid <= '0'; dev_bready <= '1';

                when   5030     => dev_awaddr <= x"1C"; dev_awvalid <= '1'; dev_wdata <= x"F3000000"; dev_wstrb <= x"8"; dev_wvalid <= '1'; dev_bready <= '1';
                when   5031     => dev_awaddr <= x"1C"; dev_awvalid <= '1'; dev_wdata <= x"F3000000"; dev_wstrb <= x"8"; dev_wvalid <= '1'; dev_bready <= '1';
                when   5032     => dev_awaddr <= x"1C"; dev_awvalid <= '0'; dev_wdata <= x"F3000000"; dev_wstrb <= x"8"; dev_wvalid <= '0'; dev_bready <= '1';

                when   5040     => dev_awaddr <= x"20"; dev_awvalid <= '1'; dev_wdata <= x"F7F6F5F4"; dev_wstrb <= x"F"; dev_wvalid <= '1'; dev_bready <= '1';
                when   5041     => dev_awaddr <= x"20"; dev_awvalid <= '1'; dev_wdata <= x"F7F6F5F4"; dev_wstrb <= x"F"; dev_wvalid <= '1'; dev_bready <= '1';
                when   5042     => dev_awaddr <= x"20"; dev_awvalid <= '0'; dev_wdata <= x"F7F6F5F4"; dev_wstrb <= x"F"; dev_wvalid <= '0'; dev_bready <= '1';

                when   5050     => dev_awaddr <= x"20"; dev_awvalid <= '1'; dev_wdata <= x"0000F500"; dev_wstrb <= x"2"; dev_wvalid <= '1'; dev_bready <= '1';
                when   5051     => dev_awaddr <= x"20"; dev_awvalid <= '1'; dev_wdata <= x"0000F500"; dev_wstrb <= x"2"; dev_wvalid <= '1'; dev_bready <= '1';
                when   5052     => dev_awaddr <= x"20"; dev_awvalid <= '0'; dev_wdata <= x"0000F500"; dev_wstrb <= x"2"; dev_wvalid <= '0'; dev_bready <= '1';

                when   5060     => dev_awaddr <= x"20"; dev_awvalid <= '1'; dev_wdata <= x"00F60000"; dev_wstrb <= x"4"; dev_wvalid <= '1'; dev_bready <= '1';
                when   5061     => dev_awaddr <= x"20"; dev_awvalid <= '1'; dev_wdata <= x"00F60000"; dev_wstrb <= x"4"; dev_wvalid <= '1'; dev_bready <= '1';
                when   5062     => dev_awaddr <= x"20"; dev_awvalid <= '0'; dev_wdata <= x"00F60000"; dev_wstrb <= x"4"; dev_wvalid <= '0'; dev_bready <= '1';

                when   5070     => dev_awaddr <= x"20"; dev_awvalid <= '1'; dev_wdata <= x"F7000000"; dev_wstrb <= x"8"; dev_wvalid <= '1'; dev_bready <= '1';
                when   5071     => dev_awaddr <= x"20"; dev_awvalid <= '1'; dev_wdata <= x"F7000000"; dev_wstrb <= x"8"; dev_wvalid <= '1'; dev_bready <= '1';
                when   5072     => dev_awaddr <= x"20"; dev_awvalid <= '0'; dev_wdata <= x"F7000000"; dev_wstrb <= x"8"; dev_wvalid <= '0'; dev_bready <= '1';


                when others     => dev_awaddr <= dev_awaddr; dev_awvalid <= '0'; dev_wdata <= (others => '0'); dev_wstrb <= dev_wstrb; dev_wvalid <= '0'; dev_bready <= '0';
            end case;
        end if;
    end process;


    s_axis_processing : process(CLK)
    begin
        if CLK'event aND CLK = '1' then 
            case i is 

                when 1000   => S_AXIS_TDATA <= x"E5"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1010   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1020   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1030   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1040   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1050   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1060   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1070   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1080   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1090   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1100   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1110   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1120   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1130   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1140   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1150   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1160   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1170   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1180   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1190   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1200   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1210   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1220   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1230   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1240   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1250   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1260   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1270   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1280   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1290   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1300   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1310   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1320   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1330   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1340   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1350   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1360   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1370   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1380   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1390   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1400   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1410   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1420   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1430   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1440   => S_AXIS_TDATA <= x"0F"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1450   => S_AXIS_TDATA <= x"08"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1460   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1470   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1480   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1490   => S_AXIS_TDATA <= x"08"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1500   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1510   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1520   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1530   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1540   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1550   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1560   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 1570   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '1';



                when 3000   => S_AXIS_TDATA <= x"E5"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3010   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3020   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3030   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3040   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3050   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3060   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3070   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3080   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3090   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3100   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3110   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3120   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3130   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3140   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3150   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3160   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3170   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3180   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3190   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3200   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3210   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3220   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3230   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3240   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3250   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3260   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3270   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3280   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3290   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3300   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3310   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3320   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3330   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3340   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3350   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3360   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3370   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3380   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3390   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3400   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3410   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3420   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3430   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3440   => S_AXIS_TDATA <= x"0F"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3450   => S_AXIS_TDATA <= x"08"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3460   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3470   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3480   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3490   => S_AXIS_TDATA <= x"08"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3500   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3510   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3520   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3530   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3540   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3550   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3560   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
                when 3570   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '1';


                when others => S_AXIS_TDATA <= S_AXIS_TDATA; S_AXIS_TKEEP <= S_AXIS_TKEEP; S_AXIS_TUSER <= S_AXIS_TUSER; S_AXIS_TVALID <= '0'; S_AXIS_TLAST <= S_AXIS_TLAST;

            end case;
        end if;
    end process;


    --s_axis_processing : process(CLK)
    --begin
    --    if CLK'event aND CLK = '1' then 
    --        case i is 

    --            --when 1100   => S_AXIS_TDATA <= x"02"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '1';

    --            --when 1400   => S_AXIS_TDATA <= x"04"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '1';

    --            ----when 1500   => S_AXIS_TDATA <= x"10"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '1';


    --            --when 32000   => S_AXIS_TDATA <= x"E0"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 32001   => S_AXIS_TDATA <= x"FF"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 32002   => S_AXIS_TDATA <= x"E0"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 32003   => S_AXIS_TDATA <= x"FF"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 32004   => S_AXIS_TDATA <= x"E0"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 32005   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '1';

    --            --when 80000   => S_AXIS_TDATA <= x"20"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 80001   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 80002   => S_AXIS_TDATA <= x"20"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 80003   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 80004   => S_AXIS_TDATA <= x"20"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 80005   => S_AXIS_TDATA <= x"01"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '1';

    --            --when 120000   => S_AXIS_TDATA <= x"E0"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 120001   => S_AXIS_TDATA <= x"FF"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 120002   => S_AXIS_TDATA <= x"E0"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 120003   => S_AXIS_TDATA <= x"FF"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 120004   => S_AXIS_TDATA <= x"E0"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 120005   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '1';

    --            --when 160000   => S_AXIS_TDATA <= x"20"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 160001   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 160002   => S_AXIS_TDATA <= x"20"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 160003   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 160004   => S_AXIS_TDATA <= x"20"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 160005   => S_AXIS_TDATA <= x"01"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '1';

    --            --when 200000   => S_AXIS_TDATA <= x"E0"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 200001   => S_AXIS_TDATA <= x"FF"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 200002   => S_AXIS_TDATA <= x"E0"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 200003   => S_AXIS_TDATA <= x"FF"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 200004   => S_AXIS_TDATA <= x"E0"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 200005   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '1';

    --            --when 240000   => S_AXIS_TDATA <= x"20"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 240001   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 240002   => S_AXIS_TDATA <= x"20"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 240003   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 240004   => S_AXIS_TDATA <= x"20"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 240005   => S_AXIS_TDATA <= x"01"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '1';

    --            --when 280000   => S_AXIS_TDATA <= x"E0"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 280001   => S_AXIS_TDATA <= x"FF"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 280002   => S_AXIS_TDATA <= x"E0"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 280003   => S_AXIS_TDATA <= x"FF"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 280004   => S_AXIS_TDATA <= x"E0"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 280005   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '1';

    --            --when 320000   => S_AXIS_TDATA <= x"20"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 320001   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 320002   => S_AXIS_TDATA <= x"20"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 320003   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 320004   => S_AXIS_TDATA <= x"20"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 320005   => S_AXIS_TDATA <= x"01"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '1';


    --            --when 360000   => S_AXIS_TDATA <= x"E5"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360001   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360002   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360003   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360004   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360005   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360006   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360007   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360008   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360009   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360010   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360011   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360012   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360013   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360014   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360015   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360016   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360017   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360018   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360019   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360020   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360021   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360022   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360023   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360024   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360025   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360026   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360027   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360028   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360029   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360030   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360031   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360032   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360033   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360034   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360035   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360036   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360037   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360038   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360039   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360040   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360041   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360042   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360043   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360044   => S_AXIS_TDATA <= x"0F"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360045   => S_AXIS_TDATA <= x"08"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360046   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360047   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360048   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360049   => S_AXIS_TDATA <= x"08"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360050   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360051   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360052   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360053   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360054   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360055   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360056   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 360057   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '1';


    --            --when 440000   => S_AXIS_TDATA <= x"E0"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 440001   => S_AXIS_TDATA <= x"FF"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 440002   => S_AXIS_TDATA <= x"E0"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 440003   => S_AXIS_TDATA <= x"FF"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 440004   => S_AXIS_TDATA <= x"E0"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 440005   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '1';

    --            --when 480000   => S_AXIS_TDATA <= x"E0"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 480001   => S_AXIS_TDATA <= x"FF"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 480002   => S_AXIS_TDATA <= x"E0"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 480003   => S_AXIS_TDATA <= x"FF"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 480004   => S_AXIS_TDATA <= x"E0"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 480005   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '1';

    --            --when 520000   => S_AXIS_TDATA <= x"E0"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 520001   => S_AXIS_TDATA <= x"FF"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 520002   => S_AXIS_TDATA <= x"E0"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 520003   => S_AXIS_TDATA <= x"FF"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 520004   => S_AXIS_TDATA <= x"E0"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 520005   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '1';

    --            --when 560000   => S_AXIS_TDATA <= x"E0"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 560001   => S_AXIS_TDATA <= x"FF"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 560002   => S_AXIS_TDATA <= x"E0"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 560003   => S_AXIS_TDATA <= x"FF"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 560004   => S_AXIS_TDATA <= x"E0"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 560005   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '1';

    --            --when 600000   => S_AXIS_TDATA <= x"E0"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 600001   => S_AXIS_TDATA <= x"FF"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 600002   => S_AXIS_TDATA <= x"E0"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 600003   => S_AXIS_TDATA <= x"FF"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 600004   => S_AXIS_TDATA <= x"E0"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 600005   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '1';

    --            --when 640000   => S_AXIS_TDATA <= x"E0"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 640001   => S_AXIS_TDATA <= x"FF"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 640002   => S_AXIS_TDATA <= x"E0"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 640003   => S_AXIS_TDATA <= x"FF"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 640004   => S_AXIS_TDATA <= x"E0"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 640005   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '1';

    --            --when 680000   => S_AXIS_TDATA <= x"E0"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 680001   => S_AXIS_TDATA <= x"FF"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 680002   => S_AXIS_TDATA <= x"E0"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 680003   => S_AXIS_TDATA <= x"FF"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 680004   => S_AXIS_TDATA <= x"E0"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 680005   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '1';

    --            --when 720000   => S_AXIS_TDATA <= x"E0"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 720001   => S_AXIS_TDATA <= x"FF"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 720002   => S_AXIS_TDATA <= x"E0"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 720003   => S_AXIS_TDATA <= x"FF"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 720004   => S_AXIS_TDATA <= x"E0"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '0';
    --            --when 720005   => S_AXIS_TDATA <= x"00"; S_AXIS_TKEEP <= "1"; S_AXIS_TUSER <= x"A7"; S_AXIS_TVALID <= '1'; S_AXIS_TLAST <= '1';




    --            when others => S_AXIS_TDATA <= S_AXIS_TDATA; S_AXIS_TKEEP <= S_AXIS_TKEEP; S_AXIS_TUSER <= S_AXIS_TUSER; S_AXIS_TVALID <= '0'; S_AXIS_TLAST <= S_AXIS_TLAST;

    --        end case;
    --    end if;
    --end process;


    --write_cfg_processing : process(CLK)
    --begin
    --    if CLK'event AND CLK = '1' then 
    --        case i is 

    --            --when 300   => awaddr <= x"00"; awprot <= "000"; awvalid <= '1'; wdata <= x"00005320"; wstrb <= x"F"; wvalid <= '1'; bready <= '1';
    --            --when 301   => awaddr <= x"00"; awprot <= "000"; awvalid <= '1'; wdata <= x"00005320"; wstrb <= x"F"; wvalid <= '1'; bready <= '1';
    --            --when 302   => awaddr <= x"00"; awprot <= "000"; awvalid <= '0'; wdata <= x"00005320"; wstrb <= x"F"; wvalid <= '0'; bready <= '1';

    --            --when 400000   => awaddr <= x"00"; awprot <= "000"; awvalid <= '1'; wdata <= x"00005320"; wstrb <= x"F"; wvalid <= '1'; bready <= '1';
    --            --when 400001   => awaddr <= x"00"; awprot <= "000"; awvalid <= '1'; wdata <= x"00005320"; wstrb <= x"F"; wvalid <= '1'; bready <= '1';
    --            --when 400002   => awaddr <= x"00"; awprot <= "000"; awvalid <= '0'; wdata <= x"00005320"; wstrb <= x"F"; wvalid <= '0'; bready <= '1';


    --            --when 6800   => awaddr <= x"00"; awprot <= "000"; awvalid <= '1'; wdata <= x"00005314"; wstrb <= x"F"; wvalid <= '1'; bready <= '1';
    --            --when 6801   => awaddr <= x"00"; awprot <= "000"; awvalid <= '1'; wdata <= x"00005314"; wstrb <= x"F"; wvalid <= '1'; bready <= '1';
    --            --when 6802   => awaddr <= x"00"; awprot <= "000"; awvalid <= '0'; wdata <= x"00005314"; wstrb <= x"F"; wvalid <= '0'; bready <= '1';

    --            --when 210   => awaddr <= x"04"; awprot <= "000"; awvalid <= '1'; wdata <= x"00000010"; wstrb <= x"F"; wvalid <= '1'; bready <= '1';
    --            --when 211   => awaddr <= x"04"; awprot <= "000"; awvalid <= '1'; wdata <= x"00000010"; wstrb <= x"F"; wvalid <= '1'; bready <= '1';
    --            --when 212   => awaddr <= x"04"; awprot <= "000"; awvalid <= '0'; wdata <= x"00000010"; wstrb <= x"F"; wvalid <= '0'; bready <= '1';

    --            --when 1000   => awaddr <= x"00"; awprot <= "000"; awvalid <= '1'; wdata <= x"00000001"; wstrb <= x"F"; wvalid <= '1'; bready <= '1';
    --            --when 1001   => awaddr <= x"00"; awprot <= "000"; awvalid <= '1'; wdata <= x"00000001"; wstrb <= x"F"; wvalid <= '1'; bready <= '1';
    --            --when 1002   => awaddr <= x"00"; awprot <= "000"; awvalid <= '0'; wdata <= x"00000001"; wstrb <= x"F"; wvalid <= '0'; bready <= '1';

    --            when others => awaddr <= awaddr; awprot <= awprot; awvalid <= '0'; wdata <= wdata; wstrb <= wstrb; wvalid <= '0'; bready <= '0';
    --        end case;
    --    end if;
    --end process;


    --write_dev_processing : process(CLK)
    --begin
    --    if CLK'event AND CLK = '1' then 
    --        case i is 

    --            --when 200   => DEV_AWADDR <= x"2E"; DEV_AWPROT <= "000"; DEV_AWVALID <= '1'; DEV_WDATA <= x"FF02FFFF"; DEV_WSTRB <= x"4"; DEV_WVALID <= '1'; DEV_BREADY <= '1';
    --            --when 201   => DEV_AWADDR <= x"2E"; DEV_AWPROT <= "000"; DEV_AWVALID <= '1'; DEV_WDATA <= x"FF02FFFF"; DEV_WSTRB <= x"4"; DEV_WVALID <= '1'; DEV_BREADY <= '1';
    --            --when 202   => DEV_AWADDR <= x"2E"; DEV_AWPROT <= "000"; DEV_AWVALID <= '0'; DEV_WDATA <= x"FF02FFFF"; DEV_WSTRB <= x"4"; DEV_WVALID <= '0'; DEV_BREADY <= '1';

    --            --when 205   => DEV_AWADDR <= x"2C"; DEV_AWPROT <= "000"; DEV_AWVALID <= '1'; DEV_WDATA <= x"FFFFFF0F"; DEV_WSTRB <= x"1"; DEV_WVALID <= '1'; DEV_BREADY <= '1';
    --            --when 206   => DEV_AWADDR <= x"2C"; DEV_AWPROT <= "000"; DEV_AWVALID <= '1'; DEV_WDATA <= x"FFFFFF0F"; DEV_WSTRB <= x"1"; DEV_WVALID <= '1'; DEV_BREADY <= '1';
    --            --when 207   => DEV_AWADDR <= x"2C"; DEV_AWPROT <= "000"; DEV_AWVALID <= '0'; DEV_WDATA <= x"FFFFFF0F"; DEV_WSTRB <= x"1"; DEV_WVALID <= '0'; DEV_BREADY <= '1';

    --            --when 210   => DEV_AWADDR <= x"31"; DEV_AWPROT <= "000"; DEV_AWVALID <= '1'; DEV_WDATA <= x"FFFF08FF"; DEV_WSTRB <= x"2"; DEV_WVALID <= '1'; DEV_BREADY <= '1';
    --            --when 211   => DEV_AWADDR <= x"31"; DEV_AWPROT <= "000"; DEV_AWVALID <= '1'; DEV_WDATA <= x"FFFF08FF"; DEV_WSTRB <= x"2"; DEV_WVALID <= '1'; DEV_BREADY <= '1';
    --            --when 212   => DEV_AWADDR <= x"31"; DEV_AWPROT <= "000"; DEV_AWVALID <= '0'; DEV_WDATA <= x"FFFF08FF"; DEV_WSTRB <= x"2"; DEV_WVALID <= '0'; DEV_BREADY <= '1';

    --            --when 510   => DEV_AWADDR <= x"1F"; DEV_AWPROT <= "000"; DEV_AWVALID <= '1'; DEV_WDATA <= x"FF80FFFF"; DEV_WSTRB <= x"8"; DEV_WVALID <= '1'; DEV_BREADY <= '1';
    --            --when 511   => DEV_AWADDR <= x"1F"; DEV_AWPROT <= "000"; DEV_AWVALID <= '1'; DEV_WDATA <= x"FF80FFFF"; DEV_WSTRB <= x"8"; DEV_WVALID <= '1'; DEV_BREADY <= '1';
    --            --when 512   => DEV_AWADDR <= x"1F"; DEV_AWPROT <= "000"; DEV_AWVALID <= '0'; DEV_WDATA <= x"FF80FFFF"; DEV_WSTRB <= x"8"; DEV_WVALID <= '0'; DEV_BREADY <= '1';

    --            --when 810   => DEV_AWADDR <= x"00"; DEV_AWPROT <= "000"; DEV_AWVALID <= '1'; DEV_WDATA <= x"FFFFFFFE"; DEV_WSTRB <= x"1"; DEV_WVALID <= '1'; DEV_BREADY <= '1';
    --            --when 811   => DEV_AWADDR <= x"00"; DEV_AWPROT <= "000"; DEV_AWVALID <= '1'; DEV_WDATA <= x"FFFFFFFE"; DEV_WSTRB <= x"1"; DEV_WVALID <= '1'; DEV_BREADY <= '1';
    --            --when 812   => DEV_AWADDR <= x"00"; DEV_AWPROT <= "000"; DEV_AWVALID <= '0'; DEV_WDATA <= x"FFFFFFFE"; DEV_WSTRB <= x"1"; DEV_WVALID <= '0'; DEV_BREADY <= '1';

    --            --when 200   => awaddr <= x"00"; awprot <= "000"; awvalid <= '1'; wdata <= x"00005306"; wstrb <= x"F"; wvalid <= '1'; bready <= '1';
    --            --when 201   => awaddr <= x"00"; awprot <= "000"; awvalid <= '1'; wdata <= x"00005306"; wstrb <= x"F"; wvalid <= '1'; bready <= '1';
    --            --when 202   => awaddr <= x"00"; awprot <= "000"; awvalid <= '0'; wdata <= x"00005306"; wstrb <= x"F"; wvalid <= '0'; bready <= '1';

    --            --when 210   => awaddr <= x"04"; awprot <= "000"; awvalid <= '1'; wdata <= x"00000010"; wstrb <= x"F"; wvalid <= '1'; bready <= '1';
    --            --when 211   => awaddr <= x"04"; awprot <= "000"; awvalid <= '1'; wdata <= x"00000010"; wstrb <= x"F"; wvalid <= '1'; bready <= '1';
    --            --when 212   => awaddr <= x"04"; awprot <= "000"; awvalid <= '0'; wdata <= x"00000010"; wstrb <= x"F"; wvalid <= '0'; bready <= '1';

    --            --when 1000   => awaddr <= x"00"; awprot <= "000"; awvalid <= '1'; wdata <= x"00000001"; wstrb <= x"F"; wvalid <= '1'; bready <= '1';
    --            --when 1001   => awaddr <= x"00"; awprot <= "000"; awvalid <= '1'; wdata <= x"00000001"; wstrb <= x"F"; wvalid <= '1'; bready <= '1';
    --            --when 1002   => awaddr <= x"00"; awprot <= "000"; awvalid <= '0'; wdata <= x"00000001"; wstrb <= x"F"; wvalid <= '0'; bready <= '1';

    --            when others => DEV_AWADDR <= DEV_AWADDR; DEV_AWPROT <= DEV_AWPROT; DEV_AWVALID <= '0'; DEV_WDATA <= DEV_WDATA; DEV_WSTRB <= DEV_WSTRB; DEV_WVALID <= '0'; DEV_BREADY <= '0';
    --        end case;
    --    end if;
    --end process;


    --read_processing : process(CLK)
    --begin
    --    if CLK'event AND CLK = '1' then 
    --        case i is 
    --            when 6000   => araddr <= x"1C"; arprot <= "000"; arvalid <= '1'; rready <= '1';
    --            when 6001   => araddr <= x"1C"; arprot <= "000"; arvalid <= '1'; rready <= '1';
    --            when 6002   => araddr <= x"1C"; arprot <= "000"; arvalid <= '0'; rready <= '1';
    --            when 6003   => araddr <= x"1C"; arprot <= "000"; arvalid <= '0'; rready <= '1';

    --            --when 7000   => araddr <= x"40"; arprot <= "000"; arvalid <= '1'; rready <= '1';
    --            --when 7001   => araddr <= x"40"; arprot <= "000"; arvalid <= '1'; rready <= '1';
    --            --when 7002   => araddr <= x"40"; arprot <= "000"; arvalid <= '0'; rready <= '1';

    --            --when 7010   => araddr <= x"44"; arprot <= "000"; arvalid <= '1'; rready <= '1';
    --            --when 7011   => araddr <= x"44"; arprot <= "000"; arvalid <= '1'; rready <= '1';
    --            --when 7012   => araddr <= x"44"; arprot <= "000"; arvalid <= '0'; rready <= '1';

    --            --when 7020   => araddr <= x"48"; arprot <= "000"; arvalid <= '1'; rready <= '1';
    --            --when 7021   => araddr <= x"48"; arprot <= "000"; arvalid <= '1'; rready <= '1';
    --            --when 7022   => araddr <= x"48"; arprot <= "000"; arvalid <= '0'; rready <= '1';

    --            --when 7030   => araddr <= x"4C"; arprot <= "000"; arvalid <= '1'; rready <= '1';
    --            --when 7031   => araddr <= x"4C"; arprot <= "000"; arvalid <= '1'; rready <= '1';
    --            --when 7032   => araddr <= x"4C"; arprot <= "000"; arvalid <= '0'; rready <= '1';


    --            --when 310   => araddr <= x"04"; arprot <= "000"; arvalid <= '1'; rready <= '1';
    --            --when 311   => araddr <= x"04"; arprot <= "000"; arvalid <= '1'; rready <= '1';
    --            --when 312   => araddr <= x"04"; arprot <= "000"; arvalid <= '0'; rready <= '1';

    --            --when 320   => araddr <= x"08"; arprot <= "000"; arvalid <= '1'; rready <= '1';
    --            --when 321   => araddr <= x"08"; arprot <= "000"; arvalid <= '1'; rready <= '1';
    --            --when 322   => araddr <= x"08"; arprot <= "000"; arvalid <= '0'; rready <= '1';

    --            --when 330   => araddr <= x"0C"; arprot <= "000"; arvalid <= '1'; rready <= '1';
    --            --when 331   => araddr <= x"0C"; arprot <= "000"; arvalid <= '1'; rready <= '1';
    --            --when 332   => araddr <= x"0C"; arprot <= "000"; arvalid <= '0'; rready <= '1';

    --            --when 340   => araddr <= x"10"; arprot <= "000"; arvalid <= '1'; rready <= '1';
    --            --when 341   => araddr <= x"10"; arprot <= "000"; arvalid <= '1'; rready <= '1';
    --            --when 342   => araddr <= x"10"; arprot <= "000"; arvalid <= '0'; rready <= '1';

    --            --when 350   => araddr <= x"14"; arprot <= "000"; arvalid <= '1'; rready <= '1';
    --            --when 351   => araddr <= x"14"; arprot <= "000"; arvalid <= '1'; rready <= '1';
    --            --when 352   => araddr <= x"14"; arprot <= "000"; arvalid <= '0'; rready <= '1';

    --            --when 360   => araddr <= x"18"; arprot <= "000"; arvalid <= '1'; rready <= '1';
    --            --when 361   => araddr <= x"18"; arprot <= "000"; arvalid <= '1'; rready <= '1';
    --            --when 362   => araddr <= x"18"; arprot <= "000"; arvalid <= '0'; rready <= '1';

    --            --when 370   => araddr <= x"1C"; arprot <= "000"; arvalid <= '1'; rready <= '1';
    --            --when 371   => araddr <= x"1C"; arprot <= "000"; arvalid <= '1'; rready <= '1';
    --            --when 372   => araddr <= x"1C"; arprot <= "000"; arvalid <= '0'; rready <= '1';

    --            --when 1020   => araddr <= x"00"; arprot <= "000"; arvalid <= '1'; rready <= '1';
    --            --when 1021   => araddr <= x"00"; arprot <= "000"; arvalid <= '1'; rready <= '1';
    --            --when 1022   => araddr <= x"00"; arprot <= "000"; arvalid <= '0'; rready <= '1';


    --            when others => araddr <= araddr; arprot <= arprot; arvalid <= '0'; rready <= '0';
    --        end case;
    --    end if;
    --end process;

    --ADXL_INTERRUPT_processing : process(CLK)
    --begin
    --    if CLK'event aND CLK = '1' then 
    --        case i is
    --            --when 1000   => ADXL_INTERRUPT <= '1'; 
    --            --when 5300   => ADXL_INTERRUPT <= '0';
    --            ----when 2500   => ADXL_INTERRUPT <= '0';
    --            when others => ADXL_INTERRUPT <= ADXL_INTERRUPT;

    --        end case;
    --    end if;
    --end process;        






end architecture;