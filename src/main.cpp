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
#include "Storage.h"
#include "Utils.h"

#include "Callbacks.hpp"

BluetoothSerial SerialBT;
OneButton button(BUTTON_PIN, false);
WiFiClient wifiClient;

Context *Context::instance = nullptr;
Context *ctx = Context::getInstance();

RgbLeds rgbLeds(ctx);
Sensors sensors(ctx);
Storage storage(ctx);
Utils utils(ctx);
BluetoothService bluetoothService(ctx, &SerialBT);

void setup(void)
{
  delay(1000);
  Wire.begin();

#if SERIAL_DEBUG
  Serial.begin(115200);
  while (!Serial)
    delay(10);
#endif

  // RGB LEDS
  rgbLeds.initLed();
  delay(50);

  // LED AUX
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // SENSORS
  if (!sensors.initMPU())
  {
    digitalWrite(LED_BUILTIN, HIGH);
    utils.interruptExecution();
  }
  delay(100);

  // RGB LEDS
  rgbLeds.setColor(CRGB::Blue);

  // STORAGE
  storage.init();
  if (!storage.read())
    storage.save();

  // BUTTONS
  button.attachClick(zoomIn);
  button.attachDoubleClick(zoomOut);

  // BLUETOOTH
  bluetoothService.init();
}

void loop()
{
  bool hasClient = bluetoothService.hasClient();
  rgbLeds.setColor(hasClient ? CRGB::Cyan : CRGB::Blue);

  if (!hasClient)
    return;

  button.tick();
  sensors.loop();
  bluetoothService.sendLoop(payloadCallback);
  bluetoothService.receive(receiveBluetootCallback);
}
