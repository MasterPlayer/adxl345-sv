#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "axi_adxl.h"
#include <time.h>
#include <xscugic.h>

#include <xparameters.h>


int scugic_initialize(XScuGic *ptr, axi_adxl* adxl_ptr);
void adxl_intr_handler(void *callback);

void print_menu();



int menu(axi_adxl* ptr, int mode);

void selector_change_bw(axi_adxl *ptr);
void selector_change_range(axi_adxl *ptr);
void selector_calibration(axi_adxl *ptr);
void selector_pooling(axi_adxl *ptr);
void selector_enable_interrupt(axi_adxl *ptr);
void selector_disable_interrupt(axi_adxl *ptr);
void selector_set_threshold_tap(axi_adxl *ptr);
void selector_enable_tap_axes(axi_adxl *ptr);
void selector_disable_tap_axes(axi_adxl *ptr);
void selector_change_duration(axi_adxl *ptr);
void selector_change_latent(axi_adxl *ptr);
void selector_change_window(axi_adxl *ptr);
void selector_change_thresh_act(axi_adxl *ptr);
void selector_change_act_ctl(axi_adxl *ptr);
void selector_change_inact_ctl(axi_adxl *ptr);
void selector_change_thresh_inact(axi_adxl *ptr);
void selector_change_time_inact(axi_adxl *ptr);
void selector_change_thresh_ff(axi_adxl *ptr);
void selector_change_time_ff(axi_adxl *ptr);
void selector_change_fifo_samples(axi_adxl *ptr);
void selector_change_fifo_mode(axi_adxl *ptr);
void selector_change_offset_x(axi_adxl *ptr);
void selector_change_offset_y(axi_adxl *ptr);
void selector_change_offset_z(axi_adxl *ptr);


g_coord g_min;
g_coord g_max;


int main(){

    init_platform();

    axi_adxl adxl_device;
    XScuGic gic;

    scugic_initialize(&gic, &adxl_device);

    int status = axi_adxl_init(&adxl_device, 0x40030000, 0x40040000, 0x53);
    if (status != ADXL_OK){
        printf("[MAIN] : adxl initialize failed : status %d\r\n", status);
    }

    status = axi_adxl_setup_bandwidth(&adxl_device, BW_RATE_100);
    if (status != ADXL_OK){
        printf("[MAIN] : setup bandwidth failed : status %d\r\n", status);
    }

    status = axi_adxl_start_requesting(&adxl_device);
    if (status != ADXL_OK){
        printf("[MAIN] : start requesting failed : status %d\r\n", status);
    }

    status = axi_adxl_start_measure(&adxl_device, RANGE_16G_FULL);
    if (status != ADXL_OK){
        printf("[MAIN] : start measure failed : status %d\r\n", status);
    }

    status = axi_adxl_calibration(&adxl_device, 128);
    if (status != ADXL_OK){
        printf("[MAIN] : calibration failed : status %d\r\n", status);
    }

    status = axi_adxl_stop_requesting(&adxl_device);

    int mode = 0;

    char s [256];


    while(1){

        print_menu();

        char *p = s;

        while((*p++=getchar ()) != 13);
        *p = '\0';
        mode = atoi(s);

        status = menu(&adxl_device, mode);

    }

    cleanup_platform();
    return 0;
}


void print_menu(){
    printf("***** AXL345 demo application ***** \r\n");
    printf("1. change bandwidth speed\r\n");
    printf("2. change range\r\n");
    printf("3. calibration\r\n");
    printf("4. pool Mode\r\n");
    printf("5. enable interrupt\r\n");
    printf("6. disable interrupt\r\n");
    printf("7. change threshold tap\r\n");
    printf("8. enable TapAxes\r\n");
    printf("9. disable TapAxes\r\n");
    printf("10. change Duration\r\n");
    printf("11. change latent\r\n");
    printf("12. change window\r\n");
	printf("13. change activity threshold\r\n");
	printf("14. change activity control\r\n");
	printf("15. change inactivity control\r\n");
	printf("16. change inactivity threshold\r\n");
	printf("17. change inactivity time\r\n");
	printf("18. change freefall threshold\r\n");
	printf("19. change freefall time\r\n");
	printf("20. change fifo samples value\r\n");
	printf("21. change fifo mode\r\n");
	printf("22. change offset x\r\n");
	printf("23. change offset y\r\n");
	printf("24. change offset z\r\n");
}



