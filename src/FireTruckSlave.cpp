///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This program is what will be used to communicate to the Arduino board which will operate and control the other parts of
// the firetruck including the motor controller, water pump, speakers, bluetooth module (HM-10), and micro SD card adapter.
// NOTE: To see if the circuit board and bluetooth module are communicating, use the serial monitor to check for input.
// Also note that the serial monitor does not send line endings to the HM-10.
// Programmed by Colby McClure and Andrew Woodlee at SMAP.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Links the header file
#include <slave/firetruck.h>
#include <Arduino.h>

void setup()
{

  Wire.begin(8);                // join the I2C bus with address 8
  Wire.onReceive(I2C_RxHandler); // call receiveEvent when data is received

  // Attaches the servo object to the correct servo pin and prints debug message in case it does not connect (Commented out until servo gets used)
  SteeringServo.attach(servoPin);
  delay(500);

  // Function that will initialize the speed controller
  initSC();

  // Initialize the water pump pin
  pinMode(waterPumpPin, OUTPUT);
  digitalWrite(waterPumpPin, HIGH);


  delay(500);

  // Initialize Wave Shield
  initShield();

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
  // stop sound after specified time
  if (wave.isplaying && (currentTime - timeSoundStarted >= timeToStopPlayingSound))
  {
    // Stop playing and close the sound file and go back to the beginning of the filesystem
    wave.stop();
    file.close();
    root.rewind();
  }

  // TODO: change logic for turning from slave to master 

  // if (newData)
  // {
  //   // data = BTSerial.read();

  //   // get data for speed
  //   if (data == 's')
  //   {
  //     engageMotor = true;
  //     Serial.readBytesUntil('Y', dirY, 2);
  //     dirYNum = strtod(dirY, nullptr);
  //     Serial.println(dirYNum);
  //   }
  //   // get data for direction
  //   if (data == 'd')
  //   {
  //     movement = true;

  //     Serial.readBytesUntil('X', dirX, 2);
  //     dirXNum = strtod(dirX, nullptr);
  //     Serial.println(dirXNum);
  //     data = Serial.read();
  //   }

  //   for (int i = 0; i < 3; i++)
  //   {
  //     dirY[i] = 0;
  //     dirX[i] = 0;
  //   }

  //   newData = true;
  // }

  if (newData)
  {
    newData = false;
    
    // If-else statements that'll call the specific function if the condition gets met
    if (data == 'P')
    {
      wave.stop();
      data = ' ';
    }
    else if (data == 'A')
    {
      playSound(firstSound);
      data = ' ';
    }
    else if (data == 'C')
    {
      playSound(secondSound);
      data = ' ';
    }
    else if (data == 'W')
    {
      waterPump();
    }
    // Turn the servo to 0 degrees
    if (Serial.available())
    {
      data = Serial.read();
    };

    if (movement)
    {
      movement = false;
      // Turn the servo to 0 degrees
      // if (dirXNum < 0)
      // {

      //   for (servoAngle = SteeringServo.read(); servoAngle >= 0; servoAngle--)
      //   {
      //     SteeringServo.write(servoAngle);
      //     delay(servoDelay);
      //     if (BTSerial.available())
      //       break;
      //   }
      //   data = ' ';
      // }
      // // Turn the servo perpendicular (90 degrees)
      // else if (dirXNum == 0)
      // {

      //   servoAngle = SteeringServo.read();

      //   if (servoAngle >= 0 && servoAngle < 89)
      //   {
      //     for (servoAngle = SteeringServo.read(); servoAngle < 90; servoAngle++)
      //     {
      //       SteeringServo.write(servoAngle);
      //       delay(servoDelay);
      //       if (BTSerial.available())
      //         break;
      //     }
      //   }
      //   else if (servoAngle > 89)
      //   {
      //     for (servoAngle = SteeringServo.read(); servoAngle > 90; servoAngle--)
      //     {
      //       SteeringServo.write(servoAngle);
      //       delay(servoDelay);
      //       if (BTSerial.available())
      //         break;
      //     }
      //   }
      //   else
      //   {
      //     SteeringServo.write(90);
      //   }
      //   data = ' ';
      // }
      // // Turn the servo to 180 degrees
      // else if (dirXNum > 0)
      // {

      //   servoAngle = SteeringServo.read();

      //   for (servoAngle = SteeringServo.read(); servoAngle <= 180; servoAngle++)
      //   {
      //     SteeringServo.write(servoAngle);
      //     delay(servoDelay);
      //     if (BTSerial.available())
      //       break;
      //   }
      //   data = ' ';
      // }
    }
    if (engageMotor)
    {
      engageMotor = false;
      if (dirYNum < 0)
      {
        SpeedCon.write(17);
      }
      else if (dirYNum == 0)
      {
        SpeedCon.write(90);
      }
      else if (dirYNum > 0)
      {
        SpeedCon.write(165);
      }
    }
  }
}

// I2C_RxHandler handles bytes coming over the I2C protocol from the Arduino Master 
void I2C_RxHandler(int numBytes)
{
  while(Wire.available()) {  // Read Any Received Data
    data = Wire.read();
  }

  newData = true;
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
  data = ' ';
}

// Function for turning the water pump on and off
void waterPump()
{
  // check if water pump state is off
  if (!waterPumpState)
  {
    waterPumpState = true;
    digitalWrite(waterPumpPin, LOW);
    delay(1000);
  } else {

    waterPumpState = false;
    digitalWrite(waterPumpPin, HIGH);
  }
}


// engageMotors changes the motors' speed to speed and directon to dir
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
  BTSerial.print("Time motors engaged: ");
  BTSerial.println(timeMotorsEngaged);
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

  // Serial.print("Freeram: ");
  // Serial.println(FreeRam());

  if (!card.init())
  { // play with 8 MHz spi (default faster!)
    // Serial.println("Card init. failed!");  // Something went wrong, lets print out why
    // sdErrorCheck();
    while(1);                            // then 'halt' - do nothing!
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
    while (1); // then 'halt' - do nothing!
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

  if (!file.open(root, soundFile))
  { // open the file in the directory
    // Serial.println("file.open failed");  // something went wrong :(
    while (1); // halt
  }

  // create wave object for the file and play if no error
  if (wave.create(file)) {
    BTSerial.print("Freeram: ");
    BTSerial.println(FreeRam());
    wave.play();
    timeSoundStarted = millis();
    timeToStopPlayingSound = 10 * 1000;
  }
  else {
    BTSerial.println("Failed to read file ");
    BTSerial.print(soundFile);
  }

}