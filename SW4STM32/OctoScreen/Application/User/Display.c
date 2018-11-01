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
#include "lvgl/lv_draw/lv_draw_arc.h"


// global variables for LVGL
lv_disp_drv_t disp_drv;
lv_obj_t * label1;


void initDisplay() {
	LCD_Init();									// set up the LCD
	LCD_Fill_Scr(0x8800);
}

void display_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p) {
	LCD_Display_Flush(x1,y1,x2,y2,(uint16_t*)color_p);
	lv_flush_ready();
}



void calibrationScreen() {
#define LEDSIZE 20
	uint16_t calA[2] = {30,30};
	uint16_t calB[2] = {300,100};
	uint16_t calC[2] = {150,220};

	lv_obj_t * led1  = lv_led_create(lv_scr_act(), NULL);
	//lv_obj_set_style(led1, &style_led);
	lv_obj_set_size(led1, LEDSIZE, LEDSIZE);
	lv_obj_set_x(led1, calA[0] - LEDSIZE/2);
	lv_obj_set_y(led1, calA[1] - LEDSIZE/2);

	lv_obj_t * led2  = lv_led_create(lv_scr_act(), NULL);
	lv_obj_set_size(led2, LEDSIZE, LEDSIZE);
	lv_obj_set_x(led2, calB[0] - LEDSIZE/2);
	lv_obj_set_y(led2, calB[1] - LEDSIZE/2);

	lv_obj_t * led3  = lv_led_create(lv_scr_act(), NULL);
	lv_obj_set_size(led3, LEDSIZE, LEDSIZE);
	lv_obj_set_x(led3, calC[0] - LEDSIZE/2);
	lv_obj_set_y(led3, calC[1] - LEDSIZE/2);

	lv_led_on(led1);
}

void initScreens() {
	lv_init();

	lv_disp_drv_init(&disp_drv);           /*Basic initialization*/
	disp_drv.disp_flush = display_flush;
	lv_disp_drv_register(&disp_drv);

	/*Create a Label on the currently active screen*/
	label1 =  lv_label_create(lv_scr_act(), NULL);

	/*Modify the Label's text*/
	lv_label_set_text(label1, "Hello world!");
	//lv_obj_set_style(label1, &lv_style_pretty);

	/* Align the Label to the center
	 * NULL means align on parent (which is the screen now)
	 * 0, 0 at the end means an x, y offset after alignment*/
	lv_obj_align(label1, NULL, LV_ALIGN_CENTER, 0, 0);

	calibrationScreen();
}

void setMessage(char* msg) {
	lv_label_set_text(label1, msg);
	lv_obj_align(label1, NULL, LV_ALIGN_CENTER, 0, 0);
}



