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

    unsigned long readInterval = DEFAULT_READ_INTERVAL;

    bool wifiIsSaved = false;
    String WiFiSSID;
    String WiFiPass;

    Context(){};
};

extern Context ctx;
#endif // __CONTEXT_H__