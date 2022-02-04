#pragma once

#include <stdint.h>
#include <stdio.h>
#include <xil_types.h>


typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} adxl_axis;



typedef struct {
    uint32_t ctl_reg;
    uint32_t request_interval_reg;
    uint32_t data_width_reg;
    uint32_t read_valid_count_reg;
    uint32_t write_valid_count_reg;
    uint32_t write_transactions_reg;
    uint32_t read_transactions_reg;
    uint32_t clk_period_reg;
    uint32_t rom_address_reg;
    uint32_t opt_req_interval_reg;
    uint32_t calibration_count_limit_reg;
    uint32_t calibration_time_reg;
    uint32_t reserved_3;
    uint32_t reserved_4;
    uint32_t reserved_5;
    uint32_t reserved_6;
    adxl_axis axis[32];
} adxl_cfg;


#define CTL_RESET_LOGIC_MASK                0x00000001
#define CTL_REQUEST_ENABLE_MASK             0x00000002
#define CTL_IRQ_ALLOW_MASK                  0x00000004
#define CTL_SINGLE_REQUEST_MASK             0x00000008
#define CTL_INTR_ACK_MASK                   0x00000010
#define CTL_CALIBRATION_MASK                0x00000020
#define CTL_SINGLE_REQUEST_PERFORMED_MASK   0x00000040
#define CTL_ON_WORK_MASK                    0x00000080
#define CTL_IIC_ADDRESS_MASK                0x00007F00
#define CTL_LINK_ON_MASK                    0x00008000
#define CTL_VERS_MINOR_MASK                 0x00FF0000
#define CTL_VERS_MAJOR_MASK                 0xFF000000

#define ROM_ADDRESS_MASK                    0x000000FF
#define ROM_ADDRESS_OVERRUN_MASK            0x00000100


/**************************************** CTL REG ******************************************/
/*RESET LOGIC BIT*/
// return 1 if reset in process 0 if reset is complete/not performed
#define adxl_cfg_ctl_has_reset(ptr) ((ptr)->ctl_reg & CTL_RESET_LOGIC_MASK) ? TRUE : FALSE
// perform write to reset_logic register
#define adxl_cfg_ctl_reset(ptr) ((ptr)->ctl_reg |= CTL_RESET_LOGIC_MASK)

/*REQUEST PERFORM BIT*/
// perfrom requesting of data
#define adxl_cfg_ctl_request_enable(ptr) ((ptr)->ctl_reg |= CTL_REQUEST_ENABLE_MASK)
// perform disable requesting of data
#define adxl_cfg_ctl_request_disable(ptr) ((ptr)->ctl_reg &= ~CTL_REQUEST_ENABLE_MASK)
// return 1 if requesting enabled
#define adxl_cfg_ctl_has_request_enabled(ptr) ((ptr)->ctl_reg & CTL_REQUEST_ENABLE_MASK) ? TRUE : FALSE
/* IRQ ALLOW BIT */
#define adxl_cfg_ctl_irq_allow(ptr) ((ptr)->ctl_reg |= CTL_IRQ_ALLOW_MASK)
#define adxl_cfg_ctl_irq_unallow(ptr) ((ptr)->ctl_reg &= ~CTL_IRQ_ALLOW_MASK)
#define adxl_cfg_ctl_has_irq_allowed(ptr) ((ptr)->ctl_reg & CTL_IRQ_ALLOW_MASK) ? TRUE : FALSE
/* SINGLE REQUEST bit */
#define adxl_cfg_ctl_single_request(ptr) ((ptr)->ctl_reg |= CTL_SINGLE_REQUEST_MASK)
/* INTR ACK bit */
#define adxl_cfg_ctl_intr_ack(ptr) ((ptr)->ctl_reg |= CTL_INTR_ACK_MASK)
/* CALIBRATION bit */
#define adxl_cfg_ctl_set_calibration(ptr) ((ptr)->ctl_reg |= CTL_CALIBRATION_MASK)
#define adxl_cfg_ctl_get_calibration(ptr) ((ptr)-> ctl_reg & CTL_CALIBRATION_MASK)
/* SINGLE REQUEST PERFORMED bit */
#define adxl_cfg_ctl_has_single_request_performed(ptr) ((ptr)->ctl_reg & CTL_SINGLE_REQUEST_PERFORMED_MASK) ? TRUE : FALSE
#define adxl_cfg_ctl_clear_single_request_flaq(ptr) ((ptr)->ctl_reg |= CTL_SINGLE_REQUEST_PERFORMED_MASK)
/* ON WORK flaq */
#define adxl_cfg_ctl_has_on_work(ptr) ((ptr)->ctl_reg & CTL_ON_WORK_MASK) ? TRUE : FALSE
/* iic address bits */
#define adxl_cfg_ctl_get_iic_address(ptr) (((ptr)->ctl_reg & CTL_IIC_ADDRESS_MASK) >> 8)
#define adxl_cfg_ctl_set_iic_address(ptr, addr) ((ptr)->ctl_reg = (((ptr)->ctl_reg & ~CTL_IIC_ADDRESS_MASK) + ((uint32_t)addr<<8)))
/* linkon bits */
#define adxl_cfg_ctl_has_link(ptr) ((ptr)->ctl_reg & CTL_LINK_ON_MASK) ? TRUE : FALSE

#define adxl_cfg_ctl_get_version_minor(ptr) (((ptr)->ctl_reg & CTL_VERS_MINOR_MASK) >> 16)
#define adxl_cfg_ctl_get_version_major(ptr) (((ptr)->ctl_reg & CTL_VERS_MAJOR_MASK) >> 24)

#define adxl_cfg_get_request_interval(ptr) ((ptr)->request_interval_reg)
#define adxl_cfg_set_request_interval(ptr, value) ((ptr)->request_interval_reg = value)

#define adxl_cfg_get_data_width(ptr) ((ptr)->data_width_reg)
#define adxl_cfg_get_read_valid_count(ptr) ((ptr)->read_valid_count_reg)
#define adxl_cfg_get_write_valid_count(ptr) ((ptr)->write_valid_count_reg)
#define adxl_cfg_get_write_transactions(ptr) ((ptr)->write_transactions_reg)
#define adxl_cfg_get_read_transactions(ptr) ((ptr)->read_transactions_reg)
#define adxl_cfg_get_clk_period(ptr) ((ptr)->clk_period_reg)

#define adxl_cfg_get_rom_address(ptr) ((ptr)->rom_address_reg & ROM_ADDRESS_MASK)
#define adxl_cfg_get_rom_address_overrun(ptr) ((ptr)->rom_address_reg & ROM_ADDRESS_OVERRUN_MASK) ? TRUE : FALSE

#define adxl_cfg_get_opt_request_interval(ptr) ((ptr)->opt_req_interval_reg)

#define adxl_cfg_set_calibration_count_limit(ptr, value) ((ptr)->calibration_count_limit_reg = value)
#define adxl_cfg_get_calibration_count_limit(ptr) ((ptr)->calibration_count_limit_reg)

#define adxl_cfg_get_calibration_time(ptr) ((ptr)->calibration_time_reg)
