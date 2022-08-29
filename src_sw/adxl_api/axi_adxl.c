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

	printf("[ADXL_SET_THRESH_TAP] : thresh tap from %3.3f to %3.3f\r\n", (((float)adxl_dev_get_thresh_tap(ptr->dev)) * SCALE_THRESH_TAP), (((float)thresh_tap) * SCALE_THRESH_TAP));

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

    printf("\t[ADXL_SET_DUR] : changing duration: %3.6f sec => %3.6f sec\r\n", (((float)adxl_dev_get_dur(ptr->dev)) * SCALE_DUR), (((float)duration) * SCALE_DUR));
    
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

    printf("\t[ADXL_SET_LAT] : changing latency: %3.6f sec => %3.6f sec\r\n", (((float)adxl_dev_get_latent(ptr->dev)) * SCALE_LATENT), (((float)latent) * SCALE_LATENT));

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

    printf("\t[ADXL_SET_WND] : changing window: %3.6f sec => %3.6f sec\r\n", (((float)adxl_dev_get_window(ptr->dev)) * SCALE_WINDOW), (((float)window) * SCALE_WINDOW));
	
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

    printf("\t[ADXL_SET_THRESH_ACT] : changing activity thresold : %3.6f g => %3.6f g\r\n", (((float)adxl_dev_get_thresh_act(ptr->dev)) * SCALE_THRESH_ACT), (((float)thresh_act) * SCALE_THRESH_ACT));
	
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

    printf("\t[ADXL_SET_THRESH_INACT] : changing inactivity thresold: %3.6f g => %3.6f g\r\n", (((float)adxl_dev_get_thresh_inact(ptr->dev)) * SCALE_THRESH_INACT), (((float)thresh_inact) * SCALE_THRESH_INACT));
	
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

    printf("\t[ADXL_SET_TIME_INACT] : changing inactivity time: %3f s => %3f s\r\n", (((float)adxl_dev_get_time_inact(ptr->dev)) * SCALE_TIME_INACT), (((float)time_inact) * SCALE_TIME_INACT));
	
	adxl_dev_set_time_inact(ptr->dev, time_inact);

	return ADXL_OK;

}



int axi_adxl_activity_control_enable(axi_adxl *ptr, enum act_enum act_mask){

	if (ptr->init_flaq != 1){
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_ENABLE_ACT_CTRL] : has no init device");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_ENABLE_ACT_CTRL] : Link down");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_LINK_LOST;
	}

    printf("\t[ADXL_ENABLE_ACT_CTRL] : activity coord ");

    switch (act_mask){
    	case ACT_X_MASK : 
    		printf("X ");
    		adxl_dev_set_act_inact_ctl(ptr->dev, adxl_dev_get_act_inact_ctl(ptr->dev) | ACT_X_MASK); 
    	break;

    	case ACT_Y_MASK :
    		printf("Y "); 
    		adxl_dev_set_act_inact_ctl(ptr->dev, adxl_dev_get_act_inact_ctl(ptr->dev) | ACT_Y_MASK); 
    	break;

    	case ACT_Z_MASK : 
    		printf("Z ");
    		adxl_dev_set_act_inact_ctl(ptr->dev, adxl_dev_get_act_inact_ctl(ptr->dev) | ACT_Z_MASK); 
    	break;

    	default : 
    		printf("<incorrect>\r\n");
    		return ADXL_UNCORRECT_VALUE;
    }

    printf("was enabled\r\n");

	return ADXL_OK;
}



int axi_adxl_inactivity_control_enable(axi_adxl *ptr, enum inact_enum inact_mask){

	if (ptr->init_flaq != 1){
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_ENABLE_INACT_CTRL] : has no init device");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_ENABLE_INACT_CTRL] : Link down");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_LINK_LOST;
	}

    printf("\t[ADXL_ENABLE_INACT_CTRL] : inactivity coord ");

    switch (inact_mask){
    	case INACT_X_MASK : 
    		printf("X ");
    		adxl_dev_set_act_inact_ctl(ptr->dev, adxl_dev_get_act_inact_ctl(ptr->dev) | INACT_X_MASK); 
    	break;

    	case INACT_Y_MASK :
    		printf("Y "); 
    		adxl_dev_set_act_inact_ctl(ptr->dev, adxl_dev_get_act_inact_ctl(ptr->dev) | INACT_Y_MASK); 
    	break;

    	case INACT_Z_MASK : 
    		printf("Z ");
    		adxl_dev_set_act_inact_ctl(ptr->dev, adxl_dev_get_act_inact_ctl(ptr->dev) | INACT_Z_MASK); 
    	break;

    	default : 
    		printf("<incorrect>\r\n");
    		return ADXL_UNCORRECT_VALUE;
    }

    printf("was enabled\r\n");

	return ADXL_OK;
}



