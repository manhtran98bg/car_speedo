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
#include "Services/audio_player.h"
#include "Services/mjpeg_player.h"

/*mjpeg & SD Card*/
#define MJPEG_FILENAME "/video/splash.mjpeg"

static const char *splash_video_file = "/video/splash_2.mjpeg";
static const char *splash_audio_file = "/sound/start_2.aac";
MjpegPlayer *video_player;
AudioPlayer *audio_player;

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

static long lastCmd = 0;

void onVideoPlayDone(const char * file)
{
  Serial.printf("Done playing %s\n", file);
  if (strcasecmp(file, splash_video_file)  == 0) {
    main_view_init();
    audio_player->playFile("/sound/1.mp3");
    gif_request_show("/gif/8.gif");
  }
    
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
  fs_init();
  display_init();

  video_player = new MjpegPlayer(displayBack, true, 0, 0, 240, 240);
  video_player->begin(1);
  video_player->setOnPlayDoneCallback(onVideoPlayDone);
  audio_player = new AudioPlayer();
  audio_player->begin(0);

  video_player->playFile(splash_video_file);
  audio_player->playFile(splash_audio_file);
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
