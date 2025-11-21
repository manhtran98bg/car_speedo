#pragma once
#include "Canvas.h"
#include "Drivers/Display/ScreenDriver.h"
#include "CanvasManager.hpp"
#include "lvgl.h"


class CanvasImpl : public ICanvas {
public:
    CanvasImpl(ScreenDriver* driver, int spriteId)
        : _driver(driver), _id(spriteId) {}

    void FillRectangle(int32_t x0, int32_t y0,
                       int32_t w, int32_t h,
                       uint16_t color) override;


    void FillTriangle(int32_t x0, int32_t y0,
                      int32_t x1, int32_t y1,
                      int32_t x2, int32_t y2,
                      uint16_t color) override;
    void drawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint16_t color) override;
    void drawFastHLine(int32_t x, int32_t y, int32_t w,
                       uint16_t color) override;
    
    void push(int x, int y) override;

    

    void clear(uint16_t color) override;

    int32_t width (void) const override;
    int32_t height (void) const override;

    void setBackgroundColor (uint16_t color) override;
    void setForegroundColor (uint16_t color) override;

private:
    ScreenDriver* _driver;
    int _id;
    int _x = 0;
    int _y = 0;
};


class CanvasLvImpl : public ICanvas {
public:
    CanvasLvImpl(CanvasManager *canvasManager, int id) 
        : _canvasManager(canvasManager), _id(id) {}

    void FillRectangle(int32_t x0, int32_t y0,
                       int32_t x1, int32_t y1,
                       uint16_t color) override;

    void FillTriangle(int32_t x0, int32_t y0,
                      int32_t x1, int32_t y1,
                      int32_t x2, int32_t y2,
                      uint16_t color) override;
    void drawLine(int32_t x0, int32_t y0, 
                    int32_t x1, int32_t y1, uint16_t color) override;

    void drawFastHLine(int32_t x, int32_t y,
                       int32_t w, uint16_t color) override;

    void clear(uint16_t color) override;

    int32_t width()  const override;
    int32_t height() const override;

    void setBackgroundColor(uint16_t color) override;
    void setForegroundColor(uint16_t color) override;

    void push(int x, int y) override;

private:
    CanvasManager *_canvasManager;
    int32_t _clip_l = 0, _clip_r = -1, _clip_t = 0, _clip_b = -1;
    int _id;
    lv_color_t _fgColor;
    lv_color_t _bgColor;
};