#include "Utils.h"

Utils::Utils(Context *ctx)
{
    _ctx = ctx;
}

String Utils::buildPayload()
{
    StaticJsonDocument<512> doc;
    doc["acceleration"]["x"] = _ctx->acceleration.x;
    doc["acceleration"]["y"] = _ctx->acceleration.y;
    doc["acceleration"]["z"] = _ctx->acceleration.z;
    doc["gyro"]["x"] = _ctx->gyro.x;
    doc["gyro"]["y"] = _ctx->gyro.y;
    doc["gyro"]["z"] = _ctx->gyro.z;

    String payload;
    serializeJson(doc, payload);
    doc.clear();
    return payload;
}