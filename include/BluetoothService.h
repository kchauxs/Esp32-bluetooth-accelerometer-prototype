#ifndef __BLUETOOTHSERVICE_H__
#define __BLUETOOTHSERVICE_H__

#include <Arduino.h>
#include "BluetoothSerial.h"

#include "Config.h"
#include "Context.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

class BluetoothService
{
private:
    Context *_ctx;
    BluetoothSerial *_serialBT;

public:
    BluetoothService(Context *ctx, BluetoothSerial *SerialBT);
    void init();
    void send(String message);
    void sendLoop(String (*callback)());
    String receive();
    void receive(void (*callback)(String));
    bool hasClient();
};

#endif // __BLUETOOTHSERVICE_H__