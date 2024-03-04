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

bool printAngle, printTouch;
uint8_t oldL2Value, oldR2Value;

#define LED_PIN 9

const int I2CAddress = 8; // I2C bus address

char dummyData = 'x';
volatile bool sendMovementData = false;

// Set the states of the left stick - ctm 
enum leftStickStates {leftStickUp, leftStickDown, leftStickNeutral}; 

// Set the struct of the left stick state to have an old and new value - ctm 
struct LeftStickState 
{
    leftStickStates newState;
    leftStickStates oldState; 
} leftStick;

// Set the states of the right stick - ctm 
enum rightStickStates {rightStickRight, rightStickLeft, rightStickNeutral};

// Set the struct of the right stick state to have an old and new value - ctm 
struct RightStickState {
    rightStickStates newState; 
    rightStickStates oldState; 
} rightStick; 

// Set the states of the firetruck - ctm 
enum fireTruckStates
{
    still,
    stillToForward,
    stillToBackward,
    forwardToStill,
    backwardToStill,
    forward,
    backward,
    forwardToRight,
    forwardToLeft,
    forwardToBackward,
    backwardToRight,
    backwardToLeft,
    backwardToForward,
    straight, 
    right,
    left
};

// Set the struct of the firetruck state to have an old and new value - ctm
struct fireTruckState
{
    fireTruckStates newState;
    fireTruckStates oldState; 
} firetruck; 


void sendData(char data, char secondMovementChar);

void readFromSlave();

void getState();

leftStickStates getStateOfLeftStick();

rightStickStates getStateOfRightStick(); 

void combineStates(); 

void fireTruckControl(); 