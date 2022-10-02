#include "axi_adxl.h"



int axi_adxl_dev_debug_register_space(axi_adxl* ptr) {
	int byte_cnt = 0;

	if (!axi_adxl_has_init(ptr)){
		return ADXL_UNINIT;
	}

	printf("\t|| [0] \t| [1] \t| [2] \t| [3] \t|\r\n");
	printf("========================================\r\n");

	for (int i = 0; i < 64; i++) {
		if (i % 4 == 0) {
			printf("0x%02x \t||", i);
		}
		int rw_flaq = 0;
		int ro_flaq = 0;
		int reserved_flaq = 0;

		rw_flaq = check_access_rw(i);
		ro_flaq = check_access_ro(i);
		reserved_flaq = check_access_reserved(i);

		if (rw_flaq) {
			textcolor(DEFAULT, BLACK, GREEN);
		}

		if (ro_flaq) {
			textcolor(DEFAULT, BLACK, YELLOW);
		}

		if (reserved_flaq) {
			textcolor(DIM, STD, STD);
		}


		printf(" 0x%02x\t", *((uint8_t*)(ptr->dev) + i));

		textcolor(DEFAULT, STD, STD);

		printf("|");

		if (byte_cnt == 3) {
			printf("\r\n");
			byte_cnt = 0;
		}
		else {
			byte_cnt++;
		}

	}

	return ADXL_OK;
}

/// <summary>
/// Get device parameter from internal register. 
/// in normal case this reg = 0xE5, reading register 0x00
/// </summary>
/// <param name="ptr"> Pointer AXI_ADXL instance </param>
/// <param name="device_id"> Pointer device ID which return result after completion </param>
/// <returns>
/// ADXL_UNINIT - if no axi adxl device initialized
/// ADXL_LINK_LOST - if no link over device and component
/// ADXL_OK - in other case
/// </returns>
int axi_adxl_get_device_id(axi_adxl *ptr, uint8_t *device_id){
	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_DEVID] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_DEVID] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

	*device_id = adxl_dev_get_device_id(ptr->dev);
	
	return ADXL_OK;
}


// 0x1D
int axi_adxl_set_thresh_tap(axi_adxl* ptr, uint8_t thresh_tap) {
	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_THRESH_TAP] : no init device");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_THRESH_TAP] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_SET_THRESH_TAP] : %3.4f => %3.4f ", (((float)adxl_dev_get_thresh_tap(ptr->dev)) * SCALE_THRESH_TAP), (((float)thresh_tap) * SCALE_THRESH_TAP));
#endif

	adxl_dev_set_thresh_tap(ptr->dev, thresh_tap);

	int timer = TIMER_LIMIT;
	while(adxl_dev_get_thresh_tap(ptr->dev) != thresh_tap){
		if (timer == 0){
#ifdef AXI_ADXL_LOGGING_DEV
			textcolor(DEFAULT, BLACK, RED);
			printf("failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
#endif
			return ADXL_TIMEOUT;
		}
#ifdef AXI_ADXL_LOGGING_DEV
		printf(".");
#endif
		timer--;
	}
#ifdef AXI_ADXL_LOGGING_DEV
	textcolor(DEFAULT, BLACK, GREEN);
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");
#endif

	return ADXL_OK;
}


// 0x1D
int axi_adxl_get_thresh_tap(axi_adxl* ptr, uint8_t* thresh_tap) {

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_THRESH_TAP] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_THRESH_TAP] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

	*thresh_tap = adxl_dev_get_thresh_tap(ptr->dev);
#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_GET_THRESH_TAP] : 0x%02x [%3.4f g]\r\n", *thresh_tap, (((float)*thresh_tap) * SCALE_THRESH_TAP));
#endif

	return ADXL_OK;
}


int axi_adxl_set_ofsx(axi_adxl* ptr, int8_t ofsx) {

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_OFSX] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_OFSX] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_SET_OFSX] : %3.6f => %3.6f ", ((float)adxl_dev_get_ofsx(ptr->dev) * SCALE_OFS), ((float)ofsx) * SCALE_OFS);
#endif

	adxl_dev_set_ofsx(ptr->dev, ofsx);

	int timer = TIMER_LIMIT;
	while (((int8_t)adxl_dev_get_ofsx(ptr->dev)) != ofsx) {
		if (timer == 0) {
#ifdef AXI_ADXL_LOGGING_DEV
			textcolor(DEFAULT, BLACK, RED);
			printf("failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
#endif
			return ADXL_TIMEOUT;
		}
#ifdef AXI_ADXL_LOGGING_DEV
		printf(".");
#endif
		timer--;
	}
#ifdef AXI_ADXL_LOGGING_DEV
	textcolor(DEFAULT, BLACK, GREEN);
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");
#endif

	return ADXL_OK;

}


int axi_adxl_get_ofsx(axi_adxl* ptr, int8_t* ofsx) {

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_OFSX] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_OFSX] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

	*ofsx = adxl_dev_get_ofsx(ptr->dev);

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_GET_OFSX] : %3.6f [0x%02x]\r\n", (((float)*ofsx) * SCALE_OFS), *ofsx);
#endif

	return ADXL_OK;
}


int axi_adxl_set_ofsy(axi_adxl* ptr, int8_t ofsy) {

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_OFSY] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_OFSY] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}
#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_SET_OFSY] : %3.6f to %3.6f ", ((float)adxl_dev_get_ofsy(ptr->dev) * SCALE_OFS), ((float)ofsy) * SCALE_OFS);
#endif

	adxl_dev_set_ofsy(ptr->dev, ofsy);

	int timer = TIMER_LIMIT;
	while (((int8_t)adxl_dev_get_ofsy(ptr->dev)) != ofsy) {
		if (timer == 0) {
#ifdef AXI_ADXL_LOGGING_DEV
			textcolor(DEFAULT, BLACK, RED);
			printf("failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
#endif
			return ADXL_TIMEOUT;
		}
#ifdef AXI_ADXL_LOGGING_DEV
		printf(".");
#endif
		timer--;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	textcolor(DEFAULT, BLACK, GREEN);
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");
#endif
	return ADXL_OK;

}


int axi_adxl_get_ofsy(axi_adxl* ptr, int8_t* ofsy) {

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_OFSY] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_OFSY] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

	*ofsy = adxl_dev_get_ofsy(ptr->dev);

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_GET_OFSY] : %3.6f [0x%02x]\r\n", (((float)*ofsy) * SCALE_OFS), *ofsy);
#endif

	return ADXL_OK;
}


int axi_adxl_set_ofsz(axi_adxl* ptr, int8_t ofsz) {

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_OFSZ] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_OFSZ] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_SET_OFSZ] : %3.6f to %3.6f ", ((float)adxl_dev_get_ofsz(ptr->dev) * SCALE_OFS), ((float)ofsz) * SCALE_OFS);
#endif

	adxl_dev_set_ofsz(ptr->dev, ofsz);

	int timer = TIMER_LIMIT;
	while (((int8_t)adxl_dev_get_ofsz(ptr->dev)) != ofsz) {
		if (timer == 0) {
#ifdef AXI_ADXL_LOGGING_DEV
			textcolor(DEFAULT, BLACK, RED);
			printf("failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
#endif
			return ADXL_TIMEOUT;
		}
#ifdef AXI_ADXL_LOGGING_DEV
		printf(".");
#endif
		timer--;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	textcolor(DEFAULT, BLACK, GREEN);
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");

#endif
	return ADXL_OK;
}


int axi_adxl_get_ofsz(axi_adxl* ptr, int8_t* ofsz) {

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_OFSZ] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_OFSZ] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

	*ofsz = adxl_dev_get_ofsz(ptr->dev);

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_GET_OFSZ] : %3.6f [0x%02x]\r\n", (((float)*ofsz) * SCALE_OFS), *ofsz);
#endif

	return ADXL_OK;
}



int axi_adxl_set_dur(axi_adxl* ptr, uint8_t duration) {

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_DRTN] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_DRTN] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_SET_DRTN] : %3.6f sec => %3.6f sec ", (((float)adxl_dev_get_dur(ptr->dev)) * SCALE_DUR), (((float)duration) * SCALE_DUR));
#endif

	adxl_dev_set_dur(ptr->dev, duration);

	int timer = TIMER_LIMIT;
	while (adxl_dev_get_dur(ptr->dev) != duration) {
		if (timer == 0) {
#ifdef AXI_ADXL_LOGGING_DEV
			textcolor(DEFAULT, BLACK, RED);
			printf("failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
#endif
			return ADXL_TIMEOUT;
		}
#ifdef AXI_ADXL_LOGGING_DEV
		printf(".");
#endif
		timer--;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	textcolor(DEFAULT, BLACK, GREEN);
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");
#endif
	return ADXL_OK;
}


int axi_adxl_get_dur(axi_adxl* ptr, uint8_t* duration) {

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_DRTN] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_DRTN] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

	*duration = adxl_dev_get_dur(ptr->dev);

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_GET_DUR] : %3.6f [0x%02x]\r\n", (((float)*duration) * SCALE_DUR), *duration);
#endif

	return ADXL_OK;
}


int axi_adxl_set_latent(axi_adxl* ptr, uint8_t latent) {
	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_LAT] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_LAT] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_SET_LAT] : %3.6f sec => %3.6f sec ", (((float)adxl_dev_get_latent(ptr->dev)) * SCALE_LATENT), (((float)latent) * SCALE_LATENT));
#endif

	adxl_dev_set_latent(ptr->dev, latent);

	int timer = TIMER_LIMIT;

	while (adxl_dev_get_latent(ptr->dev) != latent) {
		if (timer == 0) {
#ifdef AXI_ADXL_LOGGING_DEV
			textcolor(DEFAULT, BLACK, RED);
			printf("failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
#endif
			return ADXL_TIMEOUT;
		}
#ifdef AXI_ADXL_LOGGING_DEV
		printf(".");
#endif
		timer--;
	}
#ifdef AXI_ADXL_LOGGING_DEV
	textcolor(DEFAULT, BLACK, GREEN);
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");
#endif


	return ADXL_OK;
}


int axi_adxl_get_latent(axi_adxl* ptr, uint8_t *latent) {
	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_LAT] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_LAT] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

	*latent = adxl_dev_get_latent(ptr->dev);

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_GET_LAT] : %d[%3.6f sec]\r\n", *latent, (((float)*latent) * SCALE_LATENT));
#endif

	return ADXL_OK;

}


