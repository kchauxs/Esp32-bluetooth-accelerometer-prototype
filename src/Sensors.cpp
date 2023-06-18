#include "Sensors.h"

Sensors::Sensors(Context *ctx)
{
    this->_ctx = ctx;
}

void Sensors::initMPU()
{
    if (!_mpu.begin())
    {
        Serial.println("Error: Failed to find MPU6050 chip");
        while (1)
        {
            delay(10);
        }
    }
    Serial.println("Info: MPU6050 Found! ✅");

    _mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    _mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    _mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void Sensors::readMPU()
{
    sensors_event_t a, g, temp;
    _mpu.getEvent(&a, &g, &temp);

    _ctx->acceleration.x = a.acceleration.x;
    _ctx->acceleration.y = a.acceleration.y;
    _ctx->acceleration.z = a.acceleration.z;

    _ctx->gyro.x = g.gyro.x;
    _ctx->gyro.y = g.gyro.y;
    _ctx->gyro.z = g.gyro.z;
}

String Sensors::getPayload()
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