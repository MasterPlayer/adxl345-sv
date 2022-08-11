`timescale 1ns / 1ps



module tb_device_imitation (
    input  logic IIC_SCL_I,
    input  logic IIC_SDA_I,
    output logic IIC_SCL_O,
    output logic IIC_SDA_O,
    output logic IRQ
);


    logic clk   = 1'b0;
    logic reset = 1'b1;

    always #5 clk = ~clk; 

    initial begin 
        reset = 1'b1;
        #100 
        reset = 1'b0;
    end 

    localparam integer INTERRUPT_INTERVAL = 300000;

    logic [0:59][0:7] register_file = '{
        8'hE5, 8'h01, 8'h02, 8'h03, // 0x00
        8'h04, 8'h05, 8'h06, 8'h00, // 0x04
        8'h00, 8'h00, 8'h00, 8'h00, // 0x08
        8'h00, 8'h00, 8'h00, 8'h00, // 0x0C
        8'h00, 8'h00, 8'h00, 8'h00, // 0x10
        8'h00, 8'h00, 8'h00, 8'h00, // 0x14
        8'h00, 8'h00, 8'h00, 8'h00, // 0x18
        8'h00, 8'h00, 8'h1E, 8'h1F, // 0x1C
        8'h20, 8'h00, 8'h00, 8'h00, // 0x20
        8'h00, 8'h00, 8'h00, 8'h00, // 0x24
        8'h00, 8'h00, 8'h00, 8'h00, // 0x28
        8'h0F, 8'h00, 8'h00, 8'h00, // 0x2C
        8'h80, 8'h31, 8'h32, 8'h33, // 0x30
        8'h34, 8'h35, 8'h36, 8'h37, // 0x34
        8'h00, 8'h00, 8'h00, 8'h00  // 0x38
    };


    logic d_iic_scl = 1'b1;
    logic scl = 1'b0;

    logic write_operation = 1'b0;
    logic read_operation = 1'b0;

    logic [7:0] interrupt_register;

    logic has_readed_interrupt_register = 1'b0;


    typedef enum {
        AWAIT_CMD,
        READ_OP ,
        READ_ACK,
        WRITE_OP 
    } fsm;

    fsm current_state = AWAIT_CMD;

    logic [7:0] shift_register        = '{default : 0};
    logic [3:0] bit_counter           = '{default:0}  ;
    logic       valid                 = 1'b0          ;
    logic       d_sda                                 ;
    logic       finalize_transmission = 1'b0          ;

    logic [7:0] device_address   = '{default:0};
    logic [7:0] ptr              = '{default:0};
    logic [7:0] data_for_writing = '{default:0};

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
                    case (current_state) 
                        WRITE_OP : 
                            if (valid_event) begin 
                                ptr <= ptr + 1;
                            end else begin 
                                ptr <= ptr;
                            end 

                        default : 
                            ptr <= ptr;

                    endcase
                end 
            end else begin 
                case (current_state) 
                    READ_OP : 
                        if (valid_event) begin 
                            ptr <= ptr + 1;
                        end else begin 
                            ptr <= ptr;
                        end 

                    // WRITE_OP : 
                    //     if (valid_event) begin 
                    //         ptr <= ptr + 1;
                    //     end else begin 
                    //         ptr <= ptr;
                    //     end 

                    default : 
                        ptr <= ptr;
                endcase // current_state
            end 
        end else begin             
            ptr <= ptr;
        end 
    end 



    always_ff @(posedge clk) begin : data_for_writing_processing 
        if (valid_event) begin 
            if (write_operation) begin 
                if (register_counter == 2) begin 
                    data_for_writing <= shift_register;
                end else begin 
                    data_for_writing <= data_for_writing;
                end 
            end else begin 
                data_for_writing <= data_for_writing;
            end 
        end else begin 
            data_for_writing <= data_for_writing;
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
                        if (write_operation & register_counter == 1) begin 
                            current_state <= WRITE_OP;
                        end else begin 
                            current_state <= current_state;
                        end 
                    end 

                READ_ACK : 
                    if (scl & (bit_counter == 0)) begin 
                        current_state <= READ_OP;
                    end else begin  
                        current_state <= current_state;
                    end 

                READ_OP : 
                    current_state <= current_state;

                WRITE_OP : 
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



    always_ff @(posedge clk) begin 
        case (current_state)
            WRITE_OP : 
                if (register_counter > 1) begin 
                    if (valid_event) begin 
                        register_file[ptr] <= shift_register;
                    end else begin 
                        register_file[ptr] <= register_file[ptr];
                    end  
                end else begin 
                    register_file[ptr] <= register_file[ptr];
                end 
            default : 
                register_file[ptr] <= register_file[ptr];

        endcase
    end 



    always_comb begin 
        IIC_SCL_O = d_iic_scl;
    end 

    always_comb begin 
        interrupt_register = register_file[46];
    end 

    always_ff @(posedge clk) begin 
        case (current_state)
            READ_OP : 
                if (ptr == 8'h30) begin  
                    if (valid_event) begin 
                        has_readed_interrupt_register <= 1'b1;
                    end else begin 
                        has_readed_interrupt_register <= 1'b0;
                    end 
                end else begin 
                    has_readed_interrupt_register <= 1'b0;
                end 

            default: 
                has_readed_interrupt_register <= 1'b0;

        endcase // current_state
    end 

    logic [31:0] interrupt_timer = '{default:0};

    always_ff @(posedge clk) begin 
        if (interrupt_register == 0) begin 
            interrupt_timer <= INTERRUPT_INTERVAL;
        end else begin 
            if (interrupt_timer == 0) begin 
                if (has_readed_interrupt_register) begin 
                    interrupt_timer <= INTERRUPT_INTERVAL;
                end else begin 
                    interrupt_timer <= interrupt_timer;
                end 
            end else begin 
                interrupt_timer <= interrupt_timer - 1;
            end 
        end 
    end 

    always_comb begin 
        if (interrupt_timer == 0) begin 
            IRQ <= 1'b1;
        end else begin 
            IRQ <= 1'b0; 
        end 
    end 

endmodule