int menu(axi_adxl* ptr, int mode){

    switch(mode){
        case 1:
            selector_change_bw(ptr);
        break;

        case 2:
            selector_change_range(ptr);
        break;

        case 3:
        	selector_calibration(ptr);
		break;

        case 4:
        	selector_pooling(ptr);
		break;

        case 5: 
            selector_enable_interrupt(ptr);
        break;

        case 6:
            selector_disable_interrupt(ptr);
        break;

        case 7:
        	selector_set_threshold_tap(ptr);
        break;

        case 8:
            selector_enable_tap_axes(ptr);
        break;

        case 9:
            selector_disable_tap_axes(ptr);
        break;

        case 10:
            selector_change_duration(ptr);
        break;

        case 11:
            selector_change_latent(ptr);
        break;

        case 12:
            selector_change_window(ptr);
        break;

        case 13:
        	selector_change_thresh_act(ptr);
		break;

        case 14:
    		selector_change_act_ctl(ptr);
		break;

        case 15:
        	selector_change_inact_ctl(ptr);
		break;

        case 16:
        	selector_change_thresh_inact(ptr);
		break;

        case 17:
        	selector_change_time_inact(ptr);
		break;

        case 18:
        	selector_change_thresh_ff(ptr);
		break;

        case 19:
        	selector_change_time_ff(ptr);
		break;

        case 20:
        	selector_change_fifo_samples(ptr);
		break;

        case 21:
        	selector_change_fifo_mode(ptr);
		break;

        case 22:
        	selector_change_offset_x(ptr);
		break;

        case 23:
        	selector_change_offset_y(ptr);
		break;

        case 24:
        	selector_change_offset_z(ptr);
		break;

        default :
            return 0;
        break;
    }
    return ADXL_OK;
}


int scugic_initialize(XScuGic *ptr, axi_adxl* adxl_ptr){

    int status = 0;

    XScuGic_Config *cfg;

    cfg = XScuGic_LookupConfig(XPAR_SCUGIC_0_DEVICE_ID);
    if (!cfg){
        return XST_FAILURE;
    }

    status = XScuGic_CfgInitialize(ptr, cfg, cfg->CpuBaseAddress);
    if (status != XST_SUCCESS){
        return XST_FAILURE;
    }

    XScuGic_SetPriorityTriggerType(ptr, XPAR_FABRIC_AXI_ADXL345_VHD_0_ADXL_IRQ_INTR, 0x00, 0x1);

    status = XScuGic_Connect(ptr, XPAR_FABRIC_AXI_ADXL345_VHD_0_ADXL_IRQ_INTR, (Xil_InterruptHandler)adxl_intr_handler, adxl_ptr);
    if (status != XST_SUCCESS){
        return XST_FAILURE;
    }

    XScuGic_Enable(ptr, XPAR_FABRIC_AXI_ADXL345_VHD_0_ADXL_IRQ_INTR);

    Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT, (Xil_ExceptionHandler)XScuGic_InterruptHandler, ptr);
    Xil_ExceptionEnable();

    return status ;
}



