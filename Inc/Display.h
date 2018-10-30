/*
 * Display.h
 *
 *  Created on: 26 Oct 2018
 *      Author: sven
 */

#ifndef APPLICATION_USER_DISPLAY_H_
#define APPLICATION_USER_DISPLAY_H_

#include "stdint.h"
#include "lvgl/lvgl.h"

void initDisplay();
void display_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p);

#endif /* APPLICATION_USER_DISPLAY_H_ */
