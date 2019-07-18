#ifndef XPT2046_h
#define XPT2046_h

#include <stdbool.h>
#include <stdint.h>

#define XPT2046_DFR_MODE 0x00
#define XPT2046_SER_MODE 0x04
#define XPT2046_CONTROL  0x80

#define XPT2046_COOR_X  0x10
#define XPT2046_COOR_Y  0x50
#define XPT2046_COOR_Z1 0x30
#define XPT2046_COOR_Z2 0x40

void XPT2046_Touch_Init(uint16_t xp, uint16_t yp);
void XPT2046_Touch_Get_Raw_Data(uint16_t* x, uint16_t* y, uint16_t* z1, uint16_t* z2);
bool XPT2046_Touch_Get_Data(uint16_t* x, uint16_t* y, uint16_t* z);

#endif
