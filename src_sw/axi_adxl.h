#pragma once

#include "adxl_cfg.h"
#include "adxl_dev.h"
#include <math.h>
#include "text_color.h"


//#define AXI_ADXL_LOGGING_CFG
//#define AXI_ADXL_LOGGING_DEV

#define ADXL_OK 0
#define ADXL_UNINIT -1
#define ADXL_LINK_LOST -2
#define ADXL_RESET_INFINITE -3
#define ADXL_NO_COMPLETE_SINGLE_REQUEST -4
#define ADXL_CANNOT_STOP -5
#define ADXL_UNCORRECT_VALUE -6
#define ADXL_TIMEOUT -7
#define ADXL_NON_FUNCTION -8

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

#define FUNCTIONS_COUNT   89

static int function_index_list[FUNCTIONS_COUNT] = {
      0,   1,   2,   3,   4,   5,   6,   7,   8,
     11,  15,  17,  24, 290, 291, 300, 301, 310,
    311, 320, 321, 322, 330, 331, 340, 341, 350,
    351, 360, 361, 370, 371, 380, 381, 390, 391,
    392, 393, 394, 395, 396, 397, 400, 401, 410,
    411, 420, 421, 430, 431, 432, 440, 441, 450,
    451, 452, 453, 454, 455, 456, 457, 458, 459,
    461, 460, 470, 471, 480, 490, 491, 492, 493,
    494, 495, 496, 497, 498, 500, 501, 560, 561,
    562, 563, 564, 565, 570, 571, 100, 120
};

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
    ACT_Z_MASK      = 0x10,
};

#define ACT_AC_MASK 	0x80
#define INACT_AC_MASK 	0x08

enum acdc_enum {
    DC_MODE = 0x00,
    AC_MODE = 0x01
};

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


enum int_map_enum {
    INT0 = 0x00,
    INT1 = 0x01
};

