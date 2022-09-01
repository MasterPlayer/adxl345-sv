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

const char *error_parser[] = { "SUCCESS", "UNINITIALIZED", "LINK LOST", "INFINITE RESET", "UNCOMPLETE REQUEST", "CANNOT STOP", "UNCORRECT VALUE", "TIMEOUT" };
const char *function_list[] = {
	"CFG_DBG",
	"CFG_RST",
	"CFG_INIT",
	"SNGL_REQ",
	"INT_REQ_EN"
};


int main() {

    init_platform();

    int status = 0;
    int mode = 0;
    int enter = 0;
    axi_adxl adxl;
    XScuGic gic;

    gic_init(&gic, &adxl);

    while(1){

        print_menu();

        scanf("%d", &mode);
    	enter = 0;
        status = menu(&adxl, mode);
        int status_err = status * -1;
        if (status != ADXL_OK){
            textcolor(DEFAULT, STD, STD);
            printf("[MAIN] : <%s> current operation performed with error ", function_list[mode]);
            textcolor(DEFAULT, BLACK, RED);
            int status_err = status * -1;
            printf("<%d> : [%s]", status, error_parser[status_err]);
            textcolor(DEFAULT, STD, STD);
            printf("\r\n");
        } else {
            printf("\t[MENU] : <%s> completed, status : [%s]\r\n", function_list[mode], error_parser[status_err]);
        }

        while (enter != 1)
        	scanf("%d", &enter);

    }

    cleanup_platform();
    return 0;
}



