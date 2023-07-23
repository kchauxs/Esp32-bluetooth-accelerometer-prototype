#include <Arduino.h>
#include <ArduinoJson.h>

// --------------------------------------------------------------------------------------------
// Bluetooth Callbacks
// --------------------------------------------------------------------------------------------

String payloadCallback()
{
    // sensors.readMPU();
    String payload = utils.buildPayload();
    return payload;
}

bool checkSendInterval(unsigned long sendInterval)
{
    if (sendInterval <= MIN_SEND_INTERVAL || sendInterval >= MAX_SEND_INTERVAL)
        return false;

    if (sendInterval == ctx.sendInterval)
        return false;

    ctx.sendInterval = sendInterval;
#if SERIAL_DEBUG
    Serial.print("[INFO]\tSetting sendInterval to: ");
    Serial.println(sendInterval);
#endif
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
#if SERIAL_DEBUG
    Serial.print("[INFO]\tSetting bluetoothName to: ");
    Serial.println(bluetoothName);
#endif
    return true;
}

void receiveBluetootCallback(String message)
{
    StaticJsonDocument<1024> doc;
    if (deserializeJson(doc, message))
    {
        Serial.println("Error: Failed to parse JSON");
        return;
    }

    digitalWrite(LED_BUILTIN, HIGH);
    delay(60);

#if SERIAL_DEBUG
    Serial.println("[INFO]\tMessage received: ");
    serializeJsonPretty(doc, Serial);
    Serial.println();
#endif

    if (doc.containsKey("sendInterval"))
    {
        unsigned long sendInterval = doc["sendInterval"];
        if (checkSendInterval(sendInterval))
            storage.save();
    }
    else if (doc.containsKey("bluetoothName"))
    {
        String bluetoothName = doc["bluetoothName"];
        if (checkBluetoothName(bluetoothName))
            storage.save();
    }

    else if (doc.containsKey("zoom"))
    {
        int zoom = doc["zoom"];
        if (zoom != ctx.zoom)
        {
            ctx.zoom = zoom;
            Serial.print("[INFO]\tSetting zoom to: ");
            Serial.println(zoom);
        }
    }
    else if (doc.containsKey("reboot"))
    {
        if (doc["reboot"] == true)
        {
            Serial.println("[INFO]\tRebooting device");
            ESP.restart();
        }
    }

    else if (doc.containsKey("reset"))
    {
        if (doc["reset"] == true)
        {
            storage.reset();
            storage.save();
            Serial.println("[INFO]\tResetting device");
        }
    }

    doc.clear();
    digitalWrite(LED_BUILTIN, LOW);
}

//---------------------------------------------------------------------------------------------
// Button Callbacks
// --------------------------------------------------------------------------------------------

void zoomInCallback()
{
    ctx.zoom = ctx.zoom + 1;
    if (ctx.zoom > MAX_ZOOM)
        ctx.zoom = MAX_ZOOM;

    Serial.print("[INFO]\tSetting zoom in to: ");
    Serial.println(ctx.zoom);
}

void zoomOutCallback()
{
    ctx.zoom = ctx.zoom - 1;
    if (ctx.zoom < MIN_ZOOM)
        ctx.zoom = MIN_ZOOM;

    Serial.print("[INFO]\tSetting zoom out to: ");
    Serial.println(ctx.zoom);
}