#include "axi_adxl.h"


/*
 * Function initialize adxl software structure of adxl_device
 * */
int axi_adxl_init(axi_adxl *ptr, uint32_t cfg_baseaddr, uint32_t dev_baseaddr, uint8_t i2c_addr){
    printf("\t[INIT] : initialize device with next parameters\r\n");

    /*Assign baseaddress for configuration*/
    printf("\t\tconfig regspace : 0x%08x\r\n", (unsigned int)cfg_baseaddr);
    ptr->cfg = (adxl_cfg*)cfg_baseaddr;

    /*Assign baseaddress for device*/
    printf("\t\tdevice regspace : 0x%08x\r\n", (unsigned int)dev_baseaddr);
    ptr->dev = (adxl_dev*)dev_baseaddr;

    uint32_t timeout_timer = TIMEOUT_TIMER_LIMIT;

    printf("\t[INIT] : stop requesting");
    adxl_cfg_ctl_request_disable(ptr->cfg);
    while(adxl_cfg_ctl_has_on_work(ptr->cfg)){
        printf(".");
    }
    printf(" complete\r\n");


    printf("\t[INIT] : perform reset");
    adxl_cfg_ctl_reset(ptr->cfg);

    while(adxl_cfg_ctl_has_reset(ptr->cfg)){
        if (!timeout_timer){
            printf("timeout error\r\n");
            return ERR_RESET_INFINITE;
        }
        printf(".");
        usleep(1000);
    }
    printf(" completed\r\n");

    printf("\t[INIT] : set i2c address\r\n");
    adxl_cfg_ctl_set_iic_address(ptr->cfg, i2c_addr);

    printf("\t[INIT] : perform single request");
    adxl_cfg_ctl_single_request(ptr->cfg);
    while(!adxl_cfg_ctl_has_single_request_performed(ptr->cfg)){
        printf(".");
        usleep(1000);
    }
    printf(" completed\r\n");
    adxl_cfg_ctl_clear_single_request_flaq(ptr->cfg);

    printf("\t[INIT] : link ");
    if (!adxl_cfg_ctl_has_link(ptr->cfg)){
    	printf("failed\r\n");
    	return ERR_LINK_LOST;
    }
    printf("established\r\n");

    ptr->has_init = 1;

    printf("\t[INIT] : device initialization completed\r\n");

    return ADXL_OK;
}


int axi_adxl_setup_bandwidth(axi_adxl *ptr, uint8_t value){

    if (!axi_adxl_has_init(ptr)){
        printf("\t[SET_BW] : device uninitialized\r\n");
        return ERR_UNINIT;
    }

	printf("\t[SET_BW] : setup new bandwidth value : ");
        switch (value){
        case BW_RATE_3200 :
                printf("3200 Hz\r\n");
                printf("\t[SET_BW] : request interval : %3.6f seconds\r\n", (float)1/(float)3200);
            break;
        case BW_RATE_1600 :
                printf("1600 Hz\r\n");
                printf("\t[SET_BW] : request interval : %3.6f seconds\r\n", (float)1/(float)1600);
            break;
        case BW_RATE_800  :
                printf("800 Hz\r\n");
                printf("\t[SET_BW] : request interval : %3.6f seconds\r\n", (float)1/(float)800);
            break;
        case BW_RATE_400  :
                printf("400 Hz\r\n");
                printf("\t[SET_BW] : request interval : %3.6f seconds\r\n", (float)1/(float)400);
            break;
        case BW_RATE_200  :
                printf("200 Hz\r\n");
                printf("\t[SET_BW] : request interval : %3.6f seconds\r\n", (float)1/(float)200);
            break;
        case BW_RATE_100  :
                printf("100 Hz\r\n");
                printf("\t[SET_BW] : request interval : %3.6f seconds\r\n", (float)1/(float)100);
            break;
        case BW_RATE_50   :
                printf("50 Hz\r\n");
                printf("\t[SET_BW] : request interval : %3.6f seconds\r\n", (float)1/(float)50);
            break;
        case BW_RATE_25   :
                printf("25 Hz\r\n");
                printf("\t[SET_BW] : request interval : %3.6f seconds\r\n", (float)1/(float)25);
            break;
        case BW_RATE_12_5 :
                printf("12.5 Hz\r\n");
                printf("\t[SET_BW] : request interval : %3.6f seconds\r\n", (float)1/(float)12.5);
            break;
        case BW_RATE_6_25 :
                printf("6.25 Hz\r\n");
                printf("\t[SET_BW] : request interval : %3.6f seconds\r\n", (float)1/(float)6.25);
            break;
        case BW_RATE_3_13 :
                printf("3.13 Hz\r\n");
                printf("\t[SET_BW] : request interval : %3.6f seconds\r\n", (float)1/(float)3.13);
            break;
        case BW_RATE_1_56 :
                printf("1.56 Hz\r\n");
                printf("\t[SET_BW] : request interval : %3.6f seconds\r\n", (float)1/(float)1.56);
            break;
        case BW_RATE_0_78 :
                printf("0.78 Hz\r\n");
                printf("\t[SET_BW] : request interval : %3.6f seconds\r\n", (float)1/(float)0.78);
            break;
        case BW_RATE_0_39 :
                printf("0.39 Hz\r\n");
                printf("\t[SET_BW] : request interval : %3.6f seconds\r\n", (float)1/(float)0.39);
            break;
        case BW_RATE_0_20 :
                printf("0.20 Hz\r\n");
                printf("\t[SET_BW] : request interval : %3.6f seconds\r\n", (float)1/(float)0.20);
            break;
        case BW_RATE_0_10 :
                printf("0.10 Hz\r\n");
                printf("\t[SET_BW] : request interval : %3.6f seconds\r\n", (float)1/(float)0.10);
            break;
        case BW_RATE_400_LP  :
                printf("400 Hz\r\n");
                printf("\t[SET_BW] : request interval : %3.6f seconds\r\n", (float)1/(float)400);
                printf("\t[SET_BW] : low power mode activated\r\n");
            break;
        case BW_RATE_200_LP  :
                printf("200 Hz\r\n");
                printf("\t[SET_BW] : request interval : %3.6f seconds\r\n", (float)1/(float)200);
                printf("\t[SET_BW] : low power mode activated\r\n");
            break;
        case BW_RATE_100_LP  :
                printf("100 Hz\r\n");
                printf("\t[SET_BW] : request interval : %3.6f seconds\r\n", (float)1/(float)100);
                printf("\t[SET_BW] : low power mode activated\r\n");
            break;
        case BW_RATE_50_LP   :
                printf("50 Hz\r\n");
                printf("\t[SET_BW] : request interval : %3.6f seconds\r\n", (float)1/(float)50);
                printf("\t[SET_BW] : low power mode activated\r\n");
            break;
        case BW_RATE_25_LP   :
                printf("25 Hz\r\n");
                printf("\t[SET_BW] : request interval : %3.6f seconds\r\n", (float)1/(float)25);
                printf("\t[SET_BW] : low power mode activated\r\n");
            break;
        case BW_RATE_12_5_LP :
                printf("12.5 Hz\r\n");
                printf("\t[SET_BW] : request interval : %3.6f seconds\r\n", (float)1/(float)12.5);
                printf("\t[SET_BW] : low power mode activated\r\n");
            break;
            default:
                printf("<undefined value>\r\n");
                return ERROR_UNDEF_VALUE;
            break;
    }

    adxl_dev_set_bw_rate(ptr->dev, value);

	return ADXL_OK;
}



int axi_adxl_start_requesting(axi_adxl *ptr){

	if (!axi_adxl_has_init(ptr)){
		printf("\t[START_REQ] : device uninitialized\r\n");
		return ERR_UNINIT;
	}

	printf("\t[START_REQ] : set optimal request interval %3.9f seconds\r\n", (float)adxl_cfg_get_opt_request_interval(ptr->cfg)/(float)adxl_cfg_get_clk_period(ptr->cfg));
	adxl_cfg_set_request_interval(ptr->cfg, adxl_cfg_get_opt_request_interval(ptr->cfg));

	printf("\t[START_REQ] : enable requesting");
	adxl_cfg_ctl_request_enable(ptr->cfg);
	while (!adxl_cfg_ctl_has_on_work(ptr->cfg)){
		printf(".");
	}
	printf(" completed\r\n");

	return ADXL_OK;
}



int axi_adxl_stop_requesting(axi_adxl *ptr){
	if (!axi_adxl_has_init(ptr)){
		printf("\t[STOP_REQ] : device uninitialized\r\n");
		return ERR_UNINIT;
	}

	printf("\t[STOP_REQ] : stop requesting");
	adxl_cfg_ctl_request_disable(ptr->cfg);
	while (adxl_cfg_ctl_has_on_work(ptr->cfg)){
		printf(".");
	}
	printf(" completed");

	return ADXL_OK;
}


int axi_adxl_start_measure(axi_adxl *ptr, enum range range_value){
	if (!axi_adxl_has_init(ptr)){
		printf("\t[START_MSMT] : device uninitialized\r\n");
		return ERR_UNINIT;
	}

    axi_adxl_setup_range(ptr, range_value);

	printf("\t[START_MSMT] : enable measurement ");
	adxl_dev_set_power_ctl(ptr->dev, POWER_CTL_MEASURE_MASK);
    printf("complete\r\n");

	return ADXL_OK;
}


int axi_adxl_stop_measure(axi_adxl *ptr){
	if (!axi_adxl_has_init(ptr)){
		printf("\t[STOP_MSMT] : device uninitialized\r\n");
		return ERR_UNINIT;
	}

	adxl_dev_set_power_ctl(ptr->dev, ~POWER_CTL_MEASURE_MASK);
	printf("[STOP_MSMT] : disable measure complete\r\n");
	return ADXL_OK;
}



int axi_adxl_get_axis(axi_adxl *ptr){

	if (!axi_adxl_has_init(ptr)){
		printf("[GET_AXIS] : device uninitialized\r\n");
		return ERR_UNINIT;
	}

	ptr->axis.x = ((uint16_t)adxl_dev_get_datax1(ptr->dev)<<8) + (uint16_t)adxl_dev_get_datax0(ptr->dev);
	ptr->axis.y = ((uint16_t)adxl_dev_get_datay1(ptr->dev)<<8) + (uint16_t)adxl_dev_get_datay0(ptr->dev);
	ptr->axis.z = ((uint16_t)adxl_dev_get_dataz1(ptr->dev)<<8) + (uint16_t)adxl_dev_get_dataz0(ptr->dev);

	return ADXL_OK;
}


