#include <Arduino.h>

/* struct defining the control data for the truck */
struct ControlData
{
    char MotorForward = 'F';
    char MotorStop = 'S';
    char MotorBackward = 'B';
    char SoundOne = '1';
    char SoundTwo = '2'; 
    char SoundStop = 's';
    char ServoLeft = 'L';
    char ServoMiddle = 'p';
    char ServoRight = 'R';
    char ToggleWaterPump = 'W';

    // Added characters that correspond to a specific movement - ctm
    char ForwardLeft = 'U';
    char ForwardRight = 'V';
    char BackwardLeft = 'D'; 
    char BackwardRight = 'E';

} TruckControlData;

// Moved all of the states to the header file that will be used to implement state machine - ctm 
enum FireTruckStates 
{
   firetruckStill,
   firetruckMoveForward,
   firetruckMoveBackward,
   firetruckTurnLeft,
   firetruckTurnRight
}; 

// Initialize the state to start out while still - ctm 
unsigned char FiretruckState = firetruckStill; 

// Initialize the old state variable - ctm 
unsigned char OldState; 