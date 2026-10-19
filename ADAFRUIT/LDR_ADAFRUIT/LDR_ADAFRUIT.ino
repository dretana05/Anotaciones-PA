#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// ---------------------- 1. CREDENCIALES WI-FI Y ADAFRUIT IO ----------------------
#define WLAN_SSID "escribir_wifi"
#define WLAN_PASS "escribir_contraseña_wifi"
#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883
#define AIO_USERNAME "escribir_usuario"
#define AIO_KEY "escribir_llave" 

// ---------------------- PIN DEL SENSOR ----------------------
#define LDR_PIN 32

// ---------------------- 2. CLIENTE MQTT Y FEED ----------------------
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

//  Feed 
Adafruit_MQTT_Publish feedLuz = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/sensores.luz");

unsigned long ultimoPing = 0;
unsigned long ultimaPublicacion = 0;

// PROTOTIPOS DE FUNCIÓN
void conectarWiFi();
void conectarMQTT();

void setup() {
    Serial.begin(115200);
    pinMode(LDR_PIN, INPUT);
    conectarWiFi();
} 

void loop() {
    // Asegura que la sesión siga activa con Adafruit IO
    conectarMQTT();

    // Mantener vivo el canal MQTT (Ping cada 30 segundos)
    if (millis() - ultimoPing >= 30000) {
        ultimoPing = millis();
        mqtt.ping();
    }

    // Publicar los datos cada 5 segundos para no saturar la API de Adafruit
    if (millis() - ultimaPublicacion >= 5000) {
        ultimaPublicacion = millis();
        
        int lecturaADC = analogRead(LDR_PIN);  
        int porcentajeLuz = (lecturaADC * 100) / 4095; 

        Serial.print("ADC: ");                          
        Serial.print(lecturaADC);                       
        Serial.print(" | Luz: ");                       
        Serial.print(porcentajeLuz);                    
        Serial.println("%");  

        if (!feedLuz.publish((int32_t)porcentajeLuz)) {
            Serial.println("Fallo al publicar en Adafruit IO");
        } else {
            Serial.println("¡Luz publicada exitosamente en Adafruit!");
        }
    }
}

// ---------------------- FUNCIONES DE CONEXIÓN ----------------------
void conectarWiFi() {
    Serial.print("Conectando a Wi-Fi...");
    WiFi.begin(WLAN_SSID, WLAN_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWi-Fi Conectado. IP: " + WiFi.localIP().toString());
}

void conectarMQTT() {
    if (mqtt.connected()) return; // Si ya está conectado, no hace nada

    Serial.print("Conectando con Adafruit IO...");
    int8_t ret;
    uint8_t intentos = 3;

    // Intenta conectar y maneja los errores
    while ((ret = mqtt.connect()) != 0) {
        Serial.println(mqtt.connectErrorString(ret));
        Serial.println("Reintentando en 5 segundos...");
        mqtt.disconnect();
        delay(5000);
        if (--intentos == 0) {
            Serial.println("Fallo de conexión. Reiniciando ESP32...");
            ESP.restart(); // Reinicia la placa si no logra conectar
        }
    }
    Serial.println(" ¡Conectado exitosamente con Adafruit IO!");
}