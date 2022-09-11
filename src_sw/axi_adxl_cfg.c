#include "axi_adxl.h"



/*
 * Initialize function perform next procedures
 * 1) setup cfg/dev baseaddress
 * 2) checking current activity :
 * 		- if currently work (calibration/single request/interval requestion), then
 */
int axi_adxl_init(axi_adxl* ptr, uint32_t baseaddr_cfg, uint32_t baseaddr_dev, uint8_t iic_address) {

	// Setup baseaddresses of component for sw struct
#ifdef AXI_ADXL_LOGGING
	printf("\t\t[ADXL_INIT] : set address to pointers\r\n");
#endif
	ptr->cfg = (adxl_cfg*)baseaddr_cfg;
	ptr->dev = (adxl_dev*)baseaddr_dev;
#ifdef AXI_ADXL_LOGGING
	printf("\t\t\tconfiguration address space : 0x%08x\r\n", baseaddr_cfg);
	printf("\t\t\tconfiguration address space : 0x%08x\r\n", baseaddr_dev);
#endif

	int timer = TIMER_LIMIT;


	/*If component working with device, we must stop them for correct initialization*/
	if (adxl_cfg_ctl_work(ptr->cfg) || adxl_cfg_ctl_interval_requestion(ptr->cfg)) {

		printf("\t\t[ADXL_INIT] : component currently perform operation\r\n");

		if (adxl_cfg_ctl_interval_requestion(ptr->cfg)) {

			printf("\t\t[ADXL_INIT] : interval requestion disable ");

			adxl_cfg_ctl_interval_requestion_disable(ptr->cfg);

			/*Component perform operation?*/
			while (adxl_cfg_ctl_work(ptr->cfg)) {

				if (timer == 0) {
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
		if (adxl_cfg_ctl_irq_allowed(ptr->cfg)) {

			printf("\t\t[ADXL_INIT] : irq allowed. Unallow process ");

			adxl_cfg_ctl_irq_unallow(ptr->cfg);

			while (adxl_cfg_ctl_work(ptr->cfg)) {
				if (timer == 0) {
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

	printf("\t\t[ADXL_INIT] : reset internal logic ");
	timer = TIMER_LIMIT;
	adxl_cfg_ctl_reset(ptr->cfg);
	while (!adxl_cfg_ctl_reset_completed(ptr->cfg)) {
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
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");

	/*Set I2C Address for work with device*/
	printf("\t\t[ADXL_INIT] : install iic address of device <0x%02x> ", iic_address);
	adxl_cfg_ctl_set_iic_address(ptr->cfg, iic_address);

	timer = TIMER_LIMIT;

	while (adxl_cfg_ctl_get_iic_address(ptr->cfg) != iic_address) {
		if (timer == 0) {
			textcolor(DEFAULT, BLACK, RED);
			printf("failure");
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

	printf("\t\t[ADXL_INIT] : perform single request ");
	adxl_cfg_ctl_single_request(ptr->cfg);
	timer = TIMER_LIMIT;
	while (!adxl_cfg_ctl_single_request_complete(ptr->cfg)) {
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

	printf("\t\t[ADXL_INIT] : link ");
	while (!adxl_cfg_ctl_link(ptr->cfg)) {
		if (timer == 0) {
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
	if (adxl_cfg_ctl_link(ptr->cfg)) {
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
int axi_adxl_reset(axi_adxl* ptr) {

	int timer = TIMER_LIMIT;

	if (!axi_adxl_has_init(ptr)) {
		textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_RESET] : has no init device");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
		return ADXL_UNINIT;
	}

	/*STOP all msmt/interrupts from device*/
	if (adxl_cfg_ctl_link(ptr->cfg)) {
		/*Disable measure*/
		timer = TIMER_LIMIT;

		printf("\t[ADXL_RESET] : disable device measurement capability ");
		adxl_dev_set_power_ctl(ptr->dev, (adxl_dev_get_power_ctl(ptr->dev) & ~POWER_CTL_MEASURE_MASK));
		while ((adxl_dev_get_power_ctl(ptr->dev) & POWER_CTL_MEASURE_MASK)) {
			if (timer == 0) {
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
		while ((adxl_dev_get_int_enable(ptr->dev) & INTR_ALL_MASK)) {
			if (timer == 0) {
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

	if (adxl_cfg_ctl_work(ptr->cfg) || adxl_cfg_ctl_interval_requestion(ptr->cfg)) {

		printf("\t[ADXL_RESET] : component currently perform operation\r\n");

		if (adxl_cfg_ctl_interval_requestion(ptr->cfg)) {

			printf("\t[ADXL_RESET] : interval requestion disable ");
			adxl_cfg_ctl_interval_requestion_disable(ptr->cfg);
			while (adxl_cfg_ctl_work(ptr->cfg)) {
				if (timer == 0) {
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

		if (adxl_cfg_ctl_irq_allowed(ptr->cfg)) {
			printf("\t[ADXL_RESET] : irq allowed. Unallow process ");
			adxl_cfg_ctl_irq_unallow(ptr->cfg);
			while (adxl_cfg_ctl_work(ptr->cfg)) {
				if (timer == 0) {
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
	while (!adxl_cfg_ctl_reset_completed(ptr->cfg)) {
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
int axi_adxl_enable_interval_requestion(axi_adxl* ptr) {

	if (!axi_adxl_has_init(ptr)) {
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_INTERVAL_REQ] : has no init device");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
		return ADXL_UNINIT;
	}

	adxl_cfg_ctl_interval_requestion_enable(ptr->cfg);
	int timer = TIMER_LIMIT;

	printf("\t\t[ADXL_INTERVAL_REQ] : interval requestion ");

	while (!adxl_cfg_ctl_interval_requestion(ptr->cfg)) {
		if (timer == 0) {
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


int axi_adxl_has_reset(axi_adxl* ptr) {
	return (adxl_cfg_ctl_reset_completed(ptr->cfg)) ? FALSE : TRUE;
}


int axi_adxl_disable_interval_requestion(axi_adxl* ptr) {
	if (!axi_adxl_has_init(ptr)) {
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_DISABLE_INTERVAL_REQ] : has no init device");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
		return ADXL_UNINIT;
	}

	adxl_cfg_ctl_interval_requestion_disable(ptr->cfg);

	int timer = TIMER_LIMIT;

	printf("\t\t[ADXL_INTERVAL_REQ] : stopping requestion ");

	while (adxl_cfg_ctl_interval_requestion(ptr->cfg)) {
		if (timer == 0) {
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

	return ADXL_OK;
}


int axi_adxl_has_interval_requestion(axi_adxl* ptr) {
	return (adxl_cfg_ctl_interval_requestion(ptr->cfg));
}

/*Get requestion interval and return on top*/
int axi_adxl_get_requestion_interval(axi_adxl* ptr, uint32_t* requestion_interval) {
	if (!axi_adxl_has_init(ptr)) {
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_REQ_INT] : has no init device");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_REQ_INT] : link lost");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
		return ADXL_LINK_LOST;
	}

	*requestion_interval = adxl_cfg_get_request_interval(ptr->cfg);

	return ADXL_OK;
}



int axi_adxl_set_requestion_interval(axi_adxl* ptr, uint32_t requestion_interval) {

	if (!axi_adxl_has_init(ptr)) {
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_REQ_INT] : has no init device");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_REQ_INT] : link lost");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
		return ADXL_LINK_LOST;
	}

	printf("\t\t[ADXL_SET_REQ_INT] : change value of requestion interval to %d ", requestion_interval);

	adxl_cfg_set_request_interval(ptr->cfg, requestion_interval);
	int timer = TIMER_LIMIT;
	while (adxl_cfg_get_request_interval(ptr->cfg) != requestion_interval) {
		if (timer == 0) {
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
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");

	return ADXL_OK;

}

int axi_adxl_enable_single_request(axi_adxl* ptr) {

	if (!axi_adxl_has_init(ptr)) {
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_PRFRM_SNGL_REQ] : has no init device\r\n");
		textcolor(DEFAULT, STD, STD);
		return ADXL_UNINIT;
	}

	printf("\t\t[ADXL_PRFRM_SNGL_REQ] : perform single request ");

	adxl_cfg_ctl_single_request(ptr->cfg);

	int timer = TIMER_LIMIT;

	while (!adxl_cfg_ctl_single_request_complete(ptr->cfg)) {
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



int axi_adxl_has_single_request_completed(axi_adxl* ptr) {
	return (adxl_cfg_ctl_single_request_complete(ptr->cfg));
}



int axi_adxl_set_start_address(axi_adxl* ptr, uint8_t start_address) {

	if (!axi_adxl_has_init(ptr)) {
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_STARTADDR] : has no init device");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
		return ADXL_UNINIT;
	}

	if (start_address < 0 || start_address > 57) {
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_STARTADDR] : incorrect <address> value: ");
		textcolor(DEFAULT, BLACK, RED);
		printf("%d", start_address);
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
		return ADXL_UNCORRECT_VALUE;
	}

	if ((start_address + adxl_cfg_get_single_req_params_size(ptr->cfg)) > 58) {
		textcolor(DEFAULT, RED, STD);
		printf("\t[MENU] : current address and size greater than 58 bytes: ");
		textcolor(DEFAULT, BLACK, RED);
		printf("addr =  %d, size = %d", start_address, adxl_cfg_get_single_req_params_size(ptr->cfg));
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
		return ADXL_UNCORRECT_VALUE;
	}

	printf("\t\t[ADXL_SET_STARTADDR] : change value of address to %d ", start_address);

	adxl_cfg_set_single_req_params_address(ptr->cfg, start_address);

	int timer = TIMER_LIMIT;

	while (adxl_cfg_get_single_req_params_address(ptr->cfg) != start_address) {
		if (timer == 0) {
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
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");

	return ADXL_OK;
}



int axi_adxl_get_start_address(axi_adxl* ptr, uint8_t* start_address) {
	if (!axi_adxl_has_init(ptr)) {
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_STARTADDR] : has no init device");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
		return ADXL_UNINIT;
	}

	*start_address = adxl_cfg_get_single_req_params_address(ptr->cfg);

	return ADXL_OK;
}



int axi_adxl_set_size(axi_adxl* ptr, uint8_t size) {
	if (!axi_adxl_has_init(ptr)) {
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_SIZE] : has no init device");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
		return ADXL_UNINIT;
	}


	if (size < 1 || size > 58) {
		textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_SET_SIZE] : incorrect <size> value: ");
		textcolor(DEFAULT, BLACK, RED);
		printf("%d", size);
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
		return ADXL_UNCORRECT_VALUE;
	}

	if ((adxl_cfg_get_single_req_params_address(ptr->cfg) + size) > 58) {
		textcolor(DEFAULT, RED, STD);
		printf("\t[ADXL_SET_SIZE] : current address and size greater than 58 bytes: ");
		textcolor(DEFAULT, BLACK, RED);
		printf("addr %d size %d", adxl_cfg_get_single_req_params_address(ptr->cfg), size);
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
		return ADXL_UNCORRECT_VALUE;
	}

	printf("\t\t[ADXL_SET_SIZE] : change value of size to %d ", size);

	adxl_cfg_set_single_req_params_size(ptr->cfg, size);

	int timer = TIMER_LIMIT;

	while (adxl_cfg_get_single_req_params_size(ptr->cfg) != size) {
		if (timer == 0) {
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
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");

	return ADXL_OK;
}



int axi_adxl_get_size(axi_adxl* ptr, uint8_t* size) {
	if (!axi_adxl_has_init(ptr)) {
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_SIZE] : has no init device");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
		return ADXL_UNINIT;
	}

	*size = adxl_cfg_get_single_req_params_size(ptr->cfg);

	return ADXL_OK;
}



int axi_adxl_switch_irq_allow(axi_adxl* ptr) {

	if (!axi_adxl_has_init(ptr)) {
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SWITCH_IRQ_ALLOW] : has no init device\r\n");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
		return ADXL_UNINIT;
	}

	printf("\t\t[ADXL_SWITCH_IRQ_ALLOW] : switching irq allow from ");
	int allowed = adxl_cfg_ctl_irq_allowed(ptr->cfg);
	if (allowed) {
		textcolor(DEFAULT, GREEN, STD);
		printf("active");
		textcolor(DEFAULT, STD, STD);
		printf(" to ");
		textcolor(DEFAULT, RED, STD);
		printf("inactive");
		adxl_cfg_ctl_irq_unallow(ptr->cfg);
	}
	else {
		textcolor(DEFAULT, RED, STD);
		printf("inactive");
		textcolor(DEFAULT, STD, STD);
		printf(" to ");
		textcolor(DEFAULT, GREEN, STD);
		printf("active");
		adxl_cfg_ctl_irq_allow(ptr->cfg);

	}

	printf(" ");
	int timer = TIMER_LIMIT;

	if (allowed) {
		while (adxl_cfg_ctl_irq_allowed(ptr->cfg)) {
			if (timer == 0) {
				textcolor(DEFAULT, BLACK, RED);
				printf("failed");
				textcolor(DEFAULT, STD, STD);
				printf("\r\n");
				return ADXL_TIMEOUT;
			}
			timer--;
			printf(".");
		}
	}
	else {
		while (!adxl_cfg_ctl_irq_allowed(ptr->cfg)) {
			if (timer == 0) {
				textcolor(DEFAULT, BLACK, RED);
				printf("failed");
				textcolor(DEFAULT, STD, STD);
				printf("\r\n");
				return ADXL_TIMEOUT;
			}
			timer--;
			printf(".");
		}
	}

	textcolor(DEFAULT, BLACK, GREEN);
	printf("complete");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");

	return ADXL_OK;
}



int axi_adxl_has_irq_allow(axi_adxl* ptr) {
	return (adxl_cfg_ctl_irq_allowed(ptr->cfg)) ? TRUE : FALSE;
}



int axi_adxl_calibration_start(axi_adxl* ptr) {

	if (!axi_adxl_has_init(ptr)) {
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_CALIBRATION] : has no init device");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
		return ADXL_UNINIT;
	}

	adxl_cfg_calibration(ptr->cfg);

	printf("\t\t[ADXL_CALIBRATION] : calibration in progress and stopped automatically\r\n");

	//while (!adxl_cfg_calibration_completed(ptr->cfg)) {
	//	printf(".");
	//}

	return ADXL_OK;
}



int axi_adxl_set_iic_address(axi_adxl* ptr, uint8_t iic_address) {

	if (!axi_adxl_has_init(ptr)) {
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_IIC_ADDRESS] : has no init device");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
		return ADXL_UNINIT;
	}
	printf("\t\t[ADXL_SET_IIC_ADDRESS] : switching address from 0x%02x to 0x%02x ", adxl_cfg_ctl_get_iic_address(ptr->cfg), iic_address);
	adxl_cfg_ctl_set_iic_address(ptr->cfg, iic_address);

	int timer = TIMER_LIMIT;

	while (adxl_cfg_ctl_get_iic_address(ptr->cfg) != iic_address) {
		if (timer == 0) {
			textcolor(DEFAULT, BLACK, RED);
			printf("failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
			return ADXL_TIMEOUT;

			return ADXL_TIMEOUT;
		}
		printf(".");
		timer--;
	}
	textcolor(DEFAULT, BLACK, GREEN);
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");

	return ADXL_OK;
}



int axi_adxl_get_iic_address(axi_adxl* ptr, uint8_t* iic_address) {
	if (!axi_adxl_has_init(ptr)) {
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_IIC_ADDR] : has no init device");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
		return ADXL_UNINIT;
	}

	*iic_address = adxl_cfg_ctl_get_iic_address(ptr->cfg);

	return ADXL_OK;
}



int axi_adxl_has_work(axi_adxl* ptr) {
	return (adxl_cfg_ctl_work(ptr->cfg)) ? TRUE : FALSE;
}



int axi_adxl_has_link(axi_adxl* ptr) {
	return (adxl_cfg_ctl_link(ptr->cfg)) ? TRUE : FALSE;
}


int axi_adxl_get_version_major(axi_adxl* ptr, uint8_t* major) {
	if (!axi_adxl_has_init(ptr)) {
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_VERS_MAJOR] : has no init device");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
		return ADXL_UNINIT;
	}

	*major = adxl_cfg_ctl_get_version_major(ptr->cfg);

	return ADXL_OK;
}


int axi_adxl_get_version_minor(axi_adxl* ptr, uint8_t* minor) {
	if (!axi_adxl_has_init(ptr)) {
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_VERS_MAJOR] : has no init device");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
		return ADXL_UNINIT;
	}

	*minor = adxl_cfg_ctl_get_version_minor(ptr->cfg);

	return ADXL_OK;
}



int axi_adxl_get_calibration_pwr_count_limit(axi_adxl* ptr, uint8_t* pwr_count_limit) {
	if (!axi_adxl_has_init(ptr)) {
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_VERS_MAJOR] : has no init device");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
		return ADXL_UNINIT;
	}

	*pwr_count_limit = adxl_cfg_get_calibration_count_limit(ptr->cfg);

	return ADXL_OK;

}



int axi_adxl_set_calibration_pwr_count_limit(axi_adxl* ptr, uint8_t pwr_count_limit) {

	if (!axi_adxl_has_init(ptr)) {
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_CALIB_PWR_CNT_LIMIT] : has no init device");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
		return ADXL_UNINIT;
	}

	if (pwr_count_limit < 0 || pwr_count_limit > 31) {
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_CALIB_PWR_CNT_LIMIT] : incorrect calibration count value : ");
		textcolor(DEFAULT, BLACK, RED);
		printf("%d", pwr_count_limit);
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
		return ADXL_UNCORRECT_VALUE;
	}

	printf("[ADXL_SET_CALIB_PWR_CNT_LIMIT] : set value %d to power of calibration count ", pwr_count_limit);

	adxl_cfg_set_calibration_count_limit(ptr->cfg, pwr_count_limit);

	int timer = TIMER_LIMIT;

	while (adxl_cfg_get_calibration_count_limit(ptr->cfg) != pwr_count_limit) {
		if (timer == 0) {
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

	return ADXL_OK;

}



int axi_adxl_has_calibration_in_progress(axi_adxl* ptr) {
	return (adxl_cfg_calibration_in_progress(ptr->cfg)) ? TRUE : FALSE;
}



int axi_adxl_has_calibration_complete(axi_adxl* ptr) {
	return (adxl_cfg_calibration_completed(ptr->cfg)) ? TRUE : FALSE;
}



int axi_adxl_get_read_valid_count(axi_adxl* ptr, uint32_t* read_valid_count) {
	if (!axi_adxl_has_init(ptr)) {
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_RD_VALID_CNT] : has no init device");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
		return ADXL_UNINIT;
	}

	*read_valid_count = adxl_cfg_get_read_valid_count(ptr->cfg);

	return ADXL_OK;


}



int axi_adxl_get_write_valid_count(axi_adxl* ptr, uint32_t* write_valid_count) {
	if (!axi_adxl_has_init(ptr)) {
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_WR_VALID_CNT] : has no init device");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
		return ADXL_UNINIT;
	}

	*write_valid_count = adxl_cfg_get_write_valid_count(ptr->cfg);

	return ADXL_OK;
}



int axi_adxl_get_read_transactions(axi_adxl* ptr, uint64_t* read_transactions) {
	if (!axi_adxl_has_init(ptr)) {
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_RD_TRANSCTN] : has no init device");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
		return ADXL_UNINIT;
	}

	*read_transactions = (((uint64_t)adxl_cfg_get_read_transactions_msb(ptr->cfg)) << 32) + (uint64_t)adxl_cfg_get_read_transactions_lsb(ptr->cfg);

	return ADXL_OK;
}



int axi_adxl_get_write_transactions(axi_adxl* ptr, uint64_t* write_transactions) {
	if (!axi_adxl_has_init(ptr)) {
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_WR_TRANSCTN] : has no init device");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
		return ADXL_UNINIT;
	}

	*write_transactions = (((uint64_t)adxl_cfg_get_write_transactions_msb_reg(ptr->cfg)) << 32) + (uint64_t)adxl_cfg_get_write_transactions_lsb_reg(ptr->cfg);

	return ADXL_OK;
}



int axi_adxl_get_clk_period(axi_adxl* ptr, uint32_t* clk_period) {
	if (!axi_adxl_has_init(ptr)) {
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_CLK_PERIOD] : has no init device");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
		return ADXL_UNINIT;
	}

	*clk_period = adxl_cfg_get_clk_period(ptr->cfg);

	return ADXL_OK;
}



int axi_adxl_get_opt_request_interval(axi_adxl* ptr, uint64_t* opt_request_interval) {
	if (!axi_adxl_has_init(ptr)) {
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_OPT_REQ_INTRVL] : has no init device");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
		return ADXL_UNINIT;
	}

	*opt_request_interval = ((((uint64_t)adxl_cfg_get_opt_request_interval_msb_reg(ptr->cfg) & CFG_OPT_REQUEST_INTERVAL_MSB_MASK) << 32)) + (uint64_t)adxl_cfg_get_opt_request_interval_lsb_reg(ptr->cfg);

	return ADXL_OK;
}



int axi_adxl_get_calibration_time(axi_adxl* ptr, uint64_t* calibration_time) {
	if (!axi_adxl_has_init(ptr)) {
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_CALIB_TIME] : has no init device");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
		return ADXL_UNINIT;
	}

	*calibration_time = (((uint64_t)adxl_cfg_get_calibration_time_msb_reg(ptr->cfg)) << 32) + (uint64_t)adxl_cfg_get_calibration_time_lsb_reg(ptr->cfg);

	return ADXL_OK;
}



int axi_adxl_get_data_width(axi_adxl* ptr, uint32_t* data_width) {
	if (!axi_adxl_has_init(ptr)) {
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_DW] : has no init device");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
		return ADXL_UNINIT;
	}

	*data_width = adxl_cfg_get_data_width_reg(ptr->cfg);

	return ADXL_OK;
}




int axi_adxl_irq_ack(axi_adxl* ptr) {

	adxl_cfg_ctl_intr_ack(ptr->cfg);

	return ADXL_OK;
}

