#include "axi_adxl.h"

void axi_adxl_cfg_debug(adxl_cfg *ptr){
	printf("[CFG SPACE]\r\n");
	printf("\t[NAME] \t\t\t| [RAW]\t\t| [VALUE]\r\n");
	printf("\t[VERSION] \t\t| 0x%08x \t| %d.%d\r\n", (((uint32_t)adxl_cfg_ctl_get_version_major(ptr) << 8) + (uint32_t)adxl_cfg_ctl_get_version_minor(ptr)), adxl_cfg_ctl_get_version_major(ptr), adxl_cfg_ctl_get_version_minor(ptr));
	printf("\t[RESET] \t\t| 0x%08x \t| %s  \r\n", adxl_cfg_ctl_reset_completed(ptr), adxl_cfg_ctl_reset_completed(ptr) ? "yes" : "no");
	printf("\t[INTERVAL_REQUESTS] \t| 0x%08x \t| %s\r\n", adxl_cfg_ctl_interval_requestion(ptr), adxl_cfg_ctl_interval_requestion(ptr) : "actived" ? "inactived");
	printf("\t[IRQ_ALLOW] \t\t| 0x%08x \t| %s\r\n", adxl_cfg_ctl_irq_allowed(ptr), adxl_cfg_ctl_irq_allowed(ptr)? "yes" : "no");
	printf("\t[CALIBRATION] \t\t| 0x%08x \t| %s\r\n", adxl_cfg_ctl_calibration_completed(ptr), adxl_cfg_ctl_calibration_completed(ptr)? "completed" : "not started");
	printf("\t[I2C ADDRESS] \t\t| 0x%08x \t| 0x%02x\r\n", adxl_cfg_ctl_get_iic_address(ptr), adxl_cfg_ctl_get_iic_address(ptr));
	printf("\t[LINK] \t\t\t| 0x%08x \t| %s\r\n", adxl_cfg_ctl_link(ptr), adxl_cfg_ctl_link(ptr)? "on" : "off");
	printf("\t[WORK] \t\t\t| 0x%08x \t| %s\r\n", adxl_cfg_ctl_work(ptr), adxl_cfg_ctl_work(ptr)? "on" : "off");
	printf("\t[REQUEST_INTERVAL] \t| 0x%08x \t| %d \r\n", adxl_cfg_get_request_interval(ptr), adxl_cfg_get_request_interval(ptr));
	printf("\t[CALIBRATION_COUNT] \t| 0x%08x \t| %d\r\n", adxl_cfg_get_calibration_count_limit(ptr), (1 << adxl_cfg_get_calibration_count_limit(ptr)));
	printf("\t[READ_VALID_COUNT] \t| 0x%08x \t| %d\r\n", adxl_cfg_ctl_get_read_valid_count(ptr), adxl_cfg_ctl_get_read_valid_count(ptr));
	printf("\t[WRITE_VALID_COUNT] \t| 0x%08x \t| %d\r\n", adxl_cfg_ctl_get_write_valid_count(ptr), adxl_cfg_ctl_get_write_valid_count(ptr));
	printf("\t[READ_TRANSACTIONS_LSB] | 0x%08x \t| %d\r\n", adxl_cfg_ctl_get_read_transactions_lsb(ptr), adxl_cfg_ctl_get_read_transactions_lsb(ptr));
	printf("\t[READ_TRANSACTIONS_MSB] | 0x%08x \t| %d\r\n", adxl_cfg_ctl_get_read_transactions_msb(ptr), adxl_cfg_ctl_get_read_transactions_msb(ptr));
	printf("\t[CLK_PERIOD] \t\t| 0x%08x \t| %d\r\n", adxl_cfg_get_clk_period(ptr), adxl_cfg_get_clk_period(ptr));

	printf("\r\n");

}
