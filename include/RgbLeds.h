#ifndef __RGBLEDS_H__
#define __RGBLEDS_H__

#include <Arduino.h>
#include <FastLED.h>

#include "Config.h"
#include "Context.h"

class RgbLeds
{
private:
    CRGB leds[NUM_LEDS];

public:
    RgbLeds(){};
    void setColor(CRGB color);
    void initLed();
};

extern RgbLeds rgbLeds;
#endif // __RGBLEDS_H__