// Library used for serial communication on other digital pins of the Arduino
#include <SoftwareSerial.h>

// Libraries necessary for Audio playback
#include <WaveUtil.h>
#include <WaveHC.h>

// Library used to control the servo
#include <PWMServo.h>

// Library for connecting to the SD card adapter
#include <SPI.h>

// Library that Andrew Woodlee modified for reading battery level using the relay
#include "Battery.h"

// Creates the bluetooth object with the reciever and transmitter pins as the arguments
const int btRxPin = A3;
const int btTxPin = A4;
SoftwareSerial BTSerial(btRxPin, btTxPin);

// Macro to determine if character is avaliable to be read over Bluetooth
#define CHAR_AVAILABLE BTSerial.available()
#define CHAR_AVAILABLE_BREAK if (BTSerial.available()) break

/*
  Motor configuration
*/

// The constants that will be used with both DC motors
const int in1_3 = 7;
const int in2_4 = 6;


// The constants used for controlling the enabling pins
const int enA_B = 5; 


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

const int CSpin = SS;
const int SCKpin = SCK;
const int MOSIpin = MOSI;
const int MISOpin = MISO;

const int speakerPin = 9;
const int SDpin = 10;

// Variable used for recieving and transmitting data
char data;

// Constant used for baud rate
const int baud = 9600;

// Constant used for the water pump port
const int water = A5;

// Variables for events
const unsigned long timeToStopMotor = 1000 * 30; // 30 seconds
unsigned long currentTime;
unsigned long timeMotorsEngaged;
boolean motorsMoving = false;

// Battery sensing setup and constants
#define SENSE_PIN A0
#define ACTIVATION_PIN A1
#define MAX_VOLTAGE 12000
#define MIN_VOLTAGE 10000
// This board's reference voltage in milivolts - CHANGE FOR EVERY BOARD, details in project manual
#define BOARD_REF_VOLTAGE 5140
// Battery class initialized with values for sensing battery level
Battery batt = Battery(MIN_VOLTAGE, MAX_VOLTAGE, SENSE_PIN);

// SD card and sound objects

SdReader card;    // This object holds the information for the card
FatVolume vol;    // This holds the information for the partition on the card
FatReader root;   // This holds the information for the filesystem on the card
WaveHC wave;      // This is the only wave (audio) object, since we will only play one at a time
