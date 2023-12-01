#include <PWMServo.h>
#include <Arduino.h>


PWMServo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int speedControllerPin = 10;
void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  // myservo.write(1500);
  myservo.attach(speedControllerPin);  // attaches the servo on pin 9 to the servo object
  myservo.write(2600);
 
  Serial.begin(9600);
  delay(5000);
  
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
}

void loop() {

  for (int i = 0; i < 200; i++)
  {
    Serial.print("number = ");
    Serial.println(i);
    myservo.write(i);
    delay(1000);
  }

  
}