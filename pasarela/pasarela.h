
#include <esp_now.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <queue> 
#include <list>
#include <string>
#include <algorithm>

#include "AUTOpairing_common.h"

class espnow_gateway_t
{
 String mqtt_server;
 String mqtt_user;
 String mqtt_pass;
 int mqtt_port;
 int mqtt_packet_size;
 WiFiClient wClient;
 PubSubClient mqtt_client(wClient);
 String ID_PLACA;

// Replace with your network credentials (STATION)
 String wifi_ssid;
 String wifi_password;

 espnow_pateway_t()
 {

 }

//-----------------------------------------------------
//RECIBIR MQTT
void procesa_mensaje(char* topic, byte* payload, unsigned int length) { 
  String mensaje=String(std::string((char*) payload,length).c_str());
  Serial.println("Mensaje recibido ["+ String(topic) +"] "+ mensaje);//LEVEL CON LO DE PROCESS
  // compruebo el topic
  if(String(topic) == "infind/espnowdevice")
  {
    StaticJsonDocument<512> json; // el tamaño tiene que ser adecuado para el mensaje
    // Deserialize the JSON document
    DeserializationError error = deserializeJson(json, mensaje.c_str());

    // Compruebo si no hubo error
    if (error) {
      Serial.print("Error funcion deserializeJson(): ");
      Serial.println(error.c_str());
    }
    else
    if(!json.containsKey("mac") && !json.containsKey("topic") && !json.containsKey("payload"))  // comprobar si existe el campo/clave que estamos buscando
    { 
      Serial.print("Error : ");
      Serial.println("campos mac, topic o payload no encontrados");
    }
    else
    {
     String mac =json["mac"];
     String topic =json["topic"];
     String output;
     serializeJson(json["payload"], output);

     String compacto = topic + "|" + output;
     
     Serial.print("Mensaje OK, mac = ");
     Serial.println(mac);//es lo que hay que sacar
      
     uint8_t mac_addr[6];
     HEX2byte(mac_addr, (char*) mac.c_str() );
     mensajes_MQTT.push_back(TmensajeMQTT(mac_addr, (uint8_t *)compacto.c_str(), compacto.length()));
     Serial.print("compacto = ");
     Serial.println(compacto);//es lo que hay que sacar
     
    }
  } // if topic
  else
  {
    Serial.println("Error: Topic desconocido");
  }

}


 void set_mqtt(String server, int port=1883,int size=512, String user, String passwd)
 {
  mqtt_server=server;
  mqtt_user=user;
  mqtt_pass = passwd;
  mqtt_port = port;
  mqtt_packet_size= size;
 }

 void set_wifi(String SSID, String passwd)
 {
  wifi_ssid = SSID;
  wifi_password= passwd;
 }


 void begin()
 {
  ID_PLACA="ESP_"+ String(ESP.getEfuseMac());
  conecta_wifi();
  mqtt_client.setServer(mqtt_server.c_str(), mqtt_port);
  mqtt_client.setBufferSize(mqtt_packet_size); // para poder enviar mensajes de hasta X bytes
  mqtt_client.setCallback(procesa_mensaje);
  //initESP_NOW();
 }

private:
//-----------------------------------------------------
 void conecta_wifi() {
  Serial.printf("\nConnecting to %s:\n", ssid);
 
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(50);
    Serial.print(".");
  }
  Serial.printf("\nWiFi connected, IP address: %s\n", WiFi.localIP().toString().c_str());
}

//-----------------------------------------------------
void conecta_mqtt() {
  // Loop until we're reconnected
  while (!mqtt_client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqtt_client.connect(ID_PLACA.c_str(), mqtt_user.c_str(), mqtt_pass.c_str())) {
      Serial.printf(" conectado a broker: %s\n",mqtt_server);
      mqtt_client.subscribe("infind/espnowdevice");
    } else {
      Serial.printf("failed, rc=%d  try again in 5s\n", mqtt_client.state());
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


}