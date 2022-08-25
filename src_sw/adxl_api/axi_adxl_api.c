#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "axi_adxl.h"

#define ADXL_CFG_BASEADDRESS 0x40030000
#define ADXL_DEV_BASEADDRESS 0x40040000
#define ADXL_IIC_ADDRESS     0x53



void print_menu();
int menu(axi_adxl *ptr, int mode);



int main() {
    init_platform();

    textcolor(DEFAULT, STD, STD);

    int status = 0;
    axi_adxl adxl;

    char s[256];

    while(1){
    	print_menu();

    	char *p = s;

        while((*p++=getchar ()) != 13);
        *p = '\0';
        int mode = atoi(s);

        status = menu(&adxl, mode);
        if (status != ADXL_OK){
            textcolor(BRIGHT, RED, STD);
            printf("[MAIN] : current operation performed with error %d\r\n", status);
            textcolor(DEFAULT, STD, STD);

        }

    }


    cleanup_platform();
    return 0;
}





void print_menu(){
	printf(" ***** ADXL345 DEMO APP ***** \r\n");
	printf("0. Debug log output\r\n");
	printf("1. Reset device\r\n");
	printf("2. Initialize device\r\n");
	printf("3. Single Request\r\n");
	printf("4. Enable interval requestion\r\n");
	printf("5. Disable interval requestion\r\n");
	printf("6. IRQ allow\r\n");
	printf("7. IRQ unallow\r\n");
	printf("8. Calibration\r\n");
	printf("9. Set iic address\r\n");

	printf("99. Dump device register space\r\n");
}



int menu(axi_adxl *ptr, int mode){

	int status = ADXL_OK;

	char s [256];
	char *p = s;
	int value = 0;


	switch(mode){
		case 0:
			axi_adxl_cfg_debug(ptr->cfg);
		break;

		case 1 :
			status = axi_adxl_reset(ptr);
		break;

		case 2 :
			status = axi_adxl_init(ptr, ADXL_CFG_BASEADDRESS, ADXL_DEV_BASEADDRESS, ADXL_IIC_ADDRESS);
		break;

		case 3 :
			printf("[MENU] : Enter start address : ");
			while((*p++=getchar ()) != 13);
			*p = '\0';
			uint8_t address = atoi(s);
			printf("%d\r\n", address);

			p = &s[0];

			printf("[MENU] : Enter size : ");
			while((*p++=getchar ()) != 13);
			*p = '\0';
			uint8_t size = atoi(s);
			printf("%d\r\n", size);

			status = axi_adxl_perform_single_request(ptr, address, size);
		break;

		case 4 :
			printf("[MENU] : Enter bandwidth value : ");

			while((*p++=getchar ()) != 13);
			*p = '\0';
			value = atoi(s);
			printf("%d\r\n", value);

			status = axi_adxl_perform_interval_requestion(ptr, value);
		break;

		case 5 :
			status = axi_adxl_disable_interval_requestion(ptr);
		break;

		case 6 :
			status = axi_adxl_irq_allow(ptr);
		break;

		case 7 :
			status = axi_adxl_irq_unallow(ptr);
		break;

		case 8 :
			printf("[MENU] : Enter power of 2 for calibration: ");

			while((*p++=getchar ()) != 13);
			*p = '\0';
			value = atoi(s);
			printf("%d(calibration count : %d)\r\n", value, (1<<value));

			status = axi_adxl_calibration(ptr, value);
		break;

		case 9 :
			printf("[MENU] : Enter new address of i2c device in decimal: ");
			while((*p++=getchar ()) != 13);
			*p = '\0';
			value = atoi(s);
			printf("%d (hex address : 0x%02x)\r\n", value, value);
			status = axi_adxl_set_iic_address(ptr, value);
		break;

		case 10 : 
			printf("[MENU] : Select BW_RATE from list \r\n");
			printf("\t0. 0.10 Hz \r\n");
			printf("\t1. 0.20 Hz \r\n");
			printf("\t2. 0.39 Hz \r\n");
			printf("\t3. 0.78 Hz \r\n");
			printf("\t4. 1.56 Hz \r\n");
			printf("\t5. 3.13 Hz \r\n");
			printf("\t6. 6.25 Hz \r\n");
			printf("\t7. 12.5 Hz \r\n");
			printf("\t8. 25 Hz \r\n");
			printf("\t9. 50 Hz \r\n");
			printf("\t10. 100 Hz \r\n");
			printf("\t11. 200 Hz \r\n");
			printf("\t12. 400 Hz \r\n");
			printf("\t13. 800 Hz \r\n");
			printf("\t14. 1600 Hz \r\n");
			printf("\t15. 3200 Hz \r\n");
			printf("\t16. 400 Hz low power\r\n");
			printf("\t17. 200 Hz low power\r\n");
			printf("\t18. 100 Hz low power\r\n");
			printf("\t19. 50 Hz low power\r\n");
			printf("\t20. 25 Hz low power\r\n");
			printf("\t21. 12_5 Hz low power\r\n");

			printf("Enter bandwidth value : ");
			char *p = s;

			while((*p++=getchar ()) != 13);
			*p = '\0';
			value = atoi(s);
			printf("%d\r\n", value);

			switch(value){
				case 0  : axi_adxl_set_bw_rate(ptr, BW_RATE_0_10); break;
				case 1  : axi_adxl_set_bw_rate(ptr, BW_RATE_0_20); break;
				case 2  : axi_adxl_set_bw_rate(ptr, BW_RATE_0_39); break;
				case 3  : axi_adxl_set_bw_rate(ptr, BW_RATE_0_78); break;
				case 4  : axi_adxl_set_bw_rate(ptr, BW_RATE_1_56); break;
				case 5  : axi_adxl_set_bw_rate(ptr, BW_RATE_3_13); break;
				case 6  : axi_adxl_set_bw_rate(ptr, BW_RATE_6_25); break;
				case 7  : axi_adxl_set_bw_rate(ptr, BW_RATE_12_5); break;
				case 8  : axi_adxl_set_bw_rate(ptr, BW_RATE_25); break;
				case 9  : axi_adxl_set_bw_rate(ptr, BW_RATE_50); break;
				case 10 : axi_adxl_set_bw_rate(ptr, BW_RATE_100); break;
				case 11 : axi_adxl_set_bw_rate(ptr, BW_RATE_200); break;
				case 12 : axi_adxl_set_bw_rate(ptr, BW_RATE_400); break;
				case 13 : axi_adxl_set_bw_rate(ptr, BW_RATE_800); break;
				case 14 : axi_adxl_set_bw_rate(ptr, BW_RATE_1600); break;
				case 15 : axi_adxl_set_bw_rate(ptr, BW_RATE_3200); break;
				case 16 : axi_adxl_set_bw_rate(ptr, BW_RATE_400_LP); break;
				case 17 : axi_adxl_set_bw_rate(ptr, BW_RATE_200_LP); break;
				case 18 : axi_adxl_set_bw_rate(ptr, BW_RATE_100_LP); break;
				case 19 : axi_adxl_set_bw_rate(ptr, BW_RATE_50_LP); break;
				case 20 : axi_adxl_set_bw_rate(ptr, BW_RATE_25_LP); break;
				case 21 : axi_adxl_set_bw_rate(ptr, BW_RATE_12_5_LP); break;
				default : printf("<undefined input>\r\n"); break;

			}
		break;			

		case 99 :
			axi_adxl_dev_debug_register_space(ADXL_DEV_BASEADDRESS);
		break;

		default :
			printf("[MENU] : incorrect selection : 0x%02x\r\n", mode);

	}

	return status;

}
