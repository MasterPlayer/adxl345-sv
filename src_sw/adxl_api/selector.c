#include "selector.h"



// 00 
int selector_axi_adxl_cfg_debug(axi_adxl *ptr){
    axi_adxl_cfg_debug(ptr->cfg);
    return ADXL_OK;
}



// 01 
int selector_axi_adxl_reset(axi_adxl *ptr){
    printf("[MENU] : reset component selected\r\n");
    int status = axi_adxl_reset(ptr);
    return status;
}



// 02
int selector_axi_adxl_init(axi_adxl *ptr){
    printf("[MENU] : initialization of component selected\r\n");
    int status = axi_adxl_init(ptr, ADXL_CFG_BASEADDRESS, ADXL_DEV_BASEADDRESS, ADXL_IIC_ADDRESS);
    return status;
}



// 03
int selector_axi_adxl_perform_single_request(axi_adxl *ptr){

    printf("[MENU] : single request by component selected\r\n");

    char str [256];
    char *str_ptr = str;

    printf("[MENU] : Enter start address : ");
    while((*str_ptr++=getchar ()) != 13);
    *str_ptr = '\0';
    uint8_t address = atoi(str);
    printf("%d\r\n", address);

    str_ptr = &str[0];

    printf("[MENU] : Enter size : ");
    while((*str_ptr++=getchar ()) != 13);
    *str_ptr = '\0';
    uint8_t size = atoi(str);
    printf("%d\r\n", size);

    int status = axi_adxl_perform_single_request(ptr, address, size);
    return status;
}


//04
int selector_axi_adxl_perform_interval_requestion(axi_adxl *ptr){

    printf("[MENU] : enable interval requestion by component selected\r\n");

    char str [256];
    char *str_ptr = str;

    printf("[MENU] : Enter bandwidth value : ");

    while((*str_ptr++=getchar ()) != 13);
    *str_ptr = '\0';
    uint8_t value = atoi(str);
    printf("%d\r\n", value);

    int status = axi_adxl_perform_interval_requestion(ptr, value);

    return status;
}


//05
int selector_axi_adxl_disable_interval_requestion(axi_adxl *ptr){

    printf("[MENU] : disable interval requestion by component selected\r\n");

    int status = axi_adxl_disable_interval_requestion(ptr);
    return status;
}


//06
int selector_axi_adxl_irq_allow(axi_adxl *ptr){

    printf("[MENU] : allow irq by component selected\r\n");

    int status = axi_adxl_irq_allow(ptr);
    return status;
}



//07
int selector_axi_adxl_irq_unallow(axi_adxl *ptr){

    printf("[MENU] : unallow irq by component selected\r\n");

    int status = axi_adxl_irq_unallow(ptr);
    return status;
}



//08
int selector_axi_adxl_calibration(axi_adxl *ptr){

    printf("[MENU] : calibration selected\r\n");

    char str [256];
    char *str_ptr = str;

    printf("[MENU] : Enter power of 2 for calibration: ");

    while((*str_ptr++=getchar ()) != 13);
    *str_ptr = '\0';
    uint8_t value = atoi(str);
    printf("%d(calibration count : %d)\r\n", value, (1<<value));

    int status = axi_adxl_calibration(ptr, value);

    return status;
}



//09
int selector_axi_adxl_set_iic_address(axi_adxl *ptr){

    printf("[MENU] : setup new address for I2C device selected\r\n");

    char str [256];
    char *str_ptr = str;
    printf("[MENU] : Enter new address of i2c device in decimal: ");
    while((*str_ptr++=getchar ()) != 13);
    *str_ptr = '\0';
    uint8_t value = atoi(str);
    printf("%d (hex address : 0x%02x)\r\n", value, value);
    int status = axi_adxl_set_iic_address(ptr, value);
    return status;
}


