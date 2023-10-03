#include <PWMServo.h>
#include <Arduino.h>


PWMServo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int speedControllerPin = 9;
void setup() {
  myservo.attach(speedControllerPin, 700, 3000);  // attaches the servo on pin 9 to the servo object
  myservo.write(1500);
  delay(3000);
}

void loop() {
  
  myservo.write(850);
  delay(3000);

  myservo.write(1500);
  delay(3000);

  myservo.write(2400);
  delay(3000);

  myservo.write(1500);
  delay(3000);
  
}