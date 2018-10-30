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

#include <cstring>			// for strchr
#include "stm32f1xx_hal.h"

#include "UTFT.h"
//#include "Icons.h"

#define MKS_TFT
#define ILI9325

/*const ColourScheme colourSchemes[2] =
{
	// Light colour schema. As this one comes first, it is the default.
	{
		.index = 0,
		.pal = IconPaletteLight,

		.titleBarTextColour = white,
		.titleBarBackColour = red,
		.labelTextColour = black,
		.infoTextColour = black,
		.infoBackColour = lightBlue,
		.defaultBackColour = lightGrey,
		.activeBackColour = lightRed,
		.standbyBackColour = lightYellow,
		.tuningBackColour = lightGreen,
		.errorTextColour = white,
		.errorBackColour = magenta,

		.popupBorderColour = black,
		.popupBackColour = lightBlue,
		.popupTextColour = black,
		.popupButtonTextColour = black,
		.popupButtonBackColour = white,
		.popupInfoTextColour = black,
		.popupInfoBackColour = white,

		.alertPopupBackColour = lightGreen,
		.alertPopupTextColour = black,

		.buttonTextColour = black,
		.buttonPressedTextColour = black,
		.buttonTextBackColour = white,
		.buttonImageBackColour = white,
		.buttonGradColour = UTFT::fromRGB(255-8-1, 255-4-1, 255-8),
		.buttonPressedBackColour = lightGreen,
		.buttonPressedGradColour = UTFT::fromRGB(255-8-1, 255-8-1, 255-8),
		.buttonBorderColour = black,
		.homedButtonBackColour = lightBlue,
		.notHomedButtonBackColour = lightOrange,
		.pauseButtonBackColour = lightOrange,
		.resumeButtonBackColour = lightYellow,
		.resetButtonBackColour = lightRed,

		.progressBarColour = midGreen,
		.progressBarBackColour = white,

		.stopButtonTextColour = white,
		.stopButtonBackColour = UTFT::fromRGB(255, 32, 32)			// need enough G and B to allow for the gradient
	},

	// Dark colour scheme

	{
		.index = 1,
		.pal = IconPaletteDark,

		.titleBarTextColour = white,
		.titleBarBackColour = darkRed,
		.labelTextColour = white,
		.infoTextColour = white,
		.infoBackColour = darkBlue,
		.defaultBackColour = black,
		.activeBackColour = darkRed,
		.standbyBackColour = yellow,
		.tuningBackColour = darkGreen,
		.errorTextColour = white,
		.errorBackColour = magenta,

		.popupBorderColour = white,
		.popupBackColour = darkBlue,
		.popupTextColour = white,
		.popupButtonTextColour = white,
		.popupButtonBackColour = black,
		.popupInfoTextColour = white,
		.popupInfoBackColour = black,

		.alertPopupBackColour = darkGreen,
		.alertPopupTextColour = white,

		.buttonTextColour = white,
		.buttonPressedTextColour = white,
		.buttonTextBackColour = black,
		.buttonImageBackColour = grey,
		.buttonGradColour = 0,	//UTFT::fromRGB(8, 4, 8),
		.buttonPressedBackColour = darkGreen,
		.buttonPressedGradColour = 0,	//UTFT::fromRGB(8, 8, 8),
		.buttonBorderColour = white,
		.homedButtonBackColour = blue,
		.notHomedButtonBackColour = lightOrange,
		.pauseButtonBackColour = darkOrange,
		.resumeButtonBackColour = darkYellow,
		.resetButtonBackColour = darkRed,

		.progressBarColour = midGreen,
		.progressBarBackColour = black,

		.stopButtonTextColour = white,
		.stopButtonBackColour = red
	}
};*/

template <class T> inline void swap(T& a, T& b)
{
	T temp = a;
	a = b;
	b = temp;
}

