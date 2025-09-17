#include <Arduino.h>
#include "Views/main_view.h"
#include "Drivers/screen_driver.h"
#include "Arduino_DriveBus_Library.h"
#include <AnimatedGIF.h>
#include "SD.h"

extern void gif_splash_view_init();
void setup()
{
  delay(2000);
  Serial.begin(115200);
  display_init();
  gif_splash_view_init();
  main_view_init();

  // test sdcard
  // int attempts = 0;
  // int maxAttempts = 50;
  // int delayBetweenAttempts = 300;
  // bool isblinked = false;
  // pinMode(GPIO_NUM_40, OUTPUT);
  // SPI.begin(GPIO_NUM_41, GPIO_NUM_48, GPIO_NUM_47, GPIO_NUM_40);
  // while (!SD.begin(GPIO_NUM_40, SPI))
  // {
  //   Serial.printf("SD Card mount failed! (attempt %d of %d)\r\n", attempts, maxAttempts);
  //   isblinked = !isblinked;
  //   attempts++;
  //   // if (isblinked)
  //   // {
  //   //   tft.setTextColor(TFT_WHITE, TFT_BLACK);
  //   // }
  //   // else
  //   // {
  //   //   tft.setTextColor(TFT_BLACK, TFT_WHITE);
  //   // }
  //   // tft.drawString("INSERT SD", tft.width() / 2, tft.height() / 2);

  //   if (attempts > maxAttempts)
  //   {
  //     Serial.println("Giving up");
  //   }
  //   delay(delayBetweenAttempts);
  // }
}

void loop()
{
}


