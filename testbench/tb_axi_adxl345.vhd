library IEEE;
    use IEEE.STD_LOGIC_1164.ALL;
    use ieee.std_logic_unsigned.all;
    use ieee.std_logic_arith.all;



Library UNISIM;
use UNISIM.vcomponents.all;


entity tb_axi_adxl345 is 
end tb_axi_adxl345;



architecture tb_axi_adxl345_arch of tb_axi_adxl345 is 


    constant   S_AXI_LITE_DEV_DATA_WIDTH  : integer                         := 32                           ;
    constant   S_AXI_LITE_DEV_ADDR_WIDTH  : integer                         := 8                            ;
    constant   DEFAULT_DEVICE_ADDRESS     : std_logic_Vector ( 6 downto 0 ) := "1010011"                    ;
    constant   DEFAULT_REQUESTION_INTERVAL: integer                         := 1000                         ;
    constant   DEFAULT_CALIBRATION_MODE   : integer                         := 2                            ;
    constant   S_AXI_LITE_CFG_DATA_WIDTH  : integer                         := 32                           ;
    constant   S_AXI_LITE_CFG_ADDR_WIDTH  : integer                         := 8                            ;
    constant   CLK_PERIOD                 : integer                         := 100000000                    ;
    constant   RESET_DURATION             : integer                         := 100                          ;

    component axi_adxl345 
        generic (
            S_AXI_LITE_DEV_DATA_WIDTH   :           integer                         := 32                           ;
            S_AXI_LITE_DEV_ADDR_WIDTH   :           integer                         := 6                            ;
            DEFAULT_DEVICE_ADDRESS      :           std_logic_Vector ( 6 downto 0 ) := "1010011"                    ;
            DEFAULT_REQUESTION_INTERVAL    :           integer                         := 1000                         ;
            DEFAULT_CALIBRATION_MODE    :           integer                         := 8                            ;
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
    signal  DEV_AWADDR                  :           std_logic_vector (     S_AXI_LITE_DEV_ADDR_WIDTH-1 downto 0 ) := (others => '0')   ;
    signal  DEV_AWPROT                  :           std_logic_vector (                               2 downto 0 ) := (others => '0')   ;
    signal  DEV_AWVALID                 :           std_logic                                                     := '0'               ;
    signal  DEV_AWREADY                 :           std_logic                                                                          ;
    signal  DEV_WDATA                   :           std_logic_vector (     S_AXI_LITE_DEV_DATA_WIDTH-1 downto 0 ) := (others => '0')   ;
    signal  DEV_WSTRB                   :           std_logic_vector ( (S_AXI_LITE_DEV_DATA_WIDTH/8)-1 downto 0 ) := (others => '0')   ;
    signal  DEV_WVALID                  :           std_logic                                                     := '0'               ;
    signal  DEV_WREADY                  :           std_logic                                                                          ;
    signal  DEV_BRESP                   :           std_logic_vector (                               1 downto 0 )                      ;
    signal  DEV_BVALID                  :           std_logic                                                                          ;
    signal  DEV_BREADY                  :           std_logic                                                     := '0'               ;
    signal  DEV_ARADDR                  :           std_logic_vector (     S_AXI_LITE_DEV_ADDR_WIDTH-1 downto 0 ) := (others => '0')   ;
    signal  DEV_ARPROT                  :           std_logic_vector (                               2 downto 0 ) := (others => '0')   ;
    signal  DEV_ARVALID                 :           std_logic                                                     := '0'               ;
    signal  DEV_ARREADY                 :           std_logic                                                                          ;
    signal  DEV_RDATA                   :           std_logic_vector (     S_AXI_LITE_DEV_DATA_WIDTH-1 downto 0 )                      ;
    signal  DEV_RRESP                   :           std_logic_vector (                               1 downto 0 )                      ;
    signal  DEV_RVALID                  :           std_logic                                                                          ;
    signal  DEV_RREADY                  :           std_logic                                                     := '0'               ;
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


    component axis_iic_bridge 
        generic (
            CLK_PERIOD      : integer := 100000000 ;
            CLK_I2C_PERIOD  : integer := 25000000  ;
            N_BYTES         : integer := 32        ;
            WRITE_CONTROL   : string  := "COUNTER" ;
            DEPTH           : integer := 32         
        );
        port (
            clk               :  in   std_logic                                                                 ;
            reset             :  in   std_logic                                                                 ;
            
            s_axis_tdata      :  in   std_logic_vector ( ((N_BYTES*8)-1) downto 0 )                             ;
            s_axis_tuser      :  in   std_logic_vector (               7 downto 0 )                             ;
            s_axis_tkeep      :  in   std_logic_vector (       N_BYTES-1 downto 0 )                             ;
            s_axis_tvalid     :  in   std_logic                                                                 ;
            s_axis_tready     :  out  std_logic                                                                 ;
            s_axis_tlast      :  in   std_logic                                                                 ;

            m_axis_tdata      :  out  std_logic_vector ( ((N_BYTES*8)-1) downto 0 )                             ;
            m_axis_tkeep      :  out  std_logic_vector (       N_BYTES-1 downto 0 )                             ;
            m_axis_tuser      :  out  std_logic_vector (               7 downto 0 )                             ;
            m_axis_tvalid     :  out  std_logic                                                                 ;
            m_axis_tready     :  in   std_logic                                                                 ;
            m_axis_tlast      :  out  std_logic                                                                 ;
            
            scl_i             :  in   std_logic                                                                 ;
            sda_i             :  in   std_logic                                                                 ;
            scl_t             :  out  std_logic                                                                 ;
            sda_t             :  out  std_logic                                                                  
        );
    end component;

    signal  scl_i             :       std_logic     := '0'                                                      ;
    signal  sda_i             :       std_logic     := '0'                                                      ;
    signal  scl_t             :       std_logic                                                                 ;
    signal  sda_t             :       std_logic                                                                 ;

    signal RESET : std_logic    ;


    component tb_device_imitation 
        port (
            IIC_SCL_I               : in std_logic ;
            IIC_SDA_I               : in std_logic ;
            IIC_SCL_O               : out std_logic ;
            IIC_SDA_O               : out std_logic ;
            IRQ                     : out std_logic  
        );
    end component;

    signal  interrupt_i : integer := 0;

begin 

    CLK <= not CLK after clock_period/2;

    i_processing : process(CLK)
    begin
        if CLK'event AND CLK = '1' then 
            i <= i + 1;
        end if;
    end process;

    resetn <= '0' when i < 5 else '1';
    RESET <= not resetn;

    axi_adxl345_inst : axi_adxl345 
        generic map (
            S_AXI_LITE_DEV_DATA_WIDTH   =>  S_AXI_LITE_DEV_DATA_WIDTH                       ,
            S_AXI_LITE_DEV_ADDR_WIDTH   =>  S_AXI_LITE_DEV_ADDR_WIDTH                       ,
            DEFAULT_DEVICE_ADDRESS      =>  DEFAULT_DEVICE_ADDRESS                          ,
            DEFAULT_REQUESTION_INTERVAL =>  DEFAULT_REQUESTION_INTERVAL                        ,
            DEFAULT_CALIBRATION_MODE    =>  DEFAULT_CALIBRATION_MODE                        ,
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
    


    axis_iic_bridge_inst : axis_iic_bridge 
        generic map (
            CLK_PERIOD      =>  100000000 ,
            CLK_I2C_PERIOD  =>  400000    ,
            N_BYTES         =>  1         ,
            WRITE_CONTROL   =>  "COUNTER" ,
            DEPTH           =>  64         
        )
        port map (
            clk               =>  CLK                           ,
            reset             =>  RESET                         ,
            
            s_axis_tdata      =>  M_AXIS_TDATA                  , 
            s_axis_tkeep      =>  M_AXIS_TKEEP                  , 
            s_axis_tuser      =>  M_AXIS_TUSER                  , 
            s_axis_tvalid     =>  M_AXIS_TVALID                 , 
            s_axis_tlast      =>  M_AXIS_TLAST                  , 
            s_axis_tready     =>  M_AXIS_TREADY                 , 

            m_axis_tdata      =>  S_AXIS_TDATA                  ,
            m_axis_tkeep      =>  S_AXIS_TKEEP                  ,
            m_axis_tuser      =>  S_AXIS_TUSER                  ,
            m_axis_tvalid     =>  S_AXIS_TVALID                 ,
            m_axis_tlast      =>  S_AXIS_TLAST                  ,
            m_axis_tready     =>  S_AXIS_TREADY                 ,
            
            scl_i             =>  scl_i                         ,
            sda_i             =>  sda_i                         ,
            scl_t             =>  scl_t                         ,
            sda_t             =>  sda_t                          
        );




    tb_device_imitation_inst : tb_device_imitation 
        port map (
            IIC_SCL_I               =>  scl_t                         ,
            IIC_SDA_I               =>  sda_t                         ,
            IIC_SCL_O               =>  scl_i                         ,
            IIC_SDA_O               =>  sda_i                         ,
            IRQ                     =>  ADXL_INTERRUPT                 
        );



    configuration_write_processing : process(CLK)
    begin
        if CLK'event AND CLK = '1' then 
            if (ADXL_IRQ = '1') then 
                case (interrupt_i) is 
                    when    100000  => cfg_awaddr <= x"00"; cfg_awvalid <= '1'; cfg_wdata <= x"FFFFFF14"; cfg_wstrb <= x"1"; cfg_wvalid <= '1'; cfg_bready <= '1';
                    when    100001  => cfg_awaddr <= x"00"; cfg_awvalid <= '1'; cfg_wdata <= x"FFFFFF14"; cfg_wstrb <= x"1"; cfg_wvalid <= '1'; cfg_bready <= '1';
                    when    100002  => cfg_awaddr <= x"00"; cfg_awvalid <= '0'; cfg_wdata <= x"FFFFFF14"; cfg_wstrb <= x"1"; cfg_wvalid <= '0'; cfg_bready <= '1';
                    when    others  => cfg_awaddr <= cfg_awaddr; cfg_awvalid <= '0'; cfg_wdata <= cfg_wdata; cfg_wstrb <= cfg_wstrb; cfg_wvalid <= '0'; cfg_bready <= '0';
                end case;
            else
                case i is 

                    --when    300     => cfg_awaddr <= x"00"; cfg_awvalid <= '1'; cfg_wdata <= x"FFFFFF08"; cfg_wstrb <= x"1"; cfg_wvalid <= '1'; cfg_bready <= '1';
                    --when    301     => cfg_awaddr <= x"00"; cfg_awvalid <= '1'; cfg_wdata <= x"FFFFFF08"; cfg_wstrb <= x"1"; cfg_wvalid <= '1'; cfg_bready <= '1';
                    --when    302     => cfg_awaddr <= x"00"; cfg_awvalid <= '0'; cfg_wdata <= x"FFFFFF08"; cfg_wstrb <= x"1"; cfg_wvalid <= '0'; cfg_bready <= '1';

                    when  10000     => cfg_awaddr <= x"04"; cfg_awvalid <= '1'; cfg_wdata <= x"00000000"; cfg_wstrb <= x"F"; cfg_wvalid <= '1'; cfg_bready <= '1';
                    when  10001     => cfg_awaddr <= x"04"; cfg_awvalid <= '1'; cfg_wdata <= x"00000000"; cfg_wstrb <= x"F"; cfg_wvalid <= '1'; cfg_bready <= '1';
                    when  10002     => cfg_awaddr <= x"04"; cfg_awvalid <= '0'; cfg_wdata <= x"00000000"; cfg_wstrb <= x"F"; cfg_wvalid <= '0'; cfg_bready <= '1';

                    when  11000     => cfg_awaddr <= x"3C"; cfg_awvalid <= '1'; cfg_wdata <= x"00000632"; cfg_wstrb <= x"F"; cfg_wvalid <= '1'; cfg_bready <= '1';
                    when  11001     => cfg_awaddr <= x"3C"; cfg_awvalid <= '1'; cfg_wdata <= x"00000632"; cfg_wstrb <= x"F"; cfg_wvalid <= '1'; cfg_bready <= '1';
                    when  11002     => cfg_awaddr <= x"3C"; cfg_awvalid <= '0'; cfg_wdata <= x"00000632"; cfg_wstrb <= x"F"; cfg_wvalid <= '0'; cfg_bready <= '1';

                    when  20000     => cfg_awaddr <= x"00"; cfg_awvalid <= '1'; cfg_wdata <= x"00005308"; cfg_wstrb <= x"F"; cfg_wvalid <= '1'; cfg_bready <= '1';
                    when  20001     => cfg_awaddr <= x"00"; cfg_awvalid <= '1'; cfg_wdata <= x"00005308"; cfg_wstrb <= x"F"; cfg_wvalid <= '1'; cfg_bready <= '1';
                    when  20002     => cfg_awaddr <= x"00"; cfg_awvalid <= '0'; cfg_wdata <= x"00005308"; cfg_wstrb <= x"F"; cfg_wvalid <= '0'; cfg_bready <= '1';

                    when  190000    => cfg_awaddr <= x"04"; cfg_awvalid <= '1'; cfg_wdata <= x"00000001"; cfg_wstrb <= x"F"; cfg_wvalid <= '1'; cfg_bready <= '1';
                    when  190001    => cfg_awaddr <= x"04"; cfg_awvalid <= '1'; cfg_wdata <= x"00000001"; cfg_wstrb <= x"F"; cfg_wvalid <= '1'; cfg_bready <= '1';
                    when  190002    => cfg_awaddr <= x"04"; cfg_awvalid <= '0'; cfg_wdata <= x"00000001"; cfg_wstrb <= x"F"; cfg_wvalid <= '0'; cfg_bready <= '1';

                    when  200000    => cfg_awaddr <= x"00"; cfg_awvalid <= '1'; cfg_wdata <= x"00005302"; cfg_wstrb <= x"F"; cfg_wvalid <= '1'; cfg_bready <= '1';
                    when  200001    => cfg_awaddr <= x"00"; cfg_awvalid <= '1'; cfg_wdata <= x"00005302"; cfg_wstrb <= x"F"; cfg_wvalid <= '1'; cfg_bready <= '1';
                    when  200002    => cfg_awaddr <= x"00"; cfg_awvalid <= '0'; cfg_wdata <= x"00005302"; cfg_wstrb <= x"F"; cfg_wvalid <= '0'; cfg_bready <= '1';

                    when  600000    => cfg_awaddr <= x"00"; cfg_awvalid <= '1'; cfg_wdata <= x"00005302"; cfg_wstrb <= x"F"; cfg_wvalid <= '1'; cfg_bready <= '1';
                    when  600001    => cfg_awaddr <= x"00"; cfg_awvalid <= '1'; cfg_wdata <= x"00005302"; cfg_wstrb <= x"F"; cfg_wvalid <= '1'; cfg_bready <= '1';
                    when  600002    => cfg_awaddr <= x"00"; cfg_awvalid <= '0'; cfg_wdata <= x"00005302"; cfg_wstrb <= x"F"; cfg_wvalid <= '0'; cfg_bready <= '1';

                    when  601000    => cfg_awaddr <= x"08"; cfg_awvalid <= '1'; cfg_wdata <= x"00000104"; cfg_wstrb <= x"F"; cfg_wvalid <= '1'; cfg_bready <= '1';
                    when  601001    => cfg_awaddr <= x"08"; cfg_awvalid <= '1'; cfg_wdata <= x"00000104"; cfg_wstrb <= x"F"; cfg_wvalid <= '1'; cfg_bready <= '1';
                    when  601002    => cfg_awaddr <= x"08"; cfg_awvalid <= '0'; cfg_wdata <= x"00000104"; cfg_wstrb <= x"F"; cfg_wvalid <= '0'; cfg_bready <= '1';

                    when 2000000    => cfg_awaddr <= x"00"; cfg_awvalid <= '1'; cfg_wdata <= x"00005301"; cfg_wstrb <= x"F"; cfg_wvalid <= '1'; cfg_bready <= '1';
                    when 2000001    => cfg_awaddr <= x"00"; cfg_awvalid <= '1'; cfg_wdata <= x"00005301"; cfg_wstrb <= x"F"; cfg_wvalid <= '1'; cfg_bready <= '1';
                    when 2000002    => cfg_awaddr <= x"00"; cfg_awvalid <= '0'; cfg_wdata <= x"00005301"; cfg_wstrb <= x"F"; cfg_wvalid <= '0'; cfg_bready <= '1';

                    when 2100000    => cfg_awaddr <= x"00"; cfg_awvalid <= '1'; cfg_wdata <= x"00005308"; cfg_wstrb <= x"F"; cfg_wvalid <= '1'; cfg_bready <= '1';
                    when 2100001    => cfg_awaddr <= x"00"; cfg_awvalid <= '1'; cfg_wdata <= x"00005308"; cfg_wstrb <= x"F"; cfg_wvalid <= '1'; cfg_bready <= '1';
                    when 2100002    => cfg_awaddr <= x"00"; cfg_awvalid <= '0'; cfg_wdata <= x"00005308"; cfg_wstrb <= x"F"; cfg_wvalid <= '0'; cfg_bready <= '1';




                    when others     => cfg_awaddr <= cfg_awaddr; cfg_awprot <= cfg_awprot; cfg_awvalid <= '0'; cfg_wdata <= cfg_wdata; cfg_wstrb <= cfg_wstrb; cfg_wvalid <= '0'; cfg_bready <= '0';
                end case;
            end if;

        end if;
    end process;


    interrupt_i_processing : process(CLK)
    begin 
        if CLK'event AND CLK = '1' then 
            if ADXL_IRQ = '1' then 
                interrupt_i <= interrupt_i + 1;
            else 
                interrupt_i <= 0;
            end if;
        end if;
    end process;



    --device_write_processing : process(CLK)
    --begin
    --    if CLK'event AND CLK = '1' then 
    --        case i is 

    --            when  200000   => DEV_AWADDR <= x"2C"; DEV_AWVALID <= '1'; DEV_WDATA <= x"0001080E"; DEV_WSTRB <= x"F"; DEV_WVALID <= '1'; DEV_BREADY <= '1';
    --            when  200001   => DEV_AWADDR <= x"2C"; DEV_AWVALID <= '1'; DEV_WDATA <= x"0001080E"; DEV_WSTRB <= x"F"; DEV_WVALID <= '1'; DEV_BREADY <= '1';
    --            when  200002   => DEV_AWADDR <= x"2C"; DEV_AWVALID <= '0'; DEV_WDATA <= x"0001080E"; DEV_WSTRB <= x"F"; DEV_WVALID <= '0'; DEV_BREADY <= '1';

    --            when others     => DEV_AWADDR <= DEV_AWADDR; DEV_AWVALID <= '0'; DEV_WDATA <= (others => '0'); DEV_WSTRB <= DEV_WSTRB; DEV_WVALID <= '0'; DEV_BREADY <= '0';
    --        end case;
    --    end if;
    --end process;




    --read_processing : process(CLK)
    --begin
    --    if CLK'event AND CLK = '1' then 
    --        case i is 

    --            when 500000   => DEV_ARADDR <= x"00"; DEV_ARPROT <= "000"; DEV_ARVALID <= '1'; DEV_RREADY <= '1';
    --            when 500001   => DEV_ARADDR <= x"00"; DEV_ARPROT <= "000"; DEV_ARVALID <= '1'; DEV_RREADY <= '1';
    --            when 500002   => DEV_ARADDR <= x"00"; DEV_ARPROT <= "000"; DEV_ARVALID <= '0'; DEV_RREADY <= '1';

    --            when 500010   => DEV_ARADDR <= x"04"; DEV_ARPROT <= "000"; DEV_ARVALID <= '1'; DEV_RREADY <= '1';
    --            when 500011   => DEV_ARADDR <= x"04"; DEV_ARPROT <= "000"; DEV_ARVALID <= '1'; DEV_RREADY <= '1';
    --            when 500012   => DEV_ARADDR <= x"04"; DEV_ARPROT <= "000"; DEV_ARVALID <= '0'; DEV_RREADY <= '1';

    --            when others => DEV_ARADDR <= DEV_ARADDR; DEV_ARPROT <= DEV_ARPROT; DEV_ARVALID <= '0'; DEV_RREADY <= '0';
    --        end case;
    --    end if;
    --end process;






end architecture;