int axi_adxl_activity_control_disable(axi_adxl *ptr, enum act_enum act_mask){

	if (ptr->init_flaq != 1){
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_DISABLE_ACT_CTRL] : has no init device");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_DISABLE_ACT_CTRL] : Link down");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_LINK_LOST;
	}

    printf("\t[ADXL_DISABLE_ACT_CTRL] : activity coord ");

    switch (act_mask){
    	case ACT_X_MASK : 
    		printf("X ");
    		adxl_dev_set_act_inact_ctl(ptr->dev, adxl_dev_get_act_inact_ctl(ptr->dev) & (~ACT_X_MASK)); 
    	break;

    	case ACT_Y_MASK :
    		printf("Y "); 
    		adxl_dev_set_act_inact_ctl(ptr->dev, adxl_dev_get_act_inact_ctl(ptr->dev) & (~ACT_Y_MASK)); 
    	break;

    	case ACT_Z_MASK : 
    		printf("Z ");
    		adxl_dev_set_act_inact_ctl(ptr->dev, adxl_dev_get_act_inact_ctl(ptr->dev) & (~ACT_Z_MASK)); 
    	break;

    	default : 
    		printf("<incorrect>\r\n");
    		return ADXL_UNCORRECT_VALUE;
    }

    printf("was disabled\r\n");

	return ADXL_OK;
}



int axi_adxl_inactivity_control_disable(axi_adxl *ptr, enum inact_enum inact_mask){

	if (ptr->init_flaq != 1){
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_DISABLE_INACT_CTRL] : has no init device");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_DISABLE_INACT_CTRL] : Link down");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_LINK_LOST;
	}

    printf("\t[ADXL_DISABLE_INACT_CTRL] : inactivity coord ");

    switch (inact_mask){
    	case INACT_X_MASK : 
    		printf("X ");
    		adxl_dev_set_act_inact_ctl(ptr->dev, adxl_dev_get_act_inact_ctl(ptr->dev) & (~INACT_X_MASK)); 
    	break;

    	case INACT_Y_MASK :
    		printf("Y "); 
    		adxl_dev_set_act_inact_ctl(ptr->dev, adxl_dev_get_act_inact_ctl(ptr->dev) & (~INACT_Y_MASK)); 
    	break;

    	case INACT_Z_MASK : 
    		printf("Z ");
    		adxl_dev_set_act_inact_ctl(ptr->dev, adxl_dev_get_act_inact_ctl(ptr->dev) & (~INACT_Z_MASK)); 
    	break;

    	default : 
    		printf("<incorrect>\r\n");
    		return ADXL_UNCORRECT_VALUE;
    }

    printf("was enabled\r\n");

	return ADXL_OK;
}


int axi_adxl_change_activity_acdc(axi_adxl *ptr, uint8_t ac_mask){

	if (ptr->init_flaq != 1){
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_CHNG_ACT_ACDC] : has no init device");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_CHNG_ACT_ACDC] : Link down");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_LINK_LOST;
	}

	printf("\t[ADXL_CHNG_ACT_ACDC] : selected ");

	switch (ac_mask) {
		case 0: 
			printf("DC mode\r\n");
			adxl_dev_set_act_inact_ctl(ptr->dev, (adxl_dev_get_act_inact_ctl(ptr->dev) & ~ACT_AC_MASK)); 
			break;
		case 1: 
			printf("AC mode\r\n");
			adxl_dev_set_act_inact_ctl(ptr->dev, (adxl_dev_get_act_inact_ctl(ptr->dev) | ACT_AC_MASK)); 
		break;

		default: 
			return ADXL_UNCORRECT_VALUE;
	
	}
	return ADXL_OK;
}



