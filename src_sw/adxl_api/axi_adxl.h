#pragma once

#include "adxl_cfg.h"
#include <math.h>


#define ADXL_OK 0
#define ADXL_UNINIT -1
#define ADXL_LINK_LOST -2


typedef struct {
    int has_init;
    adxl_cfg *cfg;
//    adxl_dev *dev;
//    adxl_axis axis;
    int init_flaq;
} axi_adxl;


#define TIMER_LIMIT 1000


void axi_adxl_cfg_debug(adxl_cfg *ptr);

int axi_adxl_init(axi_adxl *ptr, uint32_t baseaddr_cfg, uint32_t baseaddr_dev, uint8_t iic_address);

int axi_adxl_reset(axi_adxl *ptr);
int axi_adxl_perform_interval_requestion(axi_adxl *ptr, uint32_t requestion_interval);
int axi_adxl_perform_single_request(axi_adxl *ptr);
int axi_adxl_irq_allow(axi_adxl *ptr);
int axi_adxl_irq_ack(axi_adxl *ptr);
int axi_adxl_calibration(axi_adxl *ptr, uint32_t calibration_count_limit);