void selector_change_bw(axi_adxl *ptr){

    char s [256];
    int value = 0;

    printf("Selected bw mode\r\n");
    printf("0. 0.10 Hz \r\n");
    printf("1. 0.20 Hz \r\n");
    printf("2. 0.39 Hz \r\n");
    printf("3. 0.78 Hz \r\n");
    printf("4. 1.56 Hz \r\n");
    printf("5. 3.13 Hz \r\n");
    printf("6. 6.25 Hz \r\n");
    printf("7. 12.5 Hz \r\n");
    printf("8. 25 Hz \r\n");
    printf("9. 50 Hz \r\n");
    printf("10. 100 Hz \r\n");
    printf("11. 200 Hz \r\n");
    printf("12. 400 Hz \r\n");
    printf("13. 800 Hz \r\n");
    printf("14. 1600 Hz \r\n");
    printf("15. 3200 Hz \r\n");
    printf("16. 400 Hz low power\r\n");
    printf("17. 200 Hz low power\r\n");
    printf("18. 100 Hz low power\r\n");
    printf("19. 50 Hz low power\r\n");
    printf("20. 25 Hz low power\r\n");
    printf("21. 12_5 Hz low power\r\n");

    printf("Enter bandwidth value : ");
    char *p = s;

    while((*p++=getchar ()) != 13);
    *p = '\0';
    value = atoi(s);
    printf("%d\r\n", value);
    switch(value){
        case 0 : axi_adxl_setup_bandwidth(ptr, BW_RATE_0_10); break;
        case 1 : axi_adxl_setup_bandwidth(ptr, BW_RATE_0_20); break;
        case 2 : axi_adxl_setup_bandwidth(ptr, BW_RATE_0_39); break;
        case 3 : axi_adxl_setup_bandwidth(ptr, BW_RATE_0_78); break;
        case 4 : axi_adxl_setup_bandwidth(ptr, BW_RATE_1_56); break;
        case 5 : axi_adxl_setup_bandwidth(ptr, BW_RATE_3_13); break;
        case 6 : axi_adxl_setup_bandwidth(ptr, BW_RATE_6_25); break;
        case 7 : axi_adxl_setup_bandwidth(ptr, BW_RATE_12_5); break;
        case 8 : axi_adxl_setup_bandwidth(ptr, BW_RATE_25); break;
        case 9 : axi_adxl_setup_bandwidth(ptr, BW_RATE_50); break;
        case 10 : axi_adxl_setup_bandwidth(ptr, BW_RATE_100); break;
        case 11 : axi_adxl_setup_bandwidth(ptr, BW_RATE_200); break;
        case 12 : axi_adxl_setup_bandwidth(ptr, BW_RATE_400); break;
        case 13 : axi_adxl_setup_bandwidth(ptr, BW_RATE_800); break;
        case 14 : axi_adxl_setup_bandwidth(ptr, BW_RATE_1600); break;
        case 15 : axi_adxl_setup_bandwidth(ptr, BW_RATE_3200); break;
        case 16 : axi_adxl_setup_bandwidth(ptr, BW_RATE_400_LP); break;
        case 17 : axi_adxl_setup_bandwidth(ptr, BW_RATE_200_LP); break;
        case 18 : axi_adxl_setup_bandwidth(ptr, BW_RATE_100_LP); break;
        case 19 : axi_adxl_setup_bandwidth(ptr, BW_RATE_50_LP); break;
        case 20 : axi_adxl_setup_bandwidth(ptr, BW_RATE_25_LP); break;
        case 21 : axi_adxl_setup_bandwidth(ptr, BW_RATE_12_5_LP); break;
        default : printf("undefined input\r\n"); break;

    }
}