//50
int selector_axi_adxl_set_bw_rate(axi_adxl *ptr){

    printf("[MENU] : setup BW_RATE for device selected\r\n");

    char str [256];
    char *str_ptr = str;

    int status = ADXL_OK;

    printf("\t0. 0.10 Hz \r\n");
    printf("\t1. 0.20 Hz \r\n");
    printf("\t2. 0.39 Hz \r\n");
    printf("\t3. 0.78 Hz \r\n");
    printf("\t4. 1.56 Hz \r\n");
    printf("\t5. 3.13 Hz \r\n");
    printf("\t6. 6.25 Hz \r\n");
    printf("\t7. 12.5 Hz \r\n");
    printf("\t8. 25 Hz \r\n");
    printf("\t9. 50 Hz \r\n");
    printf("\t10. 100 Hz \r\n");
    printf("\t11. 200 Hz \r\n");
    printf("\t12. 400 Hz \r\n");
    printf("\t13. 800 Hz \r\n");
    printf("\t14. 1600 Hz \r\n");
    printf("\t15. 3200 Hz \r\n");
    printf("\t16. 400 Hz low power\r\n");
    printf("\t17. 200 Hz low power\r\n");
    printf("\t18. 100 Hz low power\r\n");
    printf("\t19. 50 Hz low power\r\n");
    printf("\t20. 25 Hz low power\r\n");
    printf("\t21. 12_5 Hz low power\r\n");

    printf("Enter bandwidth value : ");
    while((*str_ptr++=getchar ()) != 13);
    *str_ptr = '\0';
    uint8_t value = atoi(str);
    printf("%d\r\n", value);
    switch (value){
        case  0 : status = axi_adxl_set_bw_rate(ptr, BW_RATE_0_10); break;
        case  1 : status = axi_adxl_set_bw_rate(ptr, BW_RATE_0_20); break;
        case  2 : status = axi_adxl_set_bw_rate(ptr, BW_RATE_0_39); break;
        case  3 : status = axi_adxl_set_bw_rate(ptr, BW_RATE_0_78); break;
        case  4 : status = axi_adxl_set_bw_rate(ptr, BW_RATE_1_56); break;
        case  5 : status = axi_adxl_set_bw_rate(ptr, BW_RATE_3_13); break;
        case  6 : status = axi_adxl_set_bw_rate(ptr, BW_RATE_6_25); break;
        case  7 : status = axi_adxl_set_bw_rate(ptr, BW_RATE_12_5); break;
        case  8 : status = axi_adxl_set_bw_rate(ptr, BW_RATE_25); break;
        case  9 : status = axi_adxl_set_bw_rate(ptr, BW_RATE_50); break;
        case 10 : status = axi_adxl_set_bw_rate(ptr, BW_RATE_100); break;
        case 11 : status = axi_adxl_set_bw_rate(ptr, BW_RATE_200); break;
        case 12 : status = axi_adxl_set_bw_rate(ptr, BW_RATE_400); break;
        case 13 : status = axi_adxl_set_bw_rate(ptr, BW_RATE_800); break;
        case 14 : status = axi_adxl_set_bw_rate(ptr, BW_RATE_1600); break;
        case 15 : status = axi_adxl_set_bw_rate(ptr, BW_RATE_3200); break;
        case 16 : status = axi_adxl_set_bw_rate(ptr, BW_RATE_400_LP); break;
        case 17 : status = axi_adxl_set_bw_rate(ptr, BW_RATE_200_LP); break;
        case 18 : status = axi_adxl_set_bw_rate(ptr, BW_RATE_100_LP); break;
        case 19 : status = axi_adxl_set_bw_rate(ptr, BW_RATE_50_LP); break;
        case 20 : status = axi_adxl_set_bw_rate(ptr, BW_RATE_25_LP); break;
        case 21 : status = axi_adxl_set_bw_rate(ptr, BW_RATE_12_5_LP); break;
       default : status = ADXL_UNCORRECT_VALUE;
    }
    return status;
}


//51
int selector_axi_adxl_measurement_start(axi_adxl *ptr){
 
    printf("[MENU] : start measure\r\n");
    int status = axi_adxl_measurement_start(ptr);
    return status;
}


