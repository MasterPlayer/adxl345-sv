#pragma once

typedef struct {
    uint8_t device_id_reg;
    uint8_t reserved1_reg;
    uint8_t reserved2_reg;
    uint8_t reserved3_reg;
    uint8_t reserved4_reg;
    uint8_t reserved5_reg;
    uint8_t reserved6_reg;
    uint8_t reserved7_reg;
    uint8_t reserved8_reg;
    uint8_t reserved9_reg;
    uint8_t reserved10_reg;
    uint8_t reserved11_reg;
    uint8_t reserved12_reg;
    uint8_t reserved13_reg;
    uint8_t reserved14_reg;
    uint8_t reserved15_reg;
    uint8_t reserved16_reg;
    uint8_t reserved17_reg;
    uint8_t reserved18_reg;
    uint8_t reserved19_reg;
    uint8_t reserved20_reg;
    uint8_t reserved21_reg;
    uint8_t reserved22_reg;
    uint8_t reserved23_reg;
    uint8_t reserved24_reg;
    uint8_t reserved25_reg;
    uint8_t reserved26_reg;
    uint8_t reserved27_reg;
    uint8_t reserved28_reg;
    uint8_t thresh_tap_reg;
    uint8_t ofsx_reg;
    uint8_t ofsy_reg;
    uint8_t ofsz_reg;
    uint8_t dur_reg;
    uint8_t latent_reg;
    uint8_t window_reg;
    uint8_t thresh_act_reg;
    uint8_t thresh_inact_reg;
    uint8_t time_inact_reg;
    uint8_t act_inact_ctl_reg;
    uint8_t thresh_ff_reg;
    uint8_t time_ff_reg;
    uint8_t tap_axes_reg;
    uint8_t act_tap_status_reg;
    uint8_t bw_rate_reg;
    uint8_t power_ctl_reg;
    uint8_t int_enable_reg;
    uint8_t int_map_reg;
    uint8_t int_source_reg;
    uint8_t data_format_reg;
    uint8_t datax0_reg;
    uint8_t datax1_reg;
    uint8_t datay0_reg;
    uint8_t datay1_reg;
    uint8_t dataz0_reg;
    uint8_t dataz1_reg;
    uint8_t fifo_ctl_reg;
    uint8_t fifo_status_reg;
    uint8_t reserved58_reg;
    uint8_t reserved59_reg;
    uint8_t reserved60_reg;
    uint8_t reserved61_reg;
    uint8_t reserved62_reg;
    uint8_t reserved63_reg;
}adxl_dev;



#define ACT_INACT_CTL_ACT_ACDC_MASK     0x80
#define ACT_INACT_CTL_ACT_X_EN_MASK     0x40
#define ACT_INACT_CTL_ACT_Y_EN_MASK     0x20
#define ACT_INACT_CTL_ACT_Z_EN_MASK     0x10
#define ACT_INACT_CTL_INACT_ACDC_MASK   0x08
#define ACT_INACT_CTL_INACT_X_EN_MASK   0x04
#define ACT_INACT_CTL_INACT_Y_EN_MASK   0x02
#define ACT_INACT_CTL_INACT_Z_EN_MASK   0x01

#define TAP_AXES_SUPRESS_MASK           0x08
#define TAP_AXES_TAP_X_EN_MASK          0x04
#define TAP_AXES_TAP_Y_EN_MASK          0x02
#define TAP_AXES_TAP_Z_EN_MASK          0x01

#define ACT_TAP_STATUS_ACT_X_SRC_MASK   0x40
#define ACT_TAP_STATUS_ACT_Y_SRC_MASK   0x20
#define ACT_TAP_STATUS_ACT_Z_SRC_MASK   0x10
#define ACT_TAP_STATUS_ASLEEP_MASK      0x08
#define ACT_TAP_STATUS_TAP_X_SRC_MASK   0x04
#define ACT_TAP_STATUS_TAP_Y_SRC_MASK   0x02
#define ACT_TAP_STATUS_TAP_Z_SRC_MASK   0x01

#define BW_RATE_LOW_POWER_MASK          0x10 //000X1010   LOW POWER
#define BW_RATE_RATE_MASK               0x0F //0000XXXX   RATE

#define POWER_CTL_LINK_MASK             0x20 //00X00000   LINK
#define POWER_CTL_AUTO_SLEEP_MASK       0x10 //000X0000   AUTO SLEEP
#define POWER_CTL_MEASURE_MASK          0x08 //0000X000   MEASURE
#define POWER_CTL_SLEEP_MASK            0x04 //00000X00   SLEEP
#define POWER_CTL_WAKEUP_MASK           0x03 //000000XX   WAKEUP

