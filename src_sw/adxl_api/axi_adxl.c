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

    printf("\t[CALIBRATION] \t\t\t| 0x%08x \t| ", adxl_cfg_calibration_completed(ptr));
    if (adxl_cfg_calibration_completed(ptr)){
    	textcolor(DEFAULT, BLACK, GREEN);
        printf("completed");
    }else{
    	if (adxl_cfg_calibration_in_progress(ptr)){
    		textcolor(DEFAULT, BLACK, RED);
    		printf("in progress");
    	}else{
    		textcolor(DEFAULT, RED, STD);
    		printf("not started");
    	}
    }
	textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    printf("\t[I2C ADDRESS] \t\t\t| 0x%08x \t| 0x%02x\r\n", adxl_cfg_ctl_get_iic_address(ptr), adxl_cfg_ctl_get_iic_address(ptr));

    printf("\t[LINK] \t\t\t\t| 0x%08x \t| ", adxl_cfg_ctl_link(ptr));
    if (adxl_cfg_ctl_link(ptr)){
    	textcolor(DEFAULT, BLACK, GREEN);
    	printf("established");
    }else{
    	textcolor(DEFAULT, BLACK, RED);
        printf("not founded");
    }
	textcolor(DEFAULT, STD, STD);
    printf("\r\n");


    printf("\t[WORK] \t\t\t\t| 0x%08x \t| ", adxl_cfg_ctl_work(ptr));
    if (adxl_cfg_ctl_work(ptr)){
    	textcolor(DEFAULT, GREEN, STD);
        printf("in progress");
    }else{
    	textcolor(DEFAULT, RED, STD);
        printf("not operated");
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



/*
 * Initialize function perform next procedures
 * 1) setup cfg/dev baseaddress
 * 2) checking current activity : 
 * 		- if currently work (calibration/single request/interval requestion), then
 */
int axi_adxl_init(axi_adxl *ptr, uint32_t baseaddr_cfg, uint32_t baseaddr_dev, uint8_t iic_address){

	// Setup baseaddresses of component for sw struct
    printf("\t[ADXL_INIT] : set address to pointers\r\n");

    ptr->cfg = (adxl_cfg*)baseaddr_cfg;
    ptr->dev = (adxl_dev*)baseaddr_dev;

    printf("\t\tconfiguration address space : 0x%08x\r\n", baseaddr_cfg);
    printf("\t\tconfiguration address space : 0x%08x\r\n", baseaddr_dev);

    int timer = TIMER_LIMIT;


    /*If component working with device, we must stop them for correct initialization*/
    if (adxl_cfg_ctl_work(ptr->cfg) || adxl_cfg_ctl_interval_requestion(ptr->cfg)){
    	
    	printf("\t[ADXL_INIT] : component currently perform operation\r\n");

    	if (adxl_cfg_ctl_interval_requestion(ptr->cfg)){
			
			printf("\t[ADXL_INIT] : interval requestion disable ");
			
			adxl_cfg_ctl_interval_requestion_disable(ptr->cfg);

			/*Component perform operation?*/
			while(adxl_cfg_ctl_work(ptr->cfg)){
				
				if (timer == 0){
					// Print red font on default background
					textcolor(DEFAULT, BLACK, RED);
					printf("failure");
					textcolor(DEFAULT, STD, STD);
					printf("\r\n");
					return ADXL_CANNOT_STOP;
				}

				printf(".");
				timer--;
			}
			textcolor(DEFAULT, BLACK, GREEN);
			printf("complete");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
    	}

    	timer = TIMER_LIMIT;
    	/*Unallow interrupts if allowed before*/
    	if (adxl_cfg_ctl_irq_allowed(ptr->cfg)){
    		
    		printf("\t[ADXL_INIT] : irq allowed. Unallow process ");
    		
    		adxl_cfg_ctl_irq_unallow(ptr->cfg);
			
			while(adxl_cfg_ctl_work(ptr->cfg)){
				if (timer == 0){
					textcolor(DEFAULT, BLACK, RED);
					printf("failure");
					textcolor(DEFAULT, STD, STD);
					printf("\r\n");
					return ADXL_CANNOT_STOP;
				}
				printf(".");
				timer--;
			}

			textcolor(DEFAULT, BLACK, GREEN);
			printf("completed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
    	}

    }

    printf("\t[ADXL_INIT] : reset internal logic");
    timer = TIMER_LIMIT;
    adxl_cfg_ctl_reset(ptr->cfg);
    while (!adxl_cfg_ctl_reset_completed(ptr->cfg)){
    	if (timer == 0) {
			textcolor(DEFAULT, BLACK, RED);
    		printf(" failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
    		return ADXL_RESET_INFINITE;
    	}
    	printf(".");
    	timer--;
    }
	textcolor(DEFAULT, BLACK, GREEN);
    printf(" completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");

	/*Set I2C Address for work with device*/
    printf("\t[ADXL_INIT] : set iic address for device : ");
	textcolor(DEFAULT, BLACK, GREEN);
    printf("0x%02x", iic_address);
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");
    adxl_cfg_ctl_set_iic_address(ptr->cfg, iic_address);

    printf("\t[ADXL_INIT] : perform single request ");
    adxl_cfg_ctl_single_request(ptr->cfg);
    timer = TIMER_LIMIT;
    while (!adxl_cfg_ctl_single_request_complete(ptr->cfg)){
    	if (timer == 0) {
    		textcolor(DEFAULT, BLACK, RED);
    		printf("failure");
    		textcolor(DEFAULT, STD, STD);
    		printf("\r\n");
    		return ADXL_NO_COMPLETE_SINGLE_REQUEST;
    	}
    	printf(".");
    	timer--;
    }
    textcolor(DEFAULT, BLACK, GREEN);
    printf(" complete");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    timer = TIMER_LIMIT;

    printf("\t[ADXL_INIT] : link ");
    while (!adxl_cfg_ctl_link(ptr->cfg)) {
    	if (timer == 0){
    		textcolor(DEFAULT, BLACK, RED);
    		printf("lost");
    		textcolor(DEFAULT, STD, STD);
    		printf("\r\n");
    		return ADXL_LINK_LOST;
    	}
    	printf(".");
    	timer--;
    }

    textcolor(DEFAULT, BLACK, GREEN);
    printf("established");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

	/*STOP all msmt/interrupts from device*/
	if (adxl_cfg_ctl_link(ptr->cfg)){
		/*Disable measure*/
		adxl_dev_set_power_ctl(ptr->dev, (adxl_dev_get_power_ctl(ptr->dev) & ~POWER_CTL_MEASURE_MASK));
		adxl_dev_set_int_enable(ptr->dev, ~INTR_ALL_MASK);
	}

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

    int timer = TIMER_LIMIT;

	if (ptr->init_flaq != 1) {
		textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_RESET] : has no init device");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
		return ADXL_UNINIT;
	}

	/*STOP all msmt/interrupts from device*/
	if (adxl_cfg_ctl_link(ptr->cfg)){
		/*Disable measure*/
		timer = TIMER_LIMIT;

		printf("\t[ADXL_RESET] : disable device measurement capability ");
		adxl_dev_set_power_ctl(ptr->dev, (adxl_dev_get_power_ctl(ptr->dev) & ~POWER_CTL_MEASURE_MASK));
		while((adxl_dev_get_power_ctl(ptr->dev) & POWER_CTL_MEASURE_MASK)){
			if (timer == 0){
	    		textcolor(DEFAULT, BLACK, RED);
				printf("failed");
	    		textcolor(DEFAULT, STD, STD);
				printf("\r\n");
				return ADXL_TIMEOUT;
			}
			printf(".");
			timer--;
		}
		textcolor(DEFAULT, BLACK, GREEN);
		printf("completed");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");

		timer = TIMER_LIMIT;

		printf("\t[ADXL_RESET] : disable interrupt generation from device capability ");
		adxl_dev_set_int_enable(ptr->dev, ~INTR_ALL_MASK);
		while((adxl_dev_get_int_enable(ptr->dev) & INTR_ALL_MASK)){
			if (timer == 0){
	    		textcolor(DEFAULT, BLACK, RED);
				printf("failed");
	    		textcolor(DEFAULT, STD, STD);
				printf("\r\n");
				return ADXL_TIMEOUT;
			}
			printf(".");
			timer--;
		}
		textcolor(DEFAULT, BLACK, GREEN);
		printf("completed");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
	}

	timer = TIMER_LIMIT;

    if (adxl_cfg_ctl_work(ptr->cfg) || adxl_cfg_ctl_interval_requestion(ptr->cfg)){

    	printf("\t[ADXL_RESET] : component currently perform operation\r\n");

    	if (adxl_cfg_ctl_interval_requestion(ptr->cfg)){

			printf("\t[ADXL_RESET] : interval requestion disable ");
			adxl_cfg_ctl_interval_requestion_disable(ptr->cfg);
			while(adxl_cfg_ctl_work(ptr->cfg)){
				if (timer == 0){
					textcolor(DEFAULT, RED, STD);
					printf("failure");
					textcolor(DEFAULT, STD, STD);
					printf("\r\n");
				}
				printf(".");
				timer--;
			}
			textcolor(DEFAULT, GREEN, STD);
			printf("complete");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
    	}

    	timer = TIMER_LIMIT;

    	if (adxl_cfg_ctl_irq_allowed(ptr->cfg)){
    		printf("\t[ADXL_RESET] : irq allowed. Unallow process ");
    		adxl_cfg_ctl_irq_unallow(ptr->cfg);
			while(adxl_cfg_ctl_work(ptr->cfg)){
				if (timer == 0){
					textcolor(DEFAULT, RED, STD);
					printf("failure");
					textcolor(DEFAULT, STD, STD);
					printf("\r\n");
				}
				printf(".");
				timer--;
			}
			textcolor(DEFAULT, GREEN, STD);
			printf("completed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
    	}

    }

    printf("\t[ADXL_RESET] : reset internal logic ");
    timer = TIMER_LIMIT;
    adxl_cfg_ctl_reset(ptr->cfg);
    while (!adxl_cfg_ctl_reset_completed(ptr->cfg)){
    	if (timer == 0) {
    		textcolor(DEFAULT, BLACK, RED);
    		printf("failed");
    		textcolor(DEFAULT, STD, STD);
    		printf("\r\n");
    		return ADXL_RESET_INFINITE;
    	}
    	printf(".");
    	timer--;
    }
    textcolor(DEFAULT, BLACK, GREEN);
    printf(" completed");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

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
		printf("\t[ADXL_INTERVAL_REQ] : has no init device");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
		return ADXL_UNINIT;
	}

	adxl_cfg_set_request_interval(ptr->cfg, requestion_interval);

	adxl_cfg_ctl_interval_requestion_enable(ptr->cfg);
	int timer = TIMER_LIMIT;

	printf("[ADXL_INTERVAL_REQ] : interval requestion ");

	while(!adxl_cfg_ctl_interval_requestion(ptr->cfg)){
		if (timer == 0){
			textcolor(DEFAULT, BLACK, RED);
			printf("start failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
			return ADXL_TIMEOUT;
		}
		timer--;
		printf(".");
	}
	textcolor(DEFAULT, BLACK, GREEN);
	printf("launched");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");
	
    return ADXL_OK;
}



int axi_adxl_disable_interval_requestion(axi_adxl *ptr){
	if (ptr->init_flaq != 1) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_DISABLE_INTERVAL_REQ] : has no init device");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)){
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_DISABLE_INTERVAL_REQ] : link lost");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_LINK_LOST;
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
		printf("\t[ADXL_PRFRM_SNGL_REQ] : incorrect value: ");
	    textcolor(DEFAULT, BLACK, RED);
		printf("%d", size);
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_UNCORRECT_VALUE;
	}

    printf("\t[ADXL_PRFRM_SNGL_REQ] : perform single request ");

    adxl_cfg_set_single_req_params_address(ptr->cfg, address);

    adxl_cfg_set_single_req_params_size(ptr->cfg, size);

    adxl_cfg_ctl_single_request(ptr->cfg);

    int timer = TIMER_LIMIT;

    while (!adxl_cfg_ctl_single_request_complete(ptr->cfg)){
    	if (timer == 0) {
    	    textcolor(DEFAULT, BLACK, RED);
    		printf(" FAILED");
    		textcolor(DEFAULT, STD, STD);
    		printf("\r\n");
    		return ADXL_NO_COMPLETE_SINGLE_REQUEST;
    	}
    	printf(".");
    	timer--;
    }
    textcolor(DEFAULT, BLACK, GREEN);
    printf("complete");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");
    return ADXL_OK;
}



