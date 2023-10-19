#pragma once

#include <functional>

#include "Arduino.h"

#include "Arduino_GFX_Library.h"

#include "SPIFFS.h"

#include "pin_config.h"
#include "img_logo.h"
#include "JpegFunc.h"

#define minimum(a, b) (((a) < (b)) ? (a) : (b))

Arduino_DataBus *bus;
Arduino_GFX *gfx;
Arduino_GFX *canvas;

static int jpegDrawCallback(JPEGDRAW *pDraw)
{
    Serial.printf("Draw pos = %d,%d. size = %d x %d\n", pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight);
    gfx->draw16bitBeRGBBitmap(pDraw->x, pDraw->y, pDraw->pPixels, pDraw->iWidth, pDraw->iHeight);
    return 1;
}

class TDisplay
{
public:
    TDisplay()
    {
        bus = new Arduino_ESP32LCD8(
            7 /* DC */,
            6 /* CS */,
            8 /* WR */,
            9 /* RD */,
            39 /* D0 */,
            40 /* D1 */,
            41 /* D2 */,
            42 /* D3 */,
            45 /* D4 */,
            46 /* D5 */,
            47 /* D6 */,
            48 /* D7 */);
        gfx = new Arduino_ST7789(
            bus,
            5 /* RST */,
            0 /* rotation */,
            true /* IPS */,
            170 /* width */,
            320 /* height */,
            35 /* col offset 1 */,
            0 /* row offset 1 */,
            35 /* col offset 2 */,
            0 /* row offset 2 */);

        pinMode(PIN_POWER_ON, OUTPUT);
        digitalWrite(PIN_POWER_ON, HIGH);
        ledcSetup(0, 2000, 8);
        ledcAttachPin(PIN_LCD_BL, 0);
        ledcWrite(0, 255); /* Screen brightness can be modified by adjusting this parameter. (0-255) */

        gfx->begin();
        gfx->setRotation(1);
        gfx->fillScreen(WHITE);
        gfx->draw16bitRGBBitmap(0, 0, (uint16_t *)img_logo, 320, 172);
        gfx->flush();
        delay(1000);
    }

    void drawJpeg(const char *filename, int x, int y)
    {
        jpegDraw(filename, jpegDrawCallback, true, x, y, gfx->width(), gfx->height());
    }

    void test()
    {
        Serial.println("Hello T-Display-S3");

        Serial.println("Testing text...");
        gfx->fillScreen(WHITE);
        gfx->setCursor(0, 0);
        gfx->setTextSize(1);
        gfx->setTextColor(BLACK);
        gfx->println("Hello T-Display-S3");
        gfx->flush();
        delay(5000);

        Serial.println("Testing jpeg image...");
        // for (int i = 0; i < 10; i++)
        // {
        //     String filename = String("/parrot-") + i + ".jpg";
        //     fs::File file = SPIFFS.open(filename, FILE_READ);
        //     if (!file)
        //     {
        //         Serial.printf("Failed to open image file: %s\n", filename.c_str());
        //     }
        //     else
        //     {
        //         Serial.printf("Drawing image: %s\n", filename.c_str());
        //         gfx->fillScreen(WHITE);
        //         this->drawJpeg(file, 0, 0);
        //         close(file);
        //         gfx->flush();
        //     }
        //     delay(1000);
        // }
        if (not SPIFFS.exists("/sshiki.jpg"))
        {
            Serial.println("Not found: /sshiki.jpg");
        }
        else
        {
            gfx->fillScreen(BLACK);
            this->drawJpeg("/sshiki.jpg", 20, 20);
            gfx->flush();
        }
        delay(5000);
    }
};