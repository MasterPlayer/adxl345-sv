#include "axi_adxl.h"



void axi_adxl_cfg_debug(adxl_cfg *ptr){
    printf("[CFG SPACE]\r\n");
    printf("\t[NAME] \t\t\t\t| [RAW]\t\t| [VALUE]\r\n");
    printf("\t[VERSION] \t\t\t| 0x%08x \t| %d.%d\r\n", (((uint32_t)adxl_cfg_ctl_get_version_major(ptr) << 8) + (uint32_t)adxl_cfg_ctl_get_version_minor(ptr)), adxl_cfg_ctl_get_version_major(ptr), adxl_cfg_ctl_get_version_minor(ptr));

    printf("\t[RESET_COMPLETED] \t\t| 0x%08x \t| ", adxl_cfg_ctl_reset_completed(ptr));
    if (!adxl_cfg_ctl_reset_completed(ptr)) {
    	textcolor(DEFAULT, BLACK, RED);
    	printf("no");
    }else{
    	textcolor(DEFAULT, BLACK, GREEN);
        printf("yes");
    }
	textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    printf("\t[INTERVAL_REQUESTS] \t\t| 0x%08x \t| ", adxl_cfg_ctl_interval_requestion(ptr));
    if (adxl_cfg_ctl_interval_requestion(ptr)) {
    	textcolor(DEFAULT, BLACK, GREEN);
        printf("actived");
    }else{
    	textcolor(DEFAULT, RED, STD);
        printf("inactived");
    }
	textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    printf("\t[IRQ_ALLOW] \t\t\t| 0x%08x \t| ", adxl_cfg_ctl_irq_allowed(ptr));
    if (adxl_cfg_ctl_irq_allowed(ptr)){
    	textcolor(DEFAULT, BLACK, GREEN);
    	printf("yes");
    }else{
      	textcolor(DEFAULT, RED, STD);
      	printf("no");
    }
	textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    printf("\t[CALIBRATION] \t\t\t| 0x%08x \t| ", adxl_cfg_ctl_calibration_completed(ptr));
    if (adxl_cfg_ctl_calibration_completed(ptr)){
    	textcolor(DEFAULT, BLACK, GREEN);
        printf("completed");
    }else{
    	textcolor(DEFAULT, BLACK, RED);
        printf("not started");
    }
	textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    printf("\t[I2C ADDRESS] \t\t\t| 0x%08x \t| 0x%02x\r\n", adxl_cfg_ctl_get_iic_address(ptr), adxl_cfg_ctl_get_iic_address(ptr));

    printf("\t[LINK] \t\t\t\t| 0x%08x \t| ", adxl_cfg_ctl_link(ptr));
    if (adxl_cfg_ctl_link(ptr)){
    	textcolor(DEFAULT, BLACK, GREEN);
    	printf("on");
    }else{
    	textcolor(DEFAULT, BLACK, RED);
        printf("off");
    }
	textcolor(DEFAULT, STD, STD);
    printf("\r\n");


    printf("\t[WORK] \t\t\t\t| 0x%08x \t| ", adxl_cfg_ctl_work(ptr));
    if (adxl_cfg_ctl_work(ptr)){
    	textcolor(BRIGHT, GREEN, STD);
        printf("on");
    }else{
    	textcolor(DEFAULT, RED, STD);
        printf("off");
    }
	textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    printf("\t[REQUEST_INTERVAL] \t\t| 0x%08x \t| %10.3f nanoseconds\r\n", adxl_cfg_get_request_interval(ptr), ((float)adxl_cfg_get_request_interval(ptr) * (1/(float)adxl_cfg_get_clk_period(ptr)))*1000000000 );
    printf("\t[CALIBRATION_COUNT] \t\t| 0x%08x \t| %d times\r\n", adxl_cfg_get_calibration_count_limit(ptr), (1 << adxl_cfg_get_calibration_count_limit(ptr)));
    printf("\t[READ_VALID_COUNT] \t\t| 0x%08x \t| %d bytes/s\r\n", adxl_cfg_ctl_get_read_valid_count(ptr), adxl_cfg_ctl_get_read_valid_count(ptr));
    printf("\t[WRITE_VALID_COUNT] \t\t| 0x%08x \t| %d bytes/s\r\n", adxl_cfg_ctl_get_write_valid_count(ptr), adxl_cfg_ctl_get_write_valid_count(ptr));

    uint64_t read_transactions = (((uint64_t)adxl_cfg_ctl_get_read_transactions_msb(ptr)) << 32) + (uint64_t)adxl_cfg_ctl_get_read_transactions_lsb(ptr);

    printf("\t[READ_TRANSACTIONS_LSB] \t| 0x%08x \t| %d times\r\n", adxl_cfg_ctl_get_read_transactions_lsb(ptr), read_transactions);
    printf("\t[READ_TRANSACTIONS_MSB] \t| 0x%08x \t| %d times\r\n", adxl_cfg_ctl_get_read_transactions_msb(ptr), read_transactions);

    printf("\t[CLK_PERIOD] \t\t\t| 0x%08x \t| %10.3f nanoseconds\r\n", adxl_cfg_get_clk_period(ptr), (1/(float)adxl_cfg_get_clk_period(ptr))*1000000000);

    uint64_t opt_request_interval = ((uint64_t)(adxl_cfg_get_opt_request_interval_msb_reg(ptr) & CFG_OPT_REQUEST_INTERVAL_MSB_MASK) << 32) + (uint64_t)adxl_cfg_get_opt_request_interval_lsb_reg(ptr);

    printf("\t[OPT_REQUEST_INTERVAL_LSB] \t| 0x%08x \t| %3.9f seconds\r\n", adxl_cfg_get_opt_request_interval_lsb_reg(ptr), ((double)opt_request_interval/(double)adxl_cfg_get_clk_period(ptr)));
    printf("\t[OPT_REQUEST_INTERVAL_MSB] \t| 0x%08x \t| %3.9f seconds\r\n", adxl_cfg_get_opt_request_interval_msb_reg(ptr), ((double)opt_request_interval/(double)adxl_cfg_get_clk_period(ptr)));
    printf("\t[DATA_WIDTH] \t\t\t| 0x%08x \t| %d bits\r\n", adxl_cfg_get_data_width_reg(ptr), adxl_cfg_get_data_width_reg(ptr));

    uint64_t calibration_time = (((uint64_t)adxl_cfg_get_calibration_time_msb_reg(ptr))<< 32) + (uint64_t)adxl_cfg_get_calibration_time_lsb_reg(ptr) ;

    printf("\t[CALIBRATION_TIME_LSB] \t\t| 0x%08x \t| %3.9f seconds\r\n", adxl_cfg_get_calibration_time_lsb_reg(ptr), ((double)calibration_time/(double)adxl_cfg_get_clk_period(ptr)));
    printf("\t[CALIBRATION_TIME_MSB] \t\t| 0x%08x \t| %3.9f seconds\r\n", adxl_cfg_get_calibration_time_msb_reg(ptr), ((double)calibration_time/(double)adxl_cfg_get_clk_period(ptr)));

    uint64_t write_transactions = (((uint64_t)adxl_cfg_get_write_transactions_msb_reg(ptr)) << 32) + (uint64_t)adxl_cfg_get_write_transactions_lsb_reg(ptr);

    printf("\t[WRITE_TRANSACTIONS_LSB] \t| 0x%08x \t| %d times \r\n", adxl_cfg_get_write_transactions_lsb_reg(ptr), write_transactions);
    printf("\t[WRITE_TRANSACTIONS_MSB] \t| 0x%08x \t| %d times \r\n", adxl_cfg_get_write_transactions_msb_reg(ptr), write_transactions);

    printf("\t[SINGLE_REQ_ADDRESS] \t\t| 0x%08x \t| %d \r\n", adxl_cfg_get_single_req_params_address(ptr), adxl_cfg_get_single_req_params_address(ptr));
    printf("\t[SINGLE_REQ_SIZE] \t\t| 0x%08x \t| %d bytes \r\n", adxl_cfg_get_single_req_params_size(ptr), adxl_cfg_get_single_req_params_size(ptr));

    printf("\r\n");

}


