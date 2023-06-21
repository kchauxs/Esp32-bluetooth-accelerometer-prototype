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