int axi_adxl_change_inactivity_acdc(axi_adxl *ptr, uint8_t ac_mask){

	if (ptr->init_flaq != 1){
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_CHNG_INACT_ACDC] : has no init device");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_CHNG_INACT_ACDC] : Link down");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_LINK_LOST;
	}

	printf("\t[ADXL_CHNG_INACT_ACDC] : selected ");

	switch (ac_mask){
		case 0: 
			printf("DC mode\r\n");
			adxl_dev_set_act_inact_ctl(ptr->dev, (adxl_dev_get_act_inact_ctl(ptr->dev) & ~INACT_AC_MASK)); 
			break;
		case 1: 
			printf("AC mode\r\n");
			adxl_dev_set_act_inact_ctl(ptr->dev, (adxl_dev_get_act_inact_ctl(ptr->dev) | INACT_AC_MASK)); 
		break;

		default: 
			return ADXL_UNCORRECT_VALUE;

	}
	return ADXL_OK;

}



int axi_adxl_has_act_inact_control(axi_adxl *ptr, uint8_t mask){

	return (adxl_dev_get_act_inact_ctl(ptr->dev) & mask) ? TRUE : FALSE;
}



int axi_adxl_change_thresh_ff(axi_adxl *ptr, uint8_t thresh_ff){

	if (ptr->init_flaq != 1){
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_CHNG_THRESH_FF] : has no init device");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_CHNG_THRESH_FF] : Link down");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_LINK_LOST;
	}

    printf("\t[ADXL_CHNG_THRESH_FF] : changing freefall threshold : %3.6f g => %3.6f g\r\n", (((float)adxl_dev_get_thresh_ff(ptr->dev)) * SCALE_THRESH_FF), (((float)thresh_ff) * SCALE_THRESH_FF));
	
	return adxl_dev_set_thresh_ff(ptr->dev, thresh_ff);


}



int axi_adxl_change_time_ff(axi_adxl *ptr, uint8_t time_ff){

	if (ptr->init_flaq != 1){
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_CHNG_TIME_FF] : has no init device");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_CHNG_TIME_FF] : Link down");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_LINK_LOST;
	}

    printf("\t[ADXL_CHNG_TIME_FF] : changing freefall time : %3.6f s => %3.6f s\r\n", (((float)adxl_dev_get_time_ff(ptr->dev)) * SCALE_TIME_FF), (((float)time_ff) * SCALE_TIME_FF));
	
    return adxl_dev_set_time_ff(ptr->dev, time_ff);

}



int axi_adxl_change_tap_axes(axi_adxl *ptr, uint8_t mask){

	if (ptr->init_flaq != 1){
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_CHNG_TAP_AXES] : has no init device");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_CHNG_TAP_AXES] : Link down");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_LINK_LOST;
	}

	printf("[ADXL_CHNG_TAP_AXES] : change statement of ");

	if (mask & TAP_AXES_SUPRESS_MASK){
		printf("SUPRESS ");
	}

	if (mask & TAP_AXES_TAP_X_EN_MASK){
		printf("X ");
	}

	if (mask & TAP_AXES_TAP_Y_EN_MASK){
		printf("Y ");
	}

	if (mask & TAP_AXES_TAP_Z_EN_MASK){
		printf("Z ");
	}

	printf("\r\n");

	adxl_dev_set_tap_axes(ptr->dev, adxl_dev_get_tap_axes(ptr->dev) ^ (mask));

	return ADXL_OK;
}


int axi_adxl_tap_axes_actived(axi_adxl *ptr, uint8_t mask){

	return (adxl_dev_get_tap_axes(ptr->dev) & mask);
}


int axi_adxl_get_ofsx(axi_adxl *ptr, int8_t *ofsx){

	if (ptr->init_flaq != 1){
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_GET_OFSX] : has no init device");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_GET_OFSX] : Link down");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_LINK_LOST;
	}

	*ofsx = adxl_dev_get_ofsx(ptr->dev);

	return ADXL_OK;
}


int axi_adxl_get_ofsy(axi_adxl *ptr, int8_t *ofsy){

	if (ptr->init_flaq != 1){
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_GET_OFSY] : has no init device");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_GET_OFSY] : Link down");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_LINK_LOST;
	}

	*ofsy = adxl_dev_get_ofsy(ptr->dev);

	return ADXL_OK;
}



