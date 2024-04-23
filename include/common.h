#include <Arduino.h>

// I2C bus address for Arduino Slave
const int I2CAddress = 8;

/* struct defining the control data for the truck */
struct ControlData
{
    char SoundOne = '1';
    char SoundTwo = '2'; 
    char SoundStop = 's';
    char ToggleWaterPump = 'W';

} TruckControlData;
