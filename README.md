# Sistema de Monitoreo y Riego Automatizado con ESP32  
**Proyecto Final – Mecatrónica / IoT aplicado a Agricultura Inteligente**

---

## 1. Descripción general del proyecto
Este proyecto consiste en el desarrollo de un sistema IoT basado en **ESP32** para el monitoreo de variables ambientales y del suelo, orientado a aplicaciones de **agricultura inteligente**.  

El sistema permite:
- Medir temperatura y humedad ambiente.
- Medir humedad del suelo.
- Activar actuadores (bomba de riego y extractor/ventilación).
- Visualizar datos localmente y enviarlos a la nube.
- Escalar hacia control remoto y análisis histórico.

> **REVISAR:** ajustar descripción si el alcance final cambia.

---

## 2. Hardware utilizado
- Microcontrolador: ESP32 Dev Module  
- Sensor de temperatura y humedad: DHT11  
- Sensor de humedad de suelo: Sensor capacitivo (salida analógica)  
- Módulo de relés: 2 canales (bomba y extractor)  
- Fuente de alimentación adecuada para relés y ESP32  

> **REVISAR:** agregar modelo exacto del ESP32 y tensión de alimentación final.

---

## 3. Asignación de pines (ESP32)

| Función | Pin ESP32 |
|------|---------|
| DHT11 (DATA) | GPIO 19 |
| Humedad de suelo (ADC) | GPIO 33 |
| Relé bomba | GPIO 27 |
| Relé extractor | GPIO 26 |

> **REVISAR:** confirmar pines finales y si hubo cambios durante pruebas.

---

## 4. Funcionalidad del sistema
- Lectura periódica de sensores.
- Conversión de humedad de suelo a porcentaje mediante calibración.
- Activación automática de bomba según umbral de humedad.
- Activación de extractor según temperatura ambiente.
- Envío de datos a plataforma IoT (ThingSpeak).
- Visualización local mediante monitor serie.

---

## 5. Calibración del sensor de humedad de suelo
La lectura analógica del sensor capacitivo fue calibrada empíricamente:

- Valor suelo seco: **4095**
- Valor suelo húmedo: **1900**

Estos valores se convierten a porcentaje mediante mapeo lineal.

> **REVISAR:** documentar método de calibración (cómo y dónde se midió).

---

## 6. Comunicación y envío de datos a la nube
Inicialmente se realizaron pruebas de comunicación mediante **HTTPS**, enviando y recibiendo datos desde un sitio web con base de datos MySQL.

Posteriormente se decidió migrar a plataformas IoT especializadas (ThingSpeak / MQTT) debido a:
- Menor consumo de recursos.
- Mayor escalabilidad.
- Mejor adaptación a telemetría continua.

---

## 7. Metodología de desarrollo
El proyecto se desarrolló de manera **iterativa e incremental**, realizando pruebas independientes de cada subsistema antes de la integración final.

Las pruebas previas se encuentran documentadas en la carpeta:

