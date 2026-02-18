#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 19
#define DHTTYPE DHT11

#define LCD_ADDR 0x27
#define LCD_COLS 16
#define LCD_ROWS 2

LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);

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
const int umbralSueloON = 50;    // Encendido (suelo seco)
const int umbralSueloOFF = 80;   // Apagado (suelo suficientemente húmedo)
const float umbralTemp = 25.0;

// Seguridad de riego
const unsigned long tiempoMaxRiegoMs = 10000; // 10 segundos máximo

// Calibracion ADC
const int sueloSeco = 2450;
const int sueloHumedo = 1250;

void setup() {
  Serial.begin(115200);
  
  pinMode(pinReleExtractor, OUTPUT);
  pinMode(pinReleBomba, OUTPUT);
  
  digitalWrite(pinReleBomba, HIGH);
  digitalWrite(pinReleExtractor, HIGH);

  analogReadResolution(12);
  analogSetPinAttenuation(pinSuelo, ADC_11db);
  
  dht.begin();

  Wire.begin();
  lcd.init();
  lcd.backlight();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sistema OK");
  delay(800);
  lcd.clear();

}

void loop() {
  // put your main code here, to run repeatedly:
  leerSensores();
  mostrarDatos();
  
  controlarBomba();
  controlarExtractor();
  
}

void controlarBomba() {
    if (!bombaActiva && humedadSueloPct < umbralSueloON) {
      digitalWrite(pinReleBomba, LOW);
      bombaActiva = true;
      tInicioBomba = millis();
      Serial.println("Bomba ACTIVADA");
    }
    
    if (bombaActiva && (humedadSueloPct >= umbralSueloOFF || millis() - tInicioBomba >= tiempoMaxRiegoMs)) {
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

    // --- DHT11 ---
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

    // ----- LCD 16x2 ----
    lcd.setCursor(0, 0);
    lcd.print("S:");
    lcd.print(humedadSueloPct);
    lcd.print("% T:");
    lcd.print(tempAmb, 1); // 1 decimal

    lcd.setCursor(0, 1);
    lcd.print("H:");
    lcd.print(humAmb, 0); // sin decimales
    lcd.print("% ");

    // Estados (rele ACTIVE-LOW)
    bool bombaON = (digitalRead(pinReleBomba) == LOW);
    bool extON = (digitalRead(pinReleExtractor) == LOW);

    lcd.print("B:");
    lcd.print(bombaON ? "ON " : "OFF");
    lcd.print("E:");
    lcd.print(extON ? "ON " : "OFF");
  }