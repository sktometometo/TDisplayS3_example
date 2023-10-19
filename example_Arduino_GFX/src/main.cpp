#include "Arduino.h"
#include "SPIFFS.h"

#include "Arduino_GFX_Library.h"

#include "pin_config.h"
#include "display.h"

TDisplay *display;

void setup()
{
    Serial.begin(115200);
    Serial.println("Hello T-Display-S3");

    if (!SPIFFS.begin(true))
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        while (true)
        {
            delay(100);
        }
    }

    // if (!file)
    // {
    //     Serial.println("Failed to open config file");
    //     while (true)
    //     {
    //         delay(100);
    //     }
    // }

    // // load jpeg image with JPEGDEC
    // JPEGDEC jpeg;
    // jpeg.open(&file, );

    display = new TDisplay();
    Serial.println("Setup done.");
}

void loop(void)
{
    display->test();
}
