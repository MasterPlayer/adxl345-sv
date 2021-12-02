#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "axi_adxl.h"





int main(){

	init_platform();

	adxl adxl_device;
	axi_adxl_init(&adxl_device, 0x40030000, 0x40040000);

	int status = axi_adxl_dev_enable(&adxl_device, 0x53, 1000);

//	adxl_set_data_format(adxl_device.dev, 0x00);
//
//	adxl_set_bw_rate(adxl_device.dev, 0x0f);
//
//	adxl_set_power_ctl(adxl_device.dev, 0x08);
//

	uint32_t steps = 10;

    while(1){
    	xil_printf("%c%c%c%c\r\n", 0x1b, 0x5b, 0x32, 0x4a);

    	if (!steps){
    		xil_printf("STOPS\r\n");
    		status = axi_adxl_dev_disable(&adxl_device);
    		break;
    	}


    	xil_printf("X: 0x%02x%02x\r\n", adxl_get_datax1(adxl_device.dev), adxl_get_datax0(adxl_device.dev));
    	xil_printf("Y: 0x%02x%02x\r\n", adxl_get_datay1(adxl_device.dev), adxl_get_datay0(adxl_device.dev));
    	xil_printf("Z: 0x%02x%02x\r\n", adxl_get_dataz1(adxl_device.dev), adxl_get_dataz0(adxl_device.dev));
//    	axi_adxl_cfg_debug(adxl_device.cfg);
//    	axi_adxl_debug(&adxl_device);
    	sleep(1);
    	steps--;


    }
    cleanup_platform();
    return 0;
}
