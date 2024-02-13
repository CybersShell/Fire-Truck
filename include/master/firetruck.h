#include <Arduino.h>
#include <Wire.h>
#include <usbhub.h>
#include <SPI.h>
#include <common.h>
#include <PS4Parser.h> // Added this header to the file
#include <PS4BT.h>


USB Usb;
BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so

/* You can create the instance of the PS4BT class in two ways */
// This will start an inquiry and then pair with the PS4 controller - you only have to do this once
// You will need to hold down the PS and Share button at the same time, the PS4 controller will then start to blink rapidly indicating that it is in pairing mode

// create PS4 controller Bluetooth class, enter pairing mode
PS4BT GameController(&Btd, PAIR);

#define TO_MASTER_SIZE 2
#define TO_SLAVE_SIZE 4
#define NODE_READ_DELAY 100

byte messageToMaster[TO_MASTER_SIZE];
byte nodeReceive[TO_SLAVE_SIZE];
bool printAngle, printTouch;
uint8_t oldL2Value, oldR2Value;

#define LED_PIN 9

const int I2CAddress = 8; // I2C bus address

char i2cData;

void sendData(char data);

void readFromSlave();

void setState(); 