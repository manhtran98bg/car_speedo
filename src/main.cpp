#include <Arduino.h>
#include "FreeRTOS.h"
#include "Views/main_view.h"
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

#include "Drivers/Display/ScreenDriver.h"
#include "Drivers/Display/CanvasLvgl.h"
#include "Drivers/Display/CanvasLGFX.h"
#include "Drivers/Display/CanvasManagerLvgl.h"
#include "Drivers/Display/CanvasManagerLGFX.h"
#include "Face.h"

/*mjpeg & SD Card*/
#define MJPEG_FILENAME "/video/splash.mjpeg"

static const char *splash_video_file = "/video/splash_2.mjpeg";
static const char *splash_audio_file = "/audio/splash_2.aac";

static long lastCmd = 0;

static int displayBack(JPEGDRAW *pDraw);

MjpegPlayer *videoPlayer;
AudioPlayer *audioPlayer;

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

ICanvasManager *canvasManager = new CanvasManagerLGFX();
Face *face;
ICanvas *canvas;

void setup()
{
	Serial.begin(115200);

	// Version Arduino Core
	Serial.print("Arduino Core Version: ");
	Serial.printf("%d.%d.%d\n", ESP_ARDUINO_VERSION_MAJOR, ESP_ARDUINO_VERSION_MINOR, ESP_ARDUINO_VERSION_PATCH);

	// ESP-IDF Version
	Serial.print("ESP-IDF Version: ");
	Serial.println(esp_get_idf_version());
	
	Screen.begin();


	int id = canvasManager->createCanvas(240, 240, 1);
	if (id == -1)
	{
		Serial.println("Create canvas failed");
		return;
	}
	canvas = canvasManager->getCanvasWrapper(id);
	if (canvas)
	{
		face = new Face(canvas, 50, 240, 240, BLACK, YELLOW);
	}
	// videoPlayer = new MjpegPlayer(displayBack, false, 0, 0, TFT_HOR_RES, TFT_VER_RES);
	// audioPlayer = new AudioPlayer();
	// videoPlayer->begin(1);
	// audioPlayer->begin(0);
	// videoPlayer->setOnPlayDoneCallback(onVideoPlayDone);
	// videoPlayer->playFile(splash_video_file);
	// audioPlayer->playFile(splash_audio_file);
}

void loop()
{
	static uint32_t lastCheckHeap = millis();
	// if (millis() - lastCmd > 30000)
	// {
	// 	lastCmd = millis();
	// 	size_t idx = (size_t)(esp_random() % 10 + 1);
	// 	char path[32];
	// 	sprintf(path, "/gif/%d.gif", idx);
	// 	gif_request_show(path);
	// }
	if (millis() - lastCheckHeap >= 1000) {
		Serial.printf("Free heap %u\n", esp_get_free_heap_size());
		lastCheckHeap = millis();
	}
	face->Update();
	delay(5);
}
