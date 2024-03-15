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
    char MovementData = 'M';
    char ServoStraight = 'T';

    // Added characters that correspond to a specific movement - ctm
    char ForwardLeft = 'U';
    char ForwardRight = 'V';
    char BackwardLeft = 'D'; 
    char BackwardRight = 'E';

} TruckControlData;