enum spi_enum {
    SPI_3_WIRE = 0x40,
    SPI_4_WIRE = 0x00
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

#define axi_adxl_has_init(ptr) (ptr->init_flaq == 1) ? TRUE : FALSE

#define axi_adxl_get_datax(ptr) (int16_t)(((uint16_t)adxl_dev_get_datax1((ptr)->dev)<<8) + ((uint16_t)adxl_dev_get_datax0((ptr)->dev)))
#define axi_adxl_get_datay(ptr) (int16_t)(((uint16_t)adxl_dev_get_datay1((ptr)->dev)<<8) + ((uint16_t)adxl_dev_get_datay0((ptr)->dev)))
#define axi_adxl_get_dataz(ptr) (int16_t)(((uint16_t)adxl_dev_get_dataz1((ptr)->dev)<<8) + ((uint16_t)adxl_dev_get_dataz0((ptr)->dev)))


void axi_adxl_dev_debug_register_space(adxl_dev *ptr);

int axi_adxl_init(axi_adxl *ptr, uint32_t baseaddr_cfg, uint32_t baseaddr_dev, uint8_t iic_address);

int axi_adxl_reset(axi_adxl *ptr);
int axi_adxl_has_reset(axi_adxl* ptr);

int axi_adxl_enable_interval_requestion(axi_adxl *ptr);
int axi_adxl_disable_interval_requestion(axi_adxl *ptr);
int axi_adxl_has_interval_requestion(axi_adxl* ptr);
int axi_adxl_enable_single_request(axi_adxl *ptr);
int axi_adxl_has_single_request_completed(axi_adxl* ptr);
int axi_adxl_switch_irq_allow(axi_adxl *ptr);
int axi_adxl_has_irq_allow(axi_adxl* ptr);
int axi_adxl_irq_ack(axi_adxl *ptr);
int axi_adxl_calibration_start(axi_adxl *ptr);
int axi_adxl_set_iic_address(axi_adxl *ptr, uint8_t iic_address);
int axi_adxl_get_iic_address(axi_adxl* ptr, uint8_t* iic_address);
int axi_adxl_get_requestion_interval(axi_adxl* ptr, uint32_t* requestion_interval);
int axi_adxl_set_requestion_interval(axi_adxl* ptr, uint32_t requestion_interval);

int axi_adxl_set_start_address(axi_adxl* ptr, uint8_t start_address);
int axi_adxl_get_start_address(axi_adxl* ptr, uint8_t *start_address);
int axi_adxl_set_size(axi_adxl* ptr, uint8_t size);
int axi_adxl_get_size(axi_adxl* ptr, uint8_t* size);
int axi_adxl_has_work(axi_adxl* ptr);
int axi_adxl_has_link(axi_adxl* ptr);

int axi_adxl_get_version_major(axi_adxl* ptr, uint8_t* major);
int axi_adxl_get_version_minor(axi_adxl* ptr, uint8_t* minor);


int axi_adxl_get_calibration_pwr_count_limit(axi_adxl* ptr, uint8_t* pwr_count_limit);
int axi_adxl_set_calibration_pwr_count_limit(axi_adxl* ptr, uint8_t pwr_count_limit);
int axi_adxl_has_calibration_in_progress(axi_adxl *ptr);
int axi_adxl_has_calibration_complete(axi_adxl* ptr);

int axi_adxl_get_read_valid_count(axi_adxl* ptr, uint32_t* read_valid_count);
int axi_adxl_get_write_valid_count(axi_adxl* ptr, uint32_t* write_valid_count);
int axi_adxl_get_read_transactions(axi_adxl* ptr, uint64_t* read_transactions);
int axi_adxl_get_write_transactions(axi_adxl* ptr, uint64_t* write_transactions);
int axi_adxl_get_clk_period(axi_adxl* ptr, uint32_t* clk_period);
int axi_adxl_get_opt_request_interval(axi_adxl* ptr, uint64_t* opt_request_interval);
int axi_adxl_get_calibration_time(axi_adxl* ptr, uint64_t* calibration_time);
int axi_adxl_get_data_width(axi_adxl* ptr, uint32_t* data_width);



/*Checking register space*/
int check_access_rw(int address);
int check_access_ro(int address);
int check_access_reserved(int address);

/*Function ID converter to Function list*/
int extract_function_list_address(int data);




/*Device api functions*/
// 0x1D
int axi_adxl_set_thresh_tap(axi_adxl* ptr, uint8_t thresh_tap);
int axi_adxl_get_thresh_tap(axi_adxl* ptr, uint8_t *thresh_tap);
// 0x1E
int axi_adxl_get_ofsx(axi_adxl* ptr, int8_t* ofsx);
int axi_adxl_get_ofsy(axi_adxl* ptr, int8_t* ofsy);
// 0x1F
int axi_adxl_get_ofsz(axi_adxl* ptr, int8_t* ofsz);
int axi_adxl_set_ofsx(axi_adxl* ptr, int8_t ofsx);
// 0x20
int axi_adxl_set_ofsy(axi_adxl* ptr, int8_t ofsy);
int axi_adxl_set_ofsz(axi_adxl* ptr, int8_t ofsz);
// 0x21
int axi_adxl_set_dur(axi_adxl* ptr, uint8_t duration);
int axi_adxl_get_dur(axi_adxl* ptr, uint8_t *duration);
// 0x22
int axi_adxl_set_latent(axi_adxl *ptr, uint8_t latent);
int axi_adxl_get_latent(axi_adxl *ptr, uint8_t *latent);
// 0x23
int axi_adxl_set_window(axi_adxl *ptr, uint8_t window);
int axi_adxl_get_window(axi_adxl *ptr, uint8_t *window);
// 0x24
int axi_adxl_set_thresh_act(axi_adxl *ptr, uint8_t thresh_act);
int axi_adxl_get_thresh_act(axi_adxl *ptr, uint8_t *thresh_act);
// 0x25
int axi_adxl_set_thresh_inact(axi_adxl *ptr, uint8_t thresh_inact);
int axi_adxl_get_thresh_inact(axi_adxl *ptr, uint8_t *thresh_inact);
// 0x26
int axi_adxl_set_time_inact(axi_adxl *ptr, uint8_t time_inact);
int axi_adxl_get_time_inact(axi_adxl *ptr, uint8_t *time_inact);
// 0x27 - BIT 7
int axi_adxl_set_activity_acdc(axi_adxl* ptr, enum acdc_enum acdc_mode);
int axi_adxl_get_activity_acdc(axi_adxl* ptr, enum acdc_enum *acdc_mode);
int axi_adxl_has_activity_ac(axi_adxl* ptr);
int axi_adxl_has_activity_dc(axi_adxl* ptr);
// 0x27 - BIT 3
int axi_adxl_set_inactivity_acdc(axi_adxl* ptr, enum acdc_enum acdc_mode);
int axi_adxl_get_inactivity_acdc(axi_adxl* ptr, enum acdc_enum *acdc_mode);
int axi_adxl_has_inactivity_ac(axi_adxl* ptr);
int axi_adxl_has_inactivity_dc(axi_adxl* ptr);
// 0x27 - BIT [6:4][2:0]
int axi_adxl_is_act_inact_control(axi_adxl* ptr, uint8_t mask);
// 0x27 - BIT [6:4]
int axi_adxl_enable_activity_control(axi_adxl* ptr, enum act_enum act_mask);
int axi_adxl_disable_activity_control(axi_adxl* ptr, enum act_enum act_mask);
// 0x27 - BIT [2:0]
int axi_adxl_enable_inactivity_control(axi_adxl* ptr, enum inact_enum inact_mask);
int axi_adxl_disable_inactivity_control(axi_adxl* ptr, enum inact_enum inact_mask);
// 0x28 
int axi_adxl_set_thresh_ff(axi_adxl* ptr, uint8_t thresh_ff);
int axi_adxl_get_thresh_ff(axi_adxl* ptr, uint8_t *thresh_ff);
// 0x29
int axi_adxl_set_time_ff(axi_adxl* ptr, uint8_t time_ff);
int axi_adxl_get_time_ff(axi_adxl* ptr, uint8_t *time_ff);
// 0x2A 
int axi_adxl_switch_tap_axes(axi_adxl* ptr, uint8_t mask);
int axi_adxl_has_tap_axes(axi_adxl* ptr, uint8_t mask);
// 0x2B
int axi_adxl_is_act_src_status(axi_adxl* ptr, enum act_tap_status_enum act);
int axi_adxl_is_tap_src_status(axi_adxl* ptr, enum act_tap_status_enum tap);
int axi_adxl_has_asleep_status(axi_adxl* ptr);
// 0x2C
int axi_adxl_set_bw_rate(axi_adxl *ptr, enum bw_rate_enum bw_rate);
int axi_adxl_get_bw_rate(axi_adxl *ptr, enum bw_rate_enum *bw_rate);
// 0x2D - [BIT 5]
int axi_adxl_enable_linking_mode(axi_adxl* ptr);
int axi_adxl_disable_linking_mode(axi_adxl* ptr);
int axi_adxl_has_linking_mode(axi_adxl* ptr);
// 0x2D - [BIT 4]
int axi_adxl_enable_autosleep_mode(axi_adxl* ptr);
int axi_adxl_disable_autosleep_mode(axi_adxl* ptr);
int axi_adxl_has_autosleep_mode(axi_adxl* ptr);
// 0x2D - [BIT 3]
int axi_adxl_enable_msmt(axi_adxl *ptr);
int axi_adxl_disable_msmt(axi_adxl *ptr);
int axi_adxl_has_msmt(axi_adxl* ptr);
// 0x2D - [BIT 2]
int axi_adxl_enable_sleep_mode(axi_adxl* ptr);
int axi_adxl_disable_sleep_mode(axi_adxl* ptr);
int axi_adxl_has_sleep_mode(axi_adxl* ptr);
// 0x2D - [BIT1:0]
int axi_adxl_set_wakeup(axi_adxl* ptr, enum wakeup_enum wakeup);
int axi_adxl_get_wakeup(axi_adxl* ptr, enum wakeup_enum *wakeup);
int axi_adxl_is_wakeup(axi_adxl* ptr, enum wakeup_enum wakeup);
// 0x2E
int axi_adxl_enable_int_enable(axi_adxl* ptr, enum int_mask_enum intr);
int axi_adxl_disable_int_enable(axi_adxl* ptr, enum int_mask_enum intr);
int axi_adxl_is_int_enable(axi_adxl* ptr, enum int_mask_enum intr);
// 0x2F
int axi_adxl_set_int_map(axi_adxl *ptr, enum int_mask_enum intr_mask, enum int_map_enum int_map);
int axi_adxl_is_int_map_intr0(axi_adxl* ptr, enum int_mask_enum intr_mask);
int axi_adxl_is_int_map_intr1(axi_adxl* ptr, enum int_mask_enum intr_mask);
// 0x30 
int axi_adxl_get_int_source(axi_adxl* ptr, uint8_t* interrupt_mask);
int axi_adxl_is_int_source(axi_adxl* ptr, enum int_mask_enum interrupt);
// 0x31 [BIT 7]
int axi_adxl_enable_selftest(axi_adxl *ptr);
int axi_adxl_disable_selftest(axi_adxl* ptr);
int axi_adxl_has_selftest(axi_adxl* ptr);
// 0x31 [BIT 6]
int axi_adxl_set_spi_mode(axi_adxl* ptr, enum spi_enum spi_mode);
int axi_adxl_has_spi_3_wire(axi_adxl* ptr);
int axi_adxl_has_spi_4_wire(axi_adxl* ptr);
// 0x31 [BIT 5]
int axi_adxl_enable_int_invert(axi_adxl* ptr);
int axi_adxl_disable_int_invert(axi_adxl* ptr);
int axi_adxl_has_int_invert(axi_adxl* ptr);
// 0x31 [BIT 2]
int axi_adxl_set_justify_msb(axi_adxl* ptr);
int axi_adxl_set_justify_lsb(axi_adxl* ptr);
int axi_adxl_has_justify_msb(axi_adxl* ptr);
int axi_adxl_has_justify_lsb(axi_adxl* ptr);
// 0x31 [BIT 3, 1:0]
int axi_adxl_set_range(axi_adxl* ptr, enum range_enum range);
int axi_adxl_get_range(axi_adxl* ptr, enum range_enum* range);
int axi_adxl_is_range(axi_adxl* ptr, enum range_enum range);
// 0x38 [BIT 7:6]
int axi_adxl_set_fifo_mode(axi_adxl* ptr, enum fifo_mode_enum fifo_mode);
int axi_adxl_is_fifo_mode(axi_adxl* ptr, enum fifo_mode_enum fifo_mode);
// 0x38 [BIT 5]
int axi_adxl_enable_trigger(axi_adxl *ptr);
int axi_adxl_disable_trigger(axi_adxl *ptr);
int axi_adxl_has_trigger(axi_adxl *ptr);
// 0x38 [BIT 4:0]
int axi_adxl_set_samples(axi_adxl *ptr, uint8_t samples);
int axi_adxl_get_samples(axi_adxl *ptr, uint8_t *samples);
// 0x39 [BIT 5:0]
int axi_adxl_get_fifo_sts_entries(axi_adxl *ptr, uint8_t *entries);
// 0x39 [BIT 7]
int axi_adxl_has_fifo_sts_trigger(axi_adxl *ptr);


/////////////////////////////////////////////////////////////////////////////////////////////////////



int axi_adxl_get_data(axi_adxl *ptr, adxl_data *data);
int axi_adxl_get_data_float(axi_adxl *ptr, adxl_data_float *data_float);

