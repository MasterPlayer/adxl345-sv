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
    printf("\t   29. \tThreshold tap change [REG_29]\r\n");
    printf("\t   33. \tDuration change [REG_33]\r\n");
    printf("\t   34. \tLatent change [REG_34]\r\n");
    printf("\t   35. \tWindow change [REG_35]\r\n");
    printf("\t   36. \tActivity threshold change [REG_36]\r\n");
    printf("\t   37. \tInactivity threshold change [REG_37]\r\n");
    printf("\t   38. \tInactivity time change [REG_38]\r\n");
    printf("\t   44. \tBandwidth rate setup [REG_44]\r\n");
    printf("\t 4531. \tMeasure start [REG_45][BIT_3][ENABLE]\r\n");
    printf("\t 4530. \tMeasure stop [REG_45][BIT_3][DISABLE]\r\n");
    printf("\t  461. \tInterrupt enable [REG_46][ENABLE]\r\n");
    printf("\t  460. \tInterrupt disable [REG_46][DISABLE]\r\n");
    printf("\t49310. \tRange change [REG_49][BIT_3][BIT_1][BIT_0]\r\n");


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
        // Possible masks : [REGISTER_NUMBER][BIT_NUMBER][BIT_STATE]
        // Possible masks : [REGISTER_NUMBER][REGISTER_STATE]
        // Possible masks : [REGISTER_NUMBER][BIT_NUMBERS]
        case 44 : 
            status = selector_axi_adxl_set_bw_rate(ptr);
        break;          

        case 4531 : 
            status = selector_axi_adxl_measurement_start(ptr);
        break;

        case 4530 : 
            status = selector_axi_adxl_measurement_stop(ptr); 
        break;

        case 461 : 
            status = selector_axi_adxl_interrupt_enable(ptr);
        break;

        case 460 : 
            status = selector_axi_adxl_interrupt_disable(ptr);
        break;

        case 49310 : 
            status = selector_axi_adxl_change_range(ptr);
        break;

        case 29 : //0x1D
            status = selector_axi_adxl_change_thresh_tap(ptr);
        break;

        case 33 : //0x21 
            status = selector_axi_adxl_change_dur(ptr);
        break;

        case 34 : //0x22
        	status = selector_axi_adxl_change_latent(ptr);
        break;

        case 35 : //0x23
        	status = selector_axi_adxl_change_window(ptr);
        break;

        case 36 : //0x24
        	status = selector_axi_adxl_change_thresh_act(ptr);
        break;

        case 37 : //0x25
        	status = selector_axi_adxl_change_thresh_inact(ptr);
        break;

        case 38 : //0x26
        	status = selector_axi_adxl_change_time_inact(ptr);
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