uint8_t UTFT::LCD_Cmd_Read8(uint8_t cmd) {
	GPIO_InitTypeDef GPIO_InitStruct;

	HAL_GPIO_WritePin(LCD_DCX_GPIO_Port, LCD_DCX_Pin, GPIO_PIN_RESET);
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

	HAL_GPIO_WritePin(LCD_DCX_GPIO_Port, LCD_DCX_Pin, GPIO_PIN_SET);

	HAL_GPIO_WritePin(LCD_nRD_GPIO_Port, LCD_nRD_Pin, GPIO_PIN_RESET);

	uint16_t val = GPIOE->IDR; // dummy read

	HAL_GPIO_WritePin(LCD_nRD_GPIO_Port, LCD_nRD_Pin, GPIO_PIN_SET);
	delay(1);

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

uint16_t UTFT::LCD_Read_REG(uint8_t reg) {
	GPIO_InitTypeDef GPIO_InitStruct;

	HAL_GPIO_WritePin(LCD_nCS_GPIO_Port, LCD_nCS_Pin, GPIO_PIN_RESET);
	LCD_Write_COM(reg);
	HAL_GPIO_WritePin(LCD_DCX_GPIO_Port, LCD_DCX_Pin, GPIO_PIN_SET);




	// Set Input mode
	/*GPIO_InitStruct.Pin = 0xFFFF;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);*/

	//HAL_Delay(5);

	HAL_GPIO_WritePin(LCD_nRD_GPIO_Port, LCD_nRD_Pin, GPIO_PIN_RESET);

//	HAL_Delay(5);

	uint16_t reg_val = GPIOE->IDR;

	HAL_GPIO_WritePin(LCD_nRD_GPIO_Port, LCD_nRD_Pin, GPIO_PIN_SET);

	// Set Output mode
	/*GPIO_InitStruct.Pin = 0xFFFF;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);*/


	HAL_GPIO_WritePin(LCD_nCS_GPIO_Port, LCD_nCS_Pin, GPIO_PIN_SET);
	return reg_val;
}

#if defined(STM32F107xC) && defined(MKS_TFT)
inline void UTFT::LCD_Write_COM(uint8_t VL)
{
	HAL_GPIO_WritePin(LCD_DCX_GPIO_Port, LCD_DCX_Pin, GPIO_PIN_RESET);

	GPIOE->ODR = (uint16_t)VL;
    GPIOB->BSRR = (uint32_t)LCD_nWR_Pin << 16;
    GPIOB->BSRR = LCD_nWR_Pin;
}

void UTFT::LCD_Write_DATA(uint8_t VH, uint8_t VL) {
	uint16_t VHL = (uint16_t)VH*256 + VL;
	LCD_Write_DATA16(VHL);
}

void UTFT::LCD_Write_DATA(int v) {
	LCD_Write_DATA16(v);
}

inline void UTFT::LCD_Write_DATA16(uint16_t VHL)
{
	HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_SET);

	GPIOE->ODR = VHL;
    GPIOB->BSRR = (uint32_t)LCD_nWR_Pin << 16;
    GPIOB->BSRR = LCD_nWR_Pin;
}

inline void UTFT::LCD_Write_Repeated_DATA16(uint16_t VHL, uint16_t num)
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

#elif defined(STM32F103xE) && defined(CZMINI)

#endif

// Write the data num1 * num2 times, where num1 >= 1
void UTFT::LCD_Write_Repeated_DATA16(uint16_t VHL, uint16_t num1, uint16_t num2)
{
	while (num2 != 0)
	{
		LCD_Write_Repeated_DATA16(VHL, num1);
		--num2;
	}
}

// This one is deliberately not inlined to avoid bloating the initialization code.
// Use LCD_Write_DATA16 instead where high performance is wanted.
void UTFT::LCD_Write_DATA8(uint8_t VL)
{
	LCD_Write_DATA16((uint16_t)VL);
}

void UTFT::LCD_Write_COM_DATA(uint8_t com1, uint16_t dat1) {
	LCD_Write_COM_DATA16(com1, dat1);
}

void UTFT::LCD_Write_COM_DATA16(uint8_t com1, uint16_t dat1)
{
     LCD_Write_COM(com1);
     LCD_Write_DATA16(dat1);
}

void UTFT::LCD_Write_COM_DATA8(uint8_t com1, uint8_t dat1)
{
     LCD_Write_COM(com1);
     LCD_Write_DATA8(dat1);
}

