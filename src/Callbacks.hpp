#include <Arduino.h>
#include <ArduinoJson.h>

// --------------------------------------------------------------------------------------------
// Bluetooth Callbacks
// --------------------------------------------------------------------------------------------

String sendBluetoothCallback()
{
    sensors.readMPU();
    String payload = utils.buildPayload();
    Serial.print("[INFO] Sending payload: ");
    Serial.println(payload);
    return payload;
}

bool checkSendInterval(unsigned long sendInterval)
{
    if (sendInterval <= MIN_SEND_INTERVAL || sendInterval >= MAX_SEND_INTERVAL)
        return false;

    if (sendInterval == ctx.sendInterval)
        return false;

    ctx.sendInterval = sendInterval;
    Serial.print("[INFO] Setting sendInterval to: ");
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
    Serial.print("[INFO] Setting bluetoothName to: ");
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

    digitalWrite(LED_BUILTIN, HIGH);
    delay(60);

#if SERIAL_DEBUG
    Serial.println("[INFO] Message received: ");
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

    Serial.print("[INFO] Setting zoom in to: ");
    Serial.println(ctx.zoom);
}

void zoomOutCallback()
{
    ctx.zoom = ctx.zoom - 1;
    if (ctx.zoom < MIN_ZOOM)
        ctx.zoom = MIN_ZOOM;

    Serial.print("[INFO] Setting zoom out to: ");
    Serial.println(ctx.zoom);
}