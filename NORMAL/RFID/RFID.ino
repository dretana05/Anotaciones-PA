#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN  5   // Pin SDA
#define RST_PIN 22  // Pin RST

MFRC522 rfid(SS_PIN, RST_PIN); 

byte tarjetaAutorizada[4] = {0xA2, 0x25, 0xE6, 0x1C}; // Colocar UID de la tarjeta autorizada

void setup() {
  Serial.begin(9600);    
  SPI.begin();          
  rfid.PCD_Init();      
  
  Serial.println("Lector RFID listo.");
  Serial.println("Acerca la tarjeta o el llavero...");
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent()) {
    return;
  }
  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }

  Serial.print("UID leido: ");
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(rfid.uid.uidByte[i], HEX);
  }
  Serial.println();

  bool acceso = true;
  for (byte i = 0; i < 4; i++) {
    if (rfid.uid.uidByte[i] != tarjetaAutorizada[i]) {
      acceso = false;
      break;
    }
  }

  if (acceso) {
    Serial.println("ACCESO PERMITIDO - Tarjeta Blanca");
  } else {
    Serial.println("ACCESO DENEGADO - Llavero u otra tarjeta");
  }

  rfid.PICC_HaltA();
}