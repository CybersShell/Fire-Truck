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

void setup()
{
  // Serial.begin(9600);
  // while (!Serial)
  // {
  //   delay(100);
  // }
  // Attaches the servo object to the correct servo pin and prints debug message in case it does not connect (Commented out until servo gets used)
  SteeringServo.attach(servoPin);
  delay(500);

  // Function that will initialize the speed controller
  initSC();

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

void loop()
{

  currentTime = millis();

  // motors will stop after 30 * 1000 ms = 30000 ms = 30 s
  if (motorsMoving && (currentTime - timeMotorsEngaged >= 30000))
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
    delay(5000);
  }

  if (newData)
  {
    // stop I2C bus connection so that I2C ISR is not triggered
    Wire.end();
    newData = false;
    // If-else statements that'll call the specific function if the condition gets met
    if (data == TruckControlData.SoundStop)
    {
      stopPlayback();
      data = ' ';
    }
    else if (data == TruckControlData.SoundOne)
    {
      playSound(firstSound);
      data = ' ';
    }
    else if (data == TruckControlData.SoundTwo)
    {
      playSound(secondSound);
      data = ' ';
    }
    else if (data == TruckControlData.ToggleWaterPump)
    {
      waterPump();
      data = ' ';
    }
    // begin movement conditionals
    /*
      Handled in macros:
        if firetruck control data is backward, still the truck, and then move backward
        if firetruck control data is forward, still the truck, and then move forward

        use the macros ft* to turn servos and SpeedCon* to turn move the truck

        the macros may need tweaking depending on data sent to account for the above rules for the macros
        keep fire truck straight if not turning
    */
    else if (data == TruckControlData.MotorBackward) {
      SpeedConBackward;
      // possible implementation:
      // check for servo stick left or right, if not either, do nothing
      if (movementChar == TruckControlData.ServoLeft)
      {
        ftTurnLeft;
      } else if (movementChar == TruckControlData.ServoRight) {
        ftTurnRight;
      }
      // keep fire truck straight if not turning
      ftStraight;
    }
    else if (data == TruckControlData.MotorForward) {
      SpeedConForward;
      if (movementChar == TruckControlData.ServoLeft)
      {
        ftTurnLeft;
      } else if (movementChar == TruckControlData.ServoRight) {
        ftTurnRight;
      }
      // keep fire truck straight if not turning
      ftStraight;
    }
    else if (data == TruckControlData.MotorStop) {
      SpeedConForward;
      if (movementChar == TruckControlData.ServoLeft)
      {
        ftTurnLeft;
      } else if (movementChar == TruckControlData.ServoRight) {
        ftTurnRight;
      }
    }
    else if (data == TruckControlData.ServoLeft) {
      ftTurnLeft;
    }
    else if (data == TruckControlData.ServoRight) {
      ftTurnRight;
    }


    // end movement conditionals
    // initialize and connect to I2C bus
    initI2C;
  }
}

// I2C_RxHandler handles bytes coming over the I2C protocol from the Arduino Master
void I2C_RxHandler(int numBytes)
{
  if (Wire.available() && !newData)
  { // Read Any Received Data
    data = Wire.read();
    newData = true;
  } else {
    return;
  }

  // read char for data
  if (isDataMovementChar)
  {
    movementChar = Wire.read();
  }
}

// initSC initializes the speed controller
// For the QuicRun 1060, the minimum reverse is ~17 and the neutral is ~100.
// The max forward throttle is ~160-170
void initSC()
{
  SpeedCon.attach(speedControllerPin); // attaches the servo on pin 9 to the servo object
  SpeedCon.write(2600);
  delay(1000);
  SpeedCon.write(90);
}

// Function used for pausing playback
void stopPlayback()
{
  wave.stop();
  file.close();
  root.rewind();
  data = ' ';
}

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

// engageMotors changes the motors' speed to speed and direction to dir
void engageMotors(const char *dir)
{
  if (strcmp(dir, "b"))
  {
    SpeedCon.write(17);
  }
  else if (strcmp(dir, "f"))
  {
    SpeedCon.write(180);
  }

  motorsMoving = true;
  timeMotorsEngaged = currentTime;
  data = ' ';
}

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
  { // play with 8 MHz spi (default faster!)
    // Serial.println("Card init. failed!");  // Something went wrong, lets print out why
    // sdErrorCheck();
    while (1)
      ; // then 'halt' - do nothing!
  }

  // Now we will look for a FAT partition!
  uint8_t part;
  for (part = 0; part < 5; part++)
  { // we have up to 5 slots to look in
    if (vol.init(card, part))
      break; // we found one, lets bail
  }
  if (part == 5)
  { // if we ended up not finding one  :(
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

// playSound reads a file in the root directory and plays it over the speaker
void playSound(char soundFile[12])
{

  if (wave.isplaying)
  {
    wave.stop();
    file.close();
    root.rewind();
  }

  if (!file.open(root, soundFile))
  { // open the file in the directory
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

bool checkData(char c)
{
  // cli();
  bool eq = data == c;
  // sei();
  return eq;
}

// check if data is movement char
bool isDataMovementChar (char controlData){
  return (data == TruckControlData.MotorStop || 
          data == TruckControlData.MotorForward || 
          data == TruckControlData.MotorBackward || 
          data == TruckControlData.ServoLeft || 
          data == TruckControlData.ServoRight ||
          data == TruckControlData.ServoMiddle
        );
  }