void axi_adxl_dev_debug_register_space(adxl_dev *ptr){
	int byte_cnt = 0;
	printf("\t|| [0] \t| [1] \t| [2] \t| [3] \t|\r\n");
	printf("========================================\r\n");
	for (int i = 0; i < 64; i++){
		if (i % 4 == 0) {
			printf("0x%02x \t||", i);
		}
		int rw_flaq = 0;
		int ro_flaq = 0;
		int reserved_flaq = 0;

		rw_flaq = check_access_rw(i);
		ro_flaq = check_access_ro(i);
		reserved_flaq = check_access_reserved(i);

		if (rw_flaq){
	    	textcolor(DEFAULT, BLACK, GREEN);
		}

		if (ro_flaq){
	    	textcolor(DEFAULT, BLACK, YELLOW);
		}

		if (reserved_flaq){
	    	textcolor(DIM, STD, STD);
		}


		printf(" 0x%02x\t", *((uint8_t*)ptr+i));

    	textcolor(DEFAULT, STD, STD);

    	printf("|");

		if (byte_cnt == 3){
			printf("\r\n");
			byte_cnt = 0;
		}else{
			byte_cnt++;
		}

	}
}


int axi_adxl_init(axi_adxl *ptr, uint32_t baseaddr_cfg, uint32_t baseaddr_dev, uint8_t iic_address){

    printf("\t[ADXL_INIT] : set address to pointers\r\n");
    ptr->cfg = (adxl_cfg*)baseaddr_cfg;

    printf("\t\tconfiguration address space : 0x%08x\r\n", baseaddr_cfg);

    int timer = TIMER_LIMIT;

    if (adxl_cfg_ctl_work(ptr->cfg) || adxl_cfg_ctl_interval_requestion(ptr->cfg)){
    	printf("\t[ADXL_INIT] : component currently perform operation\r\n");

    	if (adxl_cfg_ctl_interval_requestion(ptr->cfg)){
			printf("\t[ADXL_INIT] : interval requestion disable ");
			adxl_cfg_ctl_interval_requestion_disable(ptr->cfg);
			while(adxl_cfg_ctl_work(ptr->cfg)){
				printf(".");
				if (timer == 0){
					textcolor(BRIGHT, RED, STD);
					printf("failure\r\n");
					textcolor(DEFAULT, STD, STD);
					return ADXL_CANNOT_STOP;
				}
				timer--;
			}
			textcolor(BRIGHT, GREEN, STD);
			printf("complete\r\n");
			textcolor(DEFAULT, STD, STD);

    	}

    	timer = TIMER_LIMIT;

    	if (adxl_cfg_ctl_irq_allowed(ptr->cfg)){
    		printf("\t[ADXL_INIT] : irq allowed. Unallow process ");
    		adxl_cfg_ctl_irq_unallow(ptr->cfg);
			while(adxl_cfg_ctl_work(ptr->cfg)){
				printf(".");
				if (timer == 0){
					textcolor(BRIGHT, RED, STD);
					printf("failure\r\n");
					textcolor(DEFAULT, STD, STD);

					return ADXL_CANNOT_STOP;
				}
				timer--;
			}
			textcolor(BRIGHT, GREEN, STD);
			printf("completed\r\n");
			textcolor(DEFAULT, STD, STD);
    	}

    }

    printf("\t[ADXL_INIT] : reset internal logic");
    timer = TIMER_LIMIT;
    adxl_cfg_ctl_reset(ptr->cfg);
    while (!adxl_cfg_ctl_reset_completed(ptr->cfg)){
    	if (timer == 0) {
			textcolor(BRIGHT, RED, STD);
    		printf(" FAILED\r\n");
			textcolor(DEFAULT, STD, STD);
    		return ADXL_RESET_INFINITE;
    	}
    	printf(".");
    	timer--;
    }
	textcolor(BRIGHT, GREEN, STD);
    printf(" completed\r\n");
	textcolor(DEFAULT, STD, STD);

    printf("\t[ADXL_INIT] : set iic address for device : 0x%02x\r\n", iic_address);
    adxl_cfg_ctl_set_iic_address(ptr->cfg, iic_address);

    printf("\t[ADXL_INIT] : perform single request ");
    adxl_cfg_ctl_single_request(ptr->cfg);
    timer = TIMER_LIMIT;
    while (!adxl_cfg_ctl_single_request_complete(ptr->cfg)){
    	if (timer == 0) {
    		textcolor(BRIGHT, RED, STD);
    		printf(" FAILED\r\n");
    		textcolor(DEFAULT, STD, STD);
    		return ADXL_NO_COMPLETE_SINGLE_REQUEST;
    	}
    	printf(".");
    	timer--;
    }
    textcolor(BRIGHT, GREEN, STD);
    printf(" complete\r\n");
    textcolor(DEFAULT, STD, STD);

    timer = TIMER_LIMIT;
    printf("\t[ADXL_INIT] : link ");
    while (!adxl_cfg_ctl_link(ptr->cfg)) {
    	if (timer == 0){
    		textcolor(BRIGHT, RED, STD);
    		printf("lost\r\n");
    		textcolor(DEFAULT, STD, STD);
    		return ADXL_LINK_LOST;
    	}
    	printf(".");
    	timer--;
    }
    textcolor(BRIGHT, GREEN, STD);
    printf("established\r\n");
    textcolor(DEFAULT, STD, STD);

    ptr->init_flaq = 1;

    return ADXL_OK;
}


