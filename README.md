## 2. Tabla Consolidada de Mapeo de Pines (ESP32 de 38 Pines)

| Componente                           | Pin del Componente                   | Pin en ESP32 (38 Pines)                                                      | Notas de Montaje                                                                                                |
| ------------------------------------ | ------------------------------------ | ---------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------- |
| **Ultrasónico (HC-SR04)**            | VCC / GND / TRIG / ECHO              | **VIN (5V)** / **GND** / **GPIO 18** / **GPIO 19**                           | TRIG es salida digital. ECHO es entrada digital (usar divisor de voltaje a 3.3V si es posible).                 |
| **Fotoresistencia (LDR)**            | Terminal 1 / Unión LDR + Resistencia | **3.3V** / **GPIO 32** / **GND**                                             | La señal de entrada al pin GPIO 32 sale del punto medio entre la LDR y la resistencia de 330 Ω o 10 kΩ.         |
| **Temperatura (LM35)**               | VCC (Pin 1) / Vout (Pin 2) / GND (3) | **VIN (5V)** / **GPIO 34 o 35** / **GND**                                    | Entrada analógica. Usar `analogReadMilliVolts()`. Entrega 10 mV/°C.                                             |
| **Temp. / Humedad (DHT11)**          | VCC / GND / DATA (OUT)               | **3.3V** / **GND** / **GPIO 4**                                              | E/S Digital (Bus 1-Wire).                                                                                       |
| **Lector RFID (RC522)**              | VCC/GND/RST/MISO/MOSI/SCK/SDA(SS)    | **3.3V** *(¡NUNCA 5V!)* / **GND** / **22** / **19** / **23** / **18** / **5**| Bus SPI Hardware estándar del ESP32.                                                                            |
| **Pantalla OLED (SH1106 / SSD1306)** | VCC / GND / SDA / SCL                | **3.3V o 5V** / **GND** / **GPIO 21** / **GPIO 22**                          | Bus I2C Hardware (SDA = GPIO 21, SCL = GPIO 22). Dirección I2C habitual: `0x3C`.                                |
| **LED RGB (Cátodo Común)**           | Cátodo (Largo) / Rojo / Verde / Azul | **GND** / **GPIO 25** (+ Res. 330 Ω) / **GPIO 26** / **GPIO 27**             | Salidas PWM/Digitales para formar colores de 0 a 255.                                                           |

---

### 1. Fotoresistencia (LDR) — Divisor de Tensión con 10 kΩ

```text
      ESP32 (38 Pines)                     PROTOBOARD / LDR
    +------------------+                 +--------------------+
    |             3.3V |---------------->|  Terminal 1 LDR    |
    |                  |                 |      (  LDR  )     |
    |          GPIO 32 |<================|  Terminal 2 LDR    |
    |                  |        +------->|        +           |
    |                  |        |        |  Resistencia 10kΩ  |
    |              GND |--------+------->|  Terminal 2 Resis. |
    +------------------+                 +--------------------+

  Esquema eléctrico equivalente:
  [ 3.3V ESP32 ] ---> [ LDR ] ---> ( Punto Medio -> GPIO 32 ) ---> [ Resistencia 10 kΩ ] ---> [ GND ESP32 ]

```
### 2. LED RGB Cátodo Común — Con 3 Resistencias de 330 Ω
```text

ESP32 (38 Pines)                     LED RGB (Cátodo Común)
    +------------------+                 +-------------------------+
    |          GPIO 25 |----[ R 330Ω ]-->| Pin Rojo (R)            |
    |          GPIO 26 |----[ R 330Ω ]-->| Pin Verde (G)           |
    |          GPIO 27 |----[ R 330Ω ]-->| Pin Azul (B)            |
    |              GND |---------------->| Cátodo Común (Pata (-) larga) |
    +------------------+                 +-------------------------+

```
### 3. Sensor Ultrasónico (HC-SR04) — Conexión Directa
```text

ESP32 (38 Pines)                     SENSOR HC-SR04
    +------------------+                 +------------------+
    |         VIN (5V) |---------------->| VCC              |
    |              GND |---------------->| GND              |
    |          GPIO 18 |---------------->| TRIG             |
    |          GPIO 19 |<----------------| ECHO (Salida 5V) |
    +------------------+                 +------------------+

```
### 4. Sensor de Temperatura Analógico (LM35)
```text

ESP32 (38 Pines)                     SENSOR LM35 (Vista frontal)
    +------------------+                 +------------------+
    |         VIN (5V) |---------------->| Pin 1: VCC       |
    |      GPIO 34 / 35|<----------------| Pin 2: Vout (10mV/°C)
    |              GND |---------------->| Pin 3: GND       |
    +------------------+                 +------------------+

  Vista física del encapsulado del LM35 (Mirando la cara PLANA con letras):
         +-----------+
         |   LM35    |
         |           |
         +-----------+
           |   |   |
           1   2   3
           |   |   |
          VCC Vout GND
         (5V)(GPIO 34)

```
### 5. Sensor de Temperatura y Humedad (DHT11)
```text

Caso A: Módulo PCB de 3 Pines (El más común en el laboratorio)
      ESP32 (38 Pines)                     MÓDULO DHT11 PCB
    +------------------+                 +------------------+
    |             3.3V |---------------->| VCC / (+)        |
    |              GND |---------------->| GND / (-)        |
    |           GPIO 4 |<===============>| DATA / OUT / S   |
    +------------------+                 +------------------+

```
### 6. Lector RFID (RC522) — Comunicación SPI
```text

ESP32 (38 Pines)                     MÓDULO RFID RC522
    +------------------+                 +------------------+
    |             3.3V |---------------->| 3.3V (¡NUNCA 5V!)|
    |              GND |---------------->| GND              |
    |          GPIO 22 |---------------->| RST              |
    |          GPIO 19 |<----------------| MISO             |
    |          GPIO 23 |---------------->| MOSI             |
    |          GPIO 18 |---------------->| SCK              |
    |           GPIO 5 |---------------->| SDA / SS         |
    +------------------+                 +------------------+
```

| Sensor | Magnitud | Widget / Bloque Recomendado | ¿Por qué se utiliza este bloque? |
| :--- | :--- | :--- | :--- |
| Ultrasónico (HC-SR04) | Distancia en cm | Gauge (Medidor circular) o Line Chart | En el Laboratorio 5 se especifica usar un **Gauge**. Es un velocímetro/reloj circular ideal para ver distancias instantáneas configurando sus límites (por ejemplo, `0` a `100 cm`). |
| Fotorresistencia (LDR) | Nivel de Luz (0% a 100%) | Gauge o Stream / Value | Al ser un porcentaje relativo de iluminación, un **Gauge** graduado de `0` a `100%` muestra visualmente si el ambiente está oscuro o iluminado. |
| Temp. y Humedad (DHT11 / LM35) | °C y % de Humedad | Line Chart (Gráfica de líneas) o Gauge | El **Line Chart** permite graficar el comportamiento histórico y las variaciones de temperatura o humedad a lo largo del tiempo. |
| Sensor Digital / Presencia (PIR / Botón) | Estado lógico (`1` / `0`) | Indicator (Indicador numérico/color) | Muestra una luz tipo "faro" que se enciende en verde o rojo dependiendo de si detecta presencia o no. |
