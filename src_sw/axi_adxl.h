//#pragma once

#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "axi_adxl_cfg.h"
#include "axi_adxl_dev.h"

#define PI           3.14159265358979323846

//Scale for some fields in lsb/sec and other
#define SCALE_THRESH_TAP 		(0.0625)
#define SCALE_OFS        		(0.0156)        //15.6 us
#define SCALE_DUR        		(0.000625)      // 625us
#define SCALE_LATENT 			(0.00125) 		//1.25 ms
#define SCALE_WINDOW     		(0.00125) 		//1.25 ms
#define SCALE_THRESH_ACT 		(0.0625) 		//62.5 mg
#define SCALE_THRESH_INACT 		(0.0625) 		//62.5 mg
#define SCALE_TIME_INACT 		(1) 			//1 sec
#define SCALE_THRESH_FF  		(0.0625) 		//62.5 mg/lsb
#define SCALE_TIME_FF    		(0.005) 		//5ms/lsb

// Scale factor of xout yout zout
#define SCALE_FACTOR_FULL_RES 	(0.0039)
#define SCALE_FACTOR_2G  		(0.0039)
#define SCALE_FACTOR_4G  		(0.0078)
#define SCALE_FACTOR_8G  		(0.0156)
#define SCALE_FACTOR_16G 		(0.0312)

#define SCALE_RATIO_FULL_RES 	(SCALE_OFS/SCALE_FACTOR_FULL_RES)
#define SCALE_RATIO_2G 		 	(SCALE_OFS/SCALE_FACTOR_2G)
#define SCALE_RATIO_4G 			(SCALE_OFS/SCALE_FACTOR_4G)
#define SCALE_RATIO_8G 			(SCALE_OFS/SCALE_FACTOR_8G)
#define SCALE_RATIO_16G 		(SCALE_OFS/SCALE_FACTOR_16G)

//Sensitivity
#define SENSITIVITY_FULL_RES 256
#define SENSITIVITY_2G 256
#define SENSITIVITY_4G 128
#define SENSITIVITY_8G 64
#define SENSITIVITY_16G 32

#define INT_MAP_INTR0 0x00
#define INT_MAP_INTR1 0x01

#define TIMEOUT_TIMER_LIMIT 100000
#define CALIBRATION_STEPS 1000


#define ADXL_OK 0

#define ERR_UNINIT -1
#define ERR_LINK_LOST -2
#define ERR_TIMEOUT_AXI_CFG -3
#define ERR_RESET_INFINITE -4
#define ERR_DEVICE_MSMT_UNSTOP -5
#define ERR_AXICFG_DISABLE -6
#define ERROR_UNDEF_VALUE -7
//
//typedef struct {
//    int16_t x;
//    int16_t y;
//    int16_t z;
//} adxl_axis;

typedef struct {
    float x;
    float y;
    float z;
} g_coord;

typedef struct {
    int has_init;
    adxl_cfg *cfg;
    adxl_dev *dev;
    adxl_axis axis;
} axi_adxl;

typedef struct {
    int8_t x;
    int8_t y;
    int8_t z;
} offset_param;

enum mask {
    SUPRESS = 0x08,
	MASK_COUPLE = 0x08,
    MASK_X = 0x04,
    MASK_Y = 0x02,
    MASK_Z = 0x01
};

enum bw_rate {
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
    BW_RATE_0_10    = 0x00,
    BW_RATE_400_LP  = 0x1C,
    BW_RATE_200_LP  = 0x1B,
    BW_RATE_100_LP  = 0x1A,
    BW_RATE_50_LP   = 0x19,
    BW_RATE_25_LP   = 0x18,
    BW_RATE_12_5_LP = 0x17
};

enum int_mask {
	DATA_READY = 0x80,
	SINGLE_TAP = 0x40,
	DOUBLE_TAP = 0x20,
	ACTIVITY = 0x10,
	INACTIVITY = 0x08,
	FREE_FALL = 0x04,
	WATERMARK = 0x02,
	OVERRUN = 0x01,
	INTR_ALL_MASK = 0xFF
};

enum fifo_mode{
	BYPASS = 0x00,
	FIFO = 0x40,
	STREAM = 0x80,
	TRIGGER = 0xC0
};


enum range{
	RANGE_2G = 0x00,
	RANGE_4G = 0x01,
	RANGE_8G = 0x02,
	RANGE_16G = 0x03,
	RANGE_2G_FULL = 0x08,
	RANGE_4G_FULL = 0x09,
	RANGE_8G_FULL = 0x0A,
	RANGE_16G_FULL = 0x0B
};

#define AC_COUPLE 0x01
#define DC_COUPLE 0x00

#define ACT_COUPLE_MASK 0x80
#define INACT_COUPLE_MASK 0x08

#define ACT_MASK    0x70
#define INACT_MASK  0x07

#define ACTION_ENABLE  0x01
#define ACTION_DISABLE 0x00

#define axi_adxl_has_init(ptr) ((ptr)->has_init) ? TRUE : FALSE
#define axi_adxl_has_intr_enabled(ptr, mask) (adxl_dev_get_int_enable((ptr)->dev) & mask) ? TRUE : FALSE


