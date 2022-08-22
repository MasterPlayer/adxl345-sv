#pragma once

#include <stdint.h>
#include <stdio.h>
#include <xil_types.h>



typedef struct {
	uint32_t ctl_reg;
	uint32_t request_interval_reg;
	uint32_t calibration_count_reg;
	uint32_t read_valid_count_reg;
	uint32_t write_valid_count_reg;
	uint32_t read_transactions_lsb_reg;
	uint32_t read_transactions_msb_reg;
	uint32_t clk_period_reg ;
	uint32_t opt_request_interval_lsb_reg;
	uint32_t opt_request_interval_msb_reg;
	uint32_t data_width_reg;
	uint32_t calibration_time_lsb_reg;
	uint32_t calibration_time_msb_reg;
	uint32_t write_transactions_lsb_reg;
	uint32_t write_transactions_msb_reg;
	uint32_t reserved_15_reg;
} adxl_cfg;



/*MASKS for REGISTER_0 - ctl_reg*/
#define CFG_CTL_RESET_LOGIC_MASK                  0x00000001
#define CFG_CTL_ENABLE_INTERVAL_REQUESTION_MASK   0x00000002
#define CFG_CTL_IRQ_ALLOW_MASK                    0x00000004
#define CFG_CTL_SINGLE_REQUEST_MASK               0x00000008
#define CFG_CTL_INTR_ACK                          0x00000010
#define CFG_CTL_CALIBRATION_MASK                  0x00000020
#define CFG_CTL_SINGLE_REQUEST_COMPLETE_MASK      0x00000040
#define CFG_CTL_ON_WORK_MASK                      0x00000080
#define CFG_CTL_I2C_ADDRESS_MASK                  0x00007F00
#define CFG_CTL_LINK_ON_MASK                      0x00008000
#define CFG_CTL_VERSION_MINOR_MASK                0x00FF0000
#define CFG_CTL_VERSION_MAJOR_MASK                0xFF000000


/*Masks for REG_2 - Calibration_count_limit*/
#define CFG_CALIBRATION_COUNT_LIMIT_MASK          0x0000001F

/*Mask for REG_9 - OPT_REQUEST_INTERVAL_MSB*/
#define CFG_OPT_REQUEST_INTERVAL_MSB_MASK         0x0000FFFF

/*CTL reg defines*/
#define adxl_cfg_ctl_reset(ptr) ((ptr)->ctl_reg | CFG_CTL_RESET_LOGIC_MASK)
#define adxl_cfg_ctl_reset_completed(ptr) ((ptr)->ctl_reg & CFG_CTL_RESET_LOGIC_MASK) ? TRUE : FALSE
#define adxl_cfg_ctl_interval_requestion_enable(ptr) ((ptr)->ctl_reg | CFG_CTL_ENABLE_INTERVAL_REQUESTION_MASK)
#define adxl_cfg_ctl_interval_requestion_disable(ptr) ((ptr)->ctl_reg & (~CFG_CTL_ENABLE_INTERVAL_REQUESTION_MASK))
#define adxl_cfg_ctl_interval_requestion(ptr) ((ptr)->ctl_reg & CFG_CTL_ENABLE_INTERVAL_REQUESTION_MASK) ? TRUE : FALSE
#define adxl_cfg_ctl_irq_allow(ptr) ((ptr)->ctl_reg | CFG_CTL_IRQ_ALLOW_MASK)
#define adxl_cfg_ctl_irq_unallow(ptr) ((ptr)->ctl_reg & (~CFG_CTL_IRQ_ALLOW_MASK))
#define adxl_cfg_ctl_irq_allowed(ptr) ((ptr)->ctl_reg & CFG_CTL_IRQ_ALLOW_MASK) ? TRUE : FALSE
#define adxl_cfg_ctl_single_request(ptr) ((ptr)->ctl_reg | CFG_CTL_SINGLE_REQUEST_MASK)
#define adxl_cfg_ctl_intr_ack(ptr) ((ptr)->ctl_reg | CFG_CTL_INTR_ACK)
#define adxl_cfg_ctl_calibration(ptr) (ptr)->ctl_reg | CFG_CTL_CALIBRATION_MASK
#define adxl_cfg_ctl_calibration_completed(ptr) ((ptr)->ctl_reg & CFG_CTL_CALIBRATION_MASK) ? TRUE : FALSE
#define adxl_cfg_ctl_single_request_complete(ptr) ((ptr)->ctl_reg & CFG_CTL_SINGLE_REQUEST_COMPLETE_MASK) ? TRUE : FALSE
#define adxl_cfg_ctl_work(ptr) ((ptr)->ctl_reg & CFG_CTL_ON_WORK_MASK) ? TRUE : FALSE
#define adxl_cfg_ctl_set_iic_address(ptr, address) ((ptr)->ctl_reg = (((ptr)->ctl_reg & ~CFG_CTL_I2C_ADDRESS_MASK) + ((uint32_t)address<<8)))
#define adxl_cfg_ctl_get_iic_address(ptr) (((ptr)->ctl_reg & CFG_CTL_I2C_ADDRESS_MASK) >> 8)
#define adxl_cfg_ctl_link(ptr) ((ptr)->ctl_reg & CFG_CTL_LINK_ON_MASK) ? TRUE : FALSE
#define adxl_cfg_ctl_get_version_minor(ptr) (((ptr)->ctl_reg & CFG_CTL_VERSION_MINOR_MASK) >> 16)
#define adxl_cfg_ctl_get_version_major(ptr) (((ptr)->ctl_reg & CFG_CTL_VERSION_MAJOR_MASK) >> 24)

#define adxl_cfg_set_request_interval(ptr, interval) ((ptr)->request_interval_reg = interval)
#define adxl_cfg_get_request_interval(ptr) (ptr->request_interval_reg)

#define adxl_cfg_set_calibration_count_limit(ptr, limit) ((ptr)->calibration_count_reg = (limit & CFG_CALIBRATION_COUNT_LIMIT_MASK))
#define adxl_cfg_get_calibration_count_limit(ptr) ((ptr)->calibration_count_reg & CFG_CALIBRATION_COUNT_LIMIT_MASK)

#define adxl_cfg_ctl_get_read_valid_count(ptr) ((ptr)->read_valid_count_reg)

#define adxl_cfg_ctl_get_write_valid_count(ptr) ((ptr)->write_valid_count_reg)

#define adxl_cfg_ctl_get_read_transactions_lsb(ptr) ((ptr)->read_transactions_lsb_reg)
#define adxl_cfg_ctl_get_read_transactions_msb(ptr) ((ptr)->read_transactions_msb_reg)

#define adxl_cfg_get_clk_period(ptr) ((ptr)->clk_period_reg)

#define adxl_cfg_get_opt_request_interval_lsb_reg(ptr) ((ptr)->opt_request_interval_lsb_reg)
#define adxl_cfg_get_opt_request_interval_msb_reg(ptr) ((ptr)->opt_request_interval_msb_reg & CFG_OPT_REQUEST_INTERVAL_MSB_MASK)
#define adxl_cfg_get_data_width_reg(ptr) ((ptr)->data_width_reg)
#define adxl_cfg_get_calibration_time_lsb_reg(ptr) ((ptr)->calibration_time_lsb_reg)
#define adxl_cfg_get_calibration_time_msb_reg(ptr) ((ptr)->calibration_time_msb_reg)
#define adxl_cfg_get_write_transactions_lsb_reg(ptr) ((ptr)->write_transactions_lsb_reg)
#define adxl_cfg_get_write_transactions_msb_reg(ptr) ((ptr)->write_transactions_msb_reg)
