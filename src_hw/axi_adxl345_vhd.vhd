library IEEE;
    use IEEE.STD_LOGIC_1164.ALL;
    use ieee.std_logic_unsigned.all;
    use ieee.std_logic_arith.all;


entity axi_adxl345_vhd is
    generic (
        S_AXI_LITE_DEV_DATA_WIDTH   : integer  := 32        ;
        S_AXI_LITE_DEV_ADDR_WIDTH   : integer  := 32        ;
        DEFAULT_DEVICE_ADDRESS      : std_logic_Vector ( 6 downto 0 ) := "1010011"  ;
        DEFAULT_REQUESTION_INTERVAL : integer  := 1000      ;
        DEFAULT_CALIBRATION_MODE    : integer  := 8         ;
        S_AXI_LITE_CFG_DATA_WIDTH   : integer  := 32        ;
        S_AXI_LITE_CFG_ADDR_WIDTH   : integer  := 32        ;
        CLK_PERIOD                  : integer  := 100000000 ;
        RESET_DURATION              : integer  := 1000
    );
    port (
        CLK                         :   in      std_logic                                                                       ;
        RESETN                      :   in      std_logic                                                                       ;
        S_AXI_LITE_CFG_AWADDR       :   in      std_logic_vector (     S_AXI_LITE_CFG_ADDR_WIDTH-1 downto 0 )                   ;
        S_AXI_LITE_CFG_AWPROT       :   in      std_logic_vector (                               2 downto 0 )                   ;
        S_AXI_LITE_CFG_AWVALID      :   in      std_logic                                                                       ;
        S_AXI_LITE_CFG_AWREADY      :   out     std_logic                                                                       ;
        S_AXI_LITE_CFG_WDATA        :   in      std_logic_vector (     S_AXI_LITE_CFG_DATA_WIDTH-1 downto 0 )                   ;
        S_AXI_LITE_CFG_WSTRB        :   in      std_logic_vector ( (S_AXI_LITE_CFG_DATA_WIDTH/8)-1 downto 0 )                   ;
        S_AXI_LITE_CFG_WVALID       :   in      std_logic                                                                       ;
        S_AXI_LITE_CFG_WREADY       :   out     std_logic                                                                       ;
        S_AXI_LITE_CFG_BRESP        :   out     std_logic_vector (                               1 downto 0 )                   ;
        S_AXI_LITE_CFG_BVALID       :   out     std_logic                                                                       ;
        S_AXI_LITE_CFG_BREADY       :   in      std_logic                                                                       ;
        S_AXI_LITE_CFG_ARADDR       :   in      std_logic_vector (     S_AXI_LITE_CFG_ADDR_WIDTH-1 downto 0 )                   ;
        S_AXI_LITE_CFG_ARPROT       :   in      std_logic_vector (                               2 downto 0 )                   ;
        S_AXI_LITE_CFG_ARVALID      :   in      std_logic                                                                       ;
        S_AXI_LITE_CFG_ARREADY      :   out     std_logic                                                                       ;
        S_AXI_LITE_CFG_RDATA        :   out     std_logic_vector (     S_AXI_LITE_CFG_DATA_WIDTH-1 downto 0 )                   ;
        S_AXI_LITE_CFG_RRESP        :   out     std_logic_vector (                               1 downto 0 )                   ;
        S_AXI_LITE_CFG_RVALID       :   out     std_logic                                                                       ;
        S_AXI_LITE_CFG_RREADY       :   in      std_logic                                                                       ;
        S_AXI_LITE_DEV_AWADDR       :   in      std_logic_vector (     S_AXI_LITE_DEV_ADDR_WIDTH-1 downto 0 )                   ;
        S_AXI_LITE_DEV_AWPROT       :   in      std_logic_vector (                               2 downto 0 )                   ;
        S_AXI_LITE_DEV_AWVALID      :   in      std_logic                                                                       ;
        S_AXI_LITE_DEV_AWREADY      :   out     std_logic                                                                       ;
        S_AXI_LITE_DEV_WDATA        :   in      std_logic_vector (     S_AXI_LITE_DEV_DATA_WIDTH-1 downto 0 )                   ;
        S_AXI_LITE_DEV_WSTRB        :   in      std_logic_vector ( (S_AXI_LITE_DEV_DATA_WIDTH/8)-1 downto 0 )                   ;
        S_AXI_LITE_DEV_WVALID       :   in      std_logic                                                                       ;
        S_AXI_LITE_DEV_WREADY       :   out     std_logic                                                                       ;
        S_AXI_LITE_DEV_BRESP        :   out     std_logic_vector (                               1 downto 0 )                   ;
        S_AXI_LITE_DEV_BVALID       :   out     std_logic                                                                       ;
        S_AXI_LITE_DEV_BREADY       :   in      std_logic                                                                       ;
        S_AXI_LITE_DEV_ARADDR       :   in      std_logic_vector (     S_AXI_LITE_DEV_ADDR_WIDTH-1 downto 0 )                   ;
        S_AXI_LITE_DEV_ARPROT       :   in      std_logic_vector (                               2 downto 0 )                   ;
        S_AXI_LITE_DEV_ARVALID      :   in      std_logic                                                                       ;
        S_AXI_LITE_DEV_ARREADY      :   out     std_logic                                                                       ;
        S_AXI_LITE_DEV_RDATA        :   out     std_logic_vector (     S_AXI_LITE_DEV_DATA_WIDTH-1 downto 0 )                   ;
        S_AXI_LITE_DEV_RRESP        :   out     std_logic_vector (                               1 downto 0 )                   ;
        S_AXI_LITE_DEV_RVALID       :   out     std_logic                                                                       ;
        S_AXI_LITE_DEV_RREADY       :   in      std_logic                                                                       ;
        M_AXIS_TDATA                :   out     std_logic_vector (                               7 downto 0 )                   ;
        M_AXIS_TKEEP                :   out     std_logic_vector (                               0 downto 0 )                   ;
        M_AXIS_TUSER                :   out     std_logic_vector (                               7 downto 0 )                   ;
        M_AXIS_TVALID               :   out     std_logic                                                                       ;
        M_AXIS_TLAST                :   out     std_logic                                                                       ;
        M_AXIS_TREADY               :   in      std_logic                                                                       ;
        S_AXIS_TDATA                :   in      std_logic_vector (                               7 downto 0 )                   ;
        S_AXIS_TKEEP                :   in      std_logic_vector (                               0 downto 0 )                   ;
        S_AXIS_TUSER                :   in      std_logic_vector (                               7 downto 0 )                   ;
        S_AXIS_TVALID               :   in      std_logic                                                                       ;
        S_AXIS_TLAST                :   in      std_logic                                                                       ;
        S_AXIS_TREADY               :   out     std_logic                                                                       ;
        ADXL_INTERRUPT              :   in      std_logic                                                                       ;
        ADXL_IRQ                    :   out     std_Logic                                                                        
    );
    ATTRIBUTE X_INTERFACE_INFO : STRING;
    ATTRIBUTE X_INTERFACE_PARAMETER : STRING;
    ATTRIBUTE X_INTERFACE_INFO of ADXL_INTERRUPT: SIGNAL is "xilinx.com:signal:interrupt:1.0 ADXL_INTERRUPT INTERRUPT";
    ATTRIBUTE X_INTERFACE_PARAMETER of ADXL_INTERRUPT: SIGNAL is "SENSITIVITY EDGE_RISING" ;
    ATTRIBUTE X_INTERFACE_INFO of ADXL_IRQ: SIGNAL is "xilinx.com:signal:interrupt:1.0 ADXL_IRQ INTERRUPT";
    ATTRIBUTE X_INTERFACE_PARAMETER of ADXL_IRQ: SIGNAL is "SENSITIVITY EDGE_RISING";
