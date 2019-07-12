/*
 * UTouch.c
 *
 *  Created on: 30 Oct 2018
 *      Author: sven
 */


#include "stm32f1xx_hal.h"
#include "cmsis_os.h"

#include "UTouch.h"

uint16_t diff(uint16_t a, uint16_t b) { return (a < b) ? b - a : a - b; }

uint16_t disp_x_size, disp_y_size;
uint16_t scaleX, scaleY;
int16_t offsetX, offsetY;

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

	osDelay(50);

	/* warmup */
	HAL_SPI_TransmitReceive(&hspi_touch, pTxData, pRxData, 3, 1000);
	pTxData[0] = 0x94;
	HAL_SPI_TransmitReceive(&hspi_touch, pTxData, pRxData, 3, 1000);

	HAL_GPIO_WritePin(TOUCH_nCS_GPIO_Port, TOUCH_nCS_Pin, GPIO_PIN_SET);
}

// If the panel is touched, return the coordinates in x and y and return true; else return false
bool Touch_Read(uint16_t* px, uint16_t* py, uint16_t* rawX, uint16_t* rawY)
{
	bool ret = false;

	if (HAL_GPIO_ReadPin(TOUCH_DI_GPIO_Port, TOUCH_DI_Pin) == GPIO_PIN_RESET)	// if screen is touched
	{
		HAL_GPIO_WritePin(TOUCH_nCS_GPIO_Port, TOUCH_nCS_Pin, GPIO_PIN_RESET);

		osDelay(1); // allow the screen to settle
		uint16_t tx;

		if (Touch_Get_Data(false, &tx))
		{
			uint16_t ty;

			if (Touch_Get_Data(true, &ty))
			{
				if (HAL_GPIO_ReadPin(TOUCH_DI_GPIO_Port, TOUCH_DI_Pin) == GPIO_PIN_RESET)
				{
					/*int16_t valx = (orientAdjust & SwapXY) ? ty : tx;
					if (orientAdjust & ReverseX)
					{
						valx = 4095 - valx;
					}*/
					int16_t valx = tx;

					int16_t cx = (int16_t)(((uint32_t)valx * (uint32_t)scaleX) >> 16) - offsetX;
					*px = (cx < 0) ? 0 : (cx >= disp_x_size) ? disp_x_size - 1 : (uint16_t)cx;

					/*int16_t valy = (orientAdjust & SwapXY) ? tx : ty;
					if (orientAdjust & ReverseY)
					{
						valy = 4095 - valy;
					}*/
					int16_t valy = ty;

					int16_t cy = (int16_t)(((uint32_t)valy * (uint32_t)scaleY) >> 16) - offsetY;
					*py = (cy < 0) ? 0 : (cy >= disp_y_size) ? disp_y_size - 1 : (uint16_t)cy;
					if (rawX != NULL)
					{
						*rawX = valx;
					}
					if (rawY != NULL)
					{
						*rawY = valy;
					}
					ret = true;
				}
			}
		}

		HAL_GPIO_WritePin(TOUCH_nCS_GPIO_Port, TOUCH_nCS_Pin, GPIO_PIN_SET);
	}
	return ret;
}