/*
 * Function perform reset component
 * before reset, check about :
 * 1) Interval requestion is disabled
 * 2) ADXL_IRQ unallowed
 * */
int axi_adxl_reset(axi_adxl *ptr){

	if (ptr->init_flaq != 1) {
		textcolor(BRIGHT, RED, STD);
		printf("\t[ADXL_RESET] : has no init device\r\n");
		textcolor(DEFAULT, STD, STD);
		return ADXL_UNINIT;
	}

    int timer = TIMER_LIMIT;

    if (adxl_cfg_ctl_work(ptr->cfg) || adxl_cfg_ctl_interval_requestion(ptr->cfg)){

    	printf("\t[ADXL_RESET] : component currently perform operation\r\n");

    	if (adxl_cfg_ctl_interval_requestion(ptr->cfg)){
			printf("\t[ADXL_RESET] : interval requestion disable ");
			adxl_cfg_ctl_interval_requestion_disable(ptr->cfg);
			while(adxl_cfg_ctl_work(ptr->cfg)){
				printf(".");
				if (timer == 0){
					textcolor(BRIGHT, RED, STD);
					printf("failure\r\n");
					textcolor(DEFAULT, STD, STD);
				}
				timer--;
			}
			textcolor(BRIGHT, GREEN, STD);
			printf("complete\r\n");
			textcolor(DEFAULT, STD, STD);
    	}

    	timer = TIMER_LIMIT;

    	if (adxl_cfg_ctl_irq_allowed(ptr->cfg)){
    		printf("\t[ADXL_RESET] : irq allowed. Unallow process ");
    		adxl_cfg_ctl_irq_unallow(ptr->cfg);
			while(adxl_cfg_ctl_work(ptr->cfg)){
				printf(".");
				if (timer == 0){
					textcolor(BRIGHT, RED, STD);
					printf("failure\r\n");
					textcolor(DEFAULT, STD, STD);
				}
				timer--;
			}
			textcolor(BRIGHT, GREEN, STD);
			printf("completed");
			textcolor(DEFAULT, STD, STD);
    	}

    }

    printf("\t[ADXL_RESET] : reset internal logic");
    timer = TIMER_LIMIT;
    adxl_cfg_ctl_reset(ptr->cfg);
    while (!adxl_cfg_ctl_reset_completed(ptr->cfg)){
    	if (timer == 0) {
    		textcolor(BRIGHT, RED, STD);
    		printf(" FAILED\r\n");
    		textcolor(DEFAULT, STD, STD);
    		return ADXL_RESET_INFINITE;
    	}
    	printf(".");
    	timer--;
    }
    textcolor(BRIGHT, GREEN, STD);
    printf(" completed\r\n");
    textcolor(DEFAULT, STD, STD);

    return ADXL_OK;

}



