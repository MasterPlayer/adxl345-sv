`timescale 1ns / 1ps



module tb_device_imitation (
    input  logic IIC_SCL_I,
    input  logic IIC_SDA_I,
    output logic IIC_SCL_O,
    output logic IIC_SDA_O,
    output IRQ
);


    logic clk   = 1'b0;
    logic reset = 1'b1;

    always #5 clk = ~clk; 

    initial begin 
        reset = 1'b1;
        #100 
        reset = 1'b0;
    end 


    logic [0:59][0:7] register_file = '{
        8'hE5, 8'h01, 8'h02, 8'h03, 
        8'h04, 8'h05, 8'h06, 8'h07, 
        8'h08, 8'h09, 8'h0A, 8'h0B, 
        8'h0C, 8'h0D, 8'h0E, 8'h0F, 
        8'h10, 8'h11, 8'h12, 8'h13, 
        8'h14, 8'h15, 8'h16, 8'h17, 
        8'h18, 8'h19, 8'h1A, 8'h1B, 
        8'h1C, 8'h1D, 8'h1E, 8'h1F, 
        8'h20, 8'h21, 8'h22, 8'h23, 
        8'h24, 8'h25, 8'h26, 8'h27, 
        8'h28, 8'h29, 8'h2A, 8'h2B, 
        8'h2C, 8'h2D, 8'h2E, 8'h2F, 
        8'h30, 8'h31, 8'h32, 8'h33, 
        8'h34, 8'h35, 8'h36, 8'h37, 
        8'h38, 8'h39, 8'h3A, 8'h3b 
    };

    logic d_iic_scl = 1'b1;
    logic scl = 1'b0;

    logic write_operation = 1'b0;
    logic read_operation = 1'b0;

    typedef enum {
        AWAIT_CMD,
        READ_OP ,
        READ_ACK


    } fsm;

    fsm current_state = AWAIT_CMD;

    logic [7:0] shift_register        = '{default : 0};
    logic [3:0] bit_counter           = '{default:0}  ;
    logic       valid                 = 1'b0          ;
    logic       d_sda                                 ;
    logic       finalize_transmission = 1'b0          ;

    logic [7:0] device_address = '{default:0};
    logic [7:0] ptr            = '{default:0};

    logic [7:0] register_counter = '{default:0};

    always_ff @(posedge clk) begin : d_iic_scl_processing 
        d_iic_scl <= IIC_SCL_I;
    end 

    always_comb begin 
        scl = IIC_SCL_I & !d_iic_scl;
    end 

    always_ff @(posedge clk) begin 
        if (scl) begin 
            shift_register <= {shift_register[6:0], IIC_SDA_I};
        end else begin 
            shift_register <= shift_register;
        end 
    end  


    always_ff @(posedge clk) begin 
        if (reset | finalize_transmission) begin 
            bit_counter <= '{default:0};
        end else begin 
            if (scl) begin 
                if (bit_counter == 8) begin 
                    bit_counter <= '{default:0};
                end else begin 
                    bit_counter <= bit_counter + 1;
                end 
            end else begin 
                bit_counter <= bit_counter;
            end 
        end 
    end 


    always_comb begin 
        valid = bit_counter[3];
    end 

    logic d_valid = 1'b0;

    always_ff @(posedge clk) begin 
        d_valid <= valid;
    end 

    logic valid_event = 1'b0;

    always_ff @(posedge clk) begin 
        valid_event <= !d_valid & valid;
    end 


    always_ff @(posedge clk) begin 
        d_sda <= IIC_SDA_I;
    end 


    always_ff @(posedge clk) begin 
        if (IIC_SCL_I) begin 
            if (d_sda != IIC_SDA_I) begin 
                finalize_transmission <= 1'b1;
            end else begin 
                finalize_transmission <= 1'b0;
            end 
        end else begin 
            finalize_transmission <= 1'b0;
        end 
    end 


    always_ff @(posedge clk) begin 
        if (reset | finalize_transmission) begin 
            register_counter <= '{default:0};
        end else begin 
            if (valid_event) begin 
                register_counter <= register_counter + 1;
            end else begin 
                register_counter <= register_counter;
            end 
        end 
    end 

    always_ff @(posedge clk) begin 
        if (valid_event) begin 
            if (register_counter == 0) begin 
                device_address <= shift_register;
            end else begin 
                device_address <= device_address;
            end 
        end else begin 
            device_address <= device_address;
        end 
    end     

    always_ff @(posedge clk) begin 
        if (valid_event) begin 
            if (write_operation) begin 
                if (register_counter == 1) begin 
                    ptr <= shift_register;
                end else begin 
                    ptr <= ptr;
                end 
            end else begin 
                case (current_state) 
                    READ_OP : 
                        if (valid_event) begin 
                            ptr <= ptr + 1;
                        end else begin 
                            ptr <= ptr;
                        end 

                    default : 
                        ptr <= ptr;
                endcase // current_state
            end 
        end else begin             
            ptr <= ptr;
        end 
    end 

    always_ff @(posedge clk) begin 
        if (valid_event) begin 
            if (register_counter == 0) begin 
                write_operation <= ~shift_register[0];
            end else begin 
                write_operation <= write_operation;
            end 
        end else begin 
            write_operation <= write_operation;
        end 
    end 

    always_ff @(posedge clk) begin 
        if (valid_event) begin 
            if (register_counter == 0) begin 
                read_operation <= shift_register[0];
            end else begin 
                read_operation <= read_operation;
            end 
        end else begin 
            read_operation <= read_operation;
        end 
    end 

    always_ff @(posedge clk) begin 
        if (reset | finalize_transmission) begin 
            current_state <= AWAIT_CMD;
        end else begin 

            case (current_state)
                AWAIT_CMD : 
                    if (read_operation & register_counter == 1) begin 
                        current_state <= READ_ACK;
                    end else begin 
                        current_state <= current_state;
                    end 

                READ_ACK : 
                    if (scl & (bit_counter == 0)) begin 
                        current_state <= READ_OP;
                    end else begin  
                        current_state <= current_state;
                    end 

                READ_OP : 
                    current_state <= current_state;

                default : 
                    current_state <= current_state;
            endcase // current_state

        end 
    end 


    always_comb begin 
        case (current_state) 
            READ_OP : 
                if (bit_counter != 0) begin 
                    IIC_SDA_O = register_file[ptr][(bit_counter-1)];
                end else begin 
                    IIC_SDA_O = 1'b0;
                end 

            default : 
                IIC_SDA_O = 1'b0;

        endcase // current_state
    end 



    always_comb begin 
        IIC_SCL_O = d_iic_scl;
    end 


endmodule
