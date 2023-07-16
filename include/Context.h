#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include <Arduino.h>
#include "Config.h"

class Context
{
private:
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

    unsigned int zoom = DEFAULT_ZOOM;
    unsigned long sendInterval = DEFAULT_SEND_INTERVAL;
    String bluetoothName = BLUETOOTH_NAME;

    Context(){};
};

extern Context ctx;
#endif // __CONTEXT_H__