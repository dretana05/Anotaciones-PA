int PIN_LM = 34; 
int PIN_ROJO = 25;
int PIN_VERDE = 26;
int PIN_AZUL = 27;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_LM, INPUT);
  pinMode(PIN_ROJO, OUTPUT);
  pinMode(PIN_VERDE, OUTPUT);
  pinMode(PIN_AZUL, OUTPUT);
}

void alarma(float res){
  digitalWrite(PIN_ROJO, LOW);
  digitalWrite(PIN_VERDE, LOW);
  digitalWrite(PIN_AZUL, LOW);

  if(res < 30){
    digitalWrite(PIN_ROJO, HIGH);
    delay(1000);
  }else if(res >= 30 && res <= 31){
    digitalWrite(PIN_VERDE, HIGH);
    delay(1000);
  }else{
    digitalWrite(PIN_AZUL, HIGH);
    delay(1000);
  }
}

void loop() {
  float numerador = analogReadMilliVolts(PIN_LM);
  float resultado = numerador / 10.0;

  alarma(resultado);
  Serial.print("La temperatura es de: ");
  Serial.println(resultado);
  
  delay(2000);
}