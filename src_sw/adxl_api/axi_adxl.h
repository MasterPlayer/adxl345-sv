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

typedef struct {
    int has_init;
    adxl_cfg *cfg;
    adxl_dev *dev;
//    adxl_axis axis;
    int init_flaq;
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
    DATA_READY = 0x80,
    SINGLE_TAP = 0x40,
    DOUBLE_TAP = 0x20,
    ACTIVITY = 0x10,
    INACTIVITY = 0x08,
    FREE_FALL = 0x04,
    WATERMARK = 0x02,
    OVERRUN = 0x01,
    INTR_ALL_MASK = 0xFF
}


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

#define ADXL_DEV_RW_COUNT 20
#define ADXL_DEV_RO_COUNT 10
#define ADXL_DEV_RESERVED_COUNT 34

static int rw_address_const[ADXL_DEV_RW_COUNT] = {29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 44, 45, 46, 47, 49, 56};
static int ro_address_const[ADXL_DEV_RO_COUNT] = {0, 43, 48, 50, 51, 52, 53, 54, 55, 57};
static int reserved_address_const[ADXL_DEV_RESERVED_COUNT] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,58,59,60,61,62,63};


int check_access_rw(int address);
int check_access_ro(int address);
int check_access_reserved(int address);


int axi_adxl_set_bw_rate(axi_adxl *ptr, uint8_t value);
int axi_adxl_measurement_start(axi_adxl *ptr);
int axi_adxl_measurement_stop(axi_adxl *ptr);
int axi_adxl_interrupt_enable(axi_adxl *ptr, uint8_t intr);
int axi_adxl_interrupt_disable(axi_adxl *ptr, uint8_t intr);