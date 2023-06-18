#include <Arduino.h>
#include "BluetoothSerial.h"

#include "Config.h"
#include "Context.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#include "Sensors.h"

BluetoothSerial SerialBT;

Context ctx;
Sensors sensors(&ctx);

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

  // BLUETOOTH
  SerialBT.begin(BLUETOOTH_NAME);

  // SENSORS
  sensors.initMPU();
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
}

void loop()
{
  static unsigned long lastRead = 0;
  static String message = "";

  // Send data via bluetooth
  if (millis() - lastRead > ctx.readInterval)
  {
    sensors.readMPU();
    SerialBT.println(sensors.getPayload());
    lastRead = millis();
  }

  // Blink LED

  if (SerialBT.available())
  {
    char incomingChar = SerialBT.read();

    if (incomingChar != '\n')
      message += String(incomingChar);
    else
      message = "";

    Serial.write(incomingChar);
  }

  if (message == "led_on")
  {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else if (message == "led_off")
  {
    digitalWrite(LED_BUILTIN, LOW);
  }
}
