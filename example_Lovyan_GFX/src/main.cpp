#include "Arduino.h"
#include "SPIFFS.h"

#include <LovyanGFX.hpp>
#include <lgfx_tdisplay_s3.h>

#include "pin_config.h"

static LGFX lcd; // LGFXのインスタンスを作成。
static LGFX_Sprite sprite_sshiki(&lcd);
static LGFX_Sprite sprite_parrot(&lcd);

void setup()
{
  Serial.begin(115200);
  Serial.println("Hello T-Display-S3");

  if (!SPIFFS.begin(true))
  {
    Serial.println("SPIFFS Failed to mount file system");
    while (true)
    {
      delay(1000);
    }
  }

  // T-Display S3 用おまじない
  pinMode(PIN_POWER_ON, OUTPUT);
  digitalWrite(PIN_POWER_ON, HIGH);
  ledcSetup(0, 2000, 8);
  ledcAttachPin(PIN_LCD_BL, 0);
  ledcWrite(0, 255); /* Screen brightness can be modified by adjusting this parameter. (0-255) */

  // LCD Init
  lcd.init();
  lcd.setRotation(1);
  lcd.setTextColor(TFT_BLACK);
  lcd.setTextSize(1);

  // Sprite Init for sshiki.jpg
  sprite_sshiki.setColorDepth(24);
  sprite_sshiki.createSprite(200, 133);
  sprite_sshiki.fillScreen(TFT_WHITE);
  sprite_sshiki.drawJpgFile(
      SPIFFS, "/sshiki.jpg",
      0, 0,
      sprite_sshiki.width(), sprite_sshiki.height());

  // Sprite initialize for parrot
  sprite_parrot.setColorDepth(24);
  sprite_parrot.createSprite(128, 128);

  Serial.println("Setup done.");
}

void loop(void)
{
  for (int i = 0; i < 8; i++)
  {
    int direction = i % 4;

    lcd.clear(TFT_WHITE);
    lcd.setRotation(direction);
    lcd.setCursor(0, 0);
    lcd.println("Hello, world! T-Display S3");
    direction++;
    if (direction == 4)
      direction = 0;
    delay(500);
  }

  lcd.fillScreen(TFT_WHITE);
  lcd.setRotation(1);

  sprite_sshiki.pushSprite(0, 0);

  for (int i = 0; i < 100; i++)
  {
    int num = i % 10;
    String filename = String("/parrot-") + num + ".jpg";
    sprite_parrot.drawJpgFile(
        SPIFFS, filename.c_str(), 0, 0,
        sprite_parrot.width(), sprite_parrot.height());
    sprite_parrot.pushSprite(200, 0);
    delay(10);
  }
}