void UTFT::delay(int ms) {
	HAL_Delay(ms);
}

void UTFT::InitLCD(DisplayOrientation po)
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

void UTFT::setXY(uint16_t p_x1, uint16_t p_y1, uint16_t p_x2, uint16_t p_y2)
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

void UTFT::drawRect(int x1, int y1, int x2, int y2)
{
	if (x1 > x2)
	{
		swap(x1, x2);
	}
	if (y1 > y2)
	{
		swap(y1, y2);
	}

	drawHLine(x1, y1, x2-x1);
	drawHLine(x1, y2, x2-x1);
	drawVLine(x1, y1, y2-y1);
	drawVLine(x2, y1, y2-y1);
}

void UTFT::drawRoundRect(int x1, int y1, int x2, int y2)
{
	if (x1>x2)
	{
		swap(x1, x2);
	}
	if (y1>y2)
	{
		swap(y1, y2);
	}
	if ((x2-x1) > 4 && (y2-y1) > 4)
	{
		drawPixel(x1+1, y1+1);
		drawPixel(x2-1, y1+1);
		drawPixel(x1+1, y2-1);
		drawPixel(x2-1, y2-1);
		drawHLine(x1+2, y1, x2-x1-4);
		drawHLine(x1+2, y2, x2-x1-4);
		drawVLine(x1, y1+2, y2-y1-4);
		drawVLine(x2, y1+2, y2-y1-4);
	}
}

// Apply the specified gradient to the foreground colour, but avoid wrap-round
inline void UTFT::applyGradient(uint16_t grad)
{
	fcolour += grad;
}

void UTFT::fillRect(int x1, int y1, int x2, int y2, Colour grad, uint8_t gradChange)
{
	if (x1>x2)
	{
		swap(x1, x2);
	}
	if (y1>y2)
	{
		swap(y1, y2);
	}

	Colour fcolourSave = fcolour;
	uint8_t gradCount = 0;
	if (orient & SwapXY)
	{
		for (int i = x1; i <= x2; i++)
		{
			drawVLine(i, y1, y2-y1);
			if (++gradCount == gradChange)
			{
				gradCount = 0;
				applyGradient(grad);
			}
		}
	}
	else
	{
		for (int i = y1; i <= y2; i++)
		{
			drawHLine(x1, i, x2-x1);
			if (++gradCount == gradChange)
			{
				gradCount = 0;
				applyGradient(grad);
			}
		}
	}
	fcolour = fcolourSave;
}

void UTFT::fillRoundRect(int x1, int y1, int x2, int y2, Colour grad, uint8_t gradChange)
{
	if (x1>x2)
	{
		swap(x1, x2);
	}
	if (y1>y2)
	{
		swap(y1, y2);
	}

	if ((x2-x1) > 4 && (y2-y1) > 4)
	{
		Colour fcolourSave = fcolour;
		uint8_t gradCount = 0;

		drawHLine(x1+2, y1, x2-x1-4);
		++y1;
		if (++gradCount == gradChange)
		{
			gradCount = 0;
			applyGradient(grad);
		}
		drawHLine(x1+1, y1, x2-x1-2);
		++y1;
		if (++gradCount == gradChange)
		{
			gradCount = 0;
			applyGradient(grad);
		}
		while (y1 + 1 < y2)
		{
			drawHLine(x1, y1, x2-x1);
			++y1;
			if (++gradCount == gradChange)
			{
				gradCount = 0;
				applyGradient(grad);
			}
		}
		drawHLine(x1+1, y1, x2-x1-2);
		++y1;
		if (++gradCount == gradChange)
		{
			gradCount = 0;
			applyGradient(grad);
		}
		drawHLine(x1+2, y1, x2-x1-4);

		fcolour = fcolourSave;
	}
}

