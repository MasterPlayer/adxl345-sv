#include "axi_adxl.h"



int axi_adxl_init(axi_adxl *ptr, uint32_t cfg_baseaddr, uint32_t dev_baseaddr){

	int status = axi_adxl_cfg_init(ptr, cfg_baseaddr);

	status = axi_adxl_dev_init(ptr, dev_baseaddr);

	ptr->has_init = 1;

	return status;
}



int axi_adxl_cfg_init(axi_adxl *ptr, uint32_t baseaddr){
	ptr->cfg = baseaddr;
	return 0;
}



int axi_adxl_dev_init(axi_adxl *ptr, uint32_t baseaddr){
	ptr->dev = baseaddr;
	return 0;
}



int axi_adxl_enable(axi_adxl *ptr, uint8_t i2c_addr, uint32_t request_interval){
	// if struct not init, return error

	int status = 0;

	if (!axi_adxl_has_init(ptr)){
		return ERR_UNINIT;
	}

	uint32_t timeout_timer = TIMEOUT_TIMER_LIMIT;
	// perform reset configuration on axi_adxl configuration
	adxl_cfg_reset(ptr->cfg);

	// checking for reset is complete
	while(adxl_cfg_has_reset(ptr->cfg)){
		if (!timeout_timer){
			printf("[DEV_ENABLE] : temeout reset\r\n");
			return ERR_RESET_INFINITE;
		}
		timeout_timer--;
	}

	adxl_cfg_set_request_interval(ptr->cfg, request_interval);

	adxl_cfg_set_i2c_addr(ptr->cfg, i2c_addr);

	adxl_cfg_enable(ptr->cfg);

	adxl_cfg_allow_irq(ptr->cfg);

	timeout_timer = TIMEOUT_TIMER_LIMIT;

	while (!adxl_cfg_has_enable(ptr->cfg)){
		if (!timeout_timer){
			printf("[DEV_ENABLE] : timeout enable\r\n");
			return ERR_TIMEOUT_AXI_CFG;
		}
		timeout_timer--;
	}

	timeout_timer = TIMEOUT_TIMER_LIMIT;

	while (!adxl_cfg_has_link(ptr->cfg)){
		if (!timeout_timer){
			printf("[DEV_ENABLE] : timeout link establish\r\n");
			return ERR_LINK_LOST;
		}
		timeout_timer--;
	}

	adxl_dev_set_bw_rate(ptr->dev, BW_RATE_3200);

	status = axi_adxl_set_range(ptr, DATA_FORMAT_RANGE_2G);
	status = axi_adxl_set_full_resolution(ptr, DATA_FORMAT_FULL_RES);

	adxl_dev_set_power_ctl(ptr->dev, POWER_CTL_MEASURE_MASK);

	// insert condition where checking this byte is applied new value

	if (adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_FULL_RES_MASK){
		printf("[DATA_FORMAT] : Full Resolution\r\n");
	}else{
		printf("[DATA_FORMAT] : 10 bit Resolution\r\n");
	}

	switch((adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_RANGE_MASK)){
		case DATA_FORMAT_RANGE_2G : printf("[RANGE] : -2G..+2G\r\n"); break;
		case DATA_FORMAT_RANGE_4G : printf("[RANGE] : -4G..-4G\r\n"); break;
		case DATA_FORMAT_RANGE_8G : printf("[RANGE] : -8G..-8G\r\n"); break;
		case DATA_FORMAT_RANGE_16G : printf("[RANGE] : -16G..+16G\r\n"); break;
		default : printf("[RANGE] : invalid\r\n"); break;
	}


	return ADXL_OK;
}


int axi_adxl_disable(axi_adxl *ptr){
	// disable iic device
	// check for sw structure was initialized
	if (!axi_adxl_has_init(ptr)){
		printf("[DEV_DISABLE] : sw structure unitialized\r\n");
		return ERR_UNINIT;
	}
	// send data for stop power ctl
	adxl_dev_set_power_ctl(ptr->dev, (adxl_dev_get_power_ctl(ptr->dev) & ~POWER_CTL_MEASURE_MASK));

	uint32_t timeout_timer = TIMEOUT_TIMER_LIMIT;

	while(adxl_dev_get_power_ctl(ptr->dev) & POWER_CTL_MEASURE_MASK){
		if (!timeout_timer){
			printf("[DEV_DISABLE] : timeout dev stop msmt\r\n");
			return ERR_DEVICE_MSMT_UNSTOP;
		}
		timeout_timer--;
	}

	timeout_timer = TIMEOUT_TIMER_LIMIT;

	adxl_cfg_disable(ptr->cfg);

	printf("[DEV_DISABLE] : perform stop hw component");

	while(adxl_cfg_has_on_work(ptr->cfg)){
		printf(".");
	}
	printf("Complete\r\n");


	return 0;
}



/*
 * This Function only disable requesting data from device,
 * without stop device's work. Sending Functions ability to work.
 * */
int axi_adxl_disable_requesting(axi_adxl *ptr){
	int status = axi_adxl_has_runned(ptr);
	if (status != ADXL_OK){
		printf("[DSBL_RQSTN] : return status %d", status);
		return status;
	}

	adxl_cfg_disable(ptr->cfg);

	return status;
}



int axi_adxl_change_bw(axi_adxl *ptr, uint8_t value){
	int status = axi_adxl_has_runned(ptr);
	if (status != ADXL_OK){
		printf("[CHG_BW] : return with code %d", status);
	}

	if (value & ~(BW_RATE_RATE_MASK + BW_RATE_LOW_POWER_MASK)){
		printf("[CHANGE_BW] : used mask for BW_RATE set : d%\r\n", (value & (BW_RATE_RATE_MASK + BW_RATE_LOW_POWER_MASK)));
	}

	adxl_dev_set_bw_rate(ptr->dev, (value & (BW_RATE_RATE_MASK + BW_RATE_LOW_POWER_MASK)));

	return 0;
}


