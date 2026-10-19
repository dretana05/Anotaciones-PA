#include <DHT.h>

#define DHTPIN 4      
#define DHTTYPE DHT11 

DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(115200);
    dht.begin();
    Serial.println("Iniciando sensor DHT11...");
}

void loop() {
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();

    if (isnan(temp) || isnan(hum)) {
        Serial.println("Error al leer el sensor DHT11");
        delay(2000);
        return;
    }

    Serial.print("Temperatura: ");
    Serial.print(temp);
    Serial.print(" °C | Humedad: ");
    Serial.print(hum);
    Serial.println(" %");

    delay(2000);
}