#ifndef _SCREEN_DRIVER_H_
#define _SCREEN_DRIVER_H_
#include "Arduino.h"
#include "Arduino_GFX_Library.h"

void display_init();
void display_flush_data(uint16_t *data, int16_t x1, int16_t y1, int16_t x2, int16_t y2);
void display_flush_data_1(uint16_t *data, int16_t x, int16_t y, int16_t w, int16_t h);
void display_off();
extern Arduino_GFX *screen;
#endif