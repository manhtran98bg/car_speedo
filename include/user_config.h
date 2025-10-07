#ifndef _USER_CONFIG_H_
#define _USER_CONFIG_H_

#include "lvgl.h"
#include <stdio.h>
#include "driver/i2s.h"
// TFT config
#define TFT_HOR_RES 240
#define TFT_VER_RES 240
#define TFT_ROTATION 0


#define LCD_DC 18
#define LCD_SDA 10
#define LCD_SCLK 3
#define LCD_CS 2
#define LCD_RST 21
#define LCD_WIDTH 240
#define LCD_HEIGHT 240

#define LCD_BL 42

// IIC
#define IIC_SDA 8
#define IIC_SCL 9

// TOUCH
#define TP_INT 11
#define TP_RST 0

// Battery Voltage ADC
#define BATTERY_VOLTAGE_ADC_DATA 4

// SD
#define SD_CS 38
#define SD_MOSI 39
#define SD_MISO 40
#define SD_SCLK 41


#define FPS 30
#define MJPEG_BUFFER_SIZE (240 * 240 * 2 / 8)
#define AUDIOASSIGNCORE 1
#define DECODEASSIGNCORE 0
#define DRAWASSIGNCORE 1

// Audio

#define I2S_DIN 7
#define I2S_WS  4
#define I2S_BCLK 5
#define I2S_PORT   I2S_NUM_0
#define SAMPLE_RATE 44100
// Color palette (constexpr để tránh multiple definition)
constexpr lv_color_t PALETTE_BLACK     = LV_COLOR_MAKE(0, 0, 0);
constexpr lv_color_t PALETTE_WHITE     = LV_COLOR_MAKE(255, 255, 255);
constexpr lv_color_t PALETTE_GREY      = LV_COLOR_MAKE(90, 90, 90);
constexpr lv_color_t PALETTE_DARK_GREY = LV_COLOR_MAKE(60, 60, 60);
constexpr lv_color_t PALETTE_AMBER     = LV_COLOR_MAKE(250, 140, 0);
constexpr lv_color_t PALETTE_RED       = LV_COLOR_MAKE(255, 0, 0);

// Meter parts
typedef struct struct_icon_parts {
    int horz_pos;
    int vert_pos;
    int vert_offset;
    float min;
    float max;
    float alert;
    float warning;
    bool flag_when;
    char unit[4];
} struct_icon_parts;

enum UiMode {
  UI_MODE_ODO,
  UI_MODE_GIF
};

// Data from the buttons
typedef struct struct_buttons {
    uint8_t flag;
    uint8_t button;
    uint8_t press_type;
} struct_buttons;

// Data for channel switching
typedef struct struct_set_channel {
    uint8_t flag;
    uint8_t channel_id;
} struct_set_channel;

// Data from the startup ping
typedef struct struct_startup {
    uint8_t flag;
} struct_startup;

// IDs for gauges displays
#define GAUGE_SMALL_SPEEDO     0
#define GAUGE_SMALL_LEVELS     1
#define GAUGE_SMALL_LOCATION   2

// Design elements - hằng số
constexpr uint8_t  TICK_WIDTH       = 2;
constexpr uint8_t  TICK_LENGTH      = 15;
constexpr uint8_t  TICK_TEXT_OFFSET = 30;
constexpr uint8_t  OUTLINE_WIDTH    = 2;
constexpr uint8_t  NEEDLE_WIDTH     = 5;
constexpr int      NEEDLE_OFFSET    = -10;
constexpr lv_color_t NEEDLE_COLOR   = PALETTE_RED;
constexpr uint8_t  HALF_METER_TICKS = 5;
constexpr char     DEFAULT_LABEL[4] = "---";

#define STARTUP_OVERRIDE_TIMER 5000

// ESPNow data sources
#define FLAG_CANBUS             0
#define FLAG_GPS                1
#define FLAG_BUTTONS            2
#define FLAG_OIL_PRESSURE       3
#define FLAG_STARTUP            4
#define FLAG_SET_CHANNEL        5
#define FLAG_FUEL               6
#define FLAG_ONLINE             7

// Console button IDs
#define BUTTON_SETTING          0
#define BUTTON_MODE             1
#define BUTTON_BRIGHTNESS_UP    2
#define BUTTON_BRIGHTNESS_DOWN  3

// Button events
#define CLICK_EVENT_CLICK       0
#define CLICK_EVENT_DOUBLE      1
#define CLICK_EVENT_HOLD        2

// Greater or less than for alerts
constexpr bool ABOVE = true;
constexpr bool BELOW = false;

// Draw buffer sizes
#define DRAW_BUF_SIZE  (TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8))
#define DRAW_BUF_SIZE1 (TFT_HOR_RES * TFT_VER_RES / 10)


// ---- extern cho biến toàn cục ----
// Các biến này sẽ được định nghĩa 1 lần trong main.cpp
extern int ICON_MOVEMENT;
extern int LABEL_LOWER;
extern bool DO_SPLASH;
extern bool is_track_mode;
extern bool is_show_num;
extern uint8_t dimmer_lv;
extern SemaphoreHandle_t displayMutex;
extern volatile UiMode currentMode;
#endif // _USER_CONFIG_H_
