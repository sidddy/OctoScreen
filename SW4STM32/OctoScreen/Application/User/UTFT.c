/*
  UTFT.cpp - Arduino/chipKit library support for Color TFT LCD Boards
  Copyright (C)2010-2012 Henning Karlsen. All right reserved

  This library is the continuation of my ITDB02_Graph, ITDB02_Graph16
  and RGB_GLCD libraries for Arduino and chipKit. As the number of
  supported display modules and controllers started to increase I felt
  it was time to make a single, universal library as it will be much
  easier to maintain in the future.

  Basic functionality of this library was originally based on the
  demo-code provided by ITead studio (for the ITDB02 modules) and
  NKC Electronics (for the RGB GLCD module/shield).

  This library supports a number of 8bit, 16bit and serial graphic
  displays, and will work with both Arduino and chipKit boards. For a
  full list of tested display modules and controllers, see the
  document UTFT_Supported_display_modules_&_controllers.pdf.

  When using 8bit and 16bit display modules there are some
  requirements you must adhere to. These requirements can be found
  in the document UTFT_Requirements.pdf.
  There are no special requirements when using serial displays.

  You can always find the latest version of the library at
  http://electronics.henningkarlsen.com/

  If you make any modifications or improvements to the code, I would
  appreciate that you share the code with me so that I might include
  it in the next release. I can be contacted through
  http://electronics.henningkarlsen.com/contact.php.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "stm32f1xx_hal.h"

#include "UTFT.h"
#include "cmsis_os.h"

uint8_t LCD_Cmd_Read8(uint8_t cmd) {
	GPIO_InitTypeDef GPIO_InitStruct;

	HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_nRD_GPIO_Port, LCD_nRD_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_nWR_GPIO_Port, LCD_nWR_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_nCS_GPIO_Port, LCD_nCS_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(LCD_nWR_GPIO_Port, LCD_nWR_Pin, GPIO_PIN_RESET);

	GPIOE->ODR = cmd;

	HAL_GPIO_WritePin(LCD_nWR_GPIO_Port, LCD_nWR_Pin, GPIO_PIN_SET);

	GPIO_InitStruct.Pin = 0xFFFF;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(LCD_nRD_GPIO_Port, LCD_nRD_Pin, GPIO_PIN_RESET);

	uint16_t val = GPIOE->IDR; // dummy read

	HAL_GPIO_WritePin(LCD_nRD_GPIO_Port, LCD_nRD_Pin, GPIO_PIN_SET);
	HAL_Delay(1);

	HAL_GPIO_WritePin(LCD_nRD_GPIO_Port, LCD_nRD_Pin, GPIO_PIN_RESET);

	val = GPIOE->IDR; // real data read

	HAL_GPIO_WritePin(LCD_nRD_GPIO_Port, LCD_nRD_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(LCD_nCS_GPIO_Port, LCD_nCS_Pin, GPIO_PIN_SET);


	// Set Output mode
	GPIO_InitStruct.Pin = 0xFFFF;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	return (uint8_t) val;
}

inline void LCD_Write_COM(uint8_t VL)
{
	HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_RESET);

	GPIOE->ODR = (uint16_t)VL;
    GPIOB->BSRR = (uint32_t)LCD_nWR_Pin << 16;
    GPIOB->BSRR = LCD_nWR_Pin;
}

inline void LCD_Write_DATA16(uint16_t VHL)
{
	HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_SET);

	GPIOE->ODR = VHL;
    GPIOB->BSRR = (uint32_t)LCD_nWR_Pin << 16;
    GPIOB->BSRR = LCD_nWR_Pin;
}

inline void LCD_Write_Repeated_DATA16(uint16_t VHL, uint16_t num)
{
	HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_SET);

	GPIOE->ODR = VHL;
	while (num != 0)
	{
        GPIOB->BSRR = (uint32_t)LCD_nWR_Pin << 16;
        GPIOB->BSRR = LCD_nWR_Pin;
		--num;
	}
}

// This one is deliberately not inlined to avoid bloating the initialization code.
// Use LCD_Write_DATA16 instead where high performance is wanted.
void LCD_Write_DATA8(uint8_t VL)
{
	LCD_Write_DATA16((uint16_t)VL);
}

void LCD_Write_COM_DATA(uint8_t com1, uint16_t dat1) {
	LCD_Write_COM_DATA16(com1, dat1);
}

void LCD_Write_COM_DATA16(uint8_t com1, uint16_t dat1)
{
     LCD_Write_COM(com1);
     LCD_Write_DATA16(dat1);
}

void LCD_Write_COM_DATA8(uint8_t com1, uint8_t dat1)
{
     LCD_Write_COM(com1);
     LCD_Write_DATA8(dat1);
}

void LCD_Init()
{
	uint8_t id = LCD_Cmd_Read8(0xDC);

	if (id != 0x52)
		return;

	HAL_GPIO_WritePin(LCD_nWR_GPIO_Port, LCD_nWR_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_nRD_GPIO_Port, LCD_nRD_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_nCS_GPIO_Port, LCD_nCS_Pin, GPIO_PIN_RESET);

	LCD_Write_COM(ST7789_SWRESET);			 // Software Reset
	HAL_Delay(150);
	LCD_Write_COM(ST7789_SLPOUT);
	HAL_Delay(500);
	LCD_Write_COM_DATA8(ST7789_COLMOD, 0x55);
	LCD_Write_COM_DATA8(ST7789_MADCTL, 0x60); // MV = 1, MX = 1
	LCD_Write_COM(ST7789_CASET);
	LCD_Write_DATA8(0);
	LCD_Write_DATA8(0);
	LCD_Write_DATA8(0x01);
	LCD_Write_DATA8(0x3F);
	LCD_Write_COM(ST7789_RASET);
	LCD_Write_DATA8(0);
	LCD_Write_DATA8(0);
	LCD_Write_DATA8(0);
	LCD_Write_DATA8(239);

	LCD_Write_COM(ST7789_NORON);
	LCD_Write_COM(ST7789_DISPON);

	HAL_GPIO_WritePin(LCD_nCS_GPIO_Port, LCD_nCS_Pin, GPIO_PIN_SET);

	// turn on backlight
	HAL_GPIO_WritePin(LCD_BACKLIGHT_GPIO_Port, LCD_BACKLIGHT_Pin, GPIO_PIN_SET);

}

void LCD_Set_XY(uint16_t p_x1, uint16_t p_y1, uint16_t p_x2, uint16_t p_y2)
{
	uint16_t x1, x2, y1, y2;

    y1 = p_x1;
    y2 = p_x2;
    x1 = p_y1;
    x2 = p_y2;

    LCD_Write_COM(ST7789_RASET);
    LCD_Write_DATA16((x1>>8) & 0xFF);
    LCD_Write_DATA16(x1 & 0xFF);
    LCD_Write_DATA16((x2>>8) & 0xFF);
    LCD_Write_DATA16(x2 & 0xFF);
    LCD_Write_COM(ST7789_CASET);
    LCD_Write_DATA16((y1>>8) & 0xFF);
    LCD_Write_DATA16(y1 & 0xFF);
    LCD_Write_DATA16((y2>>8) & 0xFF);
    LCD_Write_DATA16(y2 & 0xFF);
    LCD_Write_COM(ST7789_RAMWR);

}

void LCD_Fill_Scr(Colour c)
{
	HAL_GPIO_WritePin(LCD_nCS_GPIO_Port, LCD_nCS_Pin, GPIO_PIN_RESET);

	LCD_Set_XY(0, 0, DISPLAY_X - 1, DISPLAY_Y - 1);

	LCD_Write_Repeated_DATA16(c, DISPLAY_PIXELS);

	HAL_GPIO_WritePin(LCD_nCS_GPIO_Port, LCD_nCS_Pin, GPIO_PIN_SET);
}


void LCD_Display_Flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const uint16_t * data)
{
	HAL_GPIO_WritePin(LCD_nCS_GPIO_Port, LCD_nCS_Pin, GPIO_PIN_RESET);

	LCD_Set_XY(x1,y1,x2,y2);
	int size = (x2-x1+1)*(y2-y1+1);
	HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_SET);

	for (int pos = 0; pos < size; pos++)	{
		GPIOE->ODR = data[pos];
		GPIOB->BSRR = (uint32_t)LCD_nWR_Pin << 16;
		GPIOB->BSRR = LCD_nWR_Pin;
	}

	HAL_GPIO_WritePin(LCD_nCS_GPIO_Port, LCD_nCS_Pin, GPIO_PIN_SET);
}