int axi_adxl_has_low_power(axi_adxl *ptr){
	int status = axi_adxl_has_runned(ptr);
	if (status != ADXL_OK){
		printf("[HAS_LOW] : return with code %d", status);
	}

	return (adxl_dev_get_bw_rate(ptr->dev) & BW_RATE_LOW_POWER_MASK);

}



int axi_adxl_get_axis(axi_adxl *ptr){

	int status = axi_adxl_has_runned(ptr);
	if (status != ADXL_OK){
		printf("[GET_AXIS] : return with code %d", status);
	}

	ptr->axis.x = axi_adxl_get_datax(ptr);
	ptr->axis.y = ((uint16_t)adxl_dev_get_datay1(ptr->dev)<<8) + (uint16_t)adxl_dev_get_datay0(ptr->dev);
	ptr->axis.z = ((uint16_t)adxl_dev_get_dataz1(ptr->dev)<<8) + (uint16_t)adxl_dev_get_dataz0(ptr->dev);

	return ADXL_OK;
}



int axi_adxl_has_runned(axi_adxl *ptr){

	if (!axi_adxl_has_init(ptr)){
		return ERR_UNINIT;
	}

	if (!adxl_cfg_has_link(ptr->cfg)){
		return ERR_LINK_LOST;
	}

//	if (!adxl_cfg_has_enable(ptr->cfg)){
//		return ERR_AXICFG_DISABLE;
//	}

	return ADXL_OK;

}



int axi_adxl_set_full_resolution(axi_adxl *ptr, uint8_t flaq){
	int status = axi_adxl_has_runned(ptr);
	if (status != ADXL_OK){
		printf("[DF_FULLRES] return code %d\r\n", status);
		return status;
	}

	adxl_dev_set_data_format(ptr->dev, ((adxl_dev_get_data_format(ptr->dev) & ~DATA_FORMAT_FULL_RES) | (flaq & DATA_FORMAT_FULL_RES_MASK)));

	return ADXL_OK;
}



int axi_adxl_set_range(axi_adxl *ptr, uint8_t range){

	int status = axi_adxl_has_runned(ptr);

	if (status != ADXL_OK){
		printf("[DF_RANGE] return code %d\r\n", status);
		return status;
	}

	adxl_dev_set_data_format(ptr->dev, ((adxl_dev_get_data_format(ptr->dev) & ~DATA_FORMAT_RANGE_MASK) | range));

	return ADXL_OK;
}



int axi_adxl_get_range(axi_adxl *ptr, uint8_t *range){
	int status = axi_adxl_has_runned(ptr);

	if (status != ADXL_OK){
		printf("[GET_RANGE] return code %d\t\n", status);
	}

	*range = (adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_RANGE_MASK);

	return ADXL_OK;
}


int axi_adxl_set_offset(axi_adxl *ptr, offset_param offt_prm, enum mask mask_value){

	int status = axi_adxl_has_runned(ptr);
	if (status != ADXL_OK){
		printf("[SET_OFFT] : return with code %d", status);
	}


	if (mask_value & MASK_X){
		adxl_dev_set_ofsx(ptr->dev, offt_prm.x);
	}

	if (mask_value & MASK_Y){
		adxl_dev_set_ofsy(ptr->dev, offt_prm.y);
	}

	if (mask_value & MASK_Z){
		adxl_dev_set_ofsz(ptr->dev, offt_prm.z);
	}

	return ADXL_OK;
}


int axi_adxl_get_offset(axi_adxl *ptr, offset_param *offt_prm){
	int status = axi_adxl_has_runned(ptr);
	if (status != ADXL_OK) {
		printf("[GET_OFFT] : return with code %d", status);
		return status;
	}

	offt_prm->x = adxl_dev_get_ofsx(ptr->dev);
	offt_prm->y = adxl_dev_get_ofsy(ptr->dev);
	offt_prm->z = adxl_dev_get_ofsz(ptr->dev);

	return ADXL_OK;
}

int axi_adxl_get_offset_x(axi_adxl *ptr, uint8_t *offset){
	int status = axi_adxl_has_runned(ptr);
	if (status != ADXL_OK){
		return status;
	}

	*offset = adxl_dev_get_ofsx(ptr->dev);

	return status;
}



int axi_adxl_get_offset_y(axi_adxl *ptr, uint8_t *offset){
	int status = axi_adxl_has_runned(ptr);
	if (status != ADXL_OK){
		return status;
	}

	*offset = adxl_dev_get_ofsy(ptr->dev);

	return status;
}



int axi_adxl_get_offset_z(axi_adxl *ptr, uint8_t *offset){
	int status = axi_adxl_has_runned(ptr);
	if (status != ADXL_OK){
		return status;
	}

	*offset = adxl_dev_get_ofsz(ptr->dev);

	return status;
}