int axi_adxl_irq_allow(axi_adxl *ptr){

	if (ptr->init_flaq != 1) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_IRQ_ALLOW] : has no init device\r\n");
	    textcolor(DEFAULT, STD, STD);
		printf("\r\n");
		return ADXL_UNINIT;
	}

	printf("[ADXL_IRQ_ALLOW] : allow irq from device ");
	adxl_cfg_ctl_irq_allow(ptr->cfg);
	int timer = TIMER_LIMIT;

	while (!adxl_cfg_ctl_irq_allowed(ptr->cfg)){
		if (timer == 0){
		    textcolor(DEFAULT, BLACK, RED);
			printf("failed");
		    textcolor(DEFAULT, STD, STD);
		    printf("\r\n");
			return ADXL_TIMEOUT;
		}
		timer--;
		printf(".");
	}

    textcolor(DEFAULT, BLACK, GREEN);
	printf("complete");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");

    return ADXL_OK;
}



int axi_adxl_irq_unallow(axi_adxl *ptr){

	if (ptr->init_flaq != 1) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_IRQ_UNALLOW] : has no init device");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_UNINIT;
	}

	printf("[ADXL_IRQ_ALLOW] : unallow irq from device ");

	adxl_cfg_ctl_irq_unallow(ptr->cfg);
	int timer = TIMER_LIMIT;

	while (adxl_cfg_ctl_irq_allowed(ptr->cfg)){
		if (timer == 0){
			textcolor(DEFAULT, BLACK, RED);
			printf("failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
			return ADXL_TIMEOUT;
		}
		timer--;
		printf(".");
	}

	textcolor(DEFAULT, BLACK, GREEN);
	printf(" complete");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");

	return ADXL_OK;
}



