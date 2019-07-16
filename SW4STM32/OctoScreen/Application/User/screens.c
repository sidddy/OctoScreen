/*
 * screens.c
 *
 *  Created on: 2 Nov 2018
 *      Author: sven
 */

#include "lvgl/lvgl.h"
#include "lvgl/src/lv_draw/lv_draw_arc.h"
#include "lvgl/src/lv_themes/lv_theme_material.h"

#include "screens.h"
#include "serialCommands.h"

#include <stdio.h>
#include <stdlib.h>

#define TAB_START_X 10
#define TAB_START_Y 5

#define STATE_OPERATIONAL 0
#define STATE_PRINTING 1
#define STATE_CANCELLING 2
#define STATE_PAUSING 3
#define STATE_PAUSED 4
#define STATE_OFFLINE 5
#define STATE_OPENING 6
#define STATE_CONNECTING 7
#define STATE_RESUMING 8
#define STATE_UNKNOWN 255

#define MY_SYMBOL_UP     "\xEF\x84\x82"
#define MY_SYMBOL_DOWN   "\xEF\x84\x83"



typedef struct {
	uint8_t id;
	const char* str;
} statemap_t;

statemap_t statemap[] = { {.id = STATE_OPERATIONAL, .str = "Operational"},
		{.id = STATE_PRINTING, .str = "Printing"},
		{.id = STATE_CANCELLING, .str = "Cancelling"},
		{.id = STATE_PAUSING, .str = "Pausing"},
		{.id = STATE_PAUSED, .str = "Paused"},
		{.id = STATE_OFFLINE, .str = "Offline"},
		{.id = STATE_OPENING, .str = "Opening serial port"},
		{.id = STATE_CONNECTING, .str = "Connecting"},
		{.id = STATE_RESUMING, .str = "Resuming"},
		{.id = STATE_UNKNOWN, .str = "Unknown"}};

uint8_t currentState = STATE_UNKNOWN;
uint16_t currentPrintTime = 0;
uint16_t currentPrintTimeLeft = 0;

float tempToolActual = 0;
float tempToolTarget = 0;
float tempBedActual = 0;
float tempBedTarget = 0;

uint16_t rotationCounter = 0;


LV_IMG_DECLARE(bed_img);
LV_IMG_DECLARE(nozzle_img);
LV_IMG_DECLARE(z_height_img);


#define TAB_COUNT 4
char *tab_names[4] = { LV_SYMBOL_HOME, LV_SYMBOL_LOOP, "°C", LV_SYMBOL_LIST };

// Home Buttons & Labels
lv_obj_t * lbl_hm_temp_tool;
lv_obj_t * lbl_hm_temp_bed;
lv_obj_t * lbl_hm_z_height;
lv_obj_t * lbl_hm_job;
lv_obj_t * lbl_hm_status;
lv_obj_t * bar_hm_progress;
lv_obj_t * btn_hm_1;
lv_obj_t * btn_hm_2;
lv_obj_t * mbox_hm_pause;
lv_obj_t * mbox_hm_cancel;


// Move buttons
lv_obj_t * btn_mv_x;
lv_obj_t * btn_mv_y;
lv_obj_t * btn_mv_z;
lv_obj_t * btn_mv_e;

lv_obj_t * btn_mv_01;
lv_obj_t * btn_mv_1;
lv_obj_t * btn_mv_10;
lv_obj_t * btn_mv_100;

lv_obj_t * btn_mv_plus;
lv_obj_t * btn_mv_minus;

lv_obj_t * btn_mv_home;
lv_obj_t * btn_mv_retract;
lv_obj_t * btn_mv_load;


// Temperature Tab
lv_obj_t * btn_temp_tool_pre;
lv_obj_t * btn_temp_tool_off;
lv_obj_t * btn_temp_tool_plus;
lv_obj_t * btn_temp_tool_minus;

lv_obj_t * btn_temp_bed_pre;
lv_obj_t * btn_temp_bed_off;
lv_obj_t * btn_temp_bed_plus;
lv_obj_t * btn_temp_bed_minus;

lv_obj_t * lbl_temp_tool;
lv_obj_t * lbl_temp_bed;

// File Tab

lv_style_t default_file_style;
lv_style_t success_file_style;
lv_style_t error_file_style;
lv_obj_t * lbl_files_folder;

#define FILE_ROWS 4

uint16_t folder_pos = 0;
uint16_t folder_max = 0;

lv_obj_t * btn_file[FILE_ROWS];
lv_obj_t * btn_file_up;
lv_obj_t * btn_file_down;


void calibrationScreen() {
#define LEDSIZE 20
	uint16_t calA[2] = {20,20};
	uint16_t calB[2] = {300,100};
	uint16_t calC[2] = {20,220};

	lv_obj_t * led1  = lv_led_create(lv_scr_act(), NULL);
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
	lbl_hm_status = lv_label_create(lv_scr_act(), NULL);
	lv_obj_set_pos(lbl_hm_status, 100, 100);
	lv_label_set_text(lbl_hm_status, "n/a");
}


