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

Face *face;
const eEmotions moods[] = {	
  eEmotions::Normal,
	eEmotions::Angry,
	eEmotions::Glee,
	eEmotions::Happy,
	eEmotions::Sad,
	eEmotions::Worried,
	eEmotions::Focused,
	eEmotions::Annoyed, //Khó chịu
	eEmotions::Surprised,
	eEmotions::Skeptic, //Người hoài nghi
	eEmotions::Frustrated, //Bực bội
	eEmotions::Unimpressed, //Không ấn tượng
	eEmotions::Sleepy, //Buồn ngủ
	eEmotions::Suspicious, //Khả nghi
	eEmotions::Squint, //Nheo mắt
	eEmotions::Furious, //Giận dữ
	eEmotions::Scared, //Sợ hãi
	eEmotions::Awe, //Kinh ngạc
}; 
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
  // main_view_init();
  face = new Face(/* screenWidth = */ 240, /* screenHeight = */ 240, /* eyeSize = */ 60);
  face->RandomBehavior = true;
  face->RandomBlink = true;
  face->RandomLook = true;
}

void loop()
{
  face->Update();
}
