#include <Arduino.h>
#include "Views/main_view.h"
#include "Drivers/screen_driver.h"
#include <AnimatedGIF.h>
#include "Assets/splash_honda.h"

AnimatedGIF gif;

extern void GIFDraw(GIFDRAW *pDraw);
void setup()
{
  delay(2000);
  Serial.begin(115200);
  Serial.println("Main app running");
  display_init();
  // gif.begin(BIG_ENDIAN_PIXELS);
  // if (gif.open((uint8_t *)splash_honda, sizeof(splash_honda), GIFDraw))
  // {
  //   Serial.printf("Successfully opened GIF; Canvas size = %d x %d\n", gif.getCanvasWidth(), gif.getCanvasHeight());
  //   screen.startWrite();
  //   while (gif.playFrame(true, NULL))
  //   {
  //     yield();
  //   }
  //   gif.close();
  //   screen.endWrite();
  // }
  // screen.fillScreen(TFT_BLACK);
  // delay(100);
  register_display_flush_callback(display_flush_data);
  main_view_init();
}

void loop()
{
  Serial.println("check");
  main_view_process();
  delay(5);
}