void UTFT::drawCircle(int x, int y, int radius)
{
	int f = 1 - radius;
	int ddF_x = 1;
	int ddF_y = -2 * radius;
	int x1 = 0;
	int y1 = radius;

#if defined(STM32F107xC) && defined(MKS_TFT)
	HAL_GPIO_WritePin(LCD_nCS_GPIO_Port, LCD_nCS_Pin, GPIO_PIN_RESET);
#endif
	setXY(x, y + radius, x, y + radius);
	LCD_Write_DATA16(fcolour);
	setXY(x, y - radius, x, y - radius);
	LCD_Write_DATA16(fcolour);
	setXY(x + radius, y, x + radius, y);
	LCD_Write_DATA16(fcolour);
	setXY(x - radius, y, x - radius, y);
	LCD_Write_DATA16(fcolour);

	while(x1 < y1)
	{
		if(f >= 0)
		{
			y1--;
			ddF_y += 2;
			f += ddF_y;
		}
		x1++;
		ddF_x += 2;
		f += ddF_x;
		setXY(x + x1, y + y1, x + x1, y + y1);
		LCD_Write_DATA16(fcolour);
		setXY(x - x1, y + y1, x - x1, y + y1);
		LCD_Write_DATA16(fcolour);
		setXY(x + x1, y - y1, x + x1, y - y1);
		LCD_Write_DATA16(fcolour);
		setXY(x - x1, y - y1, x - x1, y - y1);
		LCD_Write_DATA16(fcolour);
		setXY(x + y1, y + x1, x + y1, y + x1);
		LCD_Write_DATA16(fcolour);
		setXY(x - y1, y + x1, x - y1, y + x1);
		LCD_Write_DATA16(fcolour);
		setXY(x + y1, y - x1, x + y1, y - x1);
		LCD_Write_DATA16(fcolour);
		setXY(x - y1, y - x1, x - y1, y - x1);
		LCD_Write_DATA16(fcolour);
	}
#if defined(STM32F107xC) && defined(MKS_TFT)
	HAL_GPIO_WritePin(LCD_nCS_GPIO_Port, LCD_nCS_Pin, GPIO_PIN_SET);
#endif
}

void UTFT::fillCircle(int x, int y, int radius)
{
	int f = 1 - radius;
	int ddF_x = 1;
	int ddF_y = -2 * radius;
	int x1 = 0;
	int y1 = radius;
#if defined(STM32F107xC) && defined(MKS_TFT)
	HAL_GPIO_WritePin(LCD_nCS_GPIO_Port, LCD_nCS_Pin, GPIO_PIN_RESET);
#endif
	setXY(x, y + radius, x, y + radius);
	LCD_Write_DATA16(fcolour);
	setXY(x, y - radius, x, y - radius);
	LCD_Write_DATA16(fcolour);
	setXY(x - radius, y, x + radius, y);
	LCD_Write_Repeated_DATA16(fcolour, radius + radius);

	while(x1 < y1)
	{
		if(f >= 0)
		{
			y1--;
			ddF_y += 2;
			f += ddF_y;
		}
		x1++;
		ddF_x += 2;
		f += ddF_x;
		setXY(x - x1, y + y1, x + x1, y + y1);
		LCD_Write_Repeated_DATA16(fcolour, x1 + x1);
		setXY(x - x1, y - y1, x + x1, y - y1);
		LCD_Write_Repeated_DATA16(fcolour, x1 + x1);
		setXY(x - y1, y + x1, x + y1, y + x1);
		LCD_Write_Repeated_DATA16(fcolour, y1 + y1);
		setXY(x - y1, y - x1, x + y1, y - x1);
		LCD_Write_Repeated_DATA16(fcolour, y1 + y1);
	}
#if defined(STM32F107xC) && defined(MKS_TFT)
	HAL_GPIO_WritePin(LCD_nCS_GPIO_Port, LCD_nCS_Pin, GPIO_PIN_SET);
#endif
}

void UTFT::fillScr(Colour c, uint16_t leftMargin)
{
	HAL_GPIO_WritePin(LCD_nCS_GPIO_Port, LCD_nCS_Pin, GPIO_PIN_RESET);

	setXY(leftMargin, 0, getDisplayXSize() - 1, getDisplayYSize() - 1);

	LCD_Write_Repeated_DATA16(c, getDisplayXSize() - leftMargin, getDisplayYSize());

	HAL_GPIO_WritePin(LCD_nCS_GPIO_Port, LCD_nCS_Pin, GPIO_PIN_SET);
}

