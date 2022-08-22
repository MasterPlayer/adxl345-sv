#include "axi_adxl.h"



void axi_adxl_cfg_debug(adxl_cfg *ptr){
    printf("[CFG SPACE]\r\n");
    printf("\t[NAME] \t\t\t\t| [RAW]\t\t| [VALUE]\r\n");
    printf("\t[VERSION] \t\t\t| 0x%08x \t| %d.%d\r\n", (((uint32_t)adxl_cfg_ctl_get_version_major(ptr) << 8) + (uint32_t)adxl_cfg_ctl_get_version_minor(ptr)), adxl_cfg_ctl_get_version_major(ptr), adxl_cfg_ctl_get_version_minor(ptr));
    
    printf("\t[RESET] \t\t\t| 0x%08x \t| ", adxl_cfg_ctl_reset_completed(ptr));
    if (adxl_cfg_ctl_reset_completed(ptr)) {
        printf("no\r\n");
    }else{
        printf("yes\r\n");
    }

    printf("\t[INTERVAL_REQUESTS] \t\t| 0x%08x \t| ", adxl_cfg_ctl_interval_requestion(ptr));
    if (adxl_cfg_ctl_interval_requestion(ptr)) {
        printf("actived\r\n");
    }else{
        printf("inactived\r\n");
    }

    printf("\t[IRQ_ALLOW] \t\t\t| 0x%08x \t| ", adxl_cfg_ctl_irq_allowed(ptr));
    if (adxl_cfg_ctl_irq_allowed(ptr)){
        printf("yes\r\n");
    }else{
        printf("no\r\n");
    }

    printf("\t[CALIBRATION] \t\t\t| 0x%08x \t| ", adxl_cfg_ctl_calibration_completed(ptr));
    if (adxl_cfg_ctl_calibration_completed(ptr)){
        printf("completed\r\n");
    }else{
        printf("not started\r\n");
    }

    printf("\t[I2C ADDRESS] \t\t\t| 0x%08x \t| 0x%02x\r\n", adxl_cfg_ctl_get_iic_address(ptr), adxl_cfg_ctl_get_iic_address(ptr));

    printf("\t[LINK] \t\t\t\t| 0x%08x \t| ", adxl_cfg_ctl_link(ptr));
    if (adxl_cfg_ctl_link(ptr)){
        printf("on\r\n");
    }else{
        printf("off\r\n");
    }

    printf("\t[WORK] \t\t\t\t| 0x%08x \t| ", adxl_cfg_ctl_work(ptr));
    if (adxl_cfg_ctl_work(ptr)){
        printf("on\r\n");
    }else{
        printf("off\r\n");
    }

    printf("\t[REQUEST_INTERVAL] \t\t| 0x%08x \t| %10.3f nanoseconds\r\n", adxl_cfg_get_request_interval(ptr), ((float)adxl_cfg_get_request_interval(ptr) * (1/(float)adxl_cfg_get_clk_period(ptr)))*1000000000 );
    printf("\t[CALIBRATION_COUNT] \t\t| 0x%08x \t| %d times\r\n", adxl_cfg_get_calibration_count_limit(ptr), (1 << adxl_cfg_get_calibration_count_limit(ptr)));
    printf("\t[READ_VALID_COUNT] \t\t| 0x%08x \t| %d bps\r\n", adxl_cfg_ctl_get_read_valid_count(ptr), adxl_cfg_ctl_get_read_valid_count(ptr));
    printf("\t[WRITE_VALID_COUNT] \t\t| 0x%08x \t| %d bps\r\n", adxl_cfg_ctl_get_write_valid_count(ptr), adxl_cfg_ctl_get_write_valid_count(ptr));
    
    uint64_t read_transactions = (((uint64_t)adxl_cfg_ctl_get_read_transactions_msb(ptr)) << 32) + (uint64_t)adxl_cfg_ctl_get_read_transactions_lsb(ptr);

    printf("\t[READ_TRANSACTIONS_LSB] \t| 0x%08x \t| %d times\r\n", adxl_cfg_ctl_get_read_transactions_lsb(ptr), read_transactions);
    printf("\t[READ_TRANSACTIONS_MSB] \t| 0x%08x \t| %d times\r\n", adxl_cfg_ctl_get_read_transactions_msb(ptr), read_transactions);
    
    printf("\t[CLK_PERIOD] \t\t\t| 0x%08x \t| %10.3f nanoseconds\r\n", adxl_cfg_get_clk_period(ptr), (1/(float)adxl_cfg_get_clk_period(ptr))*1000000000);

    uint64_t opt_request_interval = ((uint64_t)(adxl_cfg_get_opt_request_interval_msb_reg(ptr) & CFG_OPT_REQUEST_INTERVAL_MSB_MASK) << 32) + (uint64_t)adxl_cfg_get_opt_request_interval_lsb_reg(ptr);

    printf("\t[OPT_REQUEST_INTERVAL_LSB] \t| 0x%08x \t| %3.9f seconds\r\n", adxl_cfg_get_opt_request_interval_lsb_reg(ptr), ((double)opt_request_interval/(double)adxl_cfg_get_clk_period(ptr)));
    printf("\t[OPT_REQUEST_INTERVAL_MSB] \t| 0x%08x \t| %3.9f seconds\r\n", adxl_cfg_get_opt_request_interval_msb_reg(ptr), ((double)opt_request_interval/(double)adxl_cfg_get_clk_period(ptr)));
    printf("\t[DATA_WIDTH] \t\t\t| 0x%08x \t| %d bits\r\n", adxl_cfg_get_data_width_reg(ptr), adxl_cfg_get_data_width_reg(ptr));

    uint64_t calibration_time = (((uint64_t)adxl_cfg_get_calibration_time_msb_reg(ptr))<< 32) + (uint64_t)adxl_cfg_get_calibration_time_lsb_reg(ptr) ; 

    printf("\t[CALIBRATION_TIME_LSB] \t\t| 0x%08x \t| %3.9f seconds\r\n", adxl_cfg_get_calibration_time_lsb_reg(ptr), ((double)calibration_time/(double)adxl_cfg_get_clk_period(ptr)));
    printf("\t[CALIBRATION_TIME_MSB] \t\t| 0x%08x \t| %3.9f seconds\r\n", adxl_cfg_get_calibration_time_msb_reg(ptr), ((double)calibration_time/(double)adxl_cfg_get_clk_period(ptr)));

    uint64_t write_transactions = (((uint64_t)adxl_cfg_get_write_transactions_msb_reg(ptr)) << 32) + (uint64_t)adxl_cfg_get_write_transactions_lsb_reg(ptr); 

    printf("\t[WRITE_TRANSACTIONS_LSB] \t| 0x%08x \t| %d times \r\n", adxl_cfg_get_write_transactions_lsb_reg(ptr), write_transactions);
    printf("\t[WRITE_TRANSACTIONS_MSB] \t| 0x%08x \t| %d times \r\n", adxl_cfg_get_write_transactions_msb_reg(ptr), write_transactions);

    printf("\r\n");

}




