#include "CanvasImpl.h"

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
        return ;
    spr->setPaletteColor(0, color);
}
void CanvasImpl::setForegroundColor(uint16_t color)
{
    LGFX_Sprite *spr = _driver->getSprite(_id);
    if (!spr)
        return ;
    spr->setPaletteColor(1, color);
}
