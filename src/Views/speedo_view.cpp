#include "Arduino.h"
#include "user_config.h"
#include "lvgl.h"
#include "Assets/fonts/ubuntu_20.h"
#include "Assets/fonts/ubuntu_24_n.h"
#include "Assets/fonts/ubuntu_50.h"
#include "Assets/fonts/ubuntu_200.h"
#include "Assets/fonts/font_awesome_icons_small.h"
#include "Models/data_model.h"
// Font Awesome symbols
#define FUEL_SYMBOL "\xEF\x94\xAF"

#define BAT_SYMBOL "\xEF\x97\x9F"

#define OIL_SYMBOL "\xEF\x98\x93"

#define TEMP_SYMBOL "\xEF\x98\x94"

// ==== UI Components ==== //
lv_obj_t *speedo_scr;
static lv_obj_t *speed_meter, *speed_label, *fuel_arc;

lv_meter_indicator_t *speed_indic;

static void draw_speed_meter()
{
    speed_meter = lv_meter_create(speedo_scr);
    lv_obj_set_style_pad_all(speed_meter, 0, 10);
    lv_obj_set_style_bg_color(speed_meter, PALETTE_BLACK, 0);
    lv_obj_set_style_border_width(speed_meter, 0, 0);
    lv_obj_center(speed_meter);
    lv_obj_set_size(speed_meter, 240, 240);

    lv_obj_remove_style(speed_meter, NULL, LV_PART_INDICATOR);

    // Add a scale
    lv_meter_scale_t *scale = lv_meter_add_scale(speed_meter);
    lv_meter_set_scale_ticks(speed_meter, scale, 161, 0, 0, PALETTE_WHITE);
    lv_meter_set_scale_major_ticks(speed_meter, scale, 2, 1, 10, PALETTE_WHITE, -150);
    lv_meter_set_scale_range(speed_meter, scale, 0, 160, 270, 135);

    lv_meter_scale_t *scale_twenties = lv_meter_add_scale(speed_meter);
    lv_meter_set_scale_ticks(speed_meter, scale_twenties, 9, 0, 0, PALETTE_WHITE);
    lv_meter_set_scale_major_ticks(speed_meter, scale_twenties, 1, 2, 15, PALETTE_WHITE, 15);
    lv_meter_set_scale_range(speed_meter, scale_twenties, 0, 160, 270, 135);
    // Add a WHITE OUTLINE
    lv_meter_indicator_t *outline = lv_meter_add_arc(speed_meter, scale, OUTLINE_WIDTH, PALETTE_WHITE, 2);

    lv_meter_set_indicator_start_value(speed_meter, outline, 0);
    lv_meter_set_indicator_end_value(speed_meter, outline, 160);

    lv_obj_set_style_text_font(speed_meter, &ubuntu_20, LV_PART_TICKS);
    lv_obj_set_style_text_color(speed_meter, PALETTE_WHITE, LV_PART_TICKS);
    // Add a speed indicator
    speed_indic = lv_meter_add_needle_line(speed_meter, scale, 5, PALETTE_RED, -10);
    // Add inner circle show speed numer
    lv_obj_t *inner_circle = lv_obj_create(speedo_scr);
    lv_obj_set_size(inner_circle, 100, 100);
    lv_obj_center(inner_circle);
    lv_obj_set_style_bg_color(inner_circle, PALETTE_BLACK, 0);
    lv_obj_set_style_bg_opa(inner_circle, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(inner_circle, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_border_width(inner_circle, 2, 0);
    lv_obj_set_style_border_color(inner_circle, PALETTE_GREY, 0);
    lv_obj_set_style_shadow_width(inner_circle, 60, 0);
    lv_obj_set_style_shadow_color(inner_circle, PALETTE_GREY, 0);
}

void draw_fuel_arc(void)
{
    fuel_arc = lv_arc_create(speedo_scr);
    lv_obj_set_size(fuel_arc, 223, 223);
    lv_arc_set_rotation(fuel_arc, 65);
    lv_arc_set_bg_angles(fuel_arc, 0, 50);
    lv_arc_set_range(fuel_arc, 0, 100);
    lv_obj_center(fuel_arc);
    lv_arc_set_mode(fuel_arc, LV_ARC_MODE_REVERSE);

    lv_obj_set_style_arc_color(fuel_arc, PALETTE_WHITE, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(fuel_arc, PALETTE_DARK_GREY, LV_PART_MAIN);
    lv_obj_set_style_arc_rounded(fuel_arc, false, LV_PART_MAIN);
    lv_obj_set_style_arc_rounded(fuel_arc, false, LV_PART_INDICATOR);
    lv_obj_set_style_arc_width(fuel_arc, 4, LV_PART_MAIN);
    lv_obj_set_style_arc_width(fuel_arc, 4, LV_PART_INDICATOR);
    lv_obj_remove_style(fuel_arc, NULL, LV_PART_KNOB);

    lv_arc_set_value(fuel_arc, 50);
}
static void draw_speed_number(void)
{
    static lv_style_t style_speed_text, style_unit_text;
    lv_style_init(&style_speed_text);
    lv_style_set_text_font(&style_speed_text, &ubuntu_50);
    lv_style_set_text_color(&style_speed_text, PALETTE_WHITE);

    lv_style_init(&style_unit_text);
    lv_style_set_text_font(&style_unit_text, &ubuntu_24_n);
    lv_style_set_text_color(&style_unit_text, PALETTE_WHITE);

    speed_label = lv_label_create(speedo_scr);
    lv_obj_add_style(speed_label, &style_speed_text, 0);
    lv_obj_align(speed_label, LV_ALIGN_CENTER, 0, -5);

    lv_obj_t *speed_unit = lv_label_create(speedo_scr);

    lv_obj_add_style(speed_unit, &style_unit_text, 0);
    lv_obj_align(speed_unit, LV_ALIGN_CENTER, 0, 25);
    lv_label_set_text(speed_unit, "kmph");
}

static void draw_icon(void)
{
    lv_obj_t *fuel_icon = lv_label_create(speedo_scr);
    static lv_style_t style_icon;
    lv_style_init(&style_icon);
    lv_style_set_text_font(&style_icon, &font_awesome_icons_small);
    lv_style_set_text_color(&style_icon, PALETTE_GREY);

    lv_label_set_text(fuel_icon, TEMP_SYMBOL);
    lv_obj_add_style(fuel_icon, &style_icon, 0);
    lv_obj_align(fuel_icon, LV_ALIGN_CENTER, 0, 80);
}

static void meter_anim_cb(void * indic, int32_t v)
{
    // speed_meter là lv_obj_t* toàn cục của bạn
    lv_meter_set_indicator_value(speed_meter, (lv_meter_indicator_t *)indic, v);
}

void update_speedo_view()
{
    static int last_speed = 0;   // lưu giá trị cũ (ban đầu = 0)

    int new_speed = SpeedoData.speed_kmph;

    // --- Animate meter needle ---
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, speed_indic);   // indicator cần update
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t) meter_anim_cb);
    lv_anim_set_values(&a, last_speed, new_speed);  // từ giá trị cũ -> mới
    lv_anim_set_time(&a, 200);     // thời gian animation (ms)
    lv_anim_set_path_cb(&a, lv_anim_path_linear); // smooth hơn linear
    lv_anim_start(&a);

    // --- Update text label ---
    char speed_text[4];
    sprintf(speed_text, "%d", new_speed);
    lv_label_set_text(speed_label, speed_text);

    // Lưu giá trị mới cho lần update tiếp theo
    last_speed = new_speed;

    // TODO: update temp, fuel giống vậy
}

void make_speedo_view()
{
    speedo_scr = lv_obj_create(NULL);
    if (speedo_scr == NULL)
    {
        Serial.println("Create speedo screen failed");
        return;
    }
    lv_obj_set_style_bg_color(speedo_scr, PALETTE_BLACK, 0);
    draw_speed_meter();
    draw_fuel_arc();
    draw_speed_number();
    draw_icon();
}
