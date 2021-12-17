#pragma once

#include <stdint.h>
#include <stdio.h>
#include <xil_types.h>

typedef struct {
    uint32_t ctl_reg;
    uint32_t request_interval_reg;
    uint32_t data_width_reg;
    uint32_t read_valid_count_reg;
    uint32_t write_valid_count_reg;
    uint32_t write_transactions_reg;
    uint32_t read_transactions_reg;
    uint32_t clk_period_reg;
} adxl_cfg;


#define CTL_RESET_MASK          0x00000001
#define CTL_ENABLE_MASK         0x00000002
#define CTL_ALLOW_IRQ_MASK      0x00000004
#define CTL_ON_WORK_MASK        0x00000080
#define CTL_IIC_ADDR_MASK       0x00007F00
#define CTL_LINK_ON_MASK        0x00008000
#define CTL_VERS_MINOR_MASK     0x00FF0000
#define CTL_VERS_MAJOR_MASK     0xFF000000

#define adxl_cfg_has_reset(ptr) ((ptr)->ctl_reg & CTL_RESET_MASK) ? TRUE : FALSE
#define adxl_cfg_reset(ptr) ((ptr)->ctl_reg |= CTL_RESET_MASK)

#define adxl_cfg_has_enable(ptr) ((ptr)->ctl_reg & CTL_ENABLE_MASK) ? TRUE : FALSE
#define adxl_cfg_enable(ptr) ((ptr)-> ctl_reg |= CTL_ENABLE_MASK)
#define adxl_cfg_disable(ptr) ((ptr)->ctl_reg &= ~CTL_ENABLE_MASK)

#define adxl_cfg_has_allow_irq(ptr) ((ptr)->ctl_reg & CTL_ALLOW_IRQ_MASK) ? TRUE : FALSE
#define adxl_cfg_allow_irq(ptr) ((ptr)->ctl_reg |= CTL_ALLOW_IRQ_MASK)
#define adxl_cfg_unallow_irq(ptr) ((ptr)->ctl_reg &= ~CTL_ALLOW_IRQ_MASK)

#define adxl_cfg_has_on_work(ptr) ((ptr)->ctl_reg & CTL_ON_WORK_MASK) ? TRUE : FALSE

#define adxl_cfg_get_i2c_addr(ptr) (((ptr)->ctl_reg & CTL_IIC_ADDR_MASK) >> 8)
#define adxl_cfg_set_i2c_addr(ptr, addr) ((ptr)->ctl_reg = (((ptr)->ctl_reg & ~CTL_IIC_ADDR_MASK) + ((uint32_t)addr<<8)))

#define adxl_cfg_has_link(ptr) ((ptr)->ctl_reg & CTL_LINK_ON_MASK) ? TRUE : FALSE

#define adxl_cfg_get_version_minor(ptr) (((ptr)->ctl_reg & CTL_VERS_MINOR_MASK) >> 16)
#define adxl_cfg_get_version_major(ptr) (((ptr)->ctl_reg & CTL_VERS_MAJOR_MASK) >> 24)

#define adxl_cfg_get_request_interval(ptr) ((ptr)->request_interval_reg)
#define adxl_cfg_set_request_interval(ptr, value) ((ptr)->request_interval_reg = value)

#define adxl_cfg_get_data_width(ptr) ((ptr)->data_width_reg)
#define adxl_cfg_get_read_valid_count(ptr) ((ptr)->read_valid_count_reg)
#define adxl_cfg_get_write_valid_count(ptr) ((ptr)->write_valid_count_reg)
#define adxl_cfg_get_write_transactions(ptr) ((ptr)->write_transactions_reg)
#define adxl_cfg_get_read_transactions(ptr) ((ptr)->read_transactions_reg)
#define adxl_cfg_get_clk_period(ptr) ((ptr)->clk_period_reg)

void adxl_cfg_debug(adxl_cfg *cfg_ptr);