/*
 * Function perform configure interval requests data from device
 * 1) Set requestion interval
 * 2) Enable interval requestion
 *
 * */
int axi_adxl_perform_interval_requestion(axi_adxl *ptr, uint32_t requestion_interval){

	if (ptr->init_flaq != 1) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_INTERVAL_REQ] : has no init device\r\n");
		textcolor(BRIGHT, STD, STD);
		return ADXL_UNINIT;
	}

	adxl_cfg_set_request_interval(ptr->cfg, requestion_interval);

	adxl_cfg_ctl_interval_requestion_enable(ptr->cfg);

    return ADXL_OK;
}



int axi_adxl_disable_interval_requestion(axi_adxl *ptr){
	if (ptr->init_flaq != 1) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_DISABLE_INTERVAL_REQ] : has no init device\r\n");
	    textcolor(DEFAULT, STD, STD);

		return ADXL_UNINIT;
	}

	adxl_cfg_ctl_interval_requestion_disable(ptr->cfg);

    return ADXL_OK;
}



int axi_adxl_perform_single_request(axi_adxl *ptr, uint8_t address, uint8_t size){

	if (ptr->init_flaq != 1) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_PRFRM_SNGL_REQ] : has no init device\r\n");
	    textcolor(DEFAULT, STD, STD);
		return ADXL_UNINIT;
	}

	if (size < 1 || size > 58) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_PRFRM_SNGL_REQ] : incorrect value: %d\r\n", size);
	    textcolor(DEFAULT, STD, STD);
		return ADXL_UNCORRECT_VALUE;
	}

    printf("\t[ADXL_PRFRM_SNGL_REQ] : perform single request ");

    adxl_cfg_set_single_req_params_address(ptr->cfg, address);

    adxl_cfg_set_single_req_params_size(ptr->cfg, size);

    adxl_cfg_ctl_single_request(ptr->cfg);

    int timer = TIMER_LIMIT;

    while (!adxl_cfg_ctl_single_request_complete(ptr->cfg)){
    	if (timer == 0) {
    	    textcolor(BRIGHT, RED, STD);
    		printf(" FAILED\r\n");
    		textcolor(DEFAULT, STD, STD);
    		return ADXL_NO_COMPLETE_SINGLE_REQUEST;
    	}
    	printf(".");
    	timer--;
    }
    textcolor(BRIGHT, GREEN, STD);
    printf(" complete\r\n");
    textcolor(DEFAULT, STD, STD);
    return ADXL_OK;
}



