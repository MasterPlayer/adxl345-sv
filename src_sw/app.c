#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "axi_adxl.h"
#include <time.h>




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
		printf("[MAIN] : calibration device return code [%d]", status);
	}

	g_coord g;

	volatile int i = 0;

	float pitch = 0;
	float roll = 0;

		while(1){

		axi_adxl_get_gravity(&adxl_device, &g);

//		printf("[x]:\t%3.2f \t[y]:\t%3.2f \t[z]:\t%3.2f\r\n", g.x, g.y, g.z);

		switch (i){
			case 1 :
				status = axi_adxl_set_range(&adxl_device, DATA_FORMAT_RANGE_2G);
				status = axi_adxl_calibration(&adxl_device);
				break;
			case 2 :
				status = axi_adxl_set_range(&adxl_device, DATA_FORMAT_RANGE_4G);
				status = axi_adxl_calibration(&adxl_device);
				break;
			case 3 :
				status = axi_adxl_set_range(&adxl_device, DATA_FORMAT_RANGE_8G);
				status = axi_adxl_calibration(&adxl_device);
				break;
			case 4 :
				status = axi_adxl_set_range(&adxl_device, DATA_FORMAT_RANGE_16G);
				status = axi_adxl_calibration(&adxl_device);
				break;
			default : break;
		}

		status = axi_adxl_get_pitch(&adxl_device, &pitch);
		status = axi_adxl_get_roll(&adxl_device, &roll);

		printf("\t%3.2f \t%3.2f\r\n", pitch, roll);

    	sleep(1);




    }
    cleanup_platform();
    return 0;
}
