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

#include "Drivers/screen_driver.h"

#include "ui.h"

// ==== Display driver ==== //
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf1[TFT_HOR_RES * 40];
// static lv_color_t *buf1 = (lv_color_t*) heap_caps_aligned_alloc(32, (TFT_HOR_RES*TFT_VER_RES *2) / 6, MALLOC_CAP_DMA);
static lv_disp_drv_t disp_drv;

static disp_flush display_flush_callback = nullptr;
// ==== UI Components ==== //

static hw_timer_t *lvgl_timer = nullptr;
static portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
// ==== Private variables ==== //
static lv_obj_t *dimmer;
static bool startup_done = false;
// ==== External variables ==== //
extern lv_obj_t *speedo_scr;
extern lv_obj_t *splash_scr;
// ==== External function ==== //
extern void make_speedo_view();

extern void make_splash_view();

extern void update_speed_meter();

// ==== Static function ==== //


static void disp_flush_callback(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    if (display_flush_callback != nullptr)
        display_flush_callback((uint16_t *)&color_p->full, area->x1, area->y1, area->x2, area->y2);
    lv_disp_flush_ready(disp);
}


static void dimmer_anim_cb(void *dimmer, int32_t v)
{
    lv_obj_set_style_bg_opa((lv_obj_t *)dimmer, v, 0);
    if (v == 0)
    {
        lv_scr_load(speedo_scr);
        startup_done = true;
    }
}

static void check_speed_meter_task(void *param)
{
    Serial.println("enter check_speed_meter_task");
    while (SpeedoData.speed_kmph <= 160)
    {
        update_speed_meter();
        SpeedoData.speed_kmph++;
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    while (SpeedoData.speed_kmph > 0)
    {
        update_speed_meter();
        SpeedoData.speed_kmph--;
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    startup_done = true;
    Serial.println("Exit check_speed_meter_task");
    vTaskDelete(NULL);
}
void main_view_init()
{
    lv_init();
    lv_disp_draw_buf_init(&draw_buf, buf1, NULL, TFT_HOR_RES * 40);
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = TFT_HOR_RES;
    disp_drv.ver_res = TFT_VER_RES;
    disp_drv.flush_cb = disp_flush_callback;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);
    // make_ui
    ui_init();
    // make_speedo_view();
    // load default view
    // lv_scr_load(speedo_scr);
    // lv_timer_create([](lv_timer_t *t)
    //                 {
    //                     display_dimming();
    //                     lv_timer_del(t); }, 500, NULL);
    // xTaskCreate([](void *param)
    //             {
    //                 while (true)
    //                 {
    //                     lv_timer_handler();
    //                     vTaskDelay(pdMS_TO_TICKS(1));
    //                 } },
    //             "lvgl_loop_task", 4096, NULL, tskIDLE_PRIORITY + 2, NULL);
    // xTaskCreate(check_speed_meter_task, "check_speed_meter_task", 4096, NULL, tskIDLE_PRIORITY + 1, NULL);
}
void register_display_flush_callback(disp_flush disp_flush_cb)
{
    display_flush_callback = disp_flush_cb;
}
void main_view_process()
{
    static size_t updateSpeedo = 0;
    static size_t getData = 0;
    lv_timer_handler();
    // if (millis() - updateSpeedo >= 200 && startup_done)
    // {
    //     updateSpeedo = millis();
    //     if (SpeedoData.speed_kmph <= 160)
    //         SpeedoData.speed_kmph = (sin(lv_tick_get() / 2000.0) + 1) * 80;
    //     else
    //         SpeedoData.speed_kmph = 0;
    //     update_speed_meter();
    // }
}