// Library that may be used later (Color Sensor)
#include <Wire.h>

// Library used for serial communication on other digital pins of the Arduino
#include <SoftwareSerial.h>

// Library used to control the servo
#include <Servo.h>

// Library for connecting to the SD card adapter
#include <SPI.h>

// Libraries necessary for Audio playback
#include <TMRpcm.h>
#include <SD.h>

// Library that Andrew Woodlee modified for reading battery level using the relay
#include "Battery.h"

// Creates the bluetooth object with the reciever and transmitter pins as the arguments
const int btRxPin = A3;
const int btTxPin = A4;
SoftwareSerial BTSerial(btRxPin, btTxPin);
#define CHAR_AVAILABLE BTSerial.available()

/*
  Motor configuration
*/

// The constants that will be used with both DC motors
const int in1 = 2;
const int in2 = 4;
const int in3 = 6;
const int in4 = 7;

// The constants used for controlling the enabling pins
const int enA = 3; 
const int enB = 5;

/*
  Servo configuration
*/

// The constants used for what pin and angle the Servo will be on
const int servoPin = 8;
int servoAngle;
// The constant for the delay when writing an angle to the servo
const int servoDelay = 7;
// Creates the "Servo1" object
Servo Servo1;

/*
  Constants used for SPI Bus Ports
*/

const int CSpin = 10;
const int SCKpin = 13;
const int MOSIpin = 11;
const int MISOpin = 12;

// Create the SD card object that will play the sounds
TMRpcm audio;

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
// This board's reference voltage in milivolts - CHANGE FOR EVERY BOARD
#define BOARD_REF_VOLTAGE 5140
Battery batt = Battery(MIN_VOLTAGE, MAX_VOLTAGE, SENSE_PIN);
