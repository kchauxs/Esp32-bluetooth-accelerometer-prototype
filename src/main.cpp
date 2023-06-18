#include <Arduino.h>
#include <SPIFFS.h>
#include "Config.h"
#include "Context.h"

#include "Sensors.h"
#include "Storage.h"
#include "BluetoothService.h"
#include "Callbacks.hpp"

BluetoothSerial SerialBT;

Context ctx;
Storage storage(&ctx);
Sensors sensors(&ctx);

BluetoothService bluetoothService(&ctx, &SerialBT);

void setup(void)
{
  delay(1000);
  setCpuFrequencyMhz(240);

#if SERIAL_DEBUG
  Serial.begin(115200);
  while (!Serial)
    delay(10);
#endif

  // LED AUX
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  delay(50);

  // STORAGE
  storage.init();
  
  if (!storage.read())
    storage.save();

  // BLUETOOTH
  bluetoothService.init();

  // SENSORS
  sensors.initMPU();
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
}

void loop()
{
  static unsigned long lastRead = 0;
  // Send data via bluetooth
  if (millis() - lastRead > ctx.sendInterval)
  {
    sensors.readMPU();
    bluetoothService.send(sensors.getPayload());
    lastRead = millis();
  }

  // Receive data via bluetooth
  bluetoothService.receive(receiveBluetootCallback);
}
