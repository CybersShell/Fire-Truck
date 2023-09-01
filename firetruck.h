// Library used for serial communication on other digital pins of the Arduino
#include <SoftwareSerial.h>

// Libraries necessary for Audio playback
#include <WaveUtil.h>
#include <WaveHC.h>

// Library used to control the servo
#include <PWMServo.h>

// Library that Andrew Woodlee modified for reading battery level using the relay
#include "Battery.h"

// Creates the bluetooth object with the reciever and transmitter pins as the arguments
const int btRxPin = A4;
const int btTxPin = A3;
SoftwareSerial BTSerial(btRxPin, btTxPin);

// Macro to determine if character is avaliable to be read over Bluetooth
#define CHAR_AVAILABLE BTSerial.available()

/*
  Motor configuration
*/

// The constants used for controlling the enabling pins
const int enA_B = 5;

// The constants that will be used with both DC motors
const int in1_3 = 6;
const int in2_4 = 7;


/*
  Servo configuration
*/

// The constants used for what pin and angle the Servo will be on
const int servoPin = 8;
int servoAngle;
// The constant for the delay when writing an angle to the servo
const int servoDelay = 7;
// Creates the "Servo1" object
PWMServo Servo1;

/*
  Constants used for SPI Bus Ports
*/

const int speakerPin = 9;

// Variable used for recieving and transmitting data
char data;

// Constant used for baud rate
const int baud = 9600;

// Constant used for the water pump port
// const int water = A5;

// // Variables for events
unsigned long currentTime;
unsigned long timeMotorsEngaged;
boolean motorsMoving = false;

unsigned long timeSoundStarted;
unsigned long timeToStopPlayingSound;

// Battery sensing setup and constants
#define SENSE_PIN A0
#define ACTIVATION_PIN A1
// This board's reference voltage in milivolts - CHANGE FOR EVERY BOARD, details in project manual
// Battery class initialized with values for sensing battery level
// Parameters: min voltage, max voltage, sense pin
#define BOARD_REF_VOLTAGE 5140
Battery batt = Battery(10000, 12000, SENSE_PIN);

// SD card and sound objects

SdReader card;    // This object holds the information for the card
FatVolume vol;    // This holds the information for the partition on the card
FatReader root;   // This holds the information for the filesystem on the card
WaveHC wave;      // This is the only wave (audio) object, since we will only play one at a time
FatReader file;
