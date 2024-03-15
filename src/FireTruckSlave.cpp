///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This program is what will be used to communicate to the Arduino board which will operate and control the other parts of
// the firetruck including the speed controller, water pump, and AdaFruit Wave Shield.
// Connect the Arduino over I2C as shown here: https://archive.is/iZrCx#selection-1403.0-1412.8.
// Programmed by Colby McClure and Andrew Woodlee at SMAP.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Links the header file
#include <slave/firetruck.h>

/********************************************************************************************************************/

// Setup function that only runs once when the Arduino is powered on - ctm
void setup()
{

  // Begin the serial communication and wait for the serial port to connect
  Serial.begin(9600);
  while (!Serial)
  {
    delay(100);
  }

  delay(500);

  // Initialize the water pump pin
  // Pump is enabled if LOW, disabled if HIGH
  pinMode(waterPumpPin, OUTPUT);
  digitalWrite(waterPumpPin, HIGH);
  delay(500);

  // Initialize Wave Shield
  initShield();

  Wire.begin(8);                 // join the I2C bus with address 8
  Wire.onReceive(I2C_RxHandler); // call I2C_RxHandler when data is received
}

/********************************************************************************************************************/

void loop()
{

  // add delay for sanity
  delayMicroseconds(100);
  // track the current time
  currentTime = millis();

  // motors will stop after 2 seconds
  if (motorsMoving && (currentTime - truckControlTimes.motorsEngaged >= 2000))
  {
    SpeedCon.write(90);
    motorsMoving = false;
  }

  // stop sound after specified time - will need further testing/tweaking
  if (wave.isplaying && (currentTime - timeSoundStarted >= timeToStopPlayingSound))
  {
    timeSoundStarted = 0;
    timeToStopPlayingSound = 0;

    // Stop playing, close the sound file, and go back to the beginning of the filesystem
    wave.stop();
    file.close();
    root.rewind();
    delay(2000);
  }

  // If new data is received, stop the I2C bus connection and process the data - ctm
  if (newData)
  {
    // stop I2C bus connection so that I2C ISR is not triggered
    Wire.end();
    newData = false;

    // Uncomment for debugging - ctm
    // Serial.println(data);

    // If-else statements that'll call the specific function if the condition gets met

    // If the data is a sound stop command, stop the playback - ctm
    if (data == TruckControlData.SoundStop)
    {
      stopPlayback();
      data = ' ';
    }

    // If the data is a sound one command, play the first sound - ctm
    else if (data == TruckControlData.SoundOne)
    {
      playSound(firstSound);
      data = ' ';
    }

    // If the data is a sound two command, play the second sound - ctm
    else if (data == TruckControlData.SoundTwo)
    {
      playSound(secondSound);
      data = ' ';
    }

    // If the data is a water pump command, toggle the water pump - ctm
    else if (data == TruckControlData.ToggleWaterPump)
    {
      waterPump();
      data = ' ';

      // If the data is a servo straight command, set the servo angle to 90 - ctm
    }

    // initialize and connect to I2C bus
    initI2C;
  }
}

/********************************************************************************************************************/

// I2C_RxHandler handles bytes coming over the I2C protocol from the Arduino Master
void I2C_RxHandler(int numBytes)
{
  if (Wire.available() && !newData)
  {
    // Read Any Received Data
    data = Wire.read();
    newData = true;
  }
  else
  {
    return;
  }

  Serial.print("FreeRam in I2C handler = ");
  Serial.println(FreeRam());
  delay(50);
}

/********************************************************************************************************************

initSC initializes the speed controller
For the QuicRun 1060, the minimum reverse is ~60 and the neutral is ~90.
The max forward throttle is ~160-180
See SpeedCon* macros for more detail

********************************************************************************************************************/

void initSC()
{
  SpeedCon.attach(speedControllerPin); // attaches the speed controller on pin 10
  delay(200);
}

// Function used for pausing playback
void stopPlayback()
{
  wave.stop();
  file.close();
  root.rewind();
  data = ' ';
}

/********************************************************************************************************************/

// Function for turning the water pump on and off
void waterPump()
{
  // check if water pump state is off
  if (!waterPumpEnabled)
  {
    waterPumpEnabled = true;
    digitalWrite(waterPumpPin, LOW);
    delay(1000);
  }
  else
  {
    waterPumpEnabled = false;
    digitalWrite(waterPumpPin, HIGH);
  }
}

/********************************************************************************************************************/

// initialize and check Wave Shield and SD card
void initShield()
{

  pinMode(10, OUTPUT);

  // Set the output pins for the DAC control.
  pinMode(6, OUTPUT);
  pinMode(SCK, OUTPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(MISO, OUTPUT);

  if (!card.init())
  {
    // play with 8 MHz spi (default faster!)
    // Serial.println("Card init. failed!");  // Something went wrong, lets print out why
    // sdErrorCheck();
    while (1)
      ; // then 'halt' - do nothing!
  }

  // Now we will look for a FAT partition!
  uint8_t part;
  for (part = 0; part < 5; part++)
  {
    // we have up to 5 slots to look in
    if (vol.init(card, part))
      break; // we found one, lets bail
  }
  if (part == 5)
  {
    // if we ended up not finding one  :(
    // Serial.println("No valid FAT partition!");
    // sdErrorCheck();      // Something went wrong, lets print out why
    while (1)
      ; // then 'halt' - do nothing!
  }

  // Open the root of the SD Card
  if (!root.openRoot(vol))
  {
    // Serial.println("Couldn't open root");
  }
}

/********************************************************************************************************************/

// playSound reads a file in the root directory and plays it over the speaker
void playSound(char soundFile[6])
{

  if (wave.isplaying)
  {
    wave.stop();
    file.close();
    root.rewind();
  }

  if (!file.open(root, soundFile))
  {
    // open the file in the directory
    // Serial.println("file.open failed");  // something went wrong :(
    while (1)
      ; // halt
  }

  // create wave object for the file and play if no error
  if (wave.create(file))
  {
    // play the file
    wave.play();
    // get the current time
    timeSoundStarted = millis();
    // time = 10 * 1000 = 10 s
    timeToStopPlayingSound = 10 * 1000;
  }
}