#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include <Arduino.h>
#include "Config.h"

class Context
{
private:
    Context() {}
    static Context *instance;

public:
    struct
    {
        float x;
        float y;
        float z;
    } acceleration;

    struct
    {
        float x;
        float y;
        float z;
    } gyro;

    float mpuTemp = 0;

    String bluetoothName = BLUETOOTH_NAME;

    unsigned int zoom = DEFAULT_ZOOM;
    unsigned long sendInterval = DEFAULT_SEND_INTERVAL;
    uint8_t brightness = DEFAULT_BRIGHTNESS;

    static Context *getInstance()
    {
        if (!instance)
        {
            instance = new Context();
        }

        return instance;
    }
};

extern Context *ctx;
#endif // __CONTEXT_H__