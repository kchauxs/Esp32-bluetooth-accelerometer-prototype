#include <Arduino.h>
#include <SPIFFS.h>
#include "Config.h"
#include "Context.h"

#include "Sensors.h"
#include "Storage.h"
#include "Utils.h"
#include "BluetoothService.h"
#include "Callbacks.hpp"

BluetoothSerial SerialBT;

Context ctx;
Storage storage(&ctx);
Sensors sensors(&ctx);
Utils utils(&ctx);
BluetoothService bluetoothService(&ctx, &SerialBT);

void setup(void)
{
  delay(1000);

#if SERIAL_DEBUG
  Serial.begin(115200);
  while (!Serial)
    delay(10);
#endif

  // LED AUX
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, LOW);
  delay(50);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(50);
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
}

void loop()
{
  static unsigned long lastRead = 0;
  // Send data via bluetooth
  if (millis() - lastRead > ctx.sendInterval)
  {
    sensors.readMPU();
    bluetoothService.send(utils.buildPayload());
    lastRead = millis();
  }

  // Receive data via bluetooth
  bluetoothService.receive(receiveBluetootCallback);
}
