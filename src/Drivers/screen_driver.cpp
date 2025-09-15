#include "screen_driver.h"
#include "Arduino.h"

#define USE_DMA
TFT_eSPI screen = TFT_eSPI();

#ifdef USE_DMA
uint16_t dmaBuffer1[240 * 40]; // Toggle buffer for 16*16 MCU block, 512bytes
uint16_t dmaBuffer2[240 * 40]; // Toggle buffer for 16*16 MCU block, 512bytes
uint16_t *dmaBufferPtr = dmaBuffer1;
bool dmaBufferSel = 0;
#endif


void display_dimming() {
    digitalWrite(GPIO_NUM_42, LOW);
    ledcAttach(GPIO_NUM_42, 5000, LEDC_TIMER_12_BIT);
    ledcFade(GPIO_NUM_42, 0, 4095, 5000);
}
void display_off(){
    digitalWrite(GPIO_NUM_42, LOW);
}
void display_init()
{
    screen.init();
    screen.setRotation(0);

  
}
void display_flush_data(uint16_t *data, int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{

    uint32_t w = (x2 - x1 + 1);
    uint32_t h = (y2 - y1 + 1);
    if (w <= 0 || h <= 0)
    {
        return;
    }
    if (dmaBufferSel)
        dmaBufferPtr = dmaBuffer2;
    else
        dmaBufferPtr = dmaBuffer1;
    dmaBufferSel = !dmaBufferSel;
    // screen.startWrite();
    // screen.setAddrWindow(x1, y1, w, h);
    // screen.pushColors((uint16_t *)data, w * h, true);
    // screen.endWrite();

    screen.startWrite();
    screen.pushImageDMA(x1, y1, w, h, data, dmaBufferPtr);
    screen.endWrite();
}