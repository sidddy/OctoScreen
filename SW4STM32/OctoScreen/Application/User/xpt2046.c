/*
 * xpt2046.c
 *
 *  Created on: 18 Jul 2019
 *      Author: Sven Henkel
 */

#include <stdlib.h>

#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "xpt2046.h"

uint16_t xpt2046_width, xpt2046_height;

float alpha_x;
float beta_x;
float delta_x;
float alpha_y;
float beta_y;
float delta_y;
float calc_z1;
float calc_z2;

extern SPI_HandleTypeDef hspi3;
#define hspi_touch (hspi3)

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

uint16_t XPT2026_Read_Raw(uint8_t coor);

void XPT2046_Touch_Init(uint16_t xp, uint16_t yp)
{
	xpt2046_width      = xp;
	xpt2046_height     = yp;

	XPT2026_Read_Raw(XPT2046_COOR_X);

	float zn = 100;

    float calA[3] = {20,20,zn};
	float calB[3] = {300,100,zn};
	float calC[3] = {170,220,zn};

	float calA_raw[3] = {3565, 532, 705};
	float calB_raw[3] = {750, 1600, 2000};
	float calC_raw[3] = {2030, 3300, 1800};

	float delta = (calA_raw[0]-calC_raw[0])*(calB_raw[1]-calC_raw[1]) -
	       (calB_raw[0]-calC_raw[0])*(calA_raw[1]-calC_raw[1]);
	alpha_x = (float)((calA[0]-calC[0])*(calB_raw[1]-calC_raw[1]) -
	       (calB[0]-calC[0])*(calA_raw[1]-calC_raw[1])) / delta;
	beta_x = (float)((calA_raw[0]-calC_raw[0])*(calB[0]-calC[0]) -
	       (calB_raw[0]-calC_raw[0])*(calA[0]-calC[0])) / delta;
	delta_x = ((uint64_t)calA[0]*(calB_raw[0]*calC_raw[1]-calC_raw[0]*calB_raw[1]) -
	       (uint64_t)calB[0]*(calA_raw[0]*calC_raw[1]-calC_raw[0]*calA_raw[1]) +
	       (uint64_t)calC[0]*(calA_raw[0]*calB_raw[1]-calB_raw[0]*calA_raw[1])) / delta;
	alpha_y = (float)((calA[1]-calC[1])*(calB_raw[1]-calC_raw[1]) -
	       (calB[1]-calC[1])*(calA_raw[1]-calC_raw[1])) / delta;
	beta_y = (float)((calA_raw[0]-calC_raw[0])*(calB[1]-calC[1]) -
	       (calB_raw[0]-calC_raw[0])*(calA[1]-calC[1])) / delta;
	delta_y = ((uint64_t)calA[1]*(calB_raw[0]*calC_raw[1]-calC_raw[0]*calB_raw[1]) -
	       (uint64_t)calB[1]*(calA_raw[0]*calC_raw[1]-calC_raw[0]*calA_raw[1]) +
	       (uint64_t)calC[1]*(calA_raw[0]*calB_raw[1]-calB_raw[0]*calA_raw[1])) / delta;

	float xr1 = calA_raw[0];
	float xr2 = calC_raw[0];
	float yr1 = calA_raw[1];
	float yr2 = calC_raw[1];
	float zr1 = calA_raw[2];
	float zr2 = calC_raw[2];

	calc_z1 = ((zn/(zr1*xr1)) - ((zn*yr1)/(zr2*xr1*yr2))) * (1 / (1 - (xr2*yr1) / (yr2*xr1)));
	calc_z2 = (zn/(zr2*yr2)) - (calc_z1*xr2)/yr2;
}

uint16_t XPT2026_Read_Raw(uint8_t coor) {
	uint8_t com = coor | XPT2046_CONTROL | XPT2046_DFR_MODE;

	uint16_t data[3];

	uint8_t pTxData[3] = {0,0,0};
	uint8_t pRxData[3] = {0,0,0};

	HAL_GPIO_WritePin(TOUCH_nCS_GPIO_Port, TOUCH_nCS_Pin, GPIO_PIN_RESET);

	pTxData[0] = com;

	for (uint16_t i = 0; i < 3 ; i++) {
		uint16_t cur = 65535;
		for (uint16_t j = 0; j < 100; j++) {
			pRxData[0] = 0;
			pRxData[1] = 0;
			pRxData[2] = 0;

			HAL_SPI_TransmitReceive(&hspi_touch, pTxData, pRxData, 3, 1000);
			uint16_t val = ((pRxData[1] * 256 + pRxData[2]) >> 3);
			if (abs((int32_t)val-(int32_t)cur) < 50) {
				cur = val;
				break;
			}
			cur = val;
			if (j == 50)
				cur = 12345;
		}
		data[i] = cur;
	}

	HAL_GPIO_WritePin(TOUCH_nCS_GPIO_Port, TOUCH_nCS_Pin, GPIO_PIN_SET);

    uint16_t delta01 = MAX(data[0], data[1]) - MIN(data[0], data[1]),
	         delta02 = MAX(data[0], data[2]) - MIN(data[0], data[2]),
	         delta12 = MAX(data[1], data[2]) - MIN(data[1], data[2]);

    uint16_t ret;

	if (delta01 <= delta02 && delta01 <= delta12) {
	    ret = (data[0] + data[1]) >> 1;
	} else if (delta02 <= delta12) {
	    ret = (data[0] + data[2]) >> 1;
	} else {
		ret = (data[1] + data[2]) >> 1;
	}

	if (ret > 4095)
		ret = 0;

	return ret;
}

void XPT2046_Touch_Get_Raw_Data(uint16_t* x, uint16_t* y, uint16_t* z) {
	*x = XPT2026_Read_Raw(XPT2046_COOR_X);
	*y = XPT2026_Read_Raw(XPT2046_COOR_Y);
	*z = XPT2026_Read_Raw(XPT2046_COOR_Z1) + (4095 - XPT2026_Read_Raw(XPT2046_COOR_Z2));
}

bool XPT2046_Touch_Get_Data(uint16_t* x, uint16_t* y, uint16_t* z){
	uint16_t z1_raw = XPT2026_Read_Raw(XPT2046_COOR_Z1);
	uint16_t z2_raw = XPT2026_Read_Raw(XPT2046_COOR_Z2);
	uint16_t z_raw = z1_raw + (4095 - z2_raw);

	uint16_t xraw = XPT2026_Read_Raw(XPT2046_COOR_X);
	uint16_t yraw = XPT2026_Read_Raw(XPT2046_COOR_Y);

	if ((z1_raw == 0) || (z2_raw == 0) || (xraw == 0) || (yraw == 0) || (xraw == 4095) || (yraw == 4095) || (z1_raw == 4095))
		return false;

	uint16_t z_norm = (float)z_raw * (float)((float)xraw * calc_z1 + (float)yraw * calc_z2);

	if (z_norm > 70) {

		if ((xraw > 0) && (yraw > 0) && (xraw < 4095) && (yraw < 4095)) {
			*x = alpha_x * xraw + beta_x * yraw + delta_x;
			*y = alpha_y * xraw + beta_y * yraw + delta_y;
			*z = z_norm;
			return true;
		}
	}
	return false;
}