int axi_adxl_calibration(axi_adxl *ptr, uint32_t calibration_count_limit){

	if (ptr->init_flaq != 1) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_CALIBRATION] : has no init device");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_UNINIT;
	}

	if (calibration_count_limit < 0 || calibration_count_limit > 31){
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_CALIBRATION] : incorrect calibration count value : ");
		textcolor(DEFAULT, BLACK, RED);
		printf("%d", calibration_count_limit);
	    textcolor(DEFAULT, STD, STD);
		printf("\r\n");
		return ADXL_UNCORRECT_VALUE;
	}

	adxl_cfg_set_calibration_count_limit(ptr->cfg, calibration_count_limit);

	adxl_cfg_calibration(ptr->cfg);

	printf("[ADXL_CALIBRATION] : calibration in progress and stopped automatically\r\n");

    return ADXL_OK;
}



int axi_adxl_set_iic_address(axi_adxl *ptr, uint8_t iic_address){

	if (ptr->init_flaq != 1){
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_SET_IIC_ADDRESS] : has no init device");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_UNINIT;
	}

	adxl_cfg_ctl_set_iic_address(ptr->cfg, iic_address);

	return ADXL_OK;
}



int axi_adxl_irq_ack(axi_adxl *ptr){

	adxl_cfg_ctl_intr_ack(ptr->cfg);

    return ADXL_OK;
}



