`timescale 1ns / 1ps



module tb_device_imitation (
    input  IIC_SCL_I,
    input  IIC_SDA_I,
    output IIC_SCL_O,
    output IIC_SDA_O,
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

    logic d_iic_scl = 1'b1;
    logic scl = 1'b0;

    typedef enum {
        AWAIT_CMD 


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
            if (register_counter == 1) begin 
                ptr <= shift_register;
            end else begin 
                ptr <= ptr;
            end 
        end else begin 
            ptr <= ptr;
        end 
    end 


endmodule
