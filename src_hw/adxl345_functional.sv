`timescale 1ns / 1ps


module adxl345_functional (
    input  logic        CLK                       ,
    input  logic        RESET                     ,
    // signal from AXI_DEV interface
    input  logic [31:0] WDATA                     ,
    input  logic [ 3:0] WSTRB                     ,
    input  logic [ 3:0] WADDR                     ,
    input  logic        WVALID                    ,

    input  logic [ 3:0] RADDR                     ,
    output logic [31:0] RDATA                     ,
    // control
    input  logic [ 6:0] I2C_ADDRESS               ,
    input  logic        ENABLE_INTERVAL_REQUESTION,
    input  logic [31:0] REQUESTION_INTERVAL       ,
    input  logic        SINGLE_REQUEST            ,
    output logic        SINGLE_REQUEST_COMPLETE   ,
    // data to device
    output logic [ 7:0] M_AXIS_TDATA              ,
    output logic [ 0:0] M_AXIS_TKEEP              ,
    output logic [ 7:0] M_AXIS_TUSER              ,
    output logic        M_AXIS_TVALID             ,
    output logic        M_AXIS_TLAST              ,
    input  logic        M_AXIS_TREADY             ,
    // data from device
    input  logic [ 7:0] S_AXIS_TDATA              ,
    input  logic [ 0:0] S_AXIS_TKEEP              ,
    input  logic [ 7:0] S_AXIS_TUSER              ,
    input  logic        S_AXIS_TVALID             ,
    input  logic        S_AXIS_TLAST              ,
    output logic        S_AXIS_TREADY
);

    localparam [7:0] ADDRESS_LIMIT       = 8'h3A;
    localparam [7:0] ADDRESS_WRITE_BEGIN = 8'h1D;
    localparam [7:0] ADDRESS_WRITE_END   = 8'h38;

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

    logic need_update_flaq = 'b0;

    typedef enum {
        IDLE_ST             , // await new action
        // if request data flaq
        REQ_TX_ADDR_PTR_ST  , // send address pointer 
        REQ_TX_READ_DATA_ST , // send read request for reading 0x39 data bytes 
        REQ_RX_READ_DATA_ST , // await data from start to tlast signal 
        // if need update flaq asserted
        UPD_CHK_FLAQ_ST         ,
        UPD_TX_DATA_ST          , 
        UPD_INCREMENT_ADDR_ST   ,

        WAIT_ST              
    } fsm;

    fsm current_state = IDLE_ST;


    logic request_flaq = 'b0;

    // periodic requesting data for interval time
    logic [31:0] requestion_interval_counter  = '{default:0};
    logic        requestion_interval_assigned = 1'b0        ;

    logic [7:0] out_din_data = '{default:0};
    logic [0:0] out_din_keep = '{default:0};
    logic [7:0] out_din_user = '{default:0};
    logic       out_din_last = 1'b0        ;
    logic       out_wren     = 1'b0        ;
    logic       out_full                   ;
    logic       out_awfull                 ;

    logic [3:0] word_counter = '{default:0};
    logic [7:0] address_ptr  = '{default:0};


    // write memory signal group : 32 bit input, 8 bit output;
    logic [ 3:0] write_memory_addra = '{default:0};
    logic [ 7:0] write_memory_doutb               ;
    logic [ 5:0] write_memory_addrb = '{default:0};
    logic [31:0] write_memory_dina  = '{default:0};
    logic [ 3:0] write_memory_wea   = '{default:0};

    // read memory signal group : 8 bit input 32 bit output;
    logic [ 5:0] read_memory_addra = '{default:0};
    logic [31:0] read_memory_doutb               ;
    logic [ 3:0] read_memory_addrb = '{default:0};
    logic [ 7:0] read_memory_dina  = '{default:0};
    logic        read_memory_wea                 ;

    logic [3:0] write_memory_hi;
    logic [1:0] write_memory_lo;

    always_comb begin : RDATA_processing 
        RDATA = read_memory_doutb;
    end 

    always_comb begin : write_memory_hi_processing 
        write_memory_hi = write_memory_addrb[5:2];
    end 

    always_comb begin : write_memory_lo_processing 
        write_memory_lo = write_memory_addrb[1:0];
    end 

    generate
    
        for (genvar index = 0; index < 4; index++) begin
            
            always_ff @(posedge CLK) begin : need_update_reg_processing
                if (WVALID & WSTRB[index]) begin
                    need_update_reg[WADDR][index] <= write_mask_register[WADDR][index];
                end else begin
                    need_update_reg[WADDR][index] <= need_update_reg[WADDR][index];
                end
            end 

        end 

    endgenerate



    always_ff @(posedge CLK) begin : write_memory_wea_processing 
        write_memory_wea <= {4{WVALID}} & write_mask_register[WADDR] & WSTRB;
    end 



    always_ff @(posedge CLK) begin : write_memory_dina_processing  
        if (WVALID) begin 
            write_memory_dina <= WDATA;
        end else begin 
            write_memory_dina <= write_memory_dina;
        end     
    end 

    always_comb begin : write_memory_addra_processing
        write_memory_addra = WADDR;
    end 

    always_ff @(posedge CLK) begin 
        if (|write_memory_wea) begin 
            need_update_flaq <= 1'b1;
        end else begin 
            // to do : deassert according fsm
            case (current_state)
                UPD_CHK_FLAQ_ST : 
                    need_update_flaq <= 1'b0;

                default : 
                    need_update_flaq <= need_update_flaq;
            endcase // current_state
        end 
    end 



    // if needed request, OR for this register
    always_ff @(posedge CLK) begin : request_flaq_processing 
        request_flaq <= SINGLE_REQUEST | requestion_interval_assigned;
    end 


    always_ff @(posedge CLK) begin : write_memory_addrb_processing 
        case (current_state)
            IDLE_ST : 
                write_memory_addrb <= ADDRESS_WRITE_BEGIN;

            UPD_INCREMENT_ADDR_ST : 
                write_memory_addrb <= write_memory_addrb + 1;

            default : 
                write_memory_addrb <= write_memory_addrb;
        endcase // current_state
    end 


    always_ff @(posedge CLK) begin : requestion_interval_counter_processing 
        case (current_state) 
            IDLE_ST : 
                if (ENABLE_INTERVAL_REQUESTION) begin 
                    if (requestion_interval_counter == 0) begin 
                        requestion_interval_counter <= requestion_interval_counter;
                    end else begin 
                        requestion_interval_counter <= requestion_interval_counter - 1;
                    end 
                end else begin 
                    requestion_interval_counter <= REQUESTION_INTERVAL;
                end 

            default : 
                requestion_interval_counter <= REQUESTION_INTERVAL;

        endcase
    end 


    always_ff @(posedge CLK) begin : requestion_interval_assigned_processing 
        case (current_state) 
            IDLE_ST : 
                if (ENABLE_INTERVAL_REQUESTION) begin 
                    if (requestion_interval_counter == 0) begin 
                        requestion_interval_assigned <= 1'b1;
                    end else begin 
                        requestion_interval_assigned <= 1'b0; 
                    end 
                end else begin 
                    requestion_interval_assigned <= 1'b0;
                end 

            default : 
                requestion_interval_assigned <= 1'b0;

        endcase // current_state
    end 



    xpm_memory_sdpram #(
        .ADDR_WIDTH_A           (4              ), // DECIMAL
        .ADDR_WIDTH_B           (6              ), // DECIMAL
        .AUTO_SLEEP_TIME        (0              ), // DECIMAL
        .BYTE_WRITE_WIDTH_A     (8              ), // DECIMAL
        .CASCADE_HEIGHT         (0              ), // DECIMAL
        .CLOCKING_MODE          ("common_clock" ), // String
        .ECC_MODE               ("no_ecc"       ), // String
        .MEMORY_INIT_FILE       ("none"         ), // String
        .MEMORY_INIT_PARAM      ("0"            ), // String
        .MEMORY_OPTIMIZATION    ("true"         ), // String
        .MEMORY_PRIMITIVE       ("auto"         ), // String
        .MEMORY_SIZE            (512            ), // DECIMAL
        .MESSAGE_CONTROL        (0              ), // DECIMAL
        .READ_DATA_WIDTH_B      (8              ), // DECIMAL
        .READ_LATENCY_B         (2              ), // DECIMAL
        .READ_RESET_VALUE_B     ("0"            ), // String
        .RST_MODE_A             ("SYNC"         ), // String
        .RST_MODE_B             ("SYNC"         ), // String
        .SIM_ASSERT_CHK         (0              ), // DECIMAL; 0=disable simulation messages, 1=enable simulation messages
        .USE_EMBEDDED_CONSTRAINT(0              ), // DECIMAL
        .USE_MEM_INIT           (1              ), // DECIMAL
        .WAKEUP_TIME            ("disable_sleep"), // String
        .WRITE_DATA_WIDTH_A     (32             ), // DECIMAL
        .WRITE_MODE_B           ("no_change"    )  // String
    ) xpm_memory_sdpram_write_inst (
        .dbiterrb      (                  ), // 1-bit output: Status signal to indicate double bit error occurrence
        .sbiterrb      (                  ), // 1-bit output: Status signal to indicate single bit error occurrence
        .doutb         (write_memory_doutb), // READ_DATA_WIDTH_B-bit output: Data output for port B read operations.
        .addra         (write_memory_addra), // ADDR_WIDTH_A-bit input: Address for port A write operations.
        .addrb         (write_memory_addrb), // ADDR_WIDTH_B-bit input: Address for port B read operations.
        .clka          (CLK               ), // 1-bit input: Clock signal for port A. Also clocks port B when
        .clkb          (CLK               ), // 1-bit input: Clock signal for port B when parameter CLOCKING_MODE is
        .dina          (write_memory_dina ), // WRITE_DATA_WIDTH_A-bit input: Data input for port A write operations.
        .ena           (1'b1              ), // 1-bit input: Memory enable signal for port A. Must be high on clock
        .enb           (1'b1              ), // 1-bit input: Memory enable signal for port B. Must be high on clock
        .injectdbiterra(1'b0              ), // 1-bit input: Controls double bit error injection on input data when
        .injectsbiterra(1'b0              ), // 1-bit input: Controls single bit error injection on input data when
        .regceb        (1'b1              ), // 1-bit input: Clock Enable for the last register stage on the output
        .rstb          (RESET             ), // 1-bit input: Reset signal for the final port B output register stage.
        .sleep         (1'b0              ), // 1-bit input: sleep signal to enable the dynamic power saving feature.
        .wea           (write_memory_wea  )  // WRITE_DATA_WIDTH_A/BYTE_WRITE_WIDTH_A-bit input: Write enable vector
    );



    xpm_memory_sdpram #(
        .ADDR_WIDTH_A           (6              ), // DECIMAL
        .ADDR_WIDTH_B           (4              ), // DECIMAL
        .AUTO_SLEEP_TIME        (0              ), // DECIMAL
        .BYTE_WRITE_WIDTH_A     (8              ), // DECIMAL
        .CASCADE_HEIGHT         (0              ), // DECIMAL
        .CLOCKING_MODE          ("common_clock" ), // String
        .ECC_MODE               ("no_ecc"       ), // String
        .MEMORY_INIT_FILE       ("none"         ), // String
        .MEMORY_INIT_PARAM      ("0"            ), // String
        .MEMORY_OPTIMIZATION    ("true"         ), // String
        .MEMORY_PRIMITIVE       ("auto"         ), // String
        .MEMORY_SIZE            (512            ), // DECIMAL
        .MESSAGE_CONTROL        (0              ), // DECIMAL
        .READ_DATA_WIDTH_B      (32             ), // DECIMAL
        .READ_LATENCY_B         (1              ), // DECIMAL
        .READ_RESET_VALUE_B     ("0"            ), // String
        .RST_MODE_A             ("SYNC"         ), // String
        .RST_MODE_B             ("SYNC"         ), // String
        .SIM_ASSERT_CHK         (0              ), // DECIMAL; 0=disable simulation messages, 1=enable simulation messages
        .USE_EMBEDDED_CONSTRAINT(0              ), // DECIMAL
        .USE_MEM_INIT           (1              ), // DECIMAL
        .WAKEUP_TIME            ("disable_sleep"), // String
        .WRITE_DATA_WIDTH_A     (8              ), // DECIMAL
        .WRITE_MODE_B           ("no_change"    )  // String
    ) xpm_memory_sdpram_read_inst (
        .dbiterrb      (                 ), // 1-bit output: Status signal to indicate double bit error occurrence
        .sbiterrb      (                 ), // 1-bit output: Status signal to indicate single bit error occurrence
        .doutb         (read_memory_doutb), // READ_DATA_WIDTH_B-bit output: Data output for port B read operations.
        .addra         (read_memory_addra), // ADDR_WIDTH_A-bit input: Address for port A write operations.
        .addrb         (read_memory_addrb), // ADDR_WIDTH_B-bit input: Address for port B read operations.
        .clka          (CLK              ), // 1-bit input: Clock signal for port A. Also clocks port B when
        .clkb          (CLK              ), // 1-bit input: Clock signal for port B when parameter CLOCKING_MODE is
        .dina          (read_memory_dina ), // WRITE_DATA_WIDTH_A-bit input: Data input for port A write operations.
        .ena           (1'b1             ), // 1-bit input: Memory enable signal for port A. Must be high on clock
        .enb           (1'b1             ), // 1-bit input: Memory enable signal for port B. Must be high on clock
        .injectdbiterra(1'b0             ), // 1-bit input: Controls double bit error injection on input data when
        .injectsbiterra(1'b0             ), // 1-bit input: Controls single bit error injection on input data when
        .regceb        (1'b1             ), // 1-bit input: Clock Enable for the last register stage on the output
        .rstb          (RESET            ), // 1-bit input: Reset signal for the final port B output register stage.
        .sleep         (1'b0             ), // 1-bit input: sleep signal to enable the dynamic power saving feature.
        .wea           (read_memory_wea  )  // WRITE_DATA_WIDTH_A/BYTE_WRITE_WIDTH_A-bit input: Write enable vector
    );


    always_comb begin 
        read_memory_addrb = RADDR;
    end 

    // address : sets before receive data, implies on MEM
    always_ff @(posedge CLK) begin : read_memory_addra_processing 
        case (current_state)
            REQ_TX_READ_DATA_ST : 
                read_memory_addra <= address_ptr;

            REQ_RX_READ_DATA_ST : 
                if (read_memory_wea) begin 
                    read_memory_addra <= read_memory_addra + 1;
                end else begin 
                    read_memory_addra <= read_memory_addra;
                end 

            default : 
                read_memory_addra <= read_memory_addra;

        endcase
    end 


    // readed data from interface S_AXIS_ to porta for readmemory
    always_ff @(posedge CLK) begin : read_memory_wea_processing
        case (current_state)
            REQ_RX_READ_DATA_ST : 
                read_memory_wea <= S_AXIS_TVALID;

            default : 
                read_memory_wea <= 1'b0;
        endcase // current_state
    end 

    
    
    always_ff @(posedge CLK) begin : read_memory_dina_processing 
        case (current_state)
            REQ_RX_READ_DATA_ST : 
                read_memory_dina <= S_AXIS_TDATA;

            default 
                read_memory_dina <= read_memory_dina;
        endcase // current_state
    end 

    fifo_out_sync_tuser_xpm #(
        .DATA_WIDTH(8      ),
        .USER_WIDTH(8      ),
        .MEMTYPE   ("block"),
        .DEPTH     (16     )
    ) fifo_out_sync_tuser_xpm_inst (
        .CLK          (CLK          ),
        .RESET        (RESET        ),
        .OUT_DIN_DATA (out_din_data ),
        .OUT_DIN_KEEP (out_din_keep ),
        .OUT_DIN_USER (out_din_user ),
        .OUT_DIN_LAST (out_din_last ),
        .OUT_WREN     (out_wren     ),
        .OUT_FULL     (out_full     ),
        .OUT_AWFULL   (out_awfull   ),
        .M_AXIS_TDATA (M_AXIS_TDATA ),
        .M_AXIS_TKEEP (M_AXIS_TKEEP ),
        .M_AXIS_TUSER (M_AXIS_TUSER ),
        .M_AXIS_TVALID(M_AXIS_TVALID),
        .M_AXIS_TLAST (M_AXIS_TLAST ),
        .M_AXIS_TREADY(M_AXIS_TREADY)
    );

    always_comb begin 
        out_din_user[7:1] = I2C_ADDRESS;
    end 


    // operation : 
    // 0 - write
    // 1 - read
    always_ff @(posedge CLK) begin 
        case (current_state) 
            REQ_TX_ADDR_PTR_ST : 
                out_din_user[0] <= 1'b0; // is writing data to dev

            REQ_TX_READ_DATA_ST : 
                out_din_user[0] <= 1'b1; // cmd for reading data from dev

            UPD_TX_DATA_ST : 
                out_din_user[0] <= 1'b0;

            default : 
                out_din_user[0] <= out_din_user[0];
        endcase // current_state
    end 



    always_ff @(posedge CLK) begin 
        case (current_state) 
            REQ_TX_ADDR_PTR_ST : 
                case (word_counter) 
                    4'h0    : out_din_data <= 8'h01; // how many bytes write
                    4'h1    : out_din_data <= address_ptr; // address pointer
                    default : out_din_data <= out_din_data;
                endcase // word_counter

            REQ_TX_READ_DATA_ST : 
                out_din_data <= ADDRESS_LIMIT;

            UPD_TX_DATA_ST : 
                case (word_counter)
                    4'h0 : out_din_data <= 8'h01;
                    4'h1 : out_din_data <= write_memory_addrb;
                    4'h2 : out_din_data <= write_memory_doutb;
                    default : out_din_data <= out_din_data;
                endcase // word_counter

            default : 
                out_din_data <= out_din_data;
        endcase // current_state
    end 


    always_ff @(posedge CLK) begin 
        case (current_state) 
            REQ_TX_ADDR_PTR_ST : 
                case (word_counter) 
                    4'h0    : out_din_last <= 1'b0;
                    4'h1    : out_din_last <= 1'b1;
                    default : out_din_last <= out_din_last;
                endcase // word_counter

            UPD_TX_DATA_ST : 
                case (word_counter) 
                    4'h0    : out_din_last <= 1'b0;
                    4'h1    : out_din_last <= 1'b0;
                    4'h2    : out_din_last <= 1'b1;
                    default : out_din_last <= out_din_last;
                endcase // word_counter

            default : 
                out_din_last <= out_din_last;

        endcase // current_state
    end 



    always_ff @(posedge CLK) begin 
        case (current_state) 
            REQ_TX_ADDR_PTR_ST : 
                if (!out_awfull) begin 
                    out_wren <= 1'b1;
                end else begin 
                    out_wren <= 1'b0;
                end 

            REQ_TX_READ_DATA_ST : 
                if (!out_awfull) begin 
                    out_wren <= 1'b1; 
                end else begin 
                    out_wren <= 1'b1;
                end 

            UPD_TX_DATA_ST: 
                if (!out_awfull) begin 
                    out_wren <= 1'b1;
                end else begin 
                    out_wren <= 1'b0;
                end 

            default : 
                out_wren <= 1'b0;

        endcase // current_state
    end 



    always_ff @(posedge CLK) begin 
        if (RESET) begin 
            current_state <= IDLE_ST;
        end else begin 
            case (current_state) 
                IDLE_ST             :
                    if (need_update_reg) begin 
                        current_state <= UPD_CHK_FLAQ_ST;
                    end else begin 
                        if (request_flaq) begin 
                            current_state <= REQ_TX_ADDR_PTR_ST;
                        end else begin 
                            current_state <= current_state;
                        end 
                    end 

                REQ_TX_ADDR_PTR_ST  :
                    if (!out_awfull) begin 
                        if (word_counter == 4'h1) begin 
                            current_state <= REQ_TX_READ_DATA_ST;
                        end else begin 
                            current_state <= current_state;
                        end 
                    end else begin 
                        current_state <= current_state;
                    end 

                REQ_TX_READ_DATA_ST :
                    if (!out_awfull) begin 
                        current_state <= REQ_RX_READ_DATA_ST;
                    end else begin 
                        current_state <= current_state;
                    end 

                REQ_RX_READ_DATA_ST : 
                    if (S_AXIS_TVALID & S_AXIS_TLAST) begin 
                        current_state <= IDLE_ST;
                    end else begin 
                        current_state <= current_state;
                    end 

                UPD_CHK_FLAQ_ST : 
                    if (need_update_reg[write_memory_hi][write_memory_lo]) begin 
                        current_state <= UPD_TX_DATA_ST;
                    end else begin 
                        current_state <= UPD_INCREMENT_ADDR_ST;
                    end 

                UPD_TX_DATA_ST :
                    if (!out_awfull) begin 
                        if (word_counter == 4'h2) begin 
                            current_state <= UPD_INCREMENT_ADDR_ST;
                        end else begin 
                            current_state <= current_state;
                        end 
                    end else begin 
                        current_state <= current_state;
                    end 

                UPD_INCREMENT_ADDR_ST : 
                    if (write_memory_addrb == ADDRESS_WRITE_END) begin 
                        current_state <= IDLE_ST;
                    end else begin 
                        current_state <= UPD_CHK_FLAQ_ST;
                    end 

                default             : 
                    current_state <= current_state;
            endcase // current_state
        end 
    end 


    always_ff @(posedge CLK) begin 
        case (current_state) 
            IDLE_ST :
                if (request_flaq) begin 
                    address_ptr <= 8'h00;
                end else begin 
                    address_ptr <= address_ptr;
                end 

            default : 
                address_ptr <= address_ptr;
        endcase // current_state
    end 


    always_ff @(posedge CLK) begin 
        case (current_state)
            REQ_TX_ADDR_PTR_ST : 
                if (!out_awfull) begin 
                    word_counter <= word_counter + 1;
                end else begin 
                    word_counter <= word_counter;
                end 

            UPD_TX_DATA_ST : 
                if (!out_awfull) begin 
                    word_counter <= word_counter + 1;
                end else begin 
                    word_counter <= word_counter;
                end 

            default : 
                word_counter <= '{default:0};
        endcase // current_state
    end 


    always_ff @(posedge CLK) begin 
        case (current_state) 
            REQ_TX_READ_DATA_ST : 
                if (!out_awfull) begin 
                    SINGLE_REQUEST_COMPLETE <= SINGLE_REQUEST;
                end else begin 
                    SINGLE_REQUEST_COMPLETE <= SINGLE_REQUEST_COMPLETE;
                end 

            default : 
                SINGLE_REQUEST_COMPLETE <= 1'b0;

        endcase // current_state
    end 

endmodule