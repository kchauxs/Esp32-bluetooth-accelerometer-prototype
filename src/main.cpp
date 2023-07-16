#include <Arduino.h>
#include <SPIFFS.h>
#include "OneButton.h"

#include "Config.h"
#include "Context.h"

#include "Sensors.h"
#include "Storage.h"
#include "Utils.h"
#include "BluetoothService.h"
#include "RgbLeds.h"
#include "Callbacks.hpp"

BluetoothSerial SerialBT;
OneButton *button;

Context ctx;
RgbLeds rgbLeds;
Storage storage(&ctx);
Sensors sensors(&ctx);
Utils utils(&ctx);
BluetoothService bluetoothService(&ctx, &SerialBT);

void setup(void)
{
  delay(1000);

  // RGB LEDS
  rgbLeds.initLed();
  rgbLeds.setColor(CRGB::Black);
  delay(50);
  rgbLeds.setColor(CRGB::Purple);

#if SERIAL_DEBUG
  Serial.begin(115200);
  while (!Serial)
    delay(10);
#endif

  // BUTTONS
  button = new OneButton(BUTTON_PIN, false);
  button->attachClick(zoomInCallback);
  button->attachDoubleClick(zoomOutCallback);

  // LED AUX
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // STORAGE
  storage.init();

  if (!storage.read())
    storage.save();

  // SENSORS
  sensors.initMPU();
  delay(500);

  // BLUETOOTH
  bluetoothService.init(ctx.bluetoothName);
  delay(500);
  rgbLeds.setColor(CRGB::Blue);
}

void loop()
{
  button->tick();
  bluetoothService.sendLoop(sendBluetoothCallback);
  bluetoothService.receive(receiveBluetootCallback);
}
