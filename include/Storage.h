#ifndef __STORAGE_H__
#define __STORAGE_H__

#include <Arduino.h>
#include <Preferences.h>
#include <ArduinoJson.h>

#include "Context.h"

class Storage
{
private:
    Context *_ctx;
    Preferences _preferences;

    String _name = "setting";

public:
    Storage(Context *ctx);
    // bool init();
    void reset();
    bool read();
    bool save();
};

extern Storage storage;
#endif // __STORAGE_H__