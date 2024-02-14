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
    right,
    left
};

// Set the struct of the firetruck state to have an old and new value - ctm
struct fireTruckState
{
    fireTruckStates newState;
    fireTruckStates oldState; 
} firetruck; 

/* Part of older implementation, comment out until we get working one
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
*/ 