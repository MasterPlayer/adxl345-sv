#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "axi_adxl.h"





int main(){

	init_platform();

	adxl adxl_device;
	axi_adxl_init(&adxl_device, 0x40030000, 0x40040000);

	int status = axi_adxl_dev_enable(&adxl_device, 0x53, 100);

//	adxl_set_data_format(adxl_device.dev, 0x00);
//
//	adxl_set_bw_rate(adxl_device.dev, 0x0f);
//
//	adxl_set_power_ctl(adxl_device.dev, 0x08);
//

//	uint32_t steps = 10;

    while(1){
//    	xil_printf("%c%c%c%c\r\n", 0x1b, 0x5b, 0x32, 0x4a);

//    	if (!steps){
//    		xil_printf("STOPS\r\n");
//    		status = axi_adxl_dev_disable(&adxl_device);
//
//    	}
    	//	adxl_set_power_ctl(adxl_device.dev, 0x08);
//    		adxl_set_power_ctl(adxl_device.dev, POWER_CTL_MEASURE_MASK);

    	status = axi_adxl_dev_get_axis(&adxl_device);
    	printf("%d %d %d\r\n", (int16_t)adxl_device.axis.x, (int16_t)adxl_device.axis.y, (int16_t)adxl_device.axis.z);
//    	xil_printf("x: 0x%04x\r\n", ((uint16_t)adxl_get_datax1(adxl_device.dev)<<8) + (uint16_t)adxl_get_datax0(adxl_device.dev));
//    	axi_adxl_cfg_debug(adxl_device.cfg);
//    	axi_adxl_debug(&adxl_device);
//    	sleep(1);



    }
    cleanup_platform();
    return 0;
}
