#include "Sensors.h"

Sensors::Sensors(Context *ctx)
{
    this->_ctx = ctx;
}

bool Sensors::initMPU()
{
    if (!_mpu.begin())
    {
        Serial.println("Error: Failed to find MPU6050 chip ✖️");
        return false;
    }

    Serial.println("[INFO] MPU6050 Found! ✅");
    _mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    _mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    _mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    return true;
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

void Sensors::loop()
{
    static unsigned long lastSensorRead = 0;

    if (millis() - lastSensorRead > 50000)
    {
        lastSensorRead = millis();
        this->readMPU();
    }
}