static void cbHomeTab(lv_obj_t * btn, lv_event_t event) {
	if ((btn == btn_hm_1) && (event == LV_EVENT_CLICKED)) {
		lv_obj_t* lbl1 = lv_obj_get_child(btn_hm_1, NULL);
		if (lbl1 != NULL) {
			char* txt = lv_label_get_text(lbl1);
			if (!strcmp(txt, "Print")) {
				cmdPrint();
			} else if (!strcmp(txt, "Pause")) {
				if (lv_obj_get_hidden(mbox_hm_cancel) == true) {
					lv_obj_set_hidden(mbox_hm_pause, false);
				}
			} if (!strcmp(txt, "Resume")) {
				cmdResume();
			}
		}
	} else if ((btn == btn_hm_2) && (event == LV_EVENT_CLICKED)) {
		lv_obj_t* lbl2 = lv_obj_get_child(btn_hm_2, NULL);
		if (lbl2 != NULL) {
			char* txt = lv_label_get_text(lbl2);
			if (!strcmp(txt, "Cancel")) {
				if (lv_obj_get_hidden(mbox_hm_pause) == true) {
					lv_obj_set_hidden(mbox_hm_cancel, false);
				}
			}
		}
	}
	return;
}

static void cbHomeTabPauseMBox(lv_obj_t * obj, lv_event_t event)
{
	if (event == LV_EVENT_CLICKED) {
		lv_obj_set_hidden(mbox_hm_pause, true);
		if (!strcmp(lv_mbox_get_active_btn_text(obj), "Yes")) {
			cmdPause();
		}
	}
}

static void cbHomeTabCancelMBox(lv_obj_t * obj, lv_event_t event)
{
	if (event == LV_EVENT_CLICKED) {
		lv_obj_set_hidden(mbox_hm_cancel, true);
		if (!strcmp(lv_mbox_get_active_btn_text(obj), "Yes")) {
			cmdCancel();
		}
	}
}