int axi_adxl_set_window(axi_adxl* ptr, uint8_t window) {
	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_WND] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_WND] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_SET_WND] : %3.6f sec => %3.6f sec ", (((float)adxl_dev_get_window(ptr->dev)) * SCALE_WINDOW), (((float)window) * SCALE_WINDOW));
#endif

	adxl_dev_set_window(ptr->dev, window);

	int timer = TIMER_LIMIT;

	while (adxl_dev_get_window(ptr->dev) != window) {
		if (timer == 0) {
#ifdef AXI_ADXL_LOGGING_DEV
			textcolor(DEFAULT, BLACK, RED);
			printf("failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
#endif
			return ADXL_TIMEOUT;
		}
#ifdef AXI_ADXL_LOGGING_DEV
		printf(".");
#endif
		timer--;
	}
#ifdef AXI_ADXL_LOGGING_DEV
	textcolor(DEFAULT, BLACK, GREEN);
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");
#endif
	return ADXL_OK;

}


int axi_adxl_get_window(axi_adxl* ptr, uint8_t *window) {
	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_WND] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_WND] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

	*window = adxl_dev_get_window(ptr->dev);

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_GET_WND] : 0x%02x[%3.6f sec] \r\n", *window, (((float)*window) * SCALE_WINDOW));
#endif

	return ADXL_OK;
}


int axi_adxl_set_thresh_act(axi_adxl* ptr, uint8_t thresh_act) {
	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_THRESH_ACT] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_THRESH_ACT] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_SET_THRESH_ACT] : %3.6f g => %3.6f g ", (((float)adxl_dev_get_thresh_act(ptr->dev)) * SCALE_THRESH_ACT), (((float)thresh_act) * SCALE_THRESH_ACT));
#endif

	adxl_dev_set_thresh_act(ptr->dev, thresh_act);

	int timer = TIMER_LIMIT;

	while (adxl_dev_get_thresh_act(ptr->dev) != thresh_act) {
		if (timer == 0) {
#ifdef AXI_ADXL_LOGGING_DEV
			textcolor(DEFAULT, BLACK, RED);
			printf("failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
#endif
			return ADXL_TIMEOUT;
		}
#ifdef AXI_ADXL_LOGGING_DEV
		printf(".");
#endif
		timer--;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	textcolor(DEFAULT, BLACK, GREEN);
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");
#endif

	return ADXL_OK;

}


int axi_adxl_get_thresh_act(axi_adxl* ptr, uint8_t *thresh_act) {
	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_THRESH_ACT] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_THRESH_ACT] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

	*thresh_act = adxl_dev_get_thresh_act(ptr->dev);

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_GET_THRESH_ACT] : 0x%02x[%3.6f g]\r\n", *thresh_act, (((float)*thresh_act) * SCALE_THRESH_ACT));
#endif

	return ADXL_OK;

}


int axi_adxl_set_thresh_inact(axi_adxl* ptr, uint8_t thresh_inact) {
	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_THRESH_INACT] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_THRESH_INACT] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_SET_THRESH_INACT] : %3.6f g => %3.6f g ", (((float)adxl_dev_get_thresh_inact(ptr->dev)) * SCALE_THRESH_INACT), (((float)thresh_inact) * SCALE_THRESH_INACT));
#endif

	adxl_dev_set_thresh_inact(ptr->dev, thresh_inact);

	int timer = TIMER_LIMIT;

	while (adxl_dev_get_thresh_inact(ptr->dev) != thresh_inact) {
		if (timer == 0) {
#ifdef AXI_ADXL_LOGGING_DEV
			textcolor(DEFAULT, BLACK, RED);
			printf("failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
#endif
			return ADXL_TIMEOUT;
		}
#ifdef AXI_ADXL_LOGGING_DEV
		printf(".");
#endif
		timer--;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	textcolor(DEFAULT, BLACK, GREEN);
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");
#endif

	return ADXL_OK;

}


int axi_adxl_get_thresh_inact(axi_adxl* ptr, uint8_t *thresh_inact) {
	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_THRESH_INACT] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_THRESH_INACT] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

	*thresh_inact = adxl_dev_get_thresh_inact(ptr->dev);

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_GET_THRESH_INACT] : 0x%02x[%3.6f g]\r\n", *thresh_inact, (((float)*thresh_inact) * SCALE_THRESH_INACT));
#endif

	return ADXL_OK;

}


int axi_adxl_set_time_inact(axi_adxl* ptr, uint8_t time_inact) {
	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_TIME_INACT] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_TIME_INACT] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_SET_TIME_INACT] : %3f sec => %3f sec ", (((float)adxl_dev_get_time_inact(ptr->dev)) * SCALE_TIME_INACT), (((float)time_inact) * SCALE_TIME_INACT));
#endif

	int timer = TIMER_LIMIT;

	adxl_dev_set_time_inact(ptr->dev, time_inact);
	while (adxl_dev_get_time_inact(ptr->dev) != time_inact) {
		if (timer == 0) {
#ifdef AXI_ADXL_LOGGING_DEV
			textcolor(DEFAULT, BLACK, RED);
			printf("failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
#endif
			return ADXL_TIMEOUT;
		}
#ifdef AXI_ADXL_LOGGING_DEV
		printf(".");
#endif
		timer--;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	textcolor(DEFAULT, BLACK, GREEN);
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");
#endif

	return ADXL_OK;

}


int axi_adxl_get_time_inact(axi_adxl* ptr, uint8_t *time_inact) {
	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_TIME_INACT] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_TIME_INACT] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}
	
	*time_inact = adxl_dev_get_time_inact(ptr->dev);

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_GET_TIME_INACT] : 0x%02x[%3f s]\r\n", *time_inact, (((float)*time_inact) * SCALE_TIME_INACT));
#endif
	
	return ADXL_OK;
}


int axi_adxl_set_activity_acdc(axi_adxl* ptr, enum acdc_enum acdc_mode) {

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_ACT_ACDC] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_ACT_ACDC] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_SET_ACT_ACDC] : selected ");
#endif

	switch (acdc_mode) {
		case DC_MODE:
#ifdef AXI_ADXL_LOGGING_DEV
			printf("DC mode ");
#endif
			break;
		case AC_MODE:
#ifdef AXI_ADXL_LOGGING_DEV
			printf("AC mode ");
#endif
			break;

		default:
			return ADXL_UNCORRECT_VALUE;
	}

	adxl_dev_set_act_inact_ctl(ptr->dev, (adxl_dev_get_act_inact_ctl(ptr->dev) & ~ACT_AC_MASK) | (acdc_mode << 7) );

	int timer = TIMER_LIMIT;

	while ((adxl_dev_get_act_inact_ctl(ptr->dev) & ACT_AC_MASK)  != (acdc_mode << 7)) {
		if (timer == 0) {
#ifdef AXI_ADXL_LOGGING_DEV
			textcolor(DEFAULT, BLACK, RED);
			printf("failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
#endif
			return ADXL_TIMEOUT;
		}
#ifdef AXI_ADXL_LOGGING_DEV
		printf(".");
#endif
		timer--;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	textcolor(DEFAULT, BLACK, GREEN);
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");
#endif


	return ADXL_OK;
}


int axi_adxl_get_activity_acdc(axi_adxl* ptr, enum acdc_enum* acdc_mode) {

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_ACT_ACDC] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_ACT_ACDC] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

	*acdc_mode = (ptr->dev, (adxl_dev_get_act_inact_ctl(ptr->dev) & ACT_AC_MASK) >> 7);

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_GET_ACT_ACDC] : current_mode ");
#endif

	switch (*acdc_mode) {
	case DC_MODE:
#ifdef AXI_ADXL_LOGGING_DEV
		printf("DC mode\r\n");
#endif
		break;
	case AC_MODE:
#ifdef AXI_ADXL_LOGGING_DEV
		printf("AC mode\r\n");
#endif
		break;

	default:
		return ADXL_UNCORRECT_VALUE;
	}
	return ADXL_OK;

}


int axi_adxl_has_activity_ac(axi_adxl* ptr) {
#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_HAS_ACT_AC] : %d\r\n", (((adxl_dev_get_act_inact_ctl(ptr->dev) & ACT_AC_MASK) >> 7) == AC_MODE) ? 1 : 0);
#endif
	return (((adxl_dev_get_act_inact_ctl(ptr->dev) & ACT_AC_MASK) >> 7) == AC_MODE) ? TRUE : FALSE;
}


int axi_adxl_has_activity_dc(axi_adxl* ptr) {
#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_HAS_ACT_DC] : %d\r\n", (((adxl_dev_get_act_inact_ctl(ptr->dev) & ACT_AC_MASK) >> 7) == DC_MODE) ? 1 : 0);
#endif
	return (((adxl_dev_get_act_inact_ctl(ptr->dev) & ACT_AC_MASK) >> 7) == DC_MODE) ? TRUE : FALSE;
}


int axi_adxl_is_act_inact_control(axi_adxl* ptr, uint8_t mask) {
#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_IS_ACT_INACT] : 0x%02x : %d\r\n", mask, (adxl_dev_get_act_inact_ctl(ptr->dev) & mask) ? TRUE : FALSE);
#endif
	return (adxl_dev_get_act_inact_ctl(ptr->dev) & mask) ? TRUE : FALSE;
}


