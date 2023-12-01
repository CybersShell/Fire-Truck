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
int pos = 0; // variable to store the servo position
bool movement = false;
bool engageMotor = false;

// void initSC();

void setup()
{
  SteeringServo.attach(9);
  SteeringServo.write(125); // tell servo to go to position
  ESC.attach(10);
  delay(5000);
  ESC.write(2600);
  delay(1000);
  ESC.write(90);
  Serial.begin(9600); // Default Baud for comm, it may be different for your Module. Serial.println("The bluetooth gates are open.\n Connect to HC-05 from any other bluetooth device with 1234 as pairing key!.");
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{

  if (Serial.available())
  {
    data = Serial.read();

    if (data == 's')
    {
      engageMotor = true;
      Serial.readBytesUntil('Y', dirY, 2);
      dirYNum = strtod(dirY, nullptr);
      Serial.println(dirYNum);
    }
    if (data == 'd')
    {
      movement = true;

      Serial.readBytesUntil('X', dirX, 2);
      dirXNum = strtod(dirX, nullptr);
      Serial.println(dirXNum);
      data = Serial.read();
    }
    for (int i = 0; i < 3; i++)
    {
      dirY[i] = 0;
      dirX[i] = 0;
    }
  };

  if (movement)
  {
    movement = false;
    // Turn the servo to 0 degrees
    if (dirXNum < 0)
    {

      for (servoAngle = SteeringServo.read(); servoAngle >= 0; servoAngle--)
      {
        SteeringServo.write(servoAngle);
        delay(servoDelay);
        if (BTSerial.available())
          break;
      }
      data = NULL;
    }
    // Turn the servo perpendicular (90 degrees)
    else if (dirXNum == 0)
    {

      servoAngle = SteeringServo.read();

      if (servoAngle >= 0 && servoAngle < 89)
      {
        for (servoAngle = SteeringServo.read(); servoAngle < 90; servoAngle++)
        {
          SteeringServo.write(servoAngle);
          delay(servoDelay);
          if (BTSerial.available())
            break;
        }
      }
      else if (servoAngle > 89)
      {
        for (servoAngle = SteeringServo.read(); servoAngle > 90; servoAngle--)
        {
          SteeringServo.write(servoAngle);
          delay(servoDelay);
          if (BTSerial.available())
            break;
        }
      }
      else
      {
        SteeringServo.write(90);
      }
      data = NULL;
    }
    // Turn the servo to 180 degrees
    else if (dirXNum > 0)
    {

      servoAngle = SteeringServo.read();

      for (servoAngle = SteeringServo.read(); servoAngle <= 180; servoAngle++)
      {
        SteeringServo.write(servoAngle);
        delay(servoDelay);
        if (BTSerial.available())
          break;
      }
      data = NULL;
    }
  }
  if (engageMotor)
  {
    engageMotor = false;
    if (dirYNum < 0)
    {
      ESC.write(17);
    }
    else if (dirYNum == 0)
    {
      ESC.write(90);
    }
    else if (dirYNum > 0)
    {
      ESC.write(165);
    }
  }
}