int axi_adxl_get_gravity(axi_adxl *ptr, g_coord *g_ptr){

	int status = axi_adxl_has_runned(ptr);

	if (status != ADXL_OK){
		printf("[GET_GRVTY] : return with code %d", status);
	}

	if (adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_FULL_RES){
		g_ptr->x = (float)axi_adxl_get_datax(ptr)/SENSITIVITY_FULL_RES;
		g_ptr->y = (float)axi_adxl_get_datay(ptr)/SENSITIVITY_FULL_RES;
		g_ptr->z = (float)axi_adxl_get_dataz(ptr)/SENSITIVITY_FULL_RES;
	}else{
		switch (adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_RANGE_MASK){

			case DATA_FORMAT_RANGE_2G:
				g_ptr->x = (float)axi_adxl_get_datax(ptr)/SENSITIVITY_2G;
				g_ptr->y = (float)axi_adxl_get_datay(ptr)/SENSITIVITY_2G;
				g_ptr->z = (float)axi_adxl_get_dataz(ptr)/SENSITIVITY_2G;
			break;

			case DATA_FORMAT_RANGE_4G:
				g_ptr->x = (float)axi_adxl_get_datax(ptr)/SENSITIVITY_4G;
				g_ptr->y = (float)axi_adxl_get_datay(ptr)/SENSITIVITY_4G;
				g_ptr->z = (float)axi_adxl_get_dataz(ptr)/SENSITIVITY_4G;
			break;

			case DATA_FORMAT_RANGE_8G:
				g_ptr->x = (float)axi_adxl_get_datax(ptr)/SENSITIVITY_8G;
				g_ptr->y = (float)axi_adxl_get_datay(ptr)/SENSITIVITY_8G;
				g_ptr->z = (float)axi_adxl_get_dataz(ptr)/SENSITIVITY_8G;
			break;

			case DATA_FORMAT_RANGE_16G:
				g_ptr->x = (float)axi_adxl_get_datax(ptr)/SENSITIVITY_16G;
				g_ptr->y = (float)axi_adxl_get_datay(ptr)/SENSITIVITY_16G;
				g_ptr->z = (float)axi_adxl_get_dataz(ptr)/SENSITIVITY_16G;
			break;

			default :
				printf("[GET_GRVTY] : Unknown data format : 0x%02x\r\n", adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_RANGE_MASK);
			break;
		}
	}

	return ADXL_OK;

}



int axi_adxl_calibration(axi_adxl *ptr){

	int status = axi_adxl_has_runned(ptr);
	if (status != ADXL_OK){
		printf("[CALIBRTN] : return with code %d", status);
	}

	printf("[CALIBRTN] : Start process...\r\n");

	offset_param offset;

	offset.x = 0;
	offset.y = 0;
	offset.z = 0;

	offset_param offset_lsb;
	// zeroes value
	adxl_dev_set_ofsx(ptr->dev, offset.x);
	adxl_dev_set_ofsy(ptr->dev, offset.y);
	adxl_dev_set_ofsz(ptr->dev, offset.z);

	float avg_x = 0;
	float avg_y = 0;
	float avg_z = 0;

	for (int i = 0; i < CALIBRATION_STEPS; i++){
		axi_adxl_get_axis(ptr);
		avg_x += ptr->axis.x;
		avg_y += ptr->axis.y;
		avg_z += ptr->axis.z;
		for (int delay_cnt = 0; delay_cnt < 200000; delay_cnt++);
	}

	avg_x /= CALIBRATION_STEPS;
	avg_y /= CALIBRATION_STEPS;
	avg_z /= CALIBRATION_STEPS;


	if (adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_FULL_RES_MASK){
		offset.x = (int8_t)(avg_x - 0);
		offset.y = (int8_t)(avg_y - 0);

		offset.z = (int8_t)(avg_z - SENSITIVITY_FULL_RES);


		offset_lsb.x = (int8_t)((-1)*((float)(offset.x/(float)(SCALE_RATIO_FULL_RES))));
		offset_lsb.y = (int8_t)((-1)*((float)(offset.y/(float)(SCALE_RATIO_FULL_RES))));
		offset_lsb.z = (int8_t)((-1)*((float)(offset.z/(float)(SCALE_RATIO_FULL_RES))));
	}else{
		switch (adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_RANGE_MASK){
			case DATA_FORMAT_RANGE_2G :
				offset.x = (int8_t)(avg_x - 0);
				offset.y = (int8_t)(avg_y - 0);
				offset.z = (int8_t)(avg_z - 256);
				offset_lsb.x = (int8_t)((-1)*((float)(offset.x/(float)(SCALE_RATIO_2G))));
				offset_lsb.y = (int8_t)((-1)*((float)(offset.y/(float)(SCALE_RATIO_2G))));
				offset_lsb.z = (int8_t)((-1)*((float)(offset.z/(float)(SCALE_RATIO_2G))));
			break;

			case DATA_FORMAT_RANGE_4G :
				offset.x = (int8_t)(avg_x - 0);
				offset.y = (int8_t)(avg_y - 0);
				offset.z = (int8_t)(avg_z - 128);
				offset_lsb.x = (int8_t)((-1)*((float)(offset.x/(float)(SCALE_RATIO_4G))));
				offset_lsb.y = (int8_t)((-1)*((float)(offset.y/(float)(SCALE_RATIO_4G))));
				offset_lsb.z = (int8_t)((-1)*((float)(offset.z/(float)(SCALE_RATIO_4G))));
			break;

			case DATA_FORMAT_RANGE_8G :
				offset.x = (int8_t)(avg_x - 0);
				offset.y = (int8_t)(avg_y - 0);
				offset.z = (int8_t)(avg_z - 64);
				offset_lsb.x = (int8_t)((-1)*((float)(offset.x/(float)(SCALE_RATIO_8G))));
				offset_lsb.y = (int8_t)((-1)*((float)(offset.y/(float)(SCALE_RATIO_8G))));
				offset_lsb.z = (int8_t)((-1)*((float)(offset.z/(float)(SCALE_RATIO_8G))));
			break;
			case DATA_FORMAT_RANGE_16G :
				offset.x = (int8_t)(avg_x - 0);
				offset.y = (int8_t)(avg_y - 0);
				offset.z = (int8_t)(avg_z - 32);
				offset_lsb.x = (int8_t)((-1)*((float)(offset.x/(float)(SCALE_RATIO_16G))));
				offset_lsb.y = (int8_t)((-1)*((float)(offset.y/(float)(SCALE_RATIO_16G))));
				offset_lsb.z = (int8_t)((-1)*((float)(offset.z/(float)(SCALE_RATIO_16G))));
			break;
			default :
				printf("\t[RANGE] : not applied\r\n");
			break;
		}

	}


	printf("\t[AVERAGE]\r\n");
	printf("\t\t[X] : \t%3.3f\r\n", avg_x);
	printf("\t\t[Y] : \t%3.3f\r\n", avg_y);
	printf("\t\t[Z] : \t%3.3f\r\n", avg_z);
	printf("\t[OFFSET]\r\n");
	printf("\t\t[X] : \t%d\r\n", offset.x);
	printf("\t\t[Y] : \t%d\r\n", offset.y);
	printf("\t\t[Z] : \t%d\r\n", offset.z);
	printf("\t[LSB]\r\n");
	printf("\t\t[X] : \t%d\r\n", offset_lsb.x);
	printf("\t\t[Y] : \t%d\r\n", offset_lsb.y);
	printf("\t\t[Z] : \t%d\r\n", offset_lsb.z);

	axi_adxl_set_offset(ptr, offset_lsb, MASK_X | MASK_Y | MASK_Z);

	printf("[CALIBRTN] : completed\r\n");

	return ADXL_OK;
}