int axi_adxl_enable_activity_control(axi_adxl* ptr, enum act_enum act_mask) {

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_ENBL_ACT_CTRL] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_ENBL_ACT_CTRL] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_ENBL_ACT_CTRL] : activity coord ");
#endif

	if (act_mask & ACT_X_MASK) {
		printf("X ");
	}

	if (act_mask & ACT_Y_MASK) {
		printf("Y ");
	}

	if (act_mask & ACT_Z_MASK) {
		printf("Z ");
	}

	adxl_dev_set_act_inact_ctl(ptr->dev, (adxl_dev_get_act_inact_ctl(ptr->dev) | (act_mask & (ACT_X_MASK | ACT_Y_MASK | ACT_Z_MASK))));

	int timer = TIMER_LIMIT;

	while ((adxl_dev_get_act_inact_ctl(ptr->dev) & act_mask) != (act_mask & (ACT_X_MASK | ACT_Y_MASK | ACT_Z_MASK))) {
		if (timer == 0) {
#ifdef AXI_ADXL_LOGGING_DEV
			textcolor(DEFAULT, BLACK, RED);
			printf("failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
#endif
			return ADXL_TIMEOUT;
		}
#ifdef AXI_ADXL_LOGGING_DEV
		printf(".");
#endif
		timer--;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	textcolor(DEFAULT, BLACK, GREEN);
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");

#endif
	return ADXL_OK;
}


int axi_adxl_disable_activity_control(axi_adxl* ptr, enum act_enum act_mask) {

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_DSBL_ACT_CTRL] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_DSBL_ACT_CTRL] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_DSBL_ACT_CTRL] : activity coord [");

	if (act_mask & ACT_X_MASK) {
		printf("X ");
	}

	if (act_mask & ACT_Y_MASK) {
		printf("Y ");
	}

	if (act_mask & ACT_Z_MASK) {
		printf("Z ");
	}

	printf("] disable");
#endif

	adxl_dev_set_act_inact_ctl(ptr->dev, adxl_dev_get_act_inact_ctl(ptr->dev) & ~(act_mask & (ACT_X_MASK | ACT_Y_MASK | ACT_Z_MASK)) );

	int timer = TIMER_LIMIT;

	while ((adxl_dev_get_act_inact_ctl(ptr->dev) & (act_mask & (ACT_X_MASK | ACT_Y_MASK | ACT_Z_MASK))) != 0) {
		if (timer == 0) {
#ifdef AXI_ADXL_LOGGING_DEV
			textcolor(DEFAULT, BLACK, RED);
			printf("failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
#endif
			return ADXL_TIMEOUT;
		}
#ifdef AXI_ADXL_LOGGING_DEV
		printf(".");
#endif
		timer--;
	}
#ifdef AXI_ADXL_LOGGING_DEV
	textcolor(DEFAULT, BLACK, GREEN);
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");
#endif
	return ADXL_OK;
}


int axi_adxl_enable_inactivity_control(axi_adxl* ptr, enum inact_enum inact_mask) {

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_ENBL_INACT_CTRL] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_ENBL_INACT_CTRL] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_ENBL_INACT_CTRL] : inactivity coord [");

	if (inact_mask & INACT_X_MASK) {
		printf("X ");
	}

	if (inact_mask & INACT_Y_MASK) {
		printf("Y ");
	}

	if (inact_mask & INACT_Z_MASK) {
		printf("Z ");
	}

	printf("] enable ");
#endif

	adxl_dev_set_act_inact_ctl(ptr->dev, adxl_dev_get_act_inact_ctl(ptr->dev) | (inact_mask & (INACT_X_MASK | INACT_Y_MASK | INACT_Z_MASK)));

	int timer = TIMER_LIMIT;

	while ((adxl_dev_get_act_inact_ctl(ptr->dev) & inact_mask) != (inact_mask & (INACT_X_MASK | INACT_Y_MASK | INACT_Z_MASK))) {
		if (timer == 0) {
#ifdef AXI_ADXL_LOGGING_DEV
			textcolor(DEFAULT, BLACK, RED);
			printf("failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
#endif
			return ADXL_TIMEOUT;
		}
#ifdef AXI_ADXL_LOGGING_DEV
		printf(".");
#endif
		timer--;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	textcolor(DEFAULT, BLACK, GREEN);
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");
#endif

	return ADXL_OK;
}


int axi_adxl_disable_inactivity_control(axi_adxl* ptr, enum inact_enum inact_mask) {

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_DSBL_INACT_CTRL] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_DSBL_INACT_CTRL] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}
#ifdef AXI_ADXL_LOGGING_DEV

	printf("\t[ADXL_DSBL_INACT_CTRL] : inact coord [");

	if (inact_mask & INACT_X_MASK) {
		printf(" X");
	}

	if (inact_mask & INACT_Y_MASK) {
		printf(" Y");
	}

	if (inact_mask & INACT_Z_MASK) {
		printf(" Z");
	}

	printf("] disable ");
#endif

	adxl_dev_set_act_inact_ctl(ptr->dev, adxl_dev_get_act_inact_ctl(ptr->dev) & ~(inact_mask & (INACT_X_MASK | INACT_Y_MASK | INACT_Z_MASK)));

	int timer = TIMER_LIMIT;

	while ((adxl_dev_get_act_inact_ctl(ptr->dev) & (inact_mask & (INACT_X_MASK | INACT_Y_MASK | INACT_Z_MASK))) != 0) {
		if (timer == 0) {
#ifdef AXI_ADXL_LOGGING_DEV
			textcolor(DEFAULT, BLACK, RED);
			printf("failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
#endif
			return ADXL_TIMEOUT;
		}
#ifdef AXI_ADXL_LOGGING_DEV
		printf(".");
#endif
		timer--;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	textcolor(DEFAULT, BLACK, GREEN);
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");
#endif

	return ADXL_OK;

}


int axi_adxl_set_inactivity_acdc(axi_adxl* ptr, enum acdc_enum acdc_mode) {

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_INACT_ACDC] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_INACT_ACDC] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_SET_INACT_ACDC] : ");
#endif

	switch (acdc_mode) {
	case DC_MODE:
#ifdef AXI_ADXL_LOGGING_DEV
		printf("DC mode ");
#endif
		break;
	case AC_MODE:
#ifdef AXI_ADXL_LOGGING_DEV
		printf("AC mode ");
#endif
		break;

	default:
		return ADXL_UNCORRECT_VALUE;
	}

	adxl_dev_set_act_inact_ctl(ptr->dev, (adxl_dev_get_act_inact_ctl(ptr->dev) & ~INACT_AC_MASK) | (acdc_mode << 3));
	
	int timer = TIMER_LIMIT;

	while ((adxl_dev_get_act_inact_ctl(ptr->dev) & INACT_AC_MASK) != (acdc_mode << 3)) {
		if (timer == 0) {
#ifdef AXI_ADXL_LOGGING_DEV
			textcolor(DEFAULT, BLACK, RED);
			printf("failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
#endif
			return ADXL_TIMEOUT;
		}
#ifdef AXI_ADXL_LOGGING_DEV
		printf(".");
#endif
		timer--;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	textcolor(DEFAULT, BLACK, GREEN);
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");
#endif
	return ADXL_OK;
}


int axi_adxl_get_inactivity_acdc(axi_adxl* ptr, enum acdc_enum* acdc_mode) {

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_INACT_ACDC] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_INACT_ACDC] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

	*acdc_mode = (ptr->dev, (adxl_dev_get_act_inact_ctl(ptr->dev) & INACT_AC_MASK) >> 3);

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_GET_INACT_ACDC] : ");
#endif

	switch (*acdc_mode) {
	case DC_MODE:
#ifdef AXI_ADXL_LOGGING_DEV
		printf("DC mode\r\n");
#endif
		break;
	case AC_MODE:
#ifdef AXI_ADXL_LOGGING_DEV
		printf("AC mode\r\n");
#endif
		break;

	default:
		return ADXL_UNCORRECT_VALUE;
	}
	return ADXL_OK;

}


int axi_adxl_has_inactivity_ac(axi_adxl* ptr) {
#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_HAS_INACT_AC] : %d\r\n", (((adxl_dev_get_act_inact_ctl(ptr->dev) & INACT_AC_MASK) >> 3) == AC_MODE) ? TRUE : FALSE);
#endif
	return (((adxl_dev_get_act_inact_ctl(ptr->dev) & INACT_AC_MASK) >> 3) == AC_MODE) ? TRUE : FALSE;
}


int axi_adxl_has_inactivity_dc(axi_adxl* ptr) {
#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_HAS_INACT_DC] : %d\r\n", (((adxl_dev_get_act_inact_ctl(ptr->dev) & INACT_AC_MASK) >> 3) == DC_MODE) ? TRUE : FALSE);
#endif
	return (((adxl_dev_get_act_inact_ctl(ptr->dev) & INACT_AC_MASK) >> 3) == DC_MODE) ? TRUE : FALSE;
}


int axi_adxl_set_thresh_ff(axi_adxl* ptr, uint8_t thresh_ff) {
	
	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_THRESH_FF] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_THRESH_FF] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_SET_THRESH_FF] : %3.6f g => %3.6f g ", (((float)adxl_dev_get_thresh_ff(ptr->dev)) * SCALE_THRESH_FF), (((float)thresh_ff) * SCALE_THRESH_FF));
#endif

	adxl_dev_set_thresh_ff(ptr->dev, thresh_ff);

	int timer = TIMER_LIMIT;

	while (adxl_dev_get_thresh_ff(ptr->dev) != thresh_ff) {
		if (timer == 0) {
#ifdef AXI_ADXL_LOGGING_DEV
			textcolor(DEFAULT, BLACK, RED);
			printf("failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
#endif
			return ADXL_TIMEOUT;
		}
#ifdef AXI_ADXL_LOGGING_DEV
		printf(".");
#endif
		timer--;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	textcolor(DEFAULT, BLACK, GREEN);
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");
#endif
	return ADXL_OK;

}


int axi_adxl_get_thresh_ff(axi_adxl* ptr, uint8_t* thresh_ff) {

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_THRESH_FF] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_THRESH_FF] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}
	
	*thresh_ff = adxl_dev_get_thresh_ff(ptr->dev);
	
#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_GET_THRESH_FF] : %d [%3.6f g]\r\n", *thresh_ff, (((float)*thresh_ff) * SCALE_THRESH_FF));
#endif
	
	return ADXL_OK;
}


int axi_adxl_set_time_ff(axi_adxl* ptr, uint8_t time_ff) {

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_TIME_FF] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_TIME_FF] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_SET_TIME_FF] : %3.6f s => %3.6f s", (((float)adxl_dev_get_time_ff(ptr->dev)) * SCALE_TIME_FF), (((float)time_ff) * SCALE_TIME_FF));
#endif
	
	adxl_dev_set_time_ff(ptr->dev, time_ff);

	int timer = TIMER_LIMIT;

	while (adxl_dev_get_time_ff(ptr->dev) != time_ff) {
		if (timer == 0) {
#ifdef AXI_ADXL_LOGGING_DEV
			textcolor(DEFAULT, BLACK, RED);
			printf("failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
#endif
			return ADXL_TIMEOUT;
		}
#ifdef AXI_ADXL_LOGGING_DEV
		printf(".");
#endif
		timer--;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	textcolor(DEFAULT, BLACK, GREEN);
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");
#endif

	return ADXL_OK;

}


int axi_adxl_get_time_ff(axi_adxl* ptr, uint8_t* time_ff) {

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_TIME_FF] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_TIME_FF] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}
	*time_ff = adxl_dev_get_time_ff(ptr->dev);
