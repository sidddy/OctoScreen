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

//#include "mks_conf.h"
#include "Print.h"

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



#define LCD_DCX_Pin LCD_RS_Pin
#define LCD_DCX_GPIO_Port LCD_RS_GPIO_Port




typedef uint16_t PixelNumber;

enum DisplayOrientation {
	Default = 0x00,
	SwapXY = 0x01,
	ReverseX = 0x02,
	ReverseY = 0x04,

	Landscape = SwapXY | ReverseY
};

// This describes the structure we use to store font information.
// The first 5 fields are also the layout of the data in the font header.
class FontDescriptor
{
private:
    const uint8_t *font;

public:

    FontDescriptor() : font(nullptr) {}
	inline void setFont(const uint8_t * f) {
	    font = f;
	}

	inline uint8_t xSize() const  { return font ? *font : 0; }
	inline uint8_t ySize() const  { return font ? *(font + 1) : 0; }
	static inline uint8_t ySize(const uint8_t * f) { return *(f + 1); }
	inline uint8_t spaces() const { return font ? *(font + 2) : 0; }
	inline uint8_t ranges() const { return font ? *(font + 3) : 0; }

	inline bool inRange(uint32_t c) const {

	    if (!font) return false;

	    uint32_t *firstChar = (uint32_t *) (font + 4);
	    uint32_t *lastChar  = firstChar + 1;

	    for (uint8_t rangesLeft = ranges(); rangesLeft; rangesLeft--, firstChar+=2, lastChar+=2) {
            if ( c >= *firstChar && c <= *lastChar)
                return true;
	    }

	    return false;
	}

	inline uint8_t bytesPerColumn() const {
	    return (ySize() + 7) / 8;
	}

    inline uint8_t bytesPerChar() const {
        return bytesPerColumn() * xSize() + 1;
    }

    inline const uint8_t *fontPtr(uint32_t c) const {

	    if (!font) return nullptr;

        const uint8_t* fPtr = font + 4 + (ranges() * 8);
	    uint32_t *firstChar = (uint32_t *) (font + 4);
	    uint32_t *lastChar  = firstChar + 1;

	    for (uint8_t rangesLeft = ranges(); rangesLeft; rangesLeft--, firstChar+=2, lastChar+=2) {

            if ( c >= *firstChar && c <= *lastChar)
                return fPtr + (bytesPerChar() * (c - *firstChar));

            fPtr += bytesPerChar() * (*lastChar - *firstChar + 1);
	    }

	    return nullptr;
    }
};

typedef uint16_t Colour;
typedef const uint16_t *Palette;

class UTFT : public Print
{
public:
	void InitLCD(DisplayOrientation po);
	void fillScr(Colour c, uint16_t leftMargin = 0);
	void drawPixel(int x, int y);
	void drawLine(int x1, int y1, int x2, int y2);
	void drawRect(int x1, int y1, int x2, int y2);
	void drawRoundRect(int x1, int y1, int x2, int y2);
	void fillRect(int x1, int y1, int x2, int y2, Colour grad = 0, uint8_t gradChange = 1);
	void fillRoundRect(int x1, int y1, int x2, int y2, Colour grad = 0, uint8_t gradChange = 1);
	void drawCircle(int x, int y, int radius);
	void fillCircle(int x, int y, int radius);

	// Colour management. We store colours in native 16-bit format, but support conversion from RGB.
	void setColor(Colour c) { fcolour = c; }
	void setBackColor(Colour c) { bcolour = c; }
	void setTransparentBackground(bool b) { transparentBackground = b; }
	static constexpr Colour fromRGB(uint8_t r, uint8_t g, uint8_t b);

	// New print functions
	// Set up translation for characters. Useful for translating fullstop into decimal point, or changing the width of spaces.
	// Either the first string passed must be NULL, or the two strings must have equal lengths as returned by strlen().
	void setTranslation(const char *tFrom, const char *tTo);
	size_t write(uint8_t c) override;
	void setTextPos(uint16_t x, uint16_t y, uint16_t rm = 9999);
	void clearToMargin();
	size_t print(const char *s, uint16_t x, uint16_t y, uint16_t rm = 9999);
	using Print::print;

	void setFont(const uint8_t* font);
	void drawBitmap16(int x, int y, int sx, int sy, const uint16_t *data, int scale = 1, bool byCols = true);
    void drawBitmap4(int x, int y, int sx, int sy, const uint8_t *data, Palette palette, int scale = 1, bool byCols = true);
    void displayFlush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const uint16_t * data);

	void drawCompressedBitmap(int x, int y, int sx, int sy, const uint16_t *data);
	void drawCompressedBitmapBottomToTop(int x, int y, int sx, int sy, const uint16_t *data);
	inline uint16_t getDisplayXSize() const { return (orient & DisplayOrientation::SwapXY) ? DISPLAY_X : DISPLAY_Y; }
	inline uint16_t getDisplayYSize() const { return (orient & DisplayOrientation::SwapXY) ? DISPLAY_Y : DISPLAY_X; }
	uint16_t getTextX() const { return textXpos; }
	uint16_t getTextY() const { return textYpos; }
	uint16_t getFontHeight() const { return cfont.ySize(); }
    static uint16_t GetFontHeight(const uint8_t *f) { return FontDescriptor::ySize(f); }
    uint16_t LCD_Read_REG(uint8_t reg);

private:
	uint16_t fcolour, bcolour;
	bool transparentBackground;
	DisplayOrientation orient;
	uint16_t disp_x_size, disp_y_size;

	FontDescriptor cfont;
	uint16_t textXpos, textYpos, textRightMargin;
	uint32_t lastCharColData;		// used for auto kerning
	const char* translateFrom;
	const char* translateTo;

	uint32_t charVal;
	uint8_t numContinuationBytesLeft;

	size_t writeNative(uint32_t c);
	void applyGradient(uint16_t grad);

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


	void drawHLine(int x, int y, int len);
	void drawVLine(int x, int y, int len);
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

// Definition of a colour scheme
struct ColourScheme
{
	size_t index;
	Palette pal;

	Colour titleBarTextColour;
	Colour titleBarBackColour;
	Colour labelTextColour;
	Colour infoTextColour;
	Colour infoBackColour;
	Colour defaultBackColour;
	Colour activeBackColour;
	Colour standbyBackColour;
	Colour tuningBackColour;
	Colour errorTextColour;
	Colour errorBackColour;

	Colour popupBorderColour;
	Colour popupBackColour;
	Colour popupTextColour;
	Colour popupButtonTextColour;
	Colour popupButtonBackColour;
	Colour popupInfoTextColour;
	Colour popupInfoBackColour;

	Colour alertPopupBackColour;
	Colour alertPopupTextColour;

	Colour buttonTextColour;
	Colour buttonPressedTextColour;
	Colour buttonTextBackColour;
	Colour buttonImageBackColour;
	Colour buttonGradColour;
	Colour buttonPressedBackColour;
	Colour buttonPressedGradColour;
	Colour buttonBorderColour;
	Colour homedButtonBackColour;
	Colour notHomedButtonBackColour;
	Colour pauseButtonBackColour;
	Colour resumeButtonBackColour;
	Colour resetButtonBackColour;

	Colour progressBarColour;
	Colour progressBarBackColour;

	Colour stopButtonTextColour;
	Colour stopButtonBackColour;
};

extern const ColourScheme colourSchemes[];

#endif
