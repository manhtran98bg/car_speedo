#include <Arduino.h>
#include "Views/main_view.h"
#include "Drivers/screen_driver.h"
void setup()
{
  delay(2000);
  Serial.begin(115200);
  Serial.println("Main app running");
  display_init();
  register_display_flush_callback(display_flush_data);
  main_view_init();
  
}

void loop()
{
  main_view_process();
}