int axi_adxl_set_bw_rate(axi_adxl *ptr, uint8_t value){
	
	if (ptr->init_flaq != 1){
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_SET_BW_RATE] : has no init device");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_SET_BW_RATE] : Link down");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_LINK_LOST;
	}

	printf("\t[ADXL_SET_BW_RATE] : setup new bandwidth value : ");
    switch (value){
    case BW_RATE_3200 :
            printf("3200 Hz\r\n");
            printf("\t[ADXL_SET_BW_RATE] : request interval : %3.6f seconds\r\n", (float)1/(float)3200);
        break;
    case BW_RATE_1600 :
            printf("1600 Hz\r\n");
            printf("\t[ADXL_SET_BW_RATE] : request interval : %3.6f seconds\r\n", (float)1/(float)1600);
        break;
    case BW_RATE_800  :
            printf("800 Hz\r\n");
            printf("\t[ADXL_SET_BW_RATE] : request interval : %3.6f seconds\r\n", (float)1/(float)800);
        break;
    case BW_RATE_400  :
            printf("400 Hz\r\n");
            printf("\t[ADXL_SET_BW_RATE] : request interval : %3.6f seconds\r\n", (float)1/(float)400);
        break;
    case BW_RATE_200  :
            printf("200 Hz\r\n");
            printf("\t[ADXL_SET_BW_RATE] : request interval : %3.6f seconds\r\n", (float)1/(float)200);
        break;
    case BW_RATE_100  :
            printf("100 Hz\r\n");
            printf("\t[ADXL_SET_BW_RATE] : request interval : %3.6f seconds\r\n", (float)1/(float)100);
        break;
    case BW_RATE_50   :
            printf("50 Hz\r\n");
            printf("\t[ADXL_SET_BW_RATE] : request interval : %3.6f seconds\r\n", (float)1/(float)50);
        break;
    case BW_RATE_25   :
            printf("25 Hz\r\n");
            printf("\t[ADXL_SET_BW_RATE] : request interval : %3.6f seconds\r\n", (float)1/(float)25);
        break;
    case BW_RATE_12_5 :
            printf("12.5 Hz\r\n");
            printf("\t[ADXL_SET_BW_RATE] : request interval : %3.6f seconds\r\n", (float)1/(float)12.5);
        break;
    case BW_RATE_6_25 :
            printf("6.25 Hz\r\n");
            printf("\t[ADXL_SET_BW_RATE] : request interval : %3.6f seconds\r\n", (float)1/(float)6.25);
        break;
    case BW_RATE_3_13 :
            printf("3.13 Hz\r\n");
            printf("\t[ADXL_SET_BW_RATE] : request interval : %3.6f seconds\r\n", (float)1/(float)3.13);
        break;
    case BW_RATE_1_56 :
            printf("1.56 Hz\r\n");
            printf("\t[ADXL_SET_BW_RATE] : request interval : %3.6f seconds\r\n", (float)1/(float)1.56);
        break;
    case BW_RATE_0_78 :
            printf("0.78 Hz\r\n");
            printf("\t[ADXL_SET_BW_RATE] : request interval : %3.6f seconds\r\n", (float)1/(float)0.78);
        break;
    case BW_RATE_0_39 :
            printf("0.39 Hz\r\n");
            printf("\t[ADXL_SET_BW_RATE] : request interval : %3.6f seconds\r\n", (float)1/(float)0.39);
        break;
    case BW_RATE_0_20 :
            printf("0.20 Hz\r\n");
            printf("\t[ADXL_SET_BW_RATE] : request interval : %3.6f seconds\r\n", (float)1/(float)0.20);
        break;
    case BW_RATE_0_10 :
            printf("0.10 Hz\r\n");
            printf("\t[ADXL_SET_BW_RATE] : request interval : %3.6f seconds\r\n", (float)1/(float)0.10);
        break;
    case BW_RATE_400_LP  :
            printf("400 Hz\r\n");
            printf("\t[ADXL_SET_BW_RATE] : request interval : %3.6f seconds\r\n", (float)1/(float)400);
            printf("\t[ADXL_SET_BW_RATE] : low power mode activated\r\n");
        break;
    case BW_RATE_200_LP  :
            printf("200 Hz\r\n");
            printf("\t[ADXL_SET_BW_RATE] : request interval : %3.6f seconds\r\n", (float)1/(float)200);
            printf("\t[ADXL_SET_BW_RATE] : low power mode activated\r\n");
        break;
    case BW_RATE_100_LP  :
            printf("100 Hz\r\n");
            printf("\t[ADXL_SET_BW_RATE] : request interval : %3.6f seconds\r\n", (float)1/(float)100);
            printf("\t[ADXL_SET_BW_RATE] : low power mode activated\r\n");
        break;
    case BW_RATE_50_LP   :
            printf("50 Hz\r\n");
            printf("\t[ADXL_SET_BW_RATE] : request interval : %3.6f seconds\r\n", (float)1/(float)50);
            printf("\t[ADXL_SET_BW_RATE] : low power mode activated\r\n");
        break;
    case BW_RATE_25_LP   :
            printf("25 Hz\r\n");
            printf("\t[ADXL_SET_BW_RATE] : request interval : %3.6f seconds\r\n", (float)1/(float)25);
            printf("\t[ADXL_SET_BW_RATE] : low power mode activated\r\n");
        break;
    case BW_RATE_12_5_LP :
            printf("12.5 Hz\r\n");
            printf("\t[ADXL_SET_BW_RATE] : request interval : %3.6f seconds\r\n", (float)1/(float)12.5);
            printf("\t[ADXL_SET_BW_RATE] : low power mode activated\r\n");
        break;
        default:
            printf("<undefined value>\r\n");
            return ADXL_UNCORRECT_VALUE;
        break;
	}

	adxl_dev_set_bw_rate(ptr->dev, value);

	return ADXL_OK;

}



