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

	axi_adxl_set_int_map(&adxl_device, WATERMARK, 0x00);
	axi_adxl_set_fifo_samples(&adxl_device, 0x1F);
	axi_adxl_set_fifo_mode(&adxl_device, FIFO);
	axi_adxl_int_enable(&adxl_device, WATERMARK);


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

int iter = 0;

void adxl_intr_handler(void *callback){
	axi_adxl *ptr = (axi_adxl*)callback;

	uint8_t entries = (adxl_dev_get_fifo_status(ptr->dev) & FIFO_STATUS_ENTRIES_MASK);

	printf("[%2d]\r\n", entries);

	for (int i = 0; i < entries; i++){
		printf("\t[x] : %4d \t[y] : %4d \t [z] : %4d\r\n", ptr->cfg->axis[i].x, ptr->cfg->axis[i].y, ptr->cfg->axis[i].z);
	}

	adxl_cfg_ack(ptr->cfg);


//	g_coord g;
//
//	axi_adxl_get_gravity(ptr, &g);
//
//	printf("[x] : %3.3f \t[y] : %3.3f \t [z] %3.3f\r\n", g.x, g.y, g.z);
	return;
}

