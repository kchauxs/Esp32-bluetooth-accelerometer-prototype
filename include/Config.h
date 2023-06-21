#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <Arduino.h>

// DEVICE CONFIG
#define SERIAL_DEBUG false

// DATA CONFIG
#define DEFAULT_SEND_INTERVAL 3000
#define MIN_SEND_INTERVAL 500   // 100 Milliseconds
#define MAX_SEND_INTERVAL 60000 // 60 Seconds

// BLUETOOTH CONFIG
#define BLUETOOTH_NAME "Smart-Device-Test"

#endif // __CONFIG_H__