#pragma once
#include <lvgl.h>
#include <Arduino.h>

class CanvasManager
{
public:
    static constexpr int MAX_CANVAS = 2;

    struct CanvasEntry
    {
        lv_obj_t *obj = nullptr;
        void *buffer = nullptr;
        int w = 0;
        int h = 0;
        lv_img_cf_t format = LV_IMG_CF_UNKNOWN;
    };

    CanvasManager()
    {
        for (int i = 0; i < MAX_CANVAS; i++)
        {
            _entry[i].obj = nullptr;
            _entry[i].buffer = nullptr;
            _entry[i].w = 0;
            _entry[i].h = 0;
            _entry[i].format = LV_IMG_CF_UNKNOWN;
        }
    }

    ~CanvasManager()
    {
        // cleanup toàn bộ buffer + object
        for (int i = 0; i < MAX_CANVAS; i++)
        {
            if (_entry[i].obj)
                lv_obj_del(_entry[i].obj);

            if (_entry[i].buffer)
                free(_entry[i].buffer);
        }
    }

    bool valid(int id) const
    {
        return id >= 0 && id < MAX_CANVAS && _entry[id].obj != nullptr;
    }

    int createCanvas(int w, int h,
                     lv_img_cf_t cf = LV_IMG_CF_INDEXED_1BIT)
    {
        for (int i = 0; i < MAX_CANVAS; i++)
        {
            if (_entry[i].obj == nullptr)
            {
                size_t size = getBufferSize(w, h, cf);
                void *buf = malloc(size);
                if (!buf)
                    return -1;

                lv_obj_t *canvas = lv_canvas_create(lv_scr_act());
                if (!canvas)
                {
                    Serial.println("lv_canvas_create failed, returned NULL");
                    free(buf);
                    return -1;
                }

                lv_canvas_set_buffer(canvas, buf, w, h, cf);

                _entry[i].obj = canvas;
                _entry[i].buffer = buf;
                _entry[i].w = w;
                _entry[i].h = h;
                _entry[i].format = cf;

                return i;
            }
        }
        return -1; // full
    }

    void deleteCanvas(int id)
    {
        if (!valid(id))
            return;

        lv_obj_del(_entry[id].obj);
        free(_entry[id].buffer);

        _entry[id].obj = nullptr;
        _entry[id].buffer = nullptr;
        _entry[id].w = 0;
        _entry[id].h = 0;
        _entry[id].format = LV_IMG_CF_UNKNOWN;
    }

    lv_obj_t *getCanvas(int id) const
    {
        Serial.printf("get canvas with id %d\n", id);
        return valid(id) ? _entry[id].obj : nullptr;
    }

    void *getBuffer(int id) const
    {
        return valid(id) ? _entry[id].buffer : nullptr;
    }

private:
    size_t getBufferSize(int w, int h, lv_img_cf_t cf) const
    {
        switch (cf)
        {
        case LV_IMG_CF_TRUE_COLOR:
            return LV_CANVAS_BUF_SIZE_TRUE_COLOR(w, h);
        case LV_IMG_CF_TRUE_COLOR_ALPHA:
            return LV_CANVAS_BUF_SIZE_TRUE_COLOR_ALPHA(w, h);
        case LV_IMG_CF_INDEXED_1BIT:
            return LV_CANVAS_BUF_SIZE_INDEXED_1BIT(w, h);
        case LV_IMG_CF_INDEXED_2BIT:
            return LV_CANVAS_BUF_SIZE_INDEXED_2BIT(w, h);
        case LV_IMG_CF_INDEXED_4BIT:
            return LV_CANVAS_BUF_SIZE_INDEXED_4BIT(w, h);
        case LV_IMG_CF_INDEXED_8BIT:
            return LV_CANVAS_BUF_SIZE_INDEXED_8BIT(w, h);
        default:
            return w * h * sizeof(lv_color_t);
        }
    }

    CanvasEntry _entry[MAX_CANVAS];
};
