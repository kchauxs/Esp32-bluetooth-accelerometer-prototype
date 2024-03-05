#include "BluetoothService.h"

BluetoothService::BluetoothService(Context *ctx, BluetoothSerial *SerialBT)
{
    this->_ctx = ctx;
    this->_serialBT = SerialBT;
}

void BluetoothService::init()
{
    this->_serialBT->begin(_ctx->bluetoothName);
}

void BluetoothService::send(String message)
{
    _serialBT->println(message);
}

void BluetoothService::sendLoop(String (*callback)())
{
    static unsigned long lastRead = 0;
    if (millis() - lastRead > _ctx->sendInterval)
    {
        lastRead = millis();
        String message = callback();
        this->send(message);

#if SERIAL_DEBUG
        Serial.print("\n[INFO]\t Send message: ");
        Serial.println(message);
#endif
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

bool BluetoothService::hasClient()
{
    return _serialBT->hasClient();
}
