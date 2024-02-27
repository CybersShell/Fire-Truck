#include <Arduino.h>
#include <PS4BT.h>

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
    char MovementData = 'M';
    char ServoStraight = 'T';

    // Added characters that correspond to a specific movement - ctm
    char ForwardLeft = 'U';
    char ForwardRight = 'V';
    char BackwardLeft = 'D'; 
    char BackwardRight = 'E';

} TruckControlData;

USB Usb;
BTD Btd(&Usb);
PS4BT GameController(&Btd, PAIR);


// These defined macros keep track of if the left stick moves - ctm 
#define upConditional GameController.getAnalogHat(LeftHatY) < 70
#define downConditional GameController.getAnalogHat(LeftHatY) > 220
#define leftNeutralConditional GameController.getAnalogHat(LeftHatY) > 70 && GameController.getAnalogHat(LeftHatY) < 220

// These defined macros keep track of if the right stick moves - ctm 
#define leftConditional GameController.getAnalogHat(RightHatX) < 70
#define rightConditional GameController.getAnalogHat(RightHatX) > 220
#define rightNeutralConditional GameController.getAnalogHat(RightHatX) > 70 && GameController.getAnalogHat(RightHatX) < 220
