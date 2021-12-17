#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "axi_adxl.h"
#include <time.h>

#include <xparameters.h>


int main(){

	init_platform();

	axi_adxl adxl_device;

	axi_adxl_init(&adxl_device, 0x40030000, 0x40040000);

	int status = axi_adxl_enable(&adxl_device, 0x53, 10);
	if (status != ADXL_OK){
		printf("[MAIN] : enable device return code [%d]\r\n", status);
	}
//
	status = axi_adxl_calibration(&adxl_device);
	if (status != ADXL_OK){
		printf("[MAIN] : calibration device return code [%d]\r\n", status);
	}

	g_coord g;

	volatile int i = 0;


//
//	status = axi_adxl_disable_requesting(&adxl_device);
//	if (status != ADXL_OK){
//		xil_printf("[MAIN] : disable requests return code : [%d]\r\n", status);
//		return 0;
//	}



//	uint8_t thresh_tap = 0x10;
//	float thresh_tap_float = 0.5;

//	axi_adxl_int_invert(&adxl_device);

//	axi_adxl_set_int_map(&adxl_device, SINGLE_TAP, 0x00);
//	axi_adxl_set_dur(&adxl_device, 0x10);
//	axi_adxl_set_thresh_tap(&adxl_device, 4);
//	axi_adxl_set_latency(&adxl_device, 0x01);
//	axi_adxl_set_window(&adxl_device, 0x01);
//	axi_adxl_set_tap_axes_active(&adxl_device, TAP_AXES_TAP_X_EN_MASK | TAP_AXES_TAP_Y_EN_MASK);
//	axi_adxl_int_enable(&adxl_device, SINGLE_TAP);


	//	axi_adxl_get_thresh_tap(&adxl_device, &thresh_tap);
//	axi_adxl_set_thresh_tap_g(&adxl_device, thresh_tap_float);
//	axi_adxl_get_thresh_tap_g(&adxl_device, &thresh_tap_float);

	//

//
//	float x_data[1024];
//
//	volatile int req = 1;
//	volatile int delay = 100;
//	float avg = 0;
	while(1){
//		axi_adxl_debug(&adxl_device);
		//
//		avg = 0;
//
//		for (int index = 0; index < 1024; index++){
//			status = axi_adxl_get_gravity(&adxl_device, &g);
//			x_data[index] = g.x;
//			for (int delay_cnt = 0; delay_cnt < delay; delay_cnt++);
//		}
//
//		for (int line_index = 0; line_index < 128; line_index++){
//			printf("[%d]: \t", line_index);
//			for (int index = 0; index < 8; index++){
//				printf("%3.3f \t", x_data[line_index*8 + index]);
//				avg += x_data[line_index*8 + index];
//			}
//			printf("\r\n");
//		}
//
//		avg /= 1024;
//		printf("AVERAGE: %3.3f\r\n", avg);


//		adxl_cfg_set_request_interval(adxl_device.cfg, req);
		status = axi_adxl_get_gravity(&adxl_device, &g);

		printf("[x]:\t%3.3f \t[y]:\t%3.3f \t[z]:\t%3.3f\r\n", g.x, g.y, g.z);
//
//

//		switch (i){
//			case 1 :
//				status = axi_adxl_set_range(&adxl_device, DATA_FORMAT_RANGE_2G);
//				status = axi_adxl_calibration(&adxl_device);
//				break;
//			case 2 :
//				status = axi_adxl_set_range(&adxl_device, DATA_FORMAT_RANGE_4G);
//				status = axi_adxl_calibration(&adxl_device);
//				break;
//			case 3 :
//				status = axi_adxl_set_range(&adxl_device, DATA_FORMAT_RANGE_8G);
//				status = axi_adxl_calibration(&adxl_device);
//				break;
//			case 4 :
//				status = axi_adxl_set_range(&adxl_device, DATA_FORMAT_RANGE_16G);
//				status = axi_adxl_calibration(&adxl_device);
//				break;
//			default : break;
//		}
////
//		status = axi_adxl_get_pitch(&adxl_device, &pitch);
//		status = axi_adxl_get_roll(&adxl_device, &roll);

//		printf("\t%3.2f \t%3.2f\r\n", pitch, roll);

//    	sleep(1);




    }
    cleanup_platform();
    return 0;
}
