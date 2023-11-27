#pragma once

#ifndef KarakuriBLE_h
#define KarakuriBLE_h

#include <Arduino.h>

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

static const char *TAGx = "BLE";

extern bool deviceConnected;
extern bool oldDeviceConnected;
extern char value[50];
const int dataBLELength = 5;
extern float dataBLE[dataBLELength];
extern char valores[dataBLELength];
extern String colorSet;

extern BLEServer *pServer;
extern BLECharacteristic *pCharacteristic;
extern BLECharacteristic *pCharacteristicCommand;
extern BLECharacteristic *pCharacBateria;
extern BLECharacteristic *pCharacSensores;
extern BLECharacteristic *pCharacteristicData;

class MyServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer)
    {
        ESP_LOGI(TAGx, "BLE_ON_CONNECTED");
        Serial.println("Conectado");
        deviceConnected = true;
    };

    void onDisconnect(BLEServer *pServer)
    {
        ESP_LOGI(TAGx, "BLE_ON_DISCONNECTED");
        Serial.println("Desconectado");
        deviceConnected = false;
    }
};

class MyCallbacks : public BLECharacteristicCallbacks
{
    void onRead(BLECharacteristic *pCharacteristic)
    {
    }

    // converting a constant char into a String
    int my_atoi(const char s)
    {
        char someChar = s;
        String stringOne = String(someChar);
        int d = stringOne.toInt();
        return (d);
    }

    String valor;
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        std::string value = pCharacteristic->getValue();
        if (value.length() > 0)
        {
            valor = "";
            for (int i = 2; i < value.length(); i++)
            {
                // Serial.print("Presenta>  ");
                // Serial.println(value[i]); // Presenta value.
                valor = valor + value[i];
            }
            //dataBLE[0] = my_atoi(value[0]);
            valores[0] = value[0];
            dataBLE[1] = atoi(valor.c_str());
            colorSet=valor;
            // Serial.println(value[0]);
            // Serial.println(value[1]);
            // Serial.println(value[2]);
            // Serial.println(value[3]);
            Serial.println(valores[0]);
            Serial.println(dataBLE[1]);
            // Serial.println("DataBle");
            // for (int j = 0; j < 2; j++)
            // {
            //     Serial.println(dataBLE[j]);
            // }
        }
    }
};

class KarakuriBLE
{
public:
    void Start();
    void Update();
    void Check(float sensor);

private:

float _sensor;
};

#endif
