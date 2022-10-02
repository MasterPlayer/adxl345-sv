#include "axi_adxl.h"
#include <math.h>
//
//enum output_rule_enum{
//	XYZ_INTEGER,
//	XYZ_GRAVITY,
//	ROLL_PITCH
//};

int axi_adxl_set_output_rule(axi_adxl *ptr, enum output_rule_enum output_rule){
	if (!axi_adxl_has_init(ptr)){
#ifdef AXI_ADXL_LOGGING_SW
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_OUTPUT_RULE] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	if ((output_rule != XYZ_INTEGER) && (output_rule != XYZ_GRAVITY) && (output_rule != ROLL_PITCH)){
#ifdef AXI_ADXL_LOGGING_SW
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_SET_OUTPUT_RULE] : uncorrect selection : %d", output_rule);
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
	return ADXL_UNCORRECT_VALUE;
	}

	ptr->output_rule = output_rule;

	return ADXL_OK;
}



int axi_adxl_get_output_rule(axi_adxl *ptr, int *output_rule){

	if (!axi_adxl_has_init(ptr)){
#ifdef AXI_ADXL_LOGGING_SW
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_OUTPUT_RULE] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	*output_rule = ptr->output_rule;

	return ADXL_OK;
}



int axi_adxl_is_output_rule(axi_adxl *ptr, enum output_rule_enum output_rule){
	return ((ptr->output_rule == output_rule) ? TRUE : FALSE);
}


int axi_adxl_print(axi_adxl *ptr){
	int output_rule;
	adxl_data data;
    adxl_data_float data_float;
    adxl_data_pitch_roll pitch_roll;

	int status = axi_adxl_get_output_rule(ptr, &output_rule);


	switch(output_rule){
		case XYZ_INTEGER:
			status = axi_adxl_get_data(ptr, &data);
			if (status != ADXL_OK){
				return status;
			}
		    printf("X : %5d \tY : %5d \tZ : %d\r\n", data.x, data.y, data.z);
		break;

		case XYZ_GRAVITY:
		    status = axi_adxl_get_data_float(ptr, &data_float);
			if (status != ADXL_OK){
				return status;
			}
		    printf("X : %4.6f \tY : %4.6f \tZ : %4.6f\r\n", data_float.x, data_float.y, data_float.z);
		break;

		case ROLL_PITCH :
			status = axi_adxl_get_pitch_roll(ptr, &pitch_roll);
			if (status != ADXL_OK){
				return status;
			}
			printf("\t%3.2f \t%3.2f\r\n", pitch_roll.pitch, pitch_roll.roll);
		break;

		default :
			return ADXL_UNCORRECT_VALUE;
	}


	return status;
}



int axi_adxl_get_pitch_roll(axi_adxl *ptr, adxl_data_pitch_roll *pitch_roll_ptr){

	if (!axi_adxl_has_init(ptr)){
#ifdef AXI_ADXL_LOGGING_SW
		textcolor(DEFAULT, RED, STD);
		printf("\t\t[ADXL_GET_PITCH_ROLL] : unitialized software structure");
		textcolor(DEFAULT, STD, STD);
		printf("\r\n");
#endif
		return ADXL_UNINIT;
	}

	adxl_data_float gravity;
	int status = axi_adxl_get_data_float(ptr, &gravity);
	pitch_roll_ptr->roll = atan(gravity.y/ sqrt(pow(gravity.x, 2) + pow(gravity.z, 2))) * 180 / PI;
	pitch_roll_ptr->pitch = atan(-1 * gravity.x / sqrt(pow(gravity.y, 2) + pow(gravity.z, 2))) * 180 / PI;
	return status;
}