void selector_change_range(axi_adxl *ptr){

    char s [256];
    int value = 0;
    printf("Selected change bw mode\r\n");
    printf("0. RANGE 2G \r\n");
    printf("1. RANGE 4G \r\n");
    printf("2. RANGE 8G \r\n");
    printf("3. RANGE 16G \r\n");
    printf("4. RANGE 2G FULL \r\n");
    printf("5. RANGE 4G FULL \r\n");
    printf("6. RANGE 8G FULL \r\n");
    printf("7. RANGE 16G FULL \r\n");

    printf("Enter range value : ");
    char *p = s;

    while((*p++=getchar ()) != 13);
    *p = '\0';
    value = atoi(s);
    printf("%d\r\n", value);
    switch(value){
        case 0 : axi_adxl_setup_range(ptr, RANGE_2G); break;
        case 1 : axi_adxl_setup_range(ptr, RANGE_4G); break;
        case 2 : axi_adxl_setup_range(ptr, RANGE_8G); break;
        case 3 : axi_adxl_setup_range(ptr, RANGE_16G); break;
        case 4 : axi_adxl_setup_range(ptr, RANGE_2G_FULL); break;
        case 5 : axi_adxl_setup_range(ptr, RANGE_4G_FULL); break;
        case 6 : axi_adxl_setup_range(ptr, RANGE_8G_FULL); break;
        case 7 : axi_adxl_setup_range(ptr, RANGE_16G_FULL); break;
        default : printf("undefined input\r\n"); break;

    }
}




void selector_calibration(axi_adxl *ptr){

    char s [256];
    int value = 0;
    printf("Selected calibration\r\n");
    printf("Enter number of calibration steps : ");
    char *p = s;

    while((*p++=getchar ()) != 13);
    *p = '\0';
    value = atoi(s);
    printf("%d\r\n", value);

    axi_adxl_calibration(ptr, value);

}


void selector_pooling(axi_adxl *ptr){
    g_coord g;

    printf("Selected pooling mode\r\n");
    printf("Press 0 and enter for exit pooling mode\r\n");
	while(getchar() != 0x30){
    	axi_adxl_get_gravity(ptr, &g);
    	printf("x: %2.4f\ty: %2.4f\tz: %2.4f\r\n", g.x, g.y, g.z);
    }
	printf("Stopped pooling mode\r\n");

}



void selector_enable_interrupt(axi_adxl *ptr){
	printf("Selected interrupt enable subunit\r\n");
	printf("1. DATA_READY\r\n");
	printf("2. SINGLE TAP\r\n");
	printf("3. DOUBLE TAP\r\n");
	printf("4. ACTIVITY\r\n");
	printf("5. INACTIVITY\r\n");
	printf("6. FREE FALL\r\n");
	printf("7. WATERMARK\r\n");
	printf("8. OVERRUN\r\n");

    printf("Which interrupt event enable : ");

    char s [256];
    int value = 0;
    char *p = s;

    while((*p++=getchar ()) != 13);
    *p = '\0';
    value = atoi(s);
    printf("%d\r\n", value);

    switch (value){
        case 1 : axi_adxl_enable_interrupt(ptr, DATA_READY); break;
        case 2 : axi_adxl_enable_interrupt(ptr, SINGLE_TAP); break;
        case 3 : axi_adxl_enable_interrupt(ptr, DOUBLE_TAP); break;
        case 4 : axi_adxl_enable_interrupt(ptr, ACTIVITY); break;
        case 5 : axi_adxl_enable_interrupt(ptr, INACTIVITY); break;
        case 6 : axi_adxl_enable_interrupt(ptr, FREE_FALL); break;
        case 7 : axi_adxl_enable_interrupt(ptr, WATERMARK); break;
        case 8 : axi_adxl_enable_interrupt(ptr, OVERRUN); break;

        default : break;
    }

}



