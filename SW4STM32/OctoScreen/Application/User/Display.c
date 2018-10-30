/*
 * Display.
 *
 *  Created on: 26 Oct 2018
 *      Author: sven
 */
#include "Display.h"

#include "UTFT.h"
#include "cmsis_os.h"
#include "lvgl/lvgl.h"


void initDisplay() {
	LCD_Init();									// set up the LCD
	LCD_Fill_Scr(0x8800);
	uint16_t *buf;

	buf = (uint16_t*)pvPortMalloc(640*sizeof(uint16_t));
	for (int i=0; i<640; i++) {
		if (i <320) {
			buf[i] = 0x0000;
		} else {
			buf[i] = 0xFFFF;
		}
	}
	int x = 280;
	int y = 215;
	LCD_Display_Flush(x,y,x+31,y+19,buf);

	vPortFree(buf);
}

void display_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p) {
	LCD_Display_Flush(x1,y1,x2,y2,(uint16_t*)color_p);
	lv_flush_ready();
}



