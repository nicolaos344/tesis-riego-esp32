#include "DHT.h"

#define DHTPIN 4      // Pin de datos del sensor DHT22
#define DHTTYPE DHT11 // Tipo de sensor

DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(115200);  // Iniciar comunicación serie
    dht.begin();           // Iniciar sensor DHT22
    Serial.println("Iniciando lectura del sensor DHT11...");
}

void loop() {
    float temperatura = dht.readTemperature(); // Leer temperatura en °C
    float humedad = dht.readHumidity();       // Leer humedad en %

    // Verificar si los valores son válidos
    if (isnan(temperatura) || isnan(humedad)) {
        Serial.println("Error al leer el sensor DHT11.");
    } else {
        Serial.print("Temperatura: ");
        Serial.print(temperatura);
        Serial.print(" °C | Humedad: ");
        Serial.print(humedad);
        Serial.println(" %");
    }

    delay(10000); // Esperar 2 segundos antes de la próxima lectura
}
