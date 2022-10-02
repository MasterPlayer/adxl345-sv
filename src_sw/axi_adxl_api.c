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

const char* error_parser[] = {
    "SUCCESS",
    "UNINITIALIZED",
    "LINK LOST",
    "INFINITE RESET",
    "UNCOMPLETE REQUEST",
    "CANNOT STOP",
    "UNCORRECT VALUE",
    "TIMEOUT",
    "NON_FUNCTION"
};

const char* function_list[] = {
    "CFG_DBG",              //0
    "CFG_RST",              //1
    "CFG_INIT",             //2
    "SNGL_REQ",             //3
    "INTRVL_REQ_EN",        //4
    "INTRVL_REQ_DSBL",      //5
    "SWITCH_IRQ",           //6
    "CALIB",                //7
    "SET_IIC_ADDR",         //8
    "SET_REQ_INTRVL",       //11
    "SET_STARTADDR",
    "SET_SIZE",
    "SET_CALIB_CNT",

    "SET_THRESHOLD_TAP", // 290
    "GET_THRESHOLD_TAP", // 291
    "SET_OFFSET_X", // 300
    "GET_OFFSET_X", // 301
    "SET_OFFSET_Y", // 310
    "GET_OFFSET_Y", // 311
    "SET_OFFSET_Z", // 320
    "GET_OFFSET_Z", // 321
    "GET_OFFSETS_TO_STRUCTURE", // 322
    "SET_DURATION",  // 330
    "GET_TDURATION", // 331
    "SET_LATENCY", // 340
    "GET_LATENCY", // 341
    "SET_WINDOW", // 350
    "GET_WINDOW", // 351
    "SET_ACTIVITY_THRESHOLD", // 360
    "GET_ACTIVITY_THRESHOLD", // 361
    "SET_INACTIVITY_THRESHOLD", // 370
    "GET_INACTIVITY_THRESHOLD", // 371
    "SET_INACTIVITY_TIME", // 380
    "GET_INACTIVITY_TIME", // 381
    "SET_ACDC_ACTIVITY_MODE", // 390
    "GET_ACDC_ACTIVITY_MODE", // 391
    "SET_ACDC_INACTIVITY_MODE", // 392
    "GET_ACDC_INACTIVITY_MODE", // 393
    "ENABLE_ACTIVITY_CONTROL", // 394
    "DISABLE_ACTIVITY_CONTROL", // 395
    "ENABLE_INACTIVITY_CONTROL", // 396
    "DISABLE_INACTIVITY_CONTROL", // 397
    "SET_FREEFALL_THRESHOLD", // 400
    "GET_FREEFALL_THRESHOLD", // 401
    "SET_FREEFALL_TIME", // 410
    "GET_FREEFALL_TIME", // 411
    "SWITCH_TAP_AXES", // 420
    "GET_TAP_AXES", // 421
    "GET_ASLEEP_STATUS", // 430
    "GET_ACT_STATUS", // 431
    "GET_TAP_STATUS", // 432
    "SET_BW_RATE", // 440
    "GET_BW_RATE", // 441
    "SET_WAKEUP", // 450
    "GET_WAKEUP", // 451
    "SWITCH_SLEEP_MODE", // 452
    "START_MEASURE",  // 453
    "STOP_MEASURE",  // 454
    "SWITCH_AUTOSLEEP_MODE", // 455
    "SWITCH_LINKING_MODE", // 456
    "GET_LINKING_MODE", // 457
    "GET_SLEEP_MODE", // 458
    "GET_AUTOSLEEP_MODE", // 459
    "ENABLE_INTERRUPT",  // 461
    "DISABLE_INTERRUPT",   // 460
    "SET_INT_MAP",  // 470
    "GET_INT_MAP",  // 471
    "GET_LAST_ACTIVED_INTERRUPT",   // 480
    "SET_RANGE",   // 490
    "SWITCH_SELFTEST",  // 491
    "SWITCH_SPI_MODE",  // 492
    "SWITCH_INTERRUPT_INVERT",  // 493
    "SWITCH_JUSTIFY_MODE",  // 494
    "GET_SELFTEST", // 495
    "GET_SPI_MODE", // 496
    "GET_JUSTIFY", // 497
    "GET_INTERRUPT_INVERSION", // 498
    "PRINT_DATA_FROM_DATA_REGISTERS",  // 500
    "PRINT_FLOAT_DATA_FROM_DATA_REGISTERS",  // 501
    "SET_FIFO_MODE",  // 560
    "SET_FIFO_TRIGGER",   // 561
    "SET_SAMPLES",  // 562
    "GET_FIFO_MODE",  // 563
    "GET_FIFO_TRIGGER",  // 564
    "GET_SAMPLES",  // 565
    "GET_FIFO_ENTRIES",  // 570
    "GET_FIFO_TRIGGER",  // 571
    "DUMP_DEVICE_REGISTER_SPACE", // 100
    "DEBUG_MODE", // 120
	"SET_OUTPUT_RULES", // 121
	"GET_OUTPUT_RULES" //122
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
        int address_number = extract_function_list_address(mode);
        int status_err = status * -1;
        if (status != ADXL_OK) {
            textcolor(DEFAULT, STD, STD);
            if (status != ADXL_NON_FUNCTION) {
                printf("[MAIN] : <%s> current operation performed with error ", function_list[address_number]);
            }
            else {
                printf("[MAIN] : <nan> current operation performed with error ");
            }
            textcolor(DEFAULT, BLACK, RED);
            int status_err = status * -1;
            printf("<%d> : [%s]", status, error_parser[status_err]);
            textcolor(DEFAULT, STD, STD);
            printf("\r\n");
        }
        else {
            printf("[MAIN] : <%s> : [%s]\r\n", function_list[address_number], error_parser[status_err]);
        }

        printf("[MAIN] : Press Any Key for continue\r\n");

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
    printf("\t6. Switch IRQ allow statement\r\n");
    printf("\t7. Calibration\r\n");
    printf("\t8. Set iic address\r\n");
    printf("\t11. Set request interval\r\n");
    printf("\t15. Set startaddr \r\n");
    printf("\t17. Set size\r\n");
    printf("\t24. Set power of calibration count\r\n");


    textcolor(REVERSE, STD, STD);
    printf("=======================<Device space>========================");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");
    printf("\t     290. \tSet Threshold tap\r\n");
    printf("\t     291. \tGet Threshold tap\r\n");
    printf("\t     300. \tSet offset X\r\n");
    printf("\t     301. \tGet offset X\r\n");
    printf("\t     310. \tSet offset Y\r\n");
    printf("\t     311. \tGet offset Y\r\n");
    printf("\t     320. \tSet offset Z\r\n");
    printf("\t     321. \tGet offset Z\r\n");
    printf("\t     322. \tGet offsets X Y Z to structure\r\n");
    printf("\t     330. \tSet Duration \r\n");
    printf("\t     331. \tGet tDuration\r\n");
    printf("\t     340. \tSet Latency\r\n");
    printf("\t     341. \tGet Latency\r\n");
    printf("\t     350. \tSet Window\r\n");
    printf("\t     351. \tGet Window\r\n");
    printf("\t     360. \tSet Activity threshold\r\n");
    printf("\t     361. \tGet Activity threshold\r\n");
    printf("\t     370. \tSet Inactivity threshold\r\n");
    printf("\t     371. \tGet Inactivity threshold\r\n");
    printf("\t     380. \tSet Inactivity time\r\n");
    printf("\t     381. \tGet Inactivity time\r\n");
    printf("\t     390. \tSet AC/DC activity mode\r\n");
    printf("\t     391. \tGet AC/DC activity mode\r\n");
    printf("\t     392. \tSet AC/DC inactivity mode\r\n");
    printf("\t     393. \tSet AC/DC inactivity mode\r\n");
    printf("\t     394. \tEnable Activity control\r\n");
    printf("\t     395. \tDisable Activity control\r\n");
    printf("\t     396. \tEnable Inactivity control\r\n");
    printf("\t     397. \tDisable Inactivity control\r\n");
    printf("\t     400. \tSet Freefall threshold\r\n");
    printf("\t     401. \tGet Freefall threshold\r\n");
    printf("\t     410. \tSet Freefall time\r\n");
    printf("\t     411. \tGet Freefall time\r\n");
    printf("\t     420. \tSwitch TAP axes\r\n");
    printf("\t     421. \tGet TAP axes\r\n");
    printf("\t     430. \tGet ASLEEP status\r\n");
    printf("\t     431. \tGet ACT status\r\n");
    printf("\t     432. \tGet TAP status\r\n");
    printf("\t     440. \tSet bw_rate\r\n");
    printf("\t     441. \tGet bw_rate\r\n");
    printf("\t     450. \tSet WakeUp\r\n");
    printf("\t     451. \tGet WakeUp\r\n");
    printf("\t     452. \tSwitch sleep mode\r\n");
    printf("\t     453. \tStart Measure \r\n");
    printf("\t     454. \tStop Measure \r\n");
    printf("\t     455. \tSwitch autosleep mode\r\n");
    printf("\t     456. \tSwitch linking mode\r\n");
    printf("\t     457. \tGet Linking mode\r\n");
    printf("\t     458. \tGet Sleep mode\r\n");
    printf("\t     459. \tGet AutoSleep mode\r\n");
    printf("\t     461. \tEnable Interrupt \r\n");
    printf("\t     460. \tDisable Interrupt  \r\n");
    printf("\t     470. \tSet int map \r\n");
    printf("\t     471. \tGet int map \r\n");
    printf("\t     480. \tGet last actived interrupt  \r\n");
    printf("\t     490. \tSet Range  \r\n");
    printf("\t     491. \tSwitch Selftest \r\n");
    printf("\t     492. \tSwitch SPI Mode \r\n");
    printf("\t     493. \tSwitch Interrupt invert \r\n");
    printf("\t     494. \tSwitch Justify mode \r\n");
    printf("\t     495. \tGet SelfTest\r\n");
    printf("\t     496. \tGet SPI mode\r\n");
    printf("\t     497. \tGet Justify\r\n");
    printf("\t     498. \tGet Interrupt inversion\r\n");
    printf("\t     500. \tPrint data from data registers \r\n");
    printf("\t     501. \tPrint float data from data registers \r\n");
    printf("\t     560. \tSet FIFO mode \r\n");
    printf("\t     561. \tSet FIFO trigger  \r\n");
    printf("\t     562. \tSet Samples \r\n");
    printf("\t     563. \tGet FIFO mode \r\n");
    printf("\t     564. \tGet FIFO trigger \r\n");
    printf("\t     565. \tGet Samples \r\n");
    printf("\t     570. \tGet FIFO entries \r\n");
    printf("\t     571. \tGet FIFO trigger \r\n");

    textcolor(REVERSE, STD, STD);
    printf("======================<Show statistics>======================");
    textcolor(DEFAULT, STD, STD);
    printf("\r\n");
    printf("\t100. Dump device register space\r\n");
    printf("\t120. Debug mode\r\n");
    printf("\t121. Set output rule\r\n");
    printf("\t122. Get output rule\r\n");



}



