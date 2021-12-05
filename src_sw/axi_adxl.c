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
	status = axi_adxl_set_full_resolution(ptr, !DATA_FORMAT_FULL_RES);

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

	adxl_cfg_reset(ptr->cfg);

	printf("[DEV_RESET] : ");

	while(adxl_cfg_has_reset(ptr->cfg)){
		printf(".");
	}
	printf("complete\r\n");

	return 0;
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

	if (!adxl_cfg_has_enable(ptr->cfg)){
		return ERR_AXICFG_DISABLE;
	}

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



void axi_adxl_debug(axi_adxl *ptr){
	printf("***** CONFIGURATION REGISTER MAP *****\r\n");
	printf("\tCONTROL : 0x%08x\r\n", ptr->cfg->ctl_reg);
	printf("\tREQ_INT : 0x%08x\r\n", ptr->cfg->request_interval_reg);
	printf("\tDW_REGR : 0x%08x\r\n", ptr->cfg->data_width_reg);
	printf("\tRVALIDR : 0x%08x\r\n", ptr->cfg->read_valid_count_reg);
	printf("\tWVALIDR : 0x%08x\r\n", ptr->cfg->write_valid_count_reg);
	printf("\tWTRANSS : 0x%08x\r\n", ptr->cfg->write_transactions_reg);
	printf("\tRTRANSS : 0x%08x\r\n", ptr->cfg->read_transactions_reg);
	printf("\tCLKPERD : 0x%08x\r\n", ptr->cfg->clk_period_reg);

	printf("\r\n***** DEVICE REGISTER MAP *****\r\n");
	printf("\tDEVICE_ID : 0x%02x\r\n", ptr->dev->device_id_reg);
	printf("\tRESERVED1 : 0x%02x\r\n", ptr->dev->reserved1_reg);
	printf("\tRESERVED2 : 0x%02x\r\n", ptr->dev->reserved2_reg);
	printf("\tRESERVED3 : 0x%02x\r\n", ptr->dev->reserved3_reg);
	printf("\tRESERVED4 : 0x%02x\r\n", ptr->dev->reserved4_reg);
	printf("\tRESERVED5 : 0x%02x\r\n", ptr->dev->reserved5_reg);
	printf("\tRESERVED6 : 0x%02x\r\n", ptr->dev->reserved6_reg);
	printf("\tRESERVED7 : 0x%02x\r\n", ptr->dev->reserved7_reg);
	printf("\tRESERVED8 : 0x%02x\r\n", ptr->dev->reserved8_reg);
	printf("\tRESERVED9 : 0x%02x\r\n", ptr->dev->reserved9_reg);
	printf("\tRESERVED10 : 0x%02x\r\n", ptr->dev->reserved10_reg);
	printf("\tRESERVED11 : 0x%02x\r\n", ptr->dev->reserved11_reg);
	printf("\tRESERVED12 : 0x%02x\r\n", ptr->dev->reserved12_reg);
	printf("\tRESERVED13 : 0x%02x\r\n", ptr->dev->reserved13_reg);
	printf("\tRESERVED14 : 0x%02x\r\n", ptr->dev->reserved14_reg);
	printf("\tRESERVED15 : 0x%02x\r\n", ptr->dev->reserved15_reg);
	printf("\tRESERVED16 : 0x%02x\r\n", ptr->dev->reserved16_reg);
	printf("\tRESERVED17 : 0x%02x\r\n", ptr->dev->reserved17_reg);
	printf("\tRESERVED18 : 0x%02x\r\n", ptr->dev->reserved18_reg);
	printf("\tRESERVED19 : 0x%02x\r\n", ptr->dev->reserved19_reg);
	printf("\tRESERVED20 : 0x%02x\r\n", ptr->dev->reserved20_reg);
	printf("\tRESERVED21 : 0x%02x\r\n", ptr->dev->reserved21_reg);
	printf("\tRESERVED22 : 0x%02x\r\n", ptr->dev->reserved22_reg);
	printf("\tRESERVED23 : 0x%02x\r\n", ptr->dev->reserved23_reg);
	printf("\tRESERVED24 : 0x%02x\r\n", ptr->dev->reserved24_reg);
	printf("\tRESERVED25 : 0x%02x\r\n", ptr->dev->reserved25_reg);
	printf("\tRESERVED26 : 0x%02x\r\n", ptr->dev->reserved26_reg);
	printf("\tRESERVED27 : 0x%02x\r\n", ptr->dev->reserved27_reg);
	printf("\tRESERVED28 : 0x%02x\r\n", ptr->dev->reserved28_reg);
	printf("\tTHRESH_TAP : 0x%02x\r\n", ptr->dev->thresh_tap_reg);
	printf("\tOFSX : 0x%02x\r\n", ptr->dev->ofsx_reg);
	printf("\tOFSY : 0x%02x\r\n", ptr->dev->ofsy_reg);
	printf("\tOFSZ : 0x%02x\r\n", ptr->dev->ofsz_reg);
	printf("\tDUR : 0x%02x\r\n", ptr->dev->dur_reg);
	printf("\tLATENT : 0x%02x\r\n", ptr->dev->latent_reg);
	printf("\tWINDOW : 0x%02x\r\n", ptr->dev->window_reg);
	printf("\tTHRESH_ACT : 0x%02x\r\n", ptr->dev->thresh_act_reg);
	printf("\tTHRESH_INACT : 0x%02x\r\n", ptr->dev->thresh_inact_reg);
	printf("\tTIME_INACT : 0x%02x\r\n", ptr->dev->time_inact_reg);
	printf("\tACT_INACT_CTL : 0x%02x\r\n", ptr->dev->act_inact_ctl_reg);
	printf("\tTHRESH_FF : 0x%02x\r\n", ptr->dev->thresh_ff_reg);
	printf("\tTIME_FF : 0x%02x\r\n", ptr->dev->time_ff_reg);
	printf("\tTAP_AXES : 0x%02x\r\n", ptr->dev->tap_axes_reg);
	printf("\tACT_TAP_STATUS : 0x%02x\r\n", ptr->dev->act_tap_status_reg);
	printf("\tBW_RATE : 0x%02x\r\n", ptr->dev->bw_rate_reg);
	printf("\tPOWER_CTL : 0x%02x\r\n", ptr->dev->power_ctl_reg);
	printf("\tINT_ENABLE : 0x%02x\r\n", ptr->dev->int_enable_reg);
	printf("\tINT_MAP : 0x%02x\r\n", ptr->dev->int_map_reg);
	printf("\tINT_SOURCE : 0x%02x\r\n", ptr->dev->int_source_reg);
	printf("\tDATA_FORMAT : 0x%02x\r\n", ptr->dev->data_format_reg);
	printf("\tDATAX0 : 0x%02x\r\n", ptr->dev->datax0_reg);
	printf("\tDATAX1 : 0x%02x\r\n", ptr->dev->datax1_reg);
	printf("\tDATAY0 : 0x%02x\r\n", ptr->dev->datay0_reg);
	printf("\tDATAY1 : 0x%02x\r\n", ptr->dev->datay1_reg);
	printf("\tDATAZ0 : 0x%02x\r\n", ptr->dev->dataz0_reg);
	printf("\tDATAZ1 : 0x%02x\r\n", ptr->dev->dataz1_reg);
	printf("\tFIFO_CTL : 0x%02x\r\n", ptr->dev->fifo_ctl_reg);
	printf("\tFIFO_STATUS : 0x%02x\r\n", ptr->dev->fifo_status_reg);
	printf("\tRESERVED58 : 0x%02x\r\n", ptr->dev->reserved58_reg);
	printf("\tRESERVED59 : 0x%02x\r\n", ptr->dev->reserved59_reg);
	printf("\tRESERVED60 : 0x%02x\r\n", ptr->dev->reserved60_reg);
	printf("\tRESERVED61 : 0x%02x\r\n", ptr->dev->reserved61_reg);
	printf("\tRESERVED62 : 0x%02x\r\n", ptr->dev->reserved62_reg);
	printf("\tRESERVED63 : 0x%02x\r\n", ptr->dev->reserved63_reg);
	printf("\r\n");

}
