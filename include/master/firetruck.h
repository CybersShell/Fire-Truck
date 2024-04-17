#include <Arduino.h>
#include <Wire.h>
#include <usbhub.h>
#include <SPI.h>
#include <common.h>
#include <limits.h>
#include <PS4Parser.h> // Added this header to the file
#include <PS4BT.h>
#include <Adafruit_PWMServoDriver.h>

// These defined macros keep track of if the left stick moves - ctm 
#define motorStickForwardBoundCheck GameController.getAnalogHat(LeftHatY) < 70
#define motorStickBackwardBoundCheck GameController.getAnalogHat(LeftHatY) > 220
#define leftNeutralConditional GameController.getAnalogHat(LeftHatY) > 70 && GameController.getAnalogHat(LeftHatY) < 220

// These defined macros keep track of if the right stick moves - ctm 
#define leftConditional GameController.getAnalogHat(RightHatX) < 70
#define rightConditional GameController.getAnalogHat(RightHatX) > 220
#define rightNeutralConditional GameController.getAnalogHat(RightHatX) > 70 && GameController.getAnalogHat(RightHatX) < 220


USB Usb;
BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so

/* You can create the instance of the PS4BT class in two ways */
// This will start an inquiry and then pair with the PS4 controller - you only have to do this once
// You will need to hold down the PS and Share button at the same time, the PS4 controller will then start to blink rapidly indicating that it is in pairing mode
// create PS4 controller Bluetooth class, enter pairing mode
PS4BT GameController(&Btd, PAIR);


const int I2CAddress = 8; // I2C bus address


// Pins for the ESC and Servo
// NOTE: These pins correspond to the AdaFruit PWM servo control shield
const int speedControllerPin = 7;
const int servoPin = 9;


// The angle degree change rate for both the servo and the ESC
const int motorAngleChange = 5;
const int steeringAngleChange = 5;
int servoAngle;

// PWM Module configuration
// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
// ! if you change the address on the servo board, use this
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);
// you can also call it with a different address and I2C interface
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);

// Depending on your servo make, the pulse width min and max may vary, you 
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMIN  1000 // This is the microseconds min
#define SERVOMAX  2000 // This is the microseconds max
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

// Constant used for the water pump pin
const int waterPumpPin = A3;
bool waterPumpEnabled = false;

// Variables for events
boolean motorsMoving = false;


// motor turns every 1500 us
unsigned long motorPeriod = 1500;
// Servo turns every 1500 us
unsigned long servoPeriod = 1500;

// increase the motor
#define MotorForwardAngleCheck truckMovementAngles.motor <= 335
// decrease the motor
#define MotorBackwardAngleCheck truckMovementAngles.motor >= 235
// motor stopping point
#define MotorStopPoint 265

// Begin structs
typedef struct
{
  unsigned long current;
  // will be set in the motor control statements
  unsigned long motorsEngaged;
  // will be set in servo control statements
  unsigned long servoEngaged;
} timeVariables;

// create instance of timeVariables struct
timeVariables truckControlTimes;

typedef struct {
  int servo;
  int motor;
} movementAngles;

movementAngles truckMovementAngles;

// end structs

// Function definitions

void sendData(char data);

void getState();

void setMotorState();

void setSteeringServoState(); 

void combineStates();

void SetUpPWMModule();