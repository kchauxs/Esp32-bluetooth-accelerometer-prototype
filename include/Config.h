#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <Arduino.h>

// DEVICE CONFIG
#define SERIAL_DEBUG true
#define BUTTON_PIN D7

// RGB LEDS
#define NUM_LEDS 1
#define LED_DATA_PIN D8
#define LED_TYPE NEOPIXEL
#define DEFAULT_BRIGHTNESS 116

// DATA CONFIG
#define DEFAULT_SEND_INTERVAL 1000
#define MIN_SEND_INTERVAL 300   // 300 Milliseconds
#define MAX_SEND_INTERVAL 60000 // 60 Seconds

#define DEFAULT_ZOOM 1
#define MIN_ZOOM 1
#define MAX_ZOOM 10

// BLUETOOTH CONFIG
#define DEFAULT_BLUETOOTH_MODE true
#define BLUETOOTH_NAME "Acelorometer-device"

#endif // __CONFIG_H__