int axi_adxl_calibration(axi_adxl *ptr, int count){
	if (!axi_adxl_has_init(ptr)){
		printf("[CALIBRTN] : device uninitialized\r\n");
		return ERR_UNINIT;
	}

	adxl_cfg_set_calibration_count_limit(ptr->cfg, count);

	printf("\t[CALIBRTN] : start calibration for %d times", adxl_cfg_get_calibration_count_limit(ptr->cfg));

	adxl_cfg_ctl_set_calibration(ptr->cfg);
	//
	while(adxl_cfg_ctl_get_calibration(ptr->cfg)){
		printf(".");
		sleep(1);
	}

	printf(" complete, calibration time : %f\r\n", (float)adxl_cfg_get_calibration_time(ptr->cfg)/(float)adxl_cfg_get_clk_period(ptr->cfg));

	printf("\t\toffset x: %d\r\n", (int8_t)adxl_dev_get_ofsx(ptr->dev));
	printf("\t\toffset y: %d\r\n", (int8_t)adxl_dev_get_ofsy(ptr->dev));
	printf("\t\toffset z: %d\r\n", (int8_t)adxl_dev_get_ofsz(ptr->dev));

	return ADXL_OK;
}

int axi_adxl_get_gravity(axi_adxl *ptr, g_coord *g_ptr){

	int16_t data_x = (int16_t)axi_adxl_get_datax(ptr);
	int16_t data_y = (int16_t)axi_adxl_get_datay(ptr);
	int16_t data_z = (int16_t)axi_adxl_get_dataz(ptr);

	if (adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_FULL_RES){
		g_ptr->x = (float)data_x/SENSITIVITY_FULL_RES;
		g_ptr->y = (float)data_y/SENSITIVITY_FULL_RES;
		g_ptr->z = (float)data_z/SENSITIVITY_FULL_RES;
	}else{
		switch (adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_RANGE_MASK){

			case DATA_FORMAT_RANGE_2G:
				g_ptr->x = (float)data_x/SENSITIVITY_2G;
				g_ptr->y = (float)data_y/SENSITIVITY_2G;
				g_ptr->z = (float)data_z/SENSITIVITY_2G;
			break;

			case DATA_FORMAT_RANGE_4G:
				g_ptr->x = (float)data_x/SENSITIVITY_4G;
				g_ptr->y = (float)data_y/SENSITIVITY_4G;
				g_ptr->z = (float)data_z/SENSITIVITY_4G;
			break;

			case DATA_FORMAT_RANGE_8G:
				g_ptr->x = (float)data_x/SENSITIVITY_8G;
				g_ptr->y = (float)data_y/SENSITIVITY_8G;
				g_ptr->z = (float)data_z/SENSITIVITY_8G;
			break;

			case DATA_FORMAT_RANGE_16G:
				g_ptr->x = (float)data_x/SENSITIVITY_16G;
				g_ptr->y = (float)data_y/SENSITIVITY_16G;
				g_ptr->z = (float)data_z/SENSITIVITY_16G;
			break;

			default :
				printf("[GET_GRVTY] : Unknown data format : 0x%02x\r\n", adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_RANGE_MASK);
			break;
		}
	}
	return ADXL_OK;
}



