#include <Arduino.h>

#include "AD5593R.h"

AD5593R AD5593R(23);

bool my_DACs[8] = {1,1,1,1,0,0,0,0};
bool my_ADCs[8] = {0,0,0,0,1,1,1,1};
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  AD5593R.enable_internal_Vref();
  AD5593R.set_ADC_max_2x_Vref();
  AD5593R.configure_ADCs(my_ADCs);

}


void loop() {

  AD5593R.read_ADCs();
}
