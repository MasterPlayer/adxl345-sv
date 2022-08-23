`timescale 1 ns / 1 ps



module axi_adxl345 #(
    parameter integer       S_AXI_LITE_DEV_DATA_WIDTH   = 32       ,
    parameter integer       S_AXI_LITE_DEV_ADDR_WIDTH   = 32       ,
    parameter         [6:0] DEFAULT_DEVICE_ADDRESS      = 7'h53    ,
    parameter integer       DEFAULT_REQUESTION_INTERVAL = 1000     ,
    parameter integer       DEFAULT_CALIBRATION_MODE    = 8        ,
    parameter integer       S_AXI_LITE_CFG_DATA_WIDTH   = 32       ,
    parameter integer       S_AXI_LITE_CFG_ADDR_WIDTH   = 32       ,
    parameter integer       CLK_PERIOD                  = 100000000,
    parameter integer       RESET_DURATION              = 1000
) (
    input  logic                                     CLK                   ,
    input  logic                                     RESETN                ,
    // configuration bank address
    input  logic [    S_AXI_LITE_CFG_ADDR_WIDTH-1:0] S_AXI_LITE_CFG_AWADDR ,
    input  logic [                              2:0] S_AXI_LITE_CFG_AWPROT ,
    input  logic                                     S_AXI_LITE_CFG_AWVALID,
    output logic                                     S_AXI_LITE_CFG_AWREADY,
    input  logic [    S_AXI_LITE_CFG_DATA_WIDTH-1:0] S_AXI_LITE_CFG_WDATA  ,
    input  logic [(S_AXI_LITE_CFG_DATA_WIDTH/8)-1:0] S_AXI_LITE_CFG_WSTRB  ,
    input  logic                                     S_AXI_LITE_CFG_WVALID ,
    output logic                                     S_AXI_LITE_CFG_WREADY ,
    output logic [                              1:0] S_AXI_LITE_CFG_BRESP  ,
    output logic                                     S_AXI_LITE_CFG_BVALID ,
    input  logic                                     S_AXI_LITE_CFG_BREADY ,
    input  logic [    S_AXI_LITE_CFG_ADDR_WIDTH-1:0] S_AXI_LITE_CFG_ARADDR ,
    input  logic [                              2:0] S_AXI_LITE_CFG_ARPROT ,
    input  logic                                     S_AXI_LITE_CFG_ARVALID,
    output logic                                     S_AXI_LITE_CFG_ARREADY,
    output logic [    S_AXI_LITE_CFG_DATA_WIDTH-1:0] S_AXI_LITE_CFG_RDATA  ,
    output logic [                              1:0] S_AXI_LITE_CFG_RRESP  ,
    output logic                                     S_AXI_LITE_CFG_RVALID ,
    input  logic                                     S_AXI_LITE_CFG_RREADY ,
    // DEVICE ACCESS REGISTER UNIT
    input  logic [    S_AXI_LITE_DEV_ADDR_WIDTH-1:0] S_AXI_LITE_DEV_AWADDR ,
    input  logic [                              2:0] S_AXI_LITE_DEV_AWPROT ,
    input  logic                                     S_AXI_LITE_DEV_AWVALID,
    output logic                                     S_AXI_LITE_DEV_AWREADY,
    input  logic [    S_AXI_LITE_DEV_DATA_WIDTH-1:0] S_AXI_LITE_DEV_WDATA  ,
    input  logic [(S_AXI_LITE_DEV_DATA_WIDTH/8)-1:0] S_AXI_LITE_DEV_WSTRB  ,
    input  logic                                     S_AXI_LITE_DEV_WVALID ,
    output logic                                     S_AXI_LITE_DEV_WREADY ,
    output logic [                              1:0] S_AXI_LITE_DEV_BRESP  ,
    output logic                                     S_AXI_LITE_DEV_BVALID ,
    input  logic                                     S_AXI_LITE_DEV_BREADY ,
    input  logic [    S_AXI_LITE_DEV_ADDR_WIDTH-1:0] S_AXI_LITE_DEV_ARADDR ,
    input  logic [                              2:0] S_AXI_LITE_DEV_ARPROT ,
    input  logic                                     S_AXI_LITE_DEV_ARVALID,
    output logic                                     S_AXI_LITE_DEV_ARREADY,
    output logic [    S_AXI_LITE_DEV_DATA_WIDTH-1:0] S_AXI_LITE_DEV_RDATA  ,
    output logic [                              1:0] S_AXI_LITE_DEV_RRESP  ,
    output logic                                     S_AXI_LITE_DEV_RVALID ,
    input  logic                                     S_AXI_LITE_DEV_RREADY ,
    // data from device
    output logic [                              7:0] M_AXIS_TDATA          ,
    output logic [                              0:0] M_AXIS_TKEEP          ,
    output logic [                              7:0] M_AXIS_TUSER          ,
    output logic                                     M_AXIS_TVALID         ,
    output logic                                     M_AXIS_TLAST          ,
    input  logic                                     M_AXIS_TREADY         ,
    // data to device
    input  logic [                              7:0] S_AXIS_TDATA          ,
    input  logic [                              0:0] S_AXIS_TKEEP          ,
    input  logic [                              7:0] S_AXIS_TUSER          ,
    input  logic                                     S_AXIS_TVALID         ,
    input  logic                                     S_AXIS_TLAST          ,
    output logic                                     S_AXIS_TREADY         ,
    // interrupt signals to component/from component
    (* X_INTERFACE_INFO = "xilinx.com:signal:interrupt:1.0 ADXL_INTERRUPT INTERRUPT" *)
    (* X_INTERFACE_PARAMETER = "SENSITIVITY EDGE_RISING" *)
    input  logic                                     ADXL_INTERRUPT        ,
    (* X_INTERFACE_INFO = "xilinx.com:signal:interrupt:1.0 ADXL_IRQ INTERRUPT" *)
    (* X_INTERFACE_PARAMETER = "SENSITIVITY EDGE_RISING" *)
    output logic                                     ADXL_IRQ
);


    logic [S_AXI_LITE_CFG_ADDR_WIDTH-1:0] axi_awaddr_cfg ;
    logic                                 axi_awready_cfg;
    logic                                 axi_wready_cfg ;
    logic [                          1:0] axi_bresp_cfg  ;
    logic                                 axi_bvalid_cfg ;
    logic [S_AXI_LITE_CFG_ADDR_WIDTH-1:0] axi_araddr_cfg ;
    logic                                 axi_arready_cfg;
    logic [S_AXI_LITE_CFG_DATA_WIDTH-1:0] axi_rdata_cfg  ;
    logic [                          1:0] axi_rresp_cfg  ;
    logic                                 axi_rvalid_cfg ;

    logic [S_AXI_LITE_DEV_ADDR_WIDTH-1:0] axi_dev_awaddr ;
    logic                                 axi_dev_awready;
    logic                                 axi_dev_wready ;
    logic [                          1:0] axi_dev_bresp  ;
    logic                                 axi_dev_bvalid ;
    logic [S_AXI_LITE_DEV_ADDR_WIDTH-1:0] axi_dev_araddr ;
    logic                                 axi_dev_arready;
    logic [S_AXI_LITE_DEV_DATA_WIDTH-1:0] axi_dev_rdata  ;
    logic [                          1:0] axi_dev_rresp  ;
    logic                                 axi_dev_rvalid ;

    localparam integer ADDR_LSB_CFG          = (S_AXI_LITE_CFG_DATA_WIDTH/32) + 1;
    localparam integer OPT_MEM_ADDR_BITS_CFG = 5                                 ;
    localparam integer ADDR_LSB_DEV          = (S_AXI_LITE_DEV_DATA_WIDTH/32) + 1;
    localparam integer OPT_MEM_ADDR_BITS_DEV = 3                                 ;
    localparam integer DATA_WIDTH            = 8                                 ;
    localparam integer USER_WIDTH            = 8                                 ;

    (* dont_touch="true" *) logic                                 slv_reg_rden;
    (* dont_touch="true" *) logic                                 slv_reg_wren;
    (* dont_touch="true" *) logic [S_AXI_LITE_DEV_DATA_WIDTH-1:0] reg_data_out;
    (* dont_touch="true" *) logic                                 aw_en       ;

    (* dont_touch="true" *)logic [                         15:0][S_AXI_LITE_CFG_DATA_WIDTH-1:0] register_cfg     = '{default:'{default:0}};
    (* dont_touch="true" *)logic                                                                slv_reg_rden_cfg                          ;
    (* dont_touch="true" *)logic                                                                slv_reg_wren_cfg                          ;
    (* dont_touch="true" *)logic [S_AXI_LITE_CFG_DATA_WIDTH-1:0]                                reg_data_out_cfg                          ;
    (* dont_touch="true" *)logic                                                                aw_en_cfg                                 ;


    // typedef enum {
    //     IDLE_ST                         ,
    //     CHK_UPD_NEEDED_ST               ,
    //     SEND_WRITE_CMD_ST               ,

    //     INC_ADDR_ST                     ,
        
    //     TX_SEND_ADDR_PTR                ,
    //     TX_READ_REQUEST_ST              ,
    //     AWAIT_RECEIVE_DATA_ST           ,

    //     TX_WRITE_INT_SOURCE_PTR_ST      ,
    //     TX_READ_INT_SOURCE_ST           ,
    //     RX_INT_SOURCE_ST                ,
    //     INT_PROCESSING_ST               ,

    //     TX_WRITE_ACT_TAP_STATUS_PTR_ST  , 
    //     TX_READ_ACT_TAP_STATUS_ST       , 
    //     RX_ACT_TAP_STATUS_ST            , 

    //     TX_WRITE_INTR_DATA_PTR_ST       ,
    //     TX_READ_INTR_DATA_ST            ,
    //     RX_INTR_DATA_ST                 ,

    //     TX_WRITE_WM_FIFO_STS_PTR_ST     , 
    //     TX_READ_WM_FIFO_STS_ST          ,
    //     RX_WM_FIFO_STS_ST               ,
    //     TX_WRITE_WM_DATA_PTR_ST         , 
    //     TX_READ_WM_DATA_ST              ,
    //     RX_WM_DATA_ST                   ,

    //     CHECK_INTR_DEASSERT             ,  // 

    //     TX_WRITE_CALIB_OFS_CLEAR_ST     ,
    //     AWAIT_CALIB_TIMER_ST            ,
    //     TX_WRITE_CALIB_DATA_PTR_ST      , 
    //     TX_READ_CALIB_DATA_ST           , 
    //     RX_CALIB_DATA_ST                , 
    //     ADD_CALIB_CALC_ST               ,
    //     AVG_CALIB_CALC_ST               , 
    //     OFFSET_CALIB_CALC_ST            ,
    //     OFFSET_LSB_CALIB_CALC_ST        ,
    //     TX_WRITE_CALIB_OFS_ST            

    // } fsm;


    // (* dont_touch="true" *)logic [5:0] address            = '{default:0};
    // (* dont_touch="true" *)logic [3:0] write_cmd_word_cnt = '{default:0};


    logic [                         7:0] version_major        = 8'h02                   ; // read only,
    logic [                         7:0] version_minor        = 8'h01                   ; // read only,
    // logic                                link_on              = 1'b0                    ;
    // logic                                calibration_flaq     = 1'b0                    ;
    // logic                                perform_request_flaq = 1'b0                    ; // reg[0][3]
    // logic                                request_performed    = 1'b0                    ; // reg[0][6]
    // logic                                allow_irq            = 1'b0                    ; // reg[0][2]
    // logic                                enable               = 1'b0                    ; // reg[0][1]
    // logic [($clog2(RESET_DURATION)-1):0] reset_logic_timer    = 1'b0                    ; // reg[0][0]

    // logic [                        31:0] request_interval     = DEFAULT_REQUEST_INTERVAL;
    // logic [                        31:0] write_transactions   = '{default:0}            ;
    // logic [                        31:0] read_transactions    = '{default:0}            ;
    // logic [                        31:0] transactions_timer   = '{default:0}            ;


    // logic refresh_after_calib_flaq = 1'b0;
    // // Interrupt data
    // logic [7:0] int_source_reg = '{default:0};
    // logic [7:0] int_enable_reg = '{default:0};


    // (* dont_touch="true" *)logic has_dataready_intr;
    // (* dont_touch="true" *)logic has_st_intr;
    // (* dont_touch="true" *)logic has_dt_intr;
    // (* dont_touch="true" *)logic has_act_intr;
    // (* dont_touch="true" *)logic has_inact_intr;
    // (* dont_touch="true" *)logic has_ff_intr;
    // (* dont_touch="true" *)logic has_wm_intr;
    // (* dont_touch="true" *)logic has_ovrrn_intr;

    // logic [5:0] entries = '{default:0};

    // logic [31:0] calibration_elapsed_time = '{default:0};


    // renew 

    logic                              reset       = 1'b0        ;
    logic [$clog2(RESET_DURATION)-1:0] reset_timer = '{default:0};

    logic [6:0] i2c_address = DEFAULT_DEVICE_ADDRESS; // reg[0][14:8]

    // single requesting data from device
    (* dont_touch="true" *)logic single_request          = 1'b0;
    (* dont_touch="true" *)logic [7:0] single_request_address;
    (* dont_touch="true" *)logic [7:0] single_request_size   ;
    (* dont_touch="true" *)logic single_request_complete; // output signal from functional 
    (* dont_touch="true" *)logic single_request_complete_flaq = 1'b0; // signal to register space for reading from device


    // periodic requests for interval timer
    logic            enable_interval_requestion = 1'b0        ;
    logic [3:0][7:0] requestion_interval        = '{default:'{default:0}};

    // allow interrupt mech 
    logic allow_irq_reg = 1'b0;

    // link state
    logic link_on;

    //
    logic adxl_irq_ack;


    logic        calibration          = 1'b0        ;
    logic [ 4:0] calibration_mode                   ;
    logic        calibration_complete               ;
    logic [63:0] calibration_time                   ;

    logic [47:0] opt_request_interval;

    logic [31:0] read_valid_count ;
    logic [31:0] write_valid_count;

    logic [63:0] write_transactions;
    logic [63:0] read_transactions ;

    logic        on_work                            ;


    // always_comb begin : has_dataready_intr_proc
    //     has_dataready_intr = int_source_reg[7] & int_enable_reg[7];
    // end 

    // always_comb begin : has_st_intr_proc
    //     has_st_intr = int_source_reg[6] & int_enable_reg[6];
    // end 

    // always_comb begin : has_dt_intr_proc 
    //     has_dt_intr = int_source_reg[5] & int_enable_reg[5];
    // end 

    // always_comb begin : has_act_intr_proc
    //     has_act_intr = int_source_reg[4] & int_enable_reg[4];
    // end 

    // always_comb begin : has_inact_intr_proc
    //     has_inact_intr = int_source_reg[3] & int_enable_reg[3];
    // end 

    // always_comb begin : has_ff_intr_proc
    //     has_ff_intr = int_source_reg[2] & int_enable_reg[2];
    // end 

    // always_comb begin : has_wm_intr_proc
    //     has_wm_intr = int_source_reg[1] & int_enable_reg[1];
    // end 

    // always_comb begin : has_ovrrn_intr_proc
    //     has_ovrrn_intr = int_source_reg[0] & int_enable_reg[0];
    // end 


    // always_comb begin 
    //     int_enable_reg = register[11][2];
    // end 



    always_comb begin : S_AXI_LITE_DEV_processing 

        S_AXI_LITE_DEV_AWREADY = axi_dev_awready;
        S_AXI_LITE_DEV_WREADY  = axi_dev_wready;
        S_AXI_LITE_DEV_BRESP   = axi_dev_bresp;
        S_AXI_LITE_DEV_BVALID  = axi_dev_bvalid;
        S_AXI_LITE_DEV_ARREADY = axi_dev_arready;
        S_AXI_LITE_DEV_RDATA   = axi_dev_rdata;
        S_AXI_LITE_DEV_RRESP   = axi_dev_rresp;
    end 



    always_comb begin : S_AXI_LITE_CFG_processing 

        S_AXI_LITE_CFG_AWREADY = axi_awready_cfg;
        S_AXI_LITE_CFG_WREADY  = axi_wready_cfg;
        S_AXI_LITE_CFG_BRESP   = axi_bresp_cfg;
        S_AXI_LITE_CFG_BVALID  = axi_bvalid_cfg;
        S_AXI_LITE_CFG_ARREADY = axi_arready_cfg;
        S_AXI_LITE_CFG_RDATA   = axi_rdata_cfg;
        S_AXI_LITE_CFG_RRESP   = axi_rresp_cfg;
    end 



    always_ff @( posedge CLK ) begin : S_AXI_LITE_CFG_RVALID_proc
        
        S_AXI_LITE_CFG_RVALID <= axi_rvalid_cfg;
    end 



    always_comb begin : S_AXI_LITE_DEV_RVALID_proc

        S_AXI_LITE_DEV_RVALID = axi_dev_rvalid;
    end 



    always_ff @( posedge CLK ) begin : axi_dev_awready_proc
        if (~RESETN)
            axi_dev_awready <= 1'b0;
        else    
            if (~axi_dev_awready & S_AXI_LITE_DEV_AWVALID & S_AXI_LITE_DEV_WVALID & aw_en)
                axi_dev_awready <= 1'b1;
            else 
                if (S_AXI_LITE_DEV_BREADY & axi_dev_bvalid)
                    axi_dev_awready <= 1'b0;
                else
                    axi_dev_awready <= 1'b0;
    end       



    always_ff @( posedge CLK ) begin : aw_en_proc
        if (~RESETN)
            aw_en <= 1'b1;
        else
            if (~axi_dev_awready & S_AXI_LITE_DEV_AWVALID & S_AXI_LITE_DEV_WVALID & aw_en)
                aw_en <= 1'b0;
            else 
                if (S_AXI_LITE_DEV_BREADY & axi_dev_bvalid)
                    aw_en <= 1'b1;
    end       



    always_ff @( posedge CLK ) begin : axi_dev_awaddr_proc
        if (~RESETN)
            axi_dev_awaddr <= '{default:0};
        else
            if (~axi_dev_awready & S_AXI_LITE_DEV_AWVALID & S_AXI_LITE_DEV_WVALID & aw_en)
                axi_dev_awaddr <= S_AXI_LITE_DEV_AWADDR;
    end       



    always_ff @( posedge CLK ) begin : axi_dev_wready_proc
        if (~RESETN)
            axi_dev_wready <= 1'b0;
        else    
            if (~axi_dev_wready & S_AXI_LITE_DEV_WVALID & S_AXI_LITE_DEV_AWVALID & aw_en )
                axi_dev_wready <= 1'b1;
            else
                axi_dev_wready <= 1'b0;
    end       



    always_comb begin : slv_reg_wren_processing

        slv_reg_wren = axi_dev_wready & S_AXI_LITE_DEV_WVALID & axi_dev_awready & S_AXI_LITE_DEV_AWVALID;
    end



    always_ff @( posedge CLK ) begin : axi_dev_bvalid_proc
        if (~RESETN)
            axi_dev_bvalid  <= 1'b0;
        else
            if (axi_dev_awready & S_AXI_LITE_DEV_AWVALID & ~axi_dev_bvalid & axi_dev_wready & S_AXI_LITE_DEV_WVALID)
                axi_dev_bvalid <= 1'b1;
            else
                if (S_AXI_LITE_DEV_BREADY && axi_dev_bvalid)
                    axi_dev_bvalid <= 1'b0; 
    end   



    always_ff @( posedge CLK ) begin : axi_dev_bresp_proc
        if (~RESETN)
            axi_dev_bresp <= '{default:0};
        else
            if (axi_dev_awready & S_AXI_LITE_DEV_AWVALID & ~axi_dev_bvalid & axi_dev_wready & S_AXI_LITE_DEV_WVALID)
                axi_dev_bresp  <= 2'b0; // 'OKAY' response 
    end   

///////////////////////////////////////////// READ INTERFACE SIGNALS /////////////////////////////////////////////

    always_ff @( posedge CLK ) begin : axi_dev_arready_proc
        if (~RESETN)
            axi_dev_arready <= 1'b0;
        else    
            if (~axi_dev_arready & S_AXI_LITE_DEV_ARVALID)
                axi_dev_arready <= 1'b1;
            else
                axi_dev_arready <= 1'b0;
    end       



    always_ff @( posedge CLK ) begin : axi_dev_araddr_proc
        if (~RESETN)
            axi_dev_araddr  <= 32'b0;
        else    
            if (~axi_dev_arready & S_AXI_LITE_DEV_ARVALID)
                axi_dev_araddr  <= S_AXI_LITE_DEV_ARADDR;  
    end       



    always_ff @( posedge CLK ) begin : axi_dev_rvalid_proc
        if (~RESETN)
            axi_dev_rvalid <= 1'b0;
        else
            if (axi_dev_arready & S_AXI_LITE_DEV_ARVALID & ~axi_dev_rvalid)
                axi_dev_rvalid <= 1'b1;
            else 
                if (axi_dev_rvalid & S_AXI_LITE_DEV_RREADY)
                    axi_dev_rvalid <= 1'b0;
    end    



    always_ff @( posedge CLK ) begin : axi_dev_rresp_proc
        if (~RESETN)
            axi_dev_rresp  <= 1'b0;
        else
            if (axi_dev_arready & S_AXI_LITE_DEV_ARVALID & ~axi_dev_rvalid)
                axi_dev_rresp  <= 2'b0; // 'OKAY' response             
    end    



    always_ff @(posedge CLK) begin : slv_reg_rden_proc 

        slv_reg_rden <= axi_dev_arready & S_AXI_LITE_DEV_ARVALID & ~axi_dev_rvalid;
    end 



    always_comb begin : axi_dev_rdata_proc

        axi_dev_rdata = reg_data_out;     // register read data
    end    


////////////////////////////////////////////////////// INTERNAL LOGIC SIGNALS //////////////////////////////////////////////////////




    // always_ff @(posedge CLK) begin : current_state_proc 
    //     if (~RESETN | reset) 
    //         current_state <= IDLE_ST;
    //     else 
    //         case (current_state)

    //             IDLE_ST : 
    //                 // if (calibration_flaq) 
    //                 //     current_state <= TX_WRITE_CALIB_OFS_CLEAR_ST;
    //                 // else 
    //                     // if (ADXL_INTERRUPT & allow_irq)  
    //                     //     current_state <= TX_WRITE_INT_SOURCE_PTR_ST;
    //                     // else 
    //                         // if (update_request) 
    //                         //     current_state <= CHK_UPD_NEEDED_ST;
    //                         // else 
    //                             if (perform_request_flaq | refresh_after_calib_flaq) begin 
    //                                 current_state <= TX_SEND_ADDR_PTR;
    //                             end else begin 
    //                                 current_state <= IDLE_ST;
    //                             end  
    //                                 // if (enable) 
    //                                 //     if (request_timer == request_interval) 
    //                                 //         current_state <= TX_SEND_ADDR_PTR;
    //                                 //     else  
    //                                 //         current_state <= IDLE_ST; 
    //                                 // else  
        

    //             // CHK_UPD_NEEDED_ST : 
    //             //     if (need_update_reg[address[5:2]][address[1:0]])
    //             //         current_state <= SEND_WRITE_CMD_ST;
    //             //     else 
    //             //         current_state <= INC_ADDR_ST;

    //             // SEND_WRITE_CMD_ST  : 
    //             //     if (!out_awfull)
    //             //        if (write_cmd_word_cnt == 4'h2)
    //             //             current_state <= INC_ADDR_ST;
    //             //         else 
    //             //             current_state <= current_state;
    //             //     else 
    //             //         current_state <= current_state;

    //             // INC_ADDR_ST  : 
    //             //     if (address == ADDRESS_LIMIT) 
    //             //         current_state <= IDLE_ST;
    //             //     else 
    //             //         current_state <= CHK_UPD_NEEDED_ST;

    //             TX_SEND_ADDR_PTR: 
    //                 if (!out_awfull)
    //                     if (write_cmd_word_cnt == 4'h1)
    //                         current_state <= TX_READ_REQUEST_ST;
    //                     else 
    //                         current_state <= current_state;
    //                 else 
    //                     current_state <= current_state;

    //             TX_READ_REQUEST_ST : 
    //                 if (!out_awfull) 
    //                     current_state <= AWAIT_RECEIVE_DATA_ST;
    //                 else 
    //                     current_state <= current_state;

    //             AWAIT_RECEIVE_DATA_ST : 
    //                 if (S_AXIS_TVALID & S_AXIS_TLAST)
    //                     current_state <= IDLE_ST;
    //                 else 
    //                     current_state <= current_state;

    //             // TX_WRITE_INT_SOURCE_PTR_ST : 
    //             //     if (!out_awfull) 
    //             //         if (write_cmd_word_cnt == 4'h1) 
    //             //             current_state <= TX_READ_INT_SOURCE_ST;
    //             //         else 
    //             //             current_state <= current_state;
    //             //     else 
    //             //         current_state <= current_state;


    //             // TX_READ_INT_SOURCE_ST : 
    //             //     if (!out_awfull) 
    //             //         current_state <= RX_INT_SOURCE_ST;
    //             //     else 
    //             //         current_state <= current_state;


    //             // RX_INT_SOURCE_ST : 
    //             //     if (S_AXIS_TVALID)  
    //             //         if (S_AXIS_TLAST) 
    //             //             current_state <= INT_PROCESSING_ST;
    //             //         else 
    //             //             current_state <= current_state;
    //             //     else 
    //             //         current_state <= current_state;
                    

    //             // INT_PROCESSING_ST : 
    //             //     if (has_st_intr | has_dt_intr | has_act_intr | has_inact_intr)
    //             //         current_state <= TX_WRITE_ACT_TAP_STATUS_PTR_ST;
    //             //     else 
    //             //         if (has_dataready_intr | has_ff_intr)
    //             //             current_state <= TX_WRITE_INTR_DATA_PTR_ST;
    //             //         else
    //             //             if (has_wm_intr | has_ovrrn_intr)
    //             //                 current_state <= TX_WRITE_WM_FIFO_STS_PTR_ST;
    //             //             else     
    //             //                 current_state <= IDLE_ST;

    //             // SINGLE/DOUBLE TAP interrupt processsing states

    //             // TX_WRITE_ACT_TAP_STATUS_PTR_ST: 
    //             //     if (!out_awfull) 
    //             //         if (write_cmd_word_cnt == 4'h1) 
    //             //             current_state <= TX_READ_ACT_TAP_STATUS_ST;
    //             //         else 
    //             //             current_state <= current_state;
    //             //     else 
    //             //         current_state <= current_state;


    //             // TX_READ_ACT_TAP_STATUS_ST: 
    //             //     if (!out_awfull)
    //             //         current_state <= RX_ACT_TAP_STATUS_ST;
    //             //     else 
    //             //         current_state <= current_state;

    //             // RX_ACT_TAP_STATUS_ST: 
    //             //     if (S_AXIS_TVALID & S_AXIS_TLAST)
    //             //         current_state <= TX_WRITE_INTR_DATA_PTR_ST;
    //             //     else 
    //             //         current_state <= current_state;

    //             // TX_WRITE_INTR_DATA_PTR_ST: 
    //             //     if (!out_awfull)
    //             //         if (write_cmd_word_cnt == 4'h1) 
    //             //             current_state <= TX_READ_INTR_DATA_ST;
    //             //         else 
    //             //             current_state <= current_state;
    //             //     else 
    //             //         current_state <= current_state;
                    


    //             // TX_READ_INTR_DATA_ST: 
    //             //     if (!out_awfull)
    //             //         current_state <= RX_INTR_DATA_ST;
    //             //     else 
    //             //         current_state <= current_state;


    //             // RX_INTR_DATA_ST: 
    //             //     if (S_AXIS_TVALID & S_AXIS_TLAST)
    //             //         current_state <= CHECK_INTR_DEASSERT;
    //             //     else 
    //             //         current_state <= current_state;

    //             // TX_WRITE_WM_FIFO_STS_PTR_ST : 
    //             //     if (!out_awfull) 
    //             //         if (write_cmd_word_cnt == 4'h1) 
    //             //             current_state <= TX_READ_WM_FIFO_STS_ST;
    //             //         else 
    //             //             current_state <= current_state;
    //             //     else 
    //             //         current_state <= current_state;

    //             // TX_READ_WM_FIFO_STS_ST : 
    //             //     if (!out_awfull) 
    //             //         current_state <= RX_WM_FIFO_STS_ST;
    //             //     else 
    //             //         current_state <= current_state;


    //             // RX_WM_FIFO_STS_ST : 
    //             //     if (S_AXIS_TVALID & S_AXIS_TLAST)
    //             //         current_state <= TX_WRITE_WM_DATA_PTR_ST;
    //             //     else 
    //             //         current_state <= current_state;

    //             // TX_WRITE_WM_DATA_PTR_ST : 
    //             //     if (!out_awfull) 
    //             //         if (write_cmd_word_cnt == 4'h1) 
    //             //             current_state <= TX_READ_WM_DATA_ST;
    //             //         else 
    //             //             current_state <= current_state;
    //             //     else 
    //             //         current_state <= current_state;


    //             // TX_READ_WM_DATA_ST : 
    //             //     if (!out_awfull) 
    //             //         current_state <= RX_WM_DATA_ST;
    //             //     else 
    //             //         current_state <= current_state;

    //             // RX_WM_DATA_ST : 
    //             //     if (S_AXIS_TVALID & S_AXIS_TLAST)  
    //             //         if (!entries) 
    //             //             current_state <= CHECK_INTR_DEASSERT;
    //             //         else  
    //             //             current_state <= TX_WRITE_WM_DATA_PTR_ST;
    //             //     else 
    //             //         current_state <= current_state;

    //             // CHECK_INTR_DEASSERT: 
    //             //     if (ADXL_INTERRUPT) 
    //             //         current_state <= TX_WRITE_INT_SOURCE_PTR_ST;
    //             //         // current_state <= INT_PROCESSING_ST;
    //             //     else 
    //             //         current_state <= IDLE_ST;


    //             // TX_WRITE_CALIB_OFS_CLEAR_ST: 
    //             //     if (!out_awfull) 
    //             //         if (write_cmd_word_cnt == 4'h4)
    //             //             current_state <= AWAIT_CALIB_TIMER_ST;
    //             //         else 
    //             //             current_state <= current_state;
    //             //     else 
    //             //         current_state <= current_state;

    //             // AWAIT_CALIB_TIMER_ST : 
    //             //     if (calibration_timer < opt_request_interval) 
    //             //         current_state <= current_state;
    //             //     else 
    //             //         current_state <= TX_WRITE_CALIB_DATA_PTR_ST;

    //             // TX_WRITE_CALIB_DATA_PTR_ST : 
    //             //     if (!out_awfull)
    //             //         if (write_cmd_word_cnt == 4'h1) 
    //             //             current_state <= TX_READ_CALIB_DATA_ST;
    //             //         else 
    //             //             current_state <= current_state;
    //             //     else 
    //             //         current_state <= current_state;

    //             // TX_READ_CALIB_DATA_ST: 
    //             //     if (!out_awfull) 
    //             //         current_state <= RX_CALIB_DATA_ST;
    //             //     else 
    //             //         current_state <= current_state;

    //             // RX_CALIB_DATA_ST : 
    //             //     if (S_AXIS_TVALID & S_AXIS_TLAST)  
    //             //         current_state <= ADD_CALIB_CALC_ST;
    //             //     else 
    //             //         current_state <= current_state;

    //             // ADD_CALIB_CALC_ST : 
    //             //     if (calibration_count == calibration_count_limit_reg)
    //             //         current_state <= AVG_CALIB_CALC_ST;
    //             //     else 
    //             //         current_state <= AWAIT_CALIB_TIMER_ST;

    //             // AVG_CALIB_CALC_ST : 
    //             //     current_state <= OFFSET_CALIB_CALC_ST;

    //             // OFFSET_CALIB_CALC_ST : 
    //             //     current_state <= OFFSET_LSB_CALIB_CALC_ST;

    //             // OFFSET_LSB_CALIB_CALC_ST : 
    //             //     current_state <= TX_WRITE_CALIB_OFS_ST;

    //             // TX_WRITE_CALIB_OFS_ST : 
    //             //     if (!out_awfull) 
    //             //         if (write_cmd_word_cnt == 4'h4)
    //             //             current_state <= IDLE_ST;
    //             //         else 
    //             //             current_state <= current_state;
    //             //     else 
    //             //         current_state <= current_state;

    //             default : 
    //                 current_state <= current_state;

    //         endcase // current_state

    // end

    // always_ff @(posedge CLK) begin : address_proc 
    //     if (~RESETN) 
    //         address  <= '{default:0};
    //     else 
    //         case (current_state)

    //             IDLE_ST : 
    //                 address <= '{default:0};

    //             // INC_ADDR_ST : 
    //             //     address <= address + 1;

    //             AWAIT_RECEIVE_DATA_ST : 
    //                 if (S_AXIS_TVALID)
    //                     address <= address + 1;

    //             // TX_READ_INT_SOURCE_ST : 
    //             //     address <= 8'h30;

    //             // TX_WRITE_ACT_TAP_STATUS_PTR_ST: 
    //             //     address <= 8'h2B;   

    //             // TX_WRITE_INTR_DATA_PTR_ST: 
    //             //     address <= 8'h32;

    //             // TX_WRITE_WM_FIFO_STS_PTR_ST:
    //             //     address <= 8'h39;

    //             // RX_INTR_DATA_ST: 
    //             //     if (S_AXIS_TVALID)
    //             //         address <= address + 1;

    //             // TX_WRITE_WM_DATA_PTR_ST: 
    //             //     address <= 8'h32;

    //             // RX_WM_DATA_ST: 
    //             //     if (S_AXIS_TVALID)
    //             //         address <= address + 1;
                
    //             // TX_WRITE_CALIB_DATA_PTR_ST : 
    //             //     address <= 8'h32;

    //             // RX_CALIB_DATA_ST : 
    //             //     if (S_AXIS_TVALID)
    //             //         address <= address + 1;

    //             default : 
    //                 address <= address;

    //         endcase // current_state
    // end 

    // always_ff @(posedge CLK) begin : out_din_data_proc
    //     case (current_state)
    //         // SEND_WRITE_CMD_ST : 
    //         //     case(write_cmd_word_cnt)
    //         //         4'h0 : 
    //         //             out_din_data <= 8'h02;

    //         //         4'h1 : 
    //         //             out_din_data <= {2'b00, address};

    //         //         4'h2 : 
    //         //             out_din_data <= register[address[5:2]][address[1:0]];

    //         //         default : 
    //         //             out_din_data <= out_din_data;

    //         //     endcase // write_cmd_word_cnt

    //         TX_SEND_ADDR_PTR : 
    //             case (write_cmd_word_cnt)
    //                 4'h0   : out_din_data <= 8'h01;
    //                 4'h1   : out_din_data <= 8'h00;
    //                 default : out_din_data <= out_din_data;
    //             endcase // write_cmd_word_cnt

    //         TX_READ_REQUEST_ST : 
    //             out_din_data <= ADDRESS_LIMIT;

    //         // TX_WRITE_INT_SOURCE_PTR_ST:
    //         //     case (write_cmd_word_cnt)
    //         //         4'h0 : out_din_data <= 8'h01;
    //         //         4'h1 : out_din_data <= 8'h30;
    //         //         default : out_din_data <= out_din_data;
    //         //     endcase // write_cmd_word_cnt

    //         // TX_READ_INT_SOURCE_ST: 
    //         //     out_din_data <= 8'h01;




    //         // TX_WRITE_ACT_TAP_STATUS_PTR_ST: 
    //         //     case (write_cmd_word_cnt)
    //         //         4'h0 : out_din_data <= 8'h01;
    //         //         4'h1 : out_din_data <= 8'h2B;
    //         //         default : out_din_data <= out_din_data;
    //         //     endcase // write_cmd_word_cnt

    //         // TX_READ_ACT_TAP_STATUS_ST: 
    //         //     out_din_data <= 8'h01;

    //         // TX_WRITE_INTR_DATA_PTR_ST: 
    //         //     case (write_cmd_word_cnt)
    //         //         4'h0 : out_din_data <= 8'h01;
    //         //         4'h1 : out_din_data <= 8'h32;
    //         //         default : out_din_data <= out_din_data;
    //         //     endcase // write_cmd_word_cnt

    //         // TX_READ_INTR_DATA_ST: 
    //         //     out_din_data <= 8'h06;


    //         // TX_WRITE_WM_FIFO_STS_PTR_ST: 
    //         //     case (write_cmd_word_cnt)
    //         //         4'h0 : out_din_data <= 8'h01;
    //         //         4'h1 : out_din_data <= 8'h39;
    //         //         default : out_din_data <= out_din_data;
    //         //     endcase // write_cmd_word_cnt

    //         // TX_READ_WM_FIFO_STS_ST: 
    //         //     out_din_data <= 8'h01;


    //         // TX_WRITE_WM_DATA_PTR_ST: 
    //         //     case (write_cmd_word_cnt)
    //         //         4'h0 : out_din_data <= 8'h01;
    //         //         4'h1 : out_din_data <= 8'h32;
    //         //         default : out_din_data <= out_din_data;
    //         //     endcase // write_cmd_word_cnt

    //         // TX_READ_WM_DATA_ST: 
    //         //     out_din_data <= 8'h06;



    //         // TX_WRITE_CALIB_DATA_PTR_ST: 
    //         //     case (write_cmd_word_cnt)
    //         //         4'h0 : out_din_data <= 8'h01;
    //         //         4'h1 : out_din_data <= 8'h32;
    //         //         default : out_din_data <= out_din_data;
    //         //     endcase // write_cmd_word_cnt

    //         // TX_READ_CALIB_DATA_ST: 
    //         //     out_din_data <= 8'h06;


    //         // TX_WRITE_CALIB_OFS_ST: 
    //         //     case (write_cmd_word_cnt)
    //         //         4'h0 : out_din_data <= 8'h04;
    //         //         4'h1 : out_din_data <= 8'h1E;
    //         //         4'h2 : out_din_data <= offset_lsb_x;
    //         //         4'h3 : out_din_data <= offset_lsb_y;
    //         //         4'h4 : out_din_data <= offset_lsb_z;
    //         //         default : out_din_data <= out_din_data;
    //         //     endcase // write_cmd_word_cnt


    //         // TX_WRITE_CALIB_OFS_CLEAR_ST: 
    //         //     case (write_cmd_word_cnt)
    //         //         4'h0 : out_din_data <= 8'h04;
    //         //         4'h1 : out_din_data <= 8'h1E;
    //         //         4'h2 : out_din_data <= 8'h00;
    //         //         4'h3 : out_din_data <= 8'h00;
    //         //         4'h4 : out_din_data <= 8'h00;
    //         //         default : out_din_data <= out_din_data;
    //         //     endcase // write_cmd_word_cnt




    //         default : 
    //             out_din_data <= out_din_data;

    //     endcase // current_state
    // end 

    // always_ff @(posedge CLK) begin : out_wren_proc
    //     case (current_state)
    //         // SEND_WRITE_CMD_ST : 
    //         //     if (!out_awfull) 
    //         //         out_wren <= 1'b1;
    //         //     else 
    //         //         out_wren <= 1'b0;

    //         TX_READ_REQUEST_ST: 
    //             if (!out_awfull) 
    //                 out_wren <= 1'b1;
    //             else 
    //                 out_wren <= 1'b0;

    //         TX_SEND_ADDR_PTR : 
    //             if (!out_awfull)
    //                 out_wren <= 1'b1;
    //             else 
    //                 out_wren <= 1'b0;

    //         // TX_WRITE_INT_SOURCE_PTR_ST: 
    //         //     if (!out_awfull)
    //         //         out_wren <= 1'b1;
    //         //     else 
    //         //         out_wren <= 1'b0;

    //         // TX_READ_INT_SOURCE_ST:
    //         //     if (!out_awfull)
    //         //         out_wren <= 1'b1;
    //         //     else 
    //         //         out_wren <= 1'b0;

    //         // TX_WRITE_ACT_TAP_STATUS_PTR_ST: 
    //         //     if (!out_awfull)
    //         //         out_wren <= 1'b1;
    //         //     else 
    //         //         out_wren <= 1'b0;

    //         // TX_READ_ACT_TAP_STATUS_ST: 
    //         //     if (!out_awfull)
    //         //         out_wren <= 1'b1;
    //         //     else 
    //         //         out_wren <= 1'b0;

    //         // TX_WRITE_INTR_DATA_PTR_ST: 
    //         //     if (!out_awfull)
    //         //         out_wren <= 1'b1;
    //         //     else 
    //         //         out_wren <= 1'b0;

    //         // TX_READ_INTR_DATA_ST: 
    //         //     if (!out_awfull)
    //         //         out_wren <= 1'b1;
    //         //     else 
    //         //         out_wren <= 1'b0;

    //         // TX_WRITE_WM_FIFO_STS_PTR_ST: 
    //         //     if (!out_awfull)
    //         //         out_wren <= 1'b1;
    //         //     else 
    //         //         out_wren <= 1'b0;

    //         // TX_READ_WM_FIFO_STS_ST: 
    //         //     if (!out_awfull)
    //         //         out_wren <= 1'b1;
    //         //     else 
    //         //         out_wren <= 1'b0;

    //         // TX_WRITE_WM_DATA_PTR_ST: 
    //         //     if (!out_awfull)
    //         //         out_wren <= 1'b1;
    //         //     else 
    //         //         out_wren <= 1'b0;

    //         // TX_READ_WM_DATA_ST: 
    //         //     if (!out_awfull)
    //         //         out_wren <= 1'b1;
    //         //     else 
    //         //         out_wren <= 1'b0;

    //         // TX_WRITE_CALIB_DATA_PTR_ST: 
    //         //     if (!out_awfull)
    //         //         out_wren <= 1'b1;
    //         //     else 
    //         //         out_wren <= 1'b0;

    //         // TX_READ_CALIB_DATA_ST: 
    //         //     if (!out_awfull)
    //         //         out_wren <= 1'b1;
    //         //     else 
    //         //         out_wren <= 1'b0;

    //         // TX_WRITE_CALIB_OFS_ST: 
    //         //     if (!out_awfull)
    //         //         out_wren <= 1'b1;
    //         //     else 
    //         //         out_wren <= 1'b0;

    //         // TX_WRITE_CALIB_OFS_CLEAR_ST: 
    //         //     if (!out_awfull)
    //         //         out_wren <= 1'b1;
    //         //     else 
    //         //         out_wren <= 1'b0;

    //         default : 
    //             out_wren <= 1'b0;

    //     endcase // current_state
    // end 



    // always_ff @(posedge CLK) begin : int_source_reg_proc
    //     case (current_state)

    //         RX_INT_SOURCE_ST : 
    //             if (S_AXIS_TVALID)
    //                 int_source_reg <= S_AXIS_TDATA;

    //         default: int_source_reg <= int_source_reg;
    //     endcase // current_state
    // end 



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    always_ff @( posedge CLK ) begin : axi_awready_proc_cfg
        if (~RESETN)
            axi_awready_cfg <= 1'b0;
        else    
            if (~axi_awready_cfg & S_AXI_LITE_CFG_AWVALID & S_AXI_LITE_CFG_WVALID & aw_en_cfg)
                axi_awready_cfg <= 1'b1;
            else 
                if (S_AXI_LITE_CFG_BREADY & axi_bvalid_cfg)
                    axi_awready_cfg <= 1'b0;
                else
                    axi_awready_cfg <= 1'b0;
    end       


    always_ff @( posedge CLK ) begin : aw_en_proc_cfg
        if (~RESETN)
            aw_en_cfg <= 1'b1;
        else
            if (~axi_awready_cfg & S_AXI_LITE_CFG_AWVALID & S_AXI_LITE_CFG_WVALID & aw_en_cfg)
                aw_en_cfg <= 1'b0;
            else 
                if (S_AXI_LITE_CFG_BREADY & axi_bvalid_cfg)
                    aw_en_cfg <= 1'b1;
    end       



    always_ff @( posedge CLK ) begin : axi_awaddr_proc_cfg
        if (~RESETN)
            axi_awaddr_cfg <= '{default:0};
        else
            if (~axi_awready_cfg & S_AXI_LITE_CFG_AWVALID & S_AXI_LITE_CFG_WVALID & aw_en_cfg)
                axi_awaddr_cfg <= S_AXI_LITE_CFG_AWADDR;
    end       



    always_ff @( posedge CLK ) begin : axi_wready_proc
        if (~RESETN)
            axi_wready_cfg <= 1'b0;
        else    
            if (~axi_wready_cfg & S_AXI_LITE_CFG_WVALID & S_AXI_LITE_CFG_AWVALID & aw_en_cfg )
                axi_wready_cfg <= 1'b1;
            else
                axi_wready_cfg <= 1'b0;
    end       

    

    always_comb begin : slv_reg_wren_cfg_processing  

        slv_reg_wren_cfg = axi_wready_cfg & S_AXI_LITE_CFG_WVALID & axi_awready_cfg & S_AXI_LITE_CFG_AWVALID;
    end





    always_ff @( posedge CLK ) begin : axi_bvalid_proc_cfg
        if (~RESETN)
            axi_bvalid_cfg  <= 1'b0;
        else
            if (axi_awready_cfg & S_AXI_LITE_CFG_AWVALID & ~axi_bvalid_cfg & axi_wready_cfg & S_AXI_LITE_CFG_WVALID)
                axi_bvalid_cfg <= 1'b1;
            else
                if (S_AXI_LITE_CFG_BREADY & axi_bvalid_cfg)
                    axi_bvalid_cfg <= 1'b0; 
    end   



    always_ff @( posedge CLK ) begin : axi_bresp_proc_cfg
        if (~RESETN)
            axi_bresp_cfg   <= 2'b0;
        else
            if (axi_awready_cfg & S_AXI_LITE_CFG_AWVALID & ~axi_bvalid_cfg & axi_wready_cfg & S_AXI_LITE_CFG_WVALID)
                axi_bresp_cfg  <= 2'b0; // 'OKAY' response 
    end   



    always_ff @( posedge CLK ) begin : axi_arready_proc_cfg
        if (~RESETN)
            axi_arready_cfg <= 1'b0;
        else    
            if (~axi_arready_cfg & S_AXI_LITE_CFG_ARVALID)
                axi_arready_cfg <= 1'b1;
            else
                axi_arready_cfg <= 1'b0;
    end       


    always_ff @( posedge CLK ) begin : axi_araddr_proc_cfg
        if (~RESETN)
            axi_araddr_cfg  <= 32'b0;
        else    
            if (~axi_arready_cfg & S_AXI_LITE_CFG_ARVALID)
                axi_araddr_cfg  <= S_AXI_LITE_CFG_ARADDR;
            
    end       



    always_ff @( posedge CLK ) begin : axi_rvalid_proc_cfg
        if (~RESETN)
            axi_rvalid_cfg <= 1'b0;
        else
            if (axi_arready_cfg & S_AXI_LITE_CFG_ARVALID & ~axi_rvalid_cfg)
                axi_rvalid_cfg <= 1'b1;
            else 
                if (axi_rvalid_cfg & S_AXI_LITE_CFG_RREADY)
                    axi_rvalid_cfg <= 1'b0;
    end    



    always_ff @( posedge CLK ) begin : axi_rresp_proc_cfg
        if (~RESETN)
            axi_rresp_cfg  <= '{default:0};
        else
            if (axi_arready_cfg & S_AXI_LITE_CFG_ARVALID & ~axi_rvalid_cfg)
                axi_rresp_cfg  <= 2'b0; // 'OKAY' response             
        
    end    



    always_ff @(posedge CLK) begin : slv_reg_rden_cfg_proc

        slv_reg_rden_cfg <= axi_arready_cfg & S_AXI_LITE_CFG_ARVALID & ~axi_rvalid_cfg;
    end 



    always_ff @(posedge CLK) begin
        case ( axi_araddr_cfg[ADDR_LSB_CFG+OPT_MEM_ADDR_BITS_CFG:ADDR_LSB_CFG] )
            8'h00 : reg_data_out_cfg <= {
                version_major               , //version_major,
                version_minor               , //version_minor,
                link_on                     , // link_on,
                i2c_address                 , // register_cfg[ 0][14:8],
                on_work                     ,
                single_request_complete_flaq, //request_performed,
                calibration_complete        ,  //calibration_flaq,
                1'b0,  // ADXL_IRQ,
                1'b0,
                allow_irq_reg               , //allow_irq,
                enable_interval_requestion  , //enable,
                reset                         // reset
            };

            8'h01   : reg_data_out_cfg <= requestion_interval;
            8'h02   : reg_data_out_cfg <= {{27{1'b0}} , calibration_mode}; //DATA_WIDTH;
            8'h03   : reg_data_out_cfg <= read_valid_count; //read_valid_reg;
            8'h04   : reg_data_out_cfg <= write_valid_count; //write_valid_reg;
            8'h05   : reg_data_out_cfg <= read_transactions[31:0];
            8'h06   : reg_data_out_cfg <= read_transactions[63:32];
            8'h07   : reg_data_out_cfg <= CLK_PERIOD;
            8'h08   : reg_data_out_cfg <= opt_request_interval[31: 0]; //{23'h0, has_ovrrn_intr, sample_address};
            8'h09   : reg_data_out_cfg <= {{16{1'b0}}, opt_request_interval[47:32]}; //opt_request_interval;
            8'h0a   : reg_data_out_cfg <= DATA_WIDTH; //calibration_count_limit_reg;
            8'h0b   : reg_data_out_cfg <= calibration_time[31:0];
            8'h0c   : reg_data_out_cfg <= calibration_time[63:32];
            8'h0d   : reg_data_out_cfg <= write_transactions[31:0];
            8'h0e   : reg_data_out_cfg <= write_transactions[63:32];
            8'h0f   : reg_data_out_cfg <= {16'h0000, single_request_size, single_request_address};
            default : reg_data_out_cfg <= '{default:0};
        endcase
    end



    always_ff @(posedge CLK) begin
        if (slv_reg_rden_cfg)
            axi_rdata_cfg <= reg_data_out_cfg;     // register read data
    end    



    always_ff @(posedge CLK) begin : reset_timer_processing 
        if (~RESETN) begin 
            reset_timer <= '{default:0};
        end else begin 
            if (reset_timer < RESET_DURATION-1) begin 
                reset_timer <= reset_timer + 1;
            end else begin 
                if (slv_reg_wren_cfg) begin 
                    if (axi_awaddr_cfg[ADDR_LSB_CFG + OPT_MEM_ADDR_BITS_CFG : ADDR_LSB_CFG] == 0) begin 
                        if (S_AXI_LITE_CFG_WSTRB[0] & S_AXI_LITE_CFG_WDATA[0]) begin 
                            reset_timer <= '{default:0};
                        end else begin 
                            reset_timer <= reset_timer;
                        end 
                    end else begin 
                        reset_timer <= reset_timer;
                    end 
                end else begin 
                    reset_timer <= reset_timer;
                end     
            end 
        end 
    end 



    always_ff @(posedge CLK) begin : reset_processing  
        if (~RESETN) begin 
            reset <= 1'b1;
        end else begin 
            if (reset_timer < (RESET_DURATION-1)) begin 
                reset <= 1'b1;
            end else begin  
                reset <= 1'b0;
            end 
        end 
    end 



    // logic for perform single request
    always_ff @(posedge CLK) begin : single_request_processing 
        if (~RESETN | reset) begin 
            single_request <= 1'b0;
        end else begin 
            if (slv_reg_wren_cfg) begin 
                if (axi_awaddr_cfg[ADDR_LSB_CFG + OPT_MEM_ADDR_BITS_CFG : ADDR_LSB_CFG] == 0) begin 
                    if ( S_AXI_LITE_CFG_WSTRB[0] & S_AXI_LITE_CFG_WDATA[3]) begin 
                        single_request <= 1'b1;
                    end else begin 
                        single_request <= single_request;
                    end 
                end else begin 
                    single_request <= single_request;
                end 
            end else begin 
                if (single_request_complete) begin 
                    single_request <= 1'b0;
                end else begin 
                    single_request <= single_request;
                end 
            end 
        end 
    end 



    always_ff @(posedge CLK) begin : single_request_complete_flaq_processing 
        if (~RESETN | reset) begin 
            single_request_complete_flaq <= 1'b0;
        end else begin 
            if (single_request) begin 
                if (single_request_complete) begin 
                    single_request_complete_flaq <= 1'b1;        
                end else begin 
                    single_request_complete_flaq <= 1'b0;
                end 
            end else begin 
                single_request_complete_flaq <= single_request_complete_flaq;
            end 
        end 
    end 



    // i2c address holding 
    always_ff @(posedge CLK) begin : i2c_address_processing 
        if (~RESETN | reset) begin 
            i2c_address <= DEFAULT_DEVICE_ADDRESS;
        end else begin  
            if (slv_reg_wren_cfg) begin 
                if (axi_awaddr_cfg[ADDR_LSB_CFG + OPT_MEM_ADDR_BITS_CFG : ADDR_LSB_CFG] == 0) begin 
                    if ( S_AXI_LITE_CFG_WSTRB[1]) begin 
                        i2c_address <= S_AXI_LITE_CFG_WDATA[14:8];
                    end else begin 
                        i2c_address <= i2c_address;
                    end  
                end else begin 
                    i2c_address <= i2c_address;
                end 
            end 
        end 
    end 


    // periodic requesting of register from device 
    always_ff @(posedge CLK) begin : enable_interval_requestion_processing 
        if (~RESETN | reset) begin 
            enable_interval_requestion <= 1'b0;
        end else begin 
            if (slv_reg_wren_cfg) begin 
                if (axi_awaddr_cfg[ADDR_LSB_CFG + OPT_MEM_ADDR_BITS_CFG : ADDR_LSB_CFG] == 0) begin 
                    if ( S_AXI_LITE_CFG_WSTRB[0]) begin 
                        enable_interval_requestion <= S_AXI_LITE_CFG_WDATA[1];
                    end else begin 
                        enable_interval_requestion <= enable_interval_requestion;
                    end  
                end else begin 
                    enable_interval_requestion <= enable_interval_requestion;
                end 
            end else begin 
                enable_interval_requestion <= enable_interval_requestion;
            end 
        end 
    end 



    always_ff @(posedge CLK) begin : allow_irq_reg_processing 
        if (~RESETN | reset) begin 
            allow_irq_reg <= 1'b0;
        end else begin 
            if (slv_reg_wren_cfg) begin 
                if (axi_awaddr_cfg[ADDR_LSB_CFG + OPT_MEM_ADDR_BITS_CFG : ADDR_LSB_CFG] == 0) begin 
                    if (S_AXI_LITE_CFG_WSTRB[0]) begin 
                        allow_irq_reg <= S_AXI_LITE_CFG_WDATA[2];
                    end else begin 
                        allow_irq_reg <= allow_irq_reg;
                    end 
                end else begin 
                    allow_irq_reg <= allow_irq_reg;
                end 
            end else begin 
                allow_irq_reg <= allow_irq_reg;
            end 

        end 
    end 



    generate 

        for (genvar index = 0; index < 4; index++) begin : GEN_REGISTER_FILE_BYTE_INDEX 

            always_ff @(posedge CLK) begin : requestion_interval_processing
                if (~RESETN | reset) begin
                    requestion_interval[index] <= DEFAULT_REQUESTION_INTERVAL[ (((index+1)*8)-1):(index*8)];
                end else begin
                    if (slv_reg_wren_cfg) begin
                        if (axi_awaddr_cfg[ADDR_LSB_CFG + OPT_MEM_ADDR_BITS_CFG : ADDR_LSB_CFG] == 1) begin
                            if ( S_AXI_LITE_CFG_WSTRB[index]) begin
                                requestion_interval[index] <= S_AXI_LITE_CFG_WDATA[ (((index+1)*8)-1):(index*8)];
                            end else begin
                                requestion_interval[index] <= requestion_interval[index];
                            end
                        end else begin
                            requestion_interval[index] <= requestion_interval[index];
                        end
                    end else begin
                        requestion_interval[index] <= requestion_interval[index];
                    end
                end
            end // requestion_interval_processing

        end // GEN_REGISTER_FILE_BYTE_INDEX 

    endgenerate



    always_ff @(posedge CLK) begin : adxl_irq_ack_processing 
        if (~RESETN | reset) begin 
            adxl_irq_ack <= 1'b0;
        end else begin 
            if (slv_reg_wren_cfg) begin 
                if (axi_awaddr_cfg[ADDR_LSB_CFG + OPT_MEM_ADDR_BITS_CFG : ADDR_LSB_CFG] == 0) begin 
                    if ((S_AXI_LITE_CFG_WSTRB[0] == 1) & S_AXI_LITE_CFG_WDATA[4] & ADXL_IRQ ) begin 
                        adxl_irq_ack <= 1'b1;
                    end else begin 
                        adxl_irq_ack <= 1'b0;
                    end 
                end else begin
                    adxl_irq_ack <= 1'b0;
                end 
            end else begin  
                adxl_irq_ack <= 1'b0;
            end 
        end
    end 



    always_ff @(posedge CLK) begin : calibration_processing 
        if (~RESETN | reset) begin 
            calibration <= 1'b0;
        end else begin 
            if (slv_reg_wren_cfg) begin 
                if (axi_awaddr_cfg[ADDR_LSB_CFG + OPT_MEM_ADDR_BITS_CFG : ADDR_LSB_CFG] == 0) begin 
                    if ((S_AXI_LITE_CFG_WSTRB[0] == 1) & S_AXI_LITE_CFG_WDATA[5]) begin 
                        calibration <= 1'b1;
                    end else begin 
                        calibration <= 1'b0;
                    end 
                end else begin 
                    calibration <= 1'b0;
                end 
            end else begin 
                calibration <= 1'b0;
            end 
        end 
    end 



    always_ff @(posedge CLK) begin : calibration_mode_processing 
        if (~RESETN | reset) begin
            calibration_mode <= DEFAULT_CALIBRATION_MODE;
        end else begin
            if (slv_reg_wren_cfg) begin
                if (axi_awaddr_cfg[ADDR_LSB_CFG + OPT_MEM_ADDR_BITS_CFG : ADDR_LSB_CFG] == 2) begin
                    if ( S_AXI_LITE_CFG_WSTRB[0]) begin
                        calibration_mode <= S_AXI_LITE_CFG_WDATA[4:0];
                    end else begin
                        calibration_mode <= calibration_mode;
                    end
                end else begin
                    calibration_mode <= calibration_mode;
                end
            end else begin
                calibration_mode <= calibration_mode;
            end
        end
    end // requestion_interval_processing



    always_ff @(posedge CLK) begin : single_request_address_processing 
        if (~RESETN | reset) begin 
            single_request_address <= '{default:0};
        end else begin 
            if (slv_reg_wren_cfg) begin
                if (!on_work) begin 
                    if (axi_awaddr_cfg[ADDR_LSB_CFG + OPT_MEM_ADDR_BITS_CFG : ADDR_LSB_CFG] == 15) begin
                        if ( S_AXI_LITE_CFG_WSTRB[0]) begin
                            single_request_address <= S_AXI_LITE_CFG_WDATA[7:0];
                        end else begin
                            single_request_address <= single_request_address;
                        end
                    end else begin
                        single_request_address <= single_request_address;
                    end
                end else begin 
                    single_request_address <= single_request_address;
                end 
            end else begin
                single_request_address <= single_request_address;
            end
        end 
    end 



    always_ff @(posedge CLK) begin : single_request_size_processing 
        if (~RESETN | reset) begin 
            single_request_size <= 8'h3A;
        end else begin 
            if (!on_work) begin 
                if (slv_reg_wren_cfg) begin
                    if (axi_awaddr_cfg[ADDR_LSB_CFG + OPT_MEM_ADDR_BITS_CFG : ADDR_LSB_CFG] == 15) begin
                        if (S_AXI_LITE_CFG_WSTRB[1]) begin
                            single_request_size <= S_AXI_LITE_CFG_WDATA[15:8];
                        end else begin
                            single_request_size <= single_request_size;
                        end
                    end else begin
                        single_request_size <= single_request_size;
                    end
                end else begin 
                    single_request_size <= single_request_size;
                end 
            end else begin
                single_request_size <= single_request_size;
            end
        end 
    end 



    adxl345_functional #(.CLK_PERIOD(CLK_PERIOD)) adxl345_functional_inst (
        .CLK                       (CLK                       ),
        .RESET                     (reset                     ),
        
        .WDATA                     (S_AXI_LITE_DEV_WDATA      ),
        .WSTRB                     (S_AXI_LITE_DEV_WSTRB      ),
        .WADDR                     (axi_dev_awaddr[5:2]       ),
        .WVALID                    (slv_reg_wren              ),
        
        .RADDR                     (axi_dev_araddr[5:2]       ),
        .RDATA                     (reg_data_out              ),
        
        .LINK_ON                   (link_on                   ),
        
        .ALLOW_IRQ                 (allow_irq_reg             ),
        
        .SINGLE_REQUEST            (single_request            ),
        .SINGLE_REQUEST_ADDRESS    (single_request_address    ),
        .SINGLE_REQUEST_SIZE       (single_request_size       ),
        .SINGLE_REQUEST_COMPLETE   (single_request_complete   ),
        
        .ENABLE_INTERVAL_REQUESTION(enable_interval_requestion),
        .REQUESTION_INTERVAL       (requestion_interval       ),
        
        .I2C_ADDRESS               (i2c_address               ),
        
        .ADXL_INTERRUPT            (ADXL_INTERRUPT            ),
        .ADXL_IRQ                  (ADXL_IRQ                  ),
        .ADXL_IRQ_ACK              (adxl_irq_ack              ),
        
        .CALIBRATION               (calibration               ),
        .CALIBRATION_MODE          (calibration_mode          ),
        .CALIBRATION_COMPLETE      (calibration_complete      ),
        .CALIBRATION_TIME          (calibration_time          ),
        
        .OPT_REQUEST_INTERVAL      (opt_request_interval      ),
        
        .READ_VALID_COUNT          (read_valid_count          ),
        .WRITE_VALID_COUNT         (write_valid_count         ),
        
        .WRITE_TRANSACTIONS        (write_transactions        ),
        .READ_TRANSACTIONS         (read_transactions         ),
        
        .ON_WORK                   (on_work                   ),
        
        .M_AXIS_TDATA              (M_AXIS_TDATA              ),
        .M_AXIS_TKEEP              (M_AXIS_TKEEP              ),
        .M_AXIS_TUSER              (M_AXIS_TUSER              ),
        .M_AXIS_TVALID             (M_AXIS_TVALID             ),
        .M_AXIS_TLAST              (M_AXIS_TLAST              ),
        .M_AXIS_TREADY             (M_AXIS_TREADY             ),
        // data from device
        .S_AXIS_TDATA              (S_AXIS_TDATA              ),
        .S_AXIS_TKEEP              (S_AXIS_TKEEP              ),
        .S_AXIS_TUSER              (S_AXIS_TUSER              ),
        .S_AXIS_TVALID             (S_AXIS_TVALID             ),
        .S_AXIS_TLAST              (S_AXIS_TLAST              ),
        .S_AXIS_TREADY             (S_AXIS_TREADY             )
    );

endmodule