void initHomeTab(lv_obj_t *tab) {
	lv_obj_t * img = lv_img_create(tab,NULL);
	lv_img_set_src(img, &nozzle_img);
	lv_obj_set_pos(img, 0, TAB_START_Y);

	lbl_hm_temp_tool = lv_label_create(tab, NULL);
	lv_label_set_text(lbl_hm_temp_tool, "000\n000");
	lv_obj_align(lbl_hm_temp_tool, img, LV_ALIGN_IN_TOP_LEFT, 35, -5);

	lv_obj_t * img2 = lv_img_create(tab,NULL);
	lv_img_set_src(img2, &bed_img);
	lv_obj_align(img2, img, LV_ALIGN_IN_TOP_LEFT, 100, 0);

	lbl_hm_temp_bed = lv_label_create(tab, NULL);
	lv_label_set_text(lbl_hm_temp_bed, "000\n000");
	lv_obj_align(lbl_hm_temp_bed, img2, LV_ALIGN_IN_TOP_LEFT, 35, -5);

	lv_obj_t * img3 = lv_img_create(tab,NULL);
	lv_img_set_src(img3, &z_height_img);
	lv_obj_align(img3, img2, LV_ALIGN_IN_TOP_LEFT, 100, 0);

	lbl_hm_z_height = lv_label_create(tab, NULL);
	lv_label_set_text(lbl_hm_z_height, "?");
	lv_obj_align(lbl_hm_z_height, img3, LV_ALIGN_IN_TOP_LEFT, 35, 10);

	lv_obj_t * txt = lv_label_create(tab, NULL);
	lv_label_set_text(txt, "Job");
	lv_obj_align(txt, img, LV_ALIGN_IN_TOP_LEFT, 10, 43);

	lbl_hm_job = lv_label_create(tab, NULL);
	lv_label_set_text(lbl_hm_job, "n/a");
	lv_obj_align(lbl_hm_job, txt, LV_ALIGN_IN_TOP_LEFT, 80, 0);
	lv_label_set_long_mode(lbl_hm_job, LV_LABEL_LONG_SROLL);
	lv_obj_set_width(lbl_hm_job, 220);
	lv_label_set_align(lbl_hm_job, LV_LABEL_ALIGN_LEFT);

	lv_obj_t *txt2 = lv_label_create(tab, NULL);
	lv_label_set_text(txt2, "Status");
	lv_obj_align(txt2, txt, LV_ALIGN_IN_TOP_LEFT, 0, 27);

	txt = txt2;

	lbl_hm_status = lv_label_create(tab, NULL);
	lv_obj_align(lbl_hm_status, txt, LV_ALIGN_IN_TOP_LEFT, 80, 0);
	lv_label_set_long_mode(lbl_hm_status, LV_LABEL_LONG_SROLL);
	lv_obj_set_width(lbl_hm_status, 220);
	lv_label_set_align(lbl_hm_status, LV_LABEL_ALIGN_LEFT);
	lv_label_set_text(lbl_hm_status, "n/a");


	bar_hm_progress = lv_bar_create(tab, NULL);
	lv_obj_set_size(bar_hm_progress, 285, 10);
	lv_obj_align(bar_hm_progress, txt, LV_ALIGN_IN_TOP_LEFT, 0, 30);

	btn_hm_1 = lv_btn_create(tab, NULL);
	lv_btn_set_fit(btn_hm_1, LV_FIT_NONE);
	lv_obj_set_size(btn_hm_1,120,45);
	lv_obj_align(btn_hm_1, bar_hm_progress, LV_ALIGN_IN_TOP_LEFT, 0, 20);
	txt2 = lv_label_create(btn_hm_1, NULL);
	lv_label_set_text(txt2, "Btn1");
	lv_obj_set_event_cb(btn_hm_1, cbHomeTab);

	btn_hm_2 = lv_btn_create(tab, NULL);
	lv_btn_set_fit(btn_hm_2, LV_FIT_NONE);
	lv_obj_set_size(btn_hm_2,120,45);
	lv_obj_align(btn_hm_2, btn_hm_1, LV_ALIGN_IN_TOP_LEFT, 150, 0);
	txt2 = lv_label_create(btn_hm_2, NULL);
	lv_label_set_text(txt2, "Btn2");
	lv_obj_set_event_cb(btn_hm_2, cbHomeTab);


	static const char * btns[] ={"\221Yes", "\221No", ""};


	mbox_hm_pause = lv_mbox_create(tab, NULL);
	lv_mbox_set_text(mbox_hm_pause, "Do you really want to pause the print?");
	lv_mbox_add_btns(mbox_hm_pause, btns);
	lv_obj_set_event_cb(mbox_hm_pause, cbHomeTabPauseMBox);
	lv_obj_set_width(mbox_hm_pause, 250);
	lv_obj_align(mbox_hm_pause, NULL, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_hidden(mbox_hm_pause, true);

	mbox_hm_cancel = lv_mbox_create(tab, NULL);
	lv_mbox_set_text(mbox_hm_cancel, "Do you really want to cancel the print?");
	lv_mbox_add_btns(mbox_hm_cancel, btns);
	lv_obj_set_event_cb(mbox_hm_cancel, cbHomeTabCancelMBox);
	lv_obj_set_width(mbox_hm_cancel, 250);
	lv_obj_align(mbox_hm_cancel, NULL, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_hidden(mbox_hm_cancel, true);



}

static void cbMoveTab(lv_obj_t * btn, lv_event_t event)
{
	if (event != LV_EVENT_CLICKED) {
		return;
	}
	// X/Y/Z/E Button Group
   if (btn == btn_mv_x) {
	   lv_btn_set_state(btn_mv_x, LV_BTN_STATE_TGL_REL);
	   lv_btn_set_state(btn_mv_y, LV_BTN_STATE_REL);
	   lv_btn_set_state(btn_mv_z, LV_BTN_STATE_REL);
	   lv_btn_set_state(btn_mv_e, LV_BTN_STATE_REL);
   } else if (btn == btn_mv_y) {
	   lv_btn_set_state(btn_mv_y, LV_BTN_STATE_TGL_REL);
	   lv_btn_set_state(btn_mv_x, LV_BTN_STATE_REL);
	   lv_btn_set_state(btn_mv_z, LV_BTN_STATE_REL);
	   lv_btn_set_state(btn_mv_e, LV_BTN_STATE_REL);
   } else if (btn == btn_mv_z) {
	   lv_btn_set_state(btn_mv_z, LV_BTN_STATE_TGL_REL);
	   lv_btn_set_state(btn_mv_x, LV_BTN_STATE_REL);
	   lv_btn_set_state(btn_mv_y, LV_BTN_STATE_REL);
	   lv_btn_set_state(btn_mv_e, LV_BTN_STATE_REL);
   } else if (btn == btn_mv_e) {
	   lv_btn_set_state(btn_mv_e, LV_BTN_STATE_TGL_REL);
	   lv_btn_set_state(btn_mv_x, LV_BTN_STATE_REL);
	   lv_btn_set_state(btn_mv_y, LV_BTN_STATE_REL);
	   lv_btn_set_state(btn_mv_z, LV_BTN_STATE_REL);
   }
   // Move Length Button group
   else if (btn == btn_mv_01) {
  	   lv_btn_set_state(btn_mv_01, LV_BTN_STATE_TGL_REL);
  	   lv_btn_set_state(btn_mv_1, LV_BTN_STATE_REL);
  	   lv_btn_set_state(btn_mv_10, LV_BTN_STATE_REL);
  	   lv_btn_set_state(btn_mv_100, LV_BTN_STATE_REL);
   } else if (btn == btn_mv_1) {
  	   lv_btn_set_state(btn_mv_1, LV_BTN_STATE_TGL_REL);
  	   lv_btn_set_state(btn_mv_01, LV_BTN_STATE_REL);
  	   lv_btn_set_state(btn_mv_10, LV_BTN_STATE_REL);
  	   lv_btn_set_state(btn_mv_100, LV_BTN_STATE_REL);
   } else if (btn == btn_mv_10) {
  	   lv_btn_set_state(btn_mv_10, LV_BTN_STATE_TGL_REL);
  	   lv_btn_set_state(btn_mv_01, LV_BTN_STATE_REL);
  	   lv_btn_set_state(btn_mv_1, LV_BTN_STATE_REL);
  	   lv_btn_set_state(btn_mv_100, LV_BTN_STATE_REL);
   } else if (btn == btn_mv_100) {
  	   lv_btn_set_state(btn_mv_100, LV_BTN_STATE_TGL_REL);
  	   lv_btn_set_state(btn_mv_01, LV_BTN_STATE_REL);
  	   lv_btn_set_state(btn_mv_1, LV_BTN_STATE_REL);
  	   lv_btn_set_state(btn_mv_10, LV_BTN_STATE_REL);
   }
   // Plus/Minus
   else if ((btn == btn_mv_plus) || (btn == btn_mv_minus)) {
	   float factor = 1;
	   if (btn == btn_mv_minus) {
		   factor = -1;
	   }

	   double amount = 0;
	   if (lv_btn_get_state(btn_mv_01) == LV_BTN_STATE_TGL_REL) {
		   amount = 0.1;
	   } else if (lv_btn_get_state(btn_mv_1) == LV_BTN_STATE_TGL_REL) {
		   amount = 1;
	   } else if (lv_btn_get_state(btn_mv_10) == LV_BTN_STATE_TGL_REL) {
		   amount = 10;
	   } else if (lv_btn_get_state(btn_mv_100) == LV_BTN_STATE_TGL_REL) {
		   amount = 100;
	   }

	   if (lv_btn_get_state(btn_mv_x) == LV_BTN_STATE_TGL_REL) {
		   cmdJogX(amount*factor);
	   } else if (lv_btn_get_state(btn_mv_y) == LV_BTN_STATE_TGL_REL) {
		   cmdJogY(amount*factor);
	   } else if (lv_btn_get_state(btn_mv_z) == LV_BTN_STATE_TGL_REL) {
		   cmdJogZ(amount*factor);
	   } else if (lv_btn_get_state(btn_mv_e) == LV_BTN_STATE_TGL_REL) {
	   		cmdExtrude(amount*factor);
	   }
   }
   // HOME
   else if (btn == btn_mv_home) {
	   cmdHome();
   }
   // Unload
   else if (btn == btn_mv_retract) {
	   cmdUnload();
   }
   // Load
   else if (btn == btn_mv_load) {
   	   cmdLoad();
   }
}

void initMoveTab(lv_obj_t *tab) {
	// Home & Retract Buttons

	btn_mv_home = lv_btn_create(tab, NULL);
	lv_btn_set_fit(btn_mv_home, LV_FIT_NONE);
	lv_obj_set_size(btn_mv_home, 70, 55);
	lv_obj_set_pos(btn_mv_home, 0, TAB_START_Y);
	lv_obj_set_event_cb(btn_mv_home, cbMoveTab);

	btn_mv_retract = lv_btn_create(tab, btn_mv_home);
	lv_obj_align(btn_mv_retract, btn_mv_home, LV_ALIGN_IN_TOP_LEFT, 0, 60);
	lv_obj_set_event_cb(btn_mv_retract, cbMoveTab);

	btn_mv_load = lv_btn_create(tab, btn_mv_retract);
	lv_obj_align(btn_mv_load, btn_mv_retract, LV_ALIGN_IN_TOP_LEFT, 0, 60);
	lv_obj_set_event_cb(btn_mv_load, cbMoveTab);

	lv_obj_t *txt = lv_label_create(btn_mv_home, NULL);
	lv_label_set_text(txt, LV_SYMBOL_HOME);
	txt = lv_label_create(btn_mv_retract, NULL);
	lv_label_set_text(txt, "E "MY_SYMBOL_UP);
	txt = lv_label_create(btn_mv_load, NULL);
	lv_label_set_text(txt, "E "MY_SYMBOL_DOWN);



	// X/Y/Z/E Buttons

	btn_mv_x = lv_btn_create(tab, NULL);
	lv_btn_set_fit (btn_mv_x, LV_FIT_NONE);
	lv_obj_set_size(btn_mv_x, 70, 40);
	lv_obj_align(btn_mv_x, btn_mv_home, LV_ALIGN_IN_TOP_LEFT, 77, 0);
	lv_btn_set_toggle(btn_mv_x, true);
	lv_btn_set_state(btn_mv_x, LV_BTN_STATE_REL);
	lv_obj_set_event_cb(btn_mv_x, cbMoveTab);

	btn_mv_y = lv_btn_create(tab, btn_mv_x);
	lv_obj_align(btn_mv_y, btn_mv_x, LV_ALIGN_IN_TOP_LEFT, 0, 45);

	btn_mv_z = lv_btn_create(tab, btn_mv_x);
	lv_obj_align(btn_mv_z, btn_mv_y, LV_ALIGN_IN_TOP_LEFT, 0, 45);

	btn_mv_e = lv_btn_create(tab, btn_mv_x);
	lv_obj_align(btn_mv_e, btn_mv_z, LV_ALIGN_IN_TOP_LEFT, 0, 45);
	lv_btn_set_state(btn_mv_e, LV_BTN_STATE_TGL_REL);

	txt = lv_label_create(btn_mv_x, NULL);
	lv_label_set_text(txt, "X");
	txt = lv_label_create(btn_mv_y, NULL);
	lv_label_set_text(txt, "Y");
	txt = lv_label_create(btn_mv_z, NULL);
	lv_label_set_text(txt, "Z");
	txt = lv_label_create(btn_mv_e, NULL);
	lv_label_set_text(txt, "E");

	// Movement Length Buttons
	btn_mv_01 = lv_btn_create(tab, btn_mv_x);
	lv_obj_align(btn_mv_01, btn_mv_x, LV_ALIGN_IN_TOP_LEFT, 77, 0);

	btn_mv_1 = lv_btn_create(tab, btn_mv_01);
	lv_obj_align(btn_mv_1, btn_mv_01, LV_ALIGN_IN_TOP_LEFT, 0, 45);

	btn_mv_10 = lv_btn_create(tab, btn_mv_01);
	lv_obj_align(btn_mv_10, btn_mv_1, LV_ALIGN_IN_TOP_LEFT, 0, 45);
	lv_btn_set_state(btn_mv_10, LV_BTN_STATE_TGL_REL);

	btn_mv_100 = lv_btn_create(tab, btn_mv_01);
	lv_obj_align(btn_mv_100, btn_mv_10, LV_ALIGN_IN_TOP_LEFT, 0, 45);

	txt = lv_label_create(btn_mv_01, NULL);
	lv_label_set_text(txt, "0.1mm");
	txt = lv_label_create(btn_mv_1, NULL);
	lv_label_set_text(txt, "1mm");
	txt = lv_label_create(btn_mv_10, NULL);
	lv_label_set_text(txt, "10mm");
	txt = lv_label_create(btn_mv_100, NULL);
	lv_label_set_text(txt, "100mm");

	btn_mv_minus = lv_btn_create(tab, btn_mv_x);
	lv_obj_align(btn_mv_minus, btn_mv_01, LV_ALIGN_IN_TOP_LEFT, 77, 0);
	lv_btn_set_toggle(btn_mv_minus, false);

	btn_mv_plus = lv_btn_create(tab, btn_mv_minus);
	lv_obj_align(btn_mv_plus, btn_mv_minus, LV_ALIGN_IN_TOP_LEFT, 0, 45);

	txt = lv_label_create(btn_mv_minus, NULL);
	lv_label_set_text(txt, "-");
	txt = lv_label_create(btn_mv_plus, NULL);
	lv_label_set_text(txt, "+");
}

static void cbTemperatureTab(lv_obj_t * btn, lv_event_t event) {
	if (event != LV_EVENT_CLICKED) {
		return;
	}
	if (btn == btn_temp_tool_pre) {
		cmdSetToolTemp(220);
	} else if (btn == btn_temp_tool_off) {
		cmdSetToolTemp(0);
	} else if (btn == btn_temp_tool_plus) {
		cmdSetToolTemp((uint16_t)tempToolTarget+5);
	} else if (btn == btn_temp_tool_minus) {
		cmdSetToolTemp((uint16_t)tempToolTarget-5);
	} else if (btn == btn_temp_bed_pre) {
		cmdSetBedTemp(60);
	} else if (btn == btn_temp_bed_off) {
		cmdSetBedTemp(0);
	} else if (btn == btn_temp_bed_plus) {
		cmdSetBedTemp((uint16_t)tempBedTarget+1);
	} else if (btn == btn_temp_bed_minus) {
		cmdSetBedTemp((uint16_t)tempBedTarget-1);
	}

	return;
}


void initTemperatureTab(lv_obj_t *tab) {
	lv_obj_t * img = lv_img_create(tab,NULL);
	lv_img_set_src(img, &nozzle_img);
	lv_obj_set_pos(img, 10, TAB_START_Y);

	/*lv_obj_t *txt = lv_label_create(tab, NULL);
	lv_obj_set_pos(txt, 0, TAB_START_Y);
	lv_label_set_text(txt, "Tool");*/

	lbl_temp_tool = lv_label_create(tab, NULL);
	lv_obj_align(lbl_temp_tool, img, LV_ALIGN_IN_TOP_LEFT, 100, 10);
	lv_label_set_text(lbl_temp_tool, "000 / 000");

	btn_temp_tool_pre = lv_btn_create(tab, NULL);
	lv_btn_set_fit (btn_temp_tool_pre, LV_FIT_NONE);
	lv_obj_set_size(btn_temp_tool_pre, 70, 40);
	lv_obj_set_pos(btn_temp_tool_pre, 0, TAB_START_Y+40);
	lv_obj_set_event_cb(btn_temp_tool_pre, cbTemperatureTab);

	btn_temp_tool_off = lv_btn_create(tab, btn_temp_tool_pre);
	lv_obj_align(btn_temp_tool_off, btn_temp_tool_pre, LV_ALIGN_IN_TOP_LEFT, 77, 0);

	btn_temp_tool_plus = lv_btn_create(tab, btn_temp_tool_pre);
	lv_obj_align(btn_temp_tool_plus, btn_temp_tool_off, LV_ALIGN_IN_TOP_LEFT, 77, 0);

	btn_temp_tool_minus = lv_btn_create(tab, btn_temp_tool_pre);
	lv_obj_align(btn_temp_tool_minus, btn_temp_tool_plus, LV_ALIGN_IN_TOP_LEFT, 77, 0);


	img = lv_img_create(tab,NULL);
	lv_img_set_src(img, &bed_img);
	lv_obj_set_pos(img, 10, TAB_START_Y+90);

	lbl_temp_bed = lv_label_create(tab, NULL);
	lv_obj_align(lbl_temp_bed, img, LV_ALIGN_IN_TOP_LEFT, 100, 10);
	lv_label_set_text(lbl_temp_bed, "000 / 000");

	btn_temp_bed_pre = lv_btn_create(tab, btn_temp_tool_pre);
	lv_obj_set_pos(btn_temp_bed_pre, 0, TAB_START_Y+130);

	btn_temp_bed_off = lv_btn_create(tab, btn_temp_tool_pre);
	lv_obj_align(btn_temp_bed_off, btn_temp_bed_pre, LV_ALIGN_IN_TOP_LEFT, 77, 0);

	btn_temp_bed_plus = lv_btn_create(tab, btn_temp_tool_pre);
	lv_obj_align(btn_temp_bed_plus, btn_temp_bed_off, LV_ALIGN_IN_TOP_LEFT, 77, 0);

	btn_temp_bed_minus = lv_btn_create(tab, btn_temp_tool_pre);
	lv_obj_align(btn_temp_bed_minus, btn_temp_bed_plus, LV_ALIGN_IN_TOP_LEFT, 77, 0);

	lv_obj_t * txt = lv_label_create(btn_temp_tool_pre, NULL);
	lv_label_set_text(txt, "220");

	txt = lv_label_create(btn_temp_tool_off, NULL);
	lv_label_set_text(txt, "OFF");

	txt = lv_label_create(btn_temp_tool_plus, NULL);
	lv_label_set_text(txt, "+5");

	txt = lv_label_create(btn_temp_tool_minus, NULL);
	lv_label_set_text(txt, "-5");

	txt = lv_label_create(btn_temp_bed_pre, NULL);
	lv_label_set_text(txt, "60");

	txt = lv_label_create(btn_temp_bed_off, NULL);
	lv_label_set_text(txt, "OFF");

	txt = lv_label_create(btn_temp_bed_plus, NULL);
	lv_label_set_text(txt, "+1");

	txt = lv_label_create(btn_temp_bed_minus, NULL);
	lv_label_set_text(txt, "-1");
}

lv_obj_t* getFilenameLabel(uint8_t no) {
	if (no < FILE_ROWS) {
		lv_obj_t* btn = btn_file[no];
		if (btn != NULL) {
			lv_obj_t* lbl = lv_obj_get_child(btn, NULL);
			return lbl;
		}
	}
	return NULL;
}

void setFilename(uint8_t no, char* name) {
	lv_obj_t* lbl = getFilenameLabel(no);
	if (lbl != NULL) {
		if (!strncmp(name, "[+]", 3)) {
			name = name + 3;
			lv_label_set_style(lbl, LV_LABEL_STYLE_MAIN, &success_file_style);
		} else if (!strncmp(name, "[-]", 3)) {
			name = name + 3;
			lv_label_set_style(lbl, LV_LABEL_STYLE_MAIN, &error_file_style);
		} else {
			lv_label_set_style(lbl, LV_LABEL_STYLE_MAIN, &default_file_style);
		}
		lv_label_set_text(lbl, name);
	}
}



static void cbFileTab(lv_obj_t * btn, lv_event_t event) {
	if (event != LV_EVENT_CLICKED) {
		return;
	}
	if (btn == btn_file_up) {
		if (folder_pos > 0) {
			// move files down
			for (int i=FILE_ROWS-1; i>0; i--) {
				lv_obj_t* src = getFilenameLabel(i-1);
				lv_obj_t* dst = getFilenameLabel(i);

				if ((src != NULL) && (dst != NULL)) {
					lv_label_set_text(dst, lv_label_get_text(src));
					lv_label_set_style(dst, LV_LABEL_STYLE_MAIN, lv_label_get_style(src, LV_LABEL_STYLE_MAIN));
				}
			}
			setFilename(0, "");
			folder_pos--;
			lv_btn_set_state(btn_file_down, LV_BTN_STATE_REL);
			if (folder_pos == 0) {
				lv_btn_set_state(btn_file_up, LV_BTN_STATE_INA);
			}
			cmdGetFolderEntries(folder_pos, folder_pos);
		}
	} else if (btn == btn_file_down) {
		if (folder_pos < folder_max-FILE_ROWS) {
			// move files up
			for (int i=0; i<FILE_ROWS-1; i++) {
				lv_obj_t* src = getFilenameLabel(i+1);
				lv_obj_t* dst = getFilenameLabel(i);

				if ((src != NULL) && (dst != NULL)) {
					lv_label_set_text(dst, lv_label_get_text(src));
					lv_label_set_style(dst, LV_LABEL_STYLE_MAIN, lv_label_get_style(src, LV_LABEL_STYLE_MAIN));
				}
			}
			setFilename(FILE_ROWS-1, "");
			folder_pos++;
			lv_btn_set_state(btn_file_up, LV_BTN_STATE_REL);
			if (folder_pos == folder_max-FILE_ROWS) {
				lv_btn_set_state(btn_file_down, LV_BTN_STATE_INA);
			}
			cmdGetFolderEntries(folder_pos+FILE_ROWS-1, folder_pos+FILE_ROWS-1);
		}
	} else {
		for (int i=0; i<FILE_ROWS; i++) {
			if (btn == btn_file[i]) {
				cmdSelectFolderEntry(folder_pos+i);
				break;
			}
		}
	}
	return;
}


void initFileTab(lv_obj_t * tab) {
	lbl_files_folder = lv_label_create(tab, NULL);
	lv_obj_set_pos(lbl_files_folder, TAB_START_X, TAB_START_Y);

	btn_file_up = lv_btn_create(tab, NULL);
	lv_obj_align(btn_file_up, lbl_files_folder, LV_ALIGN_IN_TOP_LEFT, 0, 30);
	lv_obj_set_size(btn_file_up, 60, 60);
	lv_obj_t * l = lv_label_create(btn_file_up, NULL);
	lv_label_set_text(l, LV_SYMBOL_UP);
	lv_obj_set_event_cb(btn_file_up, cbFileTab);

	btn_file_down = lv_btn_create(tab, btn_file_up);
	lv_obj_align(btn_file_down, btn_file_up, LV_ALIGN_IN_TOP_LEFT, 0, 4*35-1-60);
	l = lv_label_create(btn_file_down, NULL);
	lv_label_set_text(l, LV_SYMBOL_DOWN);

	for (int i=0; i<FILE_ROWS; i++) {
		if (i == 0) {
			btn_file[i] = lv_btn_create(tab, NULL);
			lv_obj_align(btn_file[i], btn_file_up, LV_ALIGN_IN_TOP_LEFT, 70, 0);
			lv_obj_set_size(btn_file[i], 230, 33);
			const lv_style_t * st = lv_btn_get_style(btn_file[i], LV_BTN_STYLE_REL);

			lv_style_copy(&default_file_style, st);
			default_file_style.body.main_color = LV_COLOR_MAKE(0xCC, 0xCC, 0xFF);
			default_file_style.body.grad_color = LV_COLOR_MAKE(0xCC, 0xCC, 0xFF);
			default_file_style.text.color = LV_COLOR_MAKE(0x11, 0x11, 0x11);
			lv_btn_set_style(btn_file[i], LV_BTN_STYLE_REL, &default_file_style);
			lv_obj_set_event_cb(btn_file[i], cbFileTab);
		} else {
			btn_file[i] = lv_btn_create(tab, btn_file[0]);
			lv_obj_align(btn_file[i], btn_file[i-1], LV_ALIGN_IN_TOP_LEFT, 0, 35);
		}
		lv_obj_t * lbl = lv_label_create(btn_file[i], NULL);
		lv_label_set_long_mode(lbl, LV_LABEL_LONG_SROLL);
		lv_obj_set_width(lbl, 220);
		lv_label_set_align(lbl, LV_LABEL_ALIGN_LEFT);
		lv_label_set_text(lbl, "n/a");
	}

	lv_style_copy(&success_file_style, &default_file_style);
	success_file_style.text.color = LV_COLOR_MAKE(0x22, 0xBB, 0x00);

	lv_style_copy(&error_file_style, &default_file_style);
	error_file_style.text.color = LV_COLOR_MAKE(0xFF, 0x00, 0x00);
}

void initScreens() {
	lv_theme_t * th = lv_theme_material_init(240, NULL);
	lv_style_t * s = th->style.btn.pr;
	s->body.main_color = LV_COLOR_MAKE(0x00, 0x00, 0x55);
	s->body.grad_color = LV_COLOR_MAKE(0x00, 0x00, 0x33);
	s->text.color = LV_COLOR_MAKE(0xbb, 0xd5, 0xf1);
	s = th->style.btn.tgl_rel;
	s->body.main_color = LV_COLOR_MAKE(0x00, 0x00, 0x55);
	s->body.grad_color = LV_COLOR_MAKE(0x00, 0x00, 0x33);
	s->text.color = LV_COLOR_MAKE(0xbb, 0xd5, 0xf1);
	lv_theme_set_current(th);

	lv_obj_t *tabview;
	tabview = lv_tabview_create(lv_scr_act(), NULL);
	lv_tabview_set_sliding(tabview, false);
	lv_tabview_set_anim_time(tabview, 300);

	lv_obj_t *tabs[TAB_COUNT];
	for (int i=0; i<TAB_COUNT; i++) {
		tabs[i] = lv_tabview_add_tab(tabview, tab_names[i]);
		lv_page_set_scrl_fit(tabs[i], LV_FIT_NONE);
		lv_page_set_scrl_width(tabs[i], 304);
		lv_page_set_scrl_height(tabs[i], 180);
	}

	initHomeTab(tabs[0]);
	initMoveTab(tabs[1]);
	initTemperatureTab(tabs[2]);
	initFileTab(tabs[3]);

	setFolder("");
	setStatus("");

	return;
}

void setMessage(char* msg) {
	return;
	lv_label_set_text(lbl_hm_status, msg);
}

void resetTempBedLabel() {
	char buf[64];
	if ((lbl_temp_bed != NULL) && (lbl_hm_temp_bed != NULL)) {
		if (tempBedTarget > 0) {
			snprintf(buf, 64, "%3.0f°C / %0.0f°C", tempBedActual, tempBedTarget);
			lv_label_set_text(lbl_temp_bed, buf);
			snprintf(buf, 64, "%0.0f°C\n%0.0f°C", tempBedActual, tempBedTarget);
			lv_label_set_text(lbl_hm_temp_bed, buf);
		} else {
			snprintf(buf, 64, "%3.0f°C / OFF", tempBedActual);
			lv_label_set_text(lbl_temp_bed, buf);
			snprintf(buf, 64, "%0.0f°C\nOFF", tempBedActual);
			lv_label_set_text(lbl_hm_temp_bed, buf);
		}
	}
}

void resetTempToolLabel() {
	char buf[64];
	if ((lbl_temp_tool != NULL) && (lbl_hm_temp_tool != NULL)) {
		if (tempToolTarget > 0) {
			snprintf(buf, 64, "%3.0f°C / %0.0f°C", tempToolActual, tempToolTarget);
			lv_label_set_text(lbl_temp_tool, buf);
			snprintf(buf, 64, "%0.0f°C\n%0.0f°C", tempToolActual, tempToolTarget);
			lv_label_set_text(lbl_hm_temp_tool, buf);
		} else {
			snprintf(buf, 64, "%3.0f°C / OFF", tempToolActual);
			lv_label_set_text(lbl_temp_tool, buf);
			snprintf(buf, 64, "%0.0f°C\nOFF", tempToolActual);
			lv_label_set_text(lbl_hm_temp_tool, buf);
		}
	}
}

void setTempBedActual(float val) {
	tempBedActual = val;
	resetTempBedLabel();
}

void setTempBedTarget(float val) {
	tempBedTarget = val;
	resetTempBedLabel();
}

void setTempToolActual(float val) {
	tempToolActual = val;
	resetTempToolLabel();
}

void setTempToolTarget(float val) {
	tempToolTarget = val;
	resetTempToolLabel();
}

void setZHeight(float z) {
	if (lbl_hm_z_height != NULL) {
		char buf[64];
		snprintf(buf, 64, "%3.2f", z);
		lv_label_set_text(lbl_hm_z_height, buf);
	}
}

const char* getStateString(uint8_t state) {
	uint8_t l = 0;
	while (statemap[l].id != STATE_UNKNOWN) {
		if (statemap[l].id == state) {
			return statemap[l].str;
		}
		l++;
	}
	return statemap[l].str;
}

void updateStatusText() {
	if ((lbl_hm_status != NULL) && (bar_hm_progress != NULL)) {
		char buf[128];
		if (currentState == STATE_PRINTING) {
			if (rotationCounter % 3 == 0) {
				snprintf(buf, 128, "%s, %d%% done", getStateString(currentState), lv_bar_get_value(bar_hm_progress));
			} else if (rotationCounter % 3 == 1) {
				if (currentPrintTime > 0) {
					int h = currentPrintTime / 3600;
					int m = (currentPrintTime-(h*3600)) / 60;
					snprintf(buf, 128, "Print Time %02d:%02d", h, m);
				} else {
					snprintf(buf, 128, "%s, %d%% done", getStateString(currentState), lv_bar_get_value(bar_hm_progress));
				}
			} if (rotationCounter % 3 == 2) {
				if (currentPrintTimeLeft > 0) {
					int h = currentPrintTimeLeft / 3600;
					int m = (currentPrintTimeLeft-(h*3600)) / 60;
					snprintf(buf, 128, "ETL %02d:%02d", h, m);
				} else {
					snprintf(buf, 128, "%s, %d%% done", getStateString(currentState), lv_bar_get_value(bar_hm_progress));
				}
			}
		} else {
			snprintf(buf, 128, "%s", getStateString(currentState));
		}

		lv_label_set_text(lbl_hm_status, buf);
	}
}

void setProgress(float p) {
	if (bar_hm_progress != NULL) {
		lv_bar_set_value(bar_hm_progress, p, LV_ANIM_OFF);
	}
}

void setPrintTime(uint16_t t) {
	currentPrintTime = t;
	updateStatusText();
}

void setPrintTimeLeft(uint16_t t) {
	currentPrintTimeLeft = t;
	updateStatusText();
}

void setJob(char* name) {
	if (lbl_hm_job != NULL) {
		lv_label_set_text(lbl_hm_job, name);
	}
}

void setStatus(char* status) {
	uint8_t st = 0;
	rotationCounter = 0;
	while (statemap[st].id != STATE_UNKNOWN) {
		if (!strcmp(statemap[st].str, status)) {
			currentState = statemap[st].id;
			break;
		}
		st++;
	}
	if (statemap[st].id == STATE_UNKNOWN) {
		currentState = STATE_UNKNOWN;
	}
	if ((currentState != STATE_PRINTING) && (currentState != STATE_PAUSING) && (currentState != STATE_RESUMING) && (currentState != STATE_PAUSED)) {
		currentPrintTime = 0;
		currentPrintTimeLeft = 0;
		if (bar_hm_progress != NULL) {
			lv_bar_set_value(bar_hm_progress, 0, LV_ANIM_OFF);
		}
	}

	if ((btn_hm_1 != NULL) && (btn_hm_2 != NULL)) {
		lv_obj_t* lbl1 = lv_obj_get_child(btn_hm_1, NULL);
		lv_obj_t* lbl2 = lv_obj_get_child(btn_hm_2, NULL);
		if ((lbl1 != NULL) && (lbl2 != NULL)) {
			switch(currentState) {
			case STATE_OPERATIONAL:
				lv_btn_set_state(btn_hm_1, LV_BTN_STATE_REL);
				lv_btn_set_state(btn_hm_2, LV_BTN_STATE_INA);
				lv_label_set_text(lbl1, "Print");
				lv_label_set_text(lbl2, "");
				break;
			case STATE_PRINTING:
				lv_btn_set_state(btn_hm_1, LV_BTN_STATE_REL);
				lv_btn_set_state(btn_hm_2, LV_BTN_STATE_REL);
				lv_label_set_text(lbl1, "Pause");
				lv_label_set_text(lbl2, "Cancel");
				break;
			case STATE_PAUSED:
				lv_btn_set_state(btn_hm_1, LV_BTN_STATE_REL);
				lv_btn_set_state(btn_hm_2, LV_BTN_STATE_REL);
				lv_label_set_text(lbl1, "Resume");
				lv_label_set_text(lbl2, "Cancel");
				break;
			case STATE_UNKNOWN:
			case STATE_CANCELLING:
			case STATE_PAUSING:
			case STATE_OFFLINE:
			case STATE_OPENING:
			case STATE_CONNECTING:
			case STATE_RESUMING:

				lv_btn_set_state(btn_hm_1, LV_BTN_STATE_INA);
				lv_btn_set_state(btn_hm_2, LV_BTN_STATE_INA);
				lv_label_set_text(lbl1, "");
				lv_label_set_text(lbl2, "");
				break;
			}
		}
	}

	updateStatusText();
}

void setFolder(char* name) {
	folder_max = 0;
	folder_pos = 0;
	for (int i=0; i<FILE_ROWS; i++) {
		setFilename(i, "");
	}

	if (lbl_files_folder != NULL) {
		char buf[128];
		snprintf(buf, 128, "Folder: %s", name);
		lv_label_set_text(lbl_files_folder, buf);
	}
	lv_btn_set_state(btn_file_down, LV_BTN_STATE_INA);
	lv_btn_set_state(btn_file_up, LV_BTN_STATE_INA);
}

void setFolderCount(uint16_t c) {
	folder_max = c;
	folder_pos = 0;
	uint16_t to = FILE_ROWS-1;
	if (to >= folder_max) {
		to = folder_max - 1;
	}
	if (folder_max-folder_pos > FILE_ROWS) {
		lv_btn_set_state(btn_file_down, LV_BTN_STATE_REL);
	} else {
		lv_btn_set_state(btn_file_down, LV_BTN_STATE_INA);
	}
	lv_btn_set_state(btn_file_up, LV_BTN_STATE_INA);
	cmdGetFolderEntries(0, to);
}

void setFolderEntry(uint16_t no, char* name) {
	if ((no >= folder_pos) && (no <= folder_pos+FILE_ROWS)) {
		setFilename(no-folder_pos, name);
	}
}

void rotateLabels() {
	rotationCounter++;
	updateStatusText();
}