void UTFT::drawPixel(int x, int y)
{
#if defined(STM32F107xC) && defined(MKS_TFT)
	HAL_GPIO_WritePin(LCD_nCS_GPIO_Port, LCD_nCS_Pin, GPIO_PIN_RESET);
#endif
	setXY(x, y, x, y);
	LCD_Write_DATA16(fcolour);
#if defined(STM32F107xC) && defined(MKS_TFT)
	HAL_GPIO_WritePin(LCD_nCS_GPIO_Port, LCD_nCS_Pin, GPIO_PIN_SET);
#endif
}

void UTFT::drawLine(int x1, int y1, int x2, int y2)
{
	if (y1 == y2)
	{
		if (x1 > x2)
		{
			swap(x1, x2);
		}
		drawHLine(x1, y1, x2-x1);
	}
	else if (x1==x2)
	{
		if (y1>y2)
		{
			swap(y1, y2);
		}
		drawVLine(x1, y1, y2-y1);
	}
	else
	{
		// Draw a line using the Bresenham Algorithm (thanks Wikipedia)
		int dx = (x2 >= x1) ? x2 - x1 : x1 - x2;
		int dy = (y2 >= y1) ? y2 - y1 : y1 - y2;
		int sx = (x1 < x2) ? 1 : -1;
		int sy = (y1 < y2) ? 1 : -1;
		int err = dx - dy;
#if defined(STM32F107xC) && defined(MKS_TFT)
		HAL_GPIO_WritePin(LCD_nCS_GPIO_Port, LCD_nCS_Pin, GPIO_PIN_RESET);
#endif
		for (;;)
		{
			setXY(x1, y1, x1, y1);
			LCD_Write_DATA16(fcolour);
			if (x1 == x2 && y1 == y2) break;
			int e2 = err + err;
			if (e2 > -dy)
			{
				err -= dy;
				x1 += sx;
			}
			if (e2 < dx)
			{
				err += dx;
				y1 += sy;
			}
		}
#if defined(STM32F107xC) && defined(MKS_TFT)
		HAL_GPIO_WritePin(LCD_nCS_GPIO_Port, LCD_nCS_Pin, GPIO_PIN_SET);
#endif
	}
}

void UTFT::drawHLine(int x, int y, int len)
{
#if defined(STM32F107xC) && defined(MKS_TFT)
	HAL_GPIO_WritePin(LCD_nCS_GPIO_Port, LCD_nCS_Pin, GPIO_PIN_RESET);
#endif
	setXY(x, y, x+len, y);
	LCD_Write_Repeated_DATA16(fcolour, len + 1);
#if defined(STM32F107xC) && defined(MKS_TFT)
	HAL_GPIO_WritePin(LCD_nCS_GPIO_Port, LCD_nCS_Pin, GPIO_PIN_SET);
#endif
}

void UTFT::drawVLine(int x, int y, int len)
{
#if defined(STM32F107xC) && defined(MKS_TFT)
	HAL_GPIO_WritePin(LCD_nCS_GPIO_Port, LCD_nCS_Pin, GPIO_PIN_RESET);
#endif
	setXY(x, y, x, y+len);
	LCD_Write_Repeated_DATA16(fcolour, len + 1);
#if defined(STM32F107xC) && defined(MKS_TFT)
	HAL_GPIO_WritePin(LCD_nCS_GPIO_Port, LCD_nCS_Pin, GPIO_PIN_SET);
#endif
}

// New print functions
void UTFT::setTextPos(uint16_t x, uint16_t y, uint16_t rm)
{
	textXpos = x;
	textYpos = y;
	const uint16_t xSize = getDisplayXSize();
	textRightMargin = (rm > xSize) ? xSize : rm;
    lastCharColData = 0UL;    // flag that we just set the cursor position, so no space before next character
}

size_t UTFT::print(const char *s, uint16_t x, uint16_t y, uint16_t rm)
{
	setTextPos(x, y, rm);
	return 0;//Print::print(s);
}

