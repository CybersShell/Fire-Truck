// Library used for serial communication on other digital pins of the Arduino
#include <SoftwareSerial.h>

// Libraries necessary for Audio playback
#include <WaveUtil.h>
#include <WaveHC.h>

// Library used to control the servo
#include <PWMServo.h>

// Library for I2C interface
#include <Wire.h>


// Creates the bluetooth object with the receiver and transmitter pins as the arguments
const int btRxPin = A1;
const int btTxPin = A2;
SoftwareSerial BTSerial(btRxPin, btTxPin);

// Macro to determine if character is available to be read over Bluetooth
#define CHAR_AVAILABLE BTSerial.available()

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
// The constant for the delay when writing an angle to the servo
const int servoDelay = 7;
// Creates the "SteeringServo" object
PWMServo SteeringServo;



// char arrays for data
char dirX[3];
char dirY[3];
double dirXNum;
double dirYNum;
bool movement = false;
bool engageMotor = false;
// Constant used for baud rate
const int baud = 9600;

// Constant used for the water pump pin
const int waterPumpPin = A3;
bool waterPumpState = false;

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
char data;
bool newData = false;


char const * firstSound = "3.wav";
char const * secondSound = "4.wav";


// Function definitions

void I2C_RxHandler(int numBytes);

void playSound(char soundFile[12]);

void engageMotors(const char *dir);

void waterPump();

void initSC();

void initShield();