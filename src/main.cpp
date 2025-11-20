#include <Arduino.h>
#include "FreeRTOS.h"
#include "Views/main_view.h"
#include "Drivers/screen_driver.h"
#include "Arduino_DriveBus_Library.h"
#include <AnimatedGIF.h>
#include "SD.h"

#include "FS.h"
#include <LittleFS.h>
#include "SPIFFS.h"

#include "user_config.h"

#include "Views/gif_view.h"
#include "Services/audio_player.h"
#include "Services/mjpeg_player.h"

#include "Eyes/Face.h"

#include <LovyanGFX.hpp>
#include "LGFX_Screen.hpp"

/*mjpeg & SD Card*/
#define MJPEG_FILENAME "/video/splash.mjpeg"

static const char *splash_video_file = "/video/splash_2.mjpeg";
static const char *splash_audio_file = "/audio/splash_2.aac";

static long lastCmd = 0;

static int displayBack(JPEGDRAW *pDraw);

MjpegPlayer *videoPlayer;
AudioPlayer *audioPlayer;

Face face(240, 240, 60);

// Audio audio;


static int displayBack(JPEGDRAW *pDraw)
{
	int x1 = pDraw->x;
	int y1 = pDraw->y;
	int x2 = x1 + pDraw->iWidth - 1;
	int y2 = y1 + pDraw->iHeight - 1;
	Screen.drawRegion(pDraw->pPixels, x1, y1, x2, y2);
	return 1;
}

static void onVideoPlayDone(const char *file)
{
	Serial.printf("Done playing %s\n", file);
	if (strcasecmp(file, splash_video_file) == 0)
	{
		main_view_init();
		audioPlayer->playFile("/sound/1.mp3");
		gif_request_show("/gif/8.gif");
	}
}
void fsInit()
{
	Serial.println("Initializing file system...");
	if (!LittleFS.begin(true))
	{
		Serial.println("LittleFS Mount Failed");
		return;
	}
	Serial.println("File system initialized.");
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
	fsInit();
	Screen.begin();
	face.RandomBehavior = true;
	face.RandomBlink = true;
	face.RandomLook = true;
	// videoPlayer = new MjpegPlayer(displayBack, false, 0, 0, TFT_HOR_RES, TFT_VER_RES);
	// audioPlayer = new AudioPlayer();
	// videoPlayer->begin(1);
	// audioPlayer->begin(0);
	// videoPlayer->setOnPlayDoneCallback(onVideoPlayDone);
	// videoPlayer->playFile(splash_video_file);
	// audioPlayer->playFile(splash_audio_file);

	// lcd.init();
	// lcd.setRotation(0);
	// lcd.setColorDepth(16);
	// lcd.fillScreen(lcd.color565(0, 0, 0));
	// lcd.setTextColor(0);
	// lcd.setFont(&fonts::Font4);
	// lcd.drawString("string!", 50, 50);
	// sprite.setColorDepth(1);
	// sprite.createSprite(240, 240);
	// sprite.drawRect(0, 0, 65, 65, 1);
	// sprite.pushSprite(120, 120);
}

void loop()
{
	// if (millis() - lastCmd > 30000)
	// {
	// 	lastCmd = millis();
	// 	size_t idx = (size_t)(esp_random() % 10 + 1);
	// 	char path[32];
	// 	sprintf(path, "/gif/%d.gif", idx);
	// 	gif_request_show(path);
	// }
	face.Update();
}
