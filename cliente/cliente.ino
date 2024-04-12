#include "AUTOpairing.h"

AUTOpairing_t clienteAP;

//-----------------------------------------------------------
void setup() {

  // Init Serial Monitor
  Serial.begin(115200);
  clienteAP.set_channel(6);  // canal donde empieza el scaneo
  clienteAP.set_timeOut(3000,true); // tiempo máximo
  clienteAP.set_debug(true);   // depuración, inicializar Serial antes
  clienteAP.set_deepSleep(10);  //tiempo dormido en segundos
   
  clienteAP.begin();
}

//-----------------------------------------------------------
void loop() {
  clienteAP.mantener_conexion();
  
  if (clienteAP.envio_disponible() ) { 
      char mensaje[256];
      sprintf(mensaje, "{\"topic\":\"datos\",\"temp\":%4.2f, \"hum\":%4.2f }", 24.2, 46.0);
      clienteAP.espnow_send(mensaje); // hará deepsleep por defecto
  }
}
