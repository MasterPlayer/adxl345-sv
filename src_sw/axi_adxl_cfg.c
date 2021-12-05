#include "axi_adxl_cfg.h"



void adxl_cfg_debug(adxl_cfg *cfg_ptr){
	printf("\r\n***** CONFIG DEBUG *****\r\n");
	printf("\t[RESET] : %d\r\n", adxl_cfg_has_reset(cfg_ptr));
	printf("\t[ENABLE] : %d\r\n", adxl_cfg_has_enable(cfg_ptr));
	printf("\t[IRQ_ALLOW] : %d\r\n", adxl_cfg_has_allow_irq(cfg_ptr));
	printf("\t[WORK] : %d\r\n", adxl_cfg_has_on_work(cfg_ptr));
	printf("\t[IIC_ADDR] : 0x%02x\r\n", adxl_cfg_get_i2c_addr(cfg_ptr));
	printf("\t[LINK] : %d\r\n", adxl_cfg_has_link(cfg_ptr));
	printf("\t[VERSION] : %02d.%02d\r\n", adxl_cfg_get_version_major(cfg_ptr), adxl_cfg_get_version_minor(cfg_ptr));
	printf("\t[REQUEST INTERVAL] : %d\r\n", adxl_cfg_get_request_interval(cfg_ptr));
	printf("\t[DATA_WIDTH] : %d bytes\r\n", adxl_cfg_get_data_width(cfg_ptr));
	printf("\t[READ_VALID_COUNT] : %d\r\n", adxl_cfg_get_read_valid_count(cfg_ptr));
	printf("\t[WRITE_VALID_COUNT] : %d\r\n", adxl_cfg_get_write_valid_count(cfg_ptr));
	printf("\t[WRITE_TRANSACTIONS] : %d\r\n", adxl_cfg_get_write_transactions(cfg_ptr));
	printf("\t[READ_TRANSACTIONS] : %d\r\n", adxl_cfg_get_read_transactions(cfg_ptr));
	printf("\t[CLK_PERIOD] : %3.3f MHz\r\n", (float)(adxl_cfg_get_clk_period(cfg_ptr)/1000000));

	xil_printf("\r\n");
}
