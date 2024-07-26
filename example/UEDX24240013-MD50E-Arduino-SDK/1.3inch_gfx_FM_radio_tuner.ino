#include <Arduino_GFX_Library.h>

#define GFX_BL 8

Arduino_DataBus *bus = new Arduino_ESP32SPI(4 /* DC */, 10 /* CS */, 1 /* SCK */, 0 /* MOSI */, GFX_NOT_DEFINED /* MISO */);
Arduino_GFX *gfx = new Arduino_GC9A01(bus, GFX_NOT_DEFINED /* RST */, 0 /* rotation */, true /* IPS */);


#include "AiEsp32RotaryEncoder.h"

#if defined(ESP8266)
#define ROTARY_ENCODER_A_PIN D6
#define ROTARY_ENCODER_B_PIN D5
#define ROTARY_ENCODER_BUTTON_PIN D7
#else
#define ROTARY_ENCODER_A_PIN 7
#define ROTARY_ENCODER_B_PIN 6
#define ROTARY_ENCODER_BUTTON_PIN 9
#endif

#define ROTARY_ENCODER_STEPS 4
AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, -1, ROTARY_ENCODER_STEPS);

/*
FM radio tunner is suposed to set frequency between 88.0 MHz and 104.0 MHz by 0.1MHz steps
Rotary encoder works with integers so we will map 88.0 to 166 and then divide by 10 to get 0.1 steps
frequency = rotaryValue / 2;
*/

float getFrequency()
{
    return (float)rotaryEncoder.readEncoder() / 10.0;
}

void rotary_onButtonClick()
{
    static unsigned long lastTimePressed = 0;
    if (millis() - lastTimePressed < 200)
        return;
    lastTimePressed = millis();

    Serial.print("Radio station set to ");
    Serial.print(getFrequency());
    Serial.println(" MHz ");
}

void IRAM_ATTR readEncoderISR()
{
    rotaryEncoder.readEncoder_ISR();
}

void setup()
{
    Serial.begin(115200);

    #ifdef GFX_BL //打开背光
    pinMode(GFX_BL, OUTPUT);
    digitalWrite(GFX_BL, LOW);
    #endif
  //初始化gfx
    gfx->begin(80000000);

    rotaryEncoder.begin();
    rotaryEncoder.setup(readEncoderISR);
    rotaryEncoder.setBoundaries(88 * 10, 104 * 10, true); //minValue, maxValue, circleValues true|false (when max go to min and vice versa)
    rotaryEncoder.setAcceleration(50);
    rotaryEncoder.setEncoderValue(92.1 * 10); //set default to 92.1 MHz
    Serial.println("FM Radio");
    Serial.print("Radio station initially set to ");
    Serial.print(getFrequency());
    Serial.println(" MHz. Tune to some other station like 103.2... and press button ");
}

void loop()
{
  
    gfx->fillScreen(CYAN);
    gfx->setCursor(50, 90);
    gfx->setTextSize(5);
    gfx->setTextColor(RED);
    gfx->println(getFrequency());
    gfx->setCursor(90, 140);
    gfx->println("MHz");
    if (rotaryEncoder.encoderChanged())
    {
        Serial.print(getFrequency(), 1);
        Serial.println(" MHz ");
      
        
    }
    if (rotaryEncoder.isEncoderButtonClicked())
    {
        rotary_onButtonClick();
        gfx->fillScreen(BLACK);
        gfx->setCursor(80, 40);
        gfx->setTextSize(5);
        gfx->setTextColor(YELLOW);
        gfx->println("SET");
        gfx->setCursor(50, 90);
        gfx->setTextSize(5);
        gfx->setTextColor(RED);
        gfx->println(getFrequency());
        gfx->setCursor(90, 140);
        gfx->println("MHz");
        delay(10000);
    }

    gfx->flush();
    delay(500);
}