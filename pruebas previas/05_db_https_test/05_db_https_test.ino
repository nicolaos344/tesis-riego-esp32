#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

#define DHTPIN 4      // Pin donde está conectado el DHT22
#define DHTTYPE DHT11 // Tipo de sensor

DHT dht(DHTPIN, DHTTYPE);

// Configuración WiFi
const char* ssid = "Avalos";        // Nombre de tu red WiFi
const char* password = "23032252"; // Contraseña de tu WiFi

// URL del servidor donde está la base de datos
const char* serverUrl = "https://smart-green.site/guardar_datos.php";

void setup() {
    Serial.begin(115200);
    dht.begin();

    // Conectar a WiFi
    WiFi.begin(ssid, password);
    Serial.print("Conectando a WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
    }
    Serial.println("\nConectado a WiFi");
}

void loop() {
    // Leer temperatura y humedad
    float temperatura = dht.readTemperature();
    float humedad = dht.readHumidity();

    if (isnan(temperatura) || isnan(humedad)) {
        Serial.println("Error al leer el sensor DHT11");
        return;
    }

    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.print(" °C - Humedad: ");
    Serial.print(humedad);
    Serial.println(" %");

    // Enviar datos al servidor
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        String url = String(serverUrl) + "?temperatura=" + String(temperatura) + "&humedad=" + String(humedad);
        
        Serial.println("URL final: " + url);

        http.begin(url);
        int httpResponseCode = http.GET();

        if (httpResponseCode > 0) {
            Serial.print("Datos enviados. Código HTTP: ");
            Serial.println(httpResponseCode);
        } else {
            Serial.print("Error en la conexión: ");
            Serial.println(httpResponseCode);
        }

        http.end();
    } else {
        Serial.println("Error: No hay conexión WiFi");
    }

    delay(50000); // Esperar 5 segundos antes de la siguiente medición
}
