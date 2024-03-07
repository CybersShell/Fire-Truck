
#include <Arduino.h>
#include <PWMServo.h>

PWMServo SteeringServo; // create servo object to control a servo
PWMServo ESC;           // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0; // variable to store the servo position
bool motorsMoving;
unsigned long timeMotorsEngaged;
#define SpeedConForward Serial.println("SpeedConForward"); motorsMoving = true; timeMotorsEngaged = millis()
#define SpeedConStop Serial.println("SpeedConStop"); SteeringServo.write(90); motorsMoving = false
#define SpeedConBackward Serial.println("SpeedConBackward"); motorsMoving = true; timeMotorsEngaged = millis()
void setup()
{
  Serial.begin(9600);
  SteeringServo.attach(10); // attaches the servo on pin 9 to the servo object
  //ESC.attach(10);
  //delay(TurnDelay);
  //ESC.write(2600);
  // delay(1000);
  //ESC.write(90);
  pinMode(LED_BUILTIN, OUTPUT);
}
const int TurnDelay = 2000;
void loop()
{
  // Tell the servo to go to a particular angle:
  /*SpeedConStop;
  delay(TurnDelay);
  SpeedConForward;
  delay(TurnDelay);
  SpeedConStop;
  delay(TurnDelay);
  SpeedConBackward;
  delay(TurnDelay); */

  // Sweep from 0 to 180 degrees:
  // for (int angle = 0; angle <= 270; angle += 1) {
  //   Serial.println(angle);
  //   SteeringServo.write(angle);
  //   delay(20);
  // }
  // delay(TurnDelay);
  // // And back from 180 to 0 degrees:
  // for (int angle = 270; angle >= 0; angle -= 1) {
  //   Serial.println(angle);
  //   SteeringServo.write(angle);
  //   delay(20);
  // }

  for(int i = 0; i < 90; i++)
  {
    SteeringServo.write(i);
    delay(20);
  }

  for(int i = 90; i > 0; i--)
  {
    SteeringServo.write(i);
    delay(20);
  }

  delay(1000);

  for(int i = 90; i < 180; i++)
  {
    SteeringServo.write(i);
    delay(20);
  }

  delay(1000);

  for(int i = 180; i > 90; i--)
  {
    SteeringServo.write(i);
    delay(20);
  }

  delay(1000);






  // delay(TurnDelay);
  // SpeedConStop;
  // delay(TurnDelay);
  // SpeedConStop;
  // delay(TurnDelay);
  // SpeedConForward;
  // delay(TurnDelay);
  // SpeedConForward;
  // delay(TurnDelay);
  // SpeedConBackward;
  // delay(TurnDelay);
  // SpeedConBackward;
  // delay(TurnDelay);
  // delay(1000);
  //ESC.write(17);
  // SteeringServo.write(180);
  // delay(1000); // waits 4s for the servo to reach the position
  // SteeringServo.write(270);
  // //ESC.write(180);
  // delay(1000);    // waits 5s for the servo to reach the position
  // SteeringServo.write(0);
  // //ESC.write(110);
  // //delay(TurnDelay);    // waits 5s for the servo to reach the position
  // //ESC.write(180);
  // delay(1000);    // waits 5s for the servo to reach the position
  // //ESC.write(110);
  // SteeringServo.write(90);
  // delay(1000);    

}