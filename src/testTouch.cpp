#include "Arduino.h"

#include "Wire.h"
#include "CST816_TouchLib.h"

#define TOUCH_SCL GPIO_NUM_9
#define TOUCH_SDA GPIO_NUM_8
#define TOUCH_RST GPIO_NUM_0
#define TOUCH_INT GPIO_NUM_11

#define TOUCH_ADDR 0x15

using namespace MDO;

CST816Touch_SWMode oTouchController;

class MyTouchObserver : public TouchScreenObserver
{

public:
    virtual void gestureNotification(int iGestureId, int x, int y, bool bCurrentlyPressed)
    {
        Serial.print("Gesture ");
        Serial.print("detected: ");
        Serial.print(TouchScreenController::gestureIdToString((TouchScreenController::gesture_t)iGestureId));

        if ((iGestureId == (int)TouchScreenController::gesture_t::GESTURE_LONG_PRESS) ||
            (iGestureId == (int)TouchScreenController::gesture_t::GESTURE_DOUBLE_CLICK))
        {
            Serial.print(" at: (");
            Serial.print(x);
            Serial.print(",");
            Serial.print(y);
            Serial.print(")");
        }

        Serial.println();
    }
    virtual void touchNotification(int x, int y, bool bCurrentlyPressed)
    {
    }

    MyTouchObserver()
    {
    }
    virtual ~MyTouchObserver()
    {
    }
};

static MyTouchObserver oTouchObserver;	
static void touch_isr_callback()
{
}
void touch_init()
{
    Wire.begin(TOUCH_SDA, TOUCH_SCL, 400000);
    if (!oTouchController.begin(Wire, &oTouchObserver, TOUCH_INT, TOUCH_RST, TOUCH_ADDR))
    {
        Serial.println("Touch screen initialization failed..");
    }
    oTouchController.enableDoubleClickFactory_Elegant();
}

int touch_get_chip_id()
{
    TouchScreenController::device_type_t eDeviceType = TouchScreenController::device_type_t::DEVICE_UNKNOWN;
    oTouchController.getDeviceType(eDeviceType);
    Serial.print("Device is of type: ");
    Serial.println(oTouchController.deviceTypeToString(eDeviceType));
    return (int)eDeviceType;
}
void touch_process() {
    oTouchController.control();
}