//52
int selector_axi_adxl_measurement_stop(axi_adxl *ptr){
    printf("[MENU] : stop measure\r\n");
    int status = axi_adxl_measurement_stop(ptr);
    return status;
}


//53
int selector_axi_adxl_interrupt_enable(axi_adxl *ptr){
    char str [256];
    char *str_ptr = str;
    int value; 
    int status = ADXL_OK;
    printf("[MENU] : Selected which event for interrupt\r\n");
    if (axi_adxl_interrupt_enabled(ptr, DATA_READY)){
	    textcolor(DEFAULT, BLACK, GREEN);
    }else{
	    textcolor(DEFAULT, BLACK, RED);
    }
    printf("1. DATA_READY");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (axi_adxl_interrupt_enabled(ptr, SINGLE_TAP)){
	    textcolor(DEFAULT, BLACK, GREEN);
    }else{
	    textcolor(DEFAULT, BLACK, RED);
    }
    printf("2. SINGLE TAP");
    textcolor(DEFAULT, STD, STD);
	printf("\r\n");

	if (axi_adxl_interrupt_enabled(ptr, DOUBLE_TAP)){
	    textcolor(DEFAULT, BLACK, GREEN);
    }else{
	    textcolor(DEFAULT, BLACK, RED);
    }
    printf("3. DOUBLE TAP");
    textcolor(DEFAULT, STD, STD);
	printf("\r\n");

    if (axi_adxl_interrupt_enabled(ptr, ACTIVITY)){
	    textcolor(DEFAULT, BLACK, GREEN);
    }else{
	    textcolor(DEFAULT, BLACK, RED);
    }
    printf("4. ACTIVITY");
    textcolor(DEFAULT, STD, STD);
	printf("\r\n");

    if (axi_adxl_interrupt_enabled(ptr, INACTIVITY)){
	    textcolor(DEFAULT, BLACK, GREEN);
    }else{
	    textcolor(DEFAULT, BLACK, RED);
    }
    printf("5. INACTIVITY");
    textcolor(DEFAULT, STD, STD);
	printf("\r\n");

	if (axi_adxl_interrupt_enabled(ptr, FREE_FALL)){
	    textcolor(DEFAULT, BLACK, GREEN);
    }else{
	    textcolor(DEFAULT, BLACK, RED);
    }
    printf("6. FREE FALL");
    textcolor(DEFAULT, STD, STD);
	printf("\r\n");

    if (axi_adxl_interrupt_enabled(ptr, WATERMARK)){
	    textcolor(DEFAULT, BLACK, GREEN);
    }else{
	    textcolor(DEFAULT, BLACK, RED);
    }
	printf("7. WATERMARK");
    textcolor(DEFAULT, STD, STD);
	printf("\r\n");

    if (axi_adxl_interrupt_enabled(ptr, OVERRUN)){
	    textcolor(DEFAULT, BLACK, GREEN);
    }else{
	    textcolor(DEFAULT, BLACK, RED);
    }
	printf("8. OVERRUN");
    textcolor(DEFAULT, STD, STD);
	printf("\r\n");

    printf("Which interrupt will be enabled : ");
    while((*str_ptr++=getchar ()) != 13);
    *str_ptr = '\0';
    value = atoi(str);
    printf("%d\r\n", value);

    switch (value){
        case 1 : status = axi_adxl_interrupt_enable(ptr, DATA_READY); break;
        case 2 : status = axi_adxl_interrupt_enable(ptr, SINGLE_TAP); break;
        case 3 : status = axi_adxl_interrupt_enable(ptr, DOUBLE_TAP); break;
        case 4 : status = axi_adxl_interrupt_enable(ptr, ACTIVITY); break;
        case 5 : status = axi_adxl_interrupt_enable(ptr, INACTIVITY); break;
        case 6 : status = axi_adxl_interrupt_enable(ptr, FREE_FALL); break;
        case 7 : status = axi_adxl_interrupt_enable(ptr, WATERMARK); break;
        case 8 : status = axi_adxl_interrupt_enable(ptr, OVERRUN); break;
        default : status = ADXL_UNCORRECT_VALUE;
    }

    return status;
}


