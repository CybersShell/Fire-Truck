// Library used for serial communication on other digital pins of the Arduino
#include <SoftwareSerial.h>

// Libraries necessary for Audio playback
#include <WaveUtil.h>
#include <WaveHC.h>

// Library for I2C interface
#include <Wire.h>

// common headers shared between master and slave
#include <common.h>


// Constant used for baud rate
const int baud = 9600;

// Constant used for the water pump pin
const int waterPumpPin = A3;
bool waterPumpEnabled = false;

// Variables for events
unsigned long currentTime;
unsigned long timeToStopPlayingSound;
unsigned long timeSoundStarted;
boolean motorsMoving = false;



// SD card and sound objects

SdReader card;  // This object holds the information for the card
FatVolume vol;  // This holds the information for the partition on the card
FatReader root; // This holds the information for the filesystem on the card
WaveHC wave;    // This is the only wave (audio) object, since we will only play one at a time
FatReader file;

// data variables used for receiving and transmitting data
volatile char data;
volatile bool newData = false;
volatile bool canReceive = false;

char *firstSound = "3.wav";
char *secondSound = "4.wav";

// join the I2C bus with address 8
// call I2C_RxHandler when data is received
// inlined to prevent extra functions calls
#define initI2C  \
  Wire.begin(8); \
  Wire.onReceive(I2C_RxHandler);


// Begin structs
typedef struct
{
  unsigned long current;
  // this will be set in playSound()
  unsigned long soundStarted;
  // will be set in the
  unsigned long toStopPlayingSound;
} timeVariables;
 
 // create instance of timeVariables struct
timeVariables truckControlTimes;

// end structs

// Function definitions

void I2C_RxHandler(int numBytes);

void playSound(char soundFile[6]);

void stopPlayback();

void waterPump();

void initShield();
