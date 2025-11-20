#include "Arduino.h"
#include "AnimatedGIF.h"
#include "lvgl.h"
#include "ui.h"

#include "main_view.h"
#include "user_config.h"
#include "Models/data_model.h"
#include "Drivers/Display/ScreenDriver.h"
#include "gif_view.h"

#include "Eyes/Adapter/CanvasImpl.h"
#include "Eyes/Adapter/CanvasManager.hpp"

// ==== Display driver ==== //
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf1[TFT_HOR_RES * 40];

static lv_disp_drv_t disp_drv;

// ==== UI Components ==== //

// ==== Private variables ==== //
static bool ready = false;
// ==== Global variables ==== //
volatile UiMode currentMode = UI_MODE_ODO;
SemaphoreHandle_t displayMutex = xSemaphoreCreateMutex();

// ==== External variables ==== //

// ==== External function ==== //

// ==== Static function ==== //

static void disp_flush_callback(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    Screen.drawRegion((uint16_t *)&color_p->full, area->x1, area->y1, area->x2, area->y2);
    lv_disp_flush_ready(disp);
}

static void meter_anim_cb(void *needle, int32_t v)
{
    int needle_angle = map(v, 0, 10000, 0, 2500);
    int arc_value = map(v, 0, 10000, 0, 93);
    lv_img_set_angle(ui_img_needle, needle_angle);
    lv_arc_set_value(ui_Arc_rpm, arc_value);
}
static void update_ui()
{
    char speed_str[12];
    static int last_rpm = 0;
    int new_rpm = SpeedoData.rpm;
    if (SpeedoData.speed_kmph == -1)
        strcpy(speed_str, "---");
    else
        sprintf(speed_str, "%d", SpeedoData.speed_kmph);
    lv_label_set_text(ui_lbl_rpm_value, speed_str);
    // --- Animate meter needle ---
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)meter_anim_cb);
    lv_anim_set_values(&a, last_rpm, new_rpm);
    lv_anim_set_time(&a, 150);
    lv_anim_set_path_cb(&a, lv_anim_path_linear);
    lv_anim_start(&a);
    last_rpm = new_rpm;
}
static void updateData_task(void *param)
{
    while (true)
    {
        if (ready && currentMode == UI_MODE_ODO)
        {
            if (xSemaphoreTake(displayMutex, 0) == pdTRUE)
            {
                if (SpeedoData.speed_kmph <= 100)
                    SpeedoData.speed_kmph += 1;
                else
                    SpeedoData.speed_kmph = 0;
                SpeedoData.rpm = SpeedoData.speed_kmph * 100;
                update_ui();
                xSemaphoreGive(displayMutex);
            }
        }
        // Serial.println("update ui");
        vTaskDelay(200);
    }
}

static void mainUi_task(void *param)
{
    while (true)
    {
        if (currentMode == UI_MODE_ODO)
        {
            if (xSemaphoreTake(displayMutex, 0) == pdTRUE)
            {
                lv_timer_handler();
                xSemaphoreGive(displayMutex);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}
CanvasManager *canvasManager = new CanvasManager();
static void test_canvas()
{
    // lv_btn_create(lv_scr_act());

    // /*Create a buffer for the canvas*/
    // static lv_color_t cbuf[LV_CANVAS_BUF_SIZE_INDEXED_1BIT(120, 120)];

    // /*Create a canvas and initialize its palette*/
    // lv_obj_t *canvas = lv_canvas_create(lv_scr_act());
    // lv_canvas_set_buffer(canvas, cbuf, 120, 120, LV_IMG_CF_INDEXED_1BIT);
    // lv_canvas_set_palette(canvas, 0, LV_COLOR_CHROMA_KEY);
    // lv_canvas_set_palette(canvas, 1, lv_palette_main(LV_PALETTE_RED));

    // /*Create colors with the indices of the palette*/
    // lv_color_t c0;
    // lv_color_t c1;

    // c0.full = 0;
    // c1.full = 1;

    // /*Red background (There is no dedicated alpha channel in indexed images so LV_OPA_COVER is ignored)*/
    // lv_canvas_fill_bg(canvas, c1, LV_OPA_COVER);

    // /*Create hole on the canvas*/
    // uint32_t x;
    // uint32_t y;
    // for (y = 10; y < 30; y++)
    // {
    //     for (x = 5; x < 20; x++)
    //     {
    //         lv_canvas_set_px_color(canvas, x, y, c0);
    //     }
    // }
    // lv_obj_center(canvas);
    int id = canvasManager->createCanvas(50, 50, 4);
    if (id == -1 ) {
        Serial.println("Create canvas failed");
        return;
    }
    // lv_obj_t *canvas = CanvasManager::getCanvas(id);
    CanvasLvImpl *canvas = new CanvasLvImpl(canvasManager, id);
    if (canvas) {
        canvas->setBackgroundColor(RED);
        canvas->setForegroundColor(0xffff);
        canvas->FillRectangle(0, 0 , 20, 20, 1);
    }
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
    disp_drv.full_refresh = true;
    lv_disp_drv_register(&disp_drv);
    test_canvas();
    // ui_init();
    // gif_view_init();
    // lv_disp_set_bg_color(lv_disp_get_default(), PALETTE_RED);
    // lv_draw_rect_dsc_t rect_dsc;
    // lv_draw_rect_dsc_init(&rect_dsc);
    // rect_dsc.radius = 10;
    // rect_dsc.bg_opa = LV_OPA_COVER;
    // rect_dsc.bg_color = PALETTE_RED;
    // rect_dsc.border_width = 2;
    // rect_dsc.border_opa = LV_OPA_90;
    // rect_dsc.border_color = lv_color_white();
    // rect_dsc.shadow_width = 5;
    // rect_dsc.shadow_ofs_x = 5;
    // rect_dsc.shadow_ofs_y = 5;

    // static lv_color_t cbuf[LV_CANVAS_BUF_SIZE_INDEXED_1BIT(120, 120)];
    // lv_obj_t * canvas = lv_canvas_create(lv_scr_act());
    // lv_canvas_set_buffer(canvas, cbuf, 120, 120, LV_IMG_CF_INDEXED_1BIT);

    // lv_canvas_fill_bg(canvas, PALETTE_BLACK, LV_OPA_COVER);
    // lv_canvas_draw_rect(canvas, 0, 0, 50, 70, &rect_dsc);

    // lv_obj_center(canvas);

    // test canvas

    // needle_Animation(uic_img_needle, 200);
    // lv_timer_create([](lv_timer_t *t)
    //                 {
    //                    // Screen.fadeIn(1000);
    //                     lv_timer_del(t); }, 0, NULL);
    // lv_timer_create([](lv_timer_t *t)
    //                 { ready = true; }, 3000, NULL);
    xTaskCreate(mainUi_task, "mainUi_task", 4096, NULL, configMAX_PRIORITIES, NULL);
    // xTaskCreate(updateData_task, "updateData_task", 4096, NULL, configMAX_PRIORITIES - 2, NULL);
}