int axi_adxl_irq_allow(axi_adxl *ptr){

	if (ptr->init_flaq != 1) {
	    textcolor(BRIGHT, RED, STD);
		printf("\t[ADXL_IRQ_ALLOW] : has no init device\r\n");
	    textcolor(DEFAULT, STD, STD);

		return ADXL_UNINIT;
	}

	printf("[ADXL_IRQ_ALLOW] : allow irq from device ");
	adxl_cfg_ctl_irq_allow(ptr->cfg);
	int timer = TIMER_LIMIT;

	while (!adxl_cfg_ctl_irq_allowed(ptr->cfg)){
		if (timer == 0){
		    textcolor(BRIGHT, RED, STD);
			printf(" failed\r\n");
		    textcolor(DEFAULT, STD, STD);
			return ADXL_TIMEOUT;
		}
		timer--;
		printf(".");
	}

    textcolor(BRIGHT, GREEN, STD);
	printf(" complete\r\n");
    textcolor(DEFAULT, STD, STD);




    return ADXL_OK;
}


int axi_adxl_irq_unallow(axi_adxl *ptr){

	if (ptr->init_flaq != 1) {
	    textcolor(BRIGHT, RED, STD);
		printf("\t[ADXL_IRQ_UNALLOW] : has no init device\r\n");
	    textcolor(DEFAULT, STD, STD);

		return ADXL_UNINIT;
	}

	printf("[ADXL_IRQ_ALLOW] : unallow irq from device ");

	adxl_cfg_ctl_irq_unallow(ptr->cfg);
	int timer = TIMER_LIMIT;

	while (adxl_cfg_ctl_irq_allowed(ptr->cfg)){
		if (timer == 0){
			textcolor(BRIGHT, RED, STD);
			printf(" failed\r\n");
			textcolor(DEFAULT, STD, STD);
			return ADXL_TIMEOUT;
		}
		timer--;
		printf(".");
	}

	textcolor(BRIGHT, GREEN, STD);
	printf(" complete\r\n");
	textcolor(DEFAULT, STD, STD);


	return ADXL_OK;
}





int axi_adxl_calibration(axi_adxl *ptr, uint32_t calibration_count_limit){

	if (ptr->init_flaq != 1) {
	    textcolor(BRIGHT, RED, STD);
		printf("\t[ADXL_CALIBRATION] : has no init device\r\n");
	    textcolor(DEFAULT, STD, STD);

		return ADXL_UNINIT;
	}

	if (calibration_count_limit < 0 || calibration_count_limit > 31){
	    textcolor(BRIGHT, RED, STD);
		printf("\t[ADXL_CALIBRATION] : incorrect calibration count\r\n");
	    textcolor(DEFAULT, STD, STD);
		return ADXL_UNCORRECT_VALUE;
	}

	adxl_cfg_set_calibration_count_limit(ptr->cfg, calibration_count_limit);
	adxl_cfg_ctl_calibration(ptr->cfg);

	printf("[ADXL_CALIBRATION] : calibration in progress and stopped automatically\r\n");

    return ADXL_OK;
}



int axi_adxl_irq_ack(axi_adxl *ptr){
    return ADXL_OK;
}

