#include <Arduino.h>
#include "KarakuriBLE.h"
#include "../constants/config.h"

bool deviceConnected = false;
bool oldDeviceConnected = false;
char value[50] = "Default";
// const int dataBLELength = 5;
float dataBLE[dataBLELength];
char valores[dataBLELength];
String colorSet;

uint8_t sensordown[16] = {0, 0, 132, 66, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
uint8_t sensorup[16] = {0, 0, 168, 66, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic = NULL;
BLECharacteristic *pCharacteristicCommand = 0;
BLECharacteristic *pCharacBateria = NULL;
BLECharacteristic *pCharacSensores = NULL;
BLECharacteristic *pCharacteristicData = 0;
BLECharacteristic *THROTTLE = 0;
BLECharacteristic *BATTERY = 0;

void KarakuriBLE::Start()
{
    // Create the BLE Device
    BLEDevice::init("SmartInsoleL");

    // Create the BLE Server
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    // Create the BLE Service
    BLEService *pService = pServer->createService(SERVICE_UUID);

    // Create a BLE Characteristic
    pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_WRITE_NR |
            BLECharacteristic::PROPERTY_NOTIFY); //|
                                                 // BLECharacteristic::PROPERTY_INDICATE
    // https://www.bluetooth.com/specifications/gatt/viewer?
    // attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
    // Create a BLE Descriptor
    pCharacteristic->setCallbacks(new MyCallbacks());
    pCharacteristic->addDescriptor(new BLE2902());

    // nuevo
    pCharacteristicCommand = pService->createCharacteristic(
        CHARACTERISTIC_OUT_UUID,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_WRITE_NR |
            BLECharacteristic::PROPERTY_NOTIFY);

    pCharacteristicCommand->setCallbacks(new MyCallbacks());
    // pCharacteristicCommand->setValue("");
    pCharacteristicCommand->addDescriptor(new BLE2902());

    // nuevo
    pCharacBateria = pService->createCharacteristic(
        CHARACTERISTIC_BATERIA,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_WRITE_NR |
            BLECharacteristic::PROPERTY_NOTIFY);

    pCharacBateria->setCallbacks(new MyCallbacks());
    // pCharacteristicCommand->setValue("");
    pCharacBateria->addDescriptor(new BLE2902());

    pCharacSensores = pService->createCharacteristic(
        CHARACTERISTIC_SENSORES,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_WRITE_NR |
            BLECharacteristic::PROPERTY_NOTIFY);

    // https://www.bluetooth.com/specifications/gatt/viewer?
    // attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
    // Create a BLE Descriptor
    pCharacSensores->setCallbacks(new MyCallbacks());
    pCharacSensores->addDescriptor(new BLE2902());

    // Start the service
    pService->start();

    // Start advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(false);
    pAdvertising->setMinPreferred(0x00); // set value to 0x00 to not advertise this parameter
    BLEDevice::startAdvertising();
    Serial.println("Waiting a client connection to notify...");
}

void KarakuriBLE::Check(float sensor)
{
    if (deviceConnected)
    {
        if (sensor == 1)
        {
            pCharacteristic->setValue(sensordown, 16);
            // pCharacteristicCommand->setValue(sensor);
            delay(100);
        }
        else if (sensor == 2)
        {
            pCharacteristic->setValue(sensorup, 16);
            // pCharacteristicCommand->setValue(sensor);
            delay(100);
        }
    }
}
void KarakuriBLE::Update()
{
    // disconnecting
    if (!deviceConnected && oldDeviceConnected)
    {
        delay(500);                  // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected)
    {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
}