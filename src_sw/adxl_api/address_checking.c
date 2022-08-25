#include "axi_adxl.h"


int check_access_rw(int address){
	int flaq = 0;
	for (int i = 0; i < ADXL_DEV_RW_COUNT; i++){
		if (address == rw_address_const[i]){
			flaq = 1;
		}
	}
	return flaq;
}



int check_access_ro(int address){
	int flaq = 0;
	for (int i = 0; i < ADXL_DEV_RO_COUNT; i++){
		if (address == ro_address_const[i]){
			flaq = 1;
		}
	}
	return flaq;
}



int check_access_reserved(int address){
	int flaq = 0;
	for (int i = 0; i < ADXL_DEV_RESERVED_COUNT; i++){
		if (address == reserved_address_const[i]){
			flaq = 1;
		}
	}
	return flaq;
}