//54
int selector_axi_adxl_interrupt_disable(axi_adxl *ptr){
    char str [256];
    char *str_ptr = str;

    int status = ADXL_OK;
    printf("[MENU] : Selected which interrupt must be disabled\r\n");
    if (axi_adxl_interrupt_enabled(ptr, DATA_READY)){
  	    textcolor(DEFAULT, BLACK, GREEN);
      }else{
  	    textcolor(DEFAULT, BLACK, RED);
      }
      printf("1. DATA_READY");
      textcolor(DEFAULT, STD, STD);
      printf("\r\n");

      if (axi_adxl_interrupt_enabled(ptr, SINGLE_TAP)){
  	    textcolor(DEFAULT, BLACK, GREEN);
      }else{
  	    textcolor(DEFAULT, BLACK, RED);
      }
      printf("2. SINGLE TAP");
      textcolor(DEFAULT, STD, STD);
  	printf("\r\n");

  	if (axi_adxl_interrupt_enabled(ptr, DOUBLE_TAP)){
  	    textcolor(DEFAULT, BLACK, GREEN);
      }else{
  	    textcolor(DEFAULT, BLACK, RED);
      }
      printf("3. DOUBLE TAP");
      textcolor(DEFAULT, STD, STD);
  	printf("\r\n");

      if (axi_adxl_interrupt_enabled(ptr, ACTIVITY)){
  	    textcolor(DEFAULT, BLACK, GREEN);
      }else{
  	    textcolor(DEFAULT, BLACK, RED);
      }
      printf("4. ACTIVITY");
      textcolor(DEFAULT, STD, STD);
  	printf("\r\n");

      if (axi_adxl_interrupt_enabled(ptr, INACTIVITY)){
  	    textcolor(DEFAULT, BLACK, GREEN);
      }else{
  	    textcolor(DEFAULT, BLACK, RED);
      }
      printf("5. INACTIVITY");
      textcolor(DEFAULT, STD, STD);
  	printf("\r\n");

  	if (axi_adxl_interrupt_enabled(ptr, FREE_FALL)){
  	    textcolor(DEFAULT, BLACK, GREEN);
      }else{
  	    textcolor(DEFAULT, BLACK, RED);
      }
      printf("6. FREE FALL");
      textcolor(DEFAULT, STD, STD);
  	printf("\r\n");

      if (axi_adxl_interrupt_enabled(ptr, WATERMARK)){
  	    textcolor(DEFAULT, BLACK, GREEN);
      }else{
  	    textcolor(DEFAULT, BLACK, RED);
      }
  	printf("7. WATERMARK");
      textcolor(DEFAULT, STD, STD);
  	printf("\r\n");

      if (axi_adxl_interrupt_enabled(ptr, OVERRUN)){
  	    textcolor(DEFAULT, BLACK, GREEN);
      }else{
  	    textcolor(DEFAULT, BLACK, RED);
      }
  	printf("8. OVERRUN");
      textcolor(DEFAULT, STD, STD);
  	printf("\r\n");

    printf("Which interrupt will be disabled : ");
    while((*str_ptr++=getchar ()) != 13);
    *str_ptr = '\0';
    uint8_t value = atoi(str);
    printf("%d\r\n", value);

    switch (value){
        case 1 : status = axi_adxl_interrupt_disable(ptr, DATA_READY); break;
        case 2 : status = axi_adxl_interrupt_disable(ptr, SINGLE_TAP); break;
        case 3 : status = axi_adxl_interrupt_disable(ptr, DOUBLE_TAP); break;
        case 4 : status = axi_adxl_interrupt_disable(ptr, ACTIVITY); break;
        case 5 : status = axi_adxl_interrupt_disable(ptr, INACTIVITY); break;
        case 6 : status = axi_adxl_interrupt_disable(ptr, FREE_FALL); break;
        case 7 : status = axi_adxl_interrupt_disable(ptr, WATERMARK); break;
        case 8 : status = axi_adxl_interrupt_disable(ptr, OVERRUN); break;
        default : status = ADXL_UNCORRECT_VALUE;
    }

    return status;
}


