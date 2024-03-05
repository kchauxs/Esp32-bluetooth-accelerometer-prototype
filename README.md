# Bluetooth Communication with ESP32

This project focuses on bidirectional communication between an ESP32 and a mobile device via Bluetooth Serial (SPP). The ESP32 acts as a Bluetooth server, while the mobile device acts as the client.

The ESP32 collects data from an MPU6050 accelerometer, gyroscope, and temperature sensor, and sends it to the mobile device in the form of a JSON payload. Additionally, the mobile device can send commands to the ESP32 to control different aspects of the device, such as the Bluetooth name, RGB LED brightness, and data sending interval.

## Features

- Collects data from MPU6050 accelerometer, gyroscope, and temperature sensor.
- Sends data to the mobile device as a JSON payload.
- Allows mobile device to send commands to control various aspects of the ESP32 device.


## Dependencies

The project is based on several libraries, which can be installed using PlatformIO's dependency management system. See the **platformio.ini** file:

```ini
lib_deps = 
	adafruit/Adafruit MPU6050@^2.2.4
	bblanchon/ArduinoJson@^6.19.4
	mathertel/OneButton@^2.0.3
	fastled/FastLED@^3.5.0
```

These libraries provide functionality for working with the MPU6050 sensor, handling JSON data, managing button inputs, and controlling RGB LEDs.

## Bluetooth Sending Payload

The device sends the following payload to the client, containing acceleration, gyroscope, MPU temperature, zoom level, send interval, and RGB LED brightness:


```json
{
  "acceleration": {
    "x": 0.715866327,
    "y": -0.567425787,
    "z": 9.40681839
  },
  "gyro": {
    "x": -1.817486142e+31,
    "y": -35025907710,
    "z": 8.958958713e+24
  },
  "mpuTemp": 33.17411804,
  "zoom": 1,
  "interval": 1000,
  "brightness": 255
}

```


## Command payload

The client can send commands to the device, such as changing the Bluetooth name, adjusting LED brightness, setting data sending interval, rebooting the device, resetting settings, or adjusting logical zoom. The payload format for commands is as follows:

```json
{
  "command": "BLUETOOTH_NAME",
  "value": "Acelorometer-device"
}
```

This payload instructs the device to change its Bluetooth name to "Device-Accelerometer". The "command" field identifies the type of command and "value" contains the new value to be applied.


### Available Commands 
 
| Command            | Type          | Range       | Description                                 |
| ------------------ | ------------- | ----------- | ------------------------------------------- |
| BLUETOOTH_NAME_KEY | String        | -           | Change Bluetooth device name                |
| BRIGHTNESS_KEY     | uint8_t       | 0 - 255     | Adjust RGB LED Brightness                   |
| INTERVAL_KEY       | unsigned long | 300 - 60000 | Adjust the sending interval in milliseconds |
| REBOOT_KEY         | bool          | -           | Reboot the device                           |
| RESET_KEY          | bool          | -           | Reset settings to default values            |
| ZOOM_KEY           | uint8_t       | 1 - 10      | Adjust logical zoom                         |

