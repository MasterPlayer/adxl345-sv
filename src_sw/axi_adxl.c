#include "axi_adxl.h"



int axi_adxl_init(adxl *adxl_ptr, uint32_t cfg_baseaddr, uint32_t dev_baseaddr){

	int status = axi_adxl_cfg_init(adxl_ptr, cfg_baseaddr);

	status = axi_adxl_dev_init(adxl_ptr, dev_baseaddr);

	adxl_ptr->has_init = 1;

	return status;
}



int axi_adxl_cfg_init(adxl *adxl_ptr, uint32_t baseaddr){
	adxl_ptr->cfg = baseaddr;
	return 0;
}



int axi_adxl_dev_init(adxl *adxl_ptr, uint32_t baseaddr){
	adxl_ptr->dev = baseaddr;
	return 0;
}



int axi_adxl_dev_enable(adxl *adxl_ptr, uint8_t i2c_addr, uint32_t request_interval){
	// if struct not init, return error
	if (!axi_adxl_has_init(adxl_ptr)){
		return ERR_UNINIT;
	}

	uint32_t timeout_timer = TIMEOUT_TIMER_LIMIT;
	// perform reset configuration on adxl configuration
	axi_adxl_reset(adxl_ptr->cfg);

	// checking for reset is complete
	while(axi_adxl_has_reset(adxl_ptr->cfg)){
		if (!timeout_timer){
			xil_printf("[DEV_ENABLE] : temeout reset\r\n");
			return ERR_RESET_INFINITE;
		}
		timeout_timer--;
	}

	axi_adxl_set_request_interval(adxl_ptr->cfg, request_interval);

	axi_adxl_set_i2c_addr(adxl_ptr->cfg, i2c_addr);

	axi_adxl_enable(adxl_ptr->cfg);

	timeout_timer = TIMEOUT_TIMER_LIMIT;

	while (!axi_adxl_has_enable(adxl_ptr->cfg)){
		if (!timeout_timer){
			xil_printf("[DEV_ENABLE] : timeout enable\r\n");
			return ERR_TIMEOUT_AXI_CFG;
		}
		timeout_timer--;
	}

	timeout_timer = TIMEOUT_TIMER_LIMIT;

	while (!axi_adxl_has_link(adxl_ptr->cfg)){
		if (!timeout_timer){
			xil_printf("[DEV_ENABLE] : timeout link establish\r\n");
			return ERR_LINK_LOST;
		}
		timeout_timer--;
	}

	adxl_set_power_ctl(adxl_ptr->dev, POWER_CTL_MEASURE_MASK);

	return ADXL_OK;
}


int axi_adxl_dev_disable(adxl *adxl_ptr){
	// disable iic device
	if (!axi_adxl_has_init(adxl_ptr)){
		return ERR_UNINIT;
	}
	adxl_set_power_ctl(adxl_ptr->dev, (adxl_get_power_ctl(adxl_ptr->dev) & ~POWER_CTL_MEASURE_MASK));

	uint32_t timeout_timer = TIMEOUT_TIMER_LIMIT;

	while(adxl_get_power_ctl(adxl_ptr->dev) & POWER_CTL_MEASURE_MASK){
		if (!timeout_timer){
			xil_printf("[DEV_DISABLE] : timeout dev stop msmt\r\n");
			return ERR_DEVICE_MSMT_UNSTOP;
		}
		timeout_timer--;
	}

	timeout_timer = TIMEOUT_TIMER_LIMIT;

	axi_adxl_disable(adxl_ptr->cfg);

	xil_printf("[DEV_DISABLE] : perform stop hw component");
	while(axi_adxl_has_on_work(adxl_ptr->cfg)){
		xil_printf(".");
	}
	xil_printf("Complete\r\n");

	axi_adxl_reset(adxl_ptr->cfg);

	xil_printf("[DEV_RESET] : ");

	while(axi_adxl_has_reset(adxl_ptr->cfg)){
		xil_printf(".");
	}
	xil_printf("complete\r\n");


	return 0;
}


int axi_adxl_dev_change_bw(adxl *adxl_ptr, uint8_t value){

}


int axi_adxl_dev_get_mode(adxl *adxl_ptr){

}


int axi_adxl_dev_get_axis(adxl *adxl_ptr, axis *axis_ptr){

}


int axi_adxl_dev_set_offset(adxl *adxl_ptr, offset_param offt_prm, enum mask mask_value){
	return 0;
}



