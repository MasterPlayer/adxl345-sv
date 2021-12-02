#pragma once

#include <stdint.h>
#include <stdio.h>

#include "axi_adxl_cfg.h"
#include "axi_adxl_dev.h"

//Scale for some fields in lsb/sec and other
#define SCALE_THRESH_TAP 62.5 //mg
#define SCALE_OFS        15.6 //mg
#define SCALE_DUR        625 //us
#define SCALE_WINDOW     1.25 //ms
#define SCALE_THRESH_FF  62.5 //mg
#define SCALE_TIME_FF    5 // ms

#define TIMEOUT_TIMER_LIMIT 100000

#define ADXL_OK 0

#define ERR_UNINIT -1
#define ERR_LINK_LOST -2
#define ERR_TIMEOUT_AXI_CFG -3
#define ERR_RESET_INFINITE -4
#define ERR_DEVICE_MSMT_UNSTOP -5
#define ERR_AXICFG_DISABLE -6

typedef struct {
    uint16_t x;
    uint16_t y;
    uint16_t z;
} adxl_axis;


typedef struct {
    int has_init;
    adxl345_cfg *cfg;
    adxl345_dev *dev;
    adxl_axis axis;
} adxl;


typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t z;
} offset_param;


enum mask {
    MASK_X = 0x04,
    MASK_Y = 0x02,
    MASK_Z = 0x01
};

enum bw_rate {
    BW_RATE_3200 = 0x0F,
    BW_RATE_1600 = 0x0E,
    BW_RATE_800 = 0x0D,
    BW_RATE_400 = 0x0C,
    BW_RATE_200 = 0x0B,
    BW_RATE_100 = 0x0A,
    BW_RATE_50 = 0x09,
    BW_RATE_25 = 0x08,
    BW_RATE_12_5 = 0x07,
    BW_RATE_6_25 = 0x06,
    BW_RATE_3_13 = 0x05,
    BW_RATE_1_56 = 0x04,
    BW_RATE_0_78 = 0x03,
    BW_RATE_0_39 = 0x02,
    BW_RATE_0_20 = 0x01,
    BW_RATE_0_10 = 0x00,
    BW_RATE_400_LP = 0x1C,
    BW_RATE_200_LP = 0x1B,
    BW_RATE_100_LP = 0x1A,
    BW_RATE_50_LP = 0x19,
    BW_RATE_25_LP = 0x18,
    BW_RATE_12_5_LP = 0x17
};

// #define MASK_X      0x04
// #define MASK_Y      0x02
// #define MASK_Z      0x01
// #define MASK_ALL    0x07

#define AC_COUPLE 0x01
#define DC_COUPLE 0x00

#define ACT_MASK    0xF0
#define INACT_MASK  0x0F

#define axi_adxl_has_init(adxl_ptr) ((adxl_ptr)->has_init) ? TRUE : FALSE

int axi_adxl_init(adxl *adxl_ptr, uint32_t cfg_baseaddr, uint32_t dev_baseaddr);
int axi_adxl_cfg_init(adxl *adxl_ptr, uint32_t baseaddr);
int axi_adxl_dev_init(adxl *adxl_ptr, uint32_t baseaddr);

int axi_adxl_dev_enable(adxl *adxl_ptr, uint8_t i2c_addr, uint32_t request_interval);
int axi_adxl_dev_disable(adxl *adxl_ptr);
int axi_adxl_dev_change_bw(adxl *adxl_ptr, uint8_t value);
int axi_adxl_dev_get_mode(adxl *adxl_ptr);
int axi_adxl_dev_get_axis(adxl *adxl_ptr);
int axi_adxl_dev_set_offset(adxl *adxl_ptr, offset_param offt_prm, enum mask mask_value);

void axi_adxl_debug(adxl *adxl_ptr);


