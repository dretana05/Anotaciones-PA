int PIN_LM = 34; 

void setup() {
  Serial.begin(115200);
  pinMode(PIN_LM, INPUT);
  Serial.println("Iniciando sensor LM35...");
}

void loop() {
  float milivoltios = analogReadMilliVolts(PIN_LM);
  
  float temperatura = milivoltios / 10.0;

  Serial.print("La temperatura es de: ");
  Serial.print(temperatura);
  Serial.println(" °C");
  
  delay(2000);
}