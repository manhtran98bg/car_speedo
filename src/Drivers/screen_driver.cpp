#include "Arduino.h"
#include "Arduino_GFX_Library.h"
#include "Arduino_DriveBus_Library.h"
#include "screen_driver.h"
#include "user_config.h"

#define LEDC_TIMER_RES 8
#define LEDC_DUTY_MIN 0
#define LEDC_DUTY_MAX 255
#define LEDC_CHANNEL 0
#define LEDC_PIN LCD_BL
#define LEDC_FREQ 5000
// Static variables
static Arduino_DataBus *bus = new Arduino_ESP32SPI(LCD_DC, LCD_CS, LCD_SCLK, LCD_SDA, -1, HSPI);
Arduino_GFX *screen = new Arduino_GC9A01(bus, LCD_RST, 0, true, LCD_WIDTH, LCD_HEIGHT);

// Arduino_GFX *tft = new Arduino_GC9A01(bus, LCD_RST, 0, true, LCD_WIDTH, LCD_HEIGHT);
// Arduino_Canvas *screen = new Arduino_Canvas(240, 240, tft);
static void fadeIn_task(void *)
{
    for (int i = LEDC_DUTY_MIN; i < LEDC_DUTY_MAX; i++)
    {
        ledcWrite(LEDC_CHANNEL, i);
        vTaskDelay(5);
    }
    vTaskDelete(NULL);
}
void display_fadeIn()
{
    xTaskCreate(fadeIn_task, "fadeIn_task", 4096, NULL, tskIDLE_PRIORITY + 1, NULL);
}
void display_off()
{
    ledcWrite(LEDC_CHANNEL, LEDC_DUTY_MIN);
}
void display_on()
{
    ledcWrite(LEDC_CHANNEL, LEDC_DUTY_MAX);
}
void display_init()
{
    Serial.println("Initializing Display...");
    ledcSetup(LEDC_CHANNEL, LEDC_FREQ, LEDC_TIMER_RES);
    ledcAttachPin(LEDC_PIN, LEDC_CHANNEL);
    display_off();
    screen->begin();
    screen->fillScreen(BLACK);
    screen->setRotation(TFT_ROTATION);
    delay(100);
    display_on();
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
void display_flush_data_1(uint16_t *data, int16_t x, int16_t y, int16_t w, int16_t h) {
    screen->draw16bitRGBBitmap(x, y, (uint16_t *)data, w, h);
}