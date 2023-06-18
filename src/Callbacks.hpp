#include <Arduino.h>
#include <ArduinoJson.h>

bool checkSendInterval(unsigned long sendInterval)
{
    if (sendInterval <= MIN_SEND_INTERVAL || sendInterval >= MAX_SEND_INTERVAL)
        return false;

    if (sendInterval == ctx.sendInterval)
        return false;

    ctx.sendInterval = sendInterval;
    Serial.print("Setting sendInterval to: ");
    Serial.println(sendInterval);

    return true;
}

bool checkBluetoothName(String bluetoothName)
{
    bluetoothName.trim();

    if (bluetoothName == "")
        return false;

    if (bluetoothName == ctx.bluetoothName)
        return false;

    ctx.bluetoothName = bluetoothName;
    Serial.print("Setting bluetoothName to: ");
    Serial.println(bluetoothName);

    return true;
}

void receiveBluetootCallback(String message)
{
    StaticJsonDocument<512> doc;
    if (deserializeJson(doc, message))
    {
        Serial.println("Error: Failed to parse JSON");
        return;
    }

    digitalWrite(LED_BUILTIN, LOW);
    delay(60);
    
#if SERIAL_DEBUG
    Serial.println("Message received: ");
    serializeJsonPretty(doc, Serial);
    Serial.println();
#endif

    if (doc.containsKey("sendInterval"))
    {
        unsigned long sendInterval = doc["sendInterval"];
        if (checkSendInterval(sendInterval))
            storage.save();
    }

    if (doc.containsKey("bluetoothName"))
    {
        String bluetoothName = doc["bluetoothName"];
        if (checkBluetoothName(bluetoothName))
            storage.save();
    }

    doc.clear();
    digitalWrite(LED_BUILTIN, HIGH);
}