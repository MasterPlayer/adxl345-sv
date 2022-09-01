#pragma once

#include "adxl_cfg.h"
#include "adxl_dev.h"
#include <math.h>
#include "text_color.h"


#define ADXL_OK 0
#define ADXL_UNINIT -1
#define ADXL_LINK_LOST -2
#define ADXL_RESET_INFINITE -3
#define ADXL_NO_COMPLETE_SINGLE_REQUEST -4
#define ADXL_CANNOT_STOP -5
#define ADXL_UNCORRECT_VALUE -6
#define ADXL_TIMEOUT -7
/* Regsiter of device access */
#define ADXL_DEV_RW_COUNT 20
#define ADXL_DEV_RO_COUNT 10
#define ADXL_DEV_RESERVED_COUNT 34

#define SENSITIVITY_FULL_RES 256
#define SENSITIVITY_2G 256
#define SENSITIVITY_4G 128
#define SENSITIVITY_8G 64
#define SENSITIVITY_16G 32


static int rw_address_const[ADXL_DEV_RW_COUNT] = {29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 44, 45, 46, 47, 49, 56};
static int ro_address_const[ADXL_DEV_RO_COUNT] = {0, 43, 48, 50, 51, 52, 53, 54, 55, 57};
static int reserved_address_const[ADXL_DEV_RESERVED_COUNT] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,58,59,60,61,62,63};


typedef struct {
	int8_t ofsx;
	int8_t ofsy;
	int8_t ofsz;
}adxl_offset;


typedef struct {
	int16_t x;
	int16_t y;
	int16_t z;
}adxl_data;

typedef struct {
	float x;
	float y;
	float z;
}adxl_data_float;

typedef struct {
    int has_init;
    adxl_cfg *cfg;
    adxl_dev *dev;
    int init_flaq;
    adxl_offset offset;
    adxl_data data;
} axi_adxl;



enum bw_rate_enum {
    BW_RATE_400_LP  = 0x1C,
    BW_RATE_200_LP  = 0x1B,
    BW_RATE_100_LP  = 0x1A,
    BW_RATE_50_LP   = 0x19,
    BW_RATE_25_LP   = 0x18,
    BW_RATE_12_5_LP = 0x17,
    BW_RATE_3200    = 0x0F,
    BW_RATE_1600    = 0x0E,
    BW_RATE_800     = 0x0D,
    BW_RATE_400     = 0x0C,
    BW_RATE_200     = 0x0B,
    BW_RATE_100     = 0x0A,
    BW_RATE_50      = 0x09,
    BW_RATE_25      = 0x08,
    BW_RATE_12_5    = 0x07,
    BW_RATE_6_25    = 0x06,
    BW_RATE_3_13    = 0x05,
    BW_RATE_1_56    = 0x04,
    BW_RATE_0_78    = 0x03,
    BW_RATE_0_39    = 0x02,
    BW_RATE_0_20    = 0x01,
    BW_RATE_0_10    = 0x00
};

enum int_mask_enum {
    DATA_READY    = 0x80,
    SINGLE_TAP    = 0x40,
    DOUBLE_TAP    = 0x20,
    ACTIVITY      = 0x10,
    INACTIVITY    = 0x08,
    FREE_FALL     = 0x04,
    WATERMARK     = 0x02,
    OVERRUN       = 0x01,
    INTR_ALL_MASK = 0xFF
};


enum range_enum{
    RANGE_2G = 0x00,
    RANGE_4G = 0x01,
    RANGE_8G = 0x02,
    RANGE_16G = 0x03,
    RANGE_2G_FULL = 0x08,
    RANGE_4G_FULL = 0x09,
    RANGE_8G_FULL = 0x0A,
    RANGE_16G_FULL = 0x0B
};

enum act_enum {
    ACT_X_MASK      = 0x40, 
    ACT_Y_MASK      = 0x20, 
    ACT_Z_MASK      = 0x10
};

#define ACT_AC_MASK 	0x80
#define INACT_AC_MASK 	0x08

enum inact_enum {
    INACT_X_MASK    = 0x04, 
    INACT_Y_MASK    = 0x02, 
    INACT_Z_MASK    = 0x01
};

enum tap_axes_enum{
	TAP_SUPRESS_MASK = 0x08,
	TAP_X_MASK = 0x04,
	TAP_Y_MASK = 0x02,
	TAP_Z_MASK = 0x01
};

enum fifo_mode_enum {
    FIFO_MODE_BYPASS  = 0x00, 
    FIFO_MODE_FIFO    = 0x40,
    FIFO_MODE_STREAM  = 0x80, 
    FIFO_MODE_TRIGGER = 0xC0
};

