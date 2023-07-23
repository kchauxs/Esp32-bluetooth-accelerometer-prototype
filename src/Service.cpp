#include "Service.h"

String mqttServer;
String mqttPublishTopic;
int mqttPort;

Service::Service(Context *ctx)
{
    _ctx = ctx;
}

void Service::setupMqttServer(PubSubClient *client, uint16_t bufferSize)
{
    _client = client;
    mqttServer = _ctx->mqtt.server;
    mqttPublishTopic = _ctx->mqtt.publishTopic;
    mqttPort = _ctx->mqtt.port;

#if SERIAL_DEBUG
    Serial.println("\n[INFO] MQTT Server: " + mqttServer);
    Serial.println("[INFO] MQTT Port: " + String(mqttPort));
    Serial.println("[INFO] Publish topic: " + mqttPublishTopic);
#endif

    _client->setServer(mqttServer.c_str(), mqttPort);
    _client->setBufferSize(bufferSize);
}

bool Service::reconnectToMqttServer()
{
#if SERIAL_DEBUG
    Serial.println("\n[INFO] Attempting MQTT connection...");
#endif

    String clientId = "ESPClient-";
    clientId += String(random(0xffff), HEX);

    if (!_client->connect(clientId.c_str()))
    {
#if SERIAL_DEBUG
        Serial.print("\n[ERROR] failed, rc=");
        Serial.print(_client->state());
        Serial.println(" try again in 3 seconds");
#endif
    }
    return _client->connected();
}

bool Service::isMqttConnected()
{
    return _client->connected();
}

void Service::mqttLoop()
{
    static unsigned int lastReconnectAttempt = 0;

    if (WiFi.status() != WL_CONNECTED)
        return;

    if (_client->connected())
    {
        _client->loop();
    }
    else
    {
        if (millis() - lastReconnectAttempt > RECONNECT_ATTEMPT)
        {
            lastReconnectAttempt = millis();
            if (this->reconnectToMqttServer())
                lastReconnectAttempt = 0;
        }
    }
}

void Service::mqttLoop(String (*callback)())
{
    static unsigned long lastMsg = 0;
    static unsigned long lastReconnectAttempt = 0;

    if (WiFi.status() != WL_CONNECTED)
        return;

    if (!_client->connected())
    {
        long now = millis();
        if (now - lastReconnectAttempt > RECONNECT_ATTEMPT)
        {
            lastReconnectAttempt = now;
            if (this->reconnectToMqttServer())
                lastReconnectAttempt = 0;
        }
    }
    else
    {
        _client->loop();

        if (millis() - lastMsg > ctx.sendInterval)
        {
            lastMsg = millis();
            String payload = callback();
            this->publishData(mqttPublishTopic, payload);
        }
    }
}

void Service::publishData(String topic, String payload)
{
#if SERIAL_DEBUG
    Serial.print("\n[INFO] Publish message: ");
    Serial.println(payload);
#endif
    _client->publish(topic.c_str(), payload.c_str());
}
