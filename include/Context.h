#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include <Arduino.h>
#include "Config.h"

class Context
{
private:
public:
    struct
    {
        float x;
        float y;
        float z;
    } acceleration;

    struct
    {
        float x;
        float y;
        float z;
    } gyro;

    struct
    {
        String ssid;
        String pass;
    } wifi;

    struct
    {
        String server = DEFAULT_MQTT_SERVER;
        int port = DEFAULT_MQTT_PORT;
        String user;
        String pass;
        String publishTopic = DEFAULT_MQTT_PUBLISH_TOPIC;

    } mqtt;

    bool isBluetoothMode = DEFAULT_BLUETOOTH_MODE;
    String bluetoothName = BLUETOOTH_NAME;

    unsigned int zoom = DEFAULT_ZOOM;
    unsigned long sendInterval = DEFAULT_SEND_INTERVAL;

    Context(){};
};

extern Context ctx;
#endif // __CONTEXT_H__