#include "RgbLeds.h"

RgbLeds::RgbLeds(Context *ctx)
{
    _ctx = ctx;
}

void RgbLeds::setColor(CRGB color)
{
    FastLED.clear();
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = color;
    }
    FastLED.show();
}

void RgbLeds::initLed()
{
    FastLED.addLeds<LED_TYPE, LED_DATA_PIN>(leds, NUM_LEDS);
    FastLED.setBrightness(LED_BRIDHTNESS);
    FastLED.clear();
}

void RgbLeds::loop(bool hasClient)
{
    static unsigned long lastTime = 0;
    static bool isOn = false;

    if (hasClient)
    {
        if (millis() - lastTime >= _ctx->sendInterval)
        {
            lastTime = millis();
            isOn = !isOn;
            setColor(isOn ? CRGB::Green : CRGB::Black);
        }
    }
    else
    {
        setColor(CRGB::Blue);
    }
}