#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_GET_TIME_FF] : %d [%3.6f s]\r\n", *time_ff, (((float)*time_ff) * SCALE_TIME_FF));
#endif
	return ADXL_OK;
}


int axi_adxl_is_tap_axes(axi_adxl* ptr, uint8_t mask) {
#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_IS_TAP_AXES] : 0x%02x : %d\r\n", mask, (adxl_dev_get_tap_axes(ptr->dev) & mask) ? TRUE : FALSE);
#endif
	return (adxl_dev_get_tap_axes(ptr->dev) & mask) ? TRUE : FALSE;
}


int axi_adxl_switch_tap_axes(axi_adxl* ptr, uint8_t mask) {

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SW_TAP_AXES] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SW_TAP_AXES] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_SW_TAP_AXES] : change statement of ");
#endif

	if (mask & TAP_AXES_SUPRESS_MASK) {
#ifdef AXI_ADXL_LOGGING_DEV
		printf("SUPRESS ");
#endif
	}

	if (mask & TAP_AXES_TAP_X_EN_MASK) {
#ifdef AXI_ADXL_LOGGING_DEV
		printf("X ");
#endif
	}

	if (mask & TAP_AXES_TAP_Y_EN_MASK) {
#ifdef AXI_ADXL_LOGGING_DEV
		printf("Y ");
#endif
	}

	if (mask & TAP_AXES_TAP_Z_EN_MASK) {
#ifdef AXI_ADXL_LOGGING_DEV
		printf("Z ");
#endif
	}

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\r\n");
#endif

	adxl_dev_set_tap_axes(ptr->dev, adxl_dev_get_tap_axes(ptr->dev) ^ (mask));

	return ADXL_OK;
}


int axi_adxl_is_act_src_status(axi_adxl* ptr, enum act_tap_status_enum act) {
#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_IS_ACT_STS] : 0x%02x : %d\r\n", act, (adxl_dev_get_act_tap_status(ptr->dev) & act) ? TRUE : FALSE);
#endif
	return (adxl_dev_get_act_tap_status(ptr->dev) & act) ? TRUE : FALSE;
}


int axi_adxl_has_asleep_status(axi_adxl* ptr) {
#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_HAS_ASLEEP] : %d\r\n", (adxl_dev_get_act_tap_status(ptr->dev) & ASLEEP) ? TRUE : FALSE);
#endif
	return (adxl_dev_get_act_tap_status(ptr->dev) & ASLEEP) ? TRUE : FALSE;
}


int axi_adxl_is_tap_src_status(axi_adxl* ptr, enum act_tap_status_enum tap) {
#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_IS_TAP_STS] : 0x%02x : %d\r\n", tap, (adxl_dev_get_act_tap_status(ptr->dev) & tap) ? TRUE : FALSE);
#endif
	return (adxl_dev_get_act_tap_status(ptr->dev) & tap) ? TRUE : FALSE;
}


int axi_adxl_set_bw_rate(axi_adxl* ptr, enum bw_rate_enum bw_rate) {

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_BW_RATE] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_BW_RATE] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_SET_BW_RATE] : bw rate = ");
	
	switch (bw_rate) {
		case BW_RATE_3200: printf("3200 Hz <req : %3.6f seconds> ", (float)1 / (float)3200); break;
		case BW_RATE_1600: printf("1600 Hz <req : %3.6f seconds> ", (float)1 / (float)1600); break;
		case BW_RATE_800: printf("800 Hz <req : %3.6f seconds> ", (float)1 / (float)800); break;
		case BW_RATE_400: printf("400 Hz <req : %3.6f seconds> ", (float)1 / (float)400); break;
		case BW_RATE_200: printf("200 Hz <req : %3.6f seconds> ", (float)1 / (float)200); break;
		case BW_RATE_100: printf("100 Hz <req : %3.6f seconds> ", (float)1 / (float)100); break;
		case BW_RATE_50: printf("50 Hz <req : %3.6f seconds> ", (float)1 / (float)50); break;
		case BW_RATE_25: printf("25 Hz <req : %3.6f seconds> ", (float)1 / (float)25); break;
		case BW_RATE_12_5: printf("12.5 Hz <req : %3.6f seconds> ", (float)1 / (float)12.5); break;
		case BW_RATE_6_25: printf("6.25 Hz <req : %3.6f seconds> ", (float)1 / (float)6.25); break;
		case BW_RATE_3_13: printf("3.13 Hz <req : %3.6f seconds> ", (float)1 / (float)3.13); break;
		case BW_RATE_1_56: printf("1.56 Hz <req : %3.6f seconds> ", (float)1 / (float)1.56); break;
		case BW_RATE_0_78: printf("0.78 Hz <req : %3.6f seconds> ", (float)1 / (float)0.78); break;
		case BW_RATE_0_39: printf("0.39 Hz <req : %3.6f seconds> ", (float)1 / (float)0.39); break;
		case BW_RATE_0_20: printf("0.20 Hz <req : %3.6f seconds> ", (float)1 / (float)0.20); break;
		case BW_RATE_0_10: printf("0.10 Hz <req : %3.6f seconds> ", (float)1 / (float)0.10); break;
		case BW_RATE_400_LP: printf("400 Hz <req : %3.6f seconds> [low_power] ", (float)1 / (float)400); break;
		case BW_RATE_200_LP: printf("200 Hz <req : %3.6f seconds> [low_power] ", (float)1 / (float)200); break;
		case BW_RATE_100_LP: printf("100 Hz <req : %3.6f seconds> [low_power] ", (float)1 / (float)100); break;
		case BW_RATE_50_LP: printf("50 Hz <req : %3.6f seconds> [low_power] ", (float)1 / (float)50); break;
		case BW_RATE_25_LP: printf("25 Hz <req : %3.6f seconds> [low_power] ", (float)1 / (float)25); break;
		case BW_RATE_12_5_LP: printf("12.5 Hz <req : %3.6f seconds> [low_power] ", (float)1 / (float)12.5); break;
		default: printf("<undefined value>\r\n"); return ADXL_UNCORRECT_VALUE; break;
	}
#endif

	adxl_dev_set_bw_rate(ptr->dev, (bw_rate & (BW_RATE_LOW_POWER_MASK | BW_RATE_RATE_MASK)));

	int timer = TIMER_LIMIT;

	while ((adxl_dev_get_bw_rate(ptr->dev) & (BW_RATE_LOW_POWER_MASK | BW_RATE_RATE_MASK)) != bw_rate) {
		if (timer == 0) {
#ifdef AXI_ADXL_LOGGING_DEV
			textcolor(DEFAULT, BLACK, RED);
			printf("failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
#endif
			return ADXL_TIMEOUT;
		}
#ifdef AXI_ADXL_LOGGING_DEV
		printf(".");
#endif
		timer--;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	textcolor(DEFAULT, BLACK, GREEN);
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");
#endif

	return ADXL_OK;

}


int axi_adxl_get_bw_rate(axi_adxl *ptr, enum bw_rate_enum *bw_rate){
	
	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_BW_RATE] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_BW_RATE] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

	*bw_rate = (adxl_dev_get_bw_rate(ptr->dev) & (BW_RATE_LOW_POWER_MASK | BW_RATE_RATE_MASK));

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_GET_BW_RATE] : bandwidth = ");

#endif

#ifdef AXI_ADXL_LOGGING_DEV
	switch (*bw_rate) {
		case BW_RATE_3200: printf("3200 Hz <req : %3.6f seconds> ", (float)1 / (float)3200); break;
		case BW_RATE_1600: printf("1600 Hz <req : %3.6f seconds> ", (float)1 / (float)1600); break;
		case BW_RATE_800: printf("800 Hz <req : %3.6f seconds> ", (float)1 / (float)800); break;
		case BW_RATE_400: printf("400 Hz <req : %3.6f seconds> ", (float)1 / (float)400); break;
		case BW_RATE_200: printf("200 Hz <req : %3.6f seconds> ", (float)1 / (float)200); break;
		case BW_RATE_100: printf("100 Hz <req : %3.6f seconds> ", (float)1 / (float)100); break;
		case BW_RATE_50: printf("50 Hz <req : %3.6f seconds> ", (float)1 / (float)50); break;
		case BW_RATE_25: printf("25 Hz <req : %3.6f seconds> ", (float)1 / (float)25); break;
		case BW_RATE_12_5: printf("12.5 Hz <req : %3.6f seconds> ", (float)1 / (float)12.5); break;
		case BW_RATE_6_25: printf("6.25 Hz <req : %3.6f seconds> ", (float)1 / (float)6.25); break;
		case BW_RATE_3_13: printf("3.13 Hz <req : %3.6f seconds> ", (float)1 / (float)3.13); break;
		case BW_RATE_1_56: printf("1.56 Hz <req : %3.6f seconds> ", (float)1 / (float)1.56); break;
		case BW_RATE_0_78: printf("0.78 Hz <req : %3.6f seconds> ", (float)1 / (float)0.78); break;
		case BW_RATE_0_39: printf("0.39 Hz <req : %3.6f seconds> ", (float)1 / (float)0.39); break;
		case BW_RATE_0_20: printf("0.20 Hz <req : %3.6f seconds> ", (float)1 / (float)0.20); break;
		case BW_RATE_0_10: printf("0.10 Hz <req : %3.6f seconds> ", (float)1 / (float)0.10); break;
		case BW_RATE_400_LP: printf("400 Hz <req : %3.6f seconds> [low_power] ", (float)1 / (float)400); break;
		case BW_RATE_200_LP: printf("200 Hz <req : %3.6f seconds> [low_power] ", (float)1 / (float)200); break;
		case BW_RATE_100_LP: printf("100 Hz <req : %3.6f seconds> [low_power] ", (float)1 / (float)100); break;
		case BW_RATE_50_LP: printf("50 Hz <req : %3.6f seconds> [low_power] ", (float)1 / (float)50); break;
		case BW_RATE_25_LP: printf("25 Hz <req : %3.6f seconds> [low_power] ", (float)1 / (float)25); break;
		case BW_RATE_12_5_LP: printf("12.5 Hz <req : %3.6f seconds> [low_power] ", (float)1 / (float)12.5); break;
		default: printf("<undefined value>\r\n"); return ADXL_UNCORRECT_VALUE; break;
	}
	printf("\r\n");
#endif
	
	return ADXL_OK;
}