end axi_adxl345_vhd;



architecture axi_adxl345_vhd_arch of axi_adxl345_vhd is

    component axi_adxl345 
        generic (
            S_AXI_LITE_DEV_DATA_WIDTH   : integer  := 32        ;
            S_AXI_LITE_DEV_ADDR_WIDTH   : integer  := 32        ;
            DEFAULT_DEVICE_ADDRESS      : std_logic_Vector ( 6 downto 0 ) := "1010011"  ;
            DEFAULT_REQUESTION_INTERVAL : integer  := 1000      ;
            DEFAULT_CALIBRATION_MODE    : integer  := 8         ;
            S_AXI_LITE_CFG_DATA_WIDTH   : integer  := 32        ;
            S_AXI_LITE_CFG_ADDR_WIDTH   : integer  := 32        ;
            CLK_PERIOD                  : integer  := 100000000 ;
            RESET_DURATION              : integer  := 1000
        );
        port (
            CLK                         :   in      std_logic                                                                       ;
            RESETN                      :   in      std_logic                                                                       ;
            S_AXI_LITE_CFG_AWADDR       :   in      std_logic_vector (     S_AXI_LITE_CFG_ADDR_WIDTH-1 downto 0 )                   ;
            S_AXI_LITE_CFG_AWPROT       :   in      std_logic_vector (                               2 downto 0 )                   ;
            S_AXI_LITE_CFG_AWVALID      :   in      std_logic                                                                       ;
            S_AXI_LITE_CFG_AWREADY      :   out     std_logic                                                                       ;
            S_AXI_LITE_CFG_WDATA        :   in      std_logic_vector (     S_AXI_LITE_CFG_DATA_WIDTH-1 downto 0 )                   ;
            S_AXI_LITE_CFG_WSTRB        :   in      std_logic_vector ( (S_AXI_LITE_CFG_DATA_WIDTH/8)-1 downto 0 )                   ;
            S_AXI_LITE_CFG_WVALID       :   in      std_logic                                                                       ;
            S_AXI_LITE_CFG_WREADY       :   out     std_logic                                                                       ;
            S_AXI_LITE_CFG_BRESP        :   out     std_logic_vector (                               1 downto 0 )                   ;
            S_AXI_LITE_CFG_BVALID       :   out     std_logic                                                                       ;
            S_AXI_LITE_CFG_BREADY       :   in      std_logic                                                                       ;
            S_AXI_LITE_CFG_ARADDR       :   in      std_logic_vector (     S_AXI_LITE_CFG_ADDR_WIDTH-1 downto 0 )                   ;
            S_AXI_LITE_CFG_ARPROT       :   in      std_logic_vector (                               2 downto 0 )                   ;
            S_AXI_LITE_CFG_ARVALID      :   in      std_logic                                                                       ;
            S_AXI_LITE_CFG_ARREADY      :   out     std_logic                                                                       ;
            S_AXI_LITE_CFG_RDATA        :   out     std_logic_vector (     S_AXI_LITE_CFG_DATA_WIDTH-1 downto 0 )                   ;
            S_AXI_LITE_CFG_RRESP        :   out     std_logic_vector (                               1 downto 0 )                   ;
            S_AXI_LITE_CFG_RVALID       :   out     std_logic                                                                       ;
            S_AXI_LITE_CFG_RREADY       :   in      std_logic                                                                       ;
            S_AXI_LITE_DEV_AWADDR       :   in      std_logic_vector (     S_AXI_LITE_DEV_ADDR_WIDTH-1 downto 0 )                   ;
            S_AXI_LITE_DEV_AWPROT       :   in      std_logic_vector (                               2 downto 0 )                   ;
            S_AXI_LITE_DEV_AWVALID      :   in      std_logic                                                                       ;
            S_AXI_LITE_DEV_AWREADY      :   out     std_logic                                                                       ;
            S_AXI_LITE_DEV_WDATA        :   in      std_logic_vector (     S_AXI_LITE_DEV_DATA_WIDTH-1 downto 0 )                   ;
            S_AXI_LITE_DEV_WSTRB        :   in      std_logic_vector ( (S_AXI_LITE_DEV_DATA_WIDTH/8)-1 downto 0 )                   ;
            S_AXI_LITE_DEV_WVALID       :   in      std_logic                                                                       ;
            S_AXI_LITE_DEV_WREADY       :   out     std_logic                                                                       ;
            S_AXI_LITE_DEV_BRESP        :   out     std_logic_vector (                               1 downto 0 )                   ;
            S_AXI_LITE_DEV_BVALID       :   out     std_logic                                                                       ;
            S_AXI_LITE_DEV_BREADY       :   in      std_logic                                                                       ;
            S_AXI_LITE_DEV_ARADDR       :   in      std_logic_vector (     S_AXI_LITE_DEV_ADDR_WIDTH-1 downto 0 )                   ;
            S_AXI_LITE_DEV_ARPROT       :   in      std_logic_vector (                               2 downto 0 )                   ;
            S_AXI_LITE_DEV_ARVALID      :   in      std_logic                                                                       ;
            S_AXI_LITE_DEV_ARREADY      :   out     std_logic                                                                       ;
            S_AXI_LITE_DEV_RDATA        :   out     std_logic_vector (     S_AXI_LITE_DEV_DATA_WIDTH-1 downto 0 )                   ;
            S_AXI_LITE_DEV_RRESP        :   out     std_logic_vector (                               1 downto 0 )                   ;
            S_AXI_LITE_DEV_RVALID       :   out     std_logic                                                                       ;
            S_AXI_LITE_DEV_RREADY       :   in      std_logic                                                                       ;
            M_AXIS_TDATA                :   out     std_logic_vector (                               7 downto 0 )                   ;
            M_AXIS_TKEEP                :   out     std_logic_vector (                               0 downto 0 )                   ;
            M_AXIS_TUSER                :   out     std_logic_vector (                               7 downto 0 )                   ;
            M_AXIS_TVALID               :   out     std_logic                                                                       ;
            M_AXIS_TLAST                :   out     std_logic                                                                       ;
            M_AXIS_TREADY               :   in      std_logic                                                                       ;
            S_AXIS_TDATA                :   in      std_logic_vector (                               7 downto 0 )                   ;
            S_AXIS_TKEEP                :   in      std_logic_vector (                               0 downto 0 )                   ;
            S_AXIS_TUSER                :   in      std_logic_vector (                               7 downto 0 )                   ;
            S_AXIS_TVALID               :   in      std_logic                                                                       ;
            S_AXIS_TLAST                :   in      std_logic                                                                       ;
            S_AXIS_TREADY               :   out     std_logic                                                                       ;
            ADXL_INTERRUPT              :   in      std_logic                                                                       ;
            ADXL_IRQ                    :   out     std_Logic                                                                        
        );
    end component;


