#include <Arduino.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <Wire.h>
#include "OneButton.h"

#include "Config.h"
#include "Context.h"

#include "BluetoothService.h"
#include "RgbLeds.h"
#include "Sensors.h"
#include "Service.h"
#include "Storage.h"
#include "Utils.h"

#include "Callbacks.hpp"

BluetoothSerial SerialBT;
OneButton *button;
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

Context ctx;
RgbLeds rgbLeds;
Sensors sensors(&ctx);
Storage storage(&ctx);
Utils utils(&ctx);

Service *service;
BluetoothService *bluetoothService;

void setup(void)
{
  delay(1000);
  Wire.begin();

  // RGB LEDS
  rgbLeds.initLed();
  rgbLeds.setColor(CRGB::Black);
  rgbLeds.setColor(CRGB::Purple);

#if SERIAL_DEBUG
  Serial.begin(115200);
  while (!Serial)
    delay(10);
#endif

  // LED AUX
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // SENSORS
  if (!sensors.initMPU())
  {
    rgbLeds.setColor(CRGB::Black);
    rgbLeds.setColor(CRGB::Red);
    utils.interruptExecution();
  }

  // STORAGE
  if (!storage.read())
    storage.save();

  // MODE
  if (digitalRead(BUTTON_PIN) == HIGH)
  {
    ctx.isBluetoothMode = !ctx.isBluetoothMode;
    storage.save();
  }

  // BUTTONS
  button = new OneButton(BUTTON_PIN, false);
  button->attachClick(zoomInCallback);
  button->attachDoubleClick(zoomOutCallback);

  // BLUETOOTH
  if (ctx.isBluetoothMode)
  {
    rgbLeds.setColor(CRGB::Black);
    rgbLeds.setColor(CRGB::Blue);

    bluetoothService = new BluetoothService(&ctx, &SerialBT);
    bluetoothService->init(ctx.bluetoothName);
    delay(1000);
  }
  else
  {
    rgbLeds.setColor(CRGB::Black);
    rgbLeds.setColor(CRGB::Green);

    // WIFI
    utils.connecToWifi();
    delay(1000);

    // MQTT SERVICE
    service = new Service(&ctx);
    service->setupMqttServer(&mqttClient, DEFAULT_MQTT_BUFFER_SIZE);
    delay(1000);
  }
}

void loop()
{
  button->tick();
  sensors.loop();

  if (!ctx.isBluetoothMode)
    service->mqttLoop(payloadCallback);
  else
  {
    if (!bluetoothService->hasClient())
      return;

    bluetoothService->sendLoop(payloadCallback);
    bluetoothService->receive(receiveBluetootCallback);
  }
}
