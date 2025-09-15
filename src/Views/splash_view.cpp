#include "Arduino.h"
#include "lvgl.h"

lv_obj_t * splash_scr;
void make_splash_view()
{
    splash_scr = lv_obj_create(NULL);
    LV_IMG_DECLARE(splash240p);
    lv_obj_t *img;
    img = lv_gif_create(splash_scr);
    lv_gif_set_src(img, &splash240p);
    lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
}