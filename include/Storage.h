#ifndef __STORAGE_H__
#define __STORAGE_H__

#include <Arduino.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

#include "Context.h"

class Storage
{
private:
    bool _isInit = false;
    Context *_ctx;

public:
    Storage(Context *ctx);
    bool init();
    void reset();
    bool read();
    bool save();
};

extern Storage storage;
#endif // __STORAGE_H__