int axi_adxl_measurement_start(axi_adxl *ptr){
	if (ptr->init_flaq != 1){
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_MSMT_START] : has no init device");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_MSMT_START] : Link down");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_LINK_LOST;
	}

	adxl_dev_set_power_ctl(ptr->dev, adxl_dev_get_power_ctl(ptr->dev) | POWER_CTL_MEASURE_MASK);

	int timer = adxl_cfg_get_clk_period(ptr->cfg)*3;

	printf("\t[ADXL_MSMT_START] : ");
	while( !(adxl_dev_get_power_ctl(ptr->dev) & POWER_CTL_MEASURE_MASK) ){
		if (timer == 0){
			textcolor(DEFAULT, BLACK, RED);
			printf("launch measurement failed");
			textcolor(DEFAULT, STD, STD);	
			printf("\r\n");	
			return ADXL_TIMEOUT;
		}
		printf(".");
		timer--;
	}

	textcolor(DEFAULT, BLACK, GREEN);
	printf("measurement activated");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");

	return ADXL_OK;

}



int axi_adxl_measurement_stop(axi_adxl *ptr){

	if (ptr->init_flaq != 1){
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_MSMT_STOP] : has no init device");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_MSMT_STOP] : Link down");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_LINK_LOST;
	}

	adxl_dev_set_power_ctl(ptr->dev, adxl_dev_get_power_ctl(ptr->dev) & ~POWER_CTL_MEASURE_MASK);

	printf("\t[ADXL_MSMT_STOP] : ");

	int timer = adxl_cfg_get_clk_period(ptr->cfg)*3;

	while((adxl_dev_get_power_ctl(ptr->dev) & POWER_CTL_MEASURE_MASK) ){
		if (timer == 0){
			textcolor(DEFAULT, BLACK, RED);
			printf("stop measurement failed");
			textcolor(DEFAULT, STD, STD);	
			printf("\r\n");	
			return ADXL_TIMEOUT;
		}
		printf(".");
		timer--;
	}

	textcolor(DEFAULT, BLACK, GREEN);
	printf("measurement stopped");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");

	return ADXL_OK;

}