void selector_disable_interrupt(axi_adxl *ptr){
	printf("Selected interrupt disable subunit\r\n");
	printf("1. DATA_READY\r\n");
	printf("2. SINGLE TAP\r\n");
	printf("3. DOUBLE TAP\r\n");
	printf("4. ACTIVITY\r\n");
	printf("5. INACTIVITY\r\n");
	printf("6. FREE FALL\r\n");
	printf("7. WATERMARK\r\n");
	printf("8. OVERRUN\r\n");

    printf("Which interrupt will be disabled : ");

    char s [256];
    int value = 0;
    char *p = s;

    while((*p++=getchar ()) != 13);
    *p = '\0';
    value = atoi(s);
    printf("%d\r\n", value);

    switch (value){
        case 1 : axi_adxl_disable_interrupt(ptr, DATA_READY); break;
        case 2 : axi_adxl_disable_interrupt(ptr, SINGLE_TAP); break;
        case 3 : axi_adxl_disable_interrupt(ptr, DOUBLE_TAP); break;
        case 4 : axi_adxl_disable_interrupt(ptr, ACTIVITY); break;
        case 5 : axi_adxl_disable_interrupt(ptr, INACTIVITY); break;
        case 6 : axi_adxl_disable_interrupt(ptr, FREE_FALL); break;
        case 7 : axi_adxl_disable_interrupt(ptr, WATERMARK); break;
        case 8 : axi_adxl_disable_interrupt(ptr, OVERRUN); break;
        default : break;
    }

}



void selector_set_threshold_tap(axi_adxl *ptr){

	printf("Selected changing of threshold tap\r\n");
	printf("Which new value must be installed: ");

	char s [256];
	int value = 0;
	char *p = s;

	while((*p++=getchar ()) != 13);
	*p = '\0';
	value = atoi(s);
	printf("%d\r\n", value);

	axi_adxl_set_thresh_tap(ptr, value);


}


void selector_enable_tap_axes(axi_adxl *ptr){
    printf("Selected enable tap axes\r\n");
    printf("1. Suppress\r\n");
    printf("2. TAP_X\r\n");
    printf("3. TAP_Y\r\n");
    printf("4. TAP_Z\r\n");

    char s [256];
    int value = 0;
    char *p = s;

    while((*p++=getchar ()) != 13);
    *p = '\0';
    value = atoi(s);
    printf("%d\r\n", value);

    switch (value){
        case 1: axi_adxl_enable_tap_axes(ptr, SUPRESS); break;
        case 2: axi_adxl_enable_tap_axes(ptr, MASK_X); break;
        case 3: axi_adxl_enable_tap_axes(ptr, MASK_Y); break;
        case 4: axi_adxl_enable_tap_axes(ptr, MASK_Z); break;
    }

}



void selector_disable_tap_axes(axi_adxl *ptr){
	printf("Selected disable tap axes\r\n");
    printf("1. Suppress\r\n");
    printf("2. TAP_X\r\n");
    printf("3. TAP_Y\r\n");
    printf("4. TAP_Z\r\n");

    char s [256];
    int value = 0;
    char *p = s;

    while((*p++=getchar ()) != 13);
    *p = '\0';
    value = atoi(s);
    printf("%d\r\n", value);

    switch (value){
        case 1: axi_adxl_disable_tap_axes(ptr, SUPRESS); break;
        case 2: axi_adxl_disable_tap_axes(ptr, MASK_X); break;
        case 3: axi_adxl_disable_tap_axes(ptr, MASK_Y); break;
        case 4: axi_adxl_disable_tap_axes(ptr, MASK_Z); break;
    }

}



void selector_change_duration(axi_adxl *ptr){
	printf("Selected changing of duration\r\n");
	printf("Which new value must be installed: ");

	char s [256];
	int value = 0;
	char *p = s;

	while((*p++=getchar ()) != 13);
	*p = '\0';
	value = atoi(s);
	printf("%d\r\n", value);

	axi_adxl_set_duration(ptr, value);

}




void selector_change_latent(axi_adxl *ptr){
	printf("Selected changing of latent\r\n");
	printf("Which new value must be installed: ");

	char s [256];
	int value = 0;
	char *p = s;

	while((*p++=getchar ()) != 13);
	*p = '\0';
	value = atoi(s);
	printf("%d\r\n", value);

	axi_adxl_set_latent(ptr, value);

}



void selector_change_window(axi_adxl *ptr){
	printf("Selected changing of window\r\n");
	printf("Which new value must be installed: ");

	char s [256];
	int value = 0;
	char *p = s;

	while((*p++=getchar ()) != 13);
	*p = '\0';
	value = atoi(s);
	printf("%d\r\n", value);

	axi_adxl_set_window(ptr, value);

}



