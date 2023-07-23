#include "Storage.h"

Storage::Storage(Context *ctx)
{
    _ctx = ctx;
}

void Storage::reset()
{
    _ctx->bluetoothName = BLUETOOTH_NAME;
    _ctx->sendInterval = DEFAULT_SEND_INTERVAL;
    _ctx->isBluetoothMode = DEFAULT_BLUETOOTH_MODE;
    _ctx->wifi.ssid = DEFAULT_WIFI_SSID;
    _ctx->wifi.pass = DEFAULT_WIFI_PASS;
    _ctx->mqtt.server = DEFAULT_MQTT_SERVER;
    _ctx->mqtt.port = DEFAULT_MQTT_PORT;
    _ctx->mqtt.publishTopic = DEFAULT_MQTT_PUBLISH_TOPIC;
}

bool Storage::read()
{
    if (!_preferences.begin(_name.c_str(), false))
    {
        this->reset();
        return false;
    }

    StaticJsonDocument<1024> jsonConfig;
    String settingdevice = _preferences.getString("settingdevice", "{}");

    if (deserializeJson(jsonConfig, settingdevice))
    {
        this->reset();
        return false;
    }
    else
    {

        _ctx->bluetoothName = jsonConfig.containsKey("bluetoothName") ? jsonConfig["bluetoothName"].as<String>() : BLUETOOTH_NAME;
        _ctx->sendInterval = jsonConfig.containsKey("sendInterval") ? jsonConfig["sendInterval"].as<long>() : DEFAULT_SEND_INTERVAL;
        _ctx->isBluetoothMode = jsonConfig.containsKey("isBluetoothMode") ? jsonConfig["isBluetoothMode"].as<bool>() : DEFAULT_BLUETOOTH_MODE;
        _ctx->wifi.ssid = jsonConfig.containsKey("wifi") ? jsonConfig["wifi"]["ssid"].as<String>() : DEFAULT_WIFI_SSID;
        _ctx->wifi.pass = jsonConfig.containsKey("wifi") ? jsonConfig["wifi"]["pass"].as<String>() : DEFAULT_WIFI_PASS;
        _ctx->mqtt.server = jsonConfig.containsKey("mqtt") ? jsonConfig["mqtt"]["server"].as<String>() : DEFAULT_MQTT_SERVER;
        _ctx->mqtt.port = jsonConfig.containsKey("mqtt") ? jsonConfig["mqtt"]["port"].as<int>() : DEFAULT_MQTT_PORT;
        _ctx->mqtt.publishTopic = jsonConfig.containsKey("mqtt") ? jsonConfig["mqtt"]["publishTopic"].as<String>() : DEFAULT_MQTT_PUBLISH_TOPIC;

        // _ctx->bluetoothName = jsonConfig["bluetoothName"].as<String>();
        // _ctx->sendInterval = jsonConfig["sendInterval"].as<long>();
        // _ctx->isBluetoothMode = jsonConfig["isBluetoothMode"].as<bool>();
        // _ctx->wifi.ssid = jsonConfig["wifi"]["ssid"].as<String>();
        // _ctx->wifi.pass = jsonConfig["wifi"]["pass"].as<String>();
        // _ctx->mqtt.server = jsonConfig["mqtt"]["server"].as<String>();
        // _ctx->mqtt.port = jsonConfig["mqtt"]["port"].as<int>();
        // _ctx->mqtt.publishTopic = jsonConfig["mqtt"]["publishTopic"].as<String>();

#if SERIAL_DEBUG
        serializeJsonPretty(jsonConfig, Serial);
        Serial.println();
#endif
        _preferences.end();
        delay(10);
        return true;
    }
}

bool Storage::save()
{

    if (!_preferences.begin(_name.c_str(), false))
    {
        this->reset();
        return false;
    }
    StaticJsonDocument<1024> jsonConfig;

    jsonConfig["bluetoothName"] = _ctx->bluetoothName;
    jsonConfig["sendInterval"] = _ctx->sendInterval;
    jsonConfig["isBluetoothMode"] = _ctx->isBluetoothMode;
    jsonConfig["wifi"]["ssid"] = _ctx->wifi.ssid;
    jsonConfig["wifi"]["pass"] = _ctx->wifi.pass;
    jsonConfig["mqtt"]["server"] = _ctx->mqtt.server;
    jsonConfig["mqtt"]["port"] = _ctx->mqtt.port;
    jsonConfig["mqtt"]["publishTopic"] = _ctx->mqtt.publishTopic;

#if SERIAL_DEBUG
    serializeJsonPretty(jsonConfig, Serial);
    Serial.println();
#endif

    String settingdevice;
    serializeJson(jsonConfig, settingdevice);
    _preferences.putString("settingdevice", settingdevice);
    _preferences.end();
    delay(10);
    return true;
}
