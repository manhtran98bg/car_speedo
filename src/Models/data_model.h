#ifndef _DATA_MODEL_H_
#define _DATA_MODEL_H_

#pragma once
#include <Arduino.h>

struct Speedo {
    uint8_t flag;
    uint8_t speed_mph;
    uint8_t speed_kmph;
    int8_t fuel_perc;
};

// ==== Global Instances ==== //
extern Speedo SpeedoData;

// ==== Config ==== //
const int MAX_KMPH = 160;

#endif