int axi_adxl_setup_range(axi_adxl *ptr, enum range range_value){
    if (!axi_adxl_has_init(ptr)){
        printf("\t[START_MSMT] : device uninitialized\r\n");
        return ERR_UNINIT;
    }

    printf("\t[SETUP_RANGE] : setup range value ");
    switch (range_value){

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

    adxl_dev_set_data_format(ptr->dev, (adxl_dev_get_data_format(ptr->dev) & ~(DATA_FORMAT_FULL_RES_MASK | DATA_FORMAT_RANGE_MASK)) | range_value);

    return ADXL_OK;
}



int axi_adxl_enable_interrupt(axi_adxl *ptr, uint8_t mask){
    if (!axi_adxl_has_init(ptr)){
        printf("\t[INT_ENABLE] : device uninitialized\r\n");
        return ERR_UNINIT;
    }

    if (!mask){
        printf("\t[INT_ENABLE] : no interrupt selected\r\n");
        return ADXL_OK;
    }

    if (!adxl_cfg_ctl_has_irq_allowed(ptr->cfg)){
        adxl_cfg_ctl_irq_allow(ptr->cfg);
    }

    adxl_dev_set_int_map(ptr->dev, 0x00);

    printf("\t[INT_ENABLE] : next interrupts is active for current time:\r\n");
    printf("\t\tDATA_READY: %s\r\n", (adxl_dev_get_int_enable(ptr->dev) & DATA_READY) ? "yes" : "no");
    printf("\t\tSINGLE_TAP: %s\r\n", (adxl_dev_get_int_enable(ptr->dev) & SINGLE_TAP) ? "yes" : "no");
    printf("\t\tDOUBLE_TAP: %s\r\n", (adxl_dev_get_int_enable(ptr->dev) & DOUBLE_TAP) ? "yes" : "no");
    printf("\t\tACTIVITY: %s\r\n", (adxl_dev_get_int_enable(ptr->dev) & ACTIVITY) ? "yes" : "no");
    printf("\t\tINACTIVITY: %s\r\n", (adxl_dev_get_int_enable(ptr->dev) & INACTIVITY) ? "yes" : "no");
    printf("\t\tFREE_FALL: %s\r\n", (adxl_dev_get_int_enable(ptr->dev) & FREE_FALL) ? "yes" : "no");
    printf("\t\tWATERMARK: %s\r\n", (adxl_dev_get_int_enable(ptr->dev) & WATERMARK) ? "yes" : "no");
    printf("\t\tOVERRUN: %s\r\n", (adxl_dev_get_int_enable(ptr->dev) & OVERRUN) ? "yes" : "no");

	if (mask & DATA_READY){
        printf("[INT_ENABLE] : <DATA_READY> interrupt enabled\r\n");
    }

	if (mask & SINGLE_TAP){
        printf("[INT_ENABLE] : <SINGLE_TAP> interrupt enabled\r\n");
    }

	if (mask & DOUBLE_TAP){
        printf("[INT_ENABLE] : <DOUBLE_TAP> interrupt enabled\r\n");
    }

	if (mask & ACTIVITY){
        printf("[INT_ENABLE] : <ACTIVITY> interrupt enabled\r\n");
    }

	if (mask & INACTIVITY){
        printf("[INT_ENABLE] : <INACTIVITY> interrupt enabled\r\n");
    }

	if (mask & FREE_FALL){
        printf("[INT_ENABLE] : <FREE_FALL> interrupt enabled\r\n");
    }

	if (mask & WATERMARK){
        printf("[INT_ENABLE] : <WATERMARK> interrupt enabled\r\n");
    }

	if (mask & OVERRUN){
        printf("[INT_ENABLE] : <OVERRUN> interrupt enabled\r\n");
    }

    adxl_dev_set_int_enable(ptr->dev, (adxl_dev_get_int_enable(ptr->dev) | mask));

	return ADXL_OK;
}


int axi_adxl_disable_interrupt(axi_adxl *ptr, uint8_t mask){
    if (!axi_adxl_has_init(ptr)){
        printf("\t[INT_DISABLE] : device uninitialized\r\n");
        return ERR_UNINIT;
    }

    if (!mask){
        printf("[INT_DISABLE] : no interrupt selected\r\n");
        return ADXL_OK;
    }
    printf("\t[INT_ENABLE] : next interrupts is active for current time:\r\n");
    printf("\t\tDATA_READY: %s\r\n", (adxl_dev_get_int_enable(ptr->dev) & DATA_READY) ? "yes" : "no");
    printf("\t\tSINGLE_TAP: %s\r\n", (adxl_dev_get_int_enable(ptr->dev) & SINGLE_TAP) ? "yes" : "no");
    printf("\t\tDOUBLE_TAP: %s\r\n", (adxl_dev_get_int_enable(ptr->dev) & DOUBLE_TAP) ? "yes" : "no");
    printf("\t\tACTIVITY: %s\r\n", (adxl_dev_get_int_enable(ptr->dev) & ACTIVITY) ? "yes" : "no");
    printf("\t\tINACTIVITY: %s\r\n", (adxl_dev_get_int_enable(ptr->dev) & INACTIVITY) ? "yes" : "no");
    printf("\t\tFREE_FALL: %s\r\n", (adxl_dev_get_int_enable(ptr->dev) & FREE_FALL) ? "yes" : "no");
    printf("\t\tWATERMARK: %s\r\n", (adxl_dev_get_int_enable(ptr->dev) & WATERMARK) ? "yes" : "no");
    printf("\t\tOVERRUN: %s\r\n", (adxl_dev_get_int_enable(ptr->dev) & OVERRUN) ? "yes" : "no");


    if (mask & DATA_READY){
        printf("[INT_ENABLE] : <DATA_READY> interrupt disabled\r\n");
    }

    if (mask & SINGLE_TAP){
        printf("[INT_ENABLE] : <SINGLE_TAP> interrupt disabled\r\n");
    }

    if (mask & DOUBLE_TAP){
        printf("[INT_ENABLE] : <DOUBLE_TAP> interrupt disabled\r\n");
    }

    if (mask & ACTIVITY){
        printf("[INT_ENABLE] : <ACTIVITY> interrupt disabled\r\n");
    }

    if (mask & INACTIVITY){
        printf("[INT_ENABLE] : <INACTIVITY> interrupt disabled\r\n");
    }

    if (mask & FREE_FALL){
        printf("[INT_ENABLE] : <FREE_FALL> interrupt disabled\r\n");
    }

    if (mask & WATERMARK){
        printf("[INT_ENABLE] : <WATERMARK> interrupt disabled\r\n");
    }

    if (mask & OVERRUN){
        printf("[INT_ENABLE] : <OVERRUN> interrupt disabled\r\n");
    }

    adxl_dev_set_int_enable(ptr->dev, (adxl_dev_get_int_enable(ptr->dev) & ~mask));

    return ADXL_OK;

}



int axi_adxl_set_thresh_tap(axi_adxl *ptr, uint8_t thresh_tap){

    if (!axi_adxl_has_init(ptr)){
        printf("\t[SET_THRESH_TAP] : device uninitialized\r\n");
        return ERR_UNINIT;
    }
    
    printf("\t[SET_THRESH_TAP] : changing threshold tap : %3.3f => %3.3f\r\n", ((float)adxl_dev_get_thresh_tap(ptr->dev) * SCALE_THRESH_TAP), ((float)thresh_tap * SCALE_THRESH_TAP));

    adxl_dev_set_thresh_tap(ptr->dev, thresh_tap);

    return ADXL_OK;
}



int axi_adxl_get_thresh_tap(axi_adxl *ptr, uint8_t *thresh_tap){
    if (!axi_adxl_has_init(ptr)){
        printf("\t[GET_THRESH_TAP] : device uninitialized\r\n");
        return ERR_UNINIT;
    }

    printf("\t[GET_THRESH_TAP] : current value of threshold tap : %3.3f (0x%02x)\r\n", ((float)adxl_dev_get_thresh_tap(ptr->dev) * SCALE_THRESH_TAP), adxl_dev_get_thresh_tap(ptr->dev));

    *thresh_tap = (uint8_t*)adxl_dev_get_thresh_tap(ptr->dev);

    return ADXL_OK;

}



int axi_adxl_enable_tap_axes(axi_adxl *ptr, uint8_t mask){
    
    if (!axi_adxl_has_init(ptr)){
        printf("\t[ENABLE_TAP_AXES] : device uninitialized\r\n");
        return ERR_UNINIT;
    }

    if (!mask){
        printf("\t[ENABLE_TAP_AXES] : no tap axes selected\r\n");
        return ADXL_OK;
    }

    if (mask & SUPRESS){
        printf("\t[ENABLE_TAP_AXES] : SUPRESS enabled\r\n");        
    }

    if (mask & MASK_X){
    	printf("\t[ENABLE_TAP_AXES] : Axis X enabled\r\n");

    }
    if (mask & MASK_Y){
		printf("\t[ENABLE_TAP_AXES] : Axis Y enabled\r\n");

    }
    if (mask & MASK_Z){
        printf("\t[ENABLE_TAP_AXES] : Axis Z enabled\r\n");        

    }


    adxl_dev_set_tap_axes(ptr->dev, adxl_dev_get_tap_axes(ptr->dev) | mask);

    return ADXL_OK;
}



int axi_adxl_disable_tap_axes(axi_adxl *ptr, uint8_t mask){
    if (!axi_adxl_has_init(ptr)){
        printf("\t[DISABLE_TAP_AXES] : device uninitialized\r\n");
        return ERR_UNINIT;
    }

    if (!mask){
        printf("\t[DISABLE_TAP_AXES] : no tap axes selected\r\n");
        return ADXL_OK;
    }

    if (mask & SUPRESS){
        printf("\t[DISABLE_TAP_AXES] : SUPRESS disabled\r\n");        
    }
    if (mask & MASK_X){
        printf("\t[DISABLE_TAP_AXES] : Axis X disabled\r\n");        
    }
    if (mask & MASK_Y){
        printf("\t[DISABLE_TAP_AXES] : Axis Y disabled\r\n");        
    }
    if (mask & MASK_Z){
        printf("\t[DISABLE_TAP_AXES] : Axis Z disabled\r\n");        
    }

    adxl_dev_set_tap_axes(ptr->dev, adxl_dev_get_tap_axes(ptr->dev) & ~mask);

    return ADXL_OK;
}



int axi_adxl_set_duration(axi_adxl *ptr, uint8_t value){
    if (!axi_adxl_has_init(ptr)){
        printf("\t[SET_DURATION] : device uninitialized\r\n");
        return ERR_UNINIT;
    }

    printf("\t[SET_DURATION] : changing duration: %3.6f sec => %3.6f sec\r\n", ((float)adxl_dev_get_dur(ptr->dev) * SCALE_DUR), ((float)value * SCALE_DUR));

    adxl_dev_set_dur(ptr->dev, value);

	return ADXL_OK;
}



int axi_adxl_get_duration(axi_adxl *ptr, uint8_t *value){

	if (!axi_adxl_has_init(ptr)){
		printf("\t[GET_DURATION] : device uninitialized\r\n");
		return ERR_UNINIT;
	}

    printf("\t[GET_DURATION] : current value of duration : %3.6f sec(0x%02x)\r\n", ((float)adxl_dev_get_dur(ptr->dev) * SCALE_DUR), adxl_dev_get_dur(ptr->dev));

	*value = (uint8_t*)adxl_dev_get_dur(ptr->dev);

	return ADXL_OK;
}


int axi_adxl_set_latent(axi_adxl *ptr, uint8_t value){

	if (!axi_adxl_has_init(ptr)){
		printf("\t[SET_LATENCY] : device uninitialized\r\n");
		return ERR_UNINIT;
	}

	printf("\t[SET_LATENT] : changing value from %3.5f sec to %3.5f sec\r\n", ((float)adxl_dev_get_latent(ptr->dev)*SCALE_LATENT), (float)value * SCALE_LATENT);

	adxl_dev_set_latent(ptr->dev, value);

	return ADXL_OK;
}



int axi_adxl_get_latent(axi_adxl *ptr, uint8_t *value){
	if (!axi_adxl_has_init(ptr)){
		printf("\t[GET_LATENCY] : device uninitialized\r\n");
		return ERR_UNINIT;
	}

	printf("\t[GET_LATENT] : current value of latent is : %3.5f\r\n", (float)adxl_dev_get_latent(ptr->dev) * SCALE_LATENT);

	*value = (uint8_t*)adxl_dev_get_latent(ptr->dev);

	return ADXL_OK;
}



int axi_adxl_set_window(axi_adxl *ptr, uint8_t value){

	if (!axi_adxl_has_init(ptr)){
		printf("\t[SET_WINDOW] : device uninitialized\r\n");
		return ERR_UNINIT;
	}

	printf("\t[SET_WINDOW] : changing current value from %3.5f sec to %3.5f sec\r\n", (float)adxl_dev_get_window(ptr->dev) * SCALE_WINDOW, (float)value * SCALE_WINDOW );

	adxl_dev_set_window(ptr->dev, value);

	return ADXL_OK;
}



int axi_adxl_get_window(axi_adxl *ptr, uint8_t *value){
	if (!axi_adxl_has_init(ptr)){
		printf("\t[GET_WINDOW] : device uninitialized\r\n");
		return ERR_UNINIT;
	}

	printf("\t[GET_WINDOW] : current value of window is : %3.5f\r\n", (float)adxl_dev_get_window(ptr->dev) * SCALE_WINDOW);

	*value = (uint8_t*)adxl_dev_get_window(ptr->dev);


	return ADXL_OK;
}



int axi_adxl_set_thresh_act(axi_adxl *ptr, uint8_t value){
    if (!axi_adxl_has_init(ptr)){
        printf("\t[SET_THRESH_ACT] : device uninitialized\r\n");
        return ERR_UNINIT;
    }

	printf("\t[SET_THRESH_ACT] : changing current value from %2.4f g to %2.4f g \r\n", ((float)adxl_dev_get_thresh_act(ptr->dev) * SCALE_THRESH_ACT), ((float)value * SCALE_THRESH_ACT));

	adxl_dev_set_thresh_act(ptr->dev, value);

    return ADXL_OK;
}



int axi_adxl_get_thresh_act(axi_adxl *ptr, uint8_t *value){
    if (!axi_adxl_has_init(ptr)){
        printf("\t[GET_THRESH_ACT] : device uninitialized\r\n");
        return ERR_UNINIT;
    }

	printf("\t[GET_THRESH_ACT] : current value of activity threshold is : %2.4g\r\n", adxl_dev_get_thresh_act(ptr->dev)*SCALE_THRESH_ACT);

    *value = adxl_dev_get_thresh_act(ptr->dev);

    return ADXL_OK;
}



int axi_adxl_set_act_ctl(axi_adxl *ptr, enum mask mask_value, int action){
    if (!axi_adxl_has_init(ptr)){
        printf("\t[SET_ACT_CTL] : device uninitialized\r\n");
        return ERR_UNINIT;
    }

    // xoring value
    if (action){ // action enable
    	adxl_dev_set_act_inact_ctl(ptr->dev, adxl_dev_get_act_inact_ctl(ptr->dev) | (mask_value << 4));
    } else {
    	adxl_dev_set_act_inact_ctl(ptr->dev, adxl_dev_get_act_inact_ctl(ptr->dev) & ~(mask_value << 4));
    }

    return ADXL_OK;
}



int axi_adxl_set_inact_ctl(axi_adxl *ptr, enum mask mask_value, int action){
    if (!axi_adxl_has_init(ptr)){
        printf("\t[SET_INACT_CTL] : device uninitialized\r\n");
        return ERR_UNINIT;
    }

    if (action){ // action enable
    	adxl_dev_set_act_inact_ctl(ptr->dev, adxl_dev_get_act_inact_ctl(ptr->dev) | (mask_value));
    } else {
    	adxl_dev_set_act_inact_ctl(ptr->dev, adxl_dev_get_act_inact_ctl(ptr->dev) & ~(mask_value));
    }


    return ADXL_OK;
}



int axi_adxl_set_act_couple(axi_adxl *ptr, int act_type){

    if (!axi_adxl_has_init(ptr)){
        printf("\t[SET_ACT_CPL] : device uninitialized\r\n");
        return ERR_UNINIT;
    }

    if (act_type == AC_COUPLE){
		adxl_dev_set_act_inact_ctl(ptr->dev, adxl_dev_get_act_inact_ctl(ptr->dev) | ACT_COUPLE_MASK);
    }else{
    	adxl_dev_set_act_inact_ctl(ptr->dev, adxl_dev_get_act_inact_ctl(ptr->dev) & ~ACT_COUPLE_MASK);
    }

	return ADXL_OK;
}


int axi_adxl_set_inact_couple(axi_adxl *ptr, int act_type){

    if (!axi_adxl_has_init(ptr)){
        printf("\t[SET_INACT_CPL] : device uninitialized\r\n");
        return ERR_UNINIT;
    }

    if (act_type == AC_COUPLE){
		adxl_dev_set_act_inact_ctl(ptr->dev, adxl_dev_get_act_inact_ctl(ptr->dev) | INACT_COUPLE_MASK);
    }else{
    	adxl_dev_set_act_inact_ctl(ptr->dev, adxl_dev_get_act_inact_ctl(ptr->dev) & ~INACT_COUPLE_MASK);
    }

	return ADXL_OK;
}


int axi_adxl_set_thresh_inact(axi_adxl *ptr, uint8_t value){
    if (!axi_adxl_has_init(ptr)){
        printf("\t[SET_THRESH_INACT] : device uninitialized\r\n");
        return ERR_UNINIT;
    }

	printf("\t[SET_THRESH_INACT] : changing current value from %2.4f g to %2.4f g \r\n", ((float)adxl_dev_get_thresh_inact(ptr->dev) * SCALE_THRESH_INACT), ((float)value * SCALE_THRESH_INACT));

	adxl_dev_set_thresh_inact(ptr->dev, value);

    return ADXL_OK;
}



int axi_adxl_get_thresh_inact(axi_adxl *ptr, uint8_t *value){
    if (!axi_adxl_has_init(ptr)){
        printf("\t[GET_THRESH_INACT] : device uninitialized\r\n");
        return ERR_UNINIT;
    }

	printf("\t[GET_THRESH_INACT] : current value of inactivity threshold is %2.4f g \r\n", ((float)adxl_dev_get_thresh_inact(ptr->dev) * SCALE_THRESH_INACT));

    *value = adxl_dev_get_thresh_inact(ptr->dev);

    return ADXL_OK;
}



int axi_adxl_set_time_inact(axi_adxl *ptr, uint8_t value){
    if (!axi_adxl_has_init(ptr)){
        printf("\t[SET_TIME_INACT] : device uninitialized\r\n");
        return ERR_UNINIT;
    }

	printf("\t[SET_TIME_INACT] : changing current value from %3f sec to %3f sec \r\n", ((float)adxl_dev_get_time_inact(ptr->dev) * SCALE_TIME_INACT), ((float)value * SCALE_TIME_INACT));

	adxl_dev_set_time_inact(ptr->dev, value);

    return ADXL_OK;
}



int axi_adxl_get_time_inact(axi_adxl *ptr, uint8_t *value){
    if (!axi_adxl_has_init(ptr)){
        printf("\t[GET_TIME_INACT] : device uninitialized\r\n");
        return ERR_UNINIT;
    }

	printf("\t[GET_TIME_INACT] : current value of inactivity time is %3f s \r\n", ((float)adxl_dev_get_time_inact(ptr->dev) * SCALE_TIME_INACT));

    *value = adxl_dev_get_time_inact(ptr->dev);


    return ADXL_OK;
}



















int axi_adxl_set_thresh_ff(axi_adxl *ptr, uint8_t value){
    if (!axi_adxl_has_init(ptr)){
        printf("\t[SET_THRESH_FF] : device uninitialized\r\n");
        return ERR_UNINIT;
    }

	printf("\t[SET_THRESH_FF] : changing current value from %2.4f g to %2.4f g \r\n", ((float)adxl_dev_get_thresh_ff(ptr->dev) * SCALE_THRESH_FF), ((float)value * SCALE_THRESH_FF));

	adxl_dev_set_thresh_ff(ptr->dev, value);

    return ADXL_OK;
}



int axi_adxl_get_thresh_ff(axi_adxl *ptr, uint8_t *value){
    if (!axi_adxl_has_init(ptr)){
        printf("\t[GET_THRESH_FF] : device uninitialized\r\n");
        return ERR_UNINIT;
    }

	printf("\t[GET_THRESH_FF] : current value of freefall threshold is %2.4f g \r\n", ((float)adxl_dev_get_thresh_ff(ptr->dev) * SCALE_THRESH_FF));

    *value = adxl_dev_get_thresh_ff(ptr->dev);

    return ADXL_OK;
}



int axi_adxl_set_time_ff(axi_adxl *ptr, uint8_t value){
    if (!axi_adxl_has_init(ptr)){
        printf("\t[SET_TIME_FF] : device uninitialized\r\n");
        return ERR_UNINIT;
    }

	printf("\t[SET_TIME_FF] : changing current value from %1.3f sec to %1.3f sec \r\n", ((float)adxl_dev_get_time_ff(ptr->dev) * SCALE_TIME_FF), ((float)value * SCALE_TIME_FF));

	adxl_dev_set_time_ff(ptr->dev, value);

    return ADXL_OK;
}



int axi_adxl_get_time_ff(axi_adxl *ptr, uint8_t *value){
    if (!axi_adxl_has_init(ptr)){
        printf("\t[GET_TIME_FF] : device uninitialized\r\n");
        return ERR_UNINIT;
    }

	printf("\t[GET_TIME_FF] : current value of freefall time is %1.3f s \r\n", ((float)adxl_dev_get_time_ff(ptr->dev) * SCALE_TIME_FF));

    *value = adxl_dev_get_time_ff(ptr->dev);


    return ADXL_OK;
}



int axi_adxl_set_fifo_samples(axi_adxl *ptr, uint8_t value){
    if (!axi_adxl_has_init(ptr)){
        printf("\t[SET_FIFO_SAMPLES] : device uninitialized\r\n");
        return ERR_UNINIT;
    }

	printf("\t[SET_FIFO_SAMPLES] : changing current value from %d to %d\r\n", adxl_dev_get_fifo_ctl(ptr->dev) & FIFO_CTL_SAMPLES, value);

	adxl_dev_set_fifo_ctl(ptr->dev, (adxl_dev_get_fifo_ctl(ptr->dev) & ~FIFO_CTL_SAMPLES) | value);

    return ADXL_OK;
}


int axi_adxl_get_fifo_samples(axi_adxl *ptr, uint8_t *value){
    if (!axi_adxl_has_init(ptr)){
        printf("\t[GET_FIFO_SAMPLES] : device uninitialized\r\n");
        return ERR_UNINIT;
    }

	printf("\t[GET_FIFO_SAMPLES] : current value of fifo samples limit %d\r\n", adxl_dev_get_fifo_ctl(ptr->dev));

    *value = adxl_dev_get_fifo_ctl(ptr->dev) & FIFO_CTL_SAMPLES;

    return ADXL_OK;

}



int axi_adxl_set_fifo_mode(axi_adxl *ptr, enum fifo_mode value){
    if (!axi_adxl_has_init(ptr)){
        printf("\t[SET_FIFO_MODE] : device uninitialized\r\n");
        return ERR_UNINIT;
    }

    switch(value){

    case BYPASS :
    	printf("\t[SET_FIFO_MODE] : setup BYPASS MODE\r\n");
    	adxl_dev_set_fifo_ctl(ptr->dev, (adxl_dev_get_fifo_ctl(ptr->dev) & ~FIFO_CTL_FIFO_MODE) | BYPASS);
	break;

    case FIFO :
    	printf("\t[SET_FIFO_MODE] : setup FIFO MODE\r\n");
    	adxl_dev_set_fifo_ctl(ptr->dev, (adxl_dev_get_fifo_ctl(ptr->dev) & ~FIFO_CTL_FIFO_MODE) | FIFO);
	break;

    case STREAM :
    	printf("\t[SET_FIFO_MODE] : setup STREAM MODE\r\n");
    	adxl_dev_set_fifo_ctl(ptr->dev, (adxl_dev_get_fifo_ctl(ptr->dev) & ~FIFO_CTL_FIFO_MODE) | STREAM);

	break;

    case TRIGGER :
    	printf("\t[SET_FIFO_MODE] : setup TRIGGER MODE\r\n");
    	adxl_dev_set_fifo_ctl(ptr->dev, (adxl_dev_get_fifo_ctl(ptr->dev) & ~FIFO_CTL_FIFO_MODE) | TRIGGER);
	break;

    default :
    	printf("\t[SET_FIFO_MODE] : setup <undefined> mode failed, return...\r\n");
	break;




    }

	adxl_dev_set_fifo_ctl(ptr->dev, (adxl_dev_get_fifo_ctl(ptr->dev) & ~FIFO_CTL_FIFO_MODE) | value);

    return ADXL_OK;

}



int axi_adxl_set_offset_x(axi_adxl *ptr, uint8_t value){
	if (!axi_adxl_has_init(ptr)){
		printf("\t[SET_OFS_X] : device uninitialized\r\n");
		return ERR_UNINIT;
	}

	adxl_dev_set_ofsx(ptr->dev, value);
    return ADXL_OK;
}


int axi_adxl_set_offset_y(axi_adxl *ptr, uint8_t value){
	if (!axi_adxl_has_init(ptr)){
		printf("\t[SET_OFS_Y] : device uninitialized\r\n");
		return ERR_UNINIT;
	}

	adxl_dev_set_ofsy(ptr->dev, value);
    return ADXL_OK;
}


int axi_adxl_set_offset_z(axi_adxl *ptr, uint8_t value){
	if (!axi_adxl_has_init(ptr)){
		printf("\t[SET_OFS_Z] : device uninitialized\r\n");
		return ERR_UNINIT;
	}

	adxl_dev_set_ofsz(ptr->dev, value);
    return ADXL_OK;
}


int axi_adxl_convert_raw_to_g(axi_adxl *ptr, adxl_axis raw, g_coord* g_ptr){

	int16_t data_x = (int16_t)raw.x;
	int16_t data_y = (int16_t)raw.y;
	int16_t data_z = (int16_t)raw.z;

	if (adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_FULL_RES){
		g_ptr->x = (float)data_x/SENSITIVITY_FULL_RES;
		g_ptr->y = (float)data_y/SENSITIVITY_FULL_RES;
		g_ptr->z = (float)data_z/SENSITIVITY_FULL_RES;
	}else{
		switch (adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_RANGE_MASK){

			case DATA_FORMAT_RANGE_2G:
				g_ptr->x = (float)data_x/SENSITIVITY_2G;
				g_ptr->y = (float)data_y/SENSITIVITY_2G;
				g_ptr->z = (float)data_z/SENSITIVITY_2G;
			break;

			case DATA_FORMAT_RANGE_4G:
				g_ptr->x = (float)data_x/SENSITIVITY_4G;
				g_ptr->y = (float)data_y/SENSITIVITY_4G;
				g_ptr->z = (float)data_z/SENSITIVITY_4G;
			break;

			case DATA_FORMAT_RANGE_8G:
				g_ptr->x = (float)data_x/SENSITIVITY_8G;
				g_ptr->y = (float)data_y/SENSITIVITY_8G;
				g_ptr->z = (float)data_z/SENSITIVITY_8G;
			break;

			case DATA_FORMAT_RANGE_16G:
				g_ptr->x = (float)data_x/SENSITIVITY_16G;
				g_ptr->y = (float)data_y/SENSITIVITY_16G;
				g_ptr->z = (float)data_z/SENSITIVITY_16G;
			break;

			default :
				printf("[CONVERT_RAW_TO_G] : Unknown data format : 0x%02x\r\n", adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_RANGE_MASK);
			break;
		}
	}

	return ADXL_OK;
}
//
//
//int axi_adxl_get_range(axi_adxl *ptr, uint8_t *range){
//  int status = axi_adxl_has_runned(ptr);
//
//  if (status != ADXL_OK){
//      printf("[GET_RANGE] return code %d\t\n", status);
//  }
//
//  *range = (adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_RANGE_MASK);
//
//  return ADXL_OK;
//}
//
//
//int axi_adxl_set_offset(axi_adxl *ptr, offset_param offt_prm, enum mask mask_value){
//
//  int status = axi_adxl_has_runned(ptr);
//  if (status != ADXL_OK){
//      printf("[SET_OFFT] : return with code %d", status);
//  }
//
//
//  if (mask_value & MASK_X){
//      adxl_dev_set_ofsx(ptr->dev, offt_prm.x);
//  }
//
//  if (mask_value & MASK_Y){
//      adxl_dev_set_ofsy(ptr->dev, offt_prm.y);
//  }
//
//  if (mask_value & MASK_Z){
//      adxl_dev_set_ofsz(ptr->dev, offt_prm.z);
//  }
//
//  return ADXL_OK;
//}
//
//
//int axi_adxl_get_offset(axi_adxl *ptr, offset_param *offt_prm){
//  int status = axi_adxl_has_runned(ptr);
//  if (status != ADXL_OK) {
//      printf("[GET_OFFT] : return with code %d", status);
//      return status;
//  }
//
//  offt_prm->x = adxl_dev_get_ofsx(ptr->dev);
//  offt_prm->y = adxl_dev_get_ofsy(ptr->dev);
//  offt_prm->z = adxl_dev_get_ofsz(ptr->dev);
//
//  return ADXL_OK;
//}
//
//int axi_adxl_get_offset_x(axi_adxl *ptr, uint8_t *offset){
//  int status = axi_adxl_has_runned(ptr);
//  if (status != ADXL_OK){
//      return status;
//  }
//
//  *offset = adxl_dev_get_ofsx(ptr->dev);
//
//  return status;
//}
//
//
//
//int axi_adxl_get_offset_y(axi_adxl *ptr, uint8_t *offset){
//  int status = axi_adxl_has_runned(ptr);
//  if (status != ADXL_OK){
//      return status;
//  }
//
//  *offset = adxl_dev_get_ofsy(ptr->dev);
//
//  return status;
//}
//
//
//
//int axi_adxl_get_offset_z(axi_adxl *ptr, uint8_t *offset){
//  int status = axi_adxl_has_runned(ptr);
//  if (status != ADXL_OK){
//      return status;
//  }
//
//  *offset = adxl_dev_get_ofsz(ptr->dev);
//
//  return status;
//}
//
//
//
//int axi_adxl_get_gravity(axi_adxl *ptr, g_coord *g_ptr){
//
//  int status = axi_adxl_has_runned(ptr);
//
//  if (status != ADXL_OK){
//      printf("[GET_GRVTY] : return with code %d", status);
//  }
//
//  if (adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_FULL_RES){
//      g_ptr->x = (float)axi_adxl_get_datax(ptr)/SENSITIVITY_FULL_RES;
//      g_ptr->y = (float)axi_adxl_get_datay(ptr)/SENSITIVITY_FULL_RES;
//      g_ptr->z = (float)axi_adxl_get_dataz(ptr)/SENSITIVITY_FULL_RES;
//  }else{
//      switch (adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_RANGE_MASK){
//
//          case DATA_FORMAT_RANGE_2G:
//              g_ptr->x = (float)axi_adxl_get_datax(ptr)/SENSITIVITY_2G;
//              g_ptr->y = (float)axi_adxl_get_datay(ptr)/SENSITIVITY_2G;
//              g_ptr->z = (float)axi_adxl_get_dataz(ptr)/SENSITIVITY_2G;
//          break;
//
//          case DATA_FORMAT_RANGE_4G:
//              g_ptr->x = (float)axi_adxl_get_datax(ptr)/SENSITIVITY_4G;
//              g_ptr->y = (float)axi_adxl_get_datay(ptr)/SENSITIVITY_4G;
//              g_ptr->z = (float)axi_adxl_get_dataz(ptr)/SENSITIVITY_4G;
//          break;
//
//          case DATA_FORMAT_RANGE_8G:
//              g_ptr->x = (float)axi_adxl_get_datax(ptr)/SENSITIVITY_8G;
//              g_ptr->y = (float)axi_adxl_get_datay(ptr)/SENSITIVITY_8G;
//              g_ptr->z = (float)axi_adxl_get_dataz(ptr)/SENSITIVITY_8G;
//          break;
//
//          case DATA_FORMAT_RANGE_16G:
//              g_ptr->x = (float)axi_adxl_get_datax(ptr)/SENSITIVITY_16G;
//              g_ptr->y = (float)axi_adxl_get_datay(ptr)/SENSITIVITY_16G;
//              g_ptr->z = (float)axi_adxl_get_dataz(ptr)/SENSITIVITY_16G;
//          break;
//
//          default :
//              printf("[GET_GRVTY] : Unknown data format : 0x%02x\r\n", adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_RANGE_MASK);
//          break;
//      }
//  }
//
//  return ADXL_OK;
//
//}
//
//
//
//int axi_adxl_calibration(axi_adxl *ptr){
////
//  int status = axi_adxl_has_runned(ptr);
//  if (status != ADXL_OK){
//      printf("[CALIBRTN] : return with code %d", status);
//  }
//
//  printf("[CALIBRTN] : Start process...\r\n");
//
//  adxl_cfg_set_calibration(ptr->cfg);
//
//  while(adxl_cfg_get_calibration(ptr->cfg)){
//
//  }
//
//  printf("[CALIBRTN] : complete\r\n");
//
//  return ADXL_OK;
//}
//
//
//int axi_adxl_set_calib_count_limit(axi_adxl *ptr, uint32_t calib_count){
//  int status = axi_adxl_has_runned(ptr);
//  if (status != ADXL_OK){
//      printf("[CALIBRTN] : return with code %d", status);
//  }
//
//  adxl_cfg_set_calibration_count_limit(ptr->cfg, calib_count);
//
//  return status;
//}
//
//
//int axi_adxl_get_roll(axi_adxl *ptr, float *roll){
//  int status = axi_adxl_has_runned(ptr);
//  if (status != ADXL_OK){
//      return status;
//  }
//
//  g_coord g;
//
//  status = axi_adxl_get_gravity(ptr, &g);
//  *roll = atan(g.y/ sqrt(pow(g.x, 2) + pow(g.z, 2))) * 180 / PI;
//
//  return status;
//}
//
//
//
//int axi_adxl_get_pitch(axi_adxl *ptr, float *pitch){
//  int status = axi_adxl_has_runned(ptr);
//  if (status != ADXL_OK){
//      return status;
//  }
//
//  g_coord g;
//
//  status = axi_adxl_get_gravity(ptr, &g);
//  *pitch = atan(-1 * g.x / sqrt(pow(g.y, 2) + pow(g.z, 2))) * 180 / PI;
//  return ADXL_OK;
//}
//
//
//
//int axi_adxl_set_thresh_tap(axi_adxl *ptr, uint8_t thresh_tap){
//  int status = axi_adxl_has_runned(ptr);
//  if (status != ADXL_OK){
//      return status;
//  }
//
//  adxl_dev_set_thresh_tap(ptr->dev, thresh_tap);
//
//  return status;
//}
//
//
//
//int axi_adxl_get_thresh_tap(axi_adxl *ptr, uint8_t *thresh_tap){
//  int status = axi_adxl_has_runned(ptr);
//  if (status != ADXL_OK){
//      return status;
//  }
//
//  *thresh_tap = adxl_dev_get_thresh_tap(ptr->dev);
//
//  return status;
//}
//
//
//int axi_adxl_set_dur(axi_adxl *ptr, uint8_t dur){
//  int status = axi_adxl_has_runned(ptr);
//  if (status != ADXL_OK){
//      return status;
//  }
//
//  adxl_dev_set_dur(ptr->dev, dur);
//
//  return status;
//}
//
//
//
//int axi_adxl_get_dur(axi_adxl *ptr, uint8_t *dur){
//  int status = axi_adxl_has_runned(ptr);
//  if (status != ADXL_OK){
//      return status;
//  }
//
//  return status;
//}
//
//
//
//int axi_adxl_set_latency(axi_adxl *ptr, uint8_t latency){
//  int status = axi_adxl_has_runned(ptr);
//  if (status != ADXL_OK){
//      return status;
//  }
//
//  adxl_dev_set_latent(ptr->dev, latency);
//
//  return status;
//}
//
//
//int axi_adxl_get_latency(axi_adxl *ptr, uint8_t *latency){
//  int status = axi_adxl_has_runned(ptr);
//  if (status != ADXL_OK){
//      return status;
//  }
//
//  *latency = adxl_dev_get_latent(ptr->dev);
//
//  return status;
//
//}
//
//
//int axi_adxl_set_window(axi_adxl *ptr, uint8_t window){
//  int status = axi_adxl_has_runned(ptr);
//  if (status != ADXL_OK){
//      return status;
//  }
//
//  adxl_dev_set_window(ptr->dev, window);
//
//  return status;
//
//}
//
//int axi_adxl_get_window(axi_adxl *ptr, uint8_t *window){
//  int status = axi_adxl_has_runned(ptr);
//  if (status != ADXL_OK){
//      return status;
//  }
//
//  *window = adxl_dev_get_window(ptr->dev);
//
//  return status;
//
//}
//
//
//int axi_adxl_set_int_map(axi_adxl *ptr, uint8_t mask, uint8_t map){
//  int status = axi_adxl_has_runned(ptr);
//  if (status != ADXL_OK){
//      return status;
//  }
//
//  adxl_dev_set_int_map(ptr->dev, (adxl_dev_get_int_map(ptr->dev) & map));
//
//  return status;
//}
//
//
//
//
//
//int axi_adxl_int_enable(axi_adxl *ptr, uint8_t mask){
//  int status = axi_adxl_has_runned(ptr);
//  if (status != ADXL_OK){
//      return status;
//  }
//
//  adxl_dev_set_int_enable(ptr->dev, (adxl_dev_get_int_enable(ptr->dev) | mask));
//
//  return status;
//}
//
//
//
//int axi_adxl_int_disable(axi_adxl *ptr, uint8_t mask){
//
//  int status = axi_adxl_has_runned(ptr);
//  if (status != ADXL_OK){
//      return status;
//  }
//
//  adxl_dev_set_int_enable(ptr->dev, (adxl_dev_get_int_enable(ptr->dev) & ~mask ));
//
//  return status;
//}
//
//
//
//int axi_adxl_set_tap_axes_active(axi_adxl *ptr, enum mask mask_value){
//  int status = axi_adxl_has_runned(ptr);
//  if (status != ADXL_OK){
//      return status;
//  }
//
//  adxl_dev_set_tap_axes(ptr->dev, (adxl_dev_get_tap_axes(ptr->dev) | mask_value));
//
//  return status;
//}
//
//
//
//int axi_adxl_int_invert(axi_adxl *ptr){
//  int status = axi_adxl_has_runned(ptr);
//  if (status != ADXL_OK){
//      return status;
//  }
//
//  adxl_dev_set_data_format(ptr->dev, adxl_dev_get_data_format(ptr->dev) ^ DATA_FORMAT_INT_INVERT_MASK);
//
//  return status;
//}
//
//
//
//int axi_adxl_set_act_ctl(axi_adxl *ptr, int act_mode, enum mask mask_value){
//
//  int status = axi_adxl_has_runned(ptr);
//  if (status != ADXL_OK){
//      return status;
//  }
//
//  adxl_dev_set_act_inact_ctl(ptr->dev, (((act_mode<<4) | (mask_value<<4)) & ACT_MASK) | ((adxl_dev_get_act_inact_ctl(ptr->dev)) & ~ACT_MASK));
//
//  return status;
//}
//
//
//int axi_adxl_set_inact_ctl(axi_adxl *ptr, int act_mode, enum mask mask_value){
//  int status = axi_adxl_has_runned(ptr);
//  if (status != ADXL_OK){
//      return status;
//  }
//
//  adxl_dev_set_act_inact_ctl(ptr->dev, (((act_mode) | (mask_value)) & INACT_MASK) | ((adxl_dev_get_act_inact_ctl(ptr->dev)) & ~INACT_MASK));
//
//  return status;
//}
//
//
//int axi_adxl_set_thresh_act(axi_adxl *ptr, uint8_t value){
//  int status = axi_adxl_has_runned(ptr);
//  if (status != ADXL_OK){
//      return status;
//  }
//
//  adxl_dev_set_thresh_act(ptr->dev, value);
//
//  return status;
//}
//
//
//int axi_adxl_get_thresh_act(axi_adxl *ptr, uint8_t *value){
//
//  int status = axi_adxl_has_runned(ptr);
//  if (status != ADXL_OK){
//      return status;
//  }
//
//  *value = adxl_dev_get_thresh_act(ptr->dev);
//
//  return status;
//}
//
//int axi_adxl_set_thresh_inact(axi_adxl *ptr, uint8_t value){
//  int status = axi_adxl_has_runned(ptr);
//  if (status != ADXL_OK){
//      return status;
//  }
//
//  adxl_dev_set_thresh_inact(ptr->dev, value);
//
//  return status;
//}
//
//
//int axi_adxl_get_thresh_inact(axi_adxl *ptr, uint8_t *value){
//  int status = axi_adxl_has_runned(ptr);
//  if (status != ADXL_OK){
//      return status;
//  }
//
//  *value = adxl_dev_get_thresh_inact(ptr->dev);
//
//  return status;
//
//}
//
//int axi_adxl_set_time_inact(axi_adxl *ptr, uint8_t value){
//  int status = axi_adxl_has_runned(ptr);
//  if (status != ADXL_OK){
//      return status;
//  }
//
//  adxl_dev_set_time_inact(ptr->dev, value);
//
//  return status;
//
//}
//
//int axi_adxl_get_time_inact(axi_adxl *ptr, uint8_t *value){
//  int status = axi_adxl_has_runned(ptr);
//  if (status != ADXL_OK){
//      return status;
//  }
//
//  *value = adxl_dev_get_time_inact(ptr->dev);
//
//  return status;
//
//}
//
//
//int axi_adxl_set_thresh_ff (axi_adxl *ptr, uint8_t value){
//  int status = axi_adxl_has_runned(ptr);
//  if (status != ADXL_OK){
//      return status;
//  }
//
//  adxl_dev_set_thresh_ff(ptr->dev, value);
//
//  return status;
//
//
//}
//
//int axi_adxl_get_thresh_ff (axi_adxl *ptr, uint8_t *value){
//  int status = axi_adxl_has_runned(ptr);
//  if (status != ADXL_OK){
//      return status;
//  }
//
//  *value = adxl_dev_get_thresh_ff(ptr->dev);
//
//  return status;
//
//
//}
//
//int axi_adxl_set_time_ff (axi_adxl *ptr, uint8_t value){
//  int status = axi_adxl_has_runned(ptr);
//  if (status != ADXL_OK){
//      return status;
//  }
//
//  adxl_dev_set_time_ff(ptr->dev, value);
//
//  return status;
//
//
//}
//
//int axi_adxl_get_time_ff (axi_adxl *ptr, uint8_t *value){
//  int status = axi_adxl_has_runned(ptr);
//  if (status != ADXL_OK){
//      return status;
//  }
//
//  *value = adxl_dev_get_time_ff(ptr->dev);
//
//  return status;
//
//
//}
//
//
//int axi_adxl_set_fifo_mode(axi_adxl *ptr, enum fifo_mode mode){
//
//  int status = axi_adxl_has_runned(ptr);
//  if (status != ADXL_OK){
//      return status;
//  }
//
//  adxl_dev_set_fifo_ctl(ptr->dev, ((adxl_dev_get_fifo_ctl(ptr->dev) & ~FIFO_CTL_FIFO_MODE) | mode));
//
//  return status;
//}
//
//
//int axi_adxl_set_fifo_samples(axi_adxl *ptr, uint8_t samples){
//  int status = axi_adxl_has_runned(ptr);
//  if (status != ADXL_OK){
//      return status;
//  }
//
//  adxl_dev_set_fifo_ctl(ptr->dev, ((adxl_dev_get_fifo_ctl(ptr->dev) & ~FIFO_CTL_SAMPLES) | samples));
//
//  return status;
//
//}
//
//
//int axi_adxl_get_fifo_entries(axi_adxl *ptr, uint8_t *entries){
//
//  int status = axi_adxl_has_runned(ptr);
//  if (status != ADXL_OK){
//      return status;
//  }
//
//  *entries = adxl_dev_get_fifo_status(ptr->dev) & FIFO_STATUS_ENTRIES_MASK;
//
//  return status;
//}
//
//int axi_adxl_get_samples(axi_adxl *ptr, uint8_t *samples){
//
//  int status = axi_adxl_has_runned(ptr);
//  if (status != ADXL_OK){
//      return status;
//  }
//
//  *samples = adxl_cfg_get_rom_address(ptr->cfg);
//
//  return status;
//}
//
//int axi_adxl_get_optimal_request_interval(axi_adxl *ptr){
//  return (adxl_cfg_get_opt_req_interval(ptr->cfg));
//}
//
//int axi_adxl_get_calibration_time(axi_adxl *ptr){
//  float time = ((float)adxl_cfg_get_calibration_time(ptr->cfg)/(float)adxl_cfg_get_clk_period(ptr->cfg));
//  printf("[CALIBRATION TIME] : %3.6f\r\n", time);
//}
//
//
//void axi_adxl_debug(axi_adxl *ptr){
//  printf("[CONFIGURATION SPACE]\r\n");
//  printf("\t[VERSION] : %d.%d\r\n", adxl_cfg_get_version_major(ptr->cfg), adxl_cfg_get_version_minor(ptr->cfg));
//  printf("\t[I2C ADDRESS] : 0x%02x\r\n", adxl_cfg_get_i2c_addr(ptr->cfg));
//  printf("\t[LNK] : %d\r\n", adxl_cfg_has_link(ptr->cfg));
//  printf("\t[ON WORK] : %d\r\n", adxl_cfg_has_on_work(ptr->cfg));
//  printf("\t[IRQ] : %s\r\n", adxl_cfg_has_allow_irq(ptr->cfg) ? "allow" : "unallow");
//  printf("\t[ENABLED] : %d\r\n", adxl_cfg_has_enable(ptr->cfg));
//  printf("\t[REQUEST INTERVAL] : %d periods\r\n", adxl_cfg_get_request_interval(ptr->cfg));
//  printf("\t[DATA WIDTH] : %d bytes\r\n", adxl_cfg_get_data_width(ptr->cfg));
//  printf("\t[READ VALID COUNT] : %d words\r\n", adxl_cfg_get_read_valid_count(ptr->cfg));
//  printf("\t[WRITE VALID COUNT] : %d words\r\n", adxl_cfg_get_write_valid_count(ptr->cfg));
//  printf("[WRITE TRANSACTIONS] : %d\r\n", adxl_cfg_get_write_transactions(ptr->cfg));
//  printf("[READ_TRANSACTIONS] : %d\r\n", adxl_cfg_get_read_transactions(ptr->cfg));
//  printf("[CLK_PERIOD] : %3.3f MHz\r\n", (float)(adxl_cfg_get_clk_period(ptr->cfg)/1000000));
//  printf("\r\n");
//
//  printf("[DEVICE SPACE]\r\n");
//  printf("\t[DEVICE ID] : 0x%02x\r\n", adxl_dev_get_device_id(ptr->dev));
//
//  printf("\t[THRESH_TAP] : 0x%02x\r\n", adxl_dev_get_thresh_tap(ptr->dev));
//  printf("\t\t[THRESH_TAP_G] : %3.3f g\r\n", (float)(adxl_dev_get_thresh_tap(ptr->dev) * SCALE_THRESH_TAP));
//
//  printf("\t[OFSX] : 0x%02x\r\n", adxl_dev_get_ofsx(ptr->dev));
//  printf("\t\t[OFSX] : %3.3f g\r\n", (float)((int8_t)adxl_dev_get_ofsx(ptr->dev) * SCALE_OFS));
//
//  printf("\t[OFSY] : 0x%02x\r\n", adxl_dev_get_ofsy(ptr->dev));
//  printf("\t\t[OFSY_G] : %3.3f g\r\n", (float)((int8_t)adxl_dev_get_ofsy(ptr->dev) * SCALE_OFS));
//
//  printf("\t[OFSZ] : 0x%02x\r\n", adxl_dev_get_ofsz(ptr->dev));
//  printf("\t\t[OFSZ_G] : %3.3f g\r\n", (float)((int8_t)adxl_dev_get_ofsz(ptr->dev) * SCALE_OFS));
//
//  printf("\t[DUR] : 0x%02x\r\n", adxl_dev_get_dur(ptr->dev));
//  printf("\t\t[DUR] : %3.3f sec\r\n", (float)(adxl_dev_get_dur(ptr->dev) * SCALE_DUR));
//
//  printf("\t[LATENT] : 0x%02x\r\n", adxl_dev_get_latent(ptr->dev));
//  printf("\t\t[LATENT_S] : %3.3f sec\r\n", (float)(adxl_dev_get_latent(ptr->dev) * SCALE_LATENT));
//
//  printf("\t[WINDOW] : 0x%02x\r\n", adxl_dev_get_window(ptr->dev));
//  printf("\t\t[WINDOW_S] : %3.3f sec\r\n", (float)(adxl_dev_get_window(ptr->dev) * SCALE_WINDOW));
//
//  printf("\t[THRESH_ACT] : 0x%02x\r\n", adxl_dev_get_thresh_act(ptr->dev));
//  printf("\t\t[THRESH_ACT_G] : %3.3f g\r\n", (float)(adxl_dev_get_thresh_act(ptr->dev) * SCALE_THRESH_ACT));
//
//  printf("\t[THRESH_INACT] : 0x%02x\r\n", adxl_dev_get_thresh_inact(ptr->dev));
//  printf("\t\t[THRESH_INACT_G] : %3.3f g\r\n", (float)(adxl_dev_get_thresh_inact(ptr->dev) * SCALE_THRESH_INACT));
//
//  printf("\t[TIME_INACT] : 0x%02x\r\n", adxl_dev_get_time_inact(ptr->dev));
//  printf("\t[TIME_INACT_S] : %3.3f s\r\n", (float)(adxl_dev_get_time_inact(ptr->dev) * SCALE_TIME_INACT));
//
//  printf("\t[ACT_INACT_CTL] : 0x%02x\r\n", adxl_dev_get_act_inact_ctl(ptr->dev));
//  printf("\t\t[ACT_AC/DC] : %d \r\n", (adxl_dev_get_act_inact_ctl(ptr->dev) & ACT_INACT_CTL_ACT_ACDC_MASK) ? 1:0);
//  printf("\t\t[ACT_X EN] : %d\r\n", adxl_dev_get_act_inact_ctl(ptr->dev) & ACT_INACT_CTL_ACT_X_EN_MASK ? 1:0);
//  printf("\t\t[ACT_Y EN] : %d\r\n", adxl_dev_get_act_inact_ctl(ptr->dev) & ACT_INACT_CTL_ACT_Y_EN_MASK ? 1:0);
//  printf("\t\t[ACT_Z EN] : %d\r\n", adxl_dev_get_act_inact_ctl(ptr->dev) & ACT_INACT_CTL_ACT_Z_EN_MASK ? 1:0);
//  printf("\t\t[INACT AC/DC] : %d\r\n", adxl_dev_get_act_inact_ctl(ptr->dev) & ACT_INACT_CTL_INACT_ACDC_MASK ? 1:0);
//  printf("\t\t[INACT X EN] : %d\r\n", adxl_dev_get_act_inact_ctl(ptr->dev) & ACT_INACT_CTL_INACT_X_EN_MASK ? 1:0);
//  printf("\t\t[INACT Y EN] : %d\r\n", adxl_dev_get_act_inact_ctl(ptr->dev) & ACT_INACT_CTL_INACT_Y_EN_MASK ? 1:0);
//  printf("\t\t[INACT Z EN] : %d\r\n", adxl_dev_get_act_inact_ctl(ptr->dev) & ACT_INACT_CTL_INACT_Z_EN_MASK ? 1:0);
//
//  printf("\t[THRESH_FF] : 0x%02x : \r\n", adxl_dev_get_thresh_ff(ptr->dev));
//  printf("\t\t[THRESH_FF] : %3.3f: \r\n", (float)(adxl_dev_get_thresh_ff(ptr->dev) * SCALE_THRESH_FF));
//
//  printf("\t[TIME_FF] : 0x%02x\r\n", adxl_dev_get_time_ff(ptr->dev));
//  printf("\t\t[TIME_FF] : %3.3f\r\n", (float)(adxl_dev_get_time_ff(ptr->dev) * SCALE_TIME_FF));
//
//  printf("\t[TAP_AXES] : 0x%02x\r\n", adxl_dev_get_tap_axes(ptr->dev));
//  printf("\t\t[SUPRESS] : %d\r\n", adxl_dev_get_tap_axes(ptr->dev) & TAP_AXES_SUPRESS_MASK ? 1:0);
//  printf("\t\t[TAP_X_EN] : %d\r\n", adxl_dev_get_tap_axes(ptr->dev) & TAP_AXES_TAP_X_EN_MASK ? 1:0);
//  printf("\t\t[TAP_Y_EN] : %d\r\n", adxl_dev_get_tap_axes(ptr->dev) & TAP_AXES_TAP_Y_EN_MASK ? 1:0);
//  printf("\t\t[TAP_Z_EN] : %d\r\n", adxl_dev_get_tap_axes(ptr->dev) & TAP_AXES_TAP_Z_EN_MASK ? 1:0);
//  printf("\t[ACT_TAP_STATUS] : 0x%02x\r\n", adxl_dev_get_act_tap_status(ptr->dev));
//  printf("\t\t[ACT_X_SRC] : %d\r\n", adxl_dev_get_act_tap_status(ptr->dev) & ACT_TAP_STATUS_ACT_X_SRC_MASK ? 1:0);
//  printf("\t\t[ACT_Y_SRC] : %d\r\n", adxl_dev_get_act_tap_status(ptr->dev) & ACT_TAP_STATUS_ACT_Y_SRC_MASK ? 1:0);
//  printf("\t\t[ACT_Z_SRC] : %d\r\n", adxl_dev_get_act_tap_status(ptr->dev) & ACT_TAP_STATUS_ACT_Z_SRC_MASK ? 1:0);
//  printf("\t\t[ASLEEP] : %d\r\n", adxl_dev_get_act_tap_status(ptr->dev) & ACT_TAP_STATUS_ASLEEP_MASK ? 1:0);
//  printf("\t\t[TAP_X_SRC] : %d\r\n", adxl_dev_get_act_tap_status(ptr->dev) & ACT_TAP_STATUS_TAP_X_SRC_MASK ? 1:0);
//  printf("\t\t[TAP_Y_SRC] : %d\r\n", adxl_dev_get_act_tap_status(ptr->dev) & ACT_TAP_STATUS_TAP_Y_SRC_MASK ? 1:0);
//  printf("\t\t[TAP_Z_SRC] : %d\r\n", adxl_dev_get_act_tap_status(ptr->dev) & ACT_TAP_STATUS_TAP_Z_SRC_MASK ? 1:0);
//
//  printf("\t[BW_RATE] : 0x%02x\r\n", adxl_dev_get_bw_rate(ptr->dev));
//  printf("\t\t[LOW_POWER] : %d\r\n", adxl_dev_get_bw_rate(ptr->dev) & BW_RATE_LOW_POWER_MASK ? 1:0);
//  printf("\t\t[RATE] : %d\r\n", adxl_dev_get_bw_rate(ptr->dev) & BW_RATE_RATE_MASK);
//  printf("\t\t\t[RATE_HZ] : ");
//  uint8_t bw_rate = adxl_dev_get_bw_rate(ptr->dev) & (BW_RATE_RATE_MASK | BW_RATE_LOW_POWER_MASK);
//  switch(bw_rate){
//      case BW_RATE_3200 :
//          printf("3200");
//      break;
//
//      case BW_RATE_1600 :
//          printf("1600");
//      break;
//
//      case BW_RATE_800 :
//          printf("800");
//      break;
//
//      case BW_RATE_400:
//          printf("400");
//      break;
//
//      case BW_RATE_200:
//          printf("200");
//      break;
//
//      case BW_RATE_100:
//          printf("100");
//      break;
//
//      case BW_RATE_50:
//          printf("50");
//      break;
//
//      case BW_RATE_25:
//          printf("25");
//      break;
//
//      case BW_RATE_12_5:
//          printf("12.5");
//      break;
//
//      case BW_RATE_6_25:
//          printf("6.25");
//      break;
//
//      case BW_RATE_3_13:
//          printf("3.125");
//      break;
//
//      case BW_RATE_1_56:
//          printf("1.56125");
//      break;
//
//      case BW_RATE_0_78:
//          printf("0.78");
//      break;
//
//      case BW_RATE_0_39:
//          printf("0.39");
//      break;
//
//      case BW_RATE_0_20:
//          printf("0.2");
//      break;
//
//      case BW_RATE_0_10:
//          printf("0.1");
//      break;
//
//      case BW_RATE_400_LP:
//          printf("low_power 400");
//      break;
//
//      case BW_RATE_200_LP:
//          printf("low_power 200");
//      break;
//
//      case BW_RATE_100_LP:
//          printf("low_power 100");
//      break;
//
//      case BW_RATE_50_LP:
//          printf("low_power 50");
//      break;
//
//      case BW_RATE_25_LP:
//          printf("low_power 25");
//      break;
//
//      case BW_RATE_12_5_LP:
//          printf("low_power 12.5");
//      break;
//
//      default :
//          printf("<undefined>");
//
//  }
//  printf(" Hz\r\n");
//  printf("\t[POWER_CTL] : 0x%02x\r\n", adxl_dev_get_power_ctl(ptr->dev));
//  printf("\t\t[LINK] : %d\r\n", adxl_dev_get_power_ctl(ptr->dev) & POWER_CTL_LINK_MASK ? 1:0);
//  printf("\t\t[AUTOSLEEP] : %d\r\n", adxl_dev_get_power_ctl(ptr->dev) & POWER_CTL_AUTO_SLEEP_MASK ? 1:0);
//  printf("\t\t[MEASURE] : %d\r\n", adxl_dev_get_power_ctl(ptr->dev) & POWER_CTL_MEASURE_MASK ? 1:0);
//  printf("\t\t[SLEEP] : %d\r\n", adxl_dev_get_power_ctl(ptr->dev) & POWER_CTL_SLEEP_MASK ? 1:0);
//  printf("\t\t[WAKEUP] : %d\r\n", adxl_dev_get_power_ctl(ptr->dev) & POWER_CTL_WAKEUP_MASK ? 1:0);
//  printf("\t[INT_ENABLE] : 0x%02x\r\n", adxl_dev_get_int_enable(ptr->dev));
//  printf("\t\t[DATA_READY] : %d\r\n", adxl_dev_get_int_enable(ptr->dev) & INT_ENABLE_DATA_READY_MASK ? 1:0);
//  printf("\t\t[SINGLE_TAP] : %d\r\n", adxl_dev_get_int_enable(ptr->dev) & INT_ENABLE_SINGLE_TAP_MASK ? 1:0);
//  printf("\t\t[DOUBLE_TAP] : %d\r\n", adxl_dev_get_int_enable(ptr->dev) & INT_ENABLE_DOUBLE_TAP_MASK ? 1:0);
//  printf("\t\t[ACTIVITY] : %d\r\n", adxl_dev_get_int_enable(ptr->dev) & INT_ENABLE_ACTIVITY_MASK ? 1:0);
//  printf("\t\t[INACTIVITY] : %d\r\n", adxl_dev_get_int_enable(ptr->dev) & INT_ENABLE_INACTIVITY_MASK ? 1:0);
//  printf("\t\t[FREEFALL] : %d\r\n", adxl_dev_get_int_enable(ptr->dev) & INT_ENABLE_FREE_FALL_MASK ? 1:0);
//  printf("\t\t[WATERMARK] : %d\r\n", adxl_dev_get_int_enable(ptr->dev) & INT_ENABLE_WATERMARK_MASK ? 1:0);
//  printf("\t\t[OVERRUN] : %d\r\n", adxl_dev_get_int_enable(ptr->dev) & INT_ENABLE_OVERRUN_MASK ? 1:0);
//  printf("\t[INT_MAP] : 0x%02x\r\n", adxl_dev_get_int_map(ptr->dev));
//  printf("\t\t[DATA_READY] : %d\r\n", adxl_dev_get_int_map(ptr->dev) & INT_MAP_DATA_READY_MASK ? 1:0);
//  printf("\t\t[SINGLE_TAP] : %d\r\n", adxl_dev_get_int_map(ptr->dev) & INT_MAP_SINGLE_TAP_MASK ? 1:0);
//  printf("\t\t[DOUBLE_TAP] : %d\r\n", adxl_dev_get_int_map(ptr->dev) & INT_MAP_DOUBLE_TAP_MASK ? 1:0);
//  printf("\t\t[ACTIVITY] : %d\r\n", adxl_dev_get_int_map(ptr->dev) & INT_MAP_ACTIVITY_MASK ? 1:0);
//  printf("\t\t[INACTIVITY] : %d\r\n", adxl_dev_get_int_map(ptr->dev) & INT_MAP_INACTIVITY_MASK ? 1:0);
//  printf("\t\t[FREEFALL] : %d\r\n", adxl_dev_get_int_map(ptr->dev) & INT_MAP_FREE_FALL_MASK ? 1:0);
//  printf("\t\t[WATERMARK] : %d\r\n", adxl_dev_get_int_map(ptr->dev) & INT_MAP_WATERMARK_MASK ? 1:0);
//  printf("\t\t[OVERRUN] : %d\r\n", adxl_dev_get_int_map(ptr->dev) & INT_MAP_OVERRUN_MASK ? 1:0);
//  printf("\t[INT_SOURCE] : 0x%02x\r\n", adxl_dev_get_int_source(ptr->dev));
//  printf("\t\t[DATA_READY] : %d\r\n", adxl_dev_get_int_source(ptr->dev) & INT_SOURCE_DATA_READY_MASK ? 1:0);
//  printf("\t\t[SINGLE_TAP] : %d\r\n", adxl_dev_get_int_source(ptr->dev) & INT_SOURCE_SINGLE_TAP_MASK ? 1:0);
//  printf("\t\t[DOUBLE_TAP] : %d\r\n", adxl_dev_get_int_source(ptr->dev) & INT_SOURCE_DOUBLE_TAP_MASK ? 1:0);
//  printf("\t\t[ACTIVITY] : %d\r\n", adxl_dev_get_int_source(ptr->dev) & INT_SOURCE_ACTIVITY_MASK ? 1:0);
//  printf("\t\t[INACTIVITY] : %d\r\n", adxl_dev_get_int_source(ptr->dev) & INT_SOURCE_INACTIVITY_MASK ? 1:0);
//  printf("\t\t[FREEFALL] : %d\r\n", adxl_dev_get_int_source(ptr->dev) & INT_SOURCE_FREE_FALL_MASK ? 1:0);
//  printf("\t\t[WATERMARK] : %d\r\n", adxl_dev_get_int_source(ptr->dev) & INT_SOURCE_WATERMARK_MASK ? 1:0);
//  printf("\t\t[OVERRUN] : %d\r\n", adxl_dev_get_int_source(ptr->dev) & INT_SOURCE_OVERRUN_MASK ? 1:0);
//  printf("\t[DATA_FORMAT] : 0x%02x\r\n", adxl_dev_get_data_format(ptr->dev));
//  printf("\t\t[SELFTEST] : %d\r\n", adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_SELFTEST_MASK ? 1:0);
//  printf("\t\t[SPI] : %d\r\n", adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_SPI_MASK ? 1:0);
//  printf("\t\t[INT_INVERT] : %d\r\n", adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_INT_INVERT_MASK ? 1:0);
//  printf("\t\t[0] : %d\r\n", adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_0_MASK ? 1:0);
//  printf("\t\t[FULL_RES] : %d\r\n", adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_FULL_RES_MASK ? 1:0);
//  printf("\t\t[JUSTIFY] : %d\r\n", adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_JUSTIFY_MASK ? 1:0);
//  printf("\t\t[RANGE] : %d\r\n", adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_RANGE_MASK);
//  printf("\t\t\t[RANGE] : ");
//  switch( adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_RANGE_MASK ){
//      case DATA_FORMAT_RANGE_2G:
//          printf("-2g..+2g");
//      break;
//
//      case DATA_FORMAT_RANGE_4G:
//          printf("-4g..+4g");
//      break;
//
//      case DATA_FORMAT_RANGE_8G:
//          printf("-8g..+8g");
//      break;
//
//      case DATA_FORMAT_RANGE_16G:
//          printf("-16g..+16g");
//      break;
//
//      default:
//          printf("<undefined>\r\n");
//  }
//  printf("\r\n");
//
//  printf("\t[DATAX0] : 0x%02x\r\n", adxl_dev_get_datax0(ptr->dev));
//  printf("\t[DATAX1] : 0x%02x\r\n", adxl_dev_get_datax1(ptr->dev));
//  printf("\t[DATAY0] : 0x%02x\r\n", adxl_dev_get_datay0(ptr->dev));
//  printf("\t[DATAY1] : 0x%02x\r\n", adxl_dev_get_datay1(ptr->dev));
//  printf("\t[DATAZ0] : 0x%02x\r\n", adxl_dev_get_dataz0(ptr->dev));
//  printf("\t[DATAZ1] : 0x%02x\r\n", adxl_dev_get_dataz1(ptr->dev));
//
//  printf("\t[FIFO_CTL] : 0x%02x\r\n", adxl_dev_get_fifo_ctl(ptr->dev));
//  printf("\t\t[FIFO_MODE] : %d\r\n", adxl_dev_get_fifo_ctl(ptr->dev) & FIFO_CTL_FIFO_MODE);
//  printf("\t\t[TRIGGER] : %d\r\n", adxl_dev_get_fifo_ctl(ptr->dev) & FIFO_CTL_TRIGGER);
//  printf("\t\t[SAMPLES] : %d\r\n", adxl_dev_get_fifo_ctl(ptr->dev) & FIFO_CTL_SAMPLES);
//
//  printf("\t[FIFO_STATUS] : 0x%02x\r\n", adxl_dev_get_fifo_status(ptr->dev));
//  printf("\t\t[ENTRIES] : %d\r\n", adxl_dev_get_fifo_status(ptr->dev) & FIFO_STATUS_ENTRIES_MASK);
//}
