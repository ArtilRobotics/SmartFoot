#include <Arduino.h>
#include <SmartInsole.h>

SmartInsole Plantilla;

void setup()
{
  // // put your setup code here, to run once:
  // M5.begin(true, true, true, false);
  // Sensor1.enable_internal_Vref();
  // Sensor1.set_ADC_max_2x_Vref();
  // Sensor1.configure_ADCs(my_ADCs);

  // delay(1000);
Serial.begin(115200);
  Plantilla.init();
}

void loop()
{

  Plantilla.run();

  // USBSerial.println("Sensor 1");
  // Sensor1.read_ADCs();
  // delay(1000);

  // Dato_Sensor_1 = Sensor1.read_ADC(0);
  // Dato_Sensor_2 = Sensor1.read_ADC(0);
  // Dato_Sensor_3 = Sensor1.read_ADC(0);
  // Dato_Sensor_4 = Sensor1.read_ADC(0);
  // Dato_Sensor_5 = Sensor1.read_ADC(0);
  // Dato_Sensor_6 = Sensor1.read_ADC(0);
  // Dato_Sensor_7 = Sensor1.read_ADC(0);
  // Dato_Sensor_8 = Sensor1.read_ADC(0);
}
