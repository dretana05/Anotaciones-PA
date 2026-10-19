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

// ---------------------- PINES DEL SENSOR ----------------------
#define TRIG_PIN 18 
#define ECHO_PIN 19 

// ---------------------- 2. CLIENTE MQTT Y FEED ----------------------
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Feed
Adafruit_MQTT_Publish feedDistancia = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/sensores.distancia");

unsigned long ultimoPing = 0;
unsigned long ultimaPublicacion = 0;

// PROTOTIPOS DE FUNCIÓN
void conectarWiFi();
void conectarMQTT();

void setup() {
    Serial.begin(115200);
    
    pinMode(TRIG_PIN, OUTPUT); 
    pinMode(ECHO_PIN, INPUT); 
    
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

    // Se publican los datos cada 5 segundos para no saturar la API
    if (millis() - ultimaPublicacion >= 5000) {
        ultimaPublicacion = millis();
        
        digitalWrite(TRIG_PIN, LOW); 
        delayMicroseconds(2); 
        digitalWrite(TRIG_PIN, HIGH); 
        delayMicroseconds(10); 
        digitalWrite(TRIG_PIN, LOW); 
        
        long duracion = pulseIn(ECHO_PIN, HIGH); 
        float distancia = (duracion * 0.0343) / 2.0; 
        
        Serial.print("Distancia: "); 
        Serial.print(distancia); 
        Serial.println(" cm"); 

        if (!feedDistancia.publish((int32_t)distancia)) {
            Serial.println("Fallo al publicar la distancia en Adafruit IO");
        } else {
            Serial.println("¡Distancia publicada exitosamente!");
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

    while ((ret = mqtt.connect()) != 0) {
        Serial.println(mqtt.connectErrorString(ret));
        Serial.println("Reintentando en 5 segundos...");
        mqtt.disconnect();
        delay(5000);
        if (--intentos == 0) {
            Serial.println("Fallo de conexión. Reiniciando ESP32...");
            ESP.restart(); 
        }
    }
    Serial.println(" ¡Conectado exitosamente con Adafruit IO!");
}