`timescale 1ns / 1ps


module adxl345_functional #(parameter integer CLK_PERIOD = 100000000) (
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
    input  logic        ALLOW_IRQ                 ,
    output logic        LINK_ON                   ,
    input  logic        ADXL_INTERRUPT            ,
    output logic        ADXL_IRQ                  ,
    input  logic        ADXL_IRQ_ACK              ,
    input  logic        CALIBRATION               ,
    input  logic [ 4:0] CALIBRATION_MODE          ,
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

    localparam [47:0] OPT_REQ_INTERVAL = (CLK_PERIOD/3200); // optimal requestion interval constant for data

    localparam [7:0] ADDRESS_LIMIT       = 8'h3A;
    localparam [7:0] ADDRESS_WRITE_BEGIN = 8'h1D;
    localparam [7:0] ADDRESS_WRITE_END   = 8'h38;

    // constant parameters for comparison
    localparam [5:0] DEVICE_ID_ADDR      = 6'h00;
    localparam [5:0] OFSX_ADDR           = 6'h1E;
    localparam [5:0] BW_RATE_ADDR        = 6'h2C;
    localparam [5:0] INT_SOURCE_ADDR     = 6'h30;
    localparam [5:0] DATAX0_ADDR         = 6'h32;

    localparam [7:0] DEVICE_ID           = 8'hE5;

    logic [0:31][31:0] calibration_count_limit_rom = '{
        32'h00000001, 32'h00000002, 32'h00000004, 32'h00000008,
        32'h00000010, 32'h00000020, 32'h00000040, 32'h00000080,
        32'h00000100, 32'h00000200, 32'h00000400, 32'h00000800,
        32'h00001000, 32'h00002000, 32'h00004000, 32'h00008000,
        32'h00010000, 32'h00020000, 32'h00040000, 32'h00080000,
        32'h00100000, 32'h00200000, 32'h00400000, 32'h00800000,
        32'h01000000, 32'h02000000, 32'h04000000, 32'h08000000,
        32'h10000000, 32'h20000000, 32'h40000000, 32'h80000000 
    };

    logic [31:0] calibration_count_limit = '{default:0};
    logic [31:0] calibration_count       = '{default:0};

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
        IDLE_CHK_REQ_ST         , // await new action
        IDLE_CHK_UPD_ST         , // await new action
        IDLE_CHK_CAL_ST         , // checking calibration state
        // if request data flaq
        REQ_TX_ADDR_PTR_ST      , // send address pointer 
        REQ_TX_READ_DATA_ST     , // send read request for reading 0x39 data bytes 
        REQ_RX_READ_DATA_ST     , // await data from start to tlast signal 
        // if need update flaq asserted
        UPD_CHK_FLAQ_ST         ,
        UPD_TX_DATA_ST          , 
        UPD_INCREMENT_ADDR_ST   ,

        CAL_TX_OFS_ZEROS_ST     , // sending zeros for reset OFSX, OFSY, OFSZ

        WAIT_ST              
    } fsm;

    fsm current_state = IDLE_CHK_REQ_ST;


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

    logic interrupt      = 1'b0;

    logic calibration_flaq = 1'b0;


    logic [47:0] cal_optimal_request_timer_limit        = '{default:0};
    logic [47:0] cal_optimal_request_timer              = '{default:0};
    logic [ 7:0] bw_rate_reg                            = '{default:0};
    logic        allow_cal_optimal_request_timer        = 1'b0        ;
    logic        has_cal_optimal_request_timer_exceeded = 1'b0        ;
    logic        has_calibration_count_exceeded         = 1'b0        ;



    always_comb begin : RDATA_processing 
        RDATA = read_memory_doutb;
    end 



    always_comb begin : write_memory_hi_processing 
        write_memory_hi = write_memory_addrb[5:2];
    end 



    always_comb begin : write_memory_lo_processing 
        write_memory_lo = write_memory_addrb[1:0];
    end 



    always_ff @(posedge CLK) begin : interrupt_processing 
        if (ADXL_INTERRUPT & ALLOW_IRQ) begin 
            interrupt <= 1'b1;
        end else begin 
            interrupt <= 1'b0;
        end 
    end 



    always_ff @(posedge CLK) begin : adxl_irq_processing 
        if (ADXL_IRQ_ACK | RESET) begin 
            ADXL_IRQ <= 1'b0;
        end else begin 
            if (!ADXL_INTERRUPT & ALLOW_IRQ & interrupt) begin 
                ADXL_IRQ <= 1'b1;
            end else begin 
                ADXL_IRQ <= ADXL_IRQ;
            end 
        end 
    end 


    generate
    
        for (genvar index = 0; index < 4; index++) begin
            
            always_ff @(posedge CLK) begin : need_update_reg_processing
                if (WVALID & WSTRB[index]) begin
                    need_update_reg[WADDR][index] <= write_mask_register[WADDR][index];
                end else begin
                    if (index == write_memory_lo) begin 
                        if (current_state == UPD_CHK_FLAQ_ST) begin 
                            need_update_reg[write_memory_hi][index] <= 1'b0;
                        end else begin 
                            need_update_reg[write_memory_hi][index] <= need_update_reg[write_memory_hi][index];
                        end 
                    end else begin 
                        need_update_reg[WADDR][index] <= need_update_reg[WADDR][index];
                    end 
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
                IDLE_CHK_UPD_ST : 
                    if (need_update_flaq) begin 
                        need_update_flaq <= 1'b0;
                    end else begin 
                        need_update_flaq <= need_update_flaq;
                    end 

                default : 
                    need_update_flaq <= need_update_flaq;
            endcase // current_state
        end 
    end 



    // if needed request, OR for this register
    always_ff @(posedge CLK) begin : request_flaq_processing 
        request_flaq <= SINGLE_REQUEST | requestion_interval_assigned | interrupt | has_cal_optimal_request_timer_exceeded;
    end 



    always_ff @(posedge CLK) begin : write_memory_addrb_processing 
        case (current_state)
            IDLE_CHK_UPD_ST : 
                write_memory_addrb <= ADDRESS_WRITE_BEGIN;

            UPD_INCREMENT_ADDR_ST : 
                write_memory_addrb <= write_memory_addrb + 1;

            default : 
                write_memory_addrb <= write_memory_addrb;
        endcase // current_state
    end 



    always_ff @(posedge CLK) begin : requestion_interval_counter_processing 
        case (current_state) 

            REQ_RX_READ_DATA_ST : 
                requestion_interval_counter <= REQUESTION_INTERVAL;

            default : 
                if (ENABLE_INTERVAL_REQUESTION) begin 
                    if (requestion_interval_counter == 0) begin 
                        requestion_interval_counter <= requestion_interval_counter;
                    end else begin 
                        requestion_interval_counter <= requestion_interval_counter - 1;
                    end 
                end else begin 
                    requestion_interval_counter <= REQUESTION_INTERVAL;
                end 

        endcase
    end 



    always_ff @(posedge CLK) begin : requestion_interval_assigned_processing 
        if (ENABLE_INTERVAL_REQUESTION) begin 
            if (requestion_interval_counter == 0) begin 
                requestion_interval_assigned <= ~ADXL_IRQ;
            end else begin 
                requestion_interval_assigned <= 1'b0; 
            end 
        end else begin 
            requestion_interval_assigned <= 1'b0;
        end 
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



    always_comb begin : read_memory_addrb_processing
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



    always_ff @(posedge CLK) begin : bw_rate_reg_processing 
        if (read_memory_addra == BW_RATE_ADDR) begin 
            if (read_memory_wea) begin 
                bw_rate_reg <= read_memory_dina;
            end else begin 
                bw_rate_reg <= bw_rate_reg;
            end
        end else begin 
            bw_rate_reg <= bw_rate_reg;
        end 
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



    always_comb begin : out_din_user_processing 
        out_din_user[7:1] = I2C_ADDRESS;
    end 


    // operation : 
    // 0 - write
    // 1 - read
    always_ff @(posedge CLK) begin : out_din_user_0_processing
        case (current_state) 
            REQ_TX_ADDR_PTR_ST : 
                out_din_user[0] <= 1'b0; // is writing data to dev

            REQ_TX_READ_DATA_ST : 
                out_din_user[0] <= 1'b1; // cmd for reading data from dev

            UPD_TX_DATA_ST : 
                out_din_user[0] <= 1'b0;

            CAL_TX_OFS_ZEROS_ST : 
                out_din_user[0] <= 1'b0; // writing operation for clear OFSX OFSY OFSZ

            default : 
                out_din_user[0] <= out_din_user[0];
        endcase // current_state
    end 



    always_ff @(posedge CLK) begin : out_din_data_processing 
        case (current_state) 
            REQ_TX_ADDR_PTR_ST : 
                case (word_counter) 
                    4'h0    : out_din_data <= 8'h01; // how many bytes write
                    4'h1    : out_din_data <= address_ptr; // address pointer
                    default : out_din_data <= out_din_data;
                endcase // word_counter

            REQ_TX_READ_DATA_ST : 
                if (interrupt) begin 
                    out_din_data <= 8'h08;
                end else begin 
                    if (has_cal_optimal_request_timer_exceeded) begin 
                        out_din_data <= 8'h06;
                    end else begin 
                        out_din_data <= ADDRESS_LIMIT;
                    end 
                end 

            UPD_TX_DATA_ST : 
                case (word_counter)
                    4'h0 : out_din_data <= 8'h02;
                    4'h1 : out_din_data <= write_memory_addrb;
                    4'h2 : out_din_data <= write_memory_doutb;
                    default : out_din_data <= out_din_data;
                endcase // word_counter

            CAL_TX_OFS_ZEROS_ST : 
                case (word_counter)
                    4'h0 : out_din_data <= 8'h04;
                    4'h1 : out_din_data <= address_ptr;
                    4'h2 : out_din_data <= 8'h00;
                    4'h3 : out_din_data <= 8'h00;
                    4'h4 : out_din_data <= 8'h00;
                    default out_din_data <= out_din_data;
                endcase // word_counter

            default : 
                out_din_data <= out_din_data;
        endcase // current_state
    end 



    always_ff @(posedge CLK) begin : out_din_last_processing 
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

            CAL_TX_OFS_ZEROS_ST : 
                case (word_counter)
                    4'h0 : out_din_last <= 1'b0;
                    4'h1 : out_din_last <= 1'b0;
                    4'h2 : out_din_last <= 1'b0;
                    4'h3 : out_din_last <= 1'b0;
                    4'h4 : out_din_last <= 1'b1;
                    default out_din_last <= out_din_last;
                endcase // word_counter


            default : 
                out_din_last <= out_din_last;

        endcase // current_state
    end 



    always_ff @(posedge CLK) begin : out_wren_processing 
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

            CAL_TX_OFS_ZEROS_ST : 
                if (!out_awfull) begin 
                    out_wren <= 1'b1;
                end else begin 
                    out_wren <= 1'b0;
                end 

            default : 
                out_wren <= 1'b0;

        endcase // current_state
    end 



    always_ff @(posedge CLK) begin : calibration_flaq_processing 
        if (RESET) begin 
            calibration_flaq <= 1'b0;
        end else begin 
            case (current_state) 
        
                CAL_TX_OFS_ZEROS_ST : 
                    calibration_flaq <= 1'b0;

                default : 
                    if (CALIBRATION) begin 
                        calibration_flaq <= 1'b1;
                    end else begin 
                        calibration_flaq <= calibration_flaq;
                    end 

            endcase // word_counter
        end 
    end 



    always_ff @(posedge CLK) begin : calibration_count_limit_processing 
        if (calibration_flaq) begin 
            calibration_count_limit <= calibration_count_limit_rom[CALIBRATION_MODE];
        end else begin 
            calibration_count_limit <= calibration_count_limit;
        end 
    end 



    always_ff @(posedge CLK) begin : current_state_processing 
        if (RESET) begin 
            current_state <= IDLE_CHK_REQ_ST;
        end else begin 
            case (current_state) 

                IDLE_CHK_REQ_ST : 
                    if (request_flaq) begin 
                        current_state <= REQ_TX_ADDR_PTR_ST;
                    end else begin 
                        current_state <= IDLE_CHK_UPD_ST;
                    end 

                IDLE_CHK_UPD_ST : 
                    if (need_update_flaq) begin 
                        current_state <= UPD_CHK_FLAQ_ST;
                    end else begin 
                        current_state <= IDLE_CHK_CAL_ST;
                    end 

                IDLE_CHK_CAL_ST : 
                    if (calibration_flaq) begin 
                        current_state <= CAL_TX_OFS_ZEROS_ST;
                    end else begin 
                        current_state <= IDLE_CHK_REQ_ST;
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
                        current_state <= IDLE_CHK_UPD_ST;
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
                        current_state <= IDLE_CHK_CAL_ST;
                    end else begin 
                        current_state <= UPD_CHK_FLAQ_ST;
                    end 

                CAL_TX_OFS_ZEROS_ST : 
                    if (!out_awfull) begin 
                        if (word_counter == 4'h4) begin 
                            current_state <= IDLE_CHK_REQ_ST;
                        end else begin 
                            current_state <= current_state;
                        end 
                    end else begin 
                        current_state <= current_state;
                    end 

                default             : 
                    current_state <= current_state;
            endcase // current_state
        end 
    end 



    always_ff @(posedge CLK) begin : address_ptr_processing 
        case (current_state) 
            
            IDLE_CHK_REQ_ST : 
                if (interrupt) begin 
                    address_ptr <= INT_SOURCE_ADDR;
                end else begin 
                    if (has_cal_optimal_request_timer_exceeded) begin 
                        address_ptr <= DATAX0_ADDR;
                    end else begin 
                        address_ptr <= DEVICE_ID_ADDR;
                    end 
                end 

            IDLE_CHK_CAL_ST : 
                address_ptr <= OFSX_ADDR;
            
            default : 
                address_ptr <= address_ptr;
        endcase // current_state
    end 



    always_ff @(posedge CLK) begin : word_counter_processing 
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

            CAL_TX_OFS_ZEROS_ST : 
                if (!out_awfull) begin 
                    word_counter <= word_counter + 1;
                end else begin 
                    word_counter <= word_counter;
                end 

            default : 
                word_counter <= '{default:0};
        endcase // current_state
    end 



    always_ff @(posedge CLK) begin : SINGLE_REQUEST_COMPLETE_processing 
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



    always_ff @(posedge CLK) begin : LINK_ON_processing 
        if (RESET) begin 
            LINK_ON <= 1'b0;
        end else begin 
            case (current_state)
                REQ_RX_READ_DATA_ST : 
                    if (read_memory_addra == DEVICE_ID_ADDR) begin 
                        if (read_memory_dina == DEVICE_ID) begin 
                            LINK_ON <= 1'b1;
                        end else begin 
                            LINK_ON <= 1'b0;
                        end 
                    end else begin 
                        LINK_ON <= LINK_ON;
                    end 

                default : 
                    LINK_ON <= LINK_ON;

            endcase // word_counter
        end 
    end 



    always_ff @(posedge CLK) begin : cal_optimal_request_timer_limit_processing 
        case (bw_rate_reg[3:0]) 
            8'hF : 
                cal_optimal_request_timer_limit <= OPT_REQ_INTERVAL;
            8'hE :
                cal_optimal_request_timer_limit <= (OPT_REQ_INTERVAL<<1); 
            8'hD :
                cal_optimal_request_timer_limit <= (OPT_REQ_INTERVAL<<2); 
            8'hC : 
                cal_optimal_request_timer_limit <= (OPT_REQ_INTERVAL<<3);
            8'hB : 
                cal_optimal_request_timer_limit <= (OPT_REQ_INTERVAL<<4);
            8'hA : 
                cal_optimal_request_timer_limit <= (OPT_REQ_INTERVAL<<5);
            8'h9 : 
                cal_optimal_request_timer_limit <= (OPT_REQ_INTERVAL<<6);
            8'h8 : 
                cal_optimal_request_timer_limit <= (OPT_REQ_INTERVAL<<7);
            8'h7 : 
                cal_optimal_request_timer_limit <= (OPT_REQ_INTERVAL<<8);
            8'h6 :
                cal_optimal_request_timer_limit <= (OPT_REQ_INTERVAL<<9);
            8'h5 : 
                cal_optimal_request_timer_limit <= (OPT_REQ_INTERVAL<<10);
            8'h4 : 
                cal_optimal_request_timer_limit <= (OPT_REQ_INTERVAL<<11);
            8'h3 : 
                cal_optimal_request_timer_limit <= (OPT_REQ_INTERVAL<<12);
            8'h2 : 
                cal_optimal_request_timer_limit <= (OPT_REQ_INTERVAL<<13);
            8'h1 : 
                cal_optimal_request_timer_limit <= (OPT_REQ_INTERVAL<<14);
            8'h0 : 
                cal_optimal_request_timer_limit <= (OPT_REQ_INTERVAL<<15);
            default :
                cal_optimal_request_timer_limit <= (OPT_REQ_INTERVAL);
        endcase // current_state
    end 


    always_ff @(posedge CLK) begin : allow_cal_optimal_request_timer_processing 
        case (current_state)
            CAL_TX_OFS_ZEROS_ST : 
                if (!out_awfull) begin 
                    if (word_counter == 4'h4) begin 
                        allow_cal_optimal_request_timer <= 1'b1;
                    end else begin 
                        allow_cal_optimal_request_timer <= 1'b0;
                    end 
                end else begin 
                    allow_cal_optimal_request_timer <= 1'b0;
                end 

            default : 
                if (has_calibration_count_exceeded) begin 
                    allow_cal_optimal_request_timer <= 1'b0;
                end else begin 
                    allow_cal_optimal_request_timer <= allow_cal_optimal_request_timer;
                end 
        endcase // current_state
    end 



    always_ff @(posedge CLK) begin : cal_optimal_request_timer_processing 
        if (RESET | !allow_cal_optimal_request_timer) begin 
            cal_optimal_request_timer <= cal_optimal_request_timer_limit;
        end else begin 
            case (current_state)
                REQ_RX_READ_DATA_ST : 
                    if (S_AXIS_TVALID & S_AXIS_TREADY & S_AXIS_TLAST) begin 
                        cal_optimal_request_timer <= cal_optimal_request_timer_limit;
                    end else begin 
                        cal_optimal_request_timer <= cal_optimal_request_timer;
                    end 

                default: 
                    if (cal_optimal_request_timer == 0) begin 
                        cal_optimal_request_timer <= cal_optimal_request_timer;
                    end else begin 
                        cal_optimal_request_timer <= cal_optimal_request_timer - 1;
                    end 

            endcase // word_counter
        end 
    end 



    always_ff @(posedge CLK) begin : has_cal_optimal_request_timer_exceeded_processing 
        if (RESET | !allow_cal_optimal_request_timer) begin 
            has_cal_optimal_request_timer_exceeded <= 1'b0;
        end else begin 
            if (cal_optimal_request_timer == 0) begin 
                has_cal_optimal_request_timer_exceeded <= 1'b1;
            end else begin 
                has_cal_optimal_request_timer_exceeded <= 1'b0;
            end 
        end 
    end 


    always_ff @(posedge CLK) begin : calibration_count_processing 
        if (RESET | calibration_flaq) begin 
            calibration_count <= calibration_count_limit;
        end else begin 
            case (current_state)
                IDLE_CHK_REQ_ST : 
                    if (request_flaq) begin 
                        if (interrupt) begin 
                            calibration_count <= calibration_count;
                        end else begin 
                            if (has_cal_optimal_request_timer_exceeded) begin 
                                calibration_count <= calibration_count - 1;
                            end else begin 
                                calibration_count <= calibration_count;
                            end 
                        end
                    end else begin 
                        calibration_count <= calibration_count;
                    end 

                default : 
                    calibration_count <= calibration_count;

            endcase // current_state
        end 
    end 



    always_ff @(posedge CLK) begin : has_calibration_count_exceeded_processing 

        if (RESET | calibration_flaq) begin 
            has_calibration_count_exceeded <= 1'b0;
        end else begin 
            if (calibration_count == 0) begin 
                has_calibration_count_exceeded <= 1'b1;
            end else begin 
                has_calibration_count_exceeded <= 1'b0;
            end 
        end 

    end 

endmodule