void UTFT::clearToMargin()
{
	if (textXpos < textRightMargin)
	{
		uint16_t ySize = (uint16_t)cfont.ySize();
		if (textYpos + ySize > getDisplayYSize())
		{
			ySize = getDisplayYSize() - textYpos;
		}
#if defined(STM32F107xC) && defined(MKS_TFT)
		HAL_GPIO_WritePin(LCD_nCS_GPIO_Port, LCD_nCS_Pin, GPIO_PIN_RESET);
#endif
		setXY(textXpos, textYpos, textRightMargin - 1, textYpos + ySize - 1);
		LCD_Write_Repeated_DATA16(bcolour, textRightMargin - textXpos, ySize);
#if defined(STM32F107xC) && defined(MKS_TFT)
		HAL_GPIO_WritePin(LCD_nCS_GPIO_Port, LCD_nCS_Pin, GPIO_PIN_SET);
#endif
	}
}

// Write a UTF8 byte.
// If textYpos is off the end of the display, then don't write anything, just update textXpos and lastCharColData
size_t UTFT::write(uint8_t c)
{
	if (numContinuationBytesLeft == 0)
	{
		if (c < 0x80)
		{
			return writeNative(c);
		}
		else if ((c & 0xE0) == 0xC0)
		{
			charVal = (uint32_t)(c & 0x1F);
			numContinuationBytesLeft = 1;
			return 1;
		}
		else if ((c & 0xF0) == 0xE0)
		{
			charVal = (uint32_t)(c & 0x0F);
			numContinuationBytesLeft = 2;
			return 1;
		}
		else if ((c & 0xF8) == 0xF0)
		{
			charVal = (uint32_t)(c & 0x07);
			numContinuationBytesLeft = 3;
			return 1;
		}
		else if ((c & 0xFC) == 0xF8)
		{
			charVal = (uint32_t)(c & 0x03);
			numContinuationBytesLeft = 4;
			return 1;
		}
		else if ((c & 0xFE) == 0xFC)
		{
			charVal = (uint32_t)(c & 0x01);
			numContinuationBytesLeft = 5;
			return 1;
		}
		else
		{
			return writeNative(0x7F);
		}
	}
	else if ((c & 0xC0) == 0x80)
	{
		charVal = (charVal << 6) | (c & 0x3F);
		--numContinuationBytesLeft;
		if (numContinuationBytesLeft == 0)
		{
			return writeNative(charVal);
		}
		else
		{
			return 1;
		}
	}
	else
	{
		// Bad UTF8 state
		numContinuationBytesLeft = 0;
		return writeNative(0x7F);
	}
}

