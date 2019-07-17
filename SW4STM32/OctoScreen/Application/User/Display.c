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
lv_disp_t * disp;

/*A static or global variable to store the buffers*/
static lv_disp_buf_t disp_buf;

/*Static or global buffer(s). The second buffer is optional*/
static lv_color_t buf_1[320 * 10];
//static lv_color_t buf_2[320 * 10];

#define HOME_LEFT_X 10
#define HOME_EXT_Y 25
#define HOME_BED_Y 55
#define HOME_JOB_Y 85
#define HOME_STATUS_Y 115
#define HOME_TEMP_X 160
#define HOME_BTN_Y 180

void flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t * color_p) {
	int32_t x1 = area->x1;
	int32_t x2 = area->x2;
	int32_t y1 = area->y1;
	int32_t y2 = area->y2;

	LCD_Display_Flush(x1,y1,x2,y2,(uint16_t*)color_p);
	lv_disp_flush_ready(&disp_drv);
}

bool touch_input_read(lv_indev_drv_t *drv, lv_indev_data_t *data)
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

	/*Initialize `disp_buf` with the buffer(s) */
	lv_disp_buf_init(&disp_buf, buf_1, NULL, 320*10);

	lv_disp_drv_init(&disp_drv);
	disp_drv.flush_cb = flush_cb;
	disp_drv.buffer = &disp_buf;
	disp = lv_disp_drv_register(&disp_drv);
	lv_disp_set_default(disp);

	lv_indev_drv_init(&indev_drv);
	indev_drv.type = LV_INDEV_TYPE_POINTER;
	indev_drv.read_cb = touch_input_read;
	lv_indev_drv_register(&indev_drv);
}