void selector_change_thresh_act(axi_adxl *ptr){
	printf("Selected changing of activity threshold\r\n");
	printf("Which new value must be installed: ");

	char s [256];
	int value = 0;
	char *p = s;

	while((*p++=getchar ()) != 13);
	*p = '\0';
	value = atoi(s);
	printf("%d\r\n", value);

	axi_adxl_set_thresh_act(ptr, value);

}


void selector_change_act_ctl(axi_adxl *ptr){
	printf("Selected change activity control axes\r\n");
	printf("1. ACT AC MODE\r\n");
	printf("2. ACT DC MODE\r\n");
	printf("3. ACT_X enable\r\n");
	printf("4. ACT_X disable\r\n");
	printf("5. ACT_Y enable\r\n");
	printf("6. ACT_Y disable\r\n");
	printf("7. ACT_Z enable\r\n");
	printf("8. ACT_Z disable\r\n");


	char s [256];
	int value = 0;
	char *p = s;

	while((*p++=getchar ()) != 13);
	*p = '\0';
	value = atoi(s);
	printf("%d\r\n", value);

	switch (value){
		case 1: axi_adxl_set_act_couple(ptr, AC_COUPLE); break;
		case 2: axi_adxl_set_act_couple(ptr, DC_COUPLE); break;
		case 3: axi_adxl_set_act_ctl(ptr, MASK_X, ACTION_ENABLE); break;
		case 4: axi_adxl_set_act_ctl(ptr, MASK_X, ACTION_DISABLE); break;
		case 5: axi_adxl_set_act_ctl(ptr, MASK_Y, ACTION_ENABLE); break;
		case 6: axi_adxl_set_act_ctl(ptr, MASK_Y, ACTION_DISABLE); break;
		case 7: axi_adxl_set_act_ctl(ptr, MASK_Z, ACTION_ENABLE); break;
		case 8: axi_adxl_set_act_ctl(ptr, MASK_Z, ACTION_DISABLE); break;
	}

}


void selector_change_inact_ctl(axi_adxl *ptr){
	printf("Selected change inactivity control axes\r\n");
	printf("1. INACT AC MODE\r\n");
	printf("2. INACT DC MODE\r\n");
	printf("3. INACT_X enable\r\n");
	printf("4. INACT_X disable\r\n");
	printf("5. INACT_Y enable\r\n");
	printf("6. INACT_Y disable\r\n");
	printf("7. INACT_Z enable\r\n");
	printf("8. INACT_Z disable\r\n");


	char s [256];
	int value = 0;
	char *p = s;

	while((*p++=getchar ()) != 13);
	*p = '\0';
	value = atoi(s);
	printf("%d\r\n", value);

	switch (value){
		case 1: axi_adxl_set_inact_couple(ptr, AC_COUPLE); break;
		case 2: axi_adxl_set_inact_couple(ptr, DC_COUPLE); break;
		case 3: axi_adxl_set_inact_ctl(ptr, MASK_X, ACTION_ENABLE); break;
		case 4: axi_adxl_set_inact_ctl(ptr, MASK_X, ACTION_DISABLE); break;
		case 5: axi_adxl_set_inact_ctl(ptr, MASK_Y, ACTION_ENABLE); break;
		case 6: axi_adxl_set_inact_ctl(ptr, MASK_Y, ACTION_DISABLE); break;
		case 7: axi_adxl_set_inact_ctl(ptr, MASK_Z, ACTION_ENABLE); break;
		case 8: axi_adxl_set_inact_ctl(ptr, MASK_Z, ACTION_DISABLE); break;
	}

}


