#include <Arduino.h>
#include "FreeRTOS.h"
#include "Views/main_view.h"
#include "Drivers/screen_driver.h"
#include "Arduino_DriveBus_Library.h"
#include <AnimatedGIF.h>
#include "SD.h"

#include "FS.h"
#include <LittleFS.h>

#include "user_config.h"

#include "Views/gif_view.h"
#include "Services/audio_player.h"
#include "Services/mjpeg_player.h"

/*mjpeg & SD Card*/
#define MJPEG_FILENAME "/video/splash.mjpeg"

static const char *splash_video_file = "/video/splash_2.mjpeg";
static const char *splash_audio_file = "/sound/start_2.aac";

static long lastCmd = 0;

static int displayBack(JPEGDRAW *pDraw);

MjpegPlayer *videoPlayer = new MjpegPlayer(displayBack, false, 0, 0, TFT_HOR_RES, TFT_VER_RES);
AudioPlayer *audioPlayer = new AudioPlayer();

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
	// if (!LittleFS.begin(true))
	// {
	// 	Serial.println("LittleFS Mount Failed");
	// 	return;
	// }
	Serial.println("LittleFS Mount Success!");
	Screen.begin();
	main_view_init();
	// videoPlayer->begin(1);
	// audioPlayer->begin(0);

	// videoPlayer->setOnPlayDoneCallback(onVideoPlayDone);
	// videoPlayer->playFile(splash_video_file);
	// videoPlayer->playFile(splash_audio_file);
}

void loop()
{

	Serial.println("loop");
	delay(1000);
	// if (millis() - lastCmd > 30000)
	// {
	// 	lastCmd = millis();
	// 	size_t idx = (size_t)(esp_random() % 10 + 1);
	// 	char path[32];
	// 	sprintf(path, "/gif/%d.gif", idx);
	// 	gif_request_show(path);
	// }
}
