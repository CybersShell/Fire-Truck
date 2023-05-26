// Library used for serial communication on other digital pins of the Arduino
  #include <SoftwareSerial.h>

// Library used to control the servo
  #include <Servo.h>

// Library for connecting to the SD card adapter
  #include <SPI.h>

// Libraries necessary for Audio playback
  #include <TMRpcm.h>
  #include <SD.h>

// Creates the bluetooth object with the reciever and transmitter pins as the arguments
  const int rx = 7;
  const int tx = 8;
  SoftwareSerial BTSerial(rx, tx); 

// The constant used for what pin the Servo will be on
  const int servoPin = 3;

// Creates the "Servo1" object
  Servo Servo1;

// Create the SD card object
  TMRpcm audio;
  const int speakerPin = 9;
  const int SDpin = 10;

// Variables used for recieving and transmitting data
  String inData = "";
  char appData;
  boolean NL = true;

// Constant used for baud rate
  const int baud = 9600; 
