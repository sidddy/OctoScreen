/*
 * UTouch.c
 *
 *  Created on: 30 Oct 2018
 *      Author: sven
 */

#include <stdlib.h>

#include "stm32f1xx_hal.h"
#include "cmsis_os.h"

#include "UTouch.h"

#define CMD_X 0x90
#define CMD_Y 0xD0
#define CMD_Z1 0xB0
#define CMD_Z2 0xC0
#define LOOP_CNT 10
#define MAX_TOUCH_DELTA 15
#define MIN_TOUCH_Z 400

uint16_t diff(uint16_t a, uint16_t b) { return (a < b) ? b - a : a - b; }

uint16_t disp_x_size, disp_y_size;
uint16_t scaleX, scaleY;
int16_t offsetX, offsetY;

float alpha_x;
float beta_x;
float delta_x;
float alpha_y;
float beta_y;
float delta_y;


extern SPI_HandleTypeDef hspi3;
#define hspi_touch (hspi3)

void Touch_Init(uint16_t xp, uint16_t yp)
{
	disp_x_size				= xp;
	disp_y_size				= yp;
	offsetX					= 0;
	scaleX					= (uint16_t)(((uint32_t)(disp_x_size - 1) << 16)/4096);
	offsetY					= 0;
	scaleY					= (uint16_t)(((uint32_t)(disp_y_size - 1) << 16)/4096);

	HAL_GPIO_WritePin(TOUCH_nCS_GPIO_Port, TOUCH_nCS_Pin, GPIO_PIN_RESET);

	uint8_t pTxData[3] = { 0xd4, 0, 0 };
	uint8_t pRxData[3];

	HAL_Delay(50);

	/* warmup */
	HAL_SPI_TransmitReceive(&hspi_touch, pTxData, pRxData, 3, 1000);
	pTxData[0] = 0x94;
	HAL_SPI_TransmitReceive(&hspi_touch, pTxData, pRxData, 3, 1000);
	pTxData[0] = 0x00;
	HAL_SPI_TransmitReceive(&hspi_touch, pTxData, pRxData, 3, 1000);

	HAL_GPIO_WritePin(TOUCH_nCS_GPIO_Port, TOUCH_nCS_Pin, GPIO_PIN_SET);

	float calA[2] = {20,20};
	float calB[2] = {300,100};
	float calC[2] = {20,220};

	float calA_raw[2] = {3360,660};
	float calB_raw[2] = {800,1630};
	float calC_raw[2] = {3250,3250};

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

}

void BubbleSort(uint16_t a[], int array_size) {
	int i, j, temp;
	for (i = 0; i < (array_size - 1); ++i) {
		for (j = 0; j < array_size - 1 - i; ++j ) {
			if (a[j] > a[j+1]) {
				temp = a[j+1];
				a[j+1] = a[j];
				a[j] = temp;
           }
		}
	}
}

bool Touch_Get_Filtered_Data(uint16_t* x, uint16_t* y, uint16_t* z) {
	uint16_t x1,x2,x3;
	uint16_t y1,y2,y3;
	uint16_t z1,z2,z3;

	if (Touch_Get_Data(&x1, &y1, &z1)) {
		HAL_Delay(1);
		if ((z1 > MIN_TOUCH_Z) && (Touch_Get_Data(&x2, &y2, &z2))) {
			if ((abs(x1-x2) < MAX_TOUCH_DELTA)
				&& (abs(y1-y2) < MAX_TOUCH_DELTA)) {
				HAL_Delay(1);
				if ((z2 > MIN_TOUCH_Z) && (Touch_Get_Data(&x3, &y3, &z3))) {
					if ((z3 > MIN_TOUCH_Z) && (abs(x2-x3) < MAX_TOUCH_DELTA)
						&& (abs(y2-y3) < MAX_TOUCH_DELTA)) {
						*x = x3;
						*y = y3;
						*z = (z1+z2+z3)/3;
						return true;
					}
				}
			}
		}
	}
	return false;
}

// Get data from the touch chip.
// We need to allow the touch chip ADC input to settle. See TI app note http://www.ti.com/lit/pdf/sbaa036.
bool Touch_Get_Data(uint16_t* x, uint16_t* y, uint16_t* z)
{
	if (HAL_GPIO_ReadPin(TOUCH_DI_GPIO_Port, TOUCH_DI_Pin) == GPIO_PIN_RESET) {
		uint8_t pTxData[3] = {0,0,0};//{ (uint8_t) ((wantY) ? 0xD3 : 0x93), 0, 0 };
		uint8_t pRxData[3];

		HAL_GPIO_WritePin(TOUCH_nCS_GPIO_Port, TOUCH_nCS_Pin, GPIO_PIN_RESET);

		pTxData[0] = CMD_Z1; // z1
		HAL_SPI_TransmitReceive(&hspi_touch, pTxData, pRxData, 3, 1000);

		int z1 = ((pRxData[1] * 256 + pRxData[2]) >> 3)+4095;

		pTxData[0] = CMD_Z2; // z2
		HAL_SPI_TransmitReceive(&hspi_touch, pTxData, pRxData, 3, 1000);

		int z2 = (pRxData[1] * 256 + pRxData[2]) >> 3;

		pTxData[0] = CMD_X; // x, for warmup
		HAL_SPI_TransmitReceive(&hspi_touch, pTxData, pRxData, 3, 1000);

		pTxData[0] = CMD_X; // x
		HAL_SPI_TransmitReceive(&hspi_touch, pTxData, pRxData, 3, 1000);

		int x_temp = (pRxData[1] * 256 + pRxData[2]) >> 3;

		float pressure = 1000 * (1 - ((float)z2 / (float)z1)) + 150 * (((float)(x_temp-700)) / 2800);

			*z = pressure;
			uint32_t xraw = 0;
			uint32_t yraw = 0;

			pTxData[0] = CMD_X; // x, for warmup
			HAL_SPI_TransmitReceive(&hspi_touch, pTxData, pRxData, 3, 1000);

			uint16_t readings_x[LOOP_CNT];
			uint16_t readings_y[LOOP_CNT];
			for (int i=0; i < LOOP_CNT; i++) {
				pTxData[0] = CMD_X; // x, for warmup
				HAL_SPI_TransmitReceive(&hspi_touch, pTxData, pRxData, 3, 1000);

				pTxData[0] = CMD_X; // x
				HAL_SPI_TransmitReceive(&hspi_touch, pTxData, pRxData, 3, 1000);
				readings_x[i] = (pRxData[1] * 256 + pRxData[2]) >> 3;

				pTxData[0] = CMD_Y; // y
				HAL_SPI_TransmitReceive(&hspi_touch, pTxData, pRxData, 3, 1000);
				readings_y[i] = (pRxData[1] * 256 + pRxData[2]) >> 3;
			}

			HAL_GPIO_WritePin(TOUCH_nCS_GPIO_Port, TOUCH_nCS_Pin, GPIO_PIN_SET);

			BubbleSort(readings_x, LOOP_CNT);
			BubbleSort(readings_y, LOOP_CNT);

			xraw = readings_x[LOOP_CNT/2];
			yraw = readings_y[LOOP_CNT/2];

			if ((xraw == 0) || (yraw == 0)) {
				return false;
			}

			*x = alpha_x * xraw + beta_x * yraw + delta_x;
			*y = alpha_y * xraw + beta_y * yraw + delta_y;
			//*x = xraw;
			//*y = yraw;

			return true;

	}
	return false;
}
