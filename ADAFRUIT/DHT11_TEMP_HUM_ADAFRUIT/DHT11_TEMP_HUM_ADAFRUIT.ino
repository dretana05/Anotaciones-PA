#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <DHT.h>

// ---------------------- 1. CREDENCIALES WI-FI Y ADAFRUIT IO ----------------------
#define WLAN_SSID "escribir_wifi"
#define WLAN_PASS "escribir_contraseña_wifi"
#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883
#define AIO_USERNAME "escribir_usuario"
#define AIO_KEY "escribir_llave" 

// ---------------------- PINES Y SENSOR DHT11 ----------------------
#define DHTPIN 4       
#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);

// ---------------------- 2. CLIENTE MQTT Y FEEDS ----------------------
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Feeds organizados
Adafruit_MQTT_Publish feedTemp    = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/sensores.temperatura");
Adafruit_MQTT_Publish feedHumedad = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/sensores.humedad");

unsigned long ultimoPing = 0;
unsigned long ultimaPublicacion = 0;

void conectarWiFi();
void conectarMQTT();

void setup() {
    Serial.begin(115200);
    
    dht.begin();
    
    conectarWiFi();
} 

void loop() {
    conectarMQTT();

    // Mantener la conexión MQTT viva
    if (millis() - ultimoPing >= 30000) {
        ultimoPing = millis();
        mqtt.ping();
    }

    // Se publican los datos cada 5 segundos
    if (millis() - ultimaPublicacion >= 5000) {
        ultimaPublicacion = millis();
        
        // --- LECTURA DEL DHT11 ---
        float temperatura = dht.readTemperature(); 
        float humedad = dht.readHumidity();

        if (isnan(temperatura) || isnan(humedad)) {
            Serial.println("¡Error al leer del sensor DHT11!");
            return;
        }

        Serial.print("Temperatura: "); Serial.print(temperatura); Serial.println(" °C");
        Serial.print("Humedad: "); Serial.print(humedad); Serial.println(" %");

        // --- ENVIAR A ADAFRUIT IO ---
        bool envTemp = feedTemp.publish((double)temperatura);
        bool envHum = feedHumedad.publish((double)humedad);

        if (envTemp && envHum) {
            Serial.println("¡Temperatura y Humedad publicadas exitosamente!\n");
        } else {
            Serial.println("Fallo al publicar en Adafruit IO\n");
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
    if (mqtt.connected()) return;

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