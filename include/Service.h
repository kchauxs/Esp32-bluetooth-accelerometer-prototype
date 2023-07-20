#ifndef __SERVICE_H__
#define __SERVICE_H__

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

#include "Config.h"
#include "Context.h"

class Service
{
private:
    Context *_ctx;
    PubSubClient *_client;

public:
    Service(Context *ctx);
    void setupMqttServer(PubSubClient *client, uint16_t bufferSize);
    void mqttLoop();
    void mqttLoop(String (*callback)());
    void publishData(String topic, String payload);
    bool reconnectToMqttServer();
    bool isMqttConnected();
};

void subscribeCallback(char *topic, byte *payload, unsigned int length);
#endif // __SERVICE_H__