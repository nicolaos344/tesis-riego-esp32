// Código para probar la visualización del menú en la pantalla OLED
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int menuOption = 0;
String menuItems[] = {"Modo: Manual", "Modo: Automático", "Germinación", "Vegetativo", "Floración"};
const int totalItems = 5;

void setup() {
    Serial.begin(115200);
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("Error al iniciar la pantalla OLED");
        while (true);
    }
    display.clearDisplay();
}

void loop() {
    showMenu();
    delay(1000); // Simula cambio de opción cada segundo
    menuOption = (menuOption + 1) % totalItems;
}

void showMenu() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    for (int i = 0; i < totalItems; i++) {
        display.setCursor(0, i * 10);
        if (i == menuOption) {
            display.print("> "); // Indicador de selección
        } else {
            display.print("  ");
        }
        display.println(menuItems[i]);
    }
    display.display();
}
