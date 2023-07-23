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

Context ctx;
RgbLeds rgbLeds(&ctx);
Sensors sensors(&ctx);
Storage storage(&ctx);
Utils utils(&ctx);
BluetoothService bluetoothService(&ctx, &SerialBT);

void setup(void)
{
  delay(1000);
  Wire.begin();

  // RGB LEDS
  rgbLeds.initLed();
  rgbLeds.setColor(CRGB::Black);
  rgbLeds.setColor(CRGB::Blue);

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
    digitalWrite(LED_BUILTIN, HIGH);
    utils.interruptExecution();
  }

  // STORAGE
  storage.init();
  if (!storage.read())
    storage.save();

  // BUTTONS
  button.attachClick(zoomInCallback);
  button.attachDoubleClick(zoomOutCallback);

  // BLUETOOTH
  bluetoothService.init(ctx.bluetoothName);
}

void loop()
{
  bool hasClient = bluetoothService.hasClient();
  rgbLeds.setColor(hasClient ? CRGB::Green : CRGB::Blue);
  
  if (!hasClient)
    return;

  button.tick();
  sensors.loop();
  bluetoothService.sendLoop(payloadCallback);
  bluetoothService.receive(receiveBluetootCallback);
}
