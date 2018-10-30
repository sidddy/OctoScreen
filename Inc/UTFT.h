/*
  UTFT.h - Arduino/chipKit library support for Color TFT LCD Boards
  Copyright (C)2010-2012 Henning Karlsen. All right reserved

  This library is the continuation of my ITDB02_Graph, ITDB02_Graph16
  and RGB_GLCD libraries for Arduino and chipKit. As the number of
  supported display modules and controllers started to increase I felt
  it was time to make a single, universal library as it will be much
  easier to maintain in the future.

  Basic functionality of this library was origianlly based on the
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

#ifndef UTFT_h
#define UTFT_h

#define DISPLAY_X 240
#define DISPLAY_Y 320

#define ST7789_NOP     0x00
#define ST7789_SWRESET 0x01
#define ST7789_RDDID   0x04
#define ST7789_RDDST   0x09

#define ST7789_SLPIN   0x10
#define ST7789_SLPOUT  0x11
#define ST7789_PTLON   0x12
#define ST7789_NORON   0x13

#define ST7789_INVOFF  0x20
#define ST7789_INVON   0x21
#define ST7789_DISPOFF 0x28
#define ST7789_DISPON  0x29
#define ST7789_CASET   0x2A
#define ST7789_RASET   0x2B
#define ST7789_RAMWR   0x2C
#define ST7789_RAMRD   0x2E

#define ST7789_PTLAR   0x30
#define ST7789_COLMOD  0x3A
#define ST7789_MADCTL  0x36

#define ST7789_MADCTL_MY  0x80
#define ST7789_MADCTL_MX  0x40
#define ST7789_MADCTL_MV  0x20
#define ST7789_MADCTL_ML  0x10
#define ST7789_MADCTL_RGB 0x00

#define ST7789_RDID1   0xDA
#define ST7789_RDID2   0xDB
#define ST7789_RDID3   0xDC
#define ST7789_RDID4   0xDD


typedef uint16_t Colour;


class UTFT
{
public:
	void InitLCD();
	void fillScr(Colour c, uint16_t leftMargin = 0);

	// Colour management. We store colours in native 16-bit format, but support conversion from RGB.
	void setColor(Colour c) { fcolour = c; }

	static constexpr Colour fromRGB(uint8_t r, uint8_t g, uint8_t b);

    void displayFlush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const uint16_t * data);

	inline uint16_t getDisplayXSize() const { return DISPLAY_X; }
	inline uint16_t getDisplayYSize() const { return DISPLAY_Y; }


private:
	uint16_t fcolour, bcolour;


	// Hardware interface
	void LCD_Write_Bus(uint16_t VHL);
	void LCD_Write_Again(uint16_t num);
	void _set_direction_registers();

	// Low level interface
	void LCD_Write_COM(uint8_t VL);
	void LCD_Write_DATA8(uint8_t VL);
	void LCD_Write_DATA16(uint16_t VHL);
	void LCD_Write_Repeated_DATA16(uint16_t VHL, uint16_t num);
	void LCD_Write_Repeated_DATA16(uint16_t VHL, uint16_t num1, uint16_t num2);
	void LCD_Write_COM_DATA16(uint8_t com1, uint16_t dat1);
	void LCD_Write_COM_DATA8(uint8_t com1, uint8_t dat1);
	void LCD_Write_COM_DATA(uint8_t com1, uint16_t dat1);
	void LCD_Write_DATA(uint8_t VH, uint8_t VL);
	void LCD_Write_DATA(int v);
	uint8_t LCD_Cmd_Read8(uint8_t cmd);

	void delay(int ms);

	void setXY(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
};

inline constexpr uint16_t UTFT::fromRGB(uint8_t r, uint8_t g, uint8_t b)
{
	return ((r & 248) << 8) | ((g & 252) << 3) | (b >> 3);
}

// Some common colours
const Colour
	white = 0xFFFF,
	black = 0x0000,
	lightGrey = UTFT::fromRGB(180, 180, 180),
	grey = UTFT::fromRGB(128, 128, 128),
	red = UTFT::fromRGB(255,0,0),
	lightRed =  UTFT::fromRGB(255, 128, 128),
	darkRed = UTFT::fromRGB(128, 0, 0),
	yellow = UTFT::fromRGB(128,128,0),
	lightYellow = UTFT::fromRGB(255, 255, 128),
	darkYellow = UTFT::fromRGB(64, 64, 0),
	lightOrange = UTFT::fromRGB(255, 224, 192),
	darkOrange = UTFT::fromRGB(128, 64, 0),
	green = UTFT::fromRGB(0,255,0),
	lightGreen = UTFT::fromRGB(180, 255, 180),
	midGreen =  UTFT::fromRGB(0, 160, 0),
	darkGreen = UTFT::fromRGB(0, 96, 0),
	turquoise = UTFT::fromRGB(0,128,128),
	blue = UTFT::fromRGB(0,0,255),
	magenta = UTFT::fromRGB(128,0,128),
	lightBlue = UTFT::fromRGB(180, 180, 255),
	darkBlue = UTFT::fromRGB(0, 0, 64);




#endif