// Write a character. Always returns the number of bytes consumed i.e. 1.
// If textYpos is off the end of the display, then don't write anything, just update textXpos and lastCharColData
size_t UTFT::writeNative(uint32_t c)
{
	if (translateFrom != 0)
	{
		const char* p = strchr(translateFrom, c);
		if (p != 0)
		{
			c = translateTo[p - translateFrom];
		}
	}

    const uint8_t *fontPtr;

    if (!cfont.inRange(c) || (nullptr == (fontPtr = cfont.fontPtr(c))))
		return 1;

	uint8_t ySize = cfont.ySize();
	if (textYpos >= getDisplayYSize())
	{
		ySize = 0;
	}
	else if (textYpos + ySize > getDisplayYSize())
	{
		ySize = getDisplayYSize() - textYpos;
	}
	const uint32_t cmask = (1UL << cfont.ySize()) - 1;

    uint8_t nCols = *(uint8_t*)(fontPtr++);
#if defined(STM32F107xC) && defined(MKS_TFT)
	HAL_GPIO_WritePin(LCD_nCS_GPIO_Port, LCD_nCS_Pin, GPIO_PIN_RESET);
#endif
	if (lastCharColData != 0)	// if we have written anything other than spaces
	{
		uint8_t numSpaces = cfont.spaces();

		// Decide whether to add the full number of space columns first (auto-kerning)
		// We don't add a space column before a space character.
		// We add a space column after a space character if we would have added one between the preceding and following characters.
		uint32_t thisCharColData = *(uint32_t*)(fontPtr) & cmask;    // atmega328p is little-endian
		if (thisCharColData == 0)  // for characters with deliberate space row at the start, e.g. decimal point
		{
			thisCharColData = *(uint32_t*)(fontPtr + cfont.bytesPerColumn()) & cmask;	// get the next column instead
		}

		bool kern = (numSpaces >= 2)
						? ((thisCharColData & lastCharColData) == 0)
						: (((thisCharColData | (thisCharColData << 1)) & (lastCharColData | (lastCharColData << 1))) == 0);
		if (kern)
		{
			--numSpaces;	// kern the character pair
		}
		while (numSpaces != 0 && textXpos < textRightMargin)
		{
			// Add a single space column after the character
			if (ySize != 0 && !transparentBackground)
			{
				setXY(textXpos, textYpos, textXpos, textYpos + ySize - 1);
				LCD_Write_Repeated_DATA16(bcolour, ySize);
			}
			++textXpos;
			--numSpaces;
		}
	}

    while (nCols != 0 && textXpos < textRightMargin)
    {
		uint32_t colData = *(uint32_t*)(fontPtr);
		fontPtr += cfont.bytesPerColumn();
		if (colData != 0)
		{
			lastCharColData = colData & cmask;
		}
		if (ySize != 0)
		{
			bool doSetXY = true;
            for (uint8_t i = 0; i < ySize; ++i)
            {
                if (colData & 1u)
                {
                    if (doSetXY)
                    {
                        setXY(textXpos, textYpos + i, textXpos, textYpos + ySize - 1);
                        doSetXY = false;
                    }
                    LCD_Write_DATA16(fcolour);
                }
                else if (transparentBackground)
                {
                    doSetXY = true;
                }
                else
                {
                    if (doSetXY)
                    {
                        setXY(textXpos, textYpos + i, textXpos, textYpos + ySize - 1);
                        doSetXY = false;
                    }
                    LCD_Write_DATA16(bcolour);
                }
                colData >>= 1;
            }
		}
		--nCols;
		++textXpos;
    }
#if defined(STM32F107xC) && defined(MKS_TFT)
 	HAL_GPIO_WritePin(LCD_nCS_GPIO_Port, LCD_nCS_Pin, GPIO_PIN_SET);
#endif
	return 1;
}

// Set up translation for characters. Useful for translating fullstop into decimal point, or changing the width of spaces.
// Either the first string passed must be NULL, or the two strings must have equal lengths as returned by strlen().
void UTFT::setTranslation(const char *tFrom, const char *tTo)
{
	translateFrom = tFrom;
	translateTo = tTo;
}

void UTFT::setFont(const uint8_t* font)
{
	cfont.setFont(font);
}

// Draw a bitmap using 16-bit colours
void UTFT::drawBitmap16(int x, int y, int sx, int sy, const uint16_t * data, int scale, bool byCols)
{
	int curY = y;
#if defined(STM32F107xC) && defined(MKS_TFT)
	HAL_GPIO_WritePin(LCD_nCS_GPIO_Port, LCD_nCS_Pin, GPIO_PIN_RESET);
#endif
	for (int ty = 0; ty < sy; ty++)
	{
		for (int i = 0; i < scale; ++i)
		{
			bool xySet = false;
			for (int tx = 0; tx < sx; tx++)
			{
				const uint16_t col = data[(byCols) ? (tx * sy) + ty : (ty * sx) + tx];
				if (transparentBackground && col == 0xFFFF)
				{
					xySet = false;
				}
				else
				{
					if (!xySet)
					{
                        setXY(x + (tx * scale), curY, x + (sx * scale) - 1, curY);
						xySet = true;
					}
					LCD_Write_Repeated_DATA16(col, scale);
				}
			}
			++curY;
		}
	}
#if defined(STM32F107xC) && defined(MKS_TFT)
	HAL_GPIO_WritePin(LCD_nCS_GPIO_Port, LCD_nCS_Pin, GPIO_PIN_SET);
#endif
}

