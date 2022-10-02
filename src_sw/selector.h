#include "axi_adxl.h"


#define ADXL_CFG_BASEADDRESS 0x40030000
#define ADXL_DEV_BASEADDRESS 0x40040000
#define ADXL_IIC_ADDRESS     0x53



int selector_axi_adxl_cfg_debug(axi_adxl *ptr);
int selector_axi_adxl_reset(axi_adxl *ptr);
int selector_axi_adxl_init(axi_adxl *ptr);
int selector_axi_adxl_perform_single_request(axi_adxl *ptr);
int selector_axi_adxl_has_single_request_completed(axi_adxl* ptr);

int selector_axi_adxl_perform_interval_requestion(axi_adxl *ptr);
int selector_axi_adxl_disable_interval_requestion(axi_adxl *ptr);
int selector_axi_adxl_switch_irq_allow(axi_adxl* ptr);
int selector_axi_adxl_calibration(axi_adxl *ptr);
int selector_axi_adxl_set_iic_address(axi_adxl *ptr);
int selector_axi_adxl_get_iic_address(axi_adxl* ptr);
int selector_axi_adxl_get_version(axi_adxl* ptr);


/*GET*/
int selector_axi_adxl_get_requestion_interval(axi_adxl* ptr);
int selector_axi_adxl_set_requestion_interval(axi_adxl* ptr);
int selector_axi_adxl_has_irq_allow(axi_adxl* ptr);
int selector_axi_adxl_has_interval_requestion(axi_adxl* ptr);
int selector_axi_adxl_has_reset(axi_adxl* ptr);
int selector_axi_adxl_set_start_address(axi_adxl* ptr);
int selector_axi_adxl_get_start_address(axi_adxl* ptr);
int selector_axi_adxl_set_size(axi_adxl* ptr);
int selector_axi_adxl_get_size(axi_adxl* ptr);
int selector_axi_adxl_has_work(axi_adxl* ptr);
int selector_axi_adxl_has_link(axi_adxl* ptr);

int selector_axi_adxl_get_calibration_pwr_count_limit(axi_adxl* ptr);
int selector_axi_adxl_set_calibration_pwr_count_limit(axi_adxl* ptr);
int selector_axi_adxl_has_calibration_in_progress(axi_adxl* ptr);
int selector_axi_adxl_has_calibration_complete(axi_adxl* ptr);



int selector_axi_adxl_set_thresh_tap(axi_adxl* ptr);
int selector_axi_adxl_get_thresh_tap(axi_adxl* ptr);

int selector_axi_adxl_set_ofsx(axi_adxl* ptr);
int selector_axi_adxl_set_ofsy(axi_adxl* ptr);
int selector_axi_adxl_set_ofsz(axi_adxl* ptr);

int selector_axi_adxl_get_ofsx(axi_adxl* ptr);
int selector_axi_adxl_get_ofsy(axi_adxl* ptr);
int selector_axi_adxl_get_ofsz(axi_adxl* ptr);


int selector_axi_adxl_set_duration(axi_adxl* ptr);
int selector_axi_adxl_get_duration(axi_adxl* ptr);

int selector_axi_adxl_set_latent(axi_adxl* ptr);
int selector_axi_adxl_get_latent(axi_adxl* ptr);

int selector_axi_adxl_set_window(axi_adxl* ptr);
int selector_axi_adxl_get_window(axi_adxl* ptr);

int selector_axi_adxl_set_thresh_act(axi_adxl* ptr);
int selector_axi_adxl_get_thresh_act(axi_adxl* ptr);

int selector_axi_adxl_set_thresh_inact(axi_adxl* ptr);
int selector_axi_adxl_get_thresh_inact(axi_adxl* ptr);

int selector_axi_adxl_set_time_inact(axi_adxl* ptr);
int selector_axi_adxl_get_time_inact(axi_adxl* ptr);

int selector_axi_adxl_set_activity_acdc(axi_adxl* ptr);
int selector_axi_adxl_get_activity_acdc(axi_adxl* ptr);

int selector_axi_adxl_set_inactivity_acdc(axi_adxl* ptr);
int selector_axi_adxl_get_inactivity_acdc(axi_adxl* ptr);

int selector_axi_adxl_activity_control_enable(axi_adxl* ptr);
int selector_axi_adxl_inactivity_control_enable(axi_adxl* ptr);
int selector_axi_adxl_activity_control_disable(axi_adxl* ptr);
int selector_axi_adxl_inactivity_control_disable(axi_adxl* ptr);

