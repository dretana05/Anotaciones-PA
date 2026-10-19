#include <ESP32Servo.h> 

int pos = 0;
Servo servo;
int servoPin = 13; 

void setup() {

  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  
  servo.setPeriodHertz(50);
  servo.attach(servoPin, 500, 2500);
}

void loop() {
  for (pos = 0; pos <= 180; pos += 1) {
    servo.write(pos);
    delay(15);
  }
  
  for (pos = 180; pos >= 0; pos -= 1) {
    servo.write(pos);
    delay(15);
  }
}