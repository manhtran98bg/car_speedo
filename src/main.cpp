#include <Arduino.h>
#include "Views/main_view.h"
#include "Drivers/screen_driver.h"
#include "Arduino_DriveBus_Library.h"
#include <AnimatedGIF.h>
#include "SD.h"

#include "FS.h"
#include "SPIFFS.h"
#include "user_config.h"

#include "Audio.h"

#include "Eyes/Face.h"

// Audio audio;



static void fs_init()
{
  if (!SPIFFS.begin(true))
  {
    Serial.println("SPIFFS Mount Failed");
    return;
  }
}
extern void main_view_init();
void setup()
{
  delay(2000);
  Serial.begin(115200);

  // Version Arduino Core
  Serial.print("Arduino Core Version: ");
  Serial.printf("%d.%d.%d\n", ESP_ARDUINO_VERSION_MAJOR, ESP_ARDUINO_VERSION_MINOR, ESP_ARDUINO_VERSION_PATCH);

  // ESP-IDF Version
  Serial.print("ESP-IDF Version: ");
  Serial.println(esp_get_idf_version());
  // fs_init();
  display_init();
  main_view_init();
  // // Create a new face
  // face = new Face(/* screenWidth = */ 240, /* screenHeight = */ 240, /* eyeSize = */ 60);
  // // Assign the current expression
  // face->Expression.GoTo_Normal();
  // face->Behavior.SetEmotion(eEmotions::Normal, 1.0);
  // face->Behavior.SetEmotion(eEmotions::Angry, 1.0);
  // face->Behavior.SetEmotion(eEmotions::Sad, 1.0);
  // face->Behavior.SetEmotion(eEmotions::Surprised, 1.0);
  // face->Behavior.SetEmotion(eEmotions::Happy, 1.0);
  // face->Behavior.SetEmotion(eEmotions::Glee, 1.0);
  // face->Behavior.SetEmotion(eEmotions::Scared, 1.0);
  // face->RandomBehavior = true;

  // // Automatically blink
  // face->RandomBlink = true;
  // // Set blink rate
  // face->Blink.Timer.SetIntervalMillis(4000);
  // face->RandomLook = true;
}

void loop()
{
  // face->Update();
}
