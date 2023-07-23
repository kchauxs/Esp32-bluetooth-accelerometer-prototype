#include "RgbLeds.h"

void RgbLeds::setColor(CRGB color)
{
    FastLED.clear();
    for (int i = 0; i < NUM_LEDS; i++)
        leds[i] = color;

    FastLED.show();
}

void RgbLeds::initLed()
{
    FastLED.addLeds<LED_TYPE, LED_DATA_PIN>(leds, NUM_LEDS);
    FastLED.setBrightness(LED_BRIDHTNESS);
    FastLED.clear();
}
