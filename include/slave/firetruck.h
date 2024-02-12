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
// The constant for the delay when writing an angle to the servo
const int servoDelay = 7;
// Creates the "SteeringServo" object
PWMServo SteeringServo;



// booleans for movement
struct movementBools
{
  volatile bool forward = false;
  volatile bool forwardLeft = false;
  volatile bool forwardRight = false;

  volatile bool backward = false;
  volatile bool backwardRight = false;
  volatile bool backwardLeft = false;
} movementStates;

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
volatile bool newData = false;
volatile bool canReceive = false;


char *firstSound = "3.wav";
char *secondSound = "4.wav";

#define TO_MASTER_SIZE 2
#define TO_SLAVE_SIZE 4
#define NODE_READ_DELAY 100

byte messageToMaster[TO_MASTER_SIZE];
byte nodeReceive[TO_SLAVE_SIZE];

#define SpeedControllerForward SpeedCon.write(165)
#define SpeedControllerStraight SpeedCon.write(90)
#define SpeedControllerBackward SpeedCon.write(17)

// Function definitions

void I2C_RxHandler(int numBytes);

void playSound(char soundFile[12]);

void stopPlayback();

void engageMotors(const char *dir);

void waterPump();

void initSC();

void initShield();

void sendToMaster();

bool checkData(char c);