//100
int selector_axi_adxl_dev_debug_register_space(axi_adxl *ptr){
    axi_adxl_dev_debug_register_space(ADXL_DEV_BASEADDRESS);
    return ADXL_OK;
}


int selector_axi_adxl_change_range(axi_adxl *ptr){


    char str [256];
    char *str_ptr = str;

    int status = ADXL_OK;
    printf("\t[MENU] : Selected change range of device measurement\r\n");

    if (axi_adxl_get_range(ptr) == RANGE_2G){
  	    textcolor(DEFAULT, BLACK, GREEN);
    }else{
  	    textcolor(DEFAULT, STD, STD);
    }
	printf("\t0. RANGE 2G");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");


	if (axi_adxl_get_range(ptr) == RANGE_4G){
		textcolor(DEFAULT, BLACK, GREEN);
	}else{
		textcolor(DEFAULT, STD, STD);
	}
    printf("\t1. RANGE 4G");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");


    if (axi_adxl_get_range(ptr) == RANGE_8G){
  	    textcolor(DEFAULT, BLACK, GREEN);
    }else{
  	    textcolor(DEFAULT, STD, STD);
    }
    printf("\t2. RANGE 8G");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");


    if (axi_adxl_get_range(ptr) == RANGE_16G){
  	    textcolor(DEFAULT, BLACK, GREEN);
    }else{
  	    textcolor(DEFAULT, STD, STD);
    }
    printf("\t3. RANGE 16");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");


    if (axi_adxl_get_range(ptr) == RANGE_2G_FULL){
  	    textcolor(DEFAULT, BLACK, GREEN);
    }else{
  	    textcolor(DEFAULT, STD, STD);
    }
    printf("\t4. RANGE 2G FULL");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");


    if (axi_adxl_get_range(ptr) == RANGE_4G_FULL){
  	    textcolor(DEFAULT, BLACK, GREEN);
    }else{
  	    textcolor(DEFAULT, STD, STD);
    }
    printf("\t5. RANGE 4G FULL");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");


    if (axi_adxl_get_range(ptr) == RANGE_8G_FULL){
  	    textcolor(DEFAULT, BLACK, GREEN);
    }else{
  	    textcolor(DEFAULT, STD, STD);
    }
    printf("\t6. RANGE 8G FULL");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (axi_adxl_get_range(ptr) == RANGE_16G_FULL){
  	    textcolor(DEFAULT, BLACK, GREEN);
    }else{
  	    textcolor(DEFAULT, STD, STD);
    }
    printf("\t7. RANGE 16G FULL");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");


    printf("Enter range value : ");
    while((*str_ptr++=getchar ()) != 13);
    *str_ptr = '\0';

    uint8_t value = atoi(str);
    
    printf("%d\r\n", value);
    
    switch (value){
        case 0 : status = axi_adxl_change_range(ptr, RANGE_2G); break;
        case 1 : status = axi_adxl_change_range(ptr, RANGE_4G); break;
        case 2 : status = axi_adxl_change_range(ptr, RANGE_8G); break;
        case 3 : status = axi_adxl_change_range(ptr, RANGE_16G); break;
        case 4 : status = axi_adxl_change_range(ptr, RANGE_2G_FULL); break;
        case 5 : status = axi_adxl_change_range(ptr, RANGE_4G_FULL); break;
        case 6 : status = axi_adxl_change_range(ptr, RANGE_8G_FULL); break;
        case 7 : status = axi_adxl_change_range(ptr, RANGE_16G_FULL); break;
        default : status = ADXL_UNCORRECT_VALUE; break;
    }
    return status;
}