void print_menu(){

    printf(" >>>>>>>>>>>>>>>>>>>>>>>>> ADXL345 DEMO APP <<<<<<<<<<<<<<<<<<<<<<<<< \r\n");
    textcolor(REVERSE, STD, STD);
    printf("====================<Configuration space>====================");
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
    printf("=======================<Device space>========================");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");
    printf("\t      29. \tThreshold tap change [REG_29]\r\n");
    printf("\t      30. \tSet offset X\r\n");
    printf("\t      31. \tSet offset Y\r\n");
    printf("\t      32. \tSet offset Z\r\n");
    printf("\t      33. \tDuration change [REG_33]\r\n");
    printf("\t      34. \tLatent change [REG_34]\r\n");
    printf("\t      35. \tWindow change [REG_35]\r\n");
    printf("\t      36. \tActivity threshold change [REG_36]\r\n");
    printf("\t      37. \tInactivity threshold change [REG_37]\r\n");
    printf("\t      38. \tInactivity time change [REG_38]\r\n");
    printf("\t     397. \tChange AC/DC mode for activity mode [REG_38][BIT_7]\r\n");
    printf("\t     393. \tChange AC/DC mode for inactivity mode [REG_38][BIT_3]\r\n");
    printf("\t  396541. \tActivity control enable [REG_39][BIT_6][BIT_5][BIT_4][ENABLE]\r\n");
    printf("\t  396540. \tActivity control disable [REG_39][BIT_6][BIT_5][BIT_4][DISABLE]\r\n");
    printf("\t  392101. \tInactivity control enable [REG_39][BIT_2][BIT_1][BIT_0][ENABLE]\r\n");
    printf("\t  392100. \tInactivity control disable [REG_39][BIT_2][BIT_1][BIT_0][DISABLE]\r\n");
    printf("\t      40. \tFreefall threshold change [REG_40]\r\n");
    printf("\t      41. \tFreefall time change [REG_41]\r\n");
    printf("\t      42. \tTAP axes control[REG_42]\r\n");
    printf("\t      44. \tBandwidth rate setup [REG_44]\r\n");
    printf("\t    4510. \tWakeUp switch state [REG_45][BIT_1][BIT_0]\r\n");
    printf("\t     452. \tSwitch sleep mode\r\n");
    printf("\t    4531. \tMeasure start [REG_45][BIT_3][ENABLE]\r\n");
    printf("\t    4530. \tMeasure stop [REG_45][BIT_3][DISABLE]\r\n");
    printf("\t     454. \tSwitch autosleep mode\r\n");
    printf("\t     455. \tSwitch linking mode\r\n");
    printf("\t     461. \tInterrupt enable [REG_46][ENABLE]\r\n");
    printf("\t     460. \tInterrupt disable [REG_46][DISABLE]\r\n");
    printf("\t      47. \tInterrupt map changing [REG_47]\r\n");
    printf("\t      48. \tGet last actived interrupt [REG_48]\r\n");
    printf("\t   49310. \tRange change [REG_49][BIT_3][BIT_1][BIT_0]\r\n");
    printf("\t     497. \tSelftest switch [REG_49][BIT_7]\r\n"); 
    printf("\t     496. \tSPI Mode switch [REG_49][BIT_6]\r\n");
    printf("\t     495. \tInterrupt invert switch [REG_49][BIT5\r\n");
    printf("\t     492. \tJustify mode switch [REG_49][BIT_2]\r\n"); 
    printf("\t     500. \tPrint data from data registers [REG_50]\r\n");
    printf("\t     501. \tPrint float data from data registers [REG_50]\r\n");
    printf("\t    5678. \tFIFO mode setup [REG_56][BIT_7][BIT_6]\r\n");
    printf("\t    5650. \tFIFO trigger setup[REG_56][BIT_5]\r\n");
    printf("\t    5651. \tFIFO trigger show current_state [REG_56][BIT_5]\r\n");
    printf("\t56432100. \tSamples set [REG_56][BIT_4][BIT_3][BIT_2][BIT_1][BIT_0]\r\n"); 
    printf("\t56432101. \tSamples get [REG_56][BIT_4][BIT_3][BIT_2][BIT_1][BIT_0]\r\n"); 
    printf("\t    5750. \tFIFO entries get [REG_57][BIT_5][BIT_4][BIT_3][BIT_2][BIT_1][BIT_0]\r\n");
    printf("\t     578. \tFIFO trigger get [REG_57][BIT_7]\r\n");

    textcolor(REVERSE, STD, STD);
    printf("======================<Show statistics>======================");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");
    printf("\t100. Dump device register space\r\n");
    printf("\t101. Get offsets X Y Z to structure\r\n");
    printf("\t102. Get ACT status\r\n");
    printf("\t103. Get TAP status\r\n");
    printf("\t104. Get SLEEP status\r\n");
    printf("\t105. Get Linking mode\r\n");
    printf("\t106. Get AutoSleep mode\r\n");
    printf("\t107. Get Sleep mode\r\n");
    printf("\t108. Get SelfTest\r\n");
    printf("\t109. Get SPI mode\r\n");
    printf("\t110. Get Interrupt inversion\r\n");
    printf("\t111. Get Justify\r\n");
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

        case 4510 : 
            status = selector_axi_adxl_set_wakeup(ptr);
        break;

        case 452 : 
            status = selector_axi_adxl_set_sleep_mode(ptr);
        break;

        case 454 : 
            status = selector_axi_adxl_set_autosleep_mode(ptr);
        break;

        case 455 : 
            status = selector_axi_adxl_set_linking_mode(ptr);
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

        case 497 : 
            status = selector_axi_adxl_selftest(ptr);
        break;

        case 496 : 
            status = selector_axi_adxl_set_spi_mode(ptr);
        break;

        case 495 : 
            status = selector_axi_adxl_invert(ptr);
        break;

        case 492 : 
            status = selector_axi_adxl_justify(ptr);
        break;


        case 29 : //0x1D
            status = selector_axi_adxl_change_thresh_tap(ptr);
        break;

        case 30 :
        	status = selector_axi_adxl_set_ofsx(ptr);
        break;

        case 31 :
        	status = selector_axi_adxl_set_ofsy(ptr);
        break;

        case 32 :
        	status = selector_axi_adxl_set_ofsz(ptr);
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

        case 397 :
            status = selector_axi_adxl_change_activity_acdc(ptr);
        break;

        case 393 : 
            status = selector_axi_adxl_change_inactivity_acdc(ptr);
        break;

        case 396541 : //0x27
        	status = selector_axi_adxl_activity_control_enable(ptr);
		break;

        case 396540 : //0x27
        	status = selector_axi_adxl_activity_control_disable(ptr);
		break;

        case 392101 :
        	status = selector_axi_adxl_inactivity_control_enable(ptr);
		break;

        case 392100 :
        	status = selector_axi_adxl_inactivity_control_disable(ptr);
		break;

        case 40 :
            status = selector_axi_adxl_change_thresh_ff(ptr);
        break;

        case 41 :
            status = selector_axi_adxl_change_time_ff(ptr);
        break;

        case 42 :
        	status = selector_axi_adxl_change_tap_axes(ptr);
		break;
        
        case 47 : 
            status = selector_axi_adxl_change_int_map(ptr);
        break;
        /*Debug device*/

        case 48 : 
            status = selector_axi_adxl_get_int_source(ptr);
        break;

        case 500 :
        	status = selector_axi_adxl_get_data(ptr);
        break;

        case 501:
        	status = selector_axi_adxl_get_data_float(ptr);
		break;

        case 5678:
            status = selector_axi_adxl_set_fifo_mode(ptr);
        break;

        case 5650 :
        	status = selector_axi_adxl_set_trigger(ptr);
		break;

        case 5651 :
        	status = selector_axi_adxl_get_trigger(ptr);
        break;

        case 56432100 : 
            status = selector_axi_adxl_set_samples(ptr);
        break;

        case 56432101 : 
            status = selector_axi_adxl_get_samples(ptr);
        break;

        case 5750: 
            status = selector_axi_adxl_get_fifo_sts_entries(ptr);
        break;

        case 578: 
            status = selector_axi_adxl_has_fifo_sts_trigger(ptr);
        break;




        case 100 :
            status = selector_axi_adxl_dev_debug_register_space(ptr);
        break;

        case 101 :
        	status = selector_axi_adxl_get_offsets(ptr);
		break;

        case 102 :
            status = selector_axi_adxl_has_act_status(ptr);
        break;
    
        case 103 :
            status = selector_axi_adxl_has_tap_status(ptr);
        break;

        case 104 : 
            status = selector_axi_adxl_has_sleep_status(ptr);
        break;

        case 105 : 
            selector_axi_adxl_has_linking_mode(ptr);
        break;

        case 106 : 
            selector_axi_adxl_has_autosleep_mode(ptr);
        break;

        case 107 : 
            selector_axi_adxl_has_sleep_mode(ptr);
        break;

        case 108 : 
            status = selector_axi_adxl_has_selftest(ptr);
        break;

        case 109 : 
            status = selector_axi_adxl_has_spi_mode(ptr);
        break;

        case 110 : 
            status = selector_axi_adxl_has_inverted(ptr);
        break;

        case 111 : 
            status = selector_axi_adxl_has_justify(ptr);
        break;


        default :
            printf("[MENU] : incorrect selection : 0x%02x\r\n", mode);
            status = ADXL_UNCORRECT_VALUE;
		break;

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




void adxl_intr_handler(void *callback){
    axi_adxl *ptr = (axi_adxl*)callback;
    uint8_t interrupt_mask;
    
    int status = axi_adxl_get_int_source(ptr, &interrupt_mask);
    if (status != ADXL_OK){
        printf("[IRQ] : bad returning status : %d", status);
    }

    adxl_data_float data;
    axi_adxl_get_data_float(ptr, &data);

    if ((axi_adxl_has_int_source(ptr, DATA_READY)) && (axi_adxl_interrupt_enabled(ptr, DATA_READY))){
        printf("[DR] ");
    }

    if ((axi_adxl_has_int_source(ptr, SINGLE_TAP)) && (axi_adxl_interrupt_enabled(ptr, SINGLE_TAP))){
        printf("[ST] ");
    }

    if ((axi_adxl_has_int_source(ptr, DOUBLE_TAP)) && (axi_adxl_interrupt_enabled(ptr, DOUBLE_TAP))){
        printf("[DT] ");
    }

    if ((axi_adxl_has_int_source(ptr, ACTIVITY)) && (axi_adxl_interrupt_enabled(ptr, ACTIVITY))){
        printf("[AC] ");
    }

    if ((axi_adxl_has_int_source(ptr, INACTIVITY)) && (axi_adxl_interrupt_enabled(ptr, INACTIVITY))){
        printf("[IA] ");
    }

    if ((axi_adxl_has_int_source(ptr, FREE_FALL)) && (axi_adxl_interrupt_enabled(ptr, FREE_FALL))){
        printf("[FF] ");
    }

    if ((axi_adxl_has_int_source(ptr, WATERMARK)) && (axi_adxl_interrupt_enabled(ptr, WATERMARK))){
        printf("[WM] ");
    }

    if ((axi_adxl_has_int_source(ptr, OVERRUN)) && (axi_adxl_interrupt_enabled(ptr, OVERRUN))){
        printf("[OV] ");
    }

    printf("X : %4.6f \tY : %4.6f \tZ : %4.6f\r\n", data.x, data.y, data.z);

    axi_adxl_irq_ack(ptr);

    return;
}