#define INT_ENABLE_DATA_READY_MASK      0x80 //x0000000   DATA_READY
#define INT_ENABLE_SINGLE_TAP_MASK      0x40 //0x000000   SINGLE_TAP
#define INT_ENABLE_DOUBLE_TAP_MASK      0x20 //00x00000   DOUBLE_TAP
#define INT_ENABLE_ACTIVITY_MASK        0x10 //000x0000   ACTIVITY
#define INT_ENABLE_INACTIVITY_MASK      0x08 //0000x000   INACTIVITY
#define INT_ENABLE_FREE_FALL_MASK       0x04 //00000x00   FREE_FALL
#define INT_ENABLE_WATERMARK_MASK       0x02 //000000x0   WATERMARK
#define INT_ENABLE_OVERRUN_MASK         0x01 //0000000x   OVERRUN

#define INT_MAP_ALL_MASK 				0xFF
#define INT_MAP_DATA_READY_MASK         0x80 //x0000000   DATA_READY
#define INT_MAP_SINGLE_TAP_MASK         0x40 //0x000000   SINGLE_TAP
#define INT_MAP_DOUBLE_TAP_MASK         0x20 //00x00000   DOUBLE_TAP
#define INT_MAP_ACTIVITY_MASK           0x10 //000x0000   ACTIVITY
#define INT_MAP_INACTIVITY_MASK         0x08 //0000x000   INACTIVITY
#define INT_MAP_FREE_FALL_MASK          0x04 //00000x00   FREE_FALL
#define INT_MAP_WATERMARK_MASK          0x02 //000000x0   WATERMARK
#define INT_MAP_OVERRUN_MASK            0x01 //0000000x   OVERRUN

#define INT_SOURCE_DATA_READY_MASK      0x80 //x0000010   DATA_READY
#define INT_SOURCE_SINGLE_TAP_MASK      0x40 //0x000010   SINGLE_TAP
#define INT_SOURCE_DOUBLE_TAP_MASK      0x20 //00x00010   DOUBLE_TAP
#define INT_SOURCE_ACTIVITY_MASK        0x10 //000x0010   ACTIVITY
#define INT_SOURCE_INACTIVITY_MASK      0x08 //0000x010   INACTIVITY
#define INT_SOURCE_FREE_FALL_MASK       0x04 //00000x10   FREE_FALL
#define INT_SOURCE_WATERMARK_MASK       0x02 //000000x0   WATERMARK
#define INT_SOURCE_OVERRUN_MASK         0x01 //0000001x   OVERRUN

#define DATA_FORMAT_SELFTEST_MASK       0x80 //x0000000   SELFTEST
#define DATA_FORMAT_SPI_MASK            0x40 //0x000000   SPI
#define DATA_FORMAT_INT_INVERT_MASK     0x20 //00x00000   INT_INVERT
#define DATA_FORMAT_0_MASK              0x10 //000x0000   0
#define DATA_FORMAT_FULL_RES_MASK       0x08 //0000x000   FULL_RES
#define DATA_FORMAT_JUSTIFY_MASK        0x04 //00000x00   JUSTIFY
#define DATA_FORMAT_RANGE_MASK          0x03//000000xx   RANGE

#define FIFO_CTL_FIFO_MODE              0xC0 // xx000000
#define FIFO_CTL_TRIGGER                0x20 // 00x00000
#define FIFO_CTL_SAMPLES                0x1F // 000xxxxx

#define FIFO_STATUS_ENTRIES_MASK 		0x3F
#define FIFO_STATUS_TRIGGER_MASK        0x80

#define DATA_FORMAT_FULL_RES 			0x08

#define DATA_FORMAT_RANGE_2G 			0x00
#define DATA_FORMAT_RANGE_4G 			0x01
#define DATA_FORMAT_RANGE_8G 			0x02
#define DATA_FORMAT_RANGE_16G 			0x03