int axi_adxl_get_ofsz(axi_adxl *ptr, int8_t *ofsz){

	if (ptr->init_flaq != 1){
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_GET_OFSZ] : has no init device");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_GET_OFSZ] : Link down");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_LINK_LOST;
	}

	*ofsz = adxl_dev_get_ofsz(ptr->dev);

	return ADXL_OK;
}


int axi_adxl_set_ofsx(axi_adxl *ptr, int8_t ofsx){

	if (ptr->init_flaq != 1){
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_SET_OFSX] : has no init device");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_SET_OFSX] : Link down");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_LINK_LOST;
	}

	adxl_dev_set_ofsx(ptr->dev, ofsx);

	return ADXL_OK;

}



int axi_adxl_set_ofsy(axi_adxl *ptr, int8_t ofsy){

	if (ptr->init_flaq != 1){
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_SET_OFSY] : has no init device");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_SET_OFSY] : Link down");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_LINK_LOST;
	}

	adxl_dev_set_ofsy(ptr->dev, ofsy);

	return ADXL_OK;

}



int axi_adxl_set_ofsz(axi_adxl *ptr, int8_t ofsz){

	if (ptr->init_flaq != 1){
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_SET_OFSZ] : has no init device");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_SET_OFSZ] : Link down");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_LINK_LOST;
	}

	adxl_dev_set_ofsz(ptr->dev, ofsz);

	return ADXL_OK;
}



int axi_adxl_change_int_map(axi_adxl *ptr, uint8_t int_mask){

	if (ptr->init_flaq != 1){
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_CHNG_INT_MAP] : has no init device");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_CHNG_INT_MAP] : Link down");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_LINK_LOST;
	}

	printf("[ADXL_CHNG_INT_MAP] changing map of \r\n");

	if (int_mask & DATA_READY){
		printf("DATA_READY ");
	}

	if (int_mask & SINGLE_TAP){
		printf("SINGLE_TAP ");
	}

	if (int_mask & DOUBLE_TAP){
		printf("DOUBLE_TAP ");
	}
	
	if (int_mask & ACTIVITY){
		printf("ACTIVITY ");
	}

	if (int_mask & INACTIVITY){
		printf("INACTIVITY ");
	}

	if (int_mask & FREE_FALL){
		printf("FREE_FALL ");
	}

	if (int_mask & WATERMARK){
		printf("WATERMARK ");
	}

	if (int_mask & OVERRUN){
		printf("OVERRUN ");
	}

	printf("\r\n");

	adxl_dev_set_int_map(ptr->dev, adxl_dev_get_int_map(ptr->dev) ^ (int_mask));

	return ADXL_OK;

}



int axi_adxl_get_int_map(axi_adxl *ptr, uint8_t mask){
	return (adxl_dev_get_int_map(ptr->dev) & mask);
}



int axi_adxl_get_int_source(axi_adxl *ptr, uint8_t *interrupt_mask){

	if (ptr->init_flaq != 1){
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
		return ADXL_LINK_LOST;
	}

	*interrupt_mask = adxl_dev_get_int_source(ptr->dev);

	return ADXL_OK;
}



int axi_adxl_has_int_source(axi_adxl *ptr, uint8_t interrupt_mask){
	return (adxl_dev_get_int_source(ptr->dev) & interrupt_mask) ? TRUE : FALSE;
}





int axi_adxl_get_data(axi_adxl *ptr, adxl_data *data){


	if (ptr->init_flaq != 1){
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_GET_DATA] : has no init device");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_GET_DATA] : Link down");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_LINK_LOST;
	}

	(*data).x = ((int16_t)adxl_dev_get_datax1(ptr->dev) << 8) + (int16_t)adxl_dev_get_datax0(ptr->dev);
	(*data).y = ((int16_t)adxl_dev_get_datay1(ptr->dev) << 8) + (int16_t)adxl_dev_get_datay0(ptr->dev);
	(*data).z = ((int16_t)adxl_dev_get_dataz1(ptr->dev) << 8) + (int16_t)adxl_dev_get_dataz0(ptr->dev);

	return ADXL_OK;
}



