#include <Arduino.h>
#include <Wire.h>
#include <usbhub.h>
#include <SPI.h>
#include <common.h>
#include <PS4Parser.h> // Added this header to the file
#include <PS4BT.h>

// These defined macros keep track of if the left stick moves - ctm 
#define upConditional GameController.getAnalogHat(LeftHatY) < 70
#define downConditional GameController.getAnalogHat(LeftHatY) > 220
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

bool printAngle, printTouch;
uint8_t oldL2Value, oldR2Value;

#define LED_PIN 9

const int I2CAddress = 8; // I2C bus address

char dummyData = 'x';
volatile bool sendMovementData = false;

// Set the states of the left stick - ctm 
enum leftStickStates {leftStickUp, leftStickDown, leftStickNeutral}; 

// Set the struct of the left stick state to have an old and new value - ctm 
struct LeftStickState 
{
    leftStickStates newState;
    leftStickStates oldState; 
} truckMotorEscControlStick;

// Set the states of the right stick - ctm 
enum servoControlStickStates {rightStickRight, rightStickLeft, rightStickNeutral};

// Set the struct of the right stick state to have an old and new value - ctm 
struct RightStickState {
    servoControlStickStates newState; 
    servoControlStickStates oldState; 
} truckSteeringServoControlStick; 

// Set the states of the firetruck - ctm 
enum fireTruckStates
{
    still,
    stillToForward,
    stillToBackward,
    forwardToStill,
    backwardToStill,
    forward,
    backward,
    forwardToRight,
    forwardToLeft,
    forwardToBackward,
    backwardToRight,
    backwardToLeft,
    backwardToForward,
    straight, 
    right,
    left
};

#include <Servo.h>


/*
  Motor configuration
*/

// Speed controller pin
const int speedControllerPin = 2;
const int motorAngleChange = 2;
const int steeringAngleChange = 2;

// The Speed Controller PWMServo object that controls the Speed Controller
Servo SpeedCon;

/*
  Servo configuration
*/

// The constants used for what pin and angle the Servo will be on
const int servoPin = 3;
int servoAngle;
// Creates the "SteeringServo" object
Servo SteeringServo;


// Constant used for the water pump pin
const int waterPumpPin = A3;
bool waterPumpEnabled = false;

// Variables for events
unsigned long currentTime;
unsigned long timeMotorsEngaged;
boolean motorsMoving = false;


// Servo turns every 20 ms
unsigned long motorPeriod = 20;
// Servo turns every 20 ms
unsigned long servoPeriod = 20;

// increase the motor
#define MotorForwardAngleCheck truckMovementAngles.motor <= 110
// decrease the motor
#define MotorBackwardAngleCheck truckMovementAngles.motor >= 60

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

void truckMovement();



// Set the struct of the firetruck state to have an old and new value - ctm
struct fireTruckState
{
    fireTruckStates newState;
    fireTruckStates oldState; 
} firetruck; 


void sendData(char data, char secondMovementChar);

void getState();

void setMotorState();

void setSteeringServoState(); 

void combineStates(); 

void fireTruckControl(); 