void UTFT::displayFlush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const uint16_t * data)
{
	HAL_GPIO_WritePin(LCD_nCS_GPIO_Port, LCD_nCS_Pin, GPIO_PIN_RESET);

	setXY(x1,y1,x2,y2);
	int size = (x2-x1+1)*(y2-y1+1);
	for (int pos = 0; pos < size; pos++)	{
		LCD_Write_DATA16(data[pos]);
	}

	HAL_GPIO_WritePin(LCD_nCS_GPIO_Port, LCD_nCS_Pin, GPIO_PIN_SET);
}

// Seaw a bitmap using 4-bit colours and a palette
void UTFT::drawBitmap4(int x, int y, int sx, int sy, const uint8_t * data, Palette palette, int scale, bool byCols)
{
	int curY = y;
#if defined(STM32F107xC) && defined(MKS_TFT)
	HAL_GPIO_WritePin(LCD_nCS_GPIO_Port, LCD_nCS_Pin, GPIO_PIN_RESET);
#endif
	for (int ty = 0; ty < sy; ty++)
	{
		for (int i = 0; i < scale; ++i)
		{
			bool xySet = false;
			for (int tx = 0; tx < sx; tx++)
			{
				const uint16_t idx = (byCols) ? (tx * sy) + ty : (ty * sx) + tx;
				const uint16_t col = (idx & 1) ? palette[data[idx >> 1] & 0x0fu] : palette[data[idx >> 1] >> 4];
				if (transparentBackground && col == 0xFFFF)
				{
					xySet = false;
				}
				else
				{
					if (!xySet)
					{
                        setXY(x + (tx * scale), curY, x + (sx * scale) - 1, curY);
						xySet = true;
					}
					LCD_Write_Repeated_DATA16(col, scale);
				}
			}
			++curY;
		}
	}
#if defined(STM32F107xC) && defined(MKS_TFT)
	HAL_GPIO_WritePin(LCD_nCS_GPIO_Port, LCD_nCS_Pin, GPIO_PIN_SET);
#endif
}

// Draw a compressed bitmap. Data comprises alternate (repeat count - 1, data to write) pairs, both as 16-bit values.
void UTFT::drawCompressedBitmap(int x, int y, int sx, int sy, const uint16_t *data)
{
	uint32_t count = 0;
	uint16_t col = 0;
	sx += x;
	sy += y;
#if defined(STM32F107xC) && defined(MKS_TFT)
	HAL_GPIO_WritePin(LCD_nCS_GPIO_Port, LCD_nCS_Pin, GPIO_PIN_RESET);
#endif
	for (int tx = x; tx < sx; tx++)
	{
		for (int ty = y; ty < sy; ty++)
		{
			if (count == 0)
			{
				count = *data++;
				col = *data++;
			}
			else
			{
				--count;
			}
			setXY(tx, ty, tx, ty);
			LCD_Write_DATA16(col);
		}
	}
#if defined(STM32F107xC) && defined(MKS_TFT)
	HAL_GPIO_WritePin(LCD_nCS_GPIO_Port, LCD_nCS_Pin, GPIO_PIN_SET);
#endif
}

// Draw a compressed bitmap. Data comprises alternate (repeat count - 1, data to write) pairs, both as 16-bit values.
void UTFT::drawCompressedBitmapBottomToTop(int x, int y, int sx, int sy, const uint16_t *data)
{
	uint32_t count = 0;
	uint16_t col = 0;
	sx += x;
	sy += y;
#if defined(STM32F107xC) && defined(MKS_TFT)
	HAL_GPIO_WritePin(LCD_nCS_GPIO_Port, LCD_nCS_Pin, GPIO_PIN_RESET);
#endif
	for (int ty = sy; ty != 0; )
	{
		--ty;
		for (int tx = x; tx < sx; tx++)
		{
			if (count == 0)
			{
				count = *data++;
				col = *data++;
			}
			else
			{
				--count;
			}
			setXY(tx, ty, tx, ty);
			LCD_Write_DATA16(col);
		}
	}
#if defined(STM32F107xC) && defined(MKS_TFT)
	HAL_GPIO_WritePin(LCD_nCS_GPIO_Port, LCD_nCS_Pin, GPIO_PIN_SET);
#endif
}
