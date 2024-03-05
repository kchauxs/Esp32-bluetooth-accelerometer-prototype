#include "Storage.h"

Storage::Storage(Context *ctx)
{
    _ctx = ctx;
}

bool Storage::init()
{
    if (!SPIFFS.begin(true))
        _isInit = false;
    else
        _isInit = true;
    return _isInit;
}

void Storage::reset()
{
    _ctx->bluetoothName = BLUETOOTH_NAME;
    _ctx->sendInterval = DEFAULT_SEND_INTERVAL;
    _ctx->brightness = DEFAULT_BRIGHTNESS;
}

bool Storage::read()
{
    if (!_isInit)
        return false;

    StaticJsonDocument<1024> jsonConfig;
    File file = SPIFFS.open("/settingdevice.json", "r");

    if (deserializeJson(jsonConfig, file))
    {
        this->reset();
        return false;
    }
    else
    {
        _ctx->bluetoothName = jsonConfig["bluetoothName"].as<String>();
        _ctx->sendInterval = jsonConfig["sendInterval"].as<long>();
        _ctx->brightness = jsonConfig["brightness"].as<uint8_t>();

#if SERIAL_DEBUG
        Serial.println("\n[INFO]\t Read config: ");
        serializeJsonPretty(jsonConfig, Serial);
        Serial.println();
#endif

        file.close();
        return true;
    }
}

bool Storage::save()
{
    if (!_isInit)
        return false;

    StaticJsonDocument<1024> jsonConfig;
    File file = SPIFFS.open(F("/settingdevice.json"), "w+");
    if (file)
    {
        jsonConfig["bluetoothName"] = _ctx->bluetoothName;
        jsonConfig["sendInterval"] = _ctx->sendInterval;
        jsonConfig["brightness"] = _ctx->brightness;
        serializeJsonPretty(jsonConfig, file);

#if SERIAL_DEBUG
        Serial.println("\n[INFO]\t Save config: ");
        serializeJsonPretty(jsonConfig, Serial);
        Serial.println();
#endif
        file.close();
        return true;
    }
    else
    {
        return false;
    }
}
