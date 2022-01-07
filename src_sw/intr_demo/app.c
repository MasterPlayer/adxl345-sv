#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "axi_adxl.h"
#include <time.h>
#include <xscugic.h>

#include <xparameters.h>


int scugic_initialize(XScuGic *ptr, axi_adxl* adxl_ptr);
void adxl_intr_handler(void *callback);

int main(){

	init_platform();

	axi_adxl adxl_device;
	XScuGic gic;

	axi_adxl_init(&adxl_device, 0x40030000, 0x40040000);

	scugic_initialize(&gic, &adxl_device);

	int status = axi_adxl_enable(&adxl_device, 0x53, 10);
	if (status != ADXL_OK){
		printf("[MAIN] : enable device return code [%d]\r\n", status);
	}

	status = axi_adxl_calibration(&adxl_device);
	if (status != ADXL_OK){
		printf("[MAIN] : calibration device return code [%d]\r\n", status);
	}

	status = axi_adxl_disable_requesting(&adxl_device);
	if (status != ADXL_OK){
		xil_printf("[MAIN] : disable requests return code : [%d]\r\n", status);
		return 0;
	}

	axi_adxl_set_int_map(&adxl_device, SINGLE_TAP | DOUBLE_TAP | ACTIVITY | INACTIVITY | FREE_FALL | WATERMARK | OVERRUN, 0x00);


	// SingleTap & DoubleTap
	axi_adxl_set_dur(&adxl_device, 0x10);
	axi_adxl_set_thresh_tap(&adxl_device, 4);
	axi_adxl_set_latency(&adxl_device, 0x01);
	axi_adxl_set_window(&adxl_device, 0x01);
	axi_adxl_set_tap_axes_active(&adxl_device, TAP_AXES_TAP_X_EN_MASK | TAP_AXES_TAP_Y_EN_MASK);
	// Activity
	axi_adxl_set_act_ctl(&adxl_device, DC_COUPLE, MASK_X | MASK_Y);
	axi_adxl_set_thresh_act(&adxl_device, 0x10);
	// Inactivity
	axi_adxl_set_inact_ctl(&adxl_device, DC_COUPLE, MASK_X | MASK_Y);
	axi_adxl_set_thresh_inact(&adxl_device, 0x03);
	axi_adxl_set_time_inact(&adxl_device, 0x08);
	// FreeFall
	axi_adxl_set_thresh_ff(&adxl_device, 0x10);
	axi_adxl_set_time_ff(&adxl_device, 0x10);

	axi_adxl_set_fifo_samples(&adxl_device, 0x1F);
	axi_adxl_set_fifo_mode(&adxl_device, FIFO);

	axi_adxl_int_enable(&adxl_device, SINGLE_TAP | DOUBLE_TAP | ACTIVITY | INACTIVITY | FREE_FALL | WATERMARK | OVERRUN);


	volatile int change_mode = 0;

	volatile int mode = 0;

	while(1){
		if (change_mode){

			axi_adxl_change_bw(&adxl_device, mode);

			change_mode = 0;
		}

	}

	cleanup_platform();
    return 0;
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

    XScuGic_SetPriorityTriggerType(ptr, XPAR_FABRIC_AXI_ADXL345_VHD_0_ADXL_IRQ_INTR, 0x00, 0x3);

    status = XScuGic_Connect(ptr, XPAR_FABRIC_AXI_ADXL345_VHD_0_ADXL_IRQ_INTR, (Xil_InterruptHandler)adxl_intr_handler, adxl_ptr);
    if (status != XST_SUCCESS){
        return XST_FAILURE;
    }

    XScuGic_Enable(ptr, XPAR_FABRIC_AXI_ADXL345_VHD_0_ADXL_IRQ_INTR);

    Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT, (Xil_ExceptionHandler)XScuGic_InterruptHandler, ptr);
    Xil_ExceptionEnable();

    return status ;
}