enum act_tap_status_enum{
    ACT_X_SRC = 0x40,
    ACT_Y_SRC = 0x20,
    ACT_Z_SRC = 0x10,
    ASLEEP    = 0x08,
    TAP_X_SRC = 0x04,
    TAP_Y_SRC = 0x02,
    TAP_Z_SRC = 0x01
};


enum wakeup_enum{
    WAKEUP_8HZ = 0x00,
    WAKEUP_4HZ = 0x01,
    WAKEUP_2HZ = 0x02,
    WAKEUP_1HZ = 0x03
};


// measure precision in G or Seconds
#define SCALE_THRESH_TAP        (0.0625)
#define SCALE_OFS               (0.015625)        //15.6 us
#define SCALE_DUR               (0.000625)      // 625us
#define SCALE_LATENT            (0.00125)       //1.25 ms
#define SCALE_WINDOW            (0.00125)       //1.25 ms
#define SCALE_THRESH_ACT        (0.0625)        //62.5 mg
#define SCALE_THRESH_INACT      (0.0625)        //62.5 mg
#define SCALE_TIME_INACT        (1)             //1 sec
#define SCALE_THRESH_FF         (0.0625)        //62.5 mg/lsb
#define SCALE_TIME_FF           (0.005)         //5ms/lsb



#define SCALE_FACTOR_FULL_RES (0.0039)
#define SCALE_FACTOR_2G       (0.0039)
#define SCALE_FACTOR_4G       (0.0078)
#define SCALE_FACTOR_8G       (0.0156)
#define SCALE_FACTOR_16G      (0.0312)

#define TIMER_LIMIT 1000


void axi_adxl_cfg_debug(adxl_cfg *ptr);

void axi_adxl_dev_debug_register_space(adxl_dev *ptr);

int axi_adxl_init(axi_adxl *ptr, uint32_t baseaddr_cfg, uint32_t baseaddr_dev, uint8_t iic_address);

int axi_adxl_reset(axi_adxl *ptr);
int axi_adxl_perform_interval_requestion(axi_adxl *ptr, uint32_t requestion_interval);
int axi_adxl_disable_interval_requestion(axi_adxl *ptr);
int axi_adxl_perform_single_request(axi_adxl *ptr, uint8_t address, uint8_t size);
int axi_adxl_irq_allow(axi_adxl *ptr);
int axi_adxl_irq_unallow(axi_adxl *ptr);
int axi_adxl_irq_ack(axi_adxl *ptr);
int axi_adxl_calibration(axi_adxl *ptr, uint32_t calibration_count_limit);
int axi_adxl_set_iic_address(axi_adxl *ptr, uint8_t iic_address);

/*Checking register space*/
int check_access_rw(int address);
int check_access_ro(int address);
int check_access_reserved(int address);

/*Device api functions*/
int axi_adxl_set_bw_rate(axi_adxl *ptr, uint8_t value);
int axi_adxl_measurement_start(axi_adxl *ptr);
int axi_adxl_measurement_stop(axi_adxl *ptr);
int axi_adxl_interrupt_enable(axi_adxl *ptr, enum int_mask_enum intr);
int axi_adxl_interrupt_disable(axi_adxl *ptr, enum int_mask_enum intr);
int axi_adxl_change_range(axi_adxl *ptr, enum range_enum range);
int axi_adxl_change_thresh_tap(axi_adxl *ptr, uint8_t thresh_tap);
int axi_adxl_change_dur(axi_adxl *ptr, uint8_t duration);
int axi_adxl_change_latent(axi_adxl *ptr, uint8_t latent);
int axi_adxl_change_window(axi_adxl *ptr, uint8_t window);
int axi_adxl_change_thresh_act(axi_adxl *ptr, uint8_t thresh_act);
int axi_adxl_change_thresh_inact(axi_adxl *ptr, uint8_t thresh_inact);
int axi_adxl_change_time_inact(axi_adxl *ptr, uint8_t time_inact);


int axi_adxl_interrupt_enabled(axi_adxl *ptr, enum int_mask_enum intr);
int axi_adxl_get_range(axi_adxl *ptr);


// 0x27
int axi_adxl_activity_control_enable(axi_adxl *ptr, enum act_enum act_mask);
int axi_adxl_inactivity_control_enable(axi_adxl *ptr, enum inact_enum inact_mask);
int axi_adxl_activity_control_disable(axi_adxl *ptr, enum act_enum act_mask);
int axi_adxl_inactivity_control_disable(axi_adxl *ptr, enum inact_enum inact_mask);

int axi_adxl_change_activity_acdc(axi_adxl *ptr, uint8_t ac_mask);
int axi_adxl_change_inactivity_acdc(axi_adxl *ptr, uint8_t ac_mask);

int axi_adxl_has_act_inact_control(axi_adxl *ptr, uint8_t mask);

