#include "CanvasImpl.h"
#include "CanvasManager.hpp"

static inline lv_color_t lv_color_from565(uint16_t c)
{
    uint8_t r = (c >> 11) & 0x1F; // 5 bit red
    uint8_t g = (c >> 5) & 0x3F;  // 6 bit green
    uint8_t b = c & 0x1F;         // 5 bit blue

    r = (r * 527 + 23) >> 6; // convert 5-bit → 8-bit
    g = (g * 259 + 33) >> 6; // convert 6-bit → 8-bit
    b = (b * 527 + 23) >> 6; // convert 5-bit → 8-bit
    Serial.printf("Color r %d, g %d, b %d\n", r, g, b);
    return lv_color_make(r, g, b);
}

void CanvasImpl::FillRectangle(int32_t x0, int32_t y0,
                               int32_t x1, int32_t y1,
                               uint16_t color)
{
    LGFX_Sprite *spr = _driver->getSprite(_id);
    if (!spr)
        return;
    spr->fillRect(x0, y0, x1, y1, color);
}

void CanvasImpl::FillTriangle(int32_t x0, int32_t y0,
                              int32_t x1, int32_t y1,
                              int32_t x2, int32_t y2,
                              uint16_t color)
{
    LGFX_Sprite *spr = _driver->getSprite(_id);
    if (!spr)
        return;
    spr->fillTriangle(x0, y0, x1, y1, x2, y2, color);
}

void CanvasImpl::drawFastHLine(int32_t x, int32_t y, int32_t w,
                               uint16_t color)
{
    LGFX_Sprite *spr = _driver->getSprite(_id);
    if (!spr)
        return;
    spr->drawFastHLine(x, y, w, color);
}

void CanvasImpl::clear(uint16_t color)
{
    LGFX_Sprite *spr = _driver->getSprite(_id);
    if (!spr)
        return;
    spr->fillScreen(color);
}
void CanvasImpl::push(int x, int y)
{
    LGFX_Sprite *spr = _driver->getSprite(_id);
    if (!spr)
        return;
    spr->pushSprite(x, y);
}
int32_t CanvasImpl::width() const
{
    LGFX_Sprite *spr = _driver->getSprite(_id);
    if (!spr)
        return 0;
    return spr->width();
}

int32_t CanvasImpl::height() const
{
    LGFX_Sprite *spr = _driver->getSprite(_id);
    if (!spr)
        return 0;
    return spr->height();
}

void CanvasImpl::setBackgroundColor(uint16_t color)
{
    LGFX_Sprite *spr = _driver->getSprite(_id);
    if (!spr)
        return;
    spr->setPaletteColor(0, color);
}
void CanvasImpl::setForegroundColor(uint16_t color)
{
    LGFX_Sprite *spr = _driver->getSprite(_id);
    if (!spr)
        return;
    spr->setPaletteColor(1, color);
}

void CanvasLvImpl::FillRectangle(int32_t x0, int32_t y0,
                                 int32_t x1, int32_t y1,
                                 uint16_t color)
{
    lv_obj_t *canvas = _canvasManager->getCanvas(_id);
    if (!canvas)
    {
        Serial.println("canvas is null");
        return;
    }

    // uint32_t x;
    // uint32_t y;
    // for (y = 10; y < 30; y++)
    // {
    //     for (x = 5; x < 20; x++)
    //     {
    //         lv_canvas_set_px_color(canvas, x, y, _fgColor);
    //     }
    // }
    int16_t  w = x1 - x0;
    int16_t  h = y1 - y0;
    lv_color_t fillColor = (color) ? _fgColor : _bgColor;

    lv_draw_rect_dsc_t rect_dsc;
    lv_draw_rect_dsc_init(&rect_dsc);
    rect_dsc.radius = 0;
    rect_dsc.bg_opa = LV_OPA_COVER;
    rect_dsc.bg_color = lv_palette_main(LV_PALETTE_BLUE);
    rect_dsc.border_width = 2;
    rect_dsc.border_opa = LV_OPA_90;
    rect_dsc.border_color = lv_palette_main(LV_PALETTE_BLUE);;
    rect_dsc.shadow_width = 5;
    rect_dsc.shadow_ofs_x = 5;
    rect_dsc.shadow_ofs_y = 5;
    Serial.printf("rect %d %d %d %d\n", x0, y0, w, h);
    lv_canvas_draw_rect(canvas, x0, y0, w, h, &rect_dsc);
    lv_obj_center(canvas);
    // spr->fillRect(x0, y0, x1, y1, color);
}

void CanvasLvImpl::FillTriangle(int32_t x0, int32_t y0,
                                int32_t x1, int32_t y1,
                                int32_t x2, int32_t y2,
                                uint16_t color)
{
    // LGFX_Sprite *spr = _driver->getSprite(_id);
    // if (!spr)
    //     return;
    // spr->fillTriangle(x0, y0, x1, y1, x2, y2, color);
}

void CanvasLvImpl::drawFastHLine(int32_t x, int32_t y, int32_t w,
                                 uint16_t color)
{
    // LGFX_Sprite *spr = _driver->getSprite(_id);
    // if (!spr)
    //     return;
    // spr->drawFastHLine(x, y, w, color);
}

void CanvasLvImpl::clear(uint16_t color)
{
}
void CanvasLvImpl::push(int x, int y)
{
}
int32_t CanvasLvImpl::width() const
{
    // lv_obj_t *canvas = _canvasManager->getCanvas(_id);
    // if (!canvas)
    return 0;
}

int32_t CanvasLvImpl::height() const
{
    // lv_obj_t *canvas = _canvasManager->getCanvas(_id);
    // if (!canvas)
    return 0;
}

void CanvasLvImpl::setBackgroundColor(uint16_t color)
{
    lv_obj_t *canvas = _canvasManager->getCanvas(_id);
    if (!canvas)
        return;
    lv_canvas_set_palette(canvas, 0, lv_color_from565(color));
    _bgColor.full = 0;
    lv_canvas_fill_bg(canvas, _bgColor, LV_OPA_COVER);
}
void CanvasLvImpl::setForegroundColor(uint16_t color)
{
    lv_obj_t *canvas = _canvasManager->getCanvas(_id);
    if (!canvas)
        return;
    lv_canvas_set_palette(canvas, 1, lv_color_from565(color));
    _fgColor.full = 1;
}