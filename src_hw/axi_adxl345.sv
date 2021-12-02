`timescale 1 ns / 1 ps



module axi_adxl345 #(
    parameter integer       S_AXI_LITE_DEV_DATA_WIDTH = 32       ,
    parameter integer       S_AXI_LITE_DEV_ADDR_WIDTH = 6        ,
    parameter         [6:0] DEFAULT_DEVICE_ADDRESS    = 7'h53    ,
    parameter integer       DEFAULT_REQUEST_INTERVAL  = 1000     ,
    parameter integer       S_AXI_LITE_CFG_DATA_WIDTH = 32       ,
    parameter integer       S_AXI_LITE_CFG_ADDR_WIDTH = 6        ,
    parameter integer       CLK_PERIOD                = 100000000,
    parameter integer       RESET_DURATION            = 1000
) (
    input  logic                                     CLK                   ,
    input  logic                                     RESETN                ,
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
    output logic [                              7:0] M_AXIS_TDATA          ,
    output logic [                              0:0] M_AXIS_TKEEP          ,
    output logic [                              7:0] M_AXIS_TUSER          ,
    output logic                                     M_AXIS_TVALID         ,
    output logic                                     M_AXIS_TLAST          ,
    input  logic                                     M_AXIS_TREADY         ,
    input  logic [                              7:0] S_AXIS_TDATA          ,
    input  logic [                              0:0] S_AXIS_TKEEP          ,
    input  logic [                              7:0] S_AXIS_TUSER          ,
    input  logic                                     S_AXIS_TVALID         ,
    input  logic                                     S_AXIS_TLAST          ,
    output logic                                     S_AXIS_TREADY
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




    localparam integer    ADDR_LSB_CFG          = (S_AXI_LITE_CFG_DATA_WIDTH/32) + 1;
    localparam integer    OPT_MEM_ADDR_BITS_CFG = 2                                 ;
    localparam integer    ADDR_LSB_DEV          = (S_AXI_LITE_DEV_DATA_WIDTH/32) + 1;
    localparam integer    OPT_MEM_ADDR_BITS_DEV = 3                                 ;
    localparam integer    DATA_WIDTH            = 8                                 ;
    localparam integer    USER_WIDTH            = 8                                 ;
    localparam ADDRESS_LIMIT         = 'h39                              ;

    // logic [0:15][S_AXI_LITE_DEV_DATA_WIDTH-1:0] register = '{default:'{default:0}}   ;
    logic [0:15][(S_AXI_LITE_DEV_DATA_WIDTH/8)-1:0][7:0] register = '{default:'{default:'{default:0}}}   ;
    logic [0:15][3:0] need_update_reg = '{
        '{0, 0, 0, 0}, // 0x00
        '{0, 0, 0, 0}, // 0x04
        '{0, 0, 0, 0}, // 0x08
        '{0, 0, 0, 0}, // 0x0C
        '{0, 0, 0, 0}, // 0x10
        '{0, 0, 0, 0}, // 0x14
        '{0, 0, 0, 0}, // 0x18
        '{0, 0, 0, 0}, // 0x1C
        '{0, 0, 0, 0}, // 0x20
        '{0, 0, 0, 0}, // 0x24
        '{0, 0, 0, 0}, // 0x28
        '{0, 0, 0, 0}, // 0x2C
        '{0, 0, 0, 0}, // 0x30
        '{0, 0, 0, 0}, // 0x34
        '{0, 0, 0, 0}, // 0x38
        '{0, 0, 0, 0}  // 0x3C
        };

    logic [0:15][3:0] write_mask_register = '{
        '{0, 0, 0, 0}, // 0x00
        '{0, 0, 0, 0}, // 0x04
        '{0, 0, 0, 0}, // 0x08
        '{0, 0, 0, 0}, // 0x0C
        '{0, 0, 0, 0}, // 0x10
        '{0, 0, 0, 0}, // 0x14
        '{0, 0, 0, 0}, // 0x18
        '{1, 1, 1, 0}, // 0x1C
        '{1, 1, 1, 1}, // 0x20
        '{1, 1, 1, 1}, // 0x24
        '{0, 1, 1, 1}, // 0x28
        '{1, 1, 1, 1}, // 0x2C
        '{0, 0, 1, 0}, // 0x30
        '{0, 0, 0, 0}, // 0x34
        '{0, 0, 0, 1}, // 0x38
        '{0, 0, 0, 0}  // 0x3C
        };


    logic                                 slv_reg_rden;
    logic                                 slv_reg_wren;
    logic [S_AXI_LITE_DEV_DATA_WIDTH-1:0] reg_data_out;
    logic                                 aw_en       ;

    integer byte_index;

    logic update_request = 1'b0;


    logic [7:0][S_AXI_LITE_CFG_DATA_WIDTH-1:0] register_cfg = '{default:'{default:0}}   ;

    logic                                 slv_reg_rden_cfg;
    logic                                 slv_reg_wren_cfg;
    logic [S_AXI_LITE_CFG_DATA_WIDTH-1:0] reg_data_out_cfg;
    logic                                 aw_en_cfg       ;

    integer byte_index_cfg;


    typedef enum {
        IDLE_ST              ,
        CHK_UPD_NEEDED_ST    ,
        SEND_WRITE_CMD_ST    ,
        INC_ADDR_ST          ,
        TX_SEND_ADDR_PTR     ,
        TX_READ_REQUEST_ST   ,
        AWAIT_RECEIVE_DATA_ST 
    } fsm;

    fsm         current_state      = IDLE_ST     ;
    logic [5:0] address            = '{default:0};
    logic [1:0] write_cmd_word_cnt = '{default:0};

    logic [31:0] request_timer = '{default:0};

    logic [    DATA_WIDTH-1:0] out_din_data = '{default:0};
    logic [(DATA_WIDTH/8)-1:0] out_din_keep = '{default:0};
    logic [    USER_WIDTH-1:0] out_din_user = '{default:0};
    logic                      out_din_last = 1'b0        ;
    logic                      out_wren     = 1'b0        ;
    logic                      out_full                   ;
    logic                      out_awfull                 ;

    logic [                         7:0] version_major        = 8'h01                   ; // read only,
    logic [                         7:0] version_minor        = 8'h00                   ; // read only,
    logic [                         6:0] i2c_address          = DEFAULT_DEVICE_ADDRESS  ; // reg[0][14:8]
    logic                                link_on              = 1'b0                    ;
    logic                                on_work              = 1'b0                    ; // reg[0][4]
    logic                                allow_irq            = 1'b0                    ; // reg[0][2]
    logic                                enable               = 1'b0                    ; // reg[0][1]
    logic [($clog2(RESET_DURATION)-1):0] reset_logic_timer    = 1'b0                    ; // reg[0][0]
    logic                                reset                = 1'b0                    ;
    logic [                        31:0] request_interval     = DEFAULT_REQUEST_INTERVAL;
    logic [                        31:0] read_valid_count     = '{default:0}            ;
    logic [                        31:0] read_valid_reg       = '{default:0}            ;
    logic [                        31:0] write_valid_count    = '{default:0}            ;
    logic [                        31:0] write_valid_reg      = '{default:0}            ;
    logic [                        31:0] write_transactions   = '{default:0}            ;
    logic [                        31:0] read_transactions    = '{default:0}            ;
    logic [                        31:0] transactions_timer   = '{default:0}            ;

    always_comb begin
        S_AXI_LITE_DEV_AWREADY = axi_dev_awready;
        S_AXI_LITE_DEV_WREADY  = axi_dev_wready;
        S_AXI_LITE_DEV_BRESP   = axi_dev_bresp;
        S_AXI_LITE_DEV_BVALID  = axi_dev_bvalid;
        S_AXI_LITE_DEV_ARREADY = axi_dev_arready;
        S_AXI_LITE_DEV_RDATA   = axi_dev_rdata;
        S_AXI_LITE_DEV_RRESP   = axi_dev_rresp;
        S_AXI_LITE_DEV_RVALID  = axi_dev_rvalid;

        S_AXIS_TREADY = 1'b1;

    end 

    always_comb begin
        S_AXI_LITE_CFG_AWREADY = axi_awready_cfg;
        S_AXI_LITE_CFG_WREADY  = axi_wready_cfg;
        S_AXI_LITE_CFG_BRESP   = axi_bresp_cfg;
        S_AXI_LITE_CFG_BVALID  = axi_bvalid_cfg;
        S_AXI_LITE_CFG_ARREADY = axi_arready_cfg;
        S_AXI_LITE_CFG_RDATA   = axi_rdata_cfg;
        S_AXI_LITE_CFG_RRESP   = axi_rresp_cfg;
        S_AXI_LITE_CFG_RVALID  = axi_rvalid_cfg;
    end 



    always @( posedge CLK ) begin : axi_dev_awready_proc
        if (~RESETN)
            axi_dev_awready <= 1'b0;
        else    
            if (~axi_dev_awready && S_AXI_LITE_DEV_AWVALID && S_AXI_LITE_DEV_WVALID && aw_en)
                axi_dev_awready <= 1'b1;
            else 
                if (S_AXI_LITE_DEV_BREADY && axi_dev_bvalid)
                    axi_dev_awready <= 1'b0;
                else
                    axi_dev_awready <= 1'b0;
    end       

    always @( posedge CLK ) begin : aw_en_proc
        if (~RESETN)
            aw_en <= 1'b1;
        else
            if (~axi_dev_awready && S_AXI_LITE_DEV_AWVALID && S_AXI_LITE_DEV_WVALID && aw_en)
                aw_en <= 1'b0;
            else 
                if (S_AXI_LITE_DEV_BREADY && axi_dev_bvalid)
                    aw_en <= 1'b1;
    end       

    always @( posedge CLK ) begin : axi_dev_awaddr_proc
        if (~RESETN)
            axi_dev_awaddr <= 0;
        else
            if (~axi_dev_awready && S_AXI_LITE_DEV_AWVALID && S_AXI_LITE_DEV_WVALID && aw_en)
                axi_dev_awaddr <= S_AXI_LITE_DEV_AWADDR;
    end       

    always @( posedge CLK ) begin : axi_dev_wready_proc
        if (~RESETN)
            axi_dev_wready <= 1'b0;
        else    
            if (~axi_dev_wready && S_AXI_LITE_DEV_WVALID && S_AXI_LITE_DEV_AWVALID && aw_en )
                axi_dev_wready <= 1'b1;
            else
                axi_dev_wready <= 1'b0;
    end       

    always_comb begin 
        slv_reg_wren = axi_dev_wready && S_AXI_LITE_DEV_WVALID && axi_dev_awready && S_AXI_LITE_DEV_AWVALID;
    end

    generate 

        for (genvar reg_index = 0; reg_index < 15; reg_index++) begin 
    
            always @(posedge CLK) begin : register_proc
                if (~RESETN | reset)
                    register[reg_index] <= 0;
                else
                    if (slv_reg_wren) begin 
                        if (axi_dev_awaddr[ADDR_LSB_DEV+OPT_MEM_ADDR_BITS_DEV:ADDR_LSB_DEV] == reg_index) begin 
                            for ( byte_index = 0; byte_index <= (S_AXI_LITE_DEV_DATA_WIDTH/8)-1; byte_index = byte_index + 1 ) begin 
                                if ( S_AXI_LITE_DEV_WSTRB[byte_index] == 1 & write_mask_register[reg_index][byte_index]) begin 
                                    register[reg_index][byte_index] <= S_AXI_LITE_DEV_WDATA[(byte_index*8) +: 8];
                                end 
                            end 
                        end 
                    end else begin 
                        case (current_state) 
                            AWAIT_RECEIVE_DATA_ST : 
                                if (S_AXIS_TVALID) 
                                    if (address[5:2] == reg_index)  
                                        for ( byte_index = 0; byte_index <= 3; byte_index = byte_index + 1 ) begin
                                            if (byte_index == address[1:0])
                                                register[reg_index][byte_index] <= S_AXIS_TDATA;
                                        end 
    
                            default: 
                                register <= register;

                        endcase // current_state
                    end 
            end    

            always @(posedge CLK) begin : need_update_reg_proc 
                if (~RESETN | reset)
                    need_update_reg[reg_index] <= 0;
                else
                    if (slv_reg_wren)
                        if (axi_dev_awaddr[ADDR_LSB_DEV+OPT_MEM_ADDR_BITS_DEV:ADDR_LSB_DEV] == reg_index)
                            for (byte_index = 0; byte_index <= (S_AXI_LITE_DEV_DATA_WIDTH/8)-1; byte_index = byte_index + 1)
                                if (S_AXI_LITE_DEV_WSTRB[byte_index])
                                    need_update_reg[reg_index][byte_index] <= write_mask_register[reg_index][byte_index];
            end    

        end 

    endgenerate

    always @( posedge CLK ) begin : axi_dev_bvalid_proc
        if (~RESETN)
            axi_dev_bvalid  <= 0;
        else
            if (axi_dev_awready && S_AXI_LITE_DEV_AWVALID && ~axi_dev_bvalid && axi_dev_wready && S_AXI_LITE_DEV_WVALID)
                axi_dev_bvalid <= 1'b1;
            else
                if (S_AXI_LITE_DEV_BREADY && axi_dev_bvalid)
                    axi_dev_bvalid <= 1'b0; 
    end   

    always @( posedge CLK ) begin : axi_dev_bresp_proc
        if (~RESETN)
            axi_dev_bresp   <= 2'b0;
        else
            if (axi_dev_awready && S_AXI_LITE_DEV_AWVALID && ~axi_dev_bvalid && axi_dev_wready && S_AXI_LITE_DEV_WVALID)
                axi_dev_bresp  <= 2'b0; // 'OKAY' response 
    end   

///////////////////////////////////////////// READ INTERFACE SIGNALS /////////////////////////////////////////////

    always @( posedge CLK ) begin : axi_dev_arready_proc
        if (~RESETN)
            axi_dev_arready <= 1'b0;
        else    
            if (~axi_dev_arready && S_AXI_LITE_DEV_ARVALID)
                axi_dev_arready <= 1'b1;
            else
                axi_dev_arready <= 1'b0;
    end       

    always @( posedge CLK ) begin : axi_dev_araddr_proc
        if (~RESETN)
            axi_dev_araddr  <= 32'b0;
        else    
            if (~axi_dev_arready && S_AXI_LITE_DEV_ARVALID)
                axi_dev_araddr  <= S_AXI_LITE_DEV_ARADDR;
            
    end       

    always @( posedge CLK ) begin : axi_dev_rvalid_proc
        if (~RESETN)
            axi_dev_rvalid <= 0;
        else
            if (axi_dev_arready && S_AXI_LITE_DEV_ARVALID && ~axi_dev_rvalid)
                axi_dev_rvalid <= 1'b1;
            else 
                if (axi_dev_rvalid && S_AXI_LITE_DEV_RREADY)
                    axi_dev_rvalid <= 1'b0;
    end    

    always @( posedge CLK ) begin : axi_dev_rresp_proc
        if (~RESETN)
            axi_dev_rresp  <= 0;
        else
            if (axi_dev_arready && S_AXI_LITE_DEV_ARVALID && ~axi_dev_rvalid)
                axi_dev_rresp  <= 2'b0; // 'OKAY' response             
        
    end    

    always_comb begin 
        slv_reg_rden = axi_dev_arready & S_AXI_LITE_DEV_ARVALID & ~axi_dev_rvalid;
    end 

    always @(*) begin
        case ( axi_dev_araddr[ADDR_LSB_DEV+OPT_MEM_ADDR_BITS_DEV:ADDR_LSB_DEV] )
            4'h0    : reg_data_out <= register[ 0];
            4'h1    : reg_data_out <= register[ 1];
            4'h2    : reg_data_out <= register[ 2];
            4'h3    : reg_data_out <= register[ 3];
            4'h4    : reg_data_out <= register[ 4];
            4'h5    : reg_data_out <= register[ 5];
            4'h6    : reg_data_out <= register[ 6];
            4'h7    : reg_data_out <= register[ 7];
            4'h8    : reg_data_out <= register[ 8];
            4'h9    : reg_data_out <= register[ 9];
            4'hA    : reg_data_out <= register[10];
            4'hB    : reg_data_out <= register[11];
            4'hC    : reg_data_out <= register[12];
            4'hD    : reg_data_out <= register[13];
            4'hE    : reg_data_out <= register[14];
            4'hF    : reg_data_out <= register[15];
            default : reg_data_out <= 0;
        endcase
    end

    always @( posedge CLK ) begin
        if (~RESETN)
            axi_dev_rdata  <= 0;
        else 
            if (slv_reg_rden) 
                axi_dev_rdata <= reg_data_out;     // register read data
    end    


////////////////////////////////////////////////////// INTERNAL LOGIC SIGNALS //////////////////////////////////////////////////////


    always_ff @(posedge CLK) begin : write_cmd_word_cnt_proc
        if (~RESETN)
            write_cmd_word_cnt <= 1'b0;
        else 
            case (current_state)
                SEND_WRITE_CMD_ST : 
                    if (~out_awfull)
                        write_cmd_word_cnt <= write_cmd_word_cnt + 1;

                TX_SEND_ADDR_PTR : 
                    if (~out_awfull)
                        write_cmd_word_cnt <= write_cmd_word_cnt + 1;
                    
                default : 
                    write_cmd_word_cnt <= 1'b0;

            endcase // current_state
    end 

    always_ff @(posedge CLK) begin : current_state_proc 
        if (~RESETN | reset) 
            current_state <= IDLE_ST;
        else 
            case (current_state)

                IDLE_ST : 
                    if (enable) begin 
                        if (update_request)
                            current_state <= CHK_UPD_NEEDED_ST;
                        else 
                            if (request_timer == request_interval)
                                current_state <= TX_SEND_ADDR_PTR;
                    end 

                CHK_UPD_NEEDED_ST : 
                    if (need_update_reg[address[5:2]][address[1:0]])
                        current_state <= SEND_WRITE_CMD_ST;
                    else 
                        current_state <= INC_ADDR_ST;


                SEND_WRITE_CMD_ST  : 
                    if (~out_awfull)
                       if (write_cmd_word_cnt == 2'b10)
                            current_state <= INC_ADDR_ST;

                INC_ADDR_ST  : 
                    if (address == ADDRESS_LIMIT) 
                        current_state <= IDLE_ST;
                    else 
                        current_state <= CHK_UPD_NEEDED_ST;

                TX_SEND_ADDR_PTR: 
                    if (~out_awfull)
                        if (write_cmd_word_cnt == 2'b01)
                            current_state <= TX_READ_REQUEST_ST;

                TX_READ_REQUEST_ST : 
                    if (~out_awfull) 
                        current_state <= AWAIT_RECEIVE_DATA_ST;

                AWAIT_RECEIVE_DATA_ST : 
                    if (S_AXIS_TVALID & S_AXIS_TLAST)
                        current_state <= IDLE_ST;
                    else 
                        current_state <= current_state;

                default : 
                    current_state <= current_state;

            endcase // current_state

    end

    always_ff @(posedge CLK) begin : address_proc 
        if (~RESETN) 
            address  <= '{default:0};
        else 
            case (current_state)

                IDLE_ST : 
                    address <= '{default:0};

                INC_ADDR_ST : 
                    address <= address + 1;

                AWAIT_RECEIVE_DATA_ST : 
                    if (S_AXIS_TVALID)
                        address <= address + 1;

                default : 
                    address <= address;

            endcase // current_state
    end 

    // generate 

        // for (genvar reg_index = 0; reg_index < 15; reg_index++) begin 
    
    always @(posedge CLK) begin : update_request_proc
        if (~RESETN | reset)
            update_request <= 1'b0;
        else
            if (slv_reg_wren) begin 
                // if (axi_dev_awaddr[ADDR_LSB_DEV+OPT_MEM_ADDR_BITS_DEV:ADDR_LSB_DEV] == reg_index) begin 
                    for ( byte_index = 0; byte_index <= (S_AXI_LITE_DEV_DATA_WIDTH/8)-1; byte_index = byte_index + 1 ) begin 
                        if (write_mask_register[axi_dev_awaddr[ADDR_LSB_DEV+OPT_MEM_ADDR_BITS_DEV:ADDR_LSB_DEV]][byte_index]) begin 
                            update_request <= 1'b1;
                        end 
                    end 
                // end
            end else begin 
                case (current_state)
                    INC_ADDR_ST : 
                        if (address == ADDRESS_LIMIT)
                            update_request <= 1'b0;
                    
                    default : 
                        update_request <= update_request;
                endcase // current_state
            end 
    end    
// end 

    // endgenerate



    fifo_out_sync_tuser_xpm #(
        .DATA_WIDTH(DATA_WIDTH),
        .USER_WIDTH(USER_WIDTH),
        .MEMTYPE   ("block"   ),
        .DEPTH     (16        )
    ) fifo_out_sync_tuser_xpm_inst (
        .CLK          (CLK            ),
        .RESET        (~RESETN | reset),
        .OUT_DIN_DATA (out_din_data               ),
        .OUT_DIN_KEEP (out_din_keep               ),
        .OUT_DIN_USER (out_din_user               ),
        .OUT_DIN_LAST (out_din_last               ),
        .OUT_WREN     (out_wren                   ),
        .OUT_FULL     (out_full                   ),
        .OUT_AWFULL   (out_awfull                 ),
        .M_AXIS_TDATA (M_AXIS_TDATA               ),
        .M_AXIS_TKEEP (M_AXIS_TKEEP               ),
        .M_AXIS_TUSER (M_AXIS_TUSER               ),
        .M_AXIS_TVALID(M_AXIS_TVALID              ),
        .M_AXIS_TLAST (M_AXIS_TLAST               ),
        .M_AXIS_TREADY(M_AXIS_TREADY              )
    );

    always_comb begin 
        out_din_keep <= 1'b1;
    end 

    always_ff @(posedge CLK) begin : out_din_data_proc
        case (current_state)
            SEND_WRITE_CMD_ST : 
                case(write_cmd_word_cnt)
                    2'b00 : 
                        out_din_data <= 8'h02;

                    2'b01 : 
                        out_din_data <= {2'b00, address};

                    2'b10 : 
                        out_din_data <= register[address[5:2]][address[1:0]];

                    default : 
                        out_din_data <= out_din_data;

                endcase // write_cmd_word_cnt

            TX_SEND_ADDR_PTR : 
                case (write_cmd_word_cnt)
                    2'b00   : out_din_data <= 8'h01;
                    2'b01   : out_din_data <= 8'h00;
                    default : out_din_data <= out_din_data;
                endcase // write_cmd_word_cnt

            TX_READ_REQUEST_ST : 
                out_din_data <= ADDRESS_LIMIT;

            default : 
                out_din_data <= out_din_data;

        endcase // current_state
    end 

    always_ff @(posedge CLK) begin : out_wren_proc
        case (current_state)
            SEND_WRITE_CMD_ST : 
                if (~out_awfull) 
                    out_wren <= 1'b1;
                else 
                    out_wren <= 1'b0;

            TX_READ_REQUEST_ST: 
                if (~out_awfull) 
                    out_wren <= 1'b1;
                else 
                    out_wren <= 1'b0;

            TX_SEND_ADDR_PTR : 
                if (~out_awfull)
                    out_wren <= 1'b1;
                else 
                    out_wren <= 1'b0;

            default : 
                out_wren <= 1'b0;

        endcase // current_state
    end 

    always_ff @(posedge CLK) begin : out_din_user_proc
        case (current_state)
            SEND_WRITE_CMD_ST : 
                out_din_user <= {DEFAULT_DEVICE_ADDRESS, 1'b0};

            TX_READ_REQUEST_ST : 
                out_din_user <= {DEFAULT_DEVICE_ADDRESS, 1'b1};

            TX_SEND_ADDR_PTR : 
                out_din_user <= {DEFAULT_DEVICE_ADDRESS, 1'b0};

            default : 
                out_din_user <= '{default:0};
        endcase // current_state
    end 

    always_ff @(posedge CLK) begin 
        case (current_state)
            SEND_WRITE_CMD_ST : 
                case (write_cmd_word_cnt) 
                    2'b10 :
                        out_din_last <= 1'b1;

                    default: 
                        out_din_last <= 1'b0;

                endcase // write_cmd_word_cnt

            TX_READ_REQUEST_ST : 
                out_din_last <= 1'b1;

            TX_SEND_ADDR_PTR : 
                case (write_cmd_word_cnt)
                    2'b01 : 
                        out_din_last <= 1'b1;
                    default : 
                        out_din_last <= 1'b0;
                endcase // write_cmd_word_cnt

            default : 
                out_din_last <= 1'b0;

        endcase // current_state
    end 

    always_ff @(posedge CLK) begin : request_timer_proc
        case (current_state)
            IDLE_ST : 
                if (request_timer < request_interval)
                    request_timer <= request_timer + 1;
                else
                    request_timer <= '{default:0};

            AWAIT_RECEIVE_DATA_ST : 
                request_timer <= '{default:0};

        endcase
    end 




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    always @( posedge CLK ) begin : axi_awready_proc_cfg
        if (~RESETN)
            axi_awready_cfg <= 1'b0;
        else    
            if (~axi_awready_cfg && S_AXI_LITE_CFG_AWVALID && S_AXI_LITE_CFG_WVALID && aw_en_cfg)
                axi_awready_cfg <= 1'b1;
            else 
                if (S_AXI_LITE_CFG_BREADY && axi_bvalid_cfg)
                    axi_awready_cfg <= 1'b0;
                else
                    axi_awready_cfg <= 1'b0;
    end       


    always @( posedge CLK ) begin : aw_en_proc_cfg
        if (~RESETN)
            aw_en_cfg <= 1'b1;
        else
            if (~axi_awready_cfg && S_AXI_LITE_CFG_AWVALID && S_AXI_LITE_CFG_WVALID && aw_en_cfg)
                aw_en_cfg <= 1'b0;
            else 
                if (S_AXI_LITE_CFG_BREADY && axi_bvalid_cfg)
                    aw_en_cfg <= 1'b1;
    end       



    always @( posedge CLK ) begin : axi_awaddr_proc_cfg
        if (~RESETN)
            axi_awaddr_cfg <= 0;
        else
            if (~axi_awready_cfg && S_AXI_LITE_CFG_AWVALID && S_AXI_LITE_CFG_WVALID && aw_en_cfg)
                axi_awaddr_cfg <= S_AXI_LITE_CFG_AWADDR;
    end       



    always @( posedge CLK ) begin : axi_wready_proc
        if (~RESETN)
            axi_wready_cfg <= 1'b0;
        else    
            if (~axi_wready_cfg && S_AXI_LITE_CFG_WVALID && S_AXI_LITE_CFG_AWVALID && aw_en_cfg )
                axi_wready_cfg <= 1'b1;
            else
                axi_wready_cfg <= 1'b0;
    end       

    

    always_comb begin 
        slv_reg_wren_cfg = axi_wready_cfg && S_AXI_LITE_CFG_WVALID && axi_awready_cfg && S_AXI_LITE_CFG_AWVALID;
    end






    always @( posedge CLK ) begin : axi_bvalid_proc_cfg
        if (~RESETN)
            axi_bvalid_cfg  <= 0;
        else
            if (axi_awready_cfg && S_AXI_LITE_CFG_AWVALID && ~axi_bvalid_cfg && axi_wready_cfg && S_AXI_LITE_CFG_WVALID)
                axi_bvalid_cfg <= 1'b1;
            else
                if (S_AXI_LITE_CFG_BREADY && axi_bvalid_cfg)
                    axi_bvalid_cfg <= 1'b0; 
    end   



    always @( posedge CLK ) begin : axi_bresp_proc_cfg
        if (~RESETN)
            axi_bresp_cfg   <= 2'b0;
        else
            if (axi_awready_cfg && S_AXI_LITE_CFG_AWVALID && ~axi_bvalid_cfg && axi_wready_cfg && S_AXI_LITE_CFG_WVALID)
                axi_bresp_cfg  <= 2'b0; // 'OKAY' response 
    end   



    always @( posedge CLK ) begin : axi_arready_proc_cfg
        if (~RESETN)
            axi_arready_cfg <= 1'b0;
        else    
            if (~axi_arready_cfg && S_AXI_LITE_CFG_ARVALID)
                axi_arready_cfg <= 1'b1;
            else
                axi_arready_cfg <= 1'b0;
    end       


    always @( posedge CLK ) begin : axi_araddr_proc_cfg
        if (~RESETN)
            axi_araddr_cfg  <= 32'b0;
        else    
            if (~axi_arready_cfg && S_AXI_LITE_CFG_ARVALID)
                axi_araddr_cfg  <= S_AXI_LITE_CFG_ARADDR;
            
    end       



    always @( posedge CLK ) begin : axi_rvalid_proc_cfg
        if (~RESETN)
            axi_rvalid_cfg <= 0;
        else
            if (axi_arready_cfg && S_AXI_LITE_CFG_ARVALID && ~axi_rvalid_cfg)
                axi_rvalid_cfg <= 1'b1;
            else 
                if (axi_rvalid_cfg && S_AXI_LITE_CFG_RREADY)
                    axi_rvalid_cfg <= 1'b0;
    end    



    always @( posedge CLK ) begin : axi_rresp_proc_cfg
        if (~RESETN)
            axi_rresp_cfg  <= 0;
        else
            if (axi_arready_cfg && S_AXI_LITE_CFG_ARVALID && ~axi_rvalid_cfg)
                axi_rresp_cfg  <= 2'b0; // 'OKAY' response             
        
    end    


    always_comb begin 
        slv_reg_rden_cfg = axi_arready_cfg & S_AXI_LITE_CFG_ARVALID & ~axi_rvalid_cfg;
    end 

    always @(*) begin
        case ( axi_araddr_cfg[ADDR_LSB_CFG+OPT_MEM_ADDR_BITS_CFG:ADDR_LSB_CFG] )
            4'h0 : reg_data_out_cfg <= {
                version_major,
                version_minor,
                link_on,
                i2c_address, // register_cfg[ 0][14:8],
                on_work,
                4'b0,
                allow_irq,
                enable,
                reset 
            };

            4'h1    : reg_data_out_cfg <= request_interval;
            4'h2    : reg_data_out_cfg <= DATA_WIDTH;
            4'h3    : reg_data_out_cfg <= read_valid_reg;
            4'h4    : reg_data_out_cfg <= write_valid_reg;
            4'h5    : reg_data_out_cfg <= write_transactions;
            4'h6    : reg_data_out_cfg <= read_transactions;
            4'h7    : reg_data_out_cfg <= CLK_PERIOD;
            default : reg_data_out_cfg <= 0;
        endcase
    end



    always @( posedge CLK ) begin
        if (~RESETN)
            axi_rdata_cfg  <= 0;
        else 
            if (slv_reg_rden_cfg) 
                axi_rdata_cfg <= reg_data_out_cfg;     // register read data
    end    
    


    
    always @(posedge CLK) begin : slv_reg_cfg 
        if (~RESETN | reset)
            link_on <= 1'b0;
        else
            if (S_AXIS_TVALID)
                if (!address) begin 
                    if (S_AXIS_TDATA == 8'hE5) begin 
                        link_on <= 1'b1;
                    end else begin 
                        link_on <= 1'b0;
                    end 
                end 

    end    

    always_ff @(posedge CLK) begin 
        if (~RESETN | reset)
            i2c_address <= DEFAULT_DEVICE_ADDRESS;
        else 
            if (slv_reg_wren_cfg)
                if (axi_awaddr_cfg[ADDR_LSB_CFG + OPT_MEM_ADDR_BITS_CFG : ADDR_LSB_CFG] == 0)
                        if ( S_AXI_LITE_CFG_WSTRB[1] == 1 )
                            i2c_address <= S_AXI_LITE_CFG_WDATA[14:8];
    end 

    always_ff @(posedge CLK) begin 
        case (current_state)
            IDLE_ST : 
                if (~enable)
                    on_work <= 1'b0;
                else 
                    on_work <= 1'b1;
            
            default : 
                on_work <= 1'b1;

        endcase // write_cmd_word_cnt

    end


    always_ff @(posedge CLK) begin 
        if (~RESETN | reset)
            allow_irq <= 1'b0;
        else 
            if (slv_reg_wren_cfg)
                if (axi_awaddr_cfg[ADDR_LSB_CFG + OPT_MEM_ADDR_BITS_CFG : ADDR_LSB_CFG] == 0)
                        if ( S_AXI_LITE_CFG_WSTRB[0] == 1 )
                            allow_irq <= S_AXI_LITE_CFG_WDATA[2];
    end 

    always_ff @(posedge CLK) begin 
        if (~RESETN | reset)
            enable <= 1'b0;
        else 
            if (slv_reg_wren_cfg)
                if (axi_awaddr_cfg[ADDR_LSB_CFG + OPT_MEM_ADDR_BITS_CFG : ADDR_LSB_CFG] == 0)
                        if ( S_AXI_LITE_CFG_WSTRB[0] == 1 )
                            enable <= S_AXI_LITE_CFG_WDATA[1];
    end 

    always_ff @(posedge CLK) begin 
        if (~RESETN | reset )
            request_interval <= DEFAULT_REQUEST_INTERVAL;
        else 
            if (slv_reg_wren_cfg)
                if (axi_awaddr_cfg[ADDR_LSB_CFG + OPT_MEM_ADDR_BITS_CFG : ADDR_LSB_CFG] == 1)
                    for ( byte_index_cfg = 0; byte_index_cfg <= (S_AXI_LITE_CFG_DATA_WIDTH/8)-1; byte_index_cfg = byte_index_cfg + 1 )
                        if ( S_AXI_LITE_CFG_WSTRB[byte_index_cfg] == 1 )
                            request_interval[(byte_index_cfg*8) +: 8] <= S_AXI_LITE_CFG_WDATA[(byte_index_cfg*8) +: 8];
    end 



    always_ff @(posedge CLK) begin 
        if (~RESETN)
            transactions_timer <= '{default:0};
        else 
            if (transactions_timer < CLK_PERIOD-1) 
                transactions_timer <= transactions_timer + 1;
            else 
                transactions_timer <= '{default:0};
    end

    always_ff @(posedge CLK) begin 
        if (~RESETN)
            read_valid_count <= '{default:0};
        else 
            if (transactions_timer < CLK_PERIOD-1) begin 
                if (S_AXIS_TVALID & S_AXIS_TREADY) begin
                    read_valid_count <= read_valid_count + 1;
                end else begin  
                    read_valid_count <= read_valid_count;
                end  
            end else begin  
                read_valid_count <= '{default:0};
            end 
    end 

    always_ff @(posedge CLK) begin 
        if (~RESETN)
            read_valid_reg <= '{default:0};
        else 
            if (transactions_timer < (CLK_PERIOD-1)) begin 
                read_valid_reg <= read_valid_reg;
            end else begin 
                read_valid_reg <= read_valid_count;
            end 
    end 

    always_ff @(posedge CLK) begin 
        if (~RESETN)
            write_valid_count <= '{default:0};
        else 
            if (transactions_timer < (CLK_PERIOD-1)) begin 
                if (out_wren) begin
                    write_valid_count <= write_valid_count + 1;
                end else begin  
                    write_valid_count <= write_valid_count;
                end  
            end else begin  
                write_valid_count <= '{default:0};
            end 
    end 

    always_ff @(posedge CLK) begin 
        if (~RESETN)
            write_valid_reg <= '{default:0};
        else 
            if (transactions_timer < (CLK_PERIOD-1)) begin 
                write_valid_reg <= write_valid_reg;
            end else begin  
                write_valid_reg <= write_valid_count;
            end 
    end 

    always_ff @(posedge CLK) begin : write_transactions_proc
        if (~RESETN | reset)
            write_transactions <= '{default:0};
        else
            if (out_wren)
                if (out_din_last)
                    write_transactions <= write_transactions + 1;
    end 

    always_ff @(posedge CLK) begin : read_transactions_proc
        if (~RESETN | reset)
            read_transactions <= '{default:0};
        else
            if (S_AXIS_TVALID & S_AXIS_TREADY)
                if (S_AXIS_TLAST)
                    read_transactions <= read_transactions + 1;

    end 



    always_ff @(posedge CLK) begin : reset_logic_timer_proc 
        if (~RESETN)
            reset_logic_timer <= '{default:0};
        else
            if (reset_logic_timer < RESET_DURATION) begin 
                reset_logic_timer <= reset_logic_timer + 1;
            end else begin 
                if (slv_reg_wren_cfg)
                    if (axi_awaddr_cfg[ADDR_LSB_CFG + OPT_MEM_ADDR_BITS_CFG : ADDR_LSB_CFG] == 0)
                        if ( S_AXI_LITE_CFG_WSTRB[0] & S_AXI_LITE_CFG_WDATA[0])
                            reset_logic_timer <= '{default:0};
            end     
    end 



    always_ff @(posedge CLK) begin : reset_proc 
        if (~RESETN)
            reset <= 1'b1;
        else
            if (reset_logic_timer < RESET_DURATION)
                reset <= 1'b1;
            else 
                reset <= 1'b0;
    end 


endmodule
