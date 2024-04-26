#include "pasarela.h"

espnow_gateway_t GW;

void setup()
{
  // Initialize Serial Monitor
	Serial.begin(115200);
  Serial.println();
  GW.set_wifi("infind","1518wifi");
  GW.set_mqtt("iot.ac.uma.es","infind","zancudo");
  GW.begin();
}

void loop()
{
}
