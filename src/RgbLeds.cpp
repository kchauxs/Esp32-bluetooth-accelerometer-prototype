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
    FastLED.setBrightness(_ctx->brightness);
    FastLED.clear();

    setColor(CRGB::Black);
}

void RgbLeds::setBrightness(uint8_t brightness)
{
    if (brightness == _ctx->brightness)
        return;

    if (brightness > 0 && brightness <= 255)
    {
        _ctx->brightness = brightness;
        FastLED.setBrightness(_ctx->brightness);
    }
}