int axi_adxl_get_roll(axi_adxl *ptr, float *roll){
	int status = axi_adxl_has_runned(ptr);
	if (status != ADXL_OK){
		return status;
	}

	g_coord g;

	status = axi_adxl_get_gravity(ptr, &g);
	*roll = atan(g.y/ sqrt(pow(g.x, 2) + pow(g.z, 2))) * 180 / PI;

	return status;
}



int axi_adxl_get_pitch(axi_adxl *ptr, float *pitch){
	int status = axi_adxl_has_runned(ptr);
	if (status != ADXL_OK){
		return status;
	}

	g_coord g;

	status = axi_adxl_get_gravity(ptr, &g);
	*pitch = atan(-1 * g.x / sqrt(pow(g.y, 2) + pow(g.z, 2))) * 180 / PI;
	return ADXL_OK;
}



int axi_adxl_set_thresh_tap(axi_adxl *ptr, uint8_t thresh_tap){
	int status = axi_adxl_has_runned(ptr);
	if (status != ADXL_OK){
		return status;
	}

	adxl_dev_set_thresh_tap(ptr->dev, thresh_tap);

	return status;
}



int axi_adxl_get_thresh_tap(axi_adxl *ptr, uint8_t *thresh_tap){
	int status = axi_adxl_has_runned(ptr);
	if (status != ADXL_OK){
		return status;
	}

	*thresh_tap = adxl_dev_get_thresh_tap(ptr->dev);

	return status;
}


int axi_adxl_set_dur(axi_adxl *ptr, uint8_t dur){
	int status = axi_adxl_has_runned(ptr);
	if (status != ADXL_OK){
		return status;
	}

	adxl_dev_set_dur(ptr->dev, dur);

	return status;
}



int axi_adxl_get_dur(axi_adxl *ptr, uint8_t *dur){
	int status = axi_adxl_has_runned(ptr);
	if (status != ADXL_OK){
		return status;
	}

	return status;
}



int axi_adxl_set_latency(axi_adxl *ptr, uint8_t latency){
	int status = axi_adxl_has_runned(ptr);
	if (status != ADXL_OK){
		return status;
	}

	adxl_dev_set_latent(ptr->dev, latency);

	return status;
}


int axi_adxl_get_latency(axi_adxl *ptr, uint8_t *latency){
	int status = axi_adxl_has_runned(ptr);
	if (status != ADXL_OK){
		return status;
	}

	*latency = adxl_dev_get_latent(ptr->dev);

	return status;

}


int axi_adxl_set_window(axi_adxl *ptr, uint8_t window){
	int status = axi_adxl_has_runned(ptr);
	if (status != ADXL_OK){
		return status;
	}

	adxl_dev_set_window(ptr->dev, window);

	return status;

}

int axi_adxl_get_window(axi_adxl *ptr, uint8_t *window){
	int status = axi_adxl_has_runned(ptr);
	if (status != ADXL_OK){
		return status;
	}

	*window = adxl_dev_get_window(ptr->dev);

	return status;

}


int axi_adxl_set_int_map(axi_adxl *ptr, uint8_t mask, uint8_t map){
	int status = axi_adxl_has_runned(ptr);
	if (status != ADXL_OK){
		return status;
	}

	adxl_dev_set_int_map(ptr->dev, (adxl_dev_get_int_map(ptr->dev) & map));

	return status;
}





int axi_adxl_int_enable(axi_adxl *ptr, uint8_t mask){
	int status = axi_adxl_has_runned(ptr);
	if (status != ADXL_OK){
		return status;
	}

	adxl_dev_set_int_enable(ptr->dev, (adxl_dev_get_int_enable(ptr->dev) | mask));

	return status;
}



int axi_adxl_int_disable(axi_adxl *ptr, uint8_t mask){

	int status = axi_adxl_has_runned(ptr);
	if (status != ADXL_OK){
		return status;
	}

	adxl_dev_set_int_enable(ptr->dev, (adxl_dev_get_int_enable(ptr->dev) & ~mask ));

	return status;
}



int axi_adxl_set_tap_axes_active(axi_adxl *ptr, enum mask mask_value){
	int status = axi_adxl_has_runned(ptr);
	if (status != ADXL_OK){
		return status;
	}

	adxl_dev_set_tap_axes(ptr->dev, (adxl_dev_get_tap_axes(ptr->dev) | mask_value));

	return status;
}



int axi_adxl_int_invert(axi_adxl *ptr){
	int status = axi_adxl_has_runned(ptr);
	if (status != ADXL_OK){
		return status;
	}

	adxl_dev_set_data_format(ptr->dev, adxl_dev_get_data_format(ptr->dev) ^ DATA_FORMAT_INT_INVERT_MASK);

	return status;
}



int axi_adxl_set_act_ctl(axi_adxl *ptr, int act_mode, enum mask mask_value){

	int status = axi_adxl_has_runned(ptr);
	if (status != ADXL_OK){
		return status;
	}

	adxl_dev_set_act_inact_ctl(ptr->dev, (((act_mode<<4) | (mask_value<<4)) & ACT_MASK) | ((adxl_dev_get_act_inact_ctl(ptr->dev)) & ~ACT_MASK));

	return status;
}


