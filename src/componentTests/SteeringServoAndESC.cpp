
#include <Arduino.h>
#include <PWMServo.h>

PWMServo SteeringServo; // create servo object to control a servo
PWMServo ESC;           // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0; // variable to store the servo position
bool motorsMoving;
unsigned long timeMotorsEngaged;
#define SpeedConForward delay(1500); SteeringServo.write(165); motorsMoving = true; timeMotorsEngaged = millis()
#define SpeedConStop delay(1000); SteeringServo.write(90); motorsMoving = false
#define SpeedConBackward delay(1000); SteeringServo.write(17); motorsMoving = true; timeMotorsEngaged = millis()
void setup()
{
  SteeringServo.attach(10); // attaches the servo on pin 9 to the servo object
  //ESC.attach(10);
  //delay(5000);
  //ESC.write(2600);
  // delay(1000);
  //ESC.write(90);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  //   // Tell the servo to go to a particular angle:
  SpeedConStop;
  delay(10000);
  SpeedConForward;
  SpeedConBackward;
  // SteeringServo.write(0);
  // delay(15);
  // SteeringServo.write(5);
  // delay(15);
  // SteeringServo.write(180);
  // delay(1000);

  // // Sweep from 0 to 180 degrees:
  // for (int angle = 0; angle <= 180; angle += 1) {
  //   SteeringServo.write(angle);
  //   delay(15);
  // }
  // delay(1000);
  // // And back from 180 to 0 degrees:
  // for (int angle = 180; angle >= 0; angle -= 1) {
  //   SteeringServo.write(angle);
  //   delay(15);
  // }
  // delay(1000);
  //ESC.write(17);
  // SteeringServo.write(180);
  // delay(1000); // waits 4s for the servo to reach the position
  // SteeringServo.write(270);
  // //ESC.write(180);
  // delay(1000);    // waits 5s for the servo to reach the position
  // SteeringServo.write(0);
  // //ESC.write(110);
  // //delay(5000);    // waits 5s for the servo to reach the position
  // //ESC.write(180);
  // delay(1000);    // waits 5s for the servo to reach the position
  // //ESC.write(110);
  // SteeringServo.write(90);
  // delay(1000);    

}