int axi_adxl_enable_linking_mode(axi_adxl* ptr) {

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_ENBL_LNKNG] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_ENBL_LNKNG] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_ENBL_LNKNG] : ");
#endif

	adxl_dev_set_power_ctl(ptr->dev, (adxl_dev_get_power_ctl(ptr->dev) | POWER_CTL_LINK_MASK));

	int timer = TIMER_LIMIT;

	while ((adxl_dev_get_power_ctl(ptr->dev) & POWER_CTL_LINK_MASK) != POWER_CTL_LINK_MASK) {
		if (timer == 0) {
#ifdef AXI_ADXL_LOGGING_DEV
			textcolor(DEFAULT, BLACK, RED);
			printf("failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
#endif
			return ADXL_TIMEOUT;
		}
#ifdef AXI_ADXL_LOGGING_DEV
		printf(".");
#endif
		timer--;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	textcolor(DEFAULT, BLACK, GREEN);
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");
#endif

	return ADXL_OK;
}


int axi_adxl_disable_linking_mode(axi_adxl* ptr) {

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_DSBL_LNKNG] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_DSBL_LNKNG] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_DSBL_LNKNG] : ");
#endif

	adxl_dev_set_power_ctl(ptr->dev, (adxl_dev_get_power_ctl(ptr->dev) & ~POWER_CTL_LINK_MASK));

	int timer = TIMER_LIMIT;

	while ((adxl_dev_get_power_ctl(ptr->dev) & POWER_CTL_LINK_MASK) != 0) {
		if (timer == 0) {
#ifdef AXI_ADXL_LOGGING_DEV
			textcolor(DEFAULT, BLACK, RED);
			printf("failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
#endif
			return ADXL_TIMEOUT;
		}
#ifdef AXI_ADXL_LOGGING_DEV
		printf(".");
#endif
		timer--;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	textcolor(DEFAULT, BLACK, GREEN);
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");
#endif

	return ADXL_OK;
}


int axi_adxl_has_linking_mode(axi_adxl* ptr) {
#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_HAS_LINKING] : %d\r\n", (adxl_dev_get_power_ctl(ptr->dev) & POWER_CTL_LINK_MASK) ? TRUE : FALSE);
#endif
	return (adxl_dev_get_power_ctl(ptr->dev) & POWER_CTL_LINK_MASK) ? TRUE : FALSE;
}


int axi_adxl_enable_autosleep_mode(axi_adxl* ptr) {

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_ENBL_AUTOSLP] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_ENBL_AUTOSLP] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_ENBL_AUTOSLP] : ");
#endif

	adxl_dev_set_power_ctl(ptr->dev, (adxl_dev_get_power_ctl(ptr->dev) | POWER_CTL_AUTO_SLEEP_MASK));

	int timer = TIMER_LIMIT;

	while ((adxl_dev_get_power_ctl(ptr->dev) & POWER_CTL_AUTO_SLEEP_MASK) != POWER_CTL_AUTO_SLEEP_MASK) {
		if (timer == 0) {
#ifdef AXI_ADXL_LOGGING_DEV
			textcolor(DEFAULT, BLACK, RED);
			printf("failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
#endif
			return ADXL_TIMEOUT;
		}
#ifdef AXI_ADXL_LOGGING_DEV
		printf(".");
#endif
		timer--;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	textcolor(DEFAULT, BLACK, GREEN);
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");
#endif

	return ADXL_OK;
}


int axi_adxl_disable_autosleep_mode(axi_adxl* ptr) {

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_DSBL_AUTOSLP] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_DSBL_AUTOSLP] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_DSBL_AUTOSLP] : ");
#endif
	adxl_dev_set_power_ctl(ptr->dev, (adxl_dev_get_power_ctl(ptr->dev) & ~POWER_CTL_AUTO_SLEEP_MASK));

	int timer = TIMER_LIMIT;

	while ((adxl_dev_get_power_ctl(ptr->dev) & POWER_CTL_AUTO_SLEEP_MASK) != 0) {
		if (timer == 0) {
#ifdef AXI_ADXL_LOGGING_DEV
			textcolor(DEFAULT, BLACK, RED);
			printf("failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
#endif
			return ADXL_TIMEOUT;
		}
#ifdef AXI_ADXL_LOGGING_DEV
		printf(".");
#endif
		timer--;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	textcolor(DEFAULT, BLACK, GREEN);
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");
#endif

	return ADXL_OK;
}


int axi_adxl_has_autosleep_mode(axi_adxl* ptr) {
#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_HAS_AUTOSLP] : %d\r\n", (adxl_dev_get_power_ctl(ptr->dev) & POWER_CTL_AUTO_SLEEP_MASK) ? TRUE : FALSE);
#endif
	return (adxl_dev_get_power_ctl(ptr->dev) & POWER_CTL_AUTO_SLEEP_MASK) ? TRUE : FALSE;
}


int axi_adxl_enable_msmt(axi_adxl* ptr) {
	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_ENBL_MSMT] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_ENBL_MSMT] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

	adxl_dev_set_power_ctl(ptr->dev, adxl_dev_get_power_ctl(ptr->dev) | POWER_CTL_MEASURE_MASK);

	int timer = TIMER_LIMIT;

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_ENBL_MSMT] : ");
#endif
	while (!(adxl_dev_get_power_ctl(ptr->dev) & POWER_CTL_MEASURE_MASK)) {
		if (timer == 0) {
#ifdef AXI_ADXL_LOGGING_DEV
			textcolor(DEFAULT, BLACK, RED);
			printf("failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
#endif
			return ADXL_TIMEOUT;
		}
#ifdef AXI_ADXL_LOGGING_DEV
		printf(".");
#endif
		timer--;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	textcolor(DEFAULT, BLACK, GREEN);
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");
#endif

	return ADXL_OK;

}


int axi_adxl_disable_msmt(axi_adxl* ptr) {

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_DSBL_MSMT] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_DSBL_MSMT] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

	adxl_dev_set_power_ctl(ptr->dev, adxl_dev_get_power_ctl(ptr->dev) & ~POWER_CTL_MEASURE_MASK);

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_DSBL_MSMT] : ");
#endif

	int timer = TIMER_LIMIT;

	while ((adxl_dev_get_power_ctl(ptr->dev) & POWER_CTL_MEASURE_MASK)) {
		if (timer == 0) {
#ifdef AXI_ADXL_LOGGING_DEV
			textcolor(DEFAULT, BLACK, RED);
			printf("failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
#endif
			return ADXL_TIMEOUT;
		}
#ifdef AXI_ADXL_LOGGING_DEV
		printf(".");
#endif
		timer--;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	textcolor(DEFAULT, BLACK, GREEN);
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");
#endif

	return ADXL_OK;

}


int axi_adxl_has_msmt(axi_adxl* ptr) {
#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_HAS_MSMT] : %d\r\n", (adxl_dev_get_power_ctl(ptr->dev) & POWER_CTL_MEASURE_MASK) ? TRUE : FALSE);
#endif
	return (adxl_dev_get_power_ctl(ptr->dev) & POWER_CTL_MEASURE_MASK) ? TRUE : FALSE;
}


int axi_adxl_enable_sleep_mode(axi_adxl* ptr) {

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_ENBL_SLP] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_ENBL_SLP] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_ENBL_SLP] : ");
#endif
	
	adxl_dev_set_power_ctl(ptr->dev, (adxl_dev_get_power_ctl(ptr->dev) | POWER_CTL_SLEEP_MASK));

	int timer = TIMER_LIMIT;

	while ((adxl_dev_get_power_ctl(ptr->dev) & POWER_CTL_SLEEP_MASK) != POWER_CTL_SLEEP_MASK) {
		if (timer == 0) {
#ifdef AXI_ADXL_LOGGING_DEV
			textcolor(DEFAULT, BLACK, RED);
			printf("failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
#endif
			return ADXL_TIMEOUT;
		}
#ifdef AXI_ADXL_LOGGING_DEV
		printf(".");
#endif
		timer--;
	}
#ifdef AXI_ADXL_LOGGING_DEV
	textcolor(DEFAULT, BLACK, GREEN);
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");

#endif

	return ADXL_OK;
}