#define axi_adxl_get_datax(ptr) (int16_t)(((uint16_t)adxl_dev_get_datax1((ptr)->dev)<<8) + ((uint16_t)adxl_dev_get_datax0((ptr)->dev)))
#define axi_adxl_get_datay(ptr) (int16_t)(((uint16_t)adxl_dev_get_datay1((ptr)->dev)<<8) + ((uint16_t)adxl_dev_get_datay0((ptr)->dev)))
#define axi_adxl_get_dataz(ptr) (int16_t)(((uint16_t)adxl_dev_get_dataz1((ptr)->dev)<<8) + ((uint16_t)adxl_dev_get_dataz0((ptr)->dev)))


#define axi_adxl_has_tap_x_source(ptr) (adxl_dev_get_act_tap_status((ptr)->dev) & MASK_X) ? 1:0
#define axi_adxl_has_tap_y_source(ptr) (adxl_dev_get_act_tap_status((ptr)->dev) & MASK_Y) ? 1:0
#define axi_adxl_has_tap_z_source(ptr) (adxl_dev_get_act_tap_status((ptr)->dev) & MASK_Z) ? 1:0

#define axi_adxl_has_act_x_source(ptr) (adxl_dev_get_act_tap_status((ptr)->dev) & (MASK_X<<4)) ? 1:0
#define axi_adxl_has_act_y_source(ptr) (adxl_dev_get_act_tap_status((ptr)->dev) & (MASK_Y<<4)) ? 1:0
#define axi_adxl_has_act_z_source(ptr) (adxl_dev_get_act_tap_status((ptr)->dev) & (MASK_Z<<4)) ? 1:0

#define axi_adxl_get_entries(ptr) ((adxl_dev_get_fifo_status((ptr)->dev)) & FIFO_STATUS_ENTRIES_MASK)



int axi_adxl_init(axi_adxl *ptr, uint32_t cfg_baseaddr, uint32_t dev_baseaddr, uint8_t i2c_addr);
int axi_adxl_setup_bandwidth(axi_adxl *ptr, uint8_t value);
int axi_adxl_start_requesting(axi_adxl *ptr);
int axi_adxl_stop_requesting(axi_adxl *ptr);
int axi_adxl_start_measure(axi_adxl *ptr, enum range range_value);
int axi_adxl_stop_measure(axi_adxl *ptr);
int axi_adxl_get_axis(axi_adxl *ptr);
int axi_adxl_calibration(axi_adxl *ptr, int count);
int axi_adxl_get_gravity(axi_adxl *ptr, g_coord *g_ptr);
int axi_adxl_setup_range(axi_adxl *ptr, enum range range_value);
int axi_adxl_enable_interrupt(axi_adxl *ptr, uint8_t mask);
int axi_adxl_disable_interrupt(axi_adxl *ptr, uint8_t mask);
int axi_adxl_set_thresh_tap(axi_adxl *ptr, uint8_t thresh_tap);
int axi_adxl_get_thresh_tap(axi_adxl *ptr, uint8_t *thresh_tap);
int axi_adxl_enable_tap_axes(axi_adxl *ptr, uint8_t mask);
int axi_adxl_disable_tap_axes(axi_adxl *ptr, uint8_t mask);
int axi_adxl_set_duration(axi_adxl *ptr, uint8_t value);
int axi_adxl_get_duration(axi_adxl *ptr, uint8_t *value);
int axi_adxl_set_latent(axi_adxl *ptr, uint8_t value);
int axi_adxl_get_latent(axi_adxl *ptr, uint8_t *value);
int axi_adxl_set_window(axi_adxl *ptr, uint8_t value);
int axi_adxl_get_window(axi_adxl *ptr, uint8_t *value);
int axi_adxl_set_thresh_act(axi_adxl *ptr, uint8_t value);
int axi_adxl_get_thresh_act(axi_adxl *ptr, uint8_t *value);
int axi_adxl_set_act_ctl(axi_adxl *ptr, enum mask mask_value, int action);
int axi_adxl_set_inact_ctl(axi_adxl *ptr, enum mask mask_value, int action);
int axi_adxl_set_act_couple(axi_adxl *ptr, int act_type);
int axi_adxl_set_inact_couple(axi_adxl *ptr, int act_type);
int axi_adxl_set_thresh_inact(axi_adxl *ptr, uint8_t value);
int axi_adxl_get_thresh_inact(axi_adxl *ptr, uint8_t *value);
int axi_adxl_set_time_inact(axi_adxl *ptr, uint8_t value);
int axi_adxl_get_time_inact(axi_adxl *ptr, uint8_t *value);
int axi_adxl_set_thresh_ff(axi_adxl *ptr, uint8_t value);
int axi_adxl_get_thresh_ff(axi_adxl *ptr, uint8_t *value);
int axi_adxl_set_time_ff(axi_adxl *ptr, uint8_t value);
int axi_adxl_get_time_ff(axi_adxl *ptr, uint8_t *value);
int axi_adxl_set_fifo_samples(axi_adxl *ptr, uint8_t value);
int axi_adxl_get_fifo_samples(axi_adxl *ptr, uint8_t *value);
int axi_adxl_set_fifo_mode(axi_adxl *ptr, enum fifo_mode value);

int axi_adxl_set_offset_x(axi_adxl *ptr, uint8_t value);
int axi_adxl_set_offset_y(axi_adxl *ptr, uint8_t value);
int axi_adxl_set_offset_z(axi_adxl *ptr, uint8_t value);

int axi_adxl_convert_raw_to_g(axi_adxl *ptr, adxl_axis raw, g_coord* g_ptr);


void axi_adxl_debug(axi_adxl *ptr);
