#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "axi_adxl.h"

#define ADXL_CFG_BASEADDRESS 0x40030000


int main() {
    init_platform();
    while(1){
    	axi_adxl_cfg_debug(ADXL_CFG_BASEADDRESS);
    	sleep(1);
    }
    axi_adxl adxl;


    cleanup_platform();
    return 0;
}
