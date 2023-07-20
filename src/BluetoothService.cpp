#include "BluetoothService.h"

BluetoothService::BluetoothService(Context *ctx, BluetoothSerial *SerialBT)
{
    this->_ctx = ctx;
    this->_serialBT = SerialBT;
}

void BluetoothService::init(String localName)
{
    this->_serialBT->begin(localName);
}

void BluetoothService::send(String message)
{
    _serialBT->println(message);
}

void BluetoothService::sendLoop(String (*callback)())
{
    static unsigned long lastRead = 0;
    if (millis() - lastRead > ctx.sendInterval)
    {
        lastRead = millis();
        if (!_serialBT->hasClient())
            return;

        String message = callback();
#if SERIAL_DEBUG
        Serial.print("\n[INFO] Send message: ");
        Serial.println(message);
#endif

        this->send(message);
    }
}

String BluetoothService::receive()
{
    String message = "";
    if (_serialBT->available())
    {
        String message = _serialBT->readString();

        if (message.endsWith("\n"))
            message = message.substring(0, message.length() - 1);
    }
    return message;
}

void BluetoothService::receive(void (*callback)(String))
{
    String message = "";

    if (_serialBT->available())
    {
        String message = _serialBT->readString();

        if (message.endsWith("\n"))
            message = message.substring(0, message.length() - 1);

        if (message != "")
            callback(message);
    }
}
