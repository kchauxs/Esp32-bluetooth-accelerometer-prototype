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
    doc["interval"] = _ctx->sendInterval;
    doc["brightness"] = _ctx->brightness;

    String payload;
    serializeJson(doc, payload);
    return payload;
}

void Utils::interruptExecution()
{
    while (true)
        ;
}

void Utils::restartDevice(unsigned int waitTime)
{
    delay(waitTime);
    ESP.restart();
}