void adxl_intr_handler(void *callback){
	axi_adxl *ptr = (axi_adxl*)callback;

	g_coord g;
	uint8_t entries = (adxl_dev_get_fifo_status(ptr->dev) & FIFO_STATUS_ENTRIES_MASK);
	adxl_cfg_ack(ptr->cfg);


	switch (adxl_dev_get_int_source(ptr->dev) & adxl_dev_get_int_enable(ptr->dev)){
		case DATA_READY :
			axi_adxl_get_gravity(ptr, &g);
			printf("[DR] : [x] : %3.3f \t[y] : %3.3f \t [z] %3.3f\r\n", g.x, g.y, g.z);
		break;

		case SINGLE_TAP :
			axi_adxl_get_gravity(ptr, &g);
			printf("[ST] : [x] : %3.3f \t[y] : %3.3f \t [z] %3.3f\r\n", g.x, g.y, g.z);
		break;

		case DOUBLE_TAP :
			axi_adxl_get_gravity(ptr, &g);
			printf("[DT] : [x] : %3.3f \t[y] : %3.3f \t [z] %3.3f\r\n", g.x, g.y, g.z);
		break;

		case ACTIVITY:
			axi_adxl_get_gravity(ptr, &g);
			printf("[AC] : [x] : %3.3f \t[y] : %3.3f \t [z] %3.3f\r\n", g.x, g.y, g.z);
		break;

		case INACTIVITY:
			axi_adxl_get_gravity(ptr, &g);
			printf("[IA] : [x] : %3.3f \t[y] : %3.3f \t [z] %3.3f\r\n", g.x, g.y, g.z);
		break;

		case FREE_FALL:
			axi_adxl_get_gravity(ptr, &g);
			printf("[FF] : [x] : %3.3f \t[y] : %3.3f \t [z] %3.3f\r\n", g.x, g.y, g.z);
		break;

		case WATERMARK:
			for (int i = 0; i < entries; i++){
				printf("[WM]\t[x] : %4d \t[y] : %4d \t [z] : %4d\r\n", ptr->cfg->axis[i].x, ptr->cfg->axis[i].y, ptr->cfg->axis[i].z);
			}
		break;

		case OVERRUN:

			for (int i = 0; i < entries; i++){
				printf("[OR]\t[x] : %4d \t[y] : %4d \t [z] : %4d\r\n", ptr->cfg->axis[i].x, ptr->cfg->axis[i].y, ptr->cfg->axis[i].z);
			}

		break;

		default :
			printf("\t[INT_SOURCE] : 0x%02x\r\n", adxl_dev_get_int_source(ptr->dev));
			printf("\t\t[DATA_READY] : %d\r\n", adxl_dev_get_int_source(ptr->dev) & INT_SOURCE_DATA_READY_MASK ? 1:0);
			printf("\t\t[SINGLE_TAP] : %d\r\n", adxl_dev_get_int_source(ptr->dev) & INT_SOURCE_SINGLE_TAP_MASK ? 1:0);
			printf("\t\t[DOUBLE_TAP] : %d\r\n", adxl_dev_get_int_source(ptr->dev) & INT_SOURCE_DOUBLE_TAP_MASK ? 1:0);
			printf("\t\t[ACTIVITY] : %d\r\n", adxl_dev_get_int_source(ptr->dev) & INT_SOURCE_ACTIVITY_MASK ? 1:0);
			printf("\t\t[INACTIVITY] : %d\r\n", adxl_dev_get_int_source(ptr->dev) & INT_SOURCE_INACTIVITY_MASK ? 1:0);
			printf("\t\t[FREEFALL] : %d\r\n", adxl_dev_get_int_source(ptr->dev) & INT_SOURCE_FREE_FALL_MASK ? 1:0);
			printf("\t\t[WATERMARK] : %d\r\n", adxl_dev_get_int_source(ptr->dev) & INT_SOURCE_WATERMARK_MASK ? 1:0);
			printf("\t\t[OVERRUN] : %d\r\n", adxl_dev_get_int_source(ptr->dev) & INT_SOURCE_OVERRUN_MASK ? 1:0);
			adxl_cfg_ack(ptr->cfg);

	}



//	g_coord g;
//
//	axi_adxl_get_gravity(ptr, &g);
//
//	printf("[x] : %3.3f \t[y] : %3.3f \t [z] %3.3f\r\n", g.x, g.y, g.z);
	return;
}