int selector_axi_adxl_set_thresh_ff(axi_adxl* ptr);
int selector_axi_adxl_get_thresh_ff(axi_adxl* ptr);

int selector_axi_adxl_set_time_ff(axi_adxl* ptr);
int selector_axi_adxl_get_time_ff(axi_adxl* ptr);

int selector_axi_adxl_set_tap_axes(axi_adxl* ptr);
int selector_axi_adxl_get_tap_axes(axi_adxl* ptr);

int selector_axi_adxl_is_act_status(axi_adxl* ptr);
int selector_axi_adxl_is_tap_status(axi_adxl* ptr);
int selector_axi_adxl_has_asleep_status(axi_adxl* ptr);

int selector_axi_adxl_set_bw_rate(axi_adxl *ptr);
int selector_axi_adxl_get_bw_rate(axi_adxl* ptr);

int selector_axi_adxl_measurement_start(axi_adxl *ptr);
int selector_axi_adxl_measurement_stop(axi_adxl *ptr);
int selector_axi_adxl_has_msmt(axi_adxl* ptr);

int selector_axi_adxl_set_linking_mode(axi_adxl* ptr);
int selector_axi_adxl_has_linking_mode(axi_adxl* ptr);

int selector_axi_adxl_set_autosleep_mode(axi_adxl* ptr);
int selector_axi_adxl_has_autosleep_mode(axi_adxl* ptr);

int selector_axi_adxl_set_wakeup(axi_adxl* ptr);
int selector_axi_adxl_get_wakeup(axi_adxl* ptr);

int selector_axi_adxl_interrupt_enable(axi_adxl *ptr);
int selector_axi_adxl_interrupt_disable(axi_adxl *ptr);
int selector_axi_adxl_has_interrupt_enabled(axi_adxl* ptr);

int selector_axi_adxl_set_int_map(axi_adxl* ptr);
int selector_axi_adxl_get_int_map(axi_adxl* ptr);

int selector_axi_adxl_get_int_source(axi_adxl* ptr);



int selector_axi_adxl_dev_debug_register_space(axi_adxl *ptr);
int selector_axi_adxl_change_range(axi_adxl *ptr);


int selector_axi_adxl_get_offsets(axi_adxl *ptr);
int selector_axi_adxl_set_ofsx(axi_adxl *ptr);
int selector_axi_adxl_set_ofsy(axi_adxl *ptr);
int selector_axi_adxl_set_ofsz(axi_adxl *ptr);
int selector_axi_adxl_get_data(axi_adxl *ptr);
int selector_axi_adxl_get_data_float(axi_adxl *ptr);
int selector_axi_adxl_set_fifo_mode(axi_adxl *ptr);
int selector_axi_adxl_get_fifo_mode(axi_adxl* ptr);

int selector_axi_adxl_set_samples(axi_adxl *ptr);
int selector_axi_adxl_get_samples(axi_adxl *ptr);
int selector_axi_adxl_get_fifo_sts_entries(axi_adxl *ptr);
int selector_axi_adxl_has_fifo_sts_trigger(axi_adxl *ptr);
int selector_axi_adxl_set_trigger(axi_adxl *ptr);
int selector_axi_adxl_get_trigger(axi_adxl *ptr);


int selector_axi_adxl_set_sleep_mode(axi_adxl *ptr);
int selector_axi_adxl_has_sleep_mode(axi_adxl *ptr);

int selector_axi_adxl_set_wakeup(axi_adxl *ptr);
int selector_axi_adxl_get_wakeup(axi_adxl* ptr);

int selector_axi_adxl_selftest(axi_adxl *ptr);
int selector_axi_adxl_has_selftest(axi_adxl *ptr);

int selector_axi_adxl_set_spi_mode(axi_adxl *ptr);
int selector_axi_adxl_has_spi_mode(axi_adxl *ptr);

int selector_axi_adxl_set_invert(axi_adxl *ptr);
int selector_axi_adxl_has_inverted(axi_adxl *ptr);

int selector_axi_adxl_justify(axi_adxl *ptr);
int selector_axi_adxl_has_justify(axi_adxl *ptr);


int selector_axi_adxl_set_output_rules(axi_adxl *ptr);
int selector_axi_adxl_get_output_rules(axi_adxl *ptr);