int axi_adxl_set_inact_ctl(axi_adxl *ptr, int act_mode, enum mask mask_value){
	int status = axi_adxl_has_runned(ptr);
	if (status != ADXL_OK){
		return status;
	}

	adxl_dev_set_act_inact_ctl(ptr->dev, (((act_mode) | (mask_value)) & INACT_MASK) | ((adxl_dev_get_act_inact_ctl(ptr->dev)) & ~INACT_MASK));

	return status;
}


int axi_adxl_set_thresh_act(axi_adxl *ptr, uint8_t value){
	int status = axi_adxl_has_runned(ptr);
	if (status != ADXL_OK){
		return status;
	}

	adxl_dev_set_thresh_act(ptr->dev, value);

	return status;
}


int axi_adxl_get_thresh_act(axi_adxl *ptr, uint8_t *value){

	int status = axi_adxl_has_runned(ptr);
	if (status != ADXL_OK){
		return status;
	}

	*value = adxl_dev_get_thresh_act(ptr->dev);

	return status;
}

int axi_adxl_set_thresh_inact(axi_adxl *ptr, uint8_t value){
	int status = axi_adxl_has_runned(ptr);
	if (status != ADXL_OK){
		return status;
	}

	adxl_dev_set_thresh_inact(ptr->dev, value);

	return status;
}


int axi_adxl_get_thresh_inact(axi_adxl *ptr, uint8_t *value){
	int status = axi_adxl_has_runned(ptr);
	if (status != ADXL_OK){
		return status;
	}

	*value = adxl_dev_get_thresh_inact(ptr->dev);

	return status;

}

int axi_adxl_set_time_inact(axi_adxl *ptr, uint8_t value){
	int status = axi_adxl_has_runned(ptr);
	if (status != ADXL_OK){
		return status;
	}

	adxl_dev_set_time_inact(ptr->dev, value);

	return status;

}

int axi_adxl_get_time_inact(axi_adxl *ptr, uint8_t *value){
	int status = axi_adxl_has_runned(ptr);
	if (status != ADXL_OK){
		return status;
	}

	*value = adxl_dev_get_time_inact(ptr->dev);

	return status;

}


