#include "selector.h"



// 00 
int selector_axi_adxl_cfg_debug(axi_adxl *ptr){
    if (axi_adxl_has_init(ptr)) {
        
        int status = 0;
        uint8_t version_major = 0;
        uint8_t version_minor = 0;
        uint8_t iic_address = 0;
        uint32_t request_interval = 0;
        uint32_t calibration_pwr_count_limit = 0;
        uint32_t read_valid_count = 0;
        uint32_t write_valid_count = 0;
        uint64_t read_transactions = 0;
        uint64_t write_transactions = 0;
        uint32_t clk_period = 0;
        uint64_t opt_request_interval = 0;
        uint64_t calibration_time = 0;
        uint32_t data_width = 0;
        uint8_t single_req_address = 0;
        uint8_t single_req_size = 0;

        status = axi_adxl_get_version_major(ptr, &version_major);
        if (status != ADXL_OK) {
            return status;
        }
        status = axi_adxl_get_version_minor(ptr, &version_minor);
        if (status != ADXL_OK) {
            return status;
        }
        status = axi_adxl_get_iic_address(ptr, &iic_address);
        if (status != ADXL_OK) {
            return status;
        }
        status = axi_adxl_get_requestion_interval(ptr, &request_interval);
        if (status != ADXL_OK) {
            return status;
        }
        status = axi_adxl_get_calibration_pwr_count_limit(ptr, &calibration_pwr_count_limit);
        if (status != ADXL_OK) {
            return status;
        }
        status = axi_adxl_get_read_valid_count(ptr, &read_valid_count);
        if (status != ADXL_OK) {
            return status;
        }
        status = axi_adxl_get_write_valid_count(ptr, &write_valid_count);
        if (status != ADXL_OK) {
            return status;
        } 
        status = axi_adxl_get_read_transactions(ptr, &read_transactions);
        if (status != ADXL_OK) {
            return status;
        }
        status = axi_adxl_get_write_transactions(ptr, &write_transactions);
        if (status != ADXL_OK) {
            return status;
        }
        status = axi_adxl_get_clk_period(ptr, &clk_period);
        if (status != ADXL_OK) {
            return status;
        }
        status = axi_adxl_get_opt_request_interval(ptr, &opt_request_interval);
        if (status != ADXL_OK) {
            return status;
        }
        status = axi_adxl_get_calibration_time(ptr, &calibration_time);
        if (status != ADXL_OK) {
            return status;
        }
        status = axi_adxl_get_data_width(ptr, &data_width);
        if (status != ADXL_OK) {
            return status;
        }
        status = axi_adxl_get_start_address(ptr, &single_req_address);
        if (status != ADXL_OK) {
            return status;
        }
        status = axi_adxl_get_size(ptr, &single_req_size);
        if (status != ADXL_OK) {
            return status;
        }

        printf("[CFG SPACE]\r\n");
        printf("\t[NAME] \t\t\t\t| [RAW]\t\t| [VALUE]\r\n");
        printf("\t[VERSION] \t\t\t| 0x%08x \t| %d.%d\r\n", ((((uint32_t)version_major) << 8) + version_minor), version_major, version_minor);

        printf("\t[RESET_COMPLETED] \t\t| 0x%08x \t| ", axi_adxl_has_reset(ptr));
        if (axi_adxl_has_reset(ptr)) {
            textcolor(DEFAULT, BLACK, RED);
            printf("no");
        }
        else {
            textcolor(DEFAULT, BLACK, GREEN);
            printf("yes");
        }
        textcolor(DEFAULT, STD, STD);
        printf("\r\n");

        printf("\t[INTERVAL_REQUESTS] \t\t| 0x%08x \t| ", axi_adxl_has_interval_requestion(ptr));
        if (axi_adxl_has_interval_requestion(ptr)) {
            textcolor(DEFAULT, BLACK, GREEN);
            printf("actived");
        }
        else {
            textcolor(DEFAULT, RED, STD);
            printf("inactived");
        }
        textcolor(DEFAULT, STD, STD);
        printf("\r\n");

        printf("\t[IRQ_ALLOW] \t\t\t| 0x%08x \t| ", axi_adxl_has_irq_allow(ptr));
        if (axi_adxl_has_irq_allow(ptr)) {
            textcolor(DEFAULT, BLACK, GREEN);
            printf("yes");
        }
        else {
            textcolor(DEFAULT, RED, STD);
            printf("no");
        }
        textcolor(DEFAULT, STD, STD);
        printf("\r\n");

        printf("\t[CALIBRATION] \t\t\t| 0x%08x \t| ", axi_adxl_has_calibration_complete(ptr));
        if (axi_adxl_has_calibration_complete(ptr)) {
            textcolor(DEFAULT, BLACK, GREEN);
            printf("completed");
        }
        else {
            if (axi_adxl_has_calibration_in_progress(ptr)) {
                textcolor(DEFAULT, BLACK, RED);
                printf("in progress");
            }
            else {
                textcolor(DEFAULT, RED, STD);
                printf("not started");
            }
        }
        textcolor(DEFAULT, STD, STD);
        printf("\r\n");

        printf("\t[I2C ADDRESS] \t\t\t| 0x%08x \t| 0x%02x\r\n", iic_address, iic_address);

        printf("\t[LINK] \t\t\t\t| 0x%08x \t| ", axi_adxl_has_link(ptr));
        if (axi_adxl_has_link(ptr)) {
            textcolor(DEFAULT, BLACK, GREEN);
            printf("established");
        }
        else {
            textcolor(DEFAULT, BLACK, RED);
            printf("not founded");
        }
        textcolor(DEFAULT, STD, STD);
        printf("\r\n");

        printf("\t[WORK] \t\t\t\t| 0x%08x \t| ", axi_adxl_has_work(ptr));
        if (axi_adxl_has_work(ptr)) {
            textcolor(DEFAULT, GREEN, STD);
            printf("in progress");
        }
        else {
            textcolor(DEFAULT, RED, STD);
            printf("not operated");
        }
        textcolor(DEFAULT, STD, STD);
        printf("\r\n");

        printf("\t[REQUEST_INTERVAL] \t\t| 0x%08x \t| %10.3f nanoseconds\r\n", request_interval, ((float)request_interval * (1 / (float)clk_period)) * 1000000000);
        printf("\t[CALIBRATION_COUNT] \t\t| 0x%08x \t| %d times\r\n", calibration_pwr_count_limit, (1 << calibration_pwr_count_limit));
        printf("\t[READ_VALID_COUNT] \t\t| 0x%08x \t| %u bytes/s\r\n", read_valid_count, read_valid_count);
        printf("\t[WRITE_VALID_COUNT] \t\t| 0x%08x \t| %u bytes/s\r\n", write_valid_count, write_valid_count);
        printf("\t[READ_TRANSACTIONS] \t\t| 0x%08x \t| %u times\r\n", read_transactions, read_transactions);
        printf("\t[CLK_PERIOD] \t\t\t| 0x%08x \t| %4.6f MHz/%1.3f nanoseconds\r\n", clk_period, (float)clk_period / 1000000, (1 / (float)clk_period) * 1000000000);
        printf("\t[OPT_REQUEST_INTERVAL] \t\t| 0x%08x \t| %3.9f seconds\r\n", opt_request_interval, ((double)opt_request_interval / (double)clk_period));
        printf("\t[DATA_WIDTH] \t\t\t| 0x%08x \t| %d bits\r\n", data_width, data_width);
        printf("\t[CALIBRATION_TIME] \t\t| 0x%08x \t| %3.9f seconds\r\n", calibration_time, ((double)calibration_time / (double)clk_period));
        printf("\t[WRITE_TRANSACTIONS] \t\t| 0x%08x \t| %d times \r\n", write_transactions, write_transactions);
        printf("\t[SINGLE_REQ_ADDRESS] \t\t| 0x%08x \t| %d \r\n", single_req_address, single_req_address);
        printf("\t[SINGLE_REQ_SIZE] \t\t| 0x%08x \t| %d bytes \r\n", single_req_size, single_req_size);

        return status;

    }
    else {
        return ADXL_UNINIT;
    }
}



// 01 
int selector_axi_adxl_reset(axi_adxl *ptr){
    printf("\t[MENU] : reset component selected\r\n");
    return axi_adxl_reset(ptr);
    
}


int selector_axi_adxl_has_reset(axi_adxl* ptr) {
    printf("\t[MENU] : reset state : ");

        if (axi_adxl_has_reset(ptr)) {
            textcolor(DEFAULT, BLACK, RED);
            printf("active");
        }
        else {
            textcolor(DEFAULT, BLACK, GREEN);
            printf("inactive");
        }
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");
    return ADXL_OK;
}



int selector_axi_adxl_has_single_request_completed(axi_adxl* ptr) {
    printf("\t[MENU] : single request : ");
    if (axi_adxl_has_single_request_completed(ptr)) {
        textcolor(DEFAULT, BLACK, GREEN);
        printf("completed");
    }
    else {
        textcolor(DEFAULT, BLACK, RED);
        printf("not completed");
    }
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");
    return ADXL_OK;

}




// 02
int selector_axi_adxl_init(axi_adxl *ptr){
    printf("\t[MENU] : initialization of component selected\r\n");
    return axi_adxl_init(ptr, ADXL_CFG_BASEADDRESS, ADXL_DEV_BASEADDRESS, ADXL_IIC_ADDRESS);
}



// 03
int selector_axi_adxl_perform_single_request(axi_adxl *ptr){

    int address = 0x00;
    int size = 0x00;

    printf("\t[MENU] : selected perform single request\r\n");
    printf("\t[MENU] : Enter start address : ");
    scanf_s("%d", &address);
    printf("%d\r\n", address);
    printf("\t[MENU] : Enter size : ");
    scanf_s("%d", &size);
    printf("%d\r\n", size);

    if (size < 1 || size > 58) {
        textcolor(DEFAULT, RED, STD);
    	printf("\t[MENU] : incorrect <size> value: ");
        textcolor(DEFAULT, BLACK, RED);
    	printf("%d", size);
        textcolor(DEFAULT, STD, STD);
        printf("\r\n");
    	return ADXL_UNCORRECT_VALUE;
    }

    if (address < 0 || address > 57) {
    	textcolor(DEFAULT, RED, STD);
    	printf("\t[MENU] : incorrect <address> value: ");
    	textcolor(DEFAULT, BLACK, RED);
    	printf("%d", address);
    	textcolor(DEFAULT, STD, STD);
    	printf("\r\n");
    	return ADXL_UNCORRECT_VALUE;
    }

    if ((address + size) > 58) {
    	textcolor(DEFAULT, RED, STD);
    	printf("\t[MENU] : current address and size greater than 58 bytes: ");
    	textcolor(DEFAULT, BLACK, RED);
    	printf("addr %d size %d", address, size);
    	textcolor(DEFAULT, STD, STD);
    	printf("\r\n");
    	return ADXL_UNCORRECT_VALUE;
    }

    int status = axi_adxl_set_start_address(ptr, address);
    if (status != ADXL_OK) {
        return status;
    }
    
    status = axi_adxl_set_size(ptr, size);
    if (status != ADXL_OK) {
        return status;
    }

    return (axi_adxl_enable_single_request(ptr));
}