//0x00
#define adxl_dev_get_device_id(ptr) ((ptr)->device_id_reg)
//0x1d
#define adxl_dev_set_thresh_tap(ptr, value) ((ptr)->thresh_tap_reg = (uint8_t)value)
#define adxl_dev_get_thresh_tap(ptr) ((ptr)->thresh_tap_reg)
//0x1e
#define adxl_dev_set_ofsx(ptr, value) ((ptr)->ofsx_reg = (uint8_t)value)
#define adxl_dev_get_ofsx(ptr) ((ptr)->ofsx_reg)
//0x1f
#define adxl_dev_set_ofsy(ptr, value) ((ptr)->ofsy_reg = (uint8_t)value)
#define adxl_dev_get_ofsy(ptr) ((ptr)->ofsy_reg)
//0x20
#define adxl_dev_set_ofsz(ptr, value) ((ptr)->ofsz_reg = (uint8_t)value)
#define adxl_dev_get_ofsz(ptr) ((ptr)->ofsz_reg)
//0x21
#define adxl_dev_set_dur(ptr, value) ((ptr)->dur_reg = value)
#define adxl_dev_get_dur(ptr) ((ptr)->dur_reg)

#define adxl_dev_set_latent(ptr, value) ((ptr)->latent_reg = value)
#define adxl_dev_get_latent(ptr) ((ptr)->latent_reg)

#define adxl_dev_set_window(ptr, value) ((ptr)->window_reg = value)
#define adxl_dev_get_window(ptr) ((ptr)->window_reg)

#define adxl_dev_set_thresh_act(ptr, value) ((ptr)->thresh_act_reg = value)
#define adxl_dev_get_thresh_act(ptr) ((ptr)->thresh_act_reg)

#define adxl_dev_set_thresh_inact(ptr, value) ((ptr)->thresh_inact_reg = value)
#define adxl_dev_get_thresh_inact(ptr) ((ptr)->thresh_inact_reg)

#define adxl_dev_set_time_inact(ptr, value) ((ptr)->time_inact_reg = value)
#define adxl_dev_get_time_inact(ptr) ((ptr)->time_inact_reg)

#define adxl_dev_set_act_inact_ctl(ptr, value) ((ptr)->act_inact_ctl_reg = value)
#define adxl_dev_get_act_inact_ctl(ptr) ((ptr)->act_inact_ctl_reg)

#define adxl_dev_set_thresh_ff(ptr, value) ((ptr)->thresh_ff_reg = value)
#define adxl_dev_get_thresh_ff(ptr) ((ptr)->thresh_ff_reg)

#define adxl_dev_set_time_ff(ptr, value) ((ptr)->time_ff_reg = value)
#define adxl_dev_get_time_ff(ptr) ((ptr)->time_ff_reg)

#define adxl_dev_set_tap_axes(ptr, value) ((ptr)->tap_axes_reg = value)
#define adxl_dev_get_tap_axes(ptr) ((ptr)->tap_axes_reg)

#define adxl_dev_get_act_tap_status(ptr) ((ptr)->act_tap_status_reg)

#define adxl_dev_set_bw_rate(ptr, value) ((ptr)->bw_rate_reg = (uint8_t)value)
#define adxl_dev_get_bw_rate(ptr) ((ptr)->bw_rate_reg)

#define adxl_dev_set_power_ctl(ptr, value) ((ptr)->power_ctl_reg = (uint8_t)value)
#define adxl_dev_get_power_ctl(ptr) ((ptr)->power_ctl_reg)

#define adxl_dev_set_int_enable(ptr, value) ((ptr)->int_enable_reg = value)
#define adxl_dev_get_int_enable(ptr) ((ptr)->int_enable_reg)

#define adxl_dev_set_int_map(ptr, value) ((ptr)->int_map_reg = value)
#define adxl_dev_get_int_map(ptr) ((ptr)->int_map_reg)

#define adxl_dev_get_int_source(ptr) ((ptr)->int_source_reg)

#define adxl_dev_set_data_format(ptr, value) ((ptr)->data_format_reg = (uint8_t)value)
#define adxl_dev_get_data_format(ptr) ((ptr)->data_format_reg)

#define adxl_dev_get_datax0(ptr) ((ptr)->datax0_reg)
#define adxl_dev_get_datax1(ptr) ((ptr)->datax1_reg)
#define adxl_dev_get_datay0(ptr) ((ptr)->datay0_reg)
#define adxl_dev_get_datay1(ptr) ((ptr)->datay1_reg)
#define adxl_dev_get_dataz0(ptr) ((ptr)->dataz0_reg)
#define adxl_dev_get_dataz1(ptr) ((ptr)->dataz1_reg)

#define adxl_dev_set_fifo_ctl(ptr, value) ((ptr)->fifo_ctl_reg = value)
#define adxl_dev_get_fifo_ctl(ptr) ((ptr)->fifo_ctl_reg)

#define adxl_dev_get_fifo_status(ptr) ((ptr)->fifo_status_reg)