int menu(axi_adxl *ptr, int mode){

    int status = ADXL_NON_FUNCTION;

    switch(mode){


        case 0: status = selector_axi_adxl_cfg_debug(ptr); break;
        case 1 : status = selector_axi_adxl_reset(ptr); break;
        case 2 : status = selector_axi_adxl_init(ptr); break;
        case 3 : status = selector_axi_adxl_perform_single_request(ptr); break;
        case 4 : status = selector_axi_adxl_perform_interval_requestion(ptr); break;
        case 5 : status = selector_axi_adxl_disable_interval_requestion(ptr); break;
        case 6 : status = selector_axi_adxl_switch_irq_allow(ptr); break;
        case 7 : status = selector_axi_adxl_calibration(ptr); break;
        case 8 : status = selector_axi_adxl_set_iic_address(ptr); break;
        case 11 : status = selector_axi_adxl_set_requestion_interval(ptr); break;
        case 15: status = selector_axi_adxl_set_start_address(ptr); break;
        case 17: status = selector_axi_adxl_set_size(ptr); break;
        case 24: status = selector_axi_adxl_set_calibration_pwr_count_limit(ptr); break;


        //case 9:
        //    status = selector_axi_adxl_get_iic_address(ptr);
        //break;

        //case 10:
        //    status = selector_axi_adxl_get_requestion_interval(ptr);
        //break;

        //case 12:
        //    status = selector_axi_adxl_has_irq_allow(ptr);
        //break;

        //case 13:
        //    status = selector_axi_adxl_has_interval_requestion(ptr);
        //break;

        //case 14:
        //    status = selector_axi_adxl_has_reset(ptr);
        //break;

        //case 16:
        //    status = selector_axi_adxl_get_start_address(ptr);
        //break;

        //case 18:
        //    status = selector_axi_adxl_get_size(ptr);
        //break;

        //case 19:
        //    status = selector_axi_adxl_has_single_request_completed(ptr);
        //break;

        //case 20:
        //    status = selector_axi_adxl_has_work(ptr);
        //break;

        //case 21:
        //    status = selector_axi_adxl_has_link(ptr);
        //break;

        //case 22:
        //    status = selector_axi_adxl_get_version(ptr);
        //break;

        //case 23:
        //    status = selector_axi_adxl_get_calibration_pwr_count_limit(ptr);
        //break;

        //case 25:
        //    status = selector_axi_adxl_has_calibration_in_progress(ptr);
        //break;

        //case 26:
        //    status = selector_axi_adxl_has_calibration_complete(ptr);
        //break;

        /*Work with device*/
        // Possible masks : [REGISTER_NUMBER][BIT_NUMBER][BIT_STATE]
        // Possible masks : [REGISTER_NUMBER][REGISTER_STATE]
        // Possible masks : [REGISTER_NUMBER][BIT_NUMBERS]


        case 290 : status = selector_axi_adxl_set_thresh_tap(ptr); break;
        case 291 : status = selector_axi_adxl_get_thresh_tap(ptr); break;

        case 300 : status = selector_axi_adxl_set_ofsx(ptr); break;
        case 301 : status = selector_axi_adxl_get_ofsx(ptr); break;

        case 310 : status = selector_axi_adxl_set_ofsy(ptr); break;
        case 311 : status = selector_axi_adxl_get_ofsy(ptr); break;

        case 320 : status = selector_axi_adxl_set_ofsz(ptr); break;
        case 321 : status = selector_axi_adxl_get_ofsz(ptr); break;
        case 322 : status = selector_axi_adxl_get_offsets(ptr); break;

        case 330 : status = selector_axi_adxl_set_duration(ptr); break;
        case 331 : status = selector_axi_adxl_get_duration(ptr); break;

        case 340 : status = selector_axi_adxl_set_latent(ptr); break;
        case 341 : status = selector_axi_adxl_get_latent(ptr); break;

        case 350 : status = selector_axi_adxl_set_window(ptr); break;
        case 351 : status = selector_axi_adxl_get_window(ptr); break;

        case 360 : status = selector_axi_adxl_set_thresh_act(ptr); break;
        case 361 : status = selector_axi_adxl_get_thresh_act(ptr); break;

        case 370 : status = selector_axi_adxl_set_thresh_inact(ptr); break;
        case 371 : status = selector_axi_adxl_get_thresh_inact(ptr); break;

        case 380 : status = selector_axi_adxl_set_time_inact(ptr); break;
        case 381 : status = selector_axi_adxl_get_time_inact(ptr); break;

        case 390 : status = selector_axi_adxl_set_activity_acdc(ptr); break;
        case 391 : status = selector_axi_adxl_get_activity_acdc(ptr); break;
        case 392 : status = selector_axi_adxl_set_inactivity_acdc(ptr); break;
        case 393 : status = selector_axi_adxl_get_inactivity_acdc(ptr); break;
        case 394 : status = selector_axi_adxl_activity_control_enable(ptr); break;
        case 395 : status = selector_axi_adxl_activity_control_disable(ptr); break;
        case 396 : status = selector_axi_adxl_inactivity_control_enable(ptr); break;
        case 397 : status = selector_axi_adxl_inactivity_control_disable(ptr); break;

        case 400 : status = selector_axi_adxl_set_thresh_ff(ptr); break;
        case 401 : status = selector_axi_adxl_get_thresh_ff(ptr); break;

        case 410 : status = selector_axi_adxl_set_time_ff(ptr); break;
        case 411 : status = selector_axi_adxl_get_time_ff(ptr); break;

        case 420 : status = selector_axi_adxl_set_tap_axes(ptr); break;
        case 421 : status = selector_axi_adxl_get_tap_axes(ptr); break;

        case 430 : status = selector_axi_adxl_has_asleep_status(ptr); break;
        case 431 : status = selector_axi_adxl_is_act_status(ptr); break;
        case 432 : status = selector_axi_adxl_is_tap_status(ptr); break;

        case 440 : status = selector_axi_adxl_set_bw_rate(ptr); break;
        case 441 : status = selector_axi_adxl_get_bw_rate(ptr); break;

        case 450 : status = selector_axi_adxl_set_wakeup(ptr); break;
        case 451 : status = selector_axi_adxl_get_wakeup(ptr); break;
        case 452 : status = selector_axi_adxl_set_sleep_mode(ptr); break;
        case 453 : status = selector_axi_adxl_measurement_start(ptr); break;
        case 454 : status = selector_axi_adxl_measurement_stop(ptr);  break;
        case 455 : status = selector_axi_adxl_set_autosleep_mode(ptr); break;
        case 456 : status = selector_axi_adxl_set_linking_mode(ptr); break;
        case 457 : status = selector_axi_adxl_has_linking_mode(ptr); break;
        case 458 : status = selector_axi_adxl_has_sleep_mode(ptr); break;
        case 459 : status = selector_axi_adxl_has_autosleep_mode(ptr); break;

        case 460 : status = selector_axi_adxl_interrupt_disable(ptr); break;
        case 461 : status = selector_axi_adxl_interrupt_enable(ptr); break;

        case 470 : status = selector_axi_adxl_set_int_map(ptr); break;
        case 471 : status = selector_axi_adxl_get_int_map(ptr); break;

        case 480 : status = selector_axi_adxl_get_int_source(ptr); break;
        
        case 490 : status = selector_axi_adxl_change_range(ptr); break;
        case 491 : status = selector_axi_adxl_selftest(ptr); break;
        case 492 : status = selector_axi_adxl_set_spi_mode(ptr); break;
        case 493 : status = selector_axi_adxl_set_invert(ptr); break;
        case 494 : status = selector_axi_adxl_justify(ptr); break;
        case 495 : status = selector_axi_adxl_has_selftest(ptr); break;
        case 496 : status = selector_axi_adxl_has_spi_mode(ptr); break;
        case 497 : status = selector_axi_adxl_has_justify(ptr); break;
        case 498 : status = selector_axi_adxl_has_inverted(ptr); break;

        case 500 : status = selector_axi_adxl_get_data(ptr); break;
        case 501 : status = selector_axi_adxl_get_data_float(ptr); break;

        case 560 : status = selector_axi_adxl_set_fifo_mode(ptr); break;
        case 561 : status = selector_axi_adxl_set_trigger(ptr); break;
        case 562 : status = selector_axi_adxl_set_samples(ptr); break;
        case 563 : status = selector_axi_adxl_get_fifo_mode(ptr); break;
        case 564 : status = selector_axi_adxl_get_trigger(ptr); break;
        case 565 : status = selector_axi_adxl_get_samples(ptr); break;

        case 570 : status = selector_axi_adxl_get_fifo_sts_entries(ptr); break;
        case 571 : status = selector_axi_adxl_has_fifo_sts_trigger(ptr); break;

        case 100 : status = selector_axi_adxl_dev_debug_register_space(ptr); break;

        case 120 : status = dbg_set_reg(ptr); break;
        case 121 : status = selector_axi_adxl_set_output_rules(ptr); break;
        case 122 : status = selector_axi_adxl_get_output_rules(ptr); break;

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




void adxl_intr_handler(void *callback){
    axi_adxl *ptr = (axi_adxl*)callback;
    uint8_t interrupt_mask;
    
    int status = axi_adxl_get_int_source(ptr, &interrupt_mask);
    if (status != ADXL_OK){
        printf("[IRQ] : bad returning status : %d", status);
    }

     if ((axi_adxl_is_int_source(ptr, DATA_READY)) && (axi_adxl_is_int_enable(ptr, DATA_READY))){
        printf("[DR] ");
    }

    if ((axi_adxl_is_int_source(ptr, SINGLE_TAP)) && (axi_adxl_is_int_enable(ptr, SINGLE_TAP))){
        printf("[ST] ");
    }

    if ((axi_adxl_is_int_source(ptr, DOUBLE_TAP)) && (axi_adxl_is_int_enable(ptr, DOUBLE_TAP))){
        printf("[DT] ");
    }

    if ((axi_adxl_is_int_source(ptr, ACTIVITY)) && (axi_adxl_is_int_enable(ptr, ACTIVITY))){
        printf("[AC] ");
    }

    if ((axi_adxl_is_int_source(ptr, INACTIVITY)) && (axi_adxl_is_int_enable(ptr, INACTIVITY))){
        printf("[IA] ");
    }

    if ((axi_adxl_is_int_source(ptr, FREE_FALL)) && (axi_adxl_is_int_enable(ptr, FREE_FALL))){
        printf("[FF] ");
    }

    if ((axi_adxl_is_int_source(ptr, WATERMARK)) && (axi_adxl_is_int_enable(ptr, WATERMARK))){
        printf("[WM] ");
    }

    if ((axi_adxl_is_int_source(ptr, OVERRUN)) && (axi_adxl_is_int_enable(ptr, OVERRUN))){
        printf("[OV] ");
    }

    axi_adxl_print(ptr);

    axi_adxl_irq_ack(ptr);

    return;
}
