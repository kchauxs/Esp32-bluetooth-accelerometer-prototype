#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <Arduino.h>

// DEVICE CONFIG
#define SERIAL_DEBUG true
#define MIN_READ_INTERVAL 500   // 100 Milliseconds
#define MAX_READ_INTERVAL 60000 // 60 Seconds
#define PIN_BUTTON D4
// - PORTAL
#define DEFAULT_PORTAL_TIMEOUT 90
#define DEFAULT_PORTAL_PASSWORD "12345678"
#define DEFAULT_PORTAL_NAME "Device"

// - MPU
#define DEFAULT_READ_INTERVAL 3000

// BLUETOOTH CONFIG
#define BLUETOOTH_NAME "Smart-Device-Test"

#endif // __CONFIG_H__