// Get data from the touch chip. CS has already been set low.
// We need to allow the touch chip ADC input to settle. See TI app note http://www.ti.com/lit/pdf/sbaa036.
bool Touch_Get_Data(bool wantY, uint16_t* rslt)
{
	uint8_t pTxData[3] = { (uint8_t) ((wantY) ? 0xD3 : 0x93), 0, 0 };
	uint8_t pRxData1[3];
	uint8_t pRxData2[3];
	uint8_t pRxData3[3];

	HAL_GPIO_WritePin(TOUCH_nCS_GPIO_Port, TOUCH_nCS_Pin, GPIO_PIN_RESET);

	pTxData[0] = 0xB3; // z1
	HAL_StatusTypeDef res1 = HAL_SPI_TransmitReceive(&hspi_touch, pTxData, pRxData1, 3, 1000);

	pTxData[0] = 0xC3; // z2
	HAL_StatusTypeDef res2 = HAL_SPI_TransmitReceive(&hspi_touch, pTxData, pRxData2, 3, 1000);

	int z1 = ((pRxData1[1] * 256 + pRxData1[2]) >> 3)+4095;
	int z2 = (pRxData2[1] * 256 + pRxData2[2]) >> 3;

	//pTxData[0] = 0x81;
	//HAL_StatusTypeDef res3 = HAL_SPI_TransmitReceive(&hspi_touch, pTxData, pRxData3, 3, 1000);

	HAL_GPIO_WritePin(TOUCH_nCS_GPIO_Port, TOUCH_nCS_Pin, GPIO_PIN_SET);

	if (z1-z2 > 500) {
		return true;
	}

	return false;

	//	int16_t z1 = SPI.transfer16(0xC1 /* Z2 */) >> 3;
		//int z = z1 + 4095;
		//int16_t z2 = SPI.transfer16(0x91 /* X */) >> 3;
		//z -= z2;


	/* warmup */
/*	HAL_StatusTypeDef res = HAL_SPI_Transmit(&hspi_touch, pTxData, 1, 1000);

	const size_t numReadings = 6;
	const uint16_t maxDiff = 70;					// needs to be big enough to handle jitter.
													// 8 was OK for the 4.3 and 5 inch displays but not the 7 inch.
													// 25 is OK for most 7" displays.
	const unsigned int maxAttempts = 40;

	uint16_t ring[numReadings];
	uint32_t sum = 0;

	// Take enough readings to fill the ring buffer
	for (size_t i = 0; i < numReadings; ++i)
	{
		HAL_SPI_TransmitReceive(&hspi_touch, pTxData, pRxData, 3, 1000);
		uint16_t val = (((uint16_t)pRxData[1] << 8) + pRxData[2]) >> 3;

		ring[i] = val;
		sum += val;
	}

	// Test whether every reading is within 'maxDiff' of the average reading.
	// If it is, return the average reading.
	// If not, take another reading and try again, up to 'maxAttempts' times.
	uint16_t avg;
	size_t last = 0;
	bool ok;
	for (unsigned int i = 0; i < maxAttempts; ++i)
	{
		avg = (uint16_t)(sum/numReadings);
		ok = true;
		for (size_t i = 0; ok && i < numReadings; ++i)
		{
			if (diff(avg, ring[i]) > maxDiff)
			{
				ok = false;
				last = i;
				break;
			}
		}
		if (ok)
		{
			break;
		}

		// Take another reading
		sum -= ring[last];

		HAL_SPI_TransmitReceive(&hspi_touch, pTxData, pRxData, 3, 1000);
		uint16_t val = (((uint16_t)pRxData[1] << 8) + pRxData[2]) >> 3;

		ring[last] = val;
		sum += val;
		last = (last + 1) % numReadings;
	}

	pTxData[0] &= 0xF8;
	HAL_SPI_Transmit(&hspi_touch, pTxData, 3, 1000);
	pTxData[0] = 0;
	HAL_SPI_Transmit(&hspi_touch, pTxData, 3, 1000);	// read the final data
	*rslt = avg;
	return ok;*/
}
/*
void UTouch::calibrate(uint16_t xlow, uint16_t xhigh, uint16_t ylow, uint16_t yhigh, uint16_t margin)
{
	scaleX = (uint16_t)(((uint32_t)(disp_x_size - 1 - 2 * margin) << 16)/(xhigh - xlow));
	offsetX = (int16_t)(((uint32_t)xlow * (uint32_t)scaleX) >> 16) - (int16_t)margin;
	scaleY = (uint16_t)(((uint32_t)(disp_y_size - 1 - 2 * margin) << 16)/(yhigh - ylow));
	offsetY = (int16_t)(((uint32_t)ylow * (uint32_t)scaleY) >> 16) - (int16_t)margin;
}*/

// End