void axi_adxl_debug(adxl *adxl_ptr){
	xil_printf("***** CONFIGURATION REGISTER MAP *****\r\n");
	xil_printf("\tCONTROL : 0x%08x\r\n", adxl_ptr->cfg->ctl_reg);
	xil_printf("\tREQ_INT : 0x%08x\r\n", adxl_ptr->cfg->request_interval_reg);
	xil_printf("\tDW_REGR : 0x%08x\r\n", adxl_ptr->cfg->data_width_reg);
	xil_printf("\tRVALIDR : 0x%08x\r\n", adxl_ptr->cfg->read_valid_count_reg);
	xil_printf("\tWVALIDR : 0x%08x\r\n", adxl_ptr->cfg->write_valid_count_reg);
	xil_printf("\tWTRANSS : 0x%08x\r\n", adxl_ptr->cfg->write_transactions_reg);
	xil_printf("\tRTRANSS : 0x%08x\r\n", adxl_ptr->cfg->read_transactions_reg);
	xil_printf("\tCLKPERD : 0x%08x\r\n", adxl_ptr->cfg->clk_period_reg);

	xil_printf("\r\n***** DEVICE REGISTER MAP *****\r\n");
	xil_printf("\tDEVICE_ID : 0x%02x\r\n", adxl_ptr->dev->device_id_reg);
	xil_printf("\tRESERVED1 : 0x%02x\r\n", adxl_ptr->dev->reserved1_reg);
	xil_printf("\tRESERVED2 : 0x%02x\r\n", adxl_ptr->dev->reserved2_reg);
	xil_printf("\tRESERVED3 : 0x%02x\r\n", adxl_ptr->dev->reserved3_reg);
	xil_printf("\tRESERVED4 : 0x%02x\r\n", adxl_ptr->dev->reserved4_reg);
	xil_printf("\tRESERVED5 : 0x%02x\r\n", adxl_ptr->dev->reserved5_reg);
	xil_printf("\tRESERVED6 : 0x%02x\r\n", adxl_ptr->dev->reserved6_reg);
	xil_printf("\tRESERVED7 : 0x%02x\r\n", adxl_ptr->dev->reserved7_reg);
	xil_printf("\tRESERVED8 : 0x%02x\r\n", adxl_ptr->dev->reserved8_reg);
	xil_printf("\tRESERVED9 : 0x%02x\r\n", adxl_ptr->dev->reserved9_reg);
	xil_printf("\tRESERVED10 : 0x%02x\r\n", adxl_ptr->dev->reserved10_reg);
	xil_printf("\tRESERVED11 : 0x%02x\r\n", adxl_ptr->dev->reserved11_reg);
	xil_printf("\tRESERVED12 : 0x%02x\r\n", adxl_ptr->dev->reserved12_reg);
	xil_printf("\tRESERVED13 : 0x%02x\r\n", adxl_ptr->dev->reserved13_reg);
	xil_printf("\tRESERVED14 : 0x%02x\r\n", adxl_ptr->dev->reserved14_reg);
	xil_printf("\tRESERVED15 : 0x%02x\r\n", adxl_ptr->dev->reserved15_reg);
	xil_printf("\tRESERVED16 : 0x%02x\r\n", adxl_ptr->dev->reserved16_reg);
	xil_printf("\tRESERVED17 : 0x%02x\r\n", adxl_ptr->dev->reserved17_reg);
	xil_printf("\tRESERVED18 : 0x%02x\r\n", adxl_ptr->dev->reserved18_reg);
	xil_printf("\tRESERVED19 : 0x%02x\r\n", adxl_ptr->dev->reserved19_reg);
	xil_printf("\tRESERVED20 : 0x%02x\r\n", adxl_ptr->dev->reserved20_reg);
	xil_printf("\tRESERVED21 : 0x%02x\r\n", adxl_ptr->dev->reserved21_reg);
	xil_printf("\tRESERVED22 : 0x%02x\r\n", adxl_ptr->dev->reserved22_reg);
	xil_printf("\tRESERVED23 : 0x%02x\r\n", adxl_ptr->dev->reserved23_reg);
	xil_printf("\tRESERVED24 : 0x%02x\r\n", adxl_ptr->dev->reserved24_reg);
	xil_printf("\tRESERVED25 : 0x%02x\r\n", adxl_ptr->dev->reserved25_reg);
	xil_printf("\tRESERVED26 : 0x%02x\r\n", adxl_ptr->dev->reserved26_reg);
	xil_printf("\tRESERVED27 : 0x%02x\r\n", adxl_ptr->dev->reserved27_reg);
	xil_printf("\tRESERVED28 : 0x%02x\r\n", adxl_ptr->dev->reserved28_reg);
	xil_printf("\tTHRESH_TAP : 0x%02x\r\n", adxl_ptr->dev->thresh_tap_reg);
	xil_printf("\tOFSX : 0x%02x\r\n", adxl_ptr->dev->ofsx_reg);
	xil_printf("\tOFSY : 0x%02x\r\n", adxl_ptr->dev->ofsy_reg);
	xil_printf("\tOFSZ : 0x%02x\r\n", adxl_ptr->dev->ofsz_reg);
	xil_printf("\tDUR : 0x%02x\r\n", adxl_ptr->dev->dur_reg);
	xil_printf("\tLATENT : 0x%02x\r\n", adxl_ptr->dev->latent_reg);
	xil_printf("\tWINDOW : 0x%02x\r\n", adxl_ptr->dev->window_reg);
	xil_printf("\tTHRESH_ACT : 0x%02x\r\n", adxl_ptr->dev->thresh_act_reg);
	xil_printf("\tTHRESH_INACT : 0x%02x\r\n", adxl_ptr->dev->thresh_inact_reg);
	xil_printf("\tTIME_INACT : 0x%02x\r\n", adxl_ptr->dev->time_inact_reg);
	xil_printf("\tACT_INACT_CTL : 0x%02x\r\n", adxl_ptr->dev->act_inact_ctl_reg);
	xil_printf("\tTHRESH_FF : 0x%02x\r\n", adxl_ptr->dev->thresh_ff_reg);
	xil_printf("\tTIME_FF : 0x%02x\r\n", adxl_ptr->dev->time_ff_reg);
	xil_printf("\tTAP_AXES : 0x%02x\r\n", adxl_ptr->dev->tap_axes_reg);
	xil_printf("\tACT_TAP_STATUS : 0x%02x\r\n", adxl_ptr->dev->act_tap_status_reg);
	xil_printf("\tBW_RATE : 0x%02x\r\n", adxl_ptr->dev->bw_rate_reg);
	xil_printf("\tPOWER_CTL : 0x%02x\r\n", adxl_ptr->dev->power_ctl_reg);
	xil_printf("\tINT_ENABLE : 0x%02x\r\n", adxl_ptr->dev->int_enable_reg);
	xil_printf("\tINT_MAP : 0x%02x\r\n", adxl_ptr->dev->int_map_reg);
	xil_printf("\tINT_SOURCE : 0x%02x\r\n", adxl_ptr->dev->int_source_reg);
	xil_printf("\tDATA_FORMAT : 0x%02x\r\n", adxl_ptr->dev->data_format_reg);
	xil_printf("\tDATAX0 : 0x%02x\r\n", adxl_ptr->dev->datax0_reg);
	xil_printf("\tDATAX1 : 0x%02x\r\n", adxl_ptr->dev->datax1_reg);
	xil_printf("\tDATAY0 : 0x%02x\r\n", adxl_ptr->dev->datay0_reg);
	xil_printf("\tDATAY1 : 0x%02x\r\n", adxl_ptr->dev->datay1_reg);
	xil_printf("\tDATAZ0 : 0x%02x\r\n", adxl_ptr->dev->dataz0_reg);
	xil_printf("\tDATAZ1 : 0x%02x\r\n", adxl_ptr->dev->dataz1_reg);
	xil_printf("\tFIFO_CTL : 0x%02x\r\n", adxl_ptr->dev->fifo_ctl_reg);
	xil_printf("\tFIFO_STATUS : 0x%02x\r\n", adxl_ptr->dev->fifo_status_reg);
	xil_printf("\tRESERVED58 : 0x%02x\r\n", adxl_ptr->dev->reserved58_reg);
	xil_printf("\tRESERVED59 : 0x%02x\r\n", adxl_ptr->dev->reserved59_reg);
	xil_printf("\tRESERVED60 : 0x%02x\r\n", adxl_ptr->dev->reserved60_reg);
	xil_printf("\tRESERVED61 : 0x%02x\r\n", adxl_ptr->dev->reserved61_reg);
	xil_printf("\tRESERVED62 : 0x%02x\r\n", adxl_ptr->dev->reserved62_reg);
	xil_printf("\tRESERVED63 : 0x%02x\r\n", adxl_ptr->dev->reserved63_reg);
	xil_printf("\r\n");

}
