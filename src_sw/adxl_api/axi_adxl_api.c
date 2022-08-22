#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "axi_adxl.h"

#define ADXL_CFG_BASEADDRESS 0x40030000
#define ADXL_DEV_BASEADDRESS 0x40040000
#define ADXL_IIC_ADDRESS     0x53

int main() {
    init_platform();

    axi_adxl adxl;

    axi_adxl_cfg_debug(ADXL_CFG_BASEADDRESS);

    axi_adxl_init(&adxl, ADXL_CFG_BASEADDRESS, ADXL_DEV_BASEADDRESS, ADXL_IIC_ADDRESS);

    while(1){
    	axi_adxl_cfg_debug(&adxl);
    	sleep(1);
    }


    cleanup_platform();
    return 0;
}