void selector_change_thresh_inact(axi_adxl *ptr){
	printf("Selected changing of inactivity threshold\r\n");
	printf("Which new value must be installed: ");

	char s [256];
	int value = 0;
	char *p = s;

	while((*p++=getchar ()) != 13);
	*p = '\0';
	value = atoi(s);
	printf("%d\r\n", value);

	axi_adxl_set_thresh_inact(ptr, value);

}


void selector_change_time_inact(axi_adxl *ptr){
	printf("Selected changing of inactivity time \r\n");
	printf("Which new value must be installed in seconds: ");

	char s [256];
	int value = 0;
	char *p = s;

	while((*p++=getchar ()) != 13);
	*p = '\0';
	value = atoi(s);
	printf("%d\r\n", value);

	axi_adxl_set_time_inact(ptr, value);

}

void selector_change_thresh_ff(axi_adxl *ptr){
	printf("Selected changing of freefall threshold\r\n");
	printf("Which new value must be installed(0.062g/lsb): ");

	char s [256];
	int value = 0;
	char *p = s;

	while((*p++=getchar ()) != 13);
	*p = '\0';
	value = atoi(s);
	printf("%d\r\n", value);

	axi_adxl_set_thresh_ff(ptr, value);


}



void selector_change_time_ff(axi_adxl *ptr){
	printf("Selected changing of freefall time\r\n");
	printf("Which new value must be installed(0.005s/lsb): ");

	char s [256];
	int value = 0;
	char *p = s;

	while((*p++=getchar ()) != 13);
	*p = '\0';
	value = atoi(s);
	printf("%d\r\n", value);

	axi_adxl_set_time_ff(ptr, value);

}


void selector_change_fifo_samples(axi_adxl *ptr){
	printf("Selected changing of fifo sample count\r\n");
	printf("Which new value must be installed: ");

	char s [256];
	int value = 0;
	char *p = s;

	while((*p++=getchar ()) != 13);
	*p = '\0';
	value = atoi(s);
	printf("%d\r\n", value);

	axi_adxl_set_fifo_samples(ptr, value);

}


void selector_change_fifo_mode(axi_adxl *ptr){
	printf("Selected change fifo mode: \r\n");
	printf("1. BYPASS\r\n");
	printf("2. FIFO\r\n");
	printf("3. STREAM\r\n");
	printf("4. TRIGGER\r\n");

	char s [256];
	int value = 0;
	char *p = s;

	while((*p++=getchar ()) != 13);
	*p = '\0';
	value = atoi(s);
	printf("%d\r\n", value);

	switch (value){
		case 1: axi_adxl_set_fifo_mode(ptr, BYPASS); break;
		case 2: axi_adxl_set_fifo_mode(ptr, FIFO); break;
		case 3: axi_adxl_set_fifo_mode(ptr, STREAM); break;
		case 4: axi_adxl_set_fifo_mode(ptr, TRIGGER); break;
	}

}



void selector_change_offset_x(axi_adxl *ptr){
	printf("Selected changing offset x\r\n");
	printf("Which new value must be installed: ");

	char s [256];
	int value = 0;
	char *p = s;

	while((*p++=getchar ()) != 13);
	*p = '\0';
	value = atoi(s);
	printf("%d\r\n", value);

	axi_adxl_set_offset_x(ptr, value);

}



void selector_change_offset_y(axi_adxl *ptr){
	printf("Selected changing offset y\r\n");
	printf("Which new value must be installed: ");

	char s [256];
	int value = 0;
	char *p = s;

	while((*p++=getchar ()) != 13);
	*p = '\0';
	value = atoi(s);
	printf("%d\r\n", value);

	axi_adxl_set_offset_y(ptr, value);

}




void selector_change_offset_z(axi_adxl *ptr){
	printf("Selected changing offset z\r\n");
	printf("Which new value must be installed: ");

	char s [256];
	int value = 0;
	char *p = s;

	while((*p++=getchar ()) != 13);
	*p = '\0';
	value = atoi(s);
	printf("%d\r\n", value);

	axi_adxl_set_offset_z(ptr, value);

}