int axi_adxl_interrupt_enable(axi_adxl *ptr, enum int_mask_enum intr){

	if (ptr->init_flaq != 1){
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_INTR_EN] : has no init device");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_INTR_EN] : Link down");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_LINK_LOST;
	}

	adxl_dev_set_int_enable(ptr->dev, (adxl_dev_get_int_enable(ptr->dev) | intr));

	return ADXL_OK;
}



int axi_adxl_interrupt_disable(axi_adxl *ptr, enum int_mask_enum intr){

	if (ptr->init_flaq != 1){
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_INTR_DSBL] : has no init device");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_INTR_DSBL] : Link down");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_LINK_LOST;
	}

	adxl_dev_set_int_enable(ptr->dev, (adxl_dev_get_int_enable(ptr->dev) & ~intr));

	return ADXL_OK;
}



int axi_adxl_change_range(axi_adxl *ptr, enum range_enum range){

	if (ptr->init_flaq != 1){
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_SET_RANGE] : has no init device");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_SET_RANGE] : Link down");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_LINK_LOST;
	}

	printf("\t[ADXL_SET_RANGE] : setup new range ");

    switch (range){

        case RANGE_2G:
            printf("[-2G..2G]\r\n");
            printf("\t\tResolution : 10 bit\r\n");
            printf("\t\tSensitivity : %1.4f g\r\n", SCALE_FACTOR_2G);
        break;

        case RANGE_4G:
            printf("[-4G..4G]\r\n");
            printf("\t\tResolution : 10 bit\r\n");
            printf("\t\tSensitivity %1.4f g\r\n", SCALE_FACTOR_4G);
        break;

        case RANGE_8G:
            printf("[-8G..8G]\r\n");
            printf("\t\tResolution : 10 bit\r\n");
            printf("\t\tSensitivity %1.4f g\r\n", SCALE_FACTOR_8G);
        break;

        case RANGE_16G:
            printf("[-16G..16G]\r\n");
            printf("\t\tResolution : 10 bit\r\n");
            printf("\t\tSensitivity %1.4f g\r\n", SCALE_FACTOR_16G);
        break;

        case RANGE_2G_FULL:
            printf("[-2G..2G]\r\n");
            printf("\t\tFull Resolution : 10 bit\r\n");
            printf("\t\tSensitivity : %1.4f g\r\n", SCALE_FACTOR_FULL_RES);
        break;

        case RANGE_4G_FULL:
            printf("[-4G..4G]\r\n");
            printf("\t\tFull Resolution : 11 bit\r\n");
            printf("\t\tSensitivity : %1.4f g\r\n", SCALE_FACTOR_FULL_RES);
        break;

        case RANGE_8G_FULL:
            printf("[-8G..8G]\r\n");
            printf("\t\tFull Resolution : 12 bit\r\n");
            printf("\t\tSensitivity : %1.4f g\r\n", SCALE_FACTOR_FULL_RES);
        break;

        case RANGE_16G_FULL:
            printf("[-16G..16G]\r\n");
            printf("\t\tFull Resolution : 13 bit\r\n");
            printf("\t\tSensitivity : %1.4f g\r\n", SCALE_FACTOR_FULL_RES);
        break;

        default :
            printf("<undefined>\r\n");
            printf("\t\tFull Resolution : <undefined> bit\r\n");
            printf("\t\tSensitivity : <undefined> g\r\n");
        break;

    }

    adxl_dev_set_data_format(ptr->dev, (adxl_dev_get_data_format(ptr->dev) & ~(DATA_FORMAT_FULL_RES_MASK | DATA_FORMAT_RANGE_MASK)) | range);

	return ADXL_OK;
}


