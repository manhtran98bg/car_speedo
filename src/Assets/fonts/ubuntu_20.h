/*******************************************************************************
 * Size: 20 px
 * Bpp: 1
 * Opts: --bpp 1 --size 20 --no-compress --stride 1 --align 1 --font UbuntuCondensed-Regular.ttf --symbols 0123456789 --format lvgl -o ubuntu_20.c
 ******************************************************************************/

#ifdef __has_include
    #if __has_include("lvgl.h")
        #ifndef LV_LVGL_H_INCLUDE_SIMPLE
            #define LV_LVGL_H_INCLUDE_SIMPLE
        #endif
    #endif
#endif

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
    #include "lvgl.h"
#else
    #include "lvgl/lvgl.h"
#endif



#ifndef UBUNTU_20
#define UBUNTU_20 1
#endif

#if UBUNTU_20

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0030 "0" */
    0x38, 0xdb, 0x1e, 0x3c, 0x78, 0xf1, 0xe3, 0xc7,
    0x8f, 0x1e, 0x36, 0xc7, 0x0,

    /* U+0031 "1" */
    0x37, 0xfb, 0x33, 0x33, 0x33, 0x33, 0x33,

    /* U+0032 "2" */
    0xfa, 0x30, 0xc3, 0xc, 0x71, 0x8e, 0x71, 0x8e,
    0x30, 0xc3, 0xf0,

    /* U+0033 "3" */
    0x7c, 0x9c, 0x18, 0x30, 0x61, 0x8e, 0x7, 0x6,
    0xc, 0x18, 0x30, 0xcf, 0x0,

    /* U+0034 "4" */
    0xc, 0x38, 0x71, 0x62, 0xcd, 0x93, 0x66, 0xcd,
    0xfc, 0x30, 0x60, 0xc1, 0x80,

    /* U+0035 "5" */
    0x7d, 0x86, 0x18, 0x61, 0xc7, 0x87, 0xc, 0x30,
    0xc3, 0x1b, 0xc0,

    /* U+0036 "6" */
    0xc, 0x71, 0x83, 0xc, 0x1f, 0x33, 0x63, 0xc7,
    0x8f, 0x1e, 0x36, 0xc7, 0x0,

    /* U+0037 "7" */
    0xfe, 0x1c, 0x30, 0x61, 0x83, 0xc, 0x18, 0x30,
    0x61, 0x83, 0x6, 0xc, 0x0,

    /* U+0038 "8" */
    0x3d, 0x8f, 0x1e, 0x3c, 0x6d, 0x8e, 0x3c, 0xcd,
    0x8f, 0x1e, 0x36, 0xc7, 0x0,

    /* U+0039 "9" */
    0x78, 0xdb, 0x1e, 0x3c, 0x78, 0xd9, 0xbf, 0x6,
    0x1c, 0x30, 0xe7, 0x8c, 0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 138, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 13, .adv_w = 138, .box_w = 4, .box_h = 14, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 20, .adv_w = 138, .box_w = 6, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 31, .adv_w = 138, .box_w = 7, .box_h = 14, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 44, .adv_w = 138, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 57, .adv_w = 138, .box_w = 6, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 68, .adv_w = 138, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 81, .adv_w = 138, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 94, .adv_w = 138, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 107, .adv_w = 138, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = 0}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/



/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 48, .range_length = 10, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache_u_20;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 1,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache_u_20
#endif

};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t ubuntu_20 = {
#else
lv_font_t ubuntu_20 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 14,          /*The maximum line height required by the font*/
    .base_line = 0,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -2,
    .underline_thickness = 2,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if UBUNTU_20*/
