
#include <Arduino.h>
#include <PWMServo.h>

PWMServo SteeringServo; // create servo object to control a servo
PWMServo ESC;           // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0; // variable to store the servo position

void setup()
{
  SteeringServo.attach(9); // attaches the servo on pin 9 to the servo object
  //ESC.attach(10);
  //delay(5000);
  //ESC.write(2600);
  delay(1000);
  //ESC.write(90);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  //ESC.write(17);
  SteeringServo.write(180);
  delay(1000); // waits 4s for the servo to reach the position
  SteeringServo.write(270);
  //ESC.write(180);
  delay(1000);    // waits 5s for the servo to reach the position
  SteeringServo.write(0);
  //ESC.write(110);
  //delay(5000);    // waits 5s for the servo to reach the position
  //ESC.write(180);
  delay(1000);    // waits 5s for the servo to reach the position
  //ESC.write(110);
  SteeringServo.write(90);
  delay(1000);    

}