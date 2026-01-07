#include <DHT.h>

#define DHTPIN 19
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// Pines
const int pinSuelo = 33;
const int pinReleExtractor = 26;
const int pinReleBomba = 27;

//Variables
float tempAmb, humAmb;
int humedadSueloPct;

bool bombaActiva = false;
unsigned long tInicioBomba = 0;

//Umbrales
const int umbralSuelo = 50;
const float umbralTemp = 25.0;

// Calibracion ADC
const int sueloSeco = 4095;
const int sueloHumedo = 1900;

void setup() {
  Serial.begin(115200);
  
  pinMode(pinReleExtractor, OUTPUT);
  pinMode(pinReleBomba, OUTPUT);
  
  digitalWrite(pinReleBomba, HIGH);
  digitalWrite(pinReleExtractor, HIGH);

  dht.begin();

}

void loop() {
  // put your main code here, to run repeatedly:
  leerSensores();
  mostrarDatos();
  
  controlarBomba();
  controlarExtractor();
  
}

void controlarBomba() {
    if (!bombaActiva && humedadSueloPct < 50) {
      digitalWrite(pinReleBomba, LOW);
      bombaActiva = true;
      tInicioBomba = millis();
      Serial.println("Bomba ACTIVADA");
    }
    
    if (bombaActiva && millis() - tInicioBomba >= 5000) {
      digitalWrite(pinReleBomba, HIGH);
      bombaActiva = false;
      Serial.println("Bomba DESACTIVADA");
    }
  }

  void controlarExtractor() {
    if (humAmb > 60.0) {
      digitalWrite(pinReleExtractor, LOW);
    } else {
      digitalWrite(pinReleExtractor, HIGH);
    }
  }

  //Funcion auxiliar: promediar ADC
    int leerSueloPromedio(int muestras) {
      long suma = 0;
      for (int i = 0; i < muestras; i++) {
        suma += analogRead(pinSuelo);
        delay(50); // pequeño delay para estabilidad
      }
      return suma / muestras;
    }

  void leerSensores() {

    int lecturaSuelo = leerSueloPromedio(10);

    humedadSueloPct = map(lecturaSuelo, sueloSeco, sueloHumedo, 0, 100);
    humedadSueloPct = constrain(humedadSueloPct, 0, 100);

    // --- DHT22 ---
    float t = dht.readTemperature();
    float h = dht.readHumidity();

    if (!isnan(t)) tempAmb = t;
    if (!isnan(h)) humAmb = h;

  }

  void mostrarDatos() {
    Serial.print("Suelo: ");
    Serial.print(humedadSueloPct);
    Serial.print("% | T: ");
    Serial.print(tempAmb);
    Serial.print("C | H: ");
    Serial.print(humAmb);
    Serial.println("%");
  }