int axi_adxl_disable_sleep_mode(axi_adxl* ptr) {

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_DSBL_SLP] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_DSBL_SLP] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}
#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_DSBL_SLP] : ");
#endif

	adxl_dev_set_power_ctl(ptr->dev, (adxl_dev_get_power_ctl(ptr->dev) & ~POWER_CTL_SLEEP_MASK));

	int timer = TIMER_LIMIT;

	while ((adxl_dev_get_power_ctl(ptr->dev) & POWER_CTL_SLEEP_MASK) != 0) {
		if (timer == 0) {
#ifdef AXI_ADXL_LOGGING_DEV
			textcolor(DEFAULT, BLACK, RED);
			printf("failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
#endif
			return ADXL_TIMEOUT;
		}
#ifdef AXI_ADXL_LOGGING_DEV
		printf(".");
#endif
		timer--;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	textcolor(DEFAULT, BLACK, GREEN);
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");
#endif

	return ADXL_OK;
}


int axi_adxl_has_sleep_mode(axi_adxl* ptr) {
#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_HAS_SLEEP] : %d\r\n", (adxl_dev_get_power_ctl(ptr->dev) & POWER_CTL_SLEEP_MASK) ? TRUE : FALSE);
#endif
	return (adxl_dev_get_power_ctl(ptr->dev) & POWER_CTL_SLEEP_MASK) ? TRUE : FALSE;
}


int axi_adxl_set_wakeup(axi_adxl* ptr, enum wakeup_enum wakeup) {

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_WKP] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_WKP] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_SET_WKP] : changing wakeup mode from ");
#endif

	switch (adxl_dev_get_power_ctl(ptr->dev) & POWER_CTL_WAKEUP_MASK) {
		case WAKEUP_8HZ: 
#ifdef AXI_ADXL_LOGGING_DEV
			printf("8 Hz "); 
#endif
			break;
		case WAKEUP_4HZ: 
#ifdef AXI_ADXL_LOGGING_DEV
			printf("4 Hz "); 
#endif
			break;
		case WAKEUP_2HZ: 
#ifdef AXI_ADXL_LOGGING_DEV
			printf("2 Hz "); 
#endif
			break;
		case WAKEUP_1HZ: 
#ifdef AXI_ADXL_LOGGING_DEV
			printf("1 Hz "); 
#endif
			break;
		default: 
#ifdef AXI_ADXL_LOGGING_DEV
			printf("<undefined> "); 
#endif
			return ADXL_UNCORRECT_VALUE; 
			break;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	printf("to ");
#endif

	switch (wakeup) {
		case WAKEUP_8HZ: 
#ifdef AXI_ADXL_LOGGING_DEV
			printf("8 Hz "); 
#endif
			break;
		case WAKEUP_4HZ: 
#ifdef AXI_ADXL_LOGGING_DEV
			printf("4 Hz "); 
#endif
			break;
		case WAKEUP_2HZ: 
#ifdef AXI_ADXL_LOGGING_DEV
			printf("2 Hz "); 
#endif
			break;
		case WAKEUP_1HZ: 
#ifdef AXI_ADXL_LOGGING_DEV
			printf("1 Hz "); 
#endif
			break;
		default: 
#ifdef AXI_ADXL_LOGGING_DEV
			printf("<undefined> "); 
#endif
			return ADXL_UNCORRECT_VALUE; 
			break;
	}

	adxl_dev_set_power_ctl(ptr->dev, (adxl_dev_get_power_ctl(ptr->dev) & ~POWER_CTL_WAKEUP_MASK) | (wakeup & POWER_CTL_WAKEUP_MASK));

	int timer = TIMER_LIMIT;

	while ((adxl_dev_get_power_ctl(ptr->dev) & POWER_CTL_WAKEUP_MASK) != (wakeup & POWER_CTL_WAKEUP_MASK)) {
		if (timer == 0) {
#ifdef AXI_ADXL_LOGGING_DEV
			textcolor(DEFAULT, BLACK, RED);
			printf("failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
#endif
			return ADXL_TIMEOUT;
		}
#ifdef AXI_ADXL_LOGGING_DEV
		printf(".");
#endif
		timer--;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	textcolor(DEFAULT, BLACK, GREEN);
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");
#endif

	return ADXL_OK;
}


int axi_adxl_get_wakeup(axi_adxl* ptr, enum wakeup_enum *wakeup) {
	
	int status = ADXL_OK;

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_WKP] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_WKP] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

	*wakeup = (adxl_dev_get_power_ctl(ptr->dev) & POWER_CTL_WAKEUP_MASK);
	
#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_GET_WKP] : wakeup = ");
#endif

	switch (*wakeup) {
		case WAKEUP_8HZ: 
#ifdef AXI_ADXL_LOGGING_DEV
			printf("8 Hz "); 
#endif
			break;
		case WAKEUP_4HZ: 
#ifdef AXI_ADXL_LOGGING_DEV
			printf("4 Hz "); 
#endif
			break;
		case WAKEUP_2HZ: 
#ifdef AXI_ADXL_LOGGING_DEV
			printf("2 Hz "); 
#endif
			break;
		case WAKEUP_1HZ: 
#ifdef AXI_ADXL_LOGGING_DEV
			printf("1 Hz "); 
#endif
			break;
		default: 
#ifdef AXI_ADXL_LOGGING_DEV
			printf("<undefined> "); 
#endif
			status = ADXL_UNCORRECT_VALUE; 
			break;
	}
#ifdef AXI_ADXL_LOGGING_DEV
	printf("\r\n");
#endif
	return status;
}


int axi_adxl_is_wakeup(axi_adxl* ptr, enum wakeup_enum wakeup) {
#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_IS_WKP] : 0x%02x : %d\r\n", wakeup, ((adxl_dev_get_power_ctl(ptr->dev) & POWER_CTL_WAKEUP_MASK) == wakeup) ? TRUE : FALSE);
#endif
	return ((adxl_dev_get_power_ctl(ptr->dev) & POWER_CTL_WAKEUP_MASK) == wakeup) ? TRUE : FALSE;
}


int axi_adxl_enable_int_enable(axi_adxl* ptr, enum int_mask_enum intr){

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_ENBL_INT_EN] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_ENBL_INT_EN] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_ENBL_INT_EN] : ");
#endif

	adxl_dev_set_int_enable(ptr->dev, (adxl_dev_get_int_enable(ptr->dev) | intr));
	
	int timer = TIMER_LIMIT;

	while (!(adxl_dev_get_int_enable(ptr->dev) & intr)) {
		if (timer == 0) {
#ifdef AXI_ADXL_LOGGING_DEV
			textcolor(DEFAULT, BLACK, RED);
			printf("failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
#endif
			return ADXL_TIMEOUT;
		}
#ifdef AXI_ADXL_LOGGING_DEV
		printf(".");
#endif
		timer--;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	textcolor(DEFAULT, BLACK, GREEN);
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");
#endif
	return ADXL_OK;
}


int axi_adxl_disable_int_enable(axi_adxl* ptr, enum int_mask_enum intr){

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_DSBL_INT_EN] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_DISABLE_INT_EN] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}
	
#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_DISABLE_INT_EN] : ");
#endif

	adxl_dev_set_int_enable(ptr->dev, (adxl_dev_get_int_enable(ptr->dev) & ~intr));

	int timer = TIMER_LIMIT;

	while ((adxl_dev_get_int_enable(ptr->dev) & intr) != 0) {
		if (timer == 0) {
#ifdef AXI_ADXL_LOGGING_DEV
			textcolor(DEFAULT, BLACK, RED);
			printf("failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
#endif
			return ADXL_TIMEOUT;
		}
#ifdef AXI_ADXL_LOGGING_DEV
		printf(".");
#endif
		timer--;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	textcolor(DEFAULT, BLACK, GREEN);
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");
#endif

	return ADXL_OK;
}


int axi_adxl_is_int_enable(axi_adxl* ptr, enum int_mask_enum intr) {
#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_IS_INT_EN] : 0x%02x : %d\r\n", intr, (adxl_dev_get_int_enable(ptr->dev) & intr) ? TRUE : FALSE);
#endif
	return (adxl_dev_get_int_enable(ptr->dev) & intr) ? TRUE : FALSE;
}


int axi_adxl_set_int_map(axi_adxl* ptr, enum int_mask_enum intr_mask, enum int_map_enum int_map) {

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_INTMAP] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_INTMAP] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

	if (int_map != INT0 && int_map != INT1) {
		return ADXL_UNCORRECT_VALUE;
	}

	if (int_map == INT0) {
		adxl_dev_set_int_map(ptr->dev, (adxl_dev_get_int_map(ptr->dev) & ~intr_mask));
	}

	if (int_map == INT1) {
		adxl_dev_set_int_map(ptr->dev, (adxl_dev_get_int_map(ptr->dev) | intr_mask));
	}

	return ADXL_OK;
}


int axi_adxl_is_int_map_intr0(axi_adxl* ptr, enum int_mask_enum intr_mask) {
#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_IS_INTMAP_0] : 0x%02x : %d\r\n", intr_mask, ((adxl_dev_get_int_map(ptr->dev) & intr_mask)) ? FALSE : TRUE);
#endif
	return ((adxl_dev_get_int_map(ptr->dev) & intr_mask)) ? FALSE : TRUE;
}


int axi_adxl_is_int_map_intr1(axi_adxl* ptr, enum int_mask_enum intr_mask) {
#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_IS_INTMAP_1] : 0x%02x : %d\r\n", intr_mask, ((adxl_dev_get_int_map(ptr->dev) & intr_mask)) ? TRUE : FALSE);
#endif
	return ((adxl_dev_get_int_map(ptr->dev) & intr_mask)) ? TRUE : FALSE;
}


int axi_adxl_get_int_source(axi_adxl* ptr, uint8_t* interrupt_mask) {

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_INT_SRC] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_INT_SRC] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

	*interrupt_mask = adxl_dev_get_int_source(ptr->dev);

	return ADXL_OK;
}


int axi_adxl_is_int_source(axi_adxl* ptr, enum int_mask_enum interrupt) {
#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_IS_INT_SRC] : 0x%02x : %d\r\n", interrupt, (adxl_dev_get_int_source(ptr->dev) & interrupt) ? TRUE : FALSE);
#endif
	return (adxl_dev_get_int_source(ptr->dev) & interrupt) ? TRUE : FALSE;
}


int axi_adxl_enable_selftest(axi_adxl* ptr) {

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_ENBL_SLFTST] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_ENBL_SLFTST] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}
	