int axi_adxl_change_thresh_tap(axi_adxl *ptr, uint8_t thresh_tap){
	
	if (ptr->init_flaq != 1){
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_SET_THRESH_TAP] : has no init device");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_SET_THRESH_TAP] : Link down");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_LINK_LOST;
	}

	printf("[ADXL_SET_THRESH_TAP] : thresh tap from %3.3f to %3.3f\r\n", (((float)adxl_dev_get_thresh_tap(ptr->dev)+1) * SCALE_THRESH_TAP), (((float)thresh_tap+1) * SCALE_THRESH_TAP));

    adxl_dev_set_thresh_tap(ptr->dev, thresh_tap);

    return ADXL_OK;
}



int axi_adxl_change_dur(axi_adxl *ptr, uint8_t duration){
	if (ptr->init_flaq != 1){
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_SET_DUR] : has no init device");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_SET_DUR] : Link down");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_LINK_LOST;
	}

    printf("\t[ADXL_SET_DUR] : changing duration: %3.6f sec => %3.6f sec\r\n", (((float)adxl_dev_get_dur(ptr->dev)+1) * SCALE_DUR), (((float)duration+1) * SCALE_DUR));
    
    adxl_dev_set_dur(ptr->dev, duration);

	return ADXL_OK;
}




int axi_adxl_interrupt_enabled(axi_adxl *ptr, enum int_mask_enum intr){
	return (adxl_dev_get_int_enable(ptr->dev) & intr);
}



