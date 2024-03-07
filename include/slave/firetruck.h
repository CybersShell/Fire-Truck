// Library used for serial communication on other digital pins of the Arduino
#include <SoftwareSerial.h>

// Libraries necessary for Audio playback
#include <WaveUtil.h>
#include <WaveHC.h>

// Library used to control the servo
#include <PWMServo.h>

// Library for I2C interface
#include <Wire.h>

// common headers shared between master and slave
#include <common.h>


/*
  Motor configuration
*/

// Speed controller pin
const int speedControllerPin = 10;

// The Speed Controller PWMServo object that controls the Speed Controller
PWMServo SpeedCon;


/*
  Servo configuration
*/

// The constants used for what pin and angle the Servo will be on
const int servoPin = 9;
int servoAngle;
// Creates the "SteeringServo" object
PWMServo SteeringServo;



volatile char movementChar;
int escValue = 90;
volatile bool movement = false;
volatile bool engageMotor = false;
// Constant used for baud rate
const int baud = 9600;

// Constant used for the water pump pin
const int waterPumpPin = A3;
bool waterPumpEnabled = false;

// // Variables for events
unsigned long currentTime;
unsigned long timeMotorsEngaged;
boolean motorsMoving = false;

unsigned long timeSoundStarted;
unsigned long timeToStopPlayingSound;


// SD card and sound objects

SdReader card;    // This object holds the information for the card
FatVolume vol;    // This holds the information for the partition on the card
FatReader root;   // This holds the information for the filesystem on the card
WaveHC wave;      // This is the only wave (audio) object, since we will only play one at a time
FatReader file;

// data variables used for receiving and transmitting data
volatile char data;
volatile char motorControl, servoControl;
volatile bool newData = false;
volatile bool canReceive = false;


char *firstSound = "3.wav";
char *secondSound = "4.wav";

// join the I2C bus with address 8
// call I2C_RxHandler when data is received
// inlined to prevent extra functions calls
#define initI2C Wire.begin(8); Wire.onReceive(I2C_RxHandler);

/*  Begin movement macros
    The following steps prevent damaging the motors due to polarity reversal:
      when motor is backward, still the motor, and then move backward
      when motor is forward, still the motor, and then move forward
*/
#define SpeedConForward  motorsMoving = true; timeMotorsEngaged = millis()
#define SpeedConStop  motorsMoving = false
#define SpeedConBackward  motorsMoving = true; timeMotorsEngaged = millis()

#define ftTurnLeft SteeringServo.write(0)
#define ftTurnRight SteeringServo.write(180)
#define ftStraight SteeringServo.write(90)

// End movement macros

// Function definitions

void I2C_RxHandler(int numBytes);

void playSound(char soundFile[12]);

void stopPlayback();

void engageMotors(const char *dir);

void waterPump();

void initSC();

void initShield();

bool checkData(char c);

bool isDataMovementChar (char controlData);
