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
int selector_axi_adxl_change_thresh_tap(axi_adxl *ptr);
int selector_axi_adxl_change_dur(axi_adxl *ptr);
int selector_axi_adxl_change_latent(axi_adxl *ptr);
int selector_axi_adxl_change_window(axi_adxl *ptr);
int selector_axi_adxl_change_thresh_act(axi_adxl *ptr);
int selector_axi_adxl_change_thresh_inact(axi_adxl *ptr);
int selector_axi_adxl_change_time_inact(axi_adxl *ptr);

int selector_axi_adxl_activity_control_enable(axi_adxl *ptr);
int selector_axi_adxl_inactivity_control_enable(axi_adxl *ptr);
int selector_axi_adxl_activity_control_disable(axi_adxl *ptr);
int selector_axi_adxl_inactivity_control_disable(axi_adxl *ptr);
int selector_axi_adxl_change_activity_acdc(axi_adxl *ptr);
int selector_axi_adxl_change_inactivity_acdc(axi_adxl *ptr);
int selector_axi_adxl_change_thresh_ff(axi_adxl *ptr);
int selector_axi_adxl_change_time_ff(axi_adxl *ptr);
int selector_axi_adxl_change_tap_axes(axi_adxl *ptr);

int selector_axi_adxl_get_offsets(axi_adxl *ptr);
int selector_axi_adxl_set_ofsx(axi_adxl *ptr);
int selector_axi_adxl_set_ofsy(axi_adxl *ptr);
int selector_axi_adxl_set_ofsz(axi_adxl *ptr);
int selector_axi_adxl_change_int_map(axi_adxl *ptr);
int selector_axi_adxl_get_int_source(axi_adxl *ptr);
int selector_axi_adxl_get_data(axi_adxl *ptr);
int selector_axi_adxl_get_data_float(axi_adxl *ptr);
int selector_axi_adxl_set_fifo_mode(axi_adxl *ptr);
int selector_axi_adxl_set_samples(axi_adxl *ptr);
int selector_axi_adxl_get_samples(axi_adxl *ptr);
int selector_axi_adxl_get_fifo_sts_entries(axi_adxl *ptr);
int selector_axi_adxl_has_fifo_sts_trigger(axi_adxl *ptr);
int selector_axi_adxl_set_trigger(axi_adxl *ptr);
int selector_axi_adxl_get_trigger(axi_adxl *ptr);

int selector_axi_adxl_has_act_status(axi_adxl *ptr);
int selector_axi_adxl_has_tap_status(axi_adxl *ptr);
int selector_axi_adxl_has_sleep_status(axi_adxl *ptr);