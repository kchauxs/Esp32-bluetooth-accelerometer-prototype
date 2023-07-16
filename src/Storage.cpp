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
    _ctx->isBluetoothMode = DEFAULT_BLUETOOTH_MODE;
    _ctx->wifi.ssid = "";
    _ctx->wifi.pass = "";
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
        _ctx->isBluetoothMode = jsonConfig["isBluetoothMode"].as<bool>();
        _ctx->wifi.ssid = jsonConfig["wifi"]["ssid"].as<String>();
        _ctx->wifi.pass = jsonConfig["wifi"]["pass"].as<String>();

#if SERIAL_DEBUG
        serializeJsonPretty(jsonConfig, Serial);
        Serial.println();
#endif

        file.close();
        return true;
    }
}

bool Storage::save()
{
    StaticJsonDocument<1024> jsonConfig;
    File file = SPIFFS.open(F("/settingdevice.json"), "w+");
    if (file)
    {
        jsonConfig["bluetoothName"] = _ctx->bluetoothName;
        jsonConfig["sendInterval"] = _ctx->sendInterval;
        jsonConfig["isBluetoothMode"] = _ctx->isBluetoothMode;
        jsonConfig["wifi"]["ssid"] = _ctx->wifi.ssid;
        jsonConfig["wifi"]["pass"] = _ctx->wifi.pass;
        serializeJsonPretty(jsonConfig, file);

#if SERIAL_DEBUG
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
