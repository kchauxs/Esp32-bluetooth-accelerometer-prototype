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
    else if (doc.containsKey("wifi"))
    {
        JsonObject wifi = doc["wifi"];

        if (wifi.containsKey("ssid") && wifi.containsKey("pass"))
        {
            String ssid = wifi["ssid"];
            String pass = wifi["pass"];
            
            if (ssid != "" && pass != "")
            {
                ctx.wifi.ssid = ssid;
                ctx.wifi.pass = pass;
                storage.save();
            }
        }
    }
    else if (doc.containsKey("zoom"))
    {
        int zoom = doc["zoom"];
        if (zoom != ctx.zoom)
        {
            ctx.zoom = zoom;
            Serial.print("[INFO] Setting zoom to: ");
            Serial.println(zoom);
        }
    }
    else if (doc.containsKey("reboot"))
    {
        if (doc["reboot"] == true)
        {
            Serial.println("[INFO] Rebooting device");
            ESP.restart();
        }
    }
    else if (doc.containsKey("reset"))
    {
        if (doc["reset"] == true)
        {
            storage.reset();
            Serial.println("[INFO] Resetting device");
        }
    }

    // else if (doc.containsKey("factoryReset"))
    // {
    //     if (doc["factoryReset"] == true)
    //     {
    //         storage.factoryReset();
    //         Serial.println("[INFO] Factory resetting device");
    //     }
    // }
    // else if (doc.containsKey("calibrate"))
    // {
    //     if (doc["calibrate"] == true)
    //     {
    //         sensors.calibrate();
    //         Serial.println("[INFO] Calibrating device");
    //     }
    // }

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