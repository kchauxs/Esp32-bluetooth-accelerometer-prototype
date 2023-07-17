#ifndef __UTILS_H__
#define __UTILS_H__

#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>

#include "Config.h"
#include "Context.h"

class Utils
{
private:
    Context *_ctx;

public:
    Utils(Context *ctx);
    String buildPayload();
    bool connecToWifi();
    void interruptExecution();
    bool isConnectedToWifi();
};

extern Utils utils;
#endif // __UTILS_H__