int axi_adxl_get_data_float(axi_adxl *ptr, adxl_data_float *data_float){

	if (ptr->init_flaq != 1){
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_GET_DATA_FLOAT] : has no init device");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_GET_DATA_FLOAT] : Link down");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_LINK_LOST;
	}

	int16_t x = (int16_t)axi_adxl_get_datax(ptr);
	int16_t y = (int16_t)axi_adxl_get_datay(ptr);
	int16_t z = (int16_t)axi_adxl_get_datax(ptr);

	if (adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_FULL_RES){
		data_float->x = (float)x/SENSITIVITY_FULL_RES;
		data_float->y = (float)y/SENSITIVITY_FULL_RES;
		data_float->z = (float)z/SENSITIVITY_FULL_RES;

	}else{
		switch (adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_RANGE_MASK){

			case DATA_FORMAT_RANGE_2G:
				data_float->x = (float)x/SENSITIVITY_2G;
				data_float->y = (float)y/SENSITIVITY_2G;
				data_float->z = (float)z/SENSITIVITY_2G;
			break;

			case DATA_FORMAT_RANGE_4G:
				data_float->x = (float)x/SENSITIVITY_4G;
				data_float->y = (float)y/SENSITIVITY_4G;
				data_float->z = (float)z/SENSITIVITY_4G;
			break;

			case DATA_FORMAT_RANGE_8G:
				data_float->x = (float)x/SENSITIVITY_8G;
				data_float->y = (float)y/SENSITIVITY_8G;
				data_float->z = (float)z/SENSITIVITY_8G;
			break;

			case DATA_FORMAT_RANGE_16G:
				data_float->x = (float)x/SENSITIVITY_16G;
				data_float->y = (float)y/SENSITIVITY_16G;
				data_float->z = (float)z/SENSITIVITY_16G;
			break;

			default :
				printf("[ADXL_GET_DATA_FLOAT] : \r\n");
			break;
		}
	}

	return ADXL_OK;
}



int axi_adxl_set_fifo_mode(axi_adxl *ptr, enum fifo_mode_enum fifo_mode){

	if (ptr->init_flaq != 1){
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_SET_FIFO_MODE] : has no init device");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_SET_FIFO_MODE] : Link down");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_LINK_LOST;
	}

	printf("[ADXL_SET_FIFO_MODE] : changing FIFO mode from ");

	switch(adxl_dev_get_fifo_ctl(ptr->dev) & FIFO_CTL_FIFO_MODE){
		case FIFO_MODE_BYPASS : 
			printf("BYPASS ");
		break;

		case FIFO_MODE_FIFO : 
			printf("FIFO ");
		break;

		case FIFO_MODE_STREAM : 
			printf("STREAM ");
		break;

		case FIFO_MODE_TRIGGER : 
			printf("TRIGGER ");
		break;

		default : 
			printf("<undefined> ");
			return ADXL_UNCORRECT_VALUE;
		break;

	}

	printf("to ");

	switch(fifo_mode){
		case FIFO_MODE_BYPASS : 
			printf("BYPASS\r\n");
		break;

		case FIFO_MODE_FIFO : 
			printf("FIFO\r\n");
		break;

		case FIFO_MODE_STREAM : 
			printf("STREAM\r\n");
		break;

		case FIFO_MODE_TRIGGER : 
			printf("TRIGGER\r\n");
		break;

		default : 
			printf("<undefined> ");
			return ADXL_UNCORRECT_VALUE;
		break;

	}

	adxl_dev_set_fifo_ctl(ptr->dev, (adxl_dev_get_fifo_ctl(ptr->dev) & ~FIFO_CTL_FIFO_MODE) | fifo_mode);

	return ADXL_OK;
}



int axi_adxl_has_fifo_mode(axi_adxl *ptr, enum fifo_mode_enum fifo_mode){
	return ((adxl_dev_get_fifo_ctl(ptr->dev) & FIFO_CTL_FIFO_MODE) == fifo_mode) ? TRUE : FALSE;
}



int axi_adxl_set_trigger(axi_adxl *ptr, uint8_t trigger_flaq){
	if (ptr->init_flaq != 1){
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_SET_TRIGGER] : has no init device");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_SET_TRIGGER] : Link down");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_LINK_LOST;
	}

	adxl_dev_set_fifo_ctl(ptr->dev, (adxl_dev_get_fifo_ctl(ptr->dev) & ~FIFO_CTL_TRIGGER) | trigger_flaq);

	return ADXL_OK;

}	



