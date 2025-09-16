#include "Arduino.h"
#include "Arduino_GFX_Library.h"
#include "Arduino_DriveBus_Library.h"
#include "screen_driver.h"
#include "user_config.h"


// Static variables
static Arduino_DataBus *bus = new Arduino_ESP32SPI(LCD_DC, LCD_CS, LCD_SCLK, LCD_SDA, -1, HSPI);
Arduino_GFX *screen = new Arduino_GC9A01(bus, LCD_RST ,0 , true , LCD_WIDTH, LCD_HEIGHT);


void display_dimming() {
    // digitalWrite(GPIO_NUM_42, LOW);
    // ledcAttach(GPIO_NUM_42, 5000, LEDC_TIMER_12_BIT);
    // ledcFade(GPIO_NUM_42, 0, 4095, 5000);
}
void display_off(){
    digitalWrite(LCD_BL, LOW);
}
void display_on() {
    digitalWrite(LCD_BL, HIGH);
}
void display_init()
{
    Serial.println("Initializing Display...");
    pinMode(LCD_BL, OUTPUT);
    digitalWrite(LCD_BL, HIGH);
    screen->begin();
    screen->fillScreen(BLACK);
    screen->setRotation(TFT_ROTATION);  
    
}
void display_flush_data(uint16_t *data, int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{

    uint32_t w = (x2 - x1 + 1);
    uint32_t h = (y2 - y1 + 1);
    if (w <= 0 || h <= 0)
    {
        return;
    }
    screen->draw16bitRGBBitmap(x1, y1, (uint16_t *)data, w, h);
}