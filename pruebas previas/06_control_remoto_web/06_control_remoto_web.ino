#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Config WiFi
const char* ssid = "Clickylisto";               // <- Reemplazar
const char* password = "Duarte3558";       // <- Reemplazar

// Pines de salida
const int luzPin = 2;
const int ventiladorPin = 4;
const int riegoPin = 5;
const int camaraOnPin = 18;
const int camaraFotoPin = 19;
const int camaraTimelapsePin = 21;

// URLs
const char* serverURL = "https://smart-green.site/smart-green-app/obtener_comando.php";

// Prototipos
void ejecutarComando(String dispositivo, String accion);
void marcarComoEjecutado(int id);

unsigned long tiempoAnterior = 0;
const unsigned long intervalo = 5000;

void setup() {
  Serial.begin(115200);

  pinMode(luzPin, OUTPUT);
  pinMode(ventiladorPin, OUTPUT);
  pinMode(riegoPin, OUTPUT);
  pinMode(camaraOnPin, OUTPUT);
  pinMode(camaraFotoPin, OUTPUT);
  pinMode(camaraTimelapsePin, OUTPUT);

  digitalWrite(luzPin, LOW);
  digitalWrite(ventiladorPin, LOW);
  digitalWrite(riegoPin, LOW);
  digitalWrite(camaraOnPin, LOW);
  digitalWrite(camaraFotoPin, LOW);
  digitalWrite(camaraTimelapsePin, LOW);

  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ Conectado a WiFi");
  Serial.print("IP local: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (millis() - tiempoAnterior >= intervalo) {
    tiempoAnterior = millis();
    consultarComando();
  }
}

void consultarComando() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverURL);
    int httpCode = http.GET();

    if (httpCode == 200) {
      String respuesta = http.getString();
      Serial.println("\n📩 Respuesta del servidor: " + respuesta);

      StaticJsonDocument<256> doc;
      DeserializationError error = deserializeJson(doc, respuesta);

      if (!error && doc.containsKey("dispositivo")) {
        String dispositivo = doc["dispositivo"];
        String accion = doc["accion"];
        ejecutarComando(dispositivo, accion);

        if (doc.containsKey("id")) {
          int id = doc["id"];
          marcarComoEjecutado(id);
        }
      } else {
        Serial.print("❌ Error al parsear JSON: ");
        Serial.println(error.c_str());
      }
    } else {
      Serial.println("❌ Error HTTP: " + String(httpCode));
    }

    http.end();
  }
}

void ejecutarComando(String dispositivo, String accion) {
  int pin = -1;
  bool esEncender = (accion == "on" || accion.indexOf("litro") != -1 || accion == "foto" || accion == "timelapse");

  if (dispositivo == "luz") pin = luzPin;
  else if (dispositivo == "ventilador") pin = ventiladorPin;
  else if (dispositivo == "riego") pin = riegoPin;
  else if (dispositivo == "camara" && accion == "on") pin = camaraOnPin;
  else if (dispositivo == "camara" && accion == "foto") pin = camaraFotoPin;
  else if (dispositivo == "camara" && accion == "timelapse") pin = camaraTimelapsePin;

  if (pin == -1) {
    Serial.println("⚠️ Dispositivo o acción desconocida");
    return;
  }

  Serial.printf("⚙️ Ejecutando: %s -> %s (GPIO %d)\n", dispositivo.c_str(), accion.c_str(), pin);
  digitalWrite(pin, esEncender ? HIGH : LOW);

  if (accion == "foto" || accion == "timelapse") {
    delay(500);
    digitalWrite(pin, LOW);
  }
}

void marcarComoEjecutado(int id) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "https://smart-green.site/smart-green-app/marcar_comando.php?id=" + String(id);
    http.begin(url);
    int httpCode = http.GET();

    if (httpCode == 200) {
      String respuesta = http.getString();
      Serial.println("✅ Comando marcado como ejecutado: " + respuesta);
    } else {
      Serial.println("❌ Error al marcar como ejecutado (HTTP " + String(httpCode) + ")");
    }

    http.end();
  }
}