int axi_adxl_init(axi_adxl *ptr, uint32_t baseaddr_cfg, uint32_t baseaddr_dev, uint8_t iic_address){

    printf("\t[ADXL_INIT] : set address to pointers\r\n");
    ptr->cfg = (adxl_cfg*)baseaddr_cfg;
    printf("\t\tconfiguration address space : 0x%08x\r\n", baseaddr_dev);

    printf("\t[ADXL_INIT] : reset internal logic\r\n");    
    adxl_cfg_ctl_reset(ptr->cfg);

    printf("\t[ADXL_INIT] : set iic address for device : 0x%02x\r\n", iic_address);    
    adxl_cfg_ctl_set_iic_address(ptr->cfg, iic_address);

    printf("\t[ADXL_INIT] : perform single request\r\n");    
    adxl_cfg_ctl_single_request(ptr->cfg);

    printf("\t[ADXL_INIT] : link ");
    if (!adxl_cfg_ctl_link(ptr->cfg)) {
        printf("lost\r\n");
        return ADXL_LINK_LOST;
    }
    printf("established\r\n");

    ptr->init_flaq = 1;

    return ADXL_OK;
}



int axi_adxl_reset(axi_adxl *ptr){



    return ADXL_OK;
}



int axi_adxl_perform_interval_requestion(axi_adxl *ptr, uint32_t requestion_interval){



    return ADXL_OK;
}



int axi_adxl_perform_single_request(axi_adxl *ptr){



    return ADXL_OK;
}



int axi_adxl_irq_allow(axi_adxl *ptr){



    return ADXL_OK;
}



int axi_adxl_irq_ack(axi_adxl *ptr){



    return ADXL_OK;
}



int axi_adxl_calibration(axi_adxl *ptr, uint32_t calibration_count_limit){



    return ADXL_OK;
}


