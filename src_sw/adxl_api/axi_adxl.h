#pragma once

#include "adxl_cfg.h"
#include <math.h>


typedef struct {
    int has_init;
    adxl_cfg *cfg;
//    adxl_dev *dev;
//    adxl_axis axis;
} axi_adxl;



void axi_adxl_cfg_debug(adxl_cfg *ptr);