int selector_axi_adxl_set_start_address(axi_adxl* ptr) {
    printf("\t[MENU] : selected set of start address for single request\r\n");
    printf("\t[MENU] : Enter startaddr : ");
    
    int value;

    scanf_s("%d", &value);
    printf("%d\r\n", value);
    return axi_adxl_set_start_address(ptr, value);
}



int selector_axi_adxl_get_start_address(axi_adxl* ptr) {
    printf("\t[MENU] : selected getting start address\r\n");
    uint32_t start_address = 0;
    int status = axi_adxl_get_start_address(ptr, &start_address);
    if (status == ADXL_OK)
        printf("\t[MENU] : Start address = 0x%02x\r\n", start_address);
    return status;

}



int selector_axi_adxl_set_size(axi_adxl* ptr) {
    printf("\t[MENU] : selected set of size for single request\r\n");
    printf("\t[MENU] : Enter size : ");

    int value;

    scanf_s("%d", &value);
    printf("%d\r\n", value);
    return axi_adxl_set_size(ptr, value);
}



int selector_axi_adxl_get_size(axi_adxl* ptr) {
    printf("\t[MENU] : selected getting size\r\n");
    uint32_t size = 0;
    int status = axi_adxl_get_size(ptr, &size);
    if (status == ADXL_OK)
        printf("\t[MENU] : size = %d bytes\r\n", size);
    return status;

}


//04
int selector_axi_adxl_perform_interval_requestion(axi_adxl *ptr){

    printf("\t[MENU] : selected enable interval requestion\r\n");
    printf("\t[MENU] : Enter bandwidth value : ");
    int value;

    scanf_s("%d", &value);
    printf("%d\r\n", value);
    int status = axi_adxl_set_requestion_interval(ptr, value);
    if (status != ADXL_OK) {
        return status;
    }

    return axi_adxl_enable_interval_requestion(ptr);

}


//05
int selector_axi_adxl_disable_interval_requestion(axi_adxl *ptr){

    printf("\t[MENU] : selected disable interval requestion\r\n");

    return axi_adxl_disable_interval_requestion(ptr);

}


//06
int selector_axi_adxl_switch_irq_allow(axi_adxl *ptr){
    printf("\t[MENU] : selected switching allow irq\r\n");

    printf("\t[MENU] : current state of allow_irq : ");
    if (axi_adxl_has_irq_allow(ptr)) {
        textcolor(DEFAULT, BLACK, GREEN);
        printf("active");
    }
    else {
        textcolor(DEFAULT, BLACK, RED);
        printf("inactive");
    }
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    return axi_adxl_switch_irq_allow(ptr);
}




//08
int selector_axi_adxl_calibration(axi_adxl *ptr){

    printf("\t[MENU] : calibration selected\r\n");
    printf("\t[MENU] : Enter power of 2 for calibration: ");

    int calibration_power;
    scanf_s("%d", &calibration_power);
    printf("%d(calibration count : %d)\r\n", calibration_power, (1<< calibration_power));

    int status = axi_adxl_set_calibration_pwr_count_limit(ptr, calibration_power);
    if (status != ADXL_OK) {
        return status;
    }

    return axi_adxl_calibration_start(ptr);
}



//09
int selector_axi_adxl_set_iic_address(axi_adxl *ptr){

    printf("\t[MENU] : setup new address for I2C device selected\r\n");
    printf("\t[MENU] : Enter new address of i2c device in decimal: ");
    int iic_address;
    scanf_s("%d", &iic_address);
    printf("%d (hex address : 0x%02x)\r\n", iic_address, iic_address);
    return axi_adxl_set_iic_address(ptr, iic_address);
}



int selector_axi_adxl_get_iic_address(axi_adxl* ptr) {
    printf("\t[MENU] : get address of iic\r\n");
    uint8_t iic_address = 0;
    int status = axi_adxl_get_iic_address(ptr, &iic_address);
    if (status == ADXL_OK)
        printf("\t[MENU] : IIC ADDRESS = 0x%02x\r\n", iic_address);
    return status;
    
}



int selector_axi_adxl_get_version(axi_adxl* ptr) {
    printf("\t[MENU] : current version : ");
    uint8_t major;
    uint8_t minor;

    int status = axi_adxl_get_version_major(ptr, &major);
    if (status != ADXL_OK) {
        return status;
    }
    status = axi_adxl_get_version_minor(ptr, &minor);
    if (status != ADXL_OK) {
        return status;
    }
    printf("%d.%d\r\n", major, minor);
    return status;

}


//10
int selector_axi_adxl_get_requestion_interval(axi_adxl* ptr) {
    printf("\t[MENU] : selected getting requestion interval\r\n");
    uint32_t requestion_interval;
    int status = axi_adxl_get_requestion_interval(ptr, &requestion_interval);
    if (status == ADXL_OK)
        printf("\t[MENU] : Requestion interval = %d clock periods\r\n", requestion_interval);
    return status;
}


int selector_axi_adxl_set_requestion_interval(axi_adxl* ptr) {
    printf("\t[MENU] : selected changing requestion interval\r\n");
    int requestion_interval = 0;
    printf("\t[MENU] : enter new requestion interval : ");
    scanf_s("%d", &requestion_interval);
    printf("%d\r\n", requestion_interval);
    return axi_adxl_set_requestion_interval(ptr, requestion_interval);
}


int selector_axi_adxl_has_irq_allow(axi_adxl* ptr) {
    printf("\t[MENU] : IRQ allow : ");

    if (axi_adxl_has_irq_allow(ptr)) {
        textcolor(DEFAULT, BLACK, GREEN);
        printf("active");
    }
    else {
        textcolor(DEFAULT, BLACK, RED);
        printf("inactive");
    }
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");
    return ADXL_OK;
}




int selector_axi_adxl_has_interval_requestion(axi_adxl* ptr) {
    printf("\t[MENU] : interval requestion : ");

    if (axi_adxl_has_interval_requestion(ptr)) {
        textcolor(DEFAULT, BLACK, GREEN);
        printf("active");
    }
    else {
        textcolor(DEFAULT, BLACK, RED);
        printf("inactive");
    }
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");
    return ADXL_OK;
}




int selector_axi_adxl_has_work(axi_adxl* ptr) {
    printf("\t[MENU] : work state : ");
    if (axi_adxl_has_work(ptr)) {
        textcolor(DEFAULT, BLACK, GREEN);
        printf("active");
    }
    else {
        textcolor(DEFAULT, BLACK, RED);
        printf("inactive");
    }
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");
    return ADXL_OK;

}



int selector_axi_adxl_has_link(axi_adxl* ptr) {
    printf("\t[MENU] : link state : ");
    if (axi_adxl_has_link(ptr)) {
        textcolor(DEFAULT, BLACK, GREEN);
        printf("established");
    }
    else {
        textcolor(DEFAULT, BLACK, RED);
        printf("not found");
    }
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");
    return ADXL_OK;

}



int selector_axi_adxl_get_calibration_pwr_count_limit(axi_adxl* ptr) {
    printf("\t[MENU] : selected getting calibration power of count limit\r\n");
    uint32_t power_cnt_limit;
    int status = axi_adxl_get_calibration_pwr_count_limit(ptr, &power_cnt_limit);
    if (status == ADXL_OK)
        printf("\t[MENU] : Calibration count = %d times\r\n", 1<<power_cnt_limit);
    return status;

}



int selector_axi_adxl_set_calibration_pwr_count_limit(axi_adxl* ptr) {
    printf("\t[MENU] : selected changing power of count limit\r\n");
    int power_count_limit = 0;
    printf("\t[MENU] : enter new requestion interval : ");
    scanf_s("%d", &power_count_limit);
    printf("%d\r\n", power_count_limit);
    return axi_adxl_set_calibration_pwr_count_limit(ptr, power_count_limit);

}



int selector_axi_adxl_has_calibration_in_progress(axi_adxl* ptr) {
    printf("\t[MENU] : calibration : ");
    if (axi_adxl_has_calibration_in_progress(ptr)) {
        textcolor(DEFAULT, BLACK, GREEN);
        printf("active");
    }
    else {
        textcolor(DEFAULT, BLACK, RED);
        printf("inactive");
    }
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");
    return ADXL_OK;

}



int selector_axi_adxl_has_calibration_complete(axi_adxl* ptr) {
    printf("\t[MENU] : calibration : ");
    if (axi_adxl_has_calibration_complete(ptr)) {
        textcolor(DEFAULT, BLACK, GREEN);
        printf("complete");
    }
    else {
        textcolor(DEFAULT, BLACK, RED);
        printf("incomplete");
    }
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");
    return ADXL_OK;

}




