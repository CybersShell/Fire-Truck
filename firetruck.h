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

// Library for reading internal voltage
#include <CPUVolt.h>


// Creates the bluetooth object with the reciever and transmitter pins as the arguments
const int btRxPin = A4;
const int btTxPin = A3;
SoftwareSerial BTSerial(btRxPin, btTxPin);


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

// The constant used for what pin the Servo will be on
const int servoPin = 8;

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

// Variables used for recieving and transmitting data
String inData = "";
char appData;
String data;
boolean NL = true;

// Constant used for baud rate
const int baud = 9600;

// Constant used for the water pump port
const int water = A5;

// For reading voltage
double volts;

// Variables for events
const unsigned long timeToStopMotor = 1000 * 30; // 30 seconds
unsigned long currentTime;
unsigned long timeMotorsEngaged;
boolean motorsMoving;
