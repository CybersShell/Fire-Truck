#include <Arduino.h>
#include <SoftwareSerial.h>
#include <PWMServo.h>

SoftwareSerial BTSerial(A4, A5); // RX | TX

char data;

// char arrays for data
char dirX[3];
char dirY[3];
double dirXNum;
double dirYNum;
int servoAngle;
int servoDelay = 7;
PWMServo SteeringServo;
PWMServo ESC;
int pos = 0;    // variable to store the servo position
bool movement = false;

// void initSC();

void setup()
{
    SteeringServo.attach(9);
    SteeringServo.write(125);              // tell servo to go to position in variable 'pos'
    ESC.write(60);
    ESC.attach(10);
    Serial.begin(9600); //Default Baud for comm, it may be different for your Module. Serial.println("The bluetooth gates are open.\n Connect to HC-05 from any other bluetooth device with 1234 as pairing key!.");
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {




    ESC.write(65);
  // for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
  //   // in steps of 1 degree
  //   Serial.println(pos);
  //   SteeringServo.write(pos);              // tell servo to go to position in variable 'pos'
  //   delay(servoDelay);                       // waits 15ms for the servo to reach the position
  // }
  // for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
  //   Serial.println(pos);
  //   SteeringServo.write(pos);              // tell servo to go to position in variable 'pos'
  //   delay(servoDelay);                       // waits 15ms for the servo to reach the position
  // }

    // Turn on the LED if a character is available. 
    if (Serial.available()){
        data = Serial.read();

        if (data == 's') {
            movement = true;

            Serial.readBytesUntil('X', dirX, 2);
            dirXNum = strtod(dirX, nullptr);
            Serial.println(dirX);
            Serial.println(dirXNum);
            data = Serial.read();
            if (data == 's') {
              Serial.readBytesUntil('Y', dirY, 2);
              dirYNum = strtod(dirY, nullptr);
              Serial.println(dirYNum);
            }
            for (int i = 0; i < 3; i++)
          {
              dirY[i] = 0;
              dirX[i] = 0;
            }
            
        }
    };

    if (movement) {
      movement = false;
      // Turn the servo to 0 degrees
      if (dirXNum < 0) {
        
        servoAngle = SteeringServo.read();

        for (servoAngle = SteeringServo.read(); servoAngle >= 0; servoAngle--)
        {
          SteeringServo.write(servoAngle);
          delay(servoDelay);
          if (BTSerial.available()) break;
        }
        data = NULL;
      }
      // Turn the servo perpendicular (90 degrees)
      if (dirXNum == 0) {
        
        servoAngle = SteeringServo.read();
        
        if (servoAngle >= 0 && servoAngle < 89)
        {
          for (servoAngle = SteeringServo.read(); servoAngle < 90; servoAngle++)
          {
            SteeringServo.write(servoAngle);
            delay(servoDelay);
            if (BTSerial.available()) break;
          }
        } else if (servoAngle > 89)
        {
          for (servoAngle = SteeringServo.read(); servoAngle > 90; servoAngle--)
          {
            SteeringServo.write(servoAngle);
            delay(servoDelay);
            if (BTSerial.available()) break;
          }
        } else {
          SteeringServo.write(90);
        }
        data = NULL;
      }
      // Turn the servo to 180 degrees
      if (dirXNum > 0) {
        
        servoAngle = SteeringServo.read();
        
        for (servoAngle = SteeringServo.read(); servoAngle <= 180; servoAngle++)
        {
          SteeringServo.write(servoAngle);
          delay(servoDelay);
          if (BTSerial.available()) break;
        }
        data = NULL;
      }

      if (dirYNum < 0)
      {
        ESC.write(17);
      } else if (dirYNum == 0) {
        ESC.write(65);
      } else if (dirYNum > 0) {
        ESC.write(165);
      }
    }
    

}
