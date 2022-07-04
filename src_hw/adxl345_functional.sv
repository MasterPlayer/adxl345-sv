`timescale 1ns / 1ps


module adxl345_functional (
    input  logic        CLK                    ,
    input  logic        RESET                  ,
    // signal from AXI_DEV interface
    input  logic [31:0] WDATA                  ,
    input  logic [ 3:0] WSTRB                  ,
    input  logic [ 3:0] WADDR                  ,
    input  logic        WVALID                 ,
    output logic [31:0] RDATA                  ,
    // control
    input  logic [ 6:0] I2C_ADDRESS            ,
    input  logic        SINGLE_REQUEST         ,
    output logic        SINGLE_REQUEST_COMPLETE,
    // data to device
    output logic [ 7:0] M_AXIS_TDATA           ,
    output logic [ 0:0] M_AXIS_TKEEP           ,
    output logic [ 7:0] M_AXIS_TUSER           ,
    output logic        M_AXIS_TVALID          ,
    output logic        M_AXIS_TLAST           ,
    input  logic        M_AXIS_TREADY          ,
    // data from device
    input  logic [ 7:0] S_AXIS_TDATA           ,
    input  logic [ 0:0] S_AXIS_TKEEP           ,
    input  logic [ 7:0] S_AXIS_TUSER           ,
    input  logic        S_AXIS_TVALID          ,
    input  logic        S_AXIS_TLAST           ,
    output logic        S_AXIS_TREADY
);

    localparam [7:0] ADDRESS_LIMIT = 8'h3A;

    logic [ 7:0] doutb               ;
    logic [ 5:0] addrb = '{default:0};
    logic [ 7:0] dinb                ;
    logic        web   = 'b0         ;
    logic [ 3:0] wea                 ;
    logic [31:0] dina                ;

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
        REQ_TX_ADDR_PTR_ST  , // send address pointer 
        REQ_TX_READ_DATA_ST , // send read request for reading 0x39 data bytes 
        REQ_RX_READ_DATA_ST , // await data from start to tlast signal 
        WAIT_ST              
    } fsm;

    fsm current_state = IDLE_ST;


    logic request_flaq = 'b0;

    logic [7:0] out_din_data = '{default:0};
    logic [0:0] out_din_keep = '{default:0};
    logic [7:0] out_din_user               ;
    logic       out_din_last = 1'b0        ;
    logic       out_wren     = 1'b0        ;
    logic       out_full                   ;
    logic       out_awfull                 ;

    logic [3:0] word_counter = '{default:0};
    logic [7:0] address_ptr  = '{default:0};


    generate
    
        for (genvar index = 0; index < 4; index++) begin
            
            always_ff @(posedge CLK) begin
                if (WVALID & WSTRB[index]) begin
                    need_update_reg[WADDR][index] <= write_mask_register[WADDR][index];
                end else begin
                    need_update_reg[WADDR][index] <= need_update_reg[WADDR][index];
                end
            end 

        end 

    endgenerate

    always_ff @(posedge CLK) begin 
        wea <= {4{WVALID}} & write_mask_register[WADDR] & WSTRB;
    end 

    always_ff @(posedge CLK) begin 
        if (WVALID) begin 
            dina <= WDATA;
        end else begin 
            dina <= dina;
        end     
    end 

    always_ff @(posedge CLK) begin 
        if (|wea) begin 
            need_update_flaq <= 1'b1;
        end else begin 
            // to do : deassert according fsm
            need_update_flaq <= need_update_flaq;
        end 
    end 

    // if needed request, AND for this register
    always_ff @(posedge CLK) begin 
        request_flaq <= SINGLE_REQUEST;
    end 

    xpm_memory_tdpram #(
        .ADDR_WIDTH_A           (4              ), // DECIMAL
        .ADDR_WIDTH_B           (6              ), // DECIMAL
        .AUTO_SLEEP_TIME        (0              ), // DECIMAL
        .BYTE_WRITE_WIDTH_A     (8              ), // DECIMAL
        .BYTE_WRITE_WIDTH_B     (8              ), // DECIMAL
        .CASCADE_HEIGHT         (0              ), // DECIMAL
        .CLOCKING_MODE          ("common_clock" ), // String
        .ECC_MODE               ("no_ecc"       ), // String
        .MEMORY_INIT_FILE       ("none"         ), // String
        .MEMORY_INIT_PARAM      ("0"            ), // String
        .MEMORY_OPTIMIZATION    ("true"         ), // String
        .MEMORY_PRIMITIVE       ("auto"         ), // String
        .MEMORY_SIZE            (512            ), // DECIMAL
        .MESSAGE_CONTROL        (0              ), // DECIMAL
        .READ_DATA_WIDTH_A      (32             ), // DECIMAL
        .READ_DATA_WIDTH_B      (8              ), // DECIMAL
        .READ_LATENCY_A         (2              ), // DECIMAL
        .READ_LATENCY_B         (2              ), // DECIMAL
        .READ_RESET_VALUE_A     ("0"            ), // String
        .READ_RESET_VALUE_B     ("0"            ), // String
        .RST_MODE_A             ("SYNC"         ), // String
        .RST_MODE_B             ("SYNC"         ), // String
        .SIM_ASSERT_CHK         (0              ), // DECIMAL; 0=disable simulation messages, 1=enable simulation messages
        .USE_EMBEDDED_CONSTRAINT(0              ), // DECIMAL
        .USE_MEM_INIT           (1              ), // DECIMAL
        .USE_MEM_INIT_MMI       (0              ), // DECIMAL
        .WAKEUP_TIME            ("disable_sleep"), // String
        .WRITE_DATA_WIDTH_A     (32             ), // DECIMAL
        .WRITE_DATA_WIDTH_B     (8              ), // DECIMAL
        .WRITE_MODE_A           ("no_change"    ), // String
        .WRITE_MODE_B           ("no_change"    ), // String
        .WRITE_PROTECT          (1              )  // DECIMAL
    ) xpm_memory_tdpram_inst (
        .dbiterra      (     ), // 1-bit output: Status signal to indicate double bit error occurrence
        .dbiterrb      (     ), // 1-bit output: Status signal to indicate double bit error occurrence
        .douta         (RDATA), // READ_DATA_WIDTH_A-bit output: Data output for port A read operations.
        .doutb         (doutb), // READ_DATA_WIDTH_B-bit output: Data output for port B read operations.
        .sbiterra      (     ), // 1-bit output: Status signal to indicate single bit error occurrence
        .sbiterrb      (     ), // 1-bit output: Status signal to indicate single bit error occurrence
        .addra         (WADDR), // ADDR_WIDTH_A-bit input: Address for port A write and read operations.
        .addrb         (addrb), // ADDR_WIDTH_B-bit input: Address for port B write and read operations.
        .clka          (CLK  ), // 1-bit input: Clock signal for port A. Also clocks port B when
        .clkb          (CLK  ), // 1-bit input: Clock signal for port B when parameter CLOCKING_MODE is
        .dina          (dina ), // WRITE_DATA_WIDTH_A-bit input: Data input for port A write operations.
        .dinb          (dinb ), // WRITE_DATA_WIDTH_B-bit input: Data input for port B write operations.
        .ena           ('b1  ), // 1-bit input: Memory enable signal for port A. Must be high on clock
        .enb           ('b1  ), // 1-bit input: Memory enable signal for port B. Must be high on clock
        .injectdbiterra('b0  ), // 1-bit input: Controls double bit error injection on input data when
        .injectdbiterrb('b0  ), // 1-bit input: Controls double bit error injection on input data when
        .injectsbiterra('b0  ), // 1-bit input: Controls single bit error injection on input data when
        .injectsbiterrb('b0  ), // 1-bit input: Controls single bit error injection on input data when
        .regcea        ('b1  ), // 1-bit input: Clock Enable for the last register stage on the output
        .regceb        ('b1  ), // 1-bit input: Clock Enable for the last register stage on the output
        .rsta          (RESET), // 1-bit input: Reset signal for the final port A output register stage.
        .rstb          (RESET), // 1-bit input: Reset signal for the final port B output register stage.
        .sleep         ('b0  ), // 1-bit input: sleep signal to enable the dynamic power saving feature.
        .wea           (wea  ), // WRITE_DATA_WIDTH_A/BYTE_WRITE_WIDTH_A-bit input: Write enable vector
        .web           (web  )  // WRITE_DATA_WIDTH_B/BYTE_WRITE_WIDTH_B-bit input: Write enable vector
    );


    // address : sets before receive data, implies on MEM
    always_ff @(posedge CLK) begin 
        case (current_state)
            REQ_TX_READ_DATA_ST : 
                addrb <= address_ptr;

            REQ_RX_READ_DATA_ST : 
                if (web) begin 
                    addrb <= addrb + 1;
                end else begin 
                    addrb <= addrb;
                end 

            default : 
                addrb <= addrb;

        endcase
    end 


    // readed data from interface S_AXIS_ to portb
    always_ff @(posedge CLK) begin 
        case (current_state)
            REQ_RX_READ_DATA_ST : 
                web <= S_AXIS_TVALID;

            default : 
                web <= 1'b0;
        endcase // current_state
    end 

    
    
    always_ff @(posedge CLK) begin 
        case (current_state)
            REQ_RX_READ_DATA_ST : 
                dinb <= S_AXIS_TDATA;

            default 
                dinb <= dinb;
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
                    if (request_flaq) begin 
                        current_state <= REQ_TX_ADDR_PTR_ST;
                    end else begin 
                        current_state <= current_state;
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

            default : 
                word_counter <= '{default:0};
        endcase // current_state
    end 


    always_ff @(posedge CLK) begin 
        case (current_state) 
            REQ_TX_READ_DATA_ST : 
                if (!out_awfull) begin 
                    SINGLE_REQUEST_COMPLETE <= 1'b1;
                end else begin 
                    SINGLE_REQUEST_COMPLETE <= SINGLE_REQUEST_COMPLETE;
                end 

            default : 
                SINGLE_REQUEST_COMPLETE <= 1'b0;

        endcase // current_state
    end 

endmodule