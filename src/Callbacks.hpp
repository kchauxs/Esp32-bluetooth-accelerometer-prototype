#include <Arduino.h>
#include <ArduinoJson.h>

// --------------------------------------------------------------------------------------------
// Bluetooth Callbacks
// --------------------------------------------------------------------------------------------

const String BLUETOOTH_NAME_KEY = "BLUETOOTH_NAME";
const String BRIGHTNESS_KEY = "BRIGHTNESS";
const String INTERVAL_KEY = "INTERVAL";
const String REBOOT_KEY = "REBOOT";
const String RESET_KEY = "RESET";
const String ZOOM_KEY = "ZOOM";

void changeBluetoothName(const JsonObject &doc)
{
    String bluetoothName = doc["value"].as<String>();

    bluetoothName.trim();

    if (bluetoothName == "")
        return;

    if (bluetoothName == ctx->bluetoothName)
        return;

    ctx->bluetoothName = bluetoothName;
    storage.save();
}

void adjustBrightness(const JsonObject &doc)
{
    uint8_t brightness = doc["value"].as<uint8_t>();
    if (brightness > 0 && brightness <= 255)
    {
        ctx->brightness = brightness;
        FastLED.setBrightness(ctx->brightness);
        storage.save();
    }
}

void adjustSendInterval(const JsonObject &doc)
{
    unsigned long sendInterval = doc["value"].as<unsigned long>();
    if (sendInterval < MIN_SEND_INTERVAL || sendInterval > MAX_SEND_INTERVAL)
        return;

    if (sendInterval == ctx->sendInterval)
        return;

    ctx->sendInterval = sendInterval;
    storage.save();
}

void rebootDevice(const JsonObject &doc)
{
    bool isReboot = doc["value"].as<bool>();
    if (isReboot)
        utils.restartDevice();
}

void resetConfiguration(const JsonObject &doc)
{
    bool isReset = doc["value"].as<bool>();
    if (isReset)
    {
        storage.reset();
        storage.save();
        utils.restartDevice();
    }
}

void adjustZoom(const JsonObject &doc)
{
    uint8_t value = doc["value"].as<uint8_t>();
    if (value != ctx->zoom && value >= 0 && value <= MAX_ZOOM)
        ctx->zoom = value;
}

void handleCommand(const String &command, const JsonObject &doc)
{
    if (command == BLUETOOTH_NAME_KEY)
    {
        changeBluetoothName(doc);
    }
    else if (command == BRIGHTNESS_KEY)
    {
        adjustBrightness(doc);
    }
    else if (command == INTERVAL_KEY)
    {
        adjustSendInterval(doc);
    }
    else if (command == REBOOT_KEY)
    {
        rebootDevice(doc);
    }
    else if (command == RESET_KEY)
    {
        resetConfiguration(doc);
    }
    else if (command == ZOOM_KEY)
    {
        adjustZoom(doc);
    }
}

void receiveBluetootCallback(String message)
{
    StaticJsonDocument<1024> doc;
    if (deserializeJson(doc, message))
        return;

#if SERIAL_DEBUG
    Serial.println("\n[INFO]\t Message received: ");
    serializeJsonPretty(doc, Serial);
    Serial.println();
#endif

    if (!doc.containsKey("command") || !doc.containsKey("value"))
        return;

    digitalWrite(LED_BUILTIN, HIGH);
    delay(30);

    String command = doc["command"].as<String>();
    JsonObject jsonObject = doc.as<JsonObject>();
    handleCommand(command, jsonObject);
    digitalWrite(LED_BUILTIN, LOW);
}

String payloadCallback()
{
    String payload = utils.buildPayload();
    return payload;
}

//---------------------------------------------------------------------------------------------
// Button Callbacks
// --------------------------------------------------------------------------------------------

void zoomIn()
{
    ctx->zoom = ctx->zoom + 1;
    if (ctx->zoom > MAX_ZOOM)
        ctx->zoom = MAX_ZOOM;
}

void zoomOut()
{
    ctx->zoom = ctx->zoom - 1;
    if (ctx->zoom < MIN_ZOOM)
        ctx->zoom = MIN_ZOOM;
}
