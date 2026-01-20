void setup() {
    Serial.begin(115200);  // Inicializar comunicación serie
    pinMode(2, OUTPUT);    // Configurar el pin 2 como salida
    Serial.println("ESP32 iniciado correctamente...");
}

void loop() {
    Serial.println("Encendiendo LED...");
    digitalWrite(2, HIGH); // Enciende el LED
    delay(1000);

    Serial.println("Apagando LED...");
    digitalWrite(2, LOW);  // Apaga el LED
    delay(1000);
}

