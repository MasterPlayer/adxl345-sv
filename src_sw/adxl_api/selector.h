#include "axi_adxl.h"


#define ADXL_CFG_BASEADDRESS 0x40030000
#define ADXL_DEV_BASEADDRESS 0x40040000
#define ADXL_IIC_ADDRESS     0x53



int selector_axi_adxl_cfg_debug(axi_adxl *ptr);
int selector_axi_adxl_reset(axi_adxl *ptr);
int selector_axi_adxl_init(axi_adxl *ptr);
int selector_axi_adxl_perform_single_request(axi_adxl *ptr);
int selector_axi_adxl_perform_interval_requestion(axi_adxl *ptr);
int selector_axi_adxl_disable_interval_requestion(axi_adxl *ptr);
int selector_axi_adxl_irq_allow(axi_adxl *ptr);
int selector_axi_adxl_irq_unallow(axi_adxl *ptr);
int selector_axi_adxl_calibration(axi_adxl *ptr);
int selector_axi_adxl_set_iic_address(axi_adxl *ptr);
int selector_axi_adxl_set_bw_rate(axi_adxl *ptr);
int selector_axi_adxl_measurement_start(axi_adxl *ptr);
int selector_axi_adxl_measurement_stop(axi_adxl *ptr);
int selector_axi_adxl_interrupt_enable(axi_adxl *ptr);
int selector_axi_adxl_interrupt_disable(axi_adxl *ptr);
int selector_axi_adxl_dev_debug_register_space(axi_adxl *ptr);
int selector_axi_adxl_change_range(axi_adxl *ptr);