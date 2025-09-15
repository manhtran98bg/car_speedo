#ifndef _SCREEN_DRIVER_H_
#define _SCREEN_DRIVER_H_
#include "Arduino.h"
#include "TFT_eSPI.h"

void display_init();
void display_flush_data(uint16_t *data, int16_t x1, int16_t y1, int16_t x2, int16_t y2);
void display_dimming();
void display_off();
extern TFT_eSPI screen;
#endif