#ifndef UTouch_h
#define UTouch_h

#include "UTFT.h"
#include "stdbool.h"

void Touch_Init(uint16_t xp, uint16_t yp);

bool Touch_Read(uint16_t* x, uint16_t* y, uint16_t* rawX , uint16_t* rawY);


/*void Touch_Calibrate(uint16_t xlow, uint16_t xhigh, uint16_t ylow, uint16_t yhigh, uint16_t margin);
void adjustOrientation(DisplayOrientation a) { orientAdjust = (DisplayOrientation) (orientAdjust ^ a); }
	DisplayOrientation getOrientation() const { return orientAdjust; }
    
	uint16_t disp_x_size, disp_y_size;
	uint16_t scaleX, scaleY;
	int16_t offsetX, offsetY;*/

bool Touch_Get_Data(bool wantY, uint16_t* rslt);
//uint16_t diff(uint16_t a, uint16_t b) { return (a < b) ? b - a : a - b; }

#endif