int axi_adxl_get_trigger(axi_adxl *ptr, uint8_t *trigger_flaq){

	if (ptr->init_flaq != 1){
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_GET_TRIGGER] : has no init device");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_GET_TRIGGER] : Link down");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_LINK_LOST;
	}

	*trigger_flaq = (adxl_dev_get_fifo_ctl(ptr->dev) & FIFO_CTL_TRIGGER);

	return ADXL_OK;

}



int axi_adxl_set_samples(axi_adxl *ptr, uint8_t samples){

	if (ptr->init_flaq != 1){
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_SET_SAMPLES] : has no init device");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_SET_SAMPLES] : Link down");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_LINK_LOST;
	}

	if (samples > 31){
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_SET_SAMPLES] : incorrect value %d", samples);
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_UNCORRECT_VALUE;		
	}

	printf("[ADXL_SET_SAMPLES] : changing value from %d samples to %d samples\r\n", adxl_dev_get_fifo_ctl(ptr->dev) & FIFO_CTL_SAMPLES, samples);

	adxl_dev_set_fifo_ctl(ptr->dev, (adxl_dev_get_fifo_ctl(ptr->dev) & ~FIFO_CTL_SAMPLES) | samples);

	return ADXL_OK;
}



int axi_adxl_get_samples(axi_adxl *ptr, uint8_t *samples){

	if (ptr->init_flaq != 1){
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_GET_SAMPLES] : has no init device");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_GET_SAMPLES] : Link down");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_LINK_LOST;
	}

	*samples = (adxl_dev_get_fifo_ctl(ptr->dev) & FIFO_CTL_SAMPLES);

	return ADXL_OK;
}



int axi_adxl_get_fifo_sts_entries(axi_adxl *ptr, uint8_t *entries){

	if (ptr->init_flaq != 1){
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_GET_ENTRIES] : has no init device");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_GET_ENTRIES] : Link down");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_LINK_LOST;
	}

	*entries = adxl_dev_get_fifo_status(ptr->dev) & FIFO_STATUS_ENTRIES_MASK;

	return ADXL_OK;

}



int axi_adxl_has_fifo_sts_trigger(axi_adxl *ptr){
	return (adxl_dev_get_fifo_status(ptr->dev) & FIFO_STATUS_TRIGGER_MASK)	? TRUE : FALSE;
}



int axi_adxl_has_act_status(axi_adxl *ptr, enum act_tap_status_enum act){
	return (adxl_dev_get_act_tap_status(ptr->dev) & act) ? TRUE : FALSE;
}



int axi_adxl_has_tap_status(axi_adxl *ptr, enum act_tap_status_enum tap){
	return (adxl_dev_get_act_tap_status(ptr->dev) & tap) ? TRUE : FALSE;
}


int axi_adxl_has_sleep_status(axi_adxl *ptr){
	return (adxl_dev_get_act_tap_status(ptr->dev) & ASLEEP) ? TRUE : FALSE;
}



int axi_adxl_set_linking_mode(axi_adxl *ptr, int state){

	if (ptr->init_flaq != 1){
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_SET_LINKING] : has no init device");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_SET_LINKING] : Link down");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_LINK_LOST;
	}

	printf("Current state of linking mode is ");
	if (adxl_dev_get_power_ctl(ptr->dev) & POWER_CTL_LINK_MASK){
		textcolor(DEFAULT, STD, RED);
		printf("actived");
	}else{
		textcolor(DEFAULT, BLACK, GREEN);
		printf("inactived");
	}
    textcolor(DEFAULT, STD, STD);
	printf("\r\n");

	adxl_dev_set_power_ctl(ptr->dev, (adxl_dev_get_power_ctl(ptr->dev) & ~POWER_CTL_LINK_MASK) ^ (state & POWER_CTL_LINK_MASK));	

	return ADXL_OK;
}


int axi_adxl_get_linking_mode(axi_adxl *ptr, int *state){

	if (ptr->init_flaq != 1){
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_SET_LINKING] : has no init device");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
	    textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_SET_LINKING] : Link down");
	    textcolor(DEFAULT, STD, STD);
	    printf("\r\n");
		return ADXL_LINK_LOST;
	}

	*state = adxl_dev_get_power_ctl(ptr->dev) & POWER_CTL_LINK_MASK;

	return ADXL_OK;
}


int axi_adxl_has_linking_mode(axi_adxl *ptr){
	return (adxl_dev_get_power_ctl(ptr->dev) & POWER_CTL_LINK_MASK)? TRUE : FALSE;
}
