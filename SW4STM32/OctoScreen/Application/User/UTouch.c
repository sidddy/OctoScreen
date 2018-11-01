/*
 * UTouch.c
 *
 *  Created on: 30 Oct 2018
 *      Author: sven
 */


#include "stm32f1xx_hal.h"
#include "cmsis_os.h"

#include "UTouch.h"

#define CMD_X 0x91
#define CMD_Y 0xD1
#define CMD_Z1 0xB1
#define CMD_Z2 0xC1

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

	HAL_GPIO_WritePin(TOUCH_nCS_GPIO_Port, TOUCH_nCS_Pin, GPIO_PIN_SET);

	float calA[2] = {30,30};
	float calB[2] = {300,100};
	float calC[2] = {150,220};

	float calA_raw[2] = {3451,651};
	float calB_raw[2] = {812,1924};
	float calC_raw[2] = {2100,3188};

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

static int16_t besttwoavg( int16_t x , int16_t y , int16_t z ) {
  int16_t da, db, dc;
  int16_t reta = 0;
  if ( x > y ) da = x - y; else da = y - x;
  if ( x > z ) db = x - z; else db = z - x;
  if ( z > y ) dc = z - y; else dc = y - z;

  if ( da <= db && da <= dc ) reta = (x + y) >> 1;
  else if ( db <= da && db <= dc ) reta = (x + z) >> 1;
  else reta = (y + z) >> 1;   //    else if ( dc <= da && dc <= db ) reta = (x + y) >> 1;

  return (reta);
}

// Get data from the touch chip. CS has already been set low.
// We need to allow the touch chip ADC input to settle. See TI app note http://www.ti.com/lit/pdf/sbaa036.
bool Touch_Get_Data(uint16_t* x, uint16_t* y, uint16_t* z)
{
	uint8_t pTxData[3] = {0,0,0};//{ (uint8_t) ((wantY) ? 0xD3 : 0x93), 0, 0 };
	uint8_t pRxData[3];

	int16_t data[6];


	HAL_GPIO_WritePin(TOUCH_nCS_GPIO_Port, TOUCH_nCS_Pin, GPIO_PIN_RESET);

	pTxData[0] = CMD_Z1; // z1
	HAL_StatusTypeDef res = HAL_SPI_TransmitReceive(&hspi_touch, pTxData, pRxData, 3, 1000);
	int z1 = ((pRxData[1] * 256 + pRxData[2]) >> 3)+4095;

	pTxData[0] = CMD_Z2; // z2
	res = HAL_SPI_TransmitReceive(&hspi_touch, pTxData, pRxData, 3, 1000);

	int z2 = (pRxData[1] * 256 + pRxData[2]) >> 3;

	//pTxData[0] = 0x81;
	//HAL_StatusTypeDef res3 = HAL_SPI_TransmitReceive(&hspi_touch, pTxData, pRxData3, 3, 1000);

	if (z1-z2 > 700) {
		*z = z1 - z2;
		pTxData[0] = CMD_X; // x, for warmup
		HAL_SPI_TransmitReceive(&hspi_touch, pTxData, pRxData, 3, 1000);

		pTxData[0] = CMD_X; // x
		HAL_SPI_TransmitReceive(&hspi_touch, pTxData, pRxData, 3, 1000);
		data[0] = (pRxData[1] * 256 + pRxData[2]) >> 3;

		pTxData[0] = CMD_Y; // y
		HAL_SPI_TransmitReceive(&hspi_touch, pTxData, pRxData, 3, 1000);
		data[1] = (pRxData[1] * 256 + pRxData[2]) >> 3;

		pTxData[0] = CMD_X; // x
		HAL_SPI_TransmitReceive(&hspi_touch, pTxData, pRxData, 3, 1000);
		data[2] = (pRxData[1] * 256 + pRxData[2]) >> 3;

		pTxData[0] = CMD_Y; // y
		HAL_SPI_TransmitReceive(&hspi_touch, pTxData, pRxData, 3, 1000);
		data[3] = (pRxData[1] * 256 + pRxData[2]) >> 3;

		pTxData[0] = CMD_X; // x
		HAL_SPI_TransmitReceive(&hspi_touch, pTxData, pRxData, 3, 1000);
		data[4] = (pRxData[1] * 256 + pRxData[2]) >> 3;

		pTxData[0] = CMD_Y; // y
		HAL_SPI_TransmitReceive(&hspi_touch, pTxData, pRxData, 3, 1000);
		data[5] = (pRxData[1] * 256 + pRxData[2]) >> 3;

		uint16_t xraw = besttwoavg( data[0], data[2], data[4] );
		uint16_t yraw = besttwoavg( data[1], data[3], data[5] );

		*x = alpha_x * xraw + beta_x * yraw + delta_x;
		*y = alpha_y * xraw + beta_y * yraw + delta_y;


		HAL_GPIO_WritePin(TOUCH_nCS_GPIO_Port, TOUCH_nCS_Pin, GPIO_PIN_SET);
		return true;
	}

	HAL_GPIO_WritePin(TOUCH_nCS_GPIO_Port, TOUCH_nCS_Pin, GPIO_PIN_SET);
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
