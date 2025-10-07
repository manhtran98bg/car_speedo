#include <Arduino.h>
#include "FreeRTOS.h"
#include "Views/main_view.h"
#include "Drivers/screen_driver.h"
#include "Arduino_DriveBus_Library.h"
#include <AnimatedGIF.h>
#include "SD.h"

#include "FS.h"
// #include "SPIFFS.h"
#include <LittleFS.h>

#include "user_config.h"
#include "Audio.h"
#include "Views/gif_view.h"
#include "Services/audio_service.h"
#include "Services/mjpeg_player.h"

/*mjpeg & SD Card*/
#define MJPEG_FILENAME "/video/splash.mjpeg"
MjpegPlayer *video_player;

extern void app_audio_init(void);
extern BaseType_t app_audio_start_mp3_player(Stream *input, BaseType_t audioAssignCore);
// Audio audio;

static int displayBack(JPEGDRAW *pDraw)
{
  screen->draw16bitBeRGBBitmap(pDraw->x, pDraw->y, pDraw->pPixels, pDraw->iWidth, pDraw->iHeight);
  return 1;
}

static void fs_init()
{
  if (!LittleFS.begin(true))
  {
    Serial.println("LittleFS Mount Failed");
    return;
  }
}
File aFile;
static long lastCmd = 0;

static void testMjpeg()
{
  video_player = new MjpegPlayer(displayBack, true, 0, 0, 240, 240);
  video_player->begin(1); // chạy trên core 1
  video_player->playFile("/video/splash.mjpeg");
}
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
  audio_service_init();
  display_init();

  audio_play_file("/sound/start_1.mp3");
  testMjpeg();
  main_view_init();
  gif_request_show("/gif/8.gif");
}

void loop()
{
  // audio.loop();

  if (millis() - lastCmd > 30000)
  {
    lastCmd = millis();
    size_t idx = (size_t)(esp_random() % 10 + 1);
    char path[32];
    sprintf(path, "/gif/%d.gif", idx);
    gif_request_show(path);
  }
}
