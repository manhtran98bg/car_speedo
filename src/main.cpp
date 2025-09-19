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

extern void gif_splash_view_init();
extern void app_audio_init(void);
extern BaseType_t app_audio_start_mp3_player(Stream *input, BaseType_t audioAssignCore);

// Audio audio;

static void fs_init()
{
  if (!SPIFFS.begin(true))
  {
    Serial.println("SPIFFS Mount Failed");
    return;
  }
}
File aFile;
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
  // test spiffs
  fs_init();
  // audio.setPinout(I2S_BCLK, I2S_WS, I2S_DIN);
  // audio.setVolume(10); // 0...21
  // display_init();
  // gif_splash_view_init();
  // main_view_init();
  app_audio_init();
  aFile = SPIFFS.open("/music.mp3");
  if (!aFile || aFile.isDirectory())
  {
    Serial.println("ERROR: Failed to open file for reading\n");
    return;
  }
  Serial.println("Start play audio task");
  BaseType_t ret = app_audio_start_mp3_player(&aFile, AUDIOASSIGNCORE);
  if (ret != pdPASS)
  {
    Serial.printf("Audio player task start failed: %d\n", ret);
    return;
  }
  // audio.connecttoFS(SPIFFS, "/music.mp3");
}

void loop()
{
  // audio.loop();
}
