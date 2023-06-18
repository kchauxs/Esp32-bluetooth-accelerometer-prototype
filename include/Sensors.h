#ifndef __SENSORS_H__
#define __SENSORS_H__

#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <ArduinoJson.h>
#include <Wire.h>

#include "Config.h"
#include "Context.h"

class Sensors
{
private:
    Adafruit_MPU6050 _mpu;
    Context *_ctx;

public:
    Sensors(Context *ctx);
    void initMPU();
    void readMPU();
    String getPayload();
};

extern Sensors sensors;
#endif // __SENSORS_H__