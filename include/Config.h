#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <Arduino.h>

// DEVICE CONFIG
#define SERIAL_DEBUG true
#define BUTTON_PIN D8

// RGB LEDS
#define NUM_LEDS 1
#define LED_DATA_PIN 23
#define LED_TYPE NEOPIXEL
#define LED_BRIDHTNESS 128

// DATA CONFIG
#define DEFAULT_SEND_INTERVAL 3000
#define MIN_SEND_INTERVAL 550   // 100 Milliseconds
#define MAX_SEND_INTERVAL 60000 // 60 Seconds

#define DEFAULT_ZOOM 1
#define MIN_ZOOM 1
#define MAX_ZOOM 10

// BLUETOOTH CONFIG
#define DEFAULT_BLUETOOTH_MODE true
#define BLUETOOTH_NAME "Acelorometer-device"

// MQTT CONFIG
#define DEFAULT_MQTT_SERVER "broker.emqx.io"
#define DEFAULT_MQTT_PORT 1883
#define DEFAULT_MQTT_PUBLISH_TOPIC "accelerometer-output/ae49f0de-73a9-4db5-a3d5-e48bd3225791"
#define RECONNECT_ATTEMPT 5000
#define DEFAULT_MQTT_BUFFER_SIZE 2048
#endif // __CONFIG_H__