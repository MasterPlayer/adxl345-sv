//#pragma once

#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "axi_adxl_cfg.h"
#include "axi_adxl_dev.h"

#define PI           3.14159265358979323846

//Scale for some fields in lsb/sec and other
#define SCALE_THRESH_TAP 		(0.0625)
#define SCALE_OFS        		(0.0156)       //15.6 us
#define SCALE_DUR        		(0.000625)     //6.25us
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

#define SCALE_RATIO_FULL_RES (SCALE_OFS/SCALE_FACTOR_FULL_RES)
#define SCALE_RATIO_2G (SCALE_OFS/SCALE_FACTOR_2G)
#define SCALE_RATIO_4G (SCALE_OFS/SCALE_FACTOR_4G)
#define SCALE_RATIO_8G (SCALE_OFS/SCALE_FACTOR_8G)
#define SCALE_RATIO_16G (SCALE_OFS/SCALE_FACTOR_16G)

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

typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} adxl_axis;

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
	OVERRUN = 0x01
};



#define AC_COUPLE 0x01
#define DC_COUPLE 0x00

#define ACT_MASK    0xF0
#define INACT_MASK  0x0F

#define axi_adxl_has_init(ptr) ((ptr)->has_init) ? TRUE : FALSE

#define axi_adxl_get_datax(ptr) (int16_t)(((uint16_t)adxl_dev_get_datax1((ptr)->dev)<<8) + ((uint16_t)adxl_dev_get_datax0((ptr)->dev)))
#define axi_adxl_get_datay(ptr) (int16_t)(((uint16_t)adxl_dev_get_datay1((ptr)->dev)<<8) + ((uint16_t)adxl_dev_get_datay0((ptr)->dev)))
#define axi_adxl_get_dataz(ptr) (int16_t)(((uint16_t)adxl_dev_get_dataz1((ptr)->dev)<<8) + ((uint16_t)adxl_dev_get_dataz0((ptr)->dev)))



int axi_adxl_init(axi_adxl *ptr, uint32_t cfg_baseaddr, uint32_t dev_baseaddr);
int axi_adxl_cfg_init(axi_adxl *ptr, uint32_t baseaddr);
int axi_adxl_dev_init(axi_adxl *ptr, uint32_t baseaddr);

int axi_adxl_enable(axi_adxl *ptr, uint8_t i2c_addr, uint32_t request_interval);
int axi_adxl_disable(axi_adxl *ptr);
int axi_adxl_disable_requesting(axi_adxl *ptr);
int axi_adxl_change_bw(axi_adxl *ptr, uint8_t value);
int axi_adxl_has_low_power(axi_adxl *ptr);
int axi_adxl_get_axis(axi_adxl *ptr);

int axi_adxl_set_offset(axi_adxl *ptr, offset_param offt_prm, enum mask mask_value);
int axi_adxl_get_offset(axi_adxl *ptr, offset_param *offt_prm);
int axi_adxl_get_offset_x(axi_adxl *ptr, uint8_t *offset);
int axi_adxl_get_offset_y(axi_adxl *ptr, uint8_t *offset);
int axi_adxl_get_offset_z(axi_adxl *ptr, uint8_t *offset);


int axi_adxl_get_gravity(axi_adxl *ptr, g_coord *g_ptr);
int axi_adxl_calibration(axi_adxl *ptr);
int axi_adxl_has_runned(axi_adxl *ptr);

int axi_adxl_set_full_resolution(axi_adxl *ptr, uint8_t flaq);

int axi_adxl_set_range(axi_adxl *ptr, uint8_t range);
int axi_adxl_get_range(axi_adxl *ptr, uint8_t *range);


int axi_adxl_get_roll(axi_adxl *ptr, float *roll);
int axi_adxl_get_pitch(axi_adxl *ptr, float *pitch);

int axi_adxl_set_thresh_tap(axi_adxl *ptr, uint8_t thresh_tap);
int axi_adxl_get_thresh_tap(axi_adxl *ptr, uint8_t *thresh_tap);

int axi_adxl_set_dur(axi_adxl *ptr, uint8_t dur);
int axi_adxl_get_dur(axi_adxl *ptr, uint8_t *dur);

int axi_adxl_set_latency(axi_adxl *ptr, uint8_t latency);
int axi_adxl_get_latency(axi_adxl *ptr, uint8_t *latency);

int axi_adxl_set_window(axi_adxl *ptr, uint8_t window);
int axi_adxl_get_window(axi_adxl *ptr, uint8_t *window);


int axi_adxl_set_int_map(axi_adxl *ptr, uint8_t mask, uint8_t map);

int axi_adxl_int_enable(axi_adxl *ptr, uint8_t mask);
int axi_adxl_int_disable(axi_adxl *ptr, uint8_t mask);

#define axi_adxl_has_int_enabled(ptr, mask) (adxl_dev_get_int_enable(ptr->dev) & mask)


int axi_adxl_set_tap_axes_active(axi_adxl *ptr, enum mask mask_value);
#define axi_adxl_has_tap_axes_active(ptr, mask) (adxl_dev_get_tap_axes(ptr) & mask)

int axi_adxl_int_invert(axi_adxl *ptr);

int axi_adxl_set_act_ctl(axi_adxl *ptr, int act_mode, enum mask mask_value);
int axi_adxl_set_inact_ctl(axi_adxl *ptr, int act_mode, enum mask mask_value);

int axi_adxl_set_thresh_act(axi_adxl *ptr, uint8_t value);
int axi_adxl_get_thresh_act(axi_adxl *ptr, uint8_t *value);

void axi_adxl_debug(axi_adxl *ptr);