//50
int selector_axi_adxl_set_bw_rate(axi_adxl *ptr){

    printf("\t[MENU] : change BW_RATE\r\n");

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

    int value = 0;
    scanf_s("%d", &value);

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


int selector_axi_adxl_get_bw_rate(axi_adxl* ptr) {
    int bw_rate = 0;
    int status = axi_adxl_get_bw_rate(ptr, &bw_rate);
    printf("\t[MENU] : current BW RATE is ");

    switch (bw_rate) {
        case BW_RATE_3200: printf("3200 Hz\r\n"); break;
        case BW_RATE_1600: printf("1600 Hz\r\n"); break;
        case BW_RATE_800: printf("800 Hz\r\n"); break;
        case BW_RATE_400: printf("400 Hz\r\n"); break;
        case BW_RATE_200: printf("200 Hz\r\n"); break;
        case BW_RATE_100: printf("100 Hz\r\n"); break;
        case BW_RATE_50: printf("50 Hz\r\n"); break;
        case BW_RATE_25: printf("25 Hz\r\n"); break;
        case BW_RATE_12_5: printf("12.5 Hz\r\n"); break;
        case BW_RATE_6_25: printf("6.25 Hz\r\n"); break;
        case BW_RATE_3_13: printf("3.13 Hz\r\n"); break;
        case BW_RATE_1_56: printf("1.56 Hz\r\n"); break;
        case BW_RATE_0_78: printf("0.78 Hz\r\n"); break;
        case BW_RATE_0_39: printf("0.39 Hz\r\n"); break;
        case BW_RATE_0_20: printf("0.20 Hz\r\n"); break;
        case BW_RATE_0_10: printf("0.10 Hz\r\n"); break;
        case BW_RATE_400_LP: printf("400 Hz <LOW POWER>\r\n"); break;
        case BW_RATE_200_LP: printf("200 Hz <LOW POWER>\r\n"); break;
        case BW_RATE_100_LP: printf("100 Hz <LOW POWER>\r\n"); break;
        case BW_RATE_50_LP: printf("50 Hz <LOW POWER>\r\n"); break;
        case BW_RATE_25_LP: printf("25 Hz <LOW POWER>\r\n"); break;
        case BW_RATE_12_5_LP: printf("12.5 Hz <LOW POWER>\r\n"); break;
        default: printf("<undefined value>\r\n"); break;
    }
    return status;
}


//51
int selector_axi_adxl_measurement_start(axi_adxl *ptr){
    printf("\t[MENU] : enable measure\r\n");
    return axi_adxl_enable_msmt(ptr);
}


//52
int selector_axi_adxl_measurement_stop(axi_adxl *ptr){
    printf("\t[MENU] : disable measure\r\n");
    return axi_adxl_disable_msmt(ptr);
}



int selector_axi_adxl_has_msmt(axi_adxl* ptr) {
    
    int has_active = axi_adxl_has_msmt(ptr);
    printf("\t[MENU] : status measure ");
    if (has_active){
        textcolor(DEFAULT, BLACK, GREEN);
        printf("active");
    }
    else {
        textcolor(DEFAULT, BLACK, RED);
        printf("inactive");
    }

    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    return ADXL_OK;
}




//53
int selector_axi_adxl_interrupt_enable(axi_adxl *ptr){

    int status = ADXL_OK;

    int enabled_dr = axi_adxl_is_int_enable(ptr, DATA_READY);
    int enabled_st = axi_adxl_is_int_enable(ptr, SINGLE_TAP);
    int enabled_dt = axi_adxl_is_int_enable(ptr, DOUBLE_TAP);
    int enabled_act = axi_adxl_is_int_enable(ptr, ACTIVITY);
    int enabled_inact = axi_adxl_is_int_enable(ptr, INACTIVITY);
    int enabled_ff = axi_adxl_is_int_enable(ptr, FREE_FALL);
    int enabled_wm = axi_adxl_is_int_enable(ptr, WATERMARK);
    int enabled_ovr = axi_adxl_is_int_enable(ptr, OVERRUN);

    printf("\t[MENU] : enable \r\n");
    if (enabled_dr) {
        textcolor(DEFAULT, BLACK, GREEN);
    }
    else {
        textcolor(DEFAULT, BLACK, RED);
    }
    printf("\t1. DATA_READY");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (enabled_st) {
        textcolor(DEFAULT, BLACK, GREEN);
    }
    else {
        textcolor(DEFAULT, BLACK, RED);
    }
    printf("\t2. SINGLE TAP");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (enabled_dt) {
        textcolor(DEFAULT, BLACK, GREEN);
    }
    else {
        textcolor(DEFAULT, BLACK, RED);
    }
    printf("\t3. DOUBLE TAP");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (enabled_act) {
        textcolor(DEFAULT, BLACK, GREEN);
    }
    else {
        textcolor(DEFAULT, BLACK, RED);
    }
    printf("\t4. ACTIVITY");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (enabled_inact) {
        textcolor(DEFAULT, BLACK, GREEN);
    }
    else {
        textcolor(DEFAULT, BLACK, RED);
    }
    printf("\t5. INACTIVITY");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (enabled_ff) {
        textcolor(DEFAULT, BLACK, GREEN);
    }
    else {
        textcolor(DEFAULT, BLACK, RED);
    }
    printf("\t6. FREE FALL");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (enabled_wm) {
        textcolor(DEFAULT, BLACK, GREEN);
    }
    else {
        textcolor(DEFAULT, BLACK, RED);
    }
    printf("\t7. WATERMARK");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (enabled_ovr) {
        textcolor(DEFAULT, BLACK, GREEN);
    }
    else {
        textcolor(DEFAULT, BLACK, RED);
    }
    printf("\t8. OVERRUN");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    int value = 0;
    scanf_s("%d", &value);

    printf("%d\r\n", value);

    switch (value){
        case 1 : status = axi_adxl_enable_int_enable(ptr, DATA_READY); break;
        case 2 : status = axi_adxl_enable_int_enable(ptr, SINGLE_TAP); break;
        case 3 : status = axi_adxl_enable_int_enable(ptr, DOUBLE_TAP); break;
        case 4 : status = axi_adxl_enable_int_enable(ptr, ACTIVITY); break;
        case 5 : status = axi_adxl_enable_int_enable(ptr, INACTIVITY); break;
        case 6 : status = axi_adxl_enable_int_enable(ptr, FREE_FALL); break;
        case 7 : status = axi_adxl_enable_int_enable(ptr, WATERMARK); break;
        case 8 : status = axi_adxl_enable_int_enable(ptr, OVERRUN); break;
        default : status = ADXL_UNCORRECT_VALUE;
    }

    return status;
}


//54
int selector_axi_adxl_interrupt_disable(axi_adxl *ptr){

    int status = ADXL_OK;

    int enabled_dr = axi_adxl_is_int_enable(ptr, DATA_READY);
    int enabled_st = axi_adxl_is_int_enable(ptr, SINGLE_TAP);
    int enabled_dt = axi_adxl_is_int_enable(ptr, DOUBLE_TAP);
    int enabled_act = axi_adxl_is_int_enable(ptr, ACTIVITY);
    int enabled_inact = axi_adxl_is_int_enable(ptr, INACTIVITY);
    int enabled_ff = axi_adxl_is_int_enable(ptr, FREE_FALL);
    int enabled_wm = axi_adxl_is_int_enable(ptr, WATERMARK);
    int enabled_ovr = axi_adxl_is_int_enable(ptr, OVERRUN);

    printf("\t[MENU] : disable\r\n");
    if (enabled_dr){
  	    textcolor(DEFAULT, BLACK, GREEN);
      }else{
  	    textcolor(DEFAULT, BLACK, RED);
      }
      printf("\t1. DATA_READY");
      textcolor(DEFAULT, STD, STD);
      printf("\r\n");

      if (enabled_st){
  	    textcolor(DEFAULT, BLACK, GREEN);
      }else{
  	    textcolor(DEFAULT, BLACK, RED);
      }
      printf("\t2. SINGLE TAP");
      textcolor(DEFAULT, STD, STD);
  	printf("\r\n");

  	if (enabled_dt){
  	    textcolor(DEFAULT, BLACK, GREEN);
      }else{
  	    textcolor(DEFAULT, BLACK, RED);
      }
      printf("\t3. DOUBLE TAP");
      textcolor(DEFAULT, STD, STD);
  	printf("\r\n");

      if (enabled_act){
  	    textcolor(DEFAULT, BLACK, GREEN);
      }else{
  	    textcolor(DEFAULT, BLACK, RED);
      }
      printf("\t4. ACTIVITY");
      textcolor(DEFAULT, STD, STD);
  	printf("\r\n");

      if (enabled_inact){
  	    textcolor(DEFAULT, BLACK, GREEN);
      }else{
  	    textcolor(DEFAULT, BLACK, RED);
      }
      printf("\t5. INACTIVITY");
      textcolor(DEFAULT, STD, STD);
  	printf("\r\n");

  	if (enabled_ff){
  	    textcolor(DEFAULT, BLACK, GREEN);
      }else{
  	    textcolor(DEFAULT, BLACK, RED);
      }
      printf("\t6. FREE FALL");
      textcolor(DEFAULT, STD, STD);
  	printf("\r\n");

      if (enabled_wm){
  	    textcolor(DEFAULT, BLACK, GREEN);
      }else{
  	    textcolor(DEFAULT, BLACK, RED);
      }
  	printf("\t7. WATERMARK");
      textcolor(DEFAULT, STD, STD);
  	printf("\r\n");

      if (enabled_ovr){
  	    textcolor(DEFAULT, BLACK, GREEN);
      }else{
  	    textcolor(DEFAULT, BLACK, RED);
      }
  	printf("\t8. OVERRUN");
      textcolor(DEFAULT, STD, STD);
  	printf("\r\n");


    int value = 0;
    scanf_s("%d", &value);
    printf("\t%d\r\n", value);

    switch (value){
        case 1 : status = axi_adxl_disable_int_enable(ptr, DATA_READY); break;
        case 2 : status = axi_adxl_disable_int_enable(ptr, SINGLE_TAP); break;
        case 3 : status = axi_adxl_disable_int_enable(ptr, DOUBLE_TAP); break;
        case 4 : status = axi_adxl_disable_int_enable(ptr, ACTIVITY); break;
        case 5 : status = axi_adxl_disable_int_enable(ptr, INACTIVITY); break;
        case 6 : status = axi_adxl_disable_int_enable(ptr, FREE_FALL); break;
        case 7 : status = axi_adxl_disable_int_enable(ptr, WATERMARK); break;
        case 8 : status = axi_adxl_disable_int_enable(ptr, OVERRUN); break;
        default : status = ADXL_UNCORRECT_VALUE;
    }

    return status;
}



int selector_axi_adxl_has_interrupt_enabled(axi_adxl* ptr) {

    int status = ADXL_OK;
    printf("\t[MENU] : intr sts\r\n");
    if (axi_adxl_is_int_enable(ptr, DATA_READY)) {
        textcolor(DEFAULT, BLACK, GREEN);
    }
    else {
        textcolor(DEFAULT, BLACK, RED);
    }
    printf("\t1. DATA_READY");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (axi_adxl_is_int_enable(ptr, SINGLE_TAP)) {
        textcolor(DEFAULT, BLACK, GREEN);
    }
    else {
        textcolor(DEFAULT, BLACK, RED);
    }
    printf("\t2. SINGLE TAP");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (axi_adxl_is_int_enable(ptr, DOUBLE_TAP)) {
        textcolor(DEFAULT, BLACK, GREEN);
    }
    else {
        textcolor(DEFAULT, BLACK, RED);
    }
    printf("\t3. DOUBLE TAP");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (axi_adxl_is_int_enable(ptr, ACTIVITY)) {
        textcolor(DEFAULT, BLACK, GREEN);
    }
    else {
        textcolor(DEFAULT, BLACK, RED);
    }
    printf("\t4. ACTIVITY");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (axi_adxl_is_int_enable(ptr, INACTIVITY)) {
        textcolor(DEFAULT, BLACK, GREEN);
    }
    else {
        textcolor(DEFAULT, BLACK, RED);
    }
    printf("\t5. INACTIVITY");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (axi_adxl_is_int_enable(ptr, FREE_FALL)) {
        textcolor(DEFAULT, BLACK, GREEN);
    }
    else {
        textcolor(DEFAULT, BLACK, RED);
    }
    printf("\t6. FREE FALL");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (axi_adxl_is_int_enable(ptr, WATERMARK)) {
        textcolor(DEFAULT, BLACK, GREEN);
    }
    else {
        textcolor(DEFAULT, BLACK, RED);
    }
    printf("\t7. WATERMARK");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (axi_adxl_is_int_enable(ptr, OVERRUN)) {
        textcolor(DEFAULT, BLACK, GREEN);
    }
    else {
        textcolor(DEFAULT, BLACK, RED);
    }
    printf("\t8. OVERRUN");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    return status;
}


//100
int selector_axi_adxl_dev_debug_register_space(axi_adxl *ptr){
    axi_adxl_dev_debug_register_space(ptr->dev);
    return ADXL_OK;
}


int selector_axi_adxl_change_range(axi_adxl *ptr){

    int status = ADXL_OK;
    int range_2g = axi_adxl_is_range(ptr, RANGE_2G);
    int range_4g = axi_adxl_is_range(ptr, RANGE_4G);
    int range_8g = axi_adxl_is_range(ptr, RANGE_8G);
    int range_16g = axi_adxl_is_range(ptr, RANGE_16G);
    int range_2g_full = axi_adxl_is_range(ptr, RANGE_2G_FULL);
    int range_4g_full = axi_adxl_is_range(ptr, RANGE_4G_FULL);
    int range_8g_full = axi_adxl_is_range(ptr, RANGE_8G_FULL);
    int range_16g_full = axi_adxl_is_range(ptr, RANGE_16G_FULL);

    printf("\t[MENU] : Change range\r\n");
    
    if (range_2g){
  	    textcolor(DEFAULT, BLACK, GREEN);
    }else{
  	    textcolor(DEFAULT, STD, STD);
    }
	printf("\t0. RANGE 2G");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");


    if (range_4g) {
        textcolor(DEFAULT, BLACK, GREEN);
	}else{
		textcolor(DEFAULT, STD, STD);
	}
    printf("\t1. RANGE 4G");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");


    if (range_8g) {
        textcolor(DEFAULT, BLACK, GREEN);
    }else{
  	    textcolor(DEFAULT, STD, STD);
    }
    printf("\t2. RANGE 8G");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");


    if (range_16g) {
        textcolor(DEFAULT, BLACK, GREEN);
    }else{
  	    textcolor(DEFAULT, STD, STD);
    }
    printf("\t3. RANGE 16");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");


    if (range_2g_full) {
        textcolor(DEFAULT, BLACK, GREEN);
    }else{
  	    textcolor(DEFAULT, STD, STD);
    }
    printf("\t4. RANGE 2G FULL");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");


    if (range_4g_full) {
        textcolor(DEFAULT, BLACK, GREEN);
    }else{
  	    textcolor(DEFAULT, STD, STD);
    }
    printf("\t5. RANGE 4G FULL");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");


    if (range_8g_full) {
        textcolor(DEFAULT, BLACK, GREEN);
    }else{
  	    textcolor(DEFAULT, STD, STD);
    }
    printf("\t6. RANGE 8G FULL");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (range_16g_full) {
        textcolor(DEFAULT, BLACK, GREEN);
    }else{
  	    textcolor(DEFAULT, STD, STD);
    }
    printf("\t7. RANGE 16G FULL");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");


    printf("Enter range value : ");

    int value = 0;

    scanf_s("%d", &value);
    
    printf("%d\r\n", value);
    
    switch (value){
        case 0 : status = axi_adxl_set_range(ptr, RANGE_2G); break;
        case 1 : status = axi_adxl_set_range(ptr, RANGE_4G); break;
        case 2 : status = axi_adxl_set_range(ptr, RANGE_8G); break;
        case 3 : status = axi_adxl_set_range(ptr, RANGE_16G); break;
        case 4 : status = axi_adxl_set_range(ptr, RANGE_2G_FULL); break;
        case 5 : status = axi_adxl_set_range(ptr, RANGE_4G_FULL); break;
        case 6 : status = axi_adxl_set_range(ptr, RANGE_8G_FULL); break;
        case 7 : status = axi_adxl_set_range(ptr, RANGE_16G_FULL); break;
        default : status = ADXL_UNCORRECT_VALUE; break;
    }
    return status;
}



int selector_axi_adxl_set_thresh_tap(axi_adxl *ptr){
    printf("\t[MENU] : change threshold tap %d LSB=%3.6f g>: ", 1, SCALE_THRESH_TAP);
    int threshold_tap = 0;
    scanf_s("%d", &threshold_tap);
    printf("\t[MENU] : %d(%3.6f g)\r\n", threshold_tap, ((float)threshold_tap)*SCALE_THRESH_TAP );
    return axi_adxl_set_thresh_tap(ptr, threshold_tap);
}



int selector_axi_adxl_get_thresh_tap(axi_adxl* ptr) {

    uint32_t threshold_tap = 0;

    int status = axi_adxl_get_thresh_tap(ptr, &threshold_tap);

    if (status == ADXL_OK)
        printf("\t[MENU] : Threshold tap = %d [%3.6f g]\r\n", threshold_tap, (float)threshold_tap*SCALE_THRESH_TAP);

    return status;

}



int selector_axi_adxl_set_duration(axi_adxl *ptr){

    printf("\t[MENU] : Selected changing for duration 1 LSB=%3.6f s>:\r\n", SCALE_DUR);
    int duration = 0;
    scanf_s("%d", &duration);
    printf("\t[MENU] : %d(%3.6f s)\r\n", duration, ((float)duration) * SCALE_DUR);
    return axi_adxl_set_dur(ptr, duration);
}



int selector_axi_adxl_get_duration(axi_adxl* ptr) {
    int duration = 0;
    int status = axi_adxl_get_dur(ptr, &duration);
    printf("\t[MENU] : Duration = %d(%3.6f g)\r\n", duration, ((float)duration) * SCALE_DUR);
    return status;
}



int selector_axi_adxl_set_latent(axi_adxl *ptr){
    printf("\t[MENU] : Selected changing latency\r\n");
    printf("\t\tEnter new value of latency <precision : %d LSB=%3.6f s>: ", 1, SCALE_LATENT);
    int latent = 0;
    scanf_s("%d", &latent);
    printf("%d(%3.6f s)\r\n", latent, (((float)latent))*SCALE_LATENT);
    return axi_adxl_set_latent(ptr, latent);
}



int selector_axi_adxl_get_latent(axi_adxl* ptr) {
    int latent = 0;
    int status = axi_adxl_get_latent(ptr, &latent);
    printf("\t[MENU] : Latency = %d(%3.6f g)\r\n", latent, ((float)latent) * SCALE_LATENT);
    return status;
}




int selector_axi_adxl_set_window(axi_adxl *ptr){

    printf("\t[MENU] : Selected changing window\r\n");
    printf("\tEnter new value of window <precision : %d LSB=%3.6f s>: ", 1, SCALE_WINDOW);

    int window = 0;
    scanf_s("%d", &window);

    printf("%d(%3.6f s)\r\n", window, ((float)window)*SCALE_WINDOW);

    return axi_adxl_set_window(ptr, window);

}


int selector_axi_adxl_get_window(axi_adxl* ptr) {
    int window = 0;
    int status = axi_adxl_get_window(ptr, &window);
    printf("\t[MENU] : Window = %d(%3.6f g)\r\n", window, ((float)window) * SCALE_WINDOW);
    return status;
}




int selector_axi_adxl_set_thresh_act(axi_adxl *ptr){
        
    printf("\t[MENU] : Selected changing activity threshold\r\n");
    printf("\tEnter new value of activity threshold <precision : %d LSB=%3.6f g>: ", 1, SCALE_THRESH_ACT);

    int thresh_act = 0;
    scanf_s("%d", &thresh_act);

    printf("%d(%3.6f g)\r\n", thresh_act, (float)thresh_act *SCALE_THRESH_ACT);

    return axi_adxl_set_thresh_act(ptr, thresh_act);

}


int selector_axi_adxl_get_thresh_act(axi_adxl* ptr) {
    int thresh_act = 0;
    int status = axi_adxl_get_thresh_act(ptr, &thresh_act);
    printf("\t[MENU] : Activity Threshold = %d(%3.6f g)\r\n", thresh_act, ((float)thresh_act) * SCALE_THRESH_ACT);
    return status;
}



int selector_axi_adxl_set_thresh_inact(axi_adxl *ptr){

    printf("\t[MENU] : Selected changing inactivity threshold\r\n");
    printf("\tEnter new value of inactivity threshold <precision : %d LSB=%3.6f g>: ", 1, SCALE_THRESH_INACT);

    int thresh_inact = 0;
    scanf_s("%d", &thresh_inact);

    printf("%d(%3.6f g)\r\n", thresh_inact, ((float)thresh_inact)*SCALE_THRESH_INACT);

    return axi_adxl_set_thresh_inact(ptr, thresh_inact);

}



int selector_axi_adxl_get_thresh_inact(axi_adxl* ptr) {
    int thresh_inact = 0;
    int status = axi_adxl_get_thresh_inact(ptr, &thresh_inact);
    printf("\t[MENU] : Inactivity Threshold = %d(%3.6f g)\r\n", thresh_inact, ((float)thresh_inact) * SCALE_THRESH_INACT);
    return status;
}



int selector_axi_adxl_set_time_inact(axi_adxl *ptr){

    printf("\t[MENU] : set inactivity time\r\n");
    printf("\tEnter new value of inactivity time <precision : 1 LSB=%3.6f s>: ", SCALE_TIME_INACT);

    int time_inact = 0;
    scanf_s("%d", &time_inact);

    printf("%d(%3.6f s)\r\n", time_inact, ((float)time_inact)*SCALE_TIME_INACT);

    return axi_adxl_set_time_inact(ptr, time_inact);

}



int selector_axi_adxl_get_time_inact(axi_adxl* ptr) {
    int time_inact = 0;
    int status = axi_adxl_get_time_inact(ptr, &time_inact);
    printf("\t[MENU] : Inactivity Time = %d(%3.6f s)\r\n", time_inact, ((float)time_inact) * SCALE_TIME_INACT);
    return status;
}




int selector_axi_adxl_activity_control_enable(axi_adxl *ptr){

    int status = 0;

    printf("\t[MENU] : Which coord enable\r\n");

    int act_x = axi_adxl_is_act_inact_control(ptr, ACT_X_MASK);
    int act_y = axi_adxl_is_act_inact_control(ptr, ACT_Y_MASK);
    int act_z = axi_adxl_is_act_inact_control(ptr, ACT_Z_MASK);

    if (act_x){
        textcolor(DEFAULT, BLACK, GREEN);
    }else{
        textcolor(DEFAULT, STD, STD);
    }
    printf("\t0. Activity X");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (act_y){
        textcolor(DEFAULT, BLACK, GREEN);
    }else{
        textcolor(DEFAULT, STD, STD);
    }
    printf("\t1. Activity Y");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (act_z){
        textcolor(DEFAULT, BLACK, GREEN);
    }else{
        textcolor(DEFAULT, STD, STD);
    }
    printf("\t2. Activity Z");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    int coord = 0;
    scanf_s("%d", &coord);

    switch (coord){
        case 0 : status = axi_adxl_enable_activity_control(ptr, ACT_X_MASK); break;
        case 1 : status = axi_adxl_enable_activity_control(ptr, ACT_Y_MASK); break;
        case 2 : status = axi_adxl_enable_activity_control(ptr, ACT_Z_MASK); break;
        default : status = ADXL_UNCORRECT_VALUE; break;
    }
    return status;
}



int selector_axi_adxl_inactivity_control_enable(axi_adxl *ptr){

    int status = ADXL_OK;

    printf("\t[MENU] : Which coord enable\r\n");
    
    int inact_x = axi_adxl_is_act_inact_control(ptr, INACT_X_MASK);
    int inact_y = axi_adxl_is_act_inact_control(ptr, INACT_Y_MASK);
    int inact_z = axi_adxl_is_act_inact_control(ptr, INACT_Z_MASK);

    if (inact_x){
        textcolor(DEFAULT, BLACK, GREEN);
    }else{
        textcolor(DEFAULT, STD, STD);
    }
    printf("\t0. Inactivity X");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (inact_y){
        textcolor(DEFAULT, BLACK, GREEN);
    }else{
        textcolor(DEFAULT, STD, STD);
    }
    printf("\t1. Inactivity Y");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (inact_z){
        textcolor(DEFAULT, BLACK, GREEN);
    }else{
        textcolor(DEFAULT, STD, STD);
    }
    printf("\t2. Inactivity Z");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    int value = 0;
    scanf_s("%d", &value);

    switch (value){
        case 0 : status = axi_adxl_enable_inactivity_control(ptr, INACT_X_MASK); break;
        case 1 : status = axi_adxl_enable_inactivity_control(ptr, INACT_Y_MASK); break;
        case 2 : status = axi_adxl_enable_inactivity_control(ptr, INACT_Z_MASK); break;
        default : status = ADXL_UNCORRECT_VALUE; break;
    }

    return status;
}




int selector_axi_adxl_activity_control_disable(axi_adxl *ptr){


    int status = ADXL_OK;

    printf("\t[MENU] : which disable\r\n");
    
    int act_x = axi_adxl_is_act_inact_control(ptr, ACT_X_MASK);
    int act_y = axi_adxl_is_act_inact_control(ptr, ACT_Y_MASK);
    int act_z = axi_adxl_is_act_inact_control(ptr, ACT_Z_MASK);

    if (act_x){
        textcolor(DEFAULT, BLACK, GREEN);
    }else{
        textcolor(DEFAULT, STD, STD);
    }
    printf("\t0. Activity X");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (act_y){
        textcolor(DEFAULT, BLACK, GREEN);
    }else{
        textcolor(DEFAULT, STD, STD);
    }
    printf("\t1. Activity Y");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (act_z){
        textcolor(DEFAULT, BLACK, GREEN);
    }else{
        textcolor(DEFAULT, STD, STD);
    }
    printf("\t2. Activity Z");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");
    
    int value = 0;
    scanf_s("%d", &value);
    
    switch (value){
        case 0 : status = axi_adxl_disable_activity_control(ptr, ACT_X_MASK); break;
        case 1 : status = axi_adxl_disable_activity_control(ptr, ACT_Y_MASK); break;
        case 2 : status = axi_adxl_disable_activity_control(ptr, ACT_Z_MASK); break;
        default : status = ADXL_UNCORRECT_VALUE; break;
    }
    return status;
}



int selector_axi_adxl_inactivity_control_disable(axi_adxl *ptr){


    int status = ADXL_OK;

    printf("\t[MENU] : Which coord disable\r\n");

    int act_x = axi_adxl_is_act_inact_control(ptr, INACT_X_MASK);
    int act_y = axi_adxl_is_act_inact_control(ptr, INACT_Y_MASK);
    int act_z = axi_adxl_is_act_inact_control(ptr, INACT_Z_MASK);

    if (act_x){
        textcolor(DEFAULT, BLACK, GREEN);
    }else{
        textcolor(DEFAULT, STD, STD);
    }
    printf("\t0. Inactivity X");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (act_y){
        textcolor(DEFAULT, BLACK, GREEN);
    }else{
        textcolor(DEFAULT, STD, STD);
    }
    printf("\t1. Inactivity Y");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (act_z){
        textcolor(DEFAULT, BLACK, GREEN);
    }else{
        textcolor(DEFAULT, STD, STD);
    }
    printf("\t2. Inactivity Z");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    int value = 0;
    scanf_s("%d", &value);

    switch (value){
        case 0 : status = axi_adxl_disable_inactivity_control(ptr, INACT_X_MASK); break;
        case 1 : status = axi_adxl_disable_inactivity_control(ptr, INACT_Y_MASK); break;
        case 2 : status = axi_adxl_disable_inactivity_control(ptr, INACT_Z_MASK); break;
        default : status = ADXL_UNCORRECT_VALUE; break;
    }
    
    return status;
}



int selector_axi_adxl_set_activity_acdc(axi_adxl *ptr){

    int status = ADXL_OK;

    printf("\t[MENU] : Select which mode must be activated\r\n");

    int has_activity_dc = axi_adxl_has_activity_dc(ptr);
    int has_activity_ac = axi_adxl_has_activity_ac(ptr);

    if (has_activity_dc){
        textcolor(DEFAULT, BLACK, GREEN);
    }else{
        textcolor(DEFAULT, BLACK, RED);
    }
    printf("\t0. DC coupled");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (has_activity_ac){
        textcolor(DEFAULT, BLACK, GREEN);
    }else{
        textcolor(DEFAULT, BLACK, RED);
    }
    printf("\t1. AC coupled");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    int mode = 0;
    scanf_s("%d", &mode);

    switch (mode){
        case 0 : status = axi_adxl_set_activity_acdc(ptr, DC_MODE); break;
        case 1 : status = axi_adxl_set_activity_acdc(ptr, AC_MODE); break;
        default : status = ADXL_UNCORRECT_VALUE; break;
    }

	return status;
}



int selector_axi_adxl_get_activity_acdc(axi_adxl* ptr) {

    int status = ADXL_OK;

    int activity = 0;
    status = axi_adxl_get_activity_acdc(ptr, &activity);
    printf("\t[MENU] : Activity mode : ");
    textcolor(DEFAULT, BLACK, GREEN);
    if (activity == AC_MODE) {
        printf("AC MODE");
    }

    if (activity == DC_MODE) {
        printf("DC MODE");
    }
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");


    return status;
}



///
//

int selector_axi_adxl_set_inactivity_acdc(axi_adxl* ptr) {

    int status = ADXL_OK;

    printf("\t[MENU] : Which mode activated\r\n");
    
    int inactivity_dc = axi_adxl_has_inactivity_dc(ptr);
    int inactivity_ac = axi_adxl_has_inactivity_ac(ptr);

    if (inactivity_dc) {
        textcolor(DEFAULT, BLACK, GREEN);
    }
    else {
        textcolor(DEFAULT, BLACK, RED);
    }
    printf("\t0. DC coupled");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (inactivity_ac) {
        textcolor(DEFAULT, BLACK, GREEN);
    }
    else {
        textcolor(DEFAULT, BLACK, RED);
    }
    printf("\t1. AC coupled");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");


    printf("\tEnter mode selection : ");
    int mode = 0;
    scanf_s("%d", &mode);

    switch (mode) {
        case 0: status = axi_adxl_set_inactivity_acdc(ptr, DC_MODE); break;
        case 1: status = axi_adxl_set_inactivity_acdc(ptr, AC_MODE); break;
        default: status = ADXL_UNCORRECT_VALUE; break;
    }

    return status;
}



int selector_axi_adxl_get_inactivity_acdc(axi_adxl* ptr) {
    printf("\t[MENU] : Selected show inactivity AC/DC mode\r\n");
    int status = ADXL_OK;

    int inactivity = 0;
    status = axi_adxl_get_inactivity_acdc(ptr, &inactivity);
    printf("\t[MENU] : inactivity mode : ");
    textcolor(DEFAULT, BLACK, GREEN);
    if (inactivity == AC_MODE) {
        printf("AC MODE");
    }

    if (inactivity == DC_MODE) {
        printf("DC MODE");
    }
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");


    return status;
}




int selector_axi_adxl_set_thresh_ff(axi_adxl *ptr){

    printf("[MENU] : Selecting FreeFall Threshold\r\n");
    printf("Enter new value of inactivity threshold <precision : %d LSB=%3.6f s>: ", 1, SCALE_THRESH_FF);

    int thresh_ff = 0;
    scanf_s("%d", &thresh_ff);

    printf("%d(%3.6fg)\r\n", thresh_ff, ((float)thresh_ff)*SCALE_THRESH_FF);

    return axi_adxl_set_thresh_ff(ptr, thresh_ff);

}


int selector_axi_adxl_get_thresh_ff(axi_adxl* ptr) {
    int thresh_ff = 0;
    int status = axi_adxl_get_thresh_ff(ptr, &thresh_ff);
    printf("\t[MENU] : FreeFall Threshold = %d(%3.6f g)\r\n", thresh_ff, ((float)thresh_ff) * SCALE_THRESH_FF);
    return status;
}


int selector_axi_adxl_set_time_ff(axi_adxl *ptr){

    printf("[MENU] : Selected FreeFall time\r\n");
    printf("Enter new value of inactivity threshold <precision : %d LSB=%3.6f s>: ", 1, SCALE_TIME_FF);

    int time_ff = 0;
    scanf_s("%d", &time_ff);
    printf("%d(%3.6f s)\r\n", time_ff, ((float)time_ff)*SCALE_TIME_FF);

    return axi_adxl_set_time_ff(ptr, time_ff);

}


int selector_axi_adxl_get_time_ff(axi_adxl* ptr) {
    int time_ff = 0;
    int status = axi_adxl_get_time_ff(ptr, &time_ff);
    printf("\t[MENU] : FreeFall Time = %d(%3.6f s)\r\n", time_ff, ((float)time_ff) * SCALE_TIME_FF);
    return status;
}



int selector_axi_adxl_set_tap_axes(axi_adxl *ptr){

	int status = ADXL_OK;

    printf("\t[MENU] : selected switch tap axes\r\n");

    int suppress = axi_adxl_is_tap_axes(ptr, TAP_SUPRESS_MASK);
    int tap_x = axi_adxl_is_tap_axes(ptr, TAP_X_MASK);
    int tap_y = axi_adxl_is_tap_axes(ptr, TAP_Y_MASK);
    int tap_z = axi_adxl_is_tap_axes(ptr, TAP_Z_MASK);

    if (suppress){
        textcolor(DEFAULT, BLACK, GREEN);
    }else{
        textcolor(DEFAULT, BLACK, RED);
    }
    printf("\t0. SUPRESS");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (tap_x){
        textcolor(DEFAULT, BLACK, GREEN);
    }else{
        textcolor(DEFAULT, BLACK, RED);
    }
    printf("\t1. TAP_X");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (tap_y){
        textcolor(DEFAULT, BLACK, GREEN);
    }else{
        textcolor(DEFAULT, BLACK, RED);
    }
    printf("\t2. TAP_Y");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (tap_z){
        textcolor(DEFAULT, BLACK, GREEN);
    }else{
        textcolor(DEFAULT, BLACK, RED);
    }
    printf("\t3. TAP_Z");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");


    int mask = 0;
    scanf_s("%d", &mask);

    switch(mask){
    	case 0 : status = axi_adxl_switch_tap_axes(ptr, TAP_SUPRESS_MASK); break;
    	case 1 : status = axi_adxl_switch_tap_axes(ptr, TAP_X_MASK); break;
    	case 2 : status = axi_adxl_switch_tap_axes(ptr, TAP_Y_MASK); break;
    	case 3 : status = axi_adxl_switch_tap_axes(ptr, TAP_Z_MASK); break;
    	default : status = ADXL_UNCORRECT_VALUE;
    }

	return status;
}


int selector_axi_adxl_get_tap_axes(axi_adxl* ptr) {

    int status = ADXL_OK;

    printf("\t[MENU] : tap axes status\r\n");

    int suppress = axi_adxl_is_tap_axes(ptr, TAP_SUPRESS_MASK);
    int x = axi_adxl_is_tap_axes(ptr, TAP_X_MASK);
    int y = axi_adxl_is_tap_axes(ptr, TAP_Y_MASK);
    int z = axi_adxl_is_tap_axes(ptr, TAP_Z_MASK);

    if (suppress) {
        textcolor(DEFAULT, BLACK, GREEN);
    }
    else {
        textcolor(DEFAULT, BLACK, RED);
    }
    printf("\t0. SUPRESS");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (x) {
        textcolor(DEFAULT, BLACK, GREEN);
    }
    else {
        textcolor(DEFAULT, BLACK, RED);
    }
    printf("\t1. TAP_X");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (y) {
        textcolor(DEFAULT, BLACK, GREEN);
    }
    else {
        textcolor(DEFAULT, BLACK, RED);
    }
    printf("\t2. TAP_Y");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (z) {
        textcolor(DEFAULT, BLACK, GREEN);
    }
    else {
        textcolor(DEFAULT, BLACK, RED);
    }
    printf("\t3. TAP_Z");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    return ADXL_OK;

}


int selector_axi_adxl_get_offsets(axi_adxl *ptr){

	int status = ADXL_OK;

	printf("[MAIN] : selecting offsets for coord x, y, z from device\r\n");

	textcolor(DIM, STD, STD);

    status = axi_adxl_get_ofsx(ptr, &(ptr->offset.ofsx));
    if (status != ADXL_OK) {
        return status;
    }
    status = axi_adxl_get_ofsy(ptr, &(ptr->offset.ofsy));
    if (status != ADXL_OK) {
        return status;
    }

    status = axi_adxl_get_ofsz(ptr, &(ptr->offset.ofsz));
    if (status != ADXL_OK) {
        return status;
    }

    textcolor(DEFAULT, STD, STD);

	printf("\tOFSX : %3.6f [0x%02x]\r\n", ((float)ptr->offset.ofsx * SCALE_OFS), ptr->offset.ofsx);
	printf("\tOFSY : %3.6f [0x%02x]\r\n", ((float)ptr->offset.ofsy * SCALE_OFS), ptr->offset.ofsy);
	printf("\tOFSZ : %3.6f [0x%02x]\r\n", ((float)ptr->offset.ofsz * SCALE_OFS), ptr->offset.ofsz);

	return status;
}


int selector_axi_adxl_set_ofsx(axi_adxl *ptr){

    int status = 0;

    printf("\t[MENU] : Enter new offset for X [resolution : %3.6f/LSB]\r\n", SCALE_OFS);
    int ofsx = 0;
    scanf_s("%d", &ofsx);

    status = axi_adxl_get_ofsx(ptr, &(ptr->offset.ofsx));
    if (status != ADXL_OK){
    	return status;
    }

    printf("\t[MENU] : Changing from value from %3.6f g to %3.6f g\r\n", (float)ptr->offset.ofsx * SCALE_OFS, (float)ofsx * SCALE_OFS);

    return axi_adxl_set_ofsx(ptr, ofsx);

}



int selector_axi_adxl_set_ofsy(axi_adxl *ptr){

    int status = 0;

    printf("\t[MENU] : Enter new offset for Y [resolution : %3.6f/LSB]\r\n", SCALE_OFS);
    int ofsy = 0;
    scanf_s("%d", &ofsy);

    status = axi_adxl_get_ofsy(ptr, &(ptr->offset.ofsy));
    if (status != ADXL_OK) {
        return status;
    }

    printf("\t[MENU] : Changing from value from %3.6f g to %3.6f g\r\n", (float)ptr->offset.ofsy * SCALE_OFS, (float)ofsy * SCALE_OFS);

    return axi_adxl_set_ofsy(ptr, ofsy);
}



int selector_axi_adxl_set_ofsz(axi_adxl *ptr){

    int status = 0;

    printf("\t[MENU] : Enter new offset for Z [resolution : %3.6f/LSB]\r\n", SCALE_OFS);
    int ofsz = 0;
    scanf_s("%d", &ofsz);

    status = axi_adxl_get_ofsz(ptr, &(ptr->offset.ofsz));
    if (status != ADXL_OK) {
        return status;
    }

    printf("\t[MENU] : Changing from value from %3.6f g to %3.6f g\r\n", (float)ptr->offset.ofsz * SCALE_OFS, (float)ofsz * SCALE_OFS);

    return axi_adxl_set_ofsz(ptr, ofsz);

}


int selector_axi_adxl_get_ofsx(axi_adxl* ptr) {
    int ofs = 0;

    int status = axi_adxl_get_ofsx(ptr, &ofs);
    printf("\t[MENU] : OFSY : 0x%02x [%3.6f g]\r\n", ofs, (float)ofs * SCALE_OFS);

    return status;

}



int selector_axi_adxl_get_ofsy(axi_adxl* ptr) {
    int ofs = 0;

    int status = axi_adxl_get_ofsy(ptr, &ofs);
    printf("\t[MENU] : OFSY : 0x%02x [%3.6f g]\r\n", ofs, (float)ofs * SCALE_OFS);

    return status;

}



int selector_axi_adxl_get_ofsz(axi_adxl* ptr) {
    int ofs = 0;

    int status = axi_adxl_get_ofsz(ptr, &ofs);
    printf("\t[MENU] : OFSZ : 0x%02x [%3.6f g]\r\n", ofs, (float)ofs * SCALE_OFS);

    return status;

}



int selector_axi_adxl_set_int_map(axi_adxl *ptr){

	int status = ADXL_OK;

    printf("\t[MENU] : selected changing interrupt map : \r\n");
    int int1_dr = axi_adxl_is_int_map_intr1(ptr, DATA_READY);
    int int1_st = axi_adxl_is_int_map_intr1(ptr, SINGLE_TAP);
    int int1_dt = axi_adxl_is_int_map_intr1(ptr, DOUBLE_TAP);
    int int1_act = axi_adxl_is_int_map_intr1(ptr, ACTIVITY);
    int int1_inact = axi_adxl_is_int_map_intr1(ptr, INACTIVITY);
    int int1_ff = axi_adxl_is_int_map_intr1(ptr, FREE_FALL);
    int int1_wm = axi_adxl_is_int_map_intr1(ptr, WATERMARK);
    int int1_ovr = axi_adxl_is_int_map_intr1(ptr, OVERRUN);

    printf("\t1. DATA_READY ");
    if (int1_dr){
        textcolor(DEFAULT, BLACK, RED);
        printf("<INT1>");
    }else{
        textcolor(DEFAULT, BLACK, GREEN);
        printf("<INT0>");
    }
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    printf("\t2. SINGLE_TAP ");
    if (int1_st){
        textcolor(DEFAULT, BLACK, RED);
        printf("<INT1>");
    }else{
        textcolor(DEFAULT, BLACK, GREEN);
        printf("<INT0>");
    }
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    printf("\t3. DOUBLE_TAP ");
    if (int1_dt){
        textcolor(DEFAULT, BLACK, RED);
        printf("<INT1>");
    }else{
        textcolor(DEFAULT, BLACK, GREEN);
        printf("<INT0>");
    }
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    printf("\t4. ACTIVITY ");
    if (int1_act){
        textcolor(DEFAULT, BLACK, RED);
        printf("<INT1>");
    }else{
        textcolor(DEFAULT, BLACK, GREEN);
        printf("<INT0>");
    }
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    printf("\t5. INACTIVITY ");
    if (int1_inact){
        textcolor(DEFAULT, BLACK, RED);
        printf("<INT1>");
    }else{
        textcolor(DEFAULT, BLACK, GREEN);
        printf("<INT0>");
    }
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    printf("\t6. FREE_FALL ");
    if (int1_ff){
        textcolor(DEFAULT, BLACK, RED);
        printf("<INT1>");
    }else{
        textcolor(DEFAULT, BLACK, GREEN);
        printf("<INT0>");
    }
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    printf("\t7. WATERMARK ");
    if (int1_wm){
        textcolor(DEFAULT, BLACK, RED);
        printf("<INT1>");
    }else{
        textcolor(DEFAULT, BLACK, GREEN);
        printf("<INT0>");
    }
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    printf("\t8. OVERRUN ");
    if (int1_ovr){
        textcolor(DEFAULT, BLACK, RED);
        printf("<INT1>");
    }else{
        textcolor(DEFAULT, BLACK, GREEN);
        printf("<INT0>");
    }
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    printf("\t[MENU] : Which interrupt select : \r\n");
    int interrupt = 0;

    scanf_s("%d", &interrupt);

    int int_map = 0;
    printf("\t[MENU] : New int map : 0 - INT0, 1 - INT1\r\n");

    scanf_s("%d", &int_map);
    if (int_map != 0 && int_map != 1) {
        return ADXL_UNCORRECT_VALUE;
    }

    switch(interrupt){
        case 1 : status = axi_adxl_set_int_map(ptr, DATA_READY, int_map); break;
        case 2 : status = axi_adxl_set_int_map(ptr, SINGLE_TAP, int_map); break;
        case 3 : status = axi_adxl_set_int_map(ptr, DOUBLE_TAP, int_map); break;
        case 4 : status = axi_adxl_set_int_map(ptr, ACTIVITY, int_map); break;
        case 5 : status = axi_adxl_set_int_map(ptr, INACTIVITY, int_map); break;
        case 6 : status = axi_adxl_set_int_map(ptr, FREE_FALL, int_map); break;
        case 7 : status = axi_adxl_set_int_map(ptr, WATERMARK, int_map); break;
        case 8 : status = axi_adxl_set_int_map(ptr, OVERRUN, int_map); break;
        default : status = ADXL_UNCORRECT_VALUE;
    }
    return status;
}


int selector_axi_adxl_get_int_map(axi_adxl* ptr) {

    int status = ADXL_OK;

    printf("\t[MENU] : selected changing interrupt map : \r\n");
    int int1_dr = axi_adxl_is_int_map_intr1(ptr, DATA_READY);
    int int1_st = axi_adxl_is_int_map_intr1(ptr, SINGLE_TAP);
    int int1_dt = axi_adxl_is_int_map_intr1(ptr, DOUBLE_TAP);
    int int1_act = axi_adxl_is_int_map_intr1(ptr, ACTIVITY);
    int int1_inact = axi_adxl_is_int_map_intr1(ptr, INACTIVITY);
    int int1_ff = axi_adxl_is_int_map_intr1(ptr, FREE_FALL);
    int int1_wm = axi_adxl_is_int_map_intr1(ptr, WATERMARK);
    int int1_ovr = axi_adxl_is_int_map_intr1(ptr, OVERRUN);

    printf("\t1. DATA_READY ");
    if (int1_dr) {
        textcolor(DEFAULT, BLACK, RED);
        printf("<INT1>");
    }
    else {
        textcolor(DEFAULT, BLACK, GREEN);
        printf("<INT0>");
    }
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    printf("\t2. SINGLE_TAP ");
    if (int1_st) {
        textcolor(DEFAULT, BLACK, RED);
        printf("<INT1>");
    }
    else {
        textcolor(DEFAULT, BLACK, GREEN);
        printf("<INT0>");
    }
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    printf("\t3. DOUBLE_TAP ");
    if (int1_dt) {
        textcolor(DEFAULT, BLACK, RED);
        printf("<INT1>");
    }
    else {
        textcolor(DEFAULT, BLACK, GREEN);
        printf("<INT0>");
    }
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    printf("\t4. ACTIVITY ");
    if (int1_act) {
        textcolor(DEFAULT, BLACK, RED);
        printf("<INT1>");
    }
    else {
        textcolor(DEFAULT, BLACK, GREEN);
        printf("<INT0>");
    }
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    printf("\t5. INACTIVITY ");
    if (int1_inact) {
        textcolor(DEFAULT, BLACK, RED);
        printf("<INT1>");
    }
    else {
        textcolor(DEFAULT, BLACK, GREEN);
        printf("<INT0>");
    }
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    printf("\t6. FREE_FALL ");
    if (int1_ff) {
        textcolor(DEFAULT, BLACK, RED);
        printf("<INT1>");
    }
    else {
        textcolor(DEFAULT, BLACK, GREEN);
        printf("<INT0>");
    }
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    printf("\t7. WATERMARK ");
    if (int1_wm) {
        textcolor(DEFAULT, BLACK, RED);
        printf("<INT1>");
    }
    else {
        textcolor(DEFAULT, BLACK, GREEN);
        printf("<INT0>");
    }
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    printf("\t8. OVERRUN ");
    if (int1_ovr) {
        textcolor(DEFAULT, BLACK, RED);
        printf("<INT1>");
    }
    else {
        textcolor(DEFAULT, BLACK, GREEN);
        printf("<INT0>");
    }
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");


    return ADXL_OK;
}



int selector_axi_adxl_get_int_source(axi_adxl *ptr){

    uint8_t interrupt_mask = 0x00;

    int status = axi_adxl_get_int_source(ptr, &interrupt_mask);
    if (status != ADXL_OK){
        return status;
    }

    printf("\t[MENU] : interrupt mask : 0x%02x\r\n", interrupt_mask);

    if ((axi_adxl_is_int_source(ptr, DATA_READY)) && (axi_adxl_is_int_enable(ptr, DATA_READY)) ){
        textcolor(DEFAULT, BLACK, GREEN);

    }else{
        textcolor(DEFAULT, BLACK, RED);

    }
    printf("DATA_READY");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (axi_adxl_is_int_source(ptr, SINGLE_TAP)){
        textcolor(DEFAULT, BLACK, GREEN);

    }else{
        textcolor(DEFAULT, BLACK, RED);

    }
    printf("SINGLE_TAP");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (axi_adxl_is_int_source(ptr, DOUBLE_TAP)){
        textcolor(DEFAULT, BLACK, GREEN);

    }else{
        textcolor(DEFAULT, BLACK, RED);

    }
    printf("DOUBLE_TAP");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (axi_adxl_is_int_source(ptr, ACTIVITY)){
        textcolor(DEFAULT, BLACK, GREEN);

    }else{
        textcolor(DEFAULT, BLACK, RED);

    }
    printf("ACTIVITY");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (axi_adxl_is_int_source(ptr, INACTIVITY)) {
        textcolor(DEFAULT, BLACK, GREEN);

    }else{
        textcolor(DEFAULT, BLACK, RED);

    }
    printf("INACTIVITY");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (axi_adxl_is_int_source(ptr, FREE_FALL)){
        textcolor(DEFAULT, BLACK, GREEN);

    }else{
        textcolor(DEFAULT, BLACK, RED);

    }
    printf("FREE_FALL");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (axi_adxl_is_int_source(ptr, WATERMARK)){
        textcolor(DEFAULT, BLACK, GREEN);

    }else{
        textcolor(DEFAULT, BLACK, RED);

    }
    printf("WATERMARK");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (axi_adxl_is_int_source(ptr, OVERRUN)){
        textcolor(DEFAULT, BLACK, GREEN);

    }else{
        textcolor(DEFAULT, BLACK, RED);

    }
    printf("OVERRUN");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    return status;
}


int selector_axi_adxl_get_data(axi_adxl *ptr){
	int status = axi_adxl_get_data(ptr, &(ptr->data));
	printf("[MENU] : selected printing text from data registers\r\n");
	printf("X : %d\r\n", ptr->data.x);
	printf("Y : %d\r\n", ptr->data.y);
	printf("Z : %d\r\n", ptr->data.z);

	return status;
}


int selector_axi_adxl_get_data_float(axi_adxl *ptr){
	adxl_data_float data;
	int status = axi_adxl_get_data_float(ptr, &data);
	printf("\t[MENU] : selected printing float text from data register\r\n");
	printf("X : %4.6f \tY : %4.6f \tZ : %4.6f\r\n", data.x, data.y, data.z);
	return status;

}



int selector_axi_adxl_set_fifo_mode(axi_adxl *ptr){
    printf("\t[MENU] : selected changing fifo_mode\r\n");
    int status = 0;
    printf("\t[MENU] : Enter new FIFO mode\r\n");
    
    if (axi_adxl_is_fifo_mode(ptr, FIFO_MODE_BYPASS)){
        textcolor(DEFAULT, BLACK, GREEN);
    }else{
        textcolor(DEFAULT, BLACK, RED);
    }
    printf("\t1. BYPASS");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (axi_adxl_is_fifo_mode(ptr, FIFO_MODE_FIFO)){
        textcolor(DEFAULT, BLACK, GREEN);
    }else{
        textcolor(DEFAULT, BLACK, RED);
    }
    printf("\t2. FIFO");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (axi_adxl_is_fifo_mode(ptr, FIFO_MODE_STREAM)){
        textcolor(DEFAULT, BLACK, GREEN);
    }else{
        textcolor(DEFAULT, BLACK, RED);
    }
    printf("\t3. STREAM");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (axi_adxl_is_fifo_mode(ptr, FIFO_MODE_TRIGGER)){
        textcolor(DEFAULT, BLACK, GREEN);
    }else{
        textcolor(DEFAULT, BLACK, RED);
    }
    printf("\t4. TRIGGER");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    int value = 0;

    scanf_s("%d", &value);

    switch (value){
        case 1 : status = axi_adxl_set_fifo_mode(ptr, FIFO_MODE_BYPASS); break;
        case 2 : status = axi_adxl_set_fifo_mode(ptr, FIFO_MODE_FIFO); break;
        case 3 : status = axi_adxl_set_fifo_mode(ptr, FIFO_MODE_STREAM); break;
        case 4 : status = axi_adxl_set_fifo_mode(ptr, FIFO_MODE_TRIGGER); break;
        default : status = ADXL_UNCORRECT_VALUE;
    }

    return status;
}


int selector_axi_adxl_get_fifo_mode(axi_adxl* ptr) {

    printf("\t[MENU] : FIFO mode is ");

    if (axi_adxl_is_fifo_mode(ptr, FIFO_MODE_BYPASS)) {
        textcolor(DEFAULT, BLACK, GREEN);
        printf("[BYPASS]");
    }
    if (axi_adxl_is_fifo_mode(ptr, FIFO_MODE_FIFO)) {
        textcolor(DEFAULT, BLACK, GREEN);
        printf("[FIFO]");
    }
    if (axi_adxl_is_fifo_mode(ptr, FIFO_MODE_STREAM)) {
        textcolor(DEFAULT, BLACK, GREEN);
        printf("[STREAM]");
    }
    if (axi_adxl_is_fifo_mode(ptr, FIFO_MODE_TRIGGER)) {
        textcolor(DEFAULT, BLACK, GREEN);
        printf("[TRIGGER]");
    }
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");


    return ADXL_OK;
}


int selector_axi_adxl_set_samples(axi_adxl *ptr){
    int status;
    printf("\t[MENU] : Enter number of samples[0-31] \r\n");

    int samples = 0;
    int value = 0;
    
    scanf_s("%d", &value);

    status = axi_adxl_set_samples(ptr, value);

    return status;
}



int selector_axi_adxl_get_samples(axi_adxl *ptr){
    int samples = 0;
    int status = axi_adxl_get_samples(ptr, &samples);
    printf("\t[MENU] : Current number of samples is %d\r\n", samples);

    return status;
}



int selector_axi_adxl_get_fifo_sts_entries(axi_adxl *ptr){
    printf("\t[MENU] : selected show entries of FIFO\r\n");
    uint8_t entries = 0;
    axi_adxl_get_fifo_sts_entries(ptr, &entries);
    printf("Number of entries : %d\r\n", entries);
    return ADXL_OK;

}



int selector_axi_adxl_has_fifo_sts_trigger(axi_adxl *ptr){
    printf("\t[MENU] : selected show trigger of FIFO status\r\n");
    printf("Trigger status : ");
    if (axi_adxl_has_fifo_sts_trigger(ptr)) {
        textcolor(DEFAULT, BLACK, RED);
        printf("Triggered");
    }else{
        textcolor(DEFAULT, BLACK, GREEN);
        printf("Clear");
    }
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    return ADXL_OK;
}



int selector_axi_adxl_set_trigger(axi_adxl *ptr){
    int status = 0;

    printf("\t[MENU] : Trigger current statement ");
    if (axi_adxl_has_trigger(ptr)){
        textcolor(DEFAULT, BLACK, RED);
        printf("actived");
    }else{
        textcolor(DEFAULT, BLACK, GREEN);
        printf("inactived");
    }
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    printf("\t[MENU] : Set next statement : \r\n");
    printf("1. Enable Trigger\r\n");
    printf("2. Disable Trigger\r\n");

    int value = 0;
    scanf_s("%d", &value);

    switch(value){
        case 1 : status = axi_adxl_enable_trigger(ptr); break;
        case 2 : status = axi_adxl_disable_trigger(ptr); break;
        default : status = ADXL_UNCORRECT_VALUE; break;
    }
    return status;

}



int selector_axi_adxl_get_trigger(axi_adxl *ptr){
    int status = 0;
    printf("\t[MENU] : Trigger statement ");
    if (axi_adxl_has_trigger(ptr)){
        textcolor(DEFAULT, BLACK, RED);
        printf("actived");
    }else{
        textcolor(DEFAULT, BLACK, GREEN);
        printf("inactived");
    }
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");
    return status;

}



int selector_axi_adxl_is_act_status(axi_adxl *ptr){
    printf("\t[MENU] : selected checking for ACT status\r\n");

    int act_x = axi_adxl_is_act_src_status(ptr, ACT_X_SRC);
    int act_y = axi_adxl_is_act_src_status(ptr, ACT_Y_SRC);
    int act_z = axi_adxl_is_act_src_status(ptr, ACT_Z_SRC);

    if (act_x){
        textcolor(DEFAULT, BLACK, GREEN);
    }else{
        textcolor(DEFAULT, BLACK, RED);
    }
    printf("ACTIVITY X");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (act_y){
        textcolor(DEFAULT, BLACK, GREEN);
    }else{
        textcolor(DEFAULT, BLACK, RED);
    }
    printf("ACTIVITY Y");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (act_z){
        textcolor(DEFAULT, BLACK, GREEN);
    }else{
        textcolor(DEFAULT, BLACK, RED);
    }
    printf("ACTIVITY Z");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    return ADXL_OK;

}



int selector_axi_adxl_is_tap_status(axi_adxl *ptr){
    printf("[MENU] : selected checking for TAP status\r\n");
    int tap_x = axi_adxl_is_act_src_status(ptr, TAP_X_SRC);
    int tap_y = axi_adxl_is_act_src_status(ptr, TAP_Y_SRC);
    int tap_z = axi_adxl_is_act_src_status(ptr, TAP_Z_SRC);

    if (tap_x){
        textcolor(DEFAULT, BLACK, GREEN);
    }else{
        textcolor(DEFAULT, BLACK, RED);
    }
    printf("TAP X");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (tap_y){
        textcolor(DEFAULT, BLACK, GREEN);
    }else{
        textcolor(DEFAULT, BLACK, RED);
    }
    printf("TAP Y");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (tap_z){
        textcolor(DEFAULT, BLACK, GREEN);
    }else{
        textcolor(DEFAULT, BLACK, RED);
    }
    printf("TAP Z");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    return ADXL_OK;

}



int selector_axi_adxl_has_asleep_status(axi_adxl *ptr){
    printf("[MENU] : selected checking for SLEEP status\r\n");
    int sleep = axi_adxl_has_asleep_status(ptr);
    printf("SLEEP MODE ");
    if (sleep){
        textcolor(DEFAULT, BLACK, GREEN);
        printf("<actived>");
    }else{
        textcolor(DEFAULT, BLACK, RED);
        printf("<inactived>");
    }
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");
    return ADXL_OK;
}




int selector_axi_adxl_set_linking_mode(axi_adxl *ptr){

    int linking_mode = axi_adxl_has_linking_mode(ptr);

    int status = 0;

    printf("\t[MENU] : Current state of linking mode is ");
    if (linking_mode){
        textcolor(DEFAULT, BLACK, RED);
        printf("actived");
    }else{
        textcolor(DEFAULT, BLACK, GREEN);
        printf("inactived");
    }
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");
    
    printf("\t[MENU] : Switch State : \r\n");
    printf("\t0. Disable\r\n");
    printf("\t1. Enable\r\n");

    int value = 0;
    scanf_s("%d", &value);

    switch (value) {
        case 0: status = axi_adxl_disable_linking_mode(ptr); break;
        case 1 : status = axi_adxl_enable_linking_mode(ptr); break;
        default: status = ADXL_UNCORRECT_VALUE; break;
    }

    return status;
}



int selector_axi_adxl_has_linking_mode(axi_adxl *ptr){
    
    if (axi_adxl_has_linking_mode(ptr)){
        printf("\t[MENU] : linking mode is ");
        textcolor(DEFAULT, BLACK, RED);
        printf("actived");
    }else{
        printf("\t[MENU] : linking mode is ");
        textcolor(DEFAULT, BLACK, GREEN);
        printf("inactived");
    }
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");
    return ADXL_OK;
}




int selector_axi_adxl_set_autosleep_mode(axi_adxl *ptr){

    int status = 0;

    int autoslp = axi_adxl_has_autosleep_mode(ptr);

    printf("\t[MENU] : Autosleep mode : ");

    if (autoslp) {
        textcolor(DEFAULT, BLACK, RED);
        printf("actived");
    }
    else {
        textcolor(DEFAULT, BLACK, GREEN);
        printf("inactived");
    }
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    printf("\t[MENU] : Switch State : \r\n");
    printf("\t0. Disable\r\n");
    printf("\t1. Enable\r\n");

    int value = 0;
    scanf_s("%d", &value);

    switch (value) {
        case 0: status = axi_adxl_disable_autosleep_mode(ptr); break;
        case 1 : status = axi_adxl_enable_autosleep_mode(ptr); break;
        default: status = ADXL_UNCORRECT_VALUE; break;
    }

    return status;
}



int selector_axi_adxl_set_sleep_mode(axi_adxl* ptr) {
    
    int status = 0;
    int has_sleep = axi_adxl_has_sleep_mode(ptr);
    printf("\t[MENU] : Current state of autosleep mode is ");
    if (has_sleep) {
        textcolor(DEFAULT, BLACK, RED);
        printf("actived");
    }
    else {
        textcolor(DEFAULT, BLACK, GREEN);
        printf("inactived");
    }
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    printf("\t[MENU] : Switch State : \r\n");
    printf("\t0. Disable\r\n");
    printf("\t1. Enable\r\n");

    int value = 0;
    scanf_s("%d", &value);

    switch (value) {
        case 0: status = axi_adxl_disable_sleep_mode(ptr); break;
        case 1 : status = axi_adxl_enable_sleep_mode(ptr); break;
        default: status = ADXL_UNCORRECT_VALUE; break;
    }


    return status;
}




int selector_axi_adxl_has_autosleep_mode(axi_adxl *ptr){
    
    int autoslp = axi_adxl_has_autosleep_mode(ptr);

    printf("\t[MENU] : autosleep mode is ");
    if (autoslp) {
        textcolor(DEFAULT, BLACK, RED);
        printf("actived");
    }
    else {
        textcolor(DEFAULT, BLACK, GREEN);
        printf("inactived");
    }
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    return ADXL_OK;
}




int selector_axi_adxl_has_sleep_mode(axi_adxl *ptr){
    
    int slp = axi_adxl_has_sleep_mode(ptr);
    
    printf("\t[MENU] : Sleep mode ");

    if (slp){
        textcolor(DEFAULT, BLACK, RED);
        printf("actived");
    }else{
        textcolor(DEFAULT, BLACK, GREEN);
        printf("inactived");
    }
    
    textcolor(DEFAULT, STD, STD);

    printf("\r\n");
    return ADXL_OK;
}



int selector_axi_adxl_set_wakeup(axi_adxl *ptr){

    printf("\t[MENU] : wakeup mode\r\n");
    
    int status = 0;
    
    int wakeup_8 = axi_adxl_is_wakeup(ptr, WAKEUP_8HZ);
    int wakeup_4 = axi_adxl_is_wakeup(ptr, WAKEUP_4HZ);
    int wakeup_2 = axi_adxl_is_wakeup(ptr, WAKEUP_2HZ);
    int wakeup_1 = axi_adxl_is_wakeup(ptr, WAKEUP_1HZ);

    if (wakeup_8){
        textcolor(DEFAULT, BLACK, GREEN);
    }else{
        textcolor(DEFAULT, BLACK, RED);
    }
    printf("\t1. 8 Hz");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (wakeup_4){
        textcolor(DEFAULT, BLACK, GREEN);
    }else{
        textcolor(DEFAULT, BLACK, RED);
    }
    printf("\t2. 4 Hz");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (wakeup_2){
        textcolor(DEFAULT, BLACK, GREEN);
    }else{
        textcolor(DEFAULT, BLACK, RED);
    }
    printf("\t3. 2 Hz");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    if (wakeup_1){
        textcolor(DEFAULT, BLACK, GREEN);
    }else{
        textcolor(DEFAULT, BLACK, RED);
    }
    printf("\t4. 1 Hz");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");
    

    int value = 0;
    scanf_s("%d", &value);

    switch (value){
        case 1 : status = axi_adxl_set_wakeup(ptr, WAKEUP_8HZ); break;
        case 2 : status = axi_adxl_set_wakeup(ptr, WAKEUP_4HZ); break;
        case 3 : status = axi_adxl_set_wakeup(ptr, WAKEUP_2HZ); break;
        case 4 : status = axi_adxl_set_wakeup(ptr, WAKEUP_1HZ); break;
        default : return ADXL_UNCORRECT_VALUE;
    }

    return status;
}




int selector_axi_adxl_get_wakeup(axi_adxl* ptr) {


    int status = 0;
    int wakeup = 0;
    status = axi_adxl_get_wakeup(ptr, &wakeup);

    printf("\t[MENU] : wakeup mode : ");

    switch (wakeup) {
        case WAKEUP_8HZ: printf("8 Hz\r\n"); break;
        case WAKEUP_4HZ: printf("4 Hz\r\n"); break;
        case WAKEUP_2HZ: printf("2 Hz\r\n"); break;
        case WAKEUP_1HZ: printf("1 Hz\r\n"); break;
        default : printf("<undefined>\r\n"); break;
    }


    return status;
}




int selector_axi_adxl_selftest(axi_adxl *ptr){

    printf("\t[MENU] : selected switch of selftest mode\r\n");

    int selected_value = 0;
    int status = 0;

    printf("\t[MENU] : Enter new wakeup mode\r\n");
    printf("\t1. Selftest on\r\n");
    printf("\t2. Selftest off\r\n");

    scanf_s("%d", &selected_value);
    
    switch (selected_value){
    	case 1 : status = axi_adxl_enable_selftest(ptr); break;
    	case 2 : status = axi_adxl_disable_selftest(ptr); break;
    	default : status = ADXL_UNCORRECT_VALUE;
    }

    return status;

}



int selector_axi_adxl_has_selftest(axi_adxl *ptr){

    printf("[MENU] : SelfTest status : ");
    if (axi_adxl_has_selftest(ptr)) {
        textcolor(DEFAULT, BLACK, RED);
        printf("actived");
    } else {
        textcolor(DEFAULT, BLACK, GREEN);
        printf("inactived");
    }
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");
    return ADXL_OK;
}



int selector_axi_adxl_set_spi_mode(axi_adxl *ptr){

    printf("\t[MENU] : selected switch of spi mode\r\n");

    int status = 0;

    int selected_value = 0;

    printf("\t[MENU] : Enter new spi mode\r\n");
    printf("\t1. 3 wire spi\r\n");
    printf("\t2. 4 wire spi\r\n");


    scanf_s("%d", &selected_value);

    switch (selected_value){
        case 1 : status = axi_adxl_set_spi_mode(ptr, SPI_3_WIRE); break;
        case 2 : status = axi_adxl_set_spi_mode(ptr, SPI_4_WIRE); break;
        default : return ADXL_UNCORRECT_VALUE;
    }

    return status;

}



int selector_axi_adxl_has_spi_mode(axi_adxl *ptr){

    printf("\t[MENU] : SPI mode ");
    if (axi_adxl_has_spi_3_wire(ptr)) {
        textcolor(DEFAULT, BLACK, RED);
        printf("3 wire");
    } 
    if (axi_adxl_has_spi_4_wire(ptr)) {
        textcolor(DEFAULT, BLACK, GREEN);
        printf("4 wire");
    }

    textcolor(DEFAULT, STD, STD);
    printf("\r\n");
    return ADXL_OK;
}



int selector_axi_adxl_set_invert(axi_adxl *ptr){

    printf("\t[MENU] : selected switch of interrupt invert\r\n");

    int status = 0;
    int selected_value = 0;

    printf("1. Inverted interrupts\r\n");
    printf("2. Direct interrupts\r\n");


    scanf_s("%d", &selected_value);

    switch (selected_value){
        case 1 : status = axi_adxl_enable_int_invert(ptr); break;
        case 2 : status = axi_adxl_disable_int_invert(ptr); break;
        default : status = ADXL_UNCORRECT_VALUE;
    }

    return status;

}


int selector_axi_adxl_has_inverted(axi_adxl *ptr){
    printf("\t[MENU] : Interrupts ");
    if (axi_adxl_has_int_invert(ptr)) {
        textcolor(DEFAULT, BLACK, RED);
        printf("inverted");
    } else {
        textcolor(DEFAULT, BLACK, GREEN);
        printf("directed");
    }
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");
    return ADXL_OK;
}


int selector_axi_adxl_justify(axi_adxl *ptr){

    printf("\t[MENU] : selected switch of justify function (MSB or LSB)\r\n");

    int status = 0;
    int selected_value = 0;

    printf("\t[MENU] : Enter new justify\r\n");
    printf("\t1. MSB\r\n");
    printf("\t2. LSB\r\n");

    scanf_s("%d", &selected_value);

    switch (selected_value){
        case 1 : status = axi_adxl_set_justify_msb(ptr); break;
        case 2 : status = axi_adxl_set_justify_lsb(ptr); break;
        default : status = ADXL_UNCORRECT_VALUE;
    }

    return status;

}



int selector_axi_adxl_has_justify(axi_adxl *ptr){
    printf("\t[MENU] : Justify ");

    if (axi_adxl_has_justify_msb(ptr)) {
        textcolor(DEFAULT, BLACK, RED);
        printf("<MSB>");
    } 
    
    if (axi_adxl_has_justify_lsb(ptr)) {
        textcolor(DEFAULT, BLACK, GREEN);
        printf("<LSB>");
    }

    textcolor(DEFAULT, STD, STD);
    
    printf("\r\n");
    return ADXL_OK;

}




int dbg_set_reg(axi_adxl *ptr){
    int reg_address;
    int value;
    printf("Address\r\n"); 
    scanf_s("%d", &reg_address);
    printf("Value\r\n");
    scanf_s("%d", &value);

    *((uint8_t*)ptr->dev + (uint8_t)reg_address) = value;
    return ADXL_OK;
}