#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_ENBL_SLFTST] : ");
#endif

	adxl_dev_set_data_format(ptr->dev, (adxl_dev_get_data_format(ptr->dev) | DATA_FORMAT_SELFTEST_MASK));

	int timer = TIMER_LIMIT;

	while (!(adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_SELFTEST_MASK)) {
		if (timer == 0) {
#ifdef AXI_ADXL_LOGGING_DEV
			textcolor(DEFAULT, BLACK, RED);
			printf("failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
#endif
			return ADXL_TIMEOUT;
		}
#ifdef AXI_ADXL_LOGGING_DEV
		printf(".");
#endif
		timer--;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	textcolor(DEFAULT, BLACK, GREEN);
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");
#endif

	return ADXL_OK;

}


int axi_adxl_disable_selftest(axi_adxl* ptr) {

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_DSBL_SLFTST] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_DSBL_SLFTST] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_DSBL_SLFTST] : ");
#endif

	adxl_dev_set_data_format(ptr->dev, (adxl_dev_get_data_format(ptr->dev) & ~DATA_FORMAT_SELFTEST_MASK));

	int timer = TIMER_LIMIT;

	while ((adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_SELFTEST_MASK)) {
		if (timer == 0) {
#ifdef AXI_ADXL_LOGGING_DEV
			textcolor(DEFAULT, BLACK, RED);
			printf("failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
#endif
			return ADXL_TIMEOUT;
		}
#ifdef AXI_ADXL_LOGGING_DEV
		printf(".");
#endif
		timer--;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	textcolor(DEFAULT, BLACK, GREEN);
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");
#endif
	return ADXL_OK;

}


int axi_adxl_has_selftest(axi_adxl* ptr) {
#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_HAS_SLFTST] : %d\r\n", (adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_SELFTEST_MASK) ? TRUE : FALSE);
#endif
	return (adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_SELFTEST_MASK) ? TRUE : FALSE;
}


int axi_adxl_set_spi_mode(axi_adxl* ptr, enum spi_enum spi_mode) {

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_SPI] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_SPI] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_SET_SPI] : ");
#endif

	adxl_dev_set_data_format(ptr->dev, (adxl_dev_get_data_format(ptr->dev) & ~DATA_FORMAT_SPI_MASK) | spi_mode);

	int timer = TIMER_LIMIT;

	while ((adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_SPI_MASK) != spi_mode) {
		if (timer == 0) {
#ifdef AXI_ADXL_LOGGING_DEV
			textcolor(DEFAULT, BLACK, RED);
			printf("failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
#endif
			return ADXL_TIMEOUT;
		}
#ifdef AXI_ADXL_LOGGING_DEV
		printf(".");
#endif
		timer--;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	textcolor(DEFAULT, BLACK, GREEN);
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");
#endif

	return ADXL_OK;

}


int axi_adxl_has_spi_3_wire(axi_adxl* ptr) {
#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_HAS_SPI_3WIRE] : %d\r\n", ((adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_SPI_MASK) == SPI_3_WIRE) ? TRUE : FALSE);
#endif
	return ((adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_SPI_MASK) == SPI_3_WIRE) ? TRUE : FALSE;
}


int axi_adxl_has_spi_4_wire(axi_adxl* ptr) {
#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_HAS_SPI_4WIRE] : %d\r\n", ((adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_SPI_MASK) == SPI_4_WIRE) ? TRUE : FALSE);
#endif
	return ((adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_SPI_MASK) == SPI_4_WIRE) ? TRUE : FALSE;
}


int axi_adxl_enable_int_invert(axi_adxl* ptr) {

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_ENBL_INVRT] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_ENBL_INVRT] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_ENBL_INVRT] : ");
#endif

	adxl_dev_set_data_format(ptr->dev, (adxl_dev_get_data_format(ptr->dev) | DATA_FORMAT_INT_INVERT_MASK));

	int timer = TIMER_LIMIT;

	while (!(adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_INT_INVERT_MASK)) {
		if (timer == 0) {
#ifdef AXI_ADXL_LOGGING_DEV
			textcolor(DEFAULT, BLACK, RED);
			printf("failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
#endif
			return ADXL_TIMEOUT;
		}
#ifdef AXI_ADXL_LOGGING_DEV
		printf(".");
#endif
		timer--;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	textcolor(DEFAULT, BLACK, GREEN);
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");
#endif

	return ADXL_OK;

}


int axi_adxl_disable_int_invert(axi_adxl* ptr) {

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_DSBL_INVRT] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_DSBL_INVRT] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_DSBL_INVRT] : ");
#endif

	adxl_dev_set_data_format(ptr->dev, (adxl_dev_get_data_format(ptr->dev) & ~DATA_FORMAT_INT_INVERT_MASK));
	
	int timer = TIMER_LIMIT;

	while ((adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_INT_INVERT_MASK)) {
		if (timer == 0) {
#ifdef AXI_ADXL_LOGGING_DEV
			textcolor(DEFAULT, BLACK, RED);
			printf("failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
#endif
			return ADXL_TIMEOUT;
		}
#ifdef AXI_ADXL_LOGGING_DEV
		printf(".");
#endif
		timer--;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	textcolor(DEFAULT, BLACK, GREEN);
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");
#endif

	return ADXL_OK;

}


int axi_adxl_has_int_invert(axi_adxl* ptr) {
#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_HAS_INVRT] : %d\r\n", (adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_INT_INVERT_MASK) ? TRUE : FALSE);
#endif
	return (adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_INT_INVERT_MASK) ? TRUE : FALSE;
}


int axi_adxl_set_range(axi_adxl* ptr, enum range_enum range) {

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_RANGE] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_RANGE] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_SET_RANGE] : ");
#endif

	switch (range) {
		case RANGE_2G: 
#ifdef AXI_ADXL_LOGGING_DEV
			printf("[-2G..2G] <10 bit> Sensitivity %1.4f g ", SCALE_FACTOR_2G); 
#endif
			break;
		case RANGE_4G: 
#ifdef AXI_ADXL_LOGGING_DEV
			printf("[-4G..4G] <10 bit> Sensitivity %1.4f g ", SCALE_FACTOR_4G); 
#endif
			break;
		case RANGE_8G: 
#ifdef AXI_ADXL_LOGGING_DEV
			printf("[-8G..8G] <10 bit> Sensitivity %1.4f g ", SCALE_FACTOR_8G); 
#endif
			break;
		case RANGE_16G: 
#ifdef AXI_ADXL_LOGGING_DEV
			printf("[-16G..16G] <10 bit> Sensitivity %1.4f g ", SCALE_FACTOR_16G); 
#endif
			break;
		case RANGE_2G_FULL: 
#ifdef AXI_ADXL_LOGGING_DEV
			printf("[-2G..2G] <10 bit> Sensitivity %1.4f g ", SCALE_FACTOR_FULL_RES); 
#endif
			break;
		case RANGE_4G_FULL: 
#ifdef AXI_ADXL_LOGGING_DEV
			printf("[-4G..4G] <11 bit> Sensitivity %1.4f g ", SCALE_FACTOR_FULL_RES); 
#endif
			break;
		case RANGE_8G_FULL: 
#ifdef AXI_ADXL_LOGGING_DEV
			printf("[-8G..8G] <12 bit> Sensitivity %1.4f g ", SCALE_FACTOR_FULL_RES); 
#endif
			break;
		case RANGE_16G_FULL: 
#ifdef AXI_ADXL_LOGGING_DEV
			printf("[-16G..16G] <13 bit> Sensitivity %1.4f g ", SCALE_FACTOR_FULL_RES); 
#endif
			break;
		default: printf("\r\n"); return ADXL_UNCORRECT_VALUE; break;
	}

	adxl_dev_set_data_format(ptr->dev, (adxl_dev_get_data_format(ptr->dev) & ~(DATA_FORMAT_FULL_RES_MASK | DATA_FORMAT_RANGE_MASK)) | range);

	int timer = TIMER_LIMIT;

	while ((adxl_dev_get_data_format(ptr->dev) & (DATA_FORMAT_FULL_RES_MASK | DATA_FORMAT_RANGE_MASK)) != (range & (DATA_FORMAT_FULL_RES_MASK | DATA_FORMAT_RANGE_MASK))) {
		if (timer == 0) {
#ifdef AXI_ADXL_LOGGING_DEV
			textcolor(DEFAULT, BLACK, RED);
			printf("failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
#endif
			return ADXL_TIMEOUT;
		}
#ifdef AXI_ADXL_LOGGING_DEV
		printf(".");
#endif
		timer--;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	textcolor(DEFAULT, BLACK, GREEN);
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");
#endif
	return ADXL_OK;
}


int axi_adxl_get_range(axi_adxl* ptr, enum range_enum *range) {

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_RANGE] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_RANGE] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

	*range = (adxl_dev_get_data_format(ptr->dev) & (DATA_FORMAT_RANGE_MASK | DATA_FORMAT_FULL_RES_MASK));
	return ADXL_OK;
}


int axi_adxl_is_range(axi_adxl* ptr, enum range_enum range) {
#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_IS_RANGE] : 0x%02x : 0x%02x\r\n", range, ((adxl_dev_get_data_format(ptr->dev) & (DATA_FORMAT_RANGE_MASK | DATA_FORMAT_FULL_RES_MASK)) == range) ? TRUE : FALSE);
#endif
	return ((adxl_dev_get_data_format(ptr->dev) & (DATA_FORMAT_RANGE_MASK | DATA_FORMAT_FULL_RES_MASK)) == range) ? TRUE : FALSE;
}


int axi_adxl_set_justify_msb(axi_adxl* ptr) {

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_JSTFY_MSB] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_JSTFY_MSB] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_JSTFY_MSB] : ");
#endif

	adxl_dev_set_data_format(ptr->dev, (adxl_dev_get_data_format(ptr->dev) | DATA_FORMAT_JUSTIFY_MASK));

	int timer = TIMER_LIMIT;

	while (!(adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_JUSTIFY_MASK)) {
		if (timer == 0) {
#ifdef AXI_ADXL_LOGGING_DEV
			textcolor(DEFAULT, BLACK, RED);
			printf("failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
#endif
			return ADXL_TIMEOUT;
		}
#ifdef AXI_ADXL_LOGGING_DEV
		printf(".");
#endif
		timer--;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	textcolor(DEFAULT, BLACK, GREEN);
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");
#endif

	return ADXL_OK;

}


int axi_adxl_set_justify_lsb(axi_adxl* ptr) {

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_JSTFY_LSB] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_JSTFY_LSB] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_JSTFY_LSB] : ");
#endif

	adxl_dev_set_data_format(ptr->dev, (adxl_dev_get_data_format(ptr->dev) & ~DATA_FORMAT_JUSTIFY_MASK));

	int timer = TIMER_LIMIT;

	while ((adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_JUSTIFY_MASK)) {
		if (timer == 0) {
#ifdef AXI_ADXL_LOGGING_DEV
			textcolor(DEFAULT, BLACK, RED);
			printf("failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
#endif
			return ADXL_TIMEOUT;
		}
#ifdef AXI_ADXL_LOGGING_DEV
		printf(".");
#endif
		timer--;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	textcolor(DEFAULT, BLACK, GREEN);
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");
#endif

	return ADXL_OK;

}


int axi_adxl_has_justify_msb(axi_adxl* ptr) {
#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_HAS_JSTFY_MSB] : 0x%02x\r\n", (adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_JUSTIFY_MASK) ? TRUE : FALSE);
#endif
	return (adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_JUSTIFY_MASK) ? TRUE : FALSE;
}


int axi_adxl_has_justify_lsb(axi_adxl* ptr) {
#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_HAS_JSTFY_LSB] : 0x%02x\r\n", (adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_JUSTIFY_MASK) ? FALSE : TRUE);
#endif
	return (adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_JUSTIFY_MASK) ? FALSE : TRUE;
}


int axi_adxl_set_fifo_mode(axi_adxl* ptr, enum fifo_mode_enum fifo_mode) {

	if (ptr->init_flaq != 1) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_FIFO_MODE] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_FIFO_MODE] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_SET_FIFO_MODE] : changing FIFO mode from ");