void adxl_intr_handler(void *callback){
    axi_adxl *ptr = (axi_adxl*)callback;

    g_coord g;

    adxl_cfg_ctl_intr_ack(ptr->cfg);
    uint8_t fifo_entries = 0;
    switch (adxl_dev_get_int_source(ptr->dev) & adxl_dev_get_int_enable(ptr->dev)){

    	case ACTIVITY :
    		axi_adxl_get_gravity(ptr, &g);
    		printf("<ACT>[x] : %3.3f \t[y] : %3.3f \t [z] %3.3f\t", g.x, g.y, g.z);
    		if (axi_adxl_has_act_x_source(ptr)){
    			printf("X");
    		}
    		if (axi_adxl_has_act_y_source(ptr)){
    			printf("Y");
    		}
    		if (axi_adxl_has_act_z_source(ptr)){
    			printf("Z");
    		}
    		printf("\r\n");
		break;

    	case INACTIVITY :
    		axi_adxl_get_gravity(ptr, &g);
    		printf("<INACT>[x] : %3.3f \t[y] : %3.3f \t [z] %3.3f\t", g.x, g.y, g.z);
    		printf("\r\n");
		break;

    	case SINGLE_TAP :
    		axi_adxl_get_gravity(ptr, &g);
    		printf("<ST>[x] : %3.3f \t[y] : %3.3f \t [z] %3.3f\t", g.x, g.y, g.z);
    		if (axi_adxl_has_tap_x_source(ptr)){
    			printf("X");
    		}
    		if (axi_adxl_has_tap_y_source(ptr)){
    			printf("Y");
    		}
    		if (axi_adxl_has_tap_z_source(ptr)){
    			printf("Z");
    		}
    		printf("\r\n");
		break;

    	case DOUBLE_TAP :
    		axi_adxl_get_gravity(ptr, &g);
    		printf("<DT>[x] : %3.3f \t[y] : %3.3f \t [z] %3.3f\t", g.x, g.y, g.z);
    		if (axi_adxl_has_tap_x_source(ptr)){
    			printf("X");
    		}
    		if (axi_adxl_has_tap_y_source(ptr)){
    			printf("Y");
    		}
    		if (axi_adxl_has_tap_z_source(ptr)){
    			printf("Z");
    		}
    		printf("\r\n");
		break;

    	case DATA_READY :
    		axi_adxl_get_gravity(ptr, &g);
    		printf("<DR>[x] : %3.3f \t[y] : %3.3f \t [z] %3.3f\r\n", g.x, g.y, g.z);
		break;

    	case FREE_FALL :
    		axi_adxl_get_gravity(ptr, &g);
    		printf("<FF>[x] : %3.3f \t[y] : %3.3f \t [z] %3.3f\r\n", g.x, g.y, g.z);
		break;

    	case WATERMARK :
    		fifo_entries = axi_adxl_get_entries(ptr);

    		printf("<FIFO>[%2d]\r\n", fifo_entries);

    		for (int i = 0; i < fifo_entries; i++){
    			adxl_axis *coord = &(((ptr)->cfg)->axis[i]);
    			axi_adxl_convert_raw_to_g(ptr, coord, &g);
        		printf("<WM[%d]>[x] : %3.3f \t[y] : %3.3f \t [z] %3.3f\r\n", i, g.x, g.y, g.z);

    		}
		break;


    	case OVERRUN:
    		fifo_entries = axi_adxl_get_entries(ptr);

    		printf("<FIFO>[%2d]\r\n", fifo_entries);

    		for (int i = 0; i < fifo_entries; i++){
    			adxl_axis *coord = &(((ptr)->cfg)->axis[i]);
    			axi_adxl_convert_raw_to_g(ptr, coord, &g);
        		printf("<OVR>[x] : %3.3f \t[y] : %3.3f \t [z] %3.3f\r\n", g.x, g.y, g.z);
    		}
		break;
    }
    return;
}