begin

    axi_adxl345_inst : axi_adxl345 
        generic map (
            S_AXI_LITE_DEV_DATA_WIDTH   =>  S_AXI_LITE_DEV_DATA_WIDTH           ,
            S_AXI_LITE_DEV_ADDR_WIDTH   =>  S_AXI_LITE_DEV_ADDR_WIDTH           ,
            DEFAULT_DEVICE_ADDRESS      =>  DEFAULT_DEVICE_ADDRESS              ,
            DEFAULT_REQUESTION_INTERVAL =>  DEFAULT_REQUESTION_INTERVAL         ,
            DEFAULT_CALIBRATION_MODE    =>  DEFAULT_CALIBRATION_MODE            ,
            S_AXI_LITE_CFG_DATA_WIDTH   =>  S_AXI_LITE_CFG_DATA_WIDTH           ,
            S_AXI_LITE_CFG_ADDR_WIDTH   =>  S_AXI_LITE_CFG_ADDR_WIDTH           ,
            CLK_PERIOD                  =>  CLK_PERIOD                          ,
            RESET_DURATION              =>  RESET_DURATION                       

        )
        port map (
            CLK                         =>  CLK                                 ,
            RESETN                      =>  RESETN                              ,
            S_AXI_LITE_CFG_AWADDR       =>  S_AXI_LITE_CFG_AWADDR               ,
            S_AXI_LITE_CFG_AWPROT       =>  S_AXI_LITE_CFG_AWPROT               ,
            S_AXI_LITE_CFG_AWVALID      =>  S_AXI_LITE_CFG_AWVALID              ,
            S_AXI_LITE_CFG_AWREADY      =>  S_AXI_LITE_CFG_AWREADY              ,
            S_AXI_LITE_CFG_WDATA        =>  S_AXI_LITE_CFG_WDATA                ,
            S_AXI_LITE_CFG_WSTRB        =>  S_AXI_LITE_CFG_WSTRB                ,
            S_AXI_LITE_CFG_WVALID       =>  S_AXI_LITE_CFG_WVALID               ,
            S_AXI_LITE_CFG_WREADY       =>  S_AXI_LITE_CFG_WREADY               ,
            S_AXI_LITE_CFG_BRESP        =>  S_AXI_LITE_CFG_BRESP                ,
            S_AXI_LITE_CFG_BVALID       =>  S_AXI_LITE_CFG_BVALID               ,
            S_AXI_LITE_CFG_BREADY       =>  S_AXI_LITE_CFG_BREADY               ,
            S_AXI_LITE_CFG_ARADDR       =>  S_AXI_LITE_CFG_ARADDR               ,
            S_AXI_LITE_CFG_ARPROT       =>  S_AXI_LITE_CFG_ARPROT               ,
            S_AXI_LITE_CFG_ARVALID      =>  S_AXI_LITE_CFG_ARVALID              ,
            S_AXI_LITE_CFG_ARREADY      =>  S_AXI_LITE_CFG_ARREADY              ,
            S_AXI_LITE_CFG_RDATA        =>  S_AXI_LITE_CFG_RDATA                ,
            S_AXI_LITE_CFG_RRESP        =>  S_AXI_LITE_CFG_RRESP                ,
            S_AXI_LITE_CFG_RVALID       =>  S_AXI_LITE_CFG_RVALID               ,
            S_AXI_LITE_CFG_RREADY       =>  S_AXI_LITE_CFG_RREADY               ,
            S_AXI_LITE_DEV_AWADDR       =>  S_AXI_LITE_DEV_AWADDR               ,
            S_AXI_LITE_DEV_AWPROT       =>  S_AXI_LITE_DEV_AWPROT               ,
            S_AXI_LITE_DEV_AWVALID      =>  S_AXI_LITE_DEV_AWVALID              ,
            S_AXI_LITE_DEV_AWREADY      =>  S_AXI_LITE_DEV_AWREADY              ,
            S_AXI_LITE_DEV_WDATA        =>  S_AXI_LITE_DEV_WDATA                ,
            S_AXI_LITE_DEV_WSTRB        =>  S_AXI_LITE_DEV_WSTRB                ,
            S_AXI_LITE_DEV_WVALID       =>  S_AXI_LITE_DEV_WVALID               ,
            S_AXI_LITE_DEV_WREADY       =>  S_AXI_LITE_DEV_WREADY               ,
            S_AXI_LITE_DEV_BRESP        =>  S_AXI_LITE_DEV_BRESP                ,
            S_AXI_LITE_DEV_BVALID       =>  S_AXI_LITE_DEV_BVALID               ,
            S_AXI_LITE_DEV_BREADY       =>  S_AXI_LITE_DEV_BREADY               ,
            S_AXI_LITE_DEV_ARADDR       =>  S_AXI_LITE_DEV_ARADDR               ,
            S_AXI_LITE_DEV_ARPROT       =>  S_AXI_LITE_DEV_ARPROT               ,
            S_AXI_LITE_DEV_ARVALID      =>  S_AXI_LITE_DEV_ARVALID              ,
            S_AXI_LITE_DEV_ARREADY      =>  S_AXI_LITE_DEV_ARREADY              ,
            S_AXI_LITE_DEV_RDATA        =>  S_AXI_LITE_DEV_RDATA                ,
            S_AXI_LITE_DEV_RRESP        =>  S_AXI_LITE_DEV_RRESP                ,
            S_AXI_LITE_DEV_RVALID       =>  S_AXI_LITE_DEV_RVALID               ,
            S_AXI_LITE_DEV_RREADY       =>  S_AXI_LITE_DEV_RREADY               ,
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
            ADXL_IRQ                    =>  ADXL_IRQ                             

        );


end axi_adxl345_vhd_arch;