void axi_adxl_debug(axi_adxl *ptr){
	printf("[CONFIGURATION SPACE]\r\n");
	printf("\t[VERSION] : %d.%d\r\n", adxl_cfg_get_version_major(ptr->cfg), adxl_cfg_get_version_minor(ptr->cfg));
	printf("\t[I2C ADDRESS] : 0x%02x\r\n", adxl_cfg_get_i2c_addr(ptr->cfg));
	printf("\t[LNK] : %d\r\n", adxl_cfg_has_link(ptr->cfg));
	printf("\t[ON WORK] : %d\r\n", adxl_cfg_has_on_work(ptr->cfg));
	printf("\t[IRQ] : %s\r\n", adxl_cfg_has_allow_irq(ptr->cfg) ? "allow" : "unallow");
	printf("\t[ENABLED] : %d\r\n", adxl_cfg_has_enable(ptr->cfg));
	printf("\t[REQUEST INTERVAL] : %d periods\r\n", adxl_cfg_get_request_interval(ptr->cfg));
	printf("\t[DATA WIDTH] : %d bytes\r\n", adxl_cfg_get_data_width(ptr->cfg));
	printf("\t[READ VALID COUNT] : %d words\r\n", adxl_cfg_get_read_valid_count(ptr->cfg));
	printf("\t[WRITE VALID COUNT] : %d words\r\n", adxl_cfg_get_write_valid_count(ptr->cfg));
	printf("[WRITE TRANSACTIONS] : %d\r\n", adxl_cfg_get_write_transactions(ptr->cfg));
	printf("[READ_TRANSACTIONS] : %d\r\n", adxl_cfg_get_read_transactions(ptr->cfg));
	printf("[CLK_PERIOD] : %3.3f MHz\r\n", (float)(adxl_cfg_get_clk_period(ptr->cfg)/1000000));
	printf("\r\n");

	printf("[DEVICE SPACE]\r\n");
	printf("\t[DEVICE ID] : 0x%02x\r\n", adxl_dev_get_device_id(ptr->dev));

	printf("\t[THRESH_TAP] : 0x%02x\r\n", adxl_dev_get_thresh_tap(ptr->dev));
	printf("\t\t[THRESH_TAP_G] : %3.3f g\r\n", (float)(adxl_dev_get_thresh_tap(ptr->dev) * SCALE_THRESH_TAP));

	printf("\t[OFSX] : 0x%02x\r\n", adxl_dev_get_ofsx(ptr->dev));
	printf("\t\t[OFSX] : %3.3f g\r\n", (float)((int8_t)adxl_dev_get_ofsx(ptr->dev) * SCALE_OFS));

	printf("\t[OFSY] : 0x%02x\r\n", adxl_dev_get_ofsy(ptr->dev));
	printf("\t\t[OFSY_G] : %3.3f g\r\n", (float)((int8_t)adxl_dev_get_ofsy(ptr->dev) * SCALE_OFS));

	printf("\t[OFSZ] : 0x%02x\r\n", adxl_dev_get_ofsz(ptr->dev));
	printf("\t\t[OFSZ_G] : %3.3f g\r\n", (float)((int8_t)adxl_dev_get_ofsz(ptr->dev) * SCALE_OFS));

	printf("\t[DUR] : 0x%02x\r\n", adxl_dev_get_dur(ptr->dev));
	printf("\t\t[DUR] : %3.3f sec\r\n", (float)(adxl_dev_get_dur(ptr->dev) * SCALE_DUR));

	printf("\t[LATENT] : 0x%02x\r\n", adxl_dev_get_latent(ptr->dev));
	printf("\t\t[LATENT_S] : %3.3f sec\r\n", (float)(adxl_dev_get_latent(ptr->dev) * SCALE_LATENT));

	printf("\t[WINDOW] : 0x%02x\r\n", adxl_dev_get_window(ptr->dev));
	printf("\t\t[WINDOW_S] : %3.3f sec\r\n", (float)(adxl_dev_get_window(ptr->dev) * SCALE_WINDOW));

	printf("\t[THRESH_ACT] : 0x%02x\r\n", adxl_dev_get_thresh_act(ptr->dev));
	printf("\t\t[THRESH_ACT_G] : %3.3f g\r\n", (float)(adxl_dev_get_thresh_act(ptr->dev) * SCALE_THRESH_ACT));

	printf("\t[THRESH_INACT] : 0x%02x\r\n", adxl_dev_get_thresh_inact(ptr->dev));
	printf("\t\t[THRESH_INACT_G] : %3.3f g\r\n", (float)(adxl_dev_get_thresh_inact(ptr->dev) * SCALE_THRESH_INACT));

	printf("\t[TIME_INACT] : 0x%02x\r\n", adxl_dev_get_time_inact(ptr->dev));
	printf("\t[TIME_INACT_S] : %3.3f s\r\n", (float)(adxl_dev_get_time_inact(ptr->dev) * SCALE_TIME_INACT));

	printf("\t[ACT_INACT_CTL] : 0x%02x\r\n", adxl_dev_get_act_inact_ctl(ptr->dev));
	printf("\t\t[ACT_AC/DC] : %d \r\n", (adxl_dev_get_act_inact_ctl(ptr->dev) & ACT_INACT_CTL_ACT_ACDC_MASK) ? 1:0);
	printf("\t\t[ACT_X EN] : %d\r\n", adxl_dev_get_act_inact_ctl(ptr->dev) & ACT_INACT_CTL_ACT_X_EN_MASK ? 1:0);
	printf("\t\t[ACT_Y EN] : %d\r\n", adxl_dev_get_act_inact_ctl(ptr->dev) & ACT_INACT_CTL_ACT_Y_EN_MASK ? 1:0);
	printf("\t\t[ACT_Z EN] : %d\r\n", adxl_dev_get_act_inact_ctl(ptr->dev) & ACT_INACT_CTL_ACT_Z_EN_MASK ? 1:0);
	printf("\t\t[INACT AC/DC] : %d\r\n", adxl_dev_get_act_inact_ctl(ptr->dev) & ACT_INACT_CTL_INACT_ACDC_MASK ? 1:0);
	printf("\t\t[INACT X EN] : %d\r\n", adxl_dev_get_act_inact_ctl(ptr->dev) & ACT_INACT_CTL_INACT_X_EN_MASK ? 1:0);
	printf("\t\t[INACT Y EN] : %d\r\n", adxl_dev_get_act_inact_ctl(ptr->dev) & ACT_INACT_CTL_INACT_Y_EN_MASK ? 1:0);
	printf("\t\t[INACT Z EN] : %d\r\n", adxl_dev_get_act_inact_ctl(ptr->dev) & ACT_INACT_CTL_INACT_Z_EN_MASK ? 1:0);

	printf("\t[THRESH_FF] : 0x%02x : \r\n", adxl_dev_get_thresh_ff(ptr->dev));
	printf("\t\t[THRESH_FF] : %3.3f: \r\n", (float)(adxl_dev_get_thresh_ff(ptr->dev) * SCALE_THRESH_FF));

	printf("\t[TIME_FF] : 0x%02x\r\n", adxl_dev_get_time_ff(ptr->dev));
	printf("\t\t[TIME_FF] : %3.3f\r\n", (float)(adxl_dev_get_time_ff(ptr->dev) * SCALE_TIME_FF));

	printf("\t[TAP_AXES] : 0x%02x\r\n", adxl_dev_get_tap_axes(ptr->dev));
	printf("\t\t[SUPRESS] : %d\r\n", adxl_dev_get_tap_axes(ptr->dev) & TAP_AXES_SUPRESS_MASK ? 1:0);
	printf("\t\t[TAP_X_EN] : %d\r\n", adxl_dev_get_tap_axes(ptr->dev) & TAP_AXES_TAP_X_EN_MASK ? 1:0);
	printf("\t\t[TAP_Y_EN] : %d\r\n", adxl_dev_get_tap_axes(ptr->dev) & TAP_AXES_TAP_Y_EN_MASK ? 1:0);
	printf("\t\t[TAP_Z_EN] : %d\r\n", adxl_dev_get_tap_axes(ptr->dev) & TAP_AXES_TAP_Z_EN_MASK ? 1:0);
	printf("\t[ACT_TAP_STATUS] : 0x%02x\r\n", adxl_dev_get_act_tap_status(ptr->dev));
	printf("\t\t[ACT_X_SRC] : %d\r\n", adxl_dev_get_act_tap_status(ptr->dev) & ACT_TAP_STATUS_ACT_X_SRC_MASK ? 1:0);
	printf("\t\t[ACT_Y_SRC] : %d\r\n", adxl_dev_get_act_tap_status(ptr->dev) & ACT_TAP_STATUS_ACT_Y_SRC_MASK ? 1:0);
	printf("\t\t[ACT_Z_SRC] : %d\r\n", adxl_dev_get_act_tap_status(ptr->dev) & ACT_TAP_STATUS_ACT_Z_SRC_MASK ? 1:0);
	printf("\t\t[ASLEEP] : %d\r\n", adxl_dev_get_act_tap_status(ptr->dev) & ACT_TAP_STATUS_ASLEEP_MASK ? 1:0);
	printf("\t\t[TAP_X_SRC] : %d\r\n", adxl_dev_get_act_tap_status(ptr->dev) & ACT_TAP_STATUS_TAP_X_SRC_MASK ? 1:0);
	printf("\t\t[TAP_Y_SRC] : %d\r\n", adxl_dev_get_act_tap_status(ptr->dev) & ACT_TAP_STATUS_TAP_Y_SRC_MASK ? 1:0);
	printf("\t\t[TAP_Z_SRC] : %d\r\n", adxl_dev_get_act_tap_status(ptr->dev) & ACT_TAP_STATUS_TAP_Z_SRC_MASK ? 1:0);

	printf("\t[BW_RATE] : 0x%02x\r\n", adxl_dev_get_bw_rate(ptr->dev));
	printf("\t\t[LOW_POWER] : %d\r\n", adxl_dev_get_bw_rate(ptr->dev) & BW_RATE_LOW_POWER_MASK ? 1:0);
	printf("\t\t[RATE] : %d\r\n", adxl_dev_get_bw_rate(ptr->dev) & BW_RATE_RATE_MASK);
	printf("\t\t\t[RATE_HZ] : ");
	uint8_t bw_rate = adxl_dev_get_bw_rate(ptr->dev) & (BW_RATE_RATE_MASK | BW_RATE_LOW_POWER_MASK);
	switch(bw_rate){
		case BW_RATE_3200 :
			printf("3200");
		break;

		case BW_RATE_1600 :
			printf("1600");
		break;

		case BW_RATE_800 :
			printf("800");
		break;

		case BW_RATE_400:
			printf("400");
		break;

		case BW_RATE_200:
			printf("200");
		break;

		case BW_RATE_100:
			printf("100");
		break;

		case BW_RATE_50:
			printf("50");
		break;

		case BW_RATE_25:
			printf("25");
		break;

		case BW_RATE_12_5:
			printf("12.5");
		break;

		case BW_RATE_6_25:
			printf("6.25");
		break;

		case BW_RATE_3_13:
			printf("3.125");
		break;

		case BW_RATE_1_56:
			printf("1.56125");
		break;

		case BW_RATE_0_78:
			printf("0.78");
		break;

		case BW_RATE_0_39:
			printf("0.39");
		break;

		case BW_RATE_0_20:
			printf("0.2");
		break;

		case BW_RATE_0_10:
			printf("0.1");
		break;

		case BW_RATE_400_LP:
			printf("low_power 400");
		break;

		case BW_RATE_200_LP:
			printf("low_power 200");
		break;

		case BW_RATE_100_LP:
			printf("low_power 100");
		break;

		case BW_RATE_50_LP:
			printf("low_power 50");
		break;

		case BW_RATE_25_LP:
			printf("low_power 25");
		break;

		case BW_RATE_12_5_LP:
			printf("low_power 12.5");
		break;

		default :
			printf("<undefined>");

	}
	printf(" Hz\r\n");
	printf("\t[POWER_CTL] : 0x%02x\r\n", adxl_dev_get_power_ctl(ptr->dev));
	printf("\t\t[LINK] : %d\r\n", adxl_dev_get_power_ctl(ptr->dev) & POWER_CTL_LINK_MASK ? 1:0);
	printf("\t\t[AUTOSLEEP] : %d\r\n", adxl_dev_get_power_ctl(ptr->dev) & POWER_CTL_AUTO_SLEEP_MASK ? 1:0);
	printf("\t\t[MEASURE] : %d\r\n", adxl_dev_get_power_ctl(ptr->dev) & POWER_CTL_MEASURE_MASK ? 1:0);
	printf("\t\t[SLEEP] : %d\r\n", adxl_dev_get_power_ctl(ptr->dev) & POWER_CTL_SLEEP_MASK ? 1:0);
	printf("\t\t[WAKEUP] : %d\r\n", adxl_dev_get_power_ctl(ptr->dev) & POWER_CTL_WAKEUP_MASK ? 1:0);
	printf("\t[INT_ENABLE] : 0x%02x\r\n", adxl_dev_get_int_enable(ptr->dev));
	printf("\t\t[DATA_READY] : %d\r\n", adxl_dev_get_int_enable(ptr->dev) & INT_ENABLE_DATA_READY_MASK ? 1:0);
	printf("\t\t[SINGLE_TAP] : %d\r\n", adxl_dev_get_int_enable(ptr->dev) & INT_ENABLE_SINGLE_TAP_MASK ? 1:0);
	printf("\t\t[DOUBLE_TAP] : %d\r\n", adxl_dev_get_int_enable(ptr->dev) & INT_ENABLE_DOUBLE_TAP_MASK ? 1:0);
	printf("\t\t[ACTIVITY] : %d\r\n", adxl_dev_get_int_enable(ptr->dev) & INT_ENABLE_ACTIVITY_MASK ? 1:0);
	printf("\t\t[INACTIVITY] : %d\r\n", adxl_dev_get_int_enable(ptr->dev) & INT_ENABLE_INACTIVITY_MASK ? 1:0);
	printf("\t\t[FREEFALL] : %d\r\n", adxl_dev_get_int_enable(ptr->dev) & INT_ENABLE_FREE_FALL_MASK ? 1:0);
	printf("\t\t[WATERMARK] : %d\r\n", adxl_dev_get_int_enable(ptr->dev) & INT_ENABLE_WATERMARK_MASK ? 1:0);
	printf("\t\t[OVERRUN] : %d\r\n", adxl_dev_get_int_enable(ptr->dev) & INT_ENABLE_OVERRUN_MASK ? 1:0);
	printf("\t[INT_MAP] : 0x%02x\r\n", adxl_dev_get_int_map(ptr->dev));
	printf("\t\t[DATA_READY] : %d\r\n", adxl_dev_get_int_map(ptr->dev) & INT_MAP_DATA_READY_MASK ? 1:0);
	printf("\t\t[SINGLE_TAP] : %d\r\n", adxl_dev_get_int_map(ptr->dev) & INT_MAP_SINGLE_TAP_MASK ? 1:0);
	printf("\t\t[DOUBLE_TAP] : %d\r\n", adxl_dev_get_int_map(ptr->dev) & INT_MAP_DOUBLE_TAP_MASK ? 1:0);
	printf("\t\t[ACTIVITY] : %d\r\n", adxl_dev_get_int_map(ptr->dev) & INT_MAP_ACTIVITY_MASK ? 1:0);
	printf("\t\t[INACTIVITY] : %d\r\n", adxl_dev_get_int_map(ptr->dev) & INT_MAP_INACTIVITY_MASK ? 1:0);
	printf("\t\t[FREEFALL] : %d\r\n", adxl_dev_get_int_map(ptr->dev) & INT_MAP_FREE_FALL_MASK ? 1:0);
	printf("\t\t[WATERMARK] : %d\r\n", adxl_dev_get_int_map(ptr->dev) & INT_MAP_WATERMARK_MASK ? 1:0);
	printf("\t\t[OVERRUN] : %d\r\n", adxl_dev_get_int_map(ptr->dev) & INT_MAP_OVERRUN_MASK ? 1:0);
	printf("\t[INT_SOURCE] : 0x%02x\r\n", adxl_dev_get_int_source(ptr->dev));
	printf("\t\t[DATA_READY] : %d\r\n", adxl_dev_get_int_source(ptr->dev) & INT_SOURCE_DATA_READY_MASK ? 1:0);
	printf("\t\t[SINGLE_TAP] : %d\r\n", adxl_dev_get_int_source(ptr->dev) & INT_SOURCE_SINGLE_TAP_MASK ? 1:0);
	printf("\t\t[DOUBLE_TAP] : %d\r\n", adxl_dev_get_int_source(ptr->dev) & INT_SOURCE_DOUBLE_TAP_MASK ? 1:0);
	printf("\t\t[ACTIVITY] : %d\r\n", adxl_dev_get_int_source(ptr->dev) & INT_SOURCE_ACTIVITY_MASK ? 1:0);
	printf("\t\t[INACTIVITY] : %d\r\n", adxl_dev_get_int_source(ptr->dev) & INT_SOURCE_INACTIVITY_MASK ? 1:0);
	printf("\t\t[FREEFALL] : %d\r\n", adxl_dev_get_int_source(ptr->dev) & INT_SOURCE_FREE_FALL_MASK ? 1:0);
	printf("\t\t[WATERMARK] : %d\r\n", adxl_dev_get_int_source(ptr->dev) & INT_SOURCE_WATERMARK_MASK ? 1:0);
	printf("\t\t[OVERRUN] : %d\r\n", adxl_dev_get_int_source(ptr->dev) & INT_SOURCE_OVERRUN_MASK ? 1:0);
	printf("\t[DATA_FORMAT] : 0x%02x\r\n", adxl_dev_get_data_format(ptr->dev));
	printf("\t\t[SELFTEST] : %d\r\n", adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_SELFTEST_MASK ? 1:0);
	printf("\t\t[SPI] : %d\r\n", adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_SPI_MASK ? 1:0);
	printf("\t\t[INT_INVERT] : %d\r\n", adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_INT_INVERT_MASK ? 1:0);
	printf("\t\t[0] : %d\r\n", adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_0_MASK ? 1:0);
	printf("\t\t[FULL_RES] : %d\r\n", adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_FULL_RES_MASK ? 1:0);
	printf("\t\t[JUSTIFY] : %d\r\n", adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_JUSTIFY_MASK ? 1:0);
	printf("\t\t[RANGE] : %d\r\n", adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_RANGE_MASK);
	printf("\t\t\t[RANGE] : ");
	switch( adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_RANGE_MASK ){
		case DATA_FORMAT_RANGE_2G:
			printf("-2g..+2g");
		break;

		case DATA_FORMAT_RANGE_4G:
			printf("-4g..+4g");
		break;

		case DATA_FORMAT_RANGE_8G:
			printf("-8g..+8g");
		break;

		case DATA_FORMAT_RANGE_16G:
			printf("-16g..+16g");
		break;

		default:
			printf("<undefined>\r\n");
	}
	printf("\r\n");

	printf("\t[DATAX0] : 0x%02x\r\n", adxl_dev_get_datax0(ptr->dev));
	printf("\t[DATAX1] : 0x%02x\r\n", adxl_dev_get_datax1(ptr->dev));
	printf("\t[DATAY0] : 0x%02x\r\n", adxl_dev_get_datay0(ptr->dev));
	printf("\t[DATAY1] : 0x%02x\r\n", adxl_dev_get_datay1(ptr->dev));
	printf("\t[DATAZ0] : 0x%02x\r\n", adxl_dev_get_dataz0(ptr->dev));
	printf("\t[DATAZ1] : 0x%02x\r\n", adxl_dev_get_dataz1(ptr->dev));
	printf("\t[FIFO_CTL] : 0x%02x\r\n", adxl_dev_get_fifo_ctl(ptr->dev));
	printf("\t\t[FIFO_MODE] : %d\r\n", adxl_dev_get_fifo_ctl(ptr->dev) & FIFO_CTL_FIFO_MODE);
	printf("\t\t[TRIGGER] : %d\r\n", adxl_dev_get_fifo_ctl(ptr->dev) & FIFO_CTL_TRIGGER);
	printf("\t\t[SAMPLES] : %d\r\n", adxl_dev_get_fifo_ctl(ptr->dev) & FIFO_CTL_SAMPLES);
	printf("\t[FIFO_STATUS] : 0x%02x\r\n", adxl_dev_get_fifo_status(ptr->dev));
	printf("\t\t[ENTRIES] : %d\r\n", adxl_dev_get_fifo_status(ptr->dev) & FIFO_STATUS_ENTRIES_MASK);
}
