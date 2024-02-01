#include <Arduino.h>
#include <Wire.h>
#include <usbhub.h>
#include <SPI.h>
#include <common.h>

#if defined(CONTROLLER_TEST)

// check if we are using the PS3 or PS4 controller
#if defined(PS4_CONTROLLER)
    #include <PS4BT.h>
#elif defined(PS3_CONTROLLER)
    #include <PS3BT.h>
#else
    #error "No game controller macros defined. Use the PS4 or PS3 environments: FireTruckMasterPS4 or FireTruckMasterPS3"
#endif

USB Usb;
// USBHub Hub1(&Usb); // Some dongles have a hub inside
BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so

/* You can create the instance of the PS4BT class in two ways */
// This will start an inquiry and then pair with the PS4 controller - you only have to do this once
// You will need to hold down the PS and Share button at the same time, the PS4 controller will then start to blink rapidly indicating that it is in pairing mode

// check if we are using the PS3 or PS4 controller
#if defined(PS4_CONTROLLER)
    PS4BT GameController(&Btd, PAIR);
#elif defined(PS3_CONTROLLER)
    PS3BT GameController(&Btd, PAIR);
#else
    #error "No game controller macros defined"
#endif

#endif


// After that you can simply create the instance like so and then press the PS button on the device
// PS4BT PS4(&Btd);
#define TO_MASTER_SIZE 3
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