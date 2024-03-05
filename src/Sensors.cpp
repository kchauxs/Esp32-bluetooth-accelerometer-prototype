#include "Sensors.h"

Sensors::Sensors(Context *ctx)
{
    this->_ctx = ctx;
}

bool Sensors::initMPU()
{
    if (!_mpu.begin())
    {
#if SERIAL_DEBUG
        Serial.println("[ERROR]\t Failed to find MPU6050 chip ✖️");
#endif
        return false;
    }

#if SERIAL_DEBUG
    Serial.println("[INFO]\t MPU6050 Found! ✅");
#endif

    _mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    _mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    _mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    return true;
}

void Sensors::readGyro()
{
    sensors_event_t a, g, temp;
    _mpu.getEvent(&a, &g, &temp);

    float gyroX_temp = g.gyro.x;
    if (abs(gyroX_temp) > gyroXerror)
        _ctx->gyro.x += gyroX_temp / 50.00;

    float gyroY_temp = g.gyro.y;
    if (abs(gyroY_temp) > gyroYerror)
        _ctx->gyro.y += gyroY_temp / 70.00;

    float gyroZ_temp = g.gyro.z;
    if (abs(gyroZ_temp) > gyroZerror)
        _ctx->gyro.z += gyroZ_temp / 90.00;
}

void Sensors::readAccel()
{
    sensors_event_t a, g, temp;
    _mpu.getEvent(&a, &g, &temp);

    _ctx->acceleration.x = a.acceleration.x;
    _ctx->acceleration.y = a.acceleration.y;
    _ctx->acceleration.z = a.acceleration.z;
}

void Sensors::readTemp()
{
    sensors_event_t a, g, temp;
    _mpu.getEvent(&a, &g, &temp);

    _ctx->mpuTemp = temp.temperature;
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

    _ctx->mpuTemp = temp.temperature;
}

void Sensors::readMPULoop()
{
    static unsigned long lastRead = 0;

    if ((millis() - lastRead) > 200)
    {
        this->readMPU();
        lastRead = millis();
    }
}

void Sensors::loop()
{
    static unsigned long lastGyroRead = 0;
    static unsigned long lastAccelRead = 0;
    static unsigned long lastTempRead = 0;

    if ((millis() - lastGyroRead) > 10)
    {
        lastGyroRead = millis();
        readGyro();
    }

    if ((millis() - lastAccelRead) > 200)
    {
        lastAccelRead = millis();
        readAccel();
    }

    if ((millis() - lastTempRead) > 5000)
    {
        lastTempRead = millis();
        readTemp();
    }
}