// 0x28 
int axi_adxl_change_thresh_ff(axi_adxl *ptr, uint8_t thresh_ff);
// 0x29
int axi_adxl_change_time_ff(axi_adxl *ptr, uint8_t time_ff);
// 0x2A 
int axi_adxl_change_tap_axes(axi_adxl *ptr, uint8_t mask);
int axi_adxl_tap_axes_actived(axi_adxl *ptr, uint8_t mask);
// 0x2B
int axi_adxl_has_act_status(axi_adxl *ptr, enum act_tap_status_enum act);
int axi_adxl_has_tap_status(axi_adxl *ptr, enum act_tap_status_enum tap);
int axi_adxl_has_sleep_status(axi_adxl *ptr);

// 0x1E
int axi_adxl_get_ofsx(axi_adxl *ptr, int8_t *ofsx);
int axi_adxl_get_ofsy(axi_adxl *ptr, int8_t *ofsy);
int axi_adxl_get_ofsz(axi_adxl *ptr, int8_t *ofsz);
int axi_adxl_set_ofsx(axi_adxl *ptr, int8_t ofsx);
int axi_adxl_set_ofsy(axi_adxl *ptr, int8_t ofsy);
int axi_adxl_set_ofsz(axi_adxl *ptr, int8_t ofsz);

//0x2F
int axi_adxl_change_int_map(axi_adxl *ptr, uint8_t int_mask);
int axi_adxl_get_int_map(axi_adxl *ptr, uint8_t mask);

// 0x30 
int axi_adxl_get_int_source(axi_adxl *ptr, uint8_t *interrupt_mask);
int axi_adxl_has_int_source(axi_adxl *ptr, uint8_t interrupt_mask);

#define axi_adxl_get_datax(ptr) (int16_t)(((uint16_t)adxl_dev_get_datax1((ptr)->dev)<<8) + ((uint16_t)adxl_dev_get_datax0((ptr)->dev)))
#define axi_adxl_get_datay(ptr) (int16_t)(((uint16_t)adxl_dev_get_datay1((ptr)->dev)<<8) + ((uint16_t)adxl_dev_get_datay0((ptr)->dev)))
#define axi_adxl_get_dataz(ptr) (int16_t)(((uint16_t)adxl_dev_get_dataz1((ptr)->dev)<<8) + ((uint16_t)adxl_dev_get_dataz0((ptr)->dev)))

int axi_adxl_get_data(axi_adxl *ptr, adxl_data *data);
int axi_adxl_get_data_float(axi_adxl *ptr, adxl_data_float *data_float);
// 0x38
int axi_adxl_set_fifo_mode(axi_adxl *ptr, enum fifo_mode_enum fifo_mode);
int axi_adxl_has_fifo_mode(axi_adxl *ptr, enum fifo_mode_enum fifo_mode);
int axi_adxl_set_trigger(axi_adxl *ptr, uint8_t trigger_flaq);
int axi_adxl_get_trigger(axi_adxl *ptr, uint8_t *trigger_flaq);
int axi_adxl_set_samples(axi_adxl *ptr, uint8_t samples);
int axi_adxl_get_samples(axi_adxl *ptr, uint8_t *samples);
// 0x39
int axi_adxl_get_fifo_sts_entries(axi_adxl *ptr, uint8_t *entries);
int axi_adxl_has_fifo_sts_trigger(axi_adxl *ptr);


int axi_adxl_set_linking_mode(axi_adxl *ptr, int state);
int axi_adxl_has_linking_mode(axi_adxl *ptr);
int axi_adxl_get_linking_mode(axi_adxl *ptr, int *state);

int axi_adxl_set_autosleep_mode(axi_adxl *ptr, int state);
int axi_adxl_has_autosleep_mode(axi_adxl *ptr);
int axi_adxl_get_autosleep_mode(axi_adxl *ptr, int *state);

int axi_adxl_set_sleep_mode(axi_adxl *ptr, int state);
int axi_adxl_has_sleep_mode(axi_adxl *ptr);
int axi_adxl_get_sleep_mode(axi_adxl *ptr, int *state);

int axi_adxl_set_wakeup(axi_adxl *ptr, enum wakeup_enum wakeup);
int axi_adxl_has_wakeup(axi_adxl *ptr, enum wakeup_enum wakeup);

int axi_adxl_selftest(axi_adxl *ptr, int state);
int axi_adxl_has_selftest(axi_adxl *ptr);

int axi_adxl_set_spi_mode(axi_adxl *ptr, int mode);
int axi_adxl_has_spi_mode(axi_adxl *ptr);

int axi_adxl_invert(axi_adxl *ptr, int mode);
int axi_adxl_has_inverted(axi_adxl *ptr);

int axi_adxl_justify(axi_adxl *ptr, int mode);
int axi_adxl_has_justify(axi_adxl *ptr);


