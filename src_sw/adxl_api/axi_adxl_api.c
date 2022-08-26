#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "axi_adxl.h"
#include "selector.h"
#include <xscugic.h>


void print_menu();
void adxl_intr_handler(void *callback);
int menu(axi_adxl *ptr, int mode);
int gic_init(XScuGic *ptr, axi_adxl* adxl_ptr);


int main() {

    init_platform();

    int status = 0;
    int mode = 0;
    char s[256];

    axi_adxl adxl;
    XScuGic gic;

    gic_init(&gic, &adxl);

    while(1){

        print_menu();

        char *p = s;

        while((*p++=getchar ()) != 13);
        *p = '\0';
        mode = atoi(s);

        status = menu(&adxl, mode);

        if (status != ADXL_OK){
            textcolor(DEFAULT, STD, STD);
            printf("[MAIN] : current operation performed with error ");
            textcolor(DEFAULT, BLACK, RED);
            printf("<%d>", status);
            textcolor(DEFAULT, STD, STD);
            printf("\r\n");
        }
    }

    cleanup_platform();
    return 0;
}



void print_menu(){

    printf(" ***** ADXL345 DEMO APP ***** \r\n");
    textcolor(REVERSE, STD, STD);
    printf("=====<Configuration space>=====");
    textcolor(DEFAULT, STD, STD);   
    printf("\r\n");
    printf("\t0. Debug log output\r\n");
    printf("\t1. Reset device\r\n");
    printf("\t2. Initialize device\r\n");
    printf("\t3. Single Request\r\n");
    printf("\t4. Enable interval requestion\r\n");
    printf("\t5. Disable interval requestion\r\n");
    printf("\t6. IRQ allow\r\n");
    printf("\t7. IRQ unallow\r\n");
    printf("\t8. Calibration\r\n");
    printf("\t9. Set iic address\r\n");

    textcolor(REVERSE, STD, STD);
    printf("=====<Device space>=====");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");
    printf("\t50. Bandwidth setup\r\n");
    printf("\t51. Measure start\r\n");
    printf("\t52. Measure stop\r\n");
    printf("\t53. Interrupt enable\r\n");
    printf("\t54. Interrupt disable\r\n");
    printf("\t55. Range change\r\n");
    printf("\t56. Threshold tap change\r\n");
    printf("\t57. Duration change\r\n");

    textcolor(REVERSE, STD, STD);
    printf("=====<Show statistics>=====");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");
    printf("\t100. Dump device register space\r\n");
}



int menu(axi_adxl *ptr, int mode){

    int status = ADXL_OK;

    switch(mode){
        case 0:
            status = selector_axi_adxl_cfg_debug(ptr);
        break;

        case 1 :
            status = selector_axi_adxl_reset(ptr);
        break;

        case 2 :
            status = selector_axi_adxl_init(ptr);
        break;

        case 3 :
            status = selector_axi_adxl_perform_single_request(ptr);
        break;

        case 4 :
            status = selector_axi_adxl_perform_interval_requestion(ptr);
        break;

        case 5 :
            status = selector_axi_adxl_disable_interval_requestion(ptr);
        break;

        case 6 :
            status = selector_axi_adxl_irq_allow(ptr);
        break;

        case 7 :
            status = selector_axi_adxl_irq_unallow(ptr);
        break;

        case 8 :
            status = selector_axi_adxl_calibration(ptr);
        break;

        case 9 :
            status = selector_axi_adxl_set_iic_address(ptr);
        break;


        /*Work with device*/
        case 50 : 
            status = selector_axi_adxl_set_bw_rate(ptr);
        break;          

        case 51 : 
            status = selector_axi_adxl_measurement_start(ptr);
        break;

        case 52 : 
            status = selector_axi_adxl_measurement_stop(ptr); 
        break;

        case 53 : 
            status = selector_axi_adxl_interrupt_enable(ptr);
        break;

        case 54 : 
            status = selector_axi_adxl_interrupt_disable(ptr);
        break;

        case 55 : 
            status = selector_axi_adxl_change_range(ptr);
        break;

        case 56 : 
            status = selector_axi_adxl_change_thresh_tap(ptr);
        break;

        case 57 : 
            status = selector_axi_adxl_change_dur(ptr);
        break;


        /*Debug device*/

        case 100 :
            status = selector_axi_adxl_dev_debug_register_space(ptr);
        break;

        default :
            printf("[MENU] : incorrect selection : 0x%02x\r\n", mode);

    }

    return status;

}





int gic_init(XScuGic *ptr, axi_adxl* adxl_ptr){

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




int irq_indexator = 0;



void adxl_intr_handler(void *callback){
    axi_adxl *ptr = (axi_adxl*)callback;
    printf("IRQ %d\r\n", irq_indexator);
    irq_indexator++;
    axi_adxl_irq_ack(ptr);

    return;
}