int axi_adxl_get_range(axi_adxl *ptr){
	return (adxl_dev_get_data_format(ptr->dev) & (DATA_FORMAT_RANGE_MASK | DATA_FORMAT_FULL_RES_MASK));
}



int axi_adxl_change_latent(axi_adxl *ptr, uint8_t latent){
	if (ptr->init_flaq != 1){
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_SET_LAT] : has no init device");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_SET_LAT] : Link down");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_LINK_LOST;
	}

    printf("\t[ADXL_SET_LAT] : changing latency: %3.6f sec => %3.6f sec\r\n", (((float)adxl_dev_get_latent(ptr->dev)+1) * SCALE_LATENT), (((float)latent+1) * SCALE_LATENT));

	adxl_dev_set_latent(ptr->dev, latent);

	return ADXL_OK;

}



int axi_adxl_change_window(axi_adxl *ptr, uint8_t window){
	if (ptr->init_flaq != 1){
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_SET_WND] : has no init device");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_SET_WND] : Link down");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_LINK_LOST;
	}

    printf("\t[ADXL_SET_WND] : changing window: %3.6f sec => %3.6f sec\r\n", (((float)adxl_dev_get_window(ptr->dev)+1) * SCALE_WINDOW), (((float)window+1) * SCALE_WINDOW));
	
	adxl_dev_set_window(ptr->dev, window);

	return ADXL_OK;

}



int axi_adxl_change_thresh_act(axi_adxl *ptr, uint8_t thresh_act){
	if (ptr->init_flaq != 1){
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_SET_THRESH_ACT] : has no init device");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_SET_THRESH_ACT] : Link down");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_LINK_LOST;
	}

    printf("\t[ADXL_SET_THRESH_ACT] : changing activity thresold : %3.6f g => %3.6f g\r\n", (((float)adxl_dev_get_thresh_act(ptr->dev)+1) * SCALE_THRESH_ACT), (((float)thresh_act+1) * SCALE_THRESH_ACT));
	
	adxl_dev_set_thresh_act(ptr->dev, thresh_act);

	return ADXL_OK;

}



int axi_adxl_change_thresh_inact(axi_adxl *ptr, uint8_t thresh_inact){
	if (ptr->init_flaq != 1){
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_SET_THRESH_INACT] : has no init device");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_SET_THRESH_INACT] : Link down");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_LINK_LOST;
	}

    printf("\t[ADXL_SET_THRESH_INACT] : changing inactivity thresold: %3.6f g => %3.6f g\r\n", (((float)adxl_dev_get_thresh_inact(ptr->dev)+1) * SCALE_THRESH_INACT), (((float)thresh_inact+1) * SCALE_THRESH_INACT));
	
	adxl_dev_set_thresh_inact(ptr->dev, thresh_inact);

	return ADXL_OK;

}



int axi_adxl_change_time_inact(axi_adxl *ptr, uint8_t time_inact){
	if (ptr->init_flaq != 1){
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_SET_TIME_INACT] : has no init device");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_SET_THRESH_INACT] : Link down");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_LINK_LOST;
	}

    printf("\t[ADXL_SET_TIME_INACT] : changing inactivity time: %3f s => %3f s\r\n", (((float)adxl_dev_get_time_inact(ptr->dev)+1) * SCALE_TIME_INACT), (((float)time_inact+1) * time_inact));
	
	adxl_dev_set_time_inact(ptr->dev, time_inact);

	return ADXL_OK;

}