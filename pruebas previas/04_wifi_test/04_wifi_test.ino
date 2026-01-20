#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Clickylisto";
const char* password = "Duarte3558";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println ("Conectando a WiFi...");

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println ("\nConexión exitosa");
  Serial.print("IP asignada: ");
  Serial.println(WiFi.localIP());

}

void loop() {
  // put your main code here, to run repeatedly:
  
  }
