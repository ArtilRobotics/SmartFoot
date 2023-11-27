#include <SmartInsole.h>
#include <Arduino.h>
#include <KarakuriBLE.h>
#include <Wire.h>
#include "AD5593R.h"
#include "M5AtomS3.h"
#include "I2C_MPU6886.h"
//#include "MadgwickAHRS.h"
#include "Madgwick.h"

#include "../constants/config.h"

AD5593R Sensor1(10, 17);
// AD5593R Sensor2(3, 17);

// AD5593R Sensor1(23, 16);
// AD5593R Sensor2(23, 17);

bool my_ADCs[8] = {1, 1, 1, 1, 1, 1, 1, 1};
bool my_ADC2s[8] = {1, 1, 1, 1, 1, 1, 1, 1};

uint8_t Sensores_1[16];
uint8_t Datos_IMU[16];

float Dato_Sensor_1;
float Dato_Sensor_2;
float Dato_Sensor_3;
float Dato_Sensor_4;
float Dato_Sensor_5;
float Dato_Sensor_6;
float Dato_Sensor_7;
float Dato_Sensor_8;
float Dato_Sensor_9;
float Dato_Sensor_10;
float Dato_Sensor_11;
float Dato_Sensor_12;
float Dato_Sensor_13;
float Dato_Sensor_14;
float Dato_Sensor_15;
float Dato_Sensor_16;

float Map_Sensor_1;
float Map_Sensor_2;
float Map_Sensor_3;
float Map_Sensor_4;
float Map_Sensor_5;
float Map_Sensor_6;
float Map_Sensor_7;
float Map_Sensor_8;
float Map_Sensor_9;
float Map_Sensor_10;
float Map_Sensor_11;
float Map_Sensor_12;
float Map_Sensor_13;
float Map_Sensor_14;
float Map_Sensor_15;
float Map_Sensor_16;

KarakuriBLE BLE;

I2C_MPU6886 imu(I2C_MPU6886_DEFAULT_ADDRESS, Wire1);

Madgwick filter;

union thing
{
    float d;
    uint8_t b[8];
};
void setFloatToArray(uint8_t array[], uint8_t index, union thing v)
{
    for (uint8_t i = 0; i < 4; i += 1)
        array[index + i] = v.b[i];
}

void SmartInsole::init()
{

    M5.begin(true, true, true, false);
    delay(200);

    Sensor1.enable_internal_Vref();
    Sensor1.set_ADC_max_2x_Vref();
    Sensor1.configure_ADCs(my_ADCs);
    delay(200);
    // Sensor2.enable_internal_Vref();
    // Sensor2.set_ADC_max_2x_Vref();
    // Sensor2.configure_ADCs(my_ADC2s);
    delay(100);
    BLE.Start();
    imu.begin();
    filter.begin(0.2f);
}

float roll, pitch, yaw;

void SmartInsole::run()
{

    float ax, ay, az, gx, gy, gz;
    imu.getAccel(&ax, &ay, &az);
    imu.getGyro(&gx, &gy, &gz);

    filter.updateIMU(gx, gy, gz, ax, ay, az);

    // roll = filter.getRoll();
    // pitch = filter.getPitch();
    // yaw = filter.getYaw();

    USBSerial.print("QW: ");
    USBSerial.print(filter.getQuatW());
    USBSerial.print("\tQX: ");
    USBSerial.print(filter.getQuatX());
    USBSerial.print("\tQY: ");
    USBSerial.print(filter.getQuatY());
    USBSerial.print("\tQZ: ");
    USBSerial.println(filter.getQuatZ());

    union thing IMU[4];

    union thing FSR[16];

    //////////////////datos para matriz
    IMU[0].d = -(roll + 90);
    IMU[1].d = pitch;
    IMU[2].d = -(yaw + 180);
    IMU[3].d = 0;

    for (uint8_t i = 0; i < 4; i += 1)
    {
        setFloatToArray(Datos_IMU, i * 4, IMU[i]);
    }
    if (deviceConnected)
    {
        pCharacteristic->setValue(Datos_IMU, 16);
    }

    FSR[0].d = Map_Sensor_1;
    FSR[1].d = Map_Sensor_2;
    FSR[2].d = Map_Sensor_3;
    FSR[3].d = Map_Sensor_4;
    FSR[4].d = Map_Sensor_5;
    FSR[5].d = Map_Sensor_6;
    FSR[6].d = Map_Sensor_7;
    FSR[7].d = Map_Sensor_8;
    FSR[8].d = Map_Sensor_9;
    FSR[9].d = Map_Sensor_10;
    FSR[10].d = Map_Sensor_11;
    FSR[11].d = Map_Sensor_12;
    FSR[12].d = Map_Sensor_13;
    FSR[13].d = Map_Sensor_14;
    FSR[14].d = Map_Sensor_15;
    FSR[15].d = Map_Sensor_16;

    for (uint8_t j = 0; j < 16; j += 1)
    {
        setFloatToArray(Sensores_1, j * 4, FSR[j]);
    }
    if (deviceConnected)
    {
        pCharacSensores->setValue(Sensores_1, 64);
    }

    BLE.Update();
}
