#include "Arduino.h"
#include "main_view.h"
#include "lvgl.h"
#include "user_config.h"
#include "Models/data_model.h"

#include "Assets/fonts/ubuntu_20.h"
#include "Assets/fonts/ubuntu_24_n.h"
#include "Assets/fonts/ubuntu_50.h"
#include "Assets/fonts/ubuntu_200.h"
#include "Assets/fonts/font_awesome_icons_small.h"



// ==== Display driver ==== //
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf1[TFT_HOR_RES * 40];
// static lv_color_t buf2[TFT_HOR_RES * 40];
static lv_disp_drv_t disp_drv;

static disp_flush display_flush_callback = nullptr;
// ==== UI Components ==== //

static hw_timer_t *lvgl_timer = nullptr;
static portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
// ==== Private variables ==== //

// ==== External variables ==== //
extern lv_obj_t *speedo_scr;
// ==== External function ==== //
extern void make_speedo_view();



// ==== Static function ==== //

static void IRAM_ATTR onTimer() {
    portENTER_CRITICAL_ISR(&timerMux);
    lv_tick_inc(1);
    portEXIT_CRITICAL_ISR(&timerMux);
}
static void disp_flush_callback(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    if (display_flush_callback != nullptr)
        display_flush_callback((uint16_t *)&color_p->full, area->x1, area->y1, area->x2, area->y2);
    lv_disp_flush_ready(disp);
}

static void lvgl_timer_init() {
    lvgl_timer = timerBegin(1000); // 1kHz = 1ms
    timerAttachInterrupt(lvgl_timer, &onTimer);
    timerAlarm(lvgl_timer, 1, true, 0);
}

lv_color_t get_state_color(struct_icon_parts obj, float value, bool is_icon) {
  // check for -1 initialisation values and return default
  if (value == -1) {
    return (is_show_num || !is_icon) ? PALETTE_WHITE : PALETTE_GREY;
  }
  // check is passed value flags an alert
  if (obj.flag_when == ABOVE) {
    // Check warning first, if defined
    if (obj.warning >= 0 && value > obj.warning) {
        return PALETTE_RED;
    }
    // Check alert if defined
    if (obj.alert >= 0 && value > obj.alert) {
        return PALETTE_AMBER;
    }
  } else if (obj.flag_when == BELOW) {
    // Check warning first, if defined
    if (obj.warning >= 0 && value < obj.warning) {
        return PALETTE_RED;
    }
    // Check alert if defined
    if (obj.alert >= 0 && value < obj.alert) {
        return PALETTE_AMBER;
    }
  }

  // If number showing or not an icon return default white
  if (is_show_num || !is_icon) {
    return PALETTE_WHITE;
  }
  
  // Otherwise, return grey
  return PALETTE_GREY;
}

static void make_ui() {

}
void main_view_init() {
    lv_init();
    lv_disp_draw_buf_init(&draw_buf, buf1, NULL, TFT_HOR_RES * 40);
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = TFT_HOR_RES;
    disp_drv.ver_res = TFT_VER_RES;
    disp_drv.flush_cb = disp_flush_callback;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);
    //init timer
    lvgl_timer_init();
    //make_ui    
    make_speedo_view();
    
    //load default view
    lv_scr_load(speedo_scr);
}
void register_display_flush_callback(disp_flush disp_flush_cb) {
    display_flush_callback = disp_flush_cb;
}
void main_view_process(){
    static size_t updateSpeedo = 0;
    static size_t getData = 0;
    lv_timer_handler();
    if (millis() - updateSpeedo >= 200) {
        updateSpeedo = millis();
         if (SpeedoData.speed_kmph <= 160)
            SpeedoData.speed_kmph = (sin(lv_tick_get() / 2000.0) + 1) * 80;
        else 
            SpeedoData.speed_kmph = 0;
        update_speedo_view();
    }
}