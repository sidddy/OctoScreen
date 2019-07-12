/*
 * Display.
 *
 *  Created on: 26 Oct 2018
 *      Author: sven
 */
#include "Display.h"

#include "UTFT.h"
#include "UTouch.h"
#include "cmsis_os.h"
#include "lvgl/lvgl.h"

// global variables for LVGL
lv_disp_drv_t disp_drv;
lv_indev_drv_t indev_drv;
lv_obj_t * label1;

#define HOME_LEFT_X 10
#define HOME_EXT_Y 25
#define HOME_BED_Y 55
#define HOME_JOB_Y 85
#define HOME_STATUS_Y 115
#define HOME_TEMP_X 160
#define HOME_BTN_Y 180

void display_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p) {
	LCD_Display_Flush(x1,y1,x2,y2,(uint16_t*)color_p);
	lv_flush_ready();
}

bool touch_input_read(lv_indev_data_t *data)
{
	uint16_t x,y,z;
	if (Touch_Get_Filtered_Data(&x, &y, &z)) {
		data->point.x = x;
		data->point.y = y;
		data->state = LV_INDEV_STATE_PR;
	} else {
		data->state = LV_INDEV_STATE_REL;
	}
    return false;  			/*No buffering so no more data read*/
}


void initDisplay() {
	LCD_Init();									// set up the LCD
	LCD_Fill_Scr(0xFFFF);
	lv_init();

	lv_disp_drv_init(&disp_drv);
	disp_drv.disp_flush = display_flush;
	lv_disp_drv_register(&disp_drv);

	lv_indev_drv_init(&indev_drv);
	indev_drv.type = LV_INDEV_TYPE_POINTER;
	indev_drv.read = touch_input_read;
	lv_indev_drv_register(&indev_drv);
}