#endif

	switch (adxl_dev_get_fifo_ctl(ptr->dev) & FIFO_CTL_FIFO_MODE) {
		case FIFO_MODE_BYPASS: 
#ifdef AXI_ADXL_LOGGING_DEV
			printf("BYPASS ");
#endif
			break;
		case FIFO_MODE_FIFO: 
#ifdef AXI_ADXL_LOGGING_DEV
			printf("FIFO ");
#endif
			break;
		case FIFO_MODE_STREAM: 
#ifdef AXI_ADXL_LOGGING_DEV
			printf("STREAM ");
#endif
			break;
		case FIFO_MODE_TRIGGER: 
#ifdef AXI_ADXL_LOGGING_DEV
			printf("TRIGGER ");
#endif
			break;
		default: 
#ifdef AXI_ADXL_LOGGING_DEV
			printf("<undefined> "); 
#endif
			return ADXL_UNCORRECT_VALUE; 
			break;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	printf("to ");
#endif

	switch (fifo_mode) {
		case FIFO_MODE_BYPASS: 
#ifdef AXI_ADXL_LOGGING_DEV
			printf("BYPASS "); 
#endif
			break;
		case FIFO_MODE_FIFO: 
#ifdef AXI_ADXL_LOGGING_DEV
			printf("FIFO "); 
#endif
			break;
		case FIFO_MODE_STREAM: 
#ifdef AXI_ADXL_LOGGING_DEV
			printf("STREAM "); 
#endif
			break;
		case FIFO_MODE_TRIGGER: 
#ifdef AXI_ADXL_LOGGING_DEV
			printf("TRIGGER "); 
#endif
			break;
		default: 
#ifdef AXI_ADXL_LOGGING_DEV
		printf("<undefined> "); 
#endif
		return ADXL_UNCORRECT_VALUE; 
		break;
	}

	adxl_dev_set_fifo_ctl(ptr->dev, (adxl_dev_get_fifo_ctl(ptr->dev) & ~FIFO_CTL_FIFO_MODE) | fifo_mode);

	int timer = TIMER_LIMIT;

	while ((adxl_dev_get_fifo_ctl(ptr->dev) & FIFO_CTL_FIFO_MODE) != fifo_mode) {
		if (timer == 0) {
#ifdef AXI_ADXL_LOGGING_DEV
			textcolor(DEFAULT, BLACK, RED);
			printf("failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
#endif
			return ADXL_TIMEOUT;
		}
#ifdef AXI_ADXL_LOGGING_DEV
		printf(".");
#endif
		timer--;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	textcolor(DEFAULT, BLACK, GREEN);
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");
#endif

	return ADXL_OK;
}


int axi_adxl_is_fifo_mode(axi_adxl* ptr, enum fifo_mode_enum fifo_mode) {
#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_IS_FIFO_MODE] : 0x%02x : %d\r\n", fifo_mode, ((adxl_dev_get_fifo_ctl(ptr->dev) & FIFO_CTL_FIFO_MODE) == fifo_mode) ? TRUE : FALSE);
#endif
	return ((adxl_dev_get_fifo_ctl(ptr->dev) & FIFO_CTL_FIFO_MODE) == fifo_mode) ? TRUE : FALSE;
}


int axi_adxl_enable_trigger(axi_adxl* ptr) {
	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_ENBL_TRGR] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_ENBL_TRGR] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_ENBL_TRGR] : ");
#endif
	
	adxl_dev_set_fifo_ctl(ptr->dev, (adxl_dev_get_fifo_ctl(ptr->dev) | FIFO_CTL_TRIGGER));
	
	int timer = TIMER_LIMIT;

	while (!(adxl_dev_get_fifo_ctl(ptr->dev) & FIFO_CTL_TRIGGER)) {
		if (timer == 0) {
#ifdef AXI_ADXL_LOGGING_DEV
			textcolor(DEFAULT, BLACK, RED);
			printf("failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
#endif
			return ADXL_TIMEOUT;
		}
#ifdef AXI_ADXL_LOGGING_DEV
		printf(".");
#endif
		timer--;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	textcolor(DEFAULT, BLACK, GREEN);
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");
#endif

	return ADXL_OK;

}


int axi_adxl_disable_trigger(axi_adxl* ptr) {
	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_DSBL_TRGR] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_DSBL_TRGR] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_DSBL_TRGR] : ");
#endif

	adxl_dev_set_fifo_ctl(ptr->dev, (adxl_dev_get_fifo_ctl(ptr->dev) & ~FIFO_CTL_TRIGGER));
	
	int timer = TIMER_LIMIT;

	while ((adxl_dev_get_fifo_ctl(ptr->dev) & FIFO_CTL_TRIGGER)) {
		if (timer == 0) {
#ifdef AXI_ADXL_LOGGING_DEV
			textcolor(DEFAULT, BLACK, RED);
			printf("failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
#endif
			return ADXL_TIMEOUT;
		}
#ifdef AXI_ADXL_LOGGING_DEV
		printf(".");
#endif
		timer--;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	textcolor(DEFAULT, BLACK, GREEN);
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");
#endif

	return ADXL_OK;

}


int axi_adxl_has_trigger(axi_adxl* ptr) {

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_HAS_TRGR] : %d\r\n", (adxl_dev_get_fifo_ctl(ptr->dev) & FIFO_CTL_TRIGGER));
#endif
	return (adxl_dev_get_fifo_ctl(ptr->dev) & FIFO_CTL_TRIGGER) ? TRUE : FALSE;

}


int axi_adxl_set_samples(axi_adxl* ptr, uint8_t samples) {

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_SMPLS] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_SMPLS] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

	if (samples > 31) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_SMPLS] : incorrect value %d", samples);
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNCORRECT_VALUE;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_SET_SMPLS] : changing value from %d samples to %d samples ", adxl_dev_get_fifo_ctl(ptr->dev) & FIFO_CTL_SAMPLES, samples);
#endif

	adxl_dev_set_fifo_ctl(ptr->dev, (adxl_dev_get_fifo_ctl(ptr->dev) & ~FIFO_CTL_SAMPLES) | (samples & FIFO_CTL_SAMPLES));

	int timer = TIMER_LIMIT;

	while ((adxl_dev_get_fifo_ctl(ptr->dev) & FIFO_CTL_SAMPLES) != (samples & FIFO_CTL_SAMPLES)) {
		if (timer == 0) {
#ifdef AXI_ADXL_LOGGING_DEV
			textcolor(DEFAULT, BLACK, RED);
			printf("failed");
			textcolor(DEFAULT, STD, STD);
			printf("\r\n");
#endif
			return ADXL_TIMEOUT;
		}
#ifdef AXI_ADXL_LOGGING_DEV
		printf(".");
#endif
		timer--;
	}

#ifdef AXI_ADXL_LOGGING_DEV
	textcolor(DEFAULT, BLACK, GREEN);
	printf("completed");
	textcolor(DEFAULT, STD, STD);
	printf("\r\n");
#endif
	return ADXL_OK;
}


int axi_adxl_get_samples(axi_adxl* ptr, uint8_t* samples) {

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_SMPLS] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_SMPLS] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

	*samples = (adxl_dev_get_fifo_ctl(ptr->dev) & FIFO_CTL_SAMPLES);

	return ADXL_OK;
}


int axi_adxl_get_fifo_sts_entries(axi_adxl* ptr, uint8_t* entries) {

	if (!axi_adxl_has_init(ptr)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_ENTRS] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_ENTRS] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

	*entries = adxl_dev_get_fifo_status(ptr->dev) & FIFO_STATUS_ENTRIES_MASK;

	return ADXL_OK;

}


int axi_adxl_has_fifo_sts_trigger(axi_adxl* ptr) {
#ifdef AXI_ADXL_LOGGING_DEV
	printf("\t\t[ADXL_HAS_FIFO_STS] : %d\r\n", (adxl_dev_get_fifo_status(ptr->dev) & FIFO_STATUS_TRIGGER_MASK) ? TRUE : FALSE);
#endif
	return (adxl_dev_get_fifo_status(ptr->dev) & FIFO_STATUS_TRIGGER_MASK) ? TRUE : FALSE;
}


/////////////////////////////////////////////////////////////////

int axi_adxl_get_data(axi_adxl* ptr, adxl_data* data) {

	if (ptr->init_flaq != 1) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_DATA] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_DATA] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_LINK_LOST;
	}

	(*data).x = ((int16_t)adxl_dev_get_datax1(ptr->dev) << 8) + (int16_t)adxl_dev_get_datax0(ptr->dev);
	(*data).y = ((int16_t)adxl_dev_get_datay1(ptr->dev) << 8) + (int16_t)adxl_dev_get_datay0(ptr->dev);
	(*data).z = ((int16_t)adxl_dev_get_dataz1(ptr->dev) << 8) + (int16_t)adxl_dev_get_dataz0(ptr->dev);

	return ADXL_OK;
}


int axi_adxl_get_data_float(axi_adxl* ptr, adxl_data_float* data_float) {

	if (ptr->init_flaq != 1) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_DATA_FLOAT] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if (!adxl_cfg_ctl_link(ptr->cfg)) {
#ifdef AXI_ADXL_LOGGING_DEV
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_DATA_FLOAT] : no link");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif

		return ADXL_LINK_LOST;
	}

	int16_t x = (int16_t)axi_adxl_get_datax(ptr);
	int16_t y = (int16_t)axi_adxl_get_datay(ptr);
	int16_t z = (int16_t)axi_adxl_get_dataz(ptr);

	if (adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_FULL_RES) {
		data_float->x = (float)x / SENSITIVITY_FULL_RES;
		data_float->y = (float)y / SENSITIVITY_FULL_RES;
		data_float->z = (float)z / SENSITIVITY_FULL_RES;

	}
	else {
		switch (adxl_dev_get_data_format(ptr->dev) & DATA_FORMAT_RANGE_MASK) {

		case DATA_FORMAT_RANGE_2G:
			data_float->x = (float)x / SENSITIVITY_2G;
			data_float->y = (float)y / SENSITIVITY_2G;
			data_float->z = (float)z / SENSITIVITY_2G;
			break;

		case DATA_FORMAT_RANGE_4G:
			data_float->x = (float)x / SENSITIVITY_4G;
			data_float->y = (float)y / SENSITIVITY_4G;
			data_float->z = (float)z / SENSITIVITY_4G;
			break;

		case DATA_FORMAT_RANGE_8G:
			data_float->x = (float)x / SENSITIVITY_8G;
			data_float->y = (float)y / SENSITIVITY_8G;
			data_float->z = (float)z / SENSITIVITY_8G;
			break;

		case DATA_FORMAT_RANGE_16G:
			data_float->x = (float)x / SENSITIVITY_16G;
			data_float->y = (float)y / SENSITIVITY_16G;
			data_float->z = (float)z / SENSITIVITY_16G;
			break;

		default:
#ifdef AXI_ADXL_LOGGING_DEV
			printf("[ADXL_GET_DATA_FLOAT] : \r\n");
#endif
			break;
		}
	}

	return ADXL_OK;
}

