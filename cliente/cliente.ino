#include "AUTOpairing.h"

AUTOpairing_t clienteAP;

typedef struct
{
  uint8_t pan;
  uint16_t timeout;
  uint16_t tsleep;
  uint16_t time;
} struct_config;
struct_config strConfig;

//-----------------------------------------------------------
void setup() {

  clienteAP.init_config_size(sizeof(strConfig));
  // if (clienteAP.get_config((uint16_t *)&strConfig) == false) {
  //   strConfig.timeout = 3000;
  //   strConfig.tsleep = 30;
  // }
  // Init Serial Monitor
  Serial.begin(115200);
  clienteAP.set_channel(1);           // canal donde empieza el scaneo
  clienteAP.set_app_area(4, 5);       // indico PAN y aplicación a la que pertenece
  clienteAP.set_timeOut(3000, true);  // tiempo máximo
  clienteAP.set_deepSleep(30);        //tiempo dormido en segundos
  clienteAP.set_debug(true);          // depuración, inicializar Serial antes
  clienteAP.begin();
}

//-----------------------------------------------------------
void loop() {
  if (clienteAP.envio_disponible()) {    
    char mensaje[256];
    char *send_topic = "unbuentopic/tienecache";
    sprintf(mensaje, "{\"topic\":\"datos\",\"temp\":%4.2f, \"hum\":%4.2f }", 24.2, 46.0);
    clienteAP.espnow_send_check(send_topic, mensaje);  // hará deepsleep por defecto
  }
}
