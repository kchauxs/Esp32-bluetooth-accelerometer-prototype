#include "Utils.h"

Utils::Utils(Context *ctx)
{
    _ctx = ctx;
}

String Utils::buildPayload()
{
    StaticJsonDocument<1024> doc;
    doc["acceleration"]["x"] = _ctx->acceleration.x;
    doc["acceleration"]["y"] = _ctx->acceleration.y;
    doc["acceleration"]["z"] = _ctx->acceleration.z;
    doc["gyro"]["x"] = _ctx->gyro.x;
    doc["gyro"]["y"] = _ctx->gyro.y;
    doc["gyro"]["z"] = _ctx->gyro.z;
    doc["mpuTemp"] = _ctx->mpuTemp;
    doc["zoom"] = _ctx->zoom;
    doc["wifi"]["ssid"] = _ctx->wifi.ssid;
    doc["mqtt"]["server"] = _ctx->mqtt.server;
    doc["mqtt"]["port"] = _ctx->mqtt.port;
    doc["mqtt"]["publishTopic"] = _ctx->mqtt.publishTopic;
    doc["sendInterval"] = _ctx->sendInterval;

    String payload;
    serializeJson(doc, payload);
    doc.clear();
    return payload;
}

bool Utils::isConnectedToWifi()
{
    return WiFi.status() == WL_CONNECTED;
}

bool Utils::connecToWifi()
{
    String SSID = _ctx->wifi.ssid;
    String PASS = _ctx->wifi.pass;

    if (SSID == "" || PASS == "")
        return false;

    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID.c_str(), PASS.c_str());

    byte attempt = 0;
    bool tick = false;

    while (WiFi.status() != WL_CONNECTED && attempt < 80)
    {
        attempt++;
        tick = !tick;
        digitalWrite(LED_BUILTIN, tick);
        delay(250);
    }
    digitalWrite(LED_BUILTIN, LOW);
    return isConnectedToWifi();
}

void Utils::interruptExecution()
{
    while (true)
        ;
}
