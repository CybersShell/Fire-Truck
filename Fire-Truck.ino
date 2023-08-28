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
#include "firetruck.h"

void setup() {
  // Sets up the baud rate of the Arduino at 9600
  Serial.begin(baud);

  // Debug message
  Serial.println("Serial started at 9600");
  Serial.println("");

  // Displays information about the sketch file and what day it got uploaded
  Serial.print("Sketch:   ");
  Serial.println(__FILE__);
  Serial.print("Uploaded: ");
  Serial.println(__DATE__);
  Serial.println();

  // Sets up the baud rate of the HM-10 bluetooth module and prints a debug message
  BTsetup(baud);
  
  // Attaches the servo object to the correct servo pin and prints debug message in case it does not connect (Commented out until servo gets used)
  Servo1.attach(servoPin);
  delay(500);
  
  // Sets up the speaker pin
  // audio.speakerPin = speakerPin;
  // pinMode(speakerPin, OUTPUT);


  // Function that will initialize the DC motors
  initDC();

  // Initalize SD card
  initSDCard();

  // Initialize the water pump port
  pinMode(water, OUTPUT);
  digitalWrite(water, HIGH);

  
  // Comment the following two blocks to make servo work

  // Debug message in case the card does not get read
  // if (!SD.begin(SDpin)) {
  //   Serial.println("Card cannot be read");
  //   Serial.println("");
  // } else {
  //   Serial.println("Card has been read");
  //   Serial.println("");
  // }

  // // The following code is to make sure that the speaker works, so it will play a clown horn to test
  // audio.setVolume(7);
  // audio.play("6.wav");
  // if (!audio.isPlaying()) {
  //   Serial.println("Audio is not playing");
  //   Serial.println("");
  // } else {
  //   Serial.println("Audio is playing");
  //   Serial.println("");
  // }
  
  // Start battery sensing
  batt.begin(BOARD_REF_VOLTAGE, 3.2, &sigmoidal);
  batt.onDemand(ACTIVATION_PIN, HIGH);
  delay(500);

  servoAngle = Servo1.read();
  Serial.println("Program started!");
}

void loop() {

  currentTime = millis();

  if (motorsMoving && (currentTime - timeMotorsEngaged >= timeToStopMotor))
  {
    brake();
    motorsMoving = false;
  }

  // Read from the Bluetooth module and send to the Arduino serial monitor
  BTSerial.listen();
  if(BTSerial.available())
  {
    data = BTSerial.read();
    Serial.print(data);
  }
  
  // If-else statements that'll call the specific function if the condition gets met
  if (data == 'P') {
    stopPlayback();
    data = NULL;
  } else if (data == 'A') {
    soundbyte1();
    data = NULL;
  } else if (data == 'C') {
    soundbyte2();
    data = NULL;
  } else if (data == 'C') {
    motorTest();
    data = NULL;
  } else if (data == 'T') {
    motorTest2();
    data = NULL;
  } else if (data == 'F') {
    engageMotors(255, "forward");
  } else if (data == 'B') {
    engageMotors(255, "backward");
  } else if (data == 'S') {
    brake();
    motorsMoving = false;
    data = NULL;
  } else if (data == 'W') {
    waterPump();
  } else if (data == 'V') {
    printVoltage();
    data = NULL;
  }
  // Turn the servo to 0 degrees
  if (data == 'R') {
    
    servoAngle = Servo1.read();

    for (servoAngle; servoAngle >= 0; servoAngle--)
    {
      Servo1.write(servoAngle);
      delay(servoDelay);
      CHAR_AVAILABLE_BREAK;
    }
    data = NULL;
  }
  // Turn the servo perpendicular (90 degrees)
  if (data == 't') {
    
    servoAngle = Servo1.read();
    
    if (servoAngle >= 0 && servoAngle < 89)
    {
      for (servoAngle; servoAngle < 90; servoAngle++)
      {
        Servo1.write(servoAngle);
        delay(servoDelay);
        CHAR_AVAILABLE_BREAK;
      }
    } else if (servoAngle > 89)
    {
      for (servoAngle; servoAngle > 90; servoAngle--)
      {
        Servo1.write(servoAngle);
        delay(servoDelay);
        CHAR_AVAILABLE_BREAK;
      }
    } else {
      Servo1.write(90);
    }
    data = NULL;
  }
  // Turn the servo to 180 degrees
  if (data == 'L') {
    
    servoAngle = Servo1.read();
    
    for (servoAngle; servoAngle <= 180; servoAngle++)
    {
      Servo1.write(servoAngle);
      delay(servoDelay);
      CHAR_AVAILABLE_BREAK;
    }
    data = NULL;
  }

  // TODO: decide on how to implement backwards and forwards

  
}

// Function that sets up bluetooth
void BTsetup(int baud) {
  BTSerial.begin(baud);
  Serial.println("BTserial started at 9600");
  Serial.println("");
}

// Function to initialize DC motors and start them in the "OFF" state
void initDC() {
  // Setup for both of the DC motors
  pinMode(in1_3, OUTPUT);
  pinMode(in2_4, OUTPUT);

  // Setup for enable pins 
  pinMode(enA_B, OUTPUT);

  // Turn off the motors as their initial state
  digitalWrite(in1_3, LOW);
  digitalWrite(in2_4, LOW);
}

// Function used to play the first soundbyte
void soundbyte1() {
  playSound("3.wav");
  data = NULL;
}

// Function used to play the second soundbyte
void soundbyte2() {
  playSound("4.wav");
  data = NULL;
}

// Function used for pausing playback
void stopPlayback() {
  wave.stop();
  data = NULL;
}

// Function for turning the water pump on and off
void waterPump() {
  digitalWrite(water, LOW);
  delay(1000);
  digitalWrite(water, HIGH);
}

// Function used for testing the DC motors
void motorTest() {

  // Set the speed for the motors
  analogWrite(enA_B, 100);

  //Test the DC motors
  digitalWrite(in1_3, HIGH); 
  digitalWrite(in2_4, LOW);
  delay(5000);

  digitalWrite(in1_3, LOW); 
  digitalWrite(in2_4, HIGH);
  delay(5000);
}

// Function to test the DC motors by slowly raising their speed over time
void motorTest2() {
  digitalWrite(in1_3, HIGH);
  digitalWrite(in2_4, LOW);

  // Accelerate from zero to maximum speed
  for (int i = 0; i < 256; i++) {
    analogWrite(enA_B, i);
    delay(20);
  }

  // Decelerate from maximum speed to zero
  for (int i = 255; i > 0; --i) {
    analogWrite(enA_B, i);
    delay(20);
  }
}

// Function that moves the DC motors clockwise/forward
void forward(int speed) {
  analogWrite(enA_B, speed);
  digitalWrite(in1_3, HIGH);
  digitalWrite(in2_4, LOW);
}

// Function that moves the DC motors counter-clockwise/backward
void backward(int speed) {
  analogWrite(enA_B, speed);
  digitalWrite(in1_3, LOW); 
  digitalWrite(in2_4, HIGH);
}

// engageMotors changes the motors' speed to speed and directon to dir
void engageMotors(int speed, String dir) {
  if (dir == "backward" || dir == "b") {
    forward(speed);
  } else if (dir == "forward" || dir == "f") {
    backward(speed);
  }

  motorsMoving = true;
  timeMotorsEngaged = currentTime;
  BTSerial.print("Time motors engaged: ");
  BTSerial.println(timeMotorsEngaged);
  data = NULL;
}

void brake() {
  digitalWrite(in1_3, LOW);
  digitalWrite(in2_4, LOW);
}

// printVoltage calculates and sends the battery level over the Bluetooth connection 
void printVoltage() {
  uint8_t batteryLvl = batt.level();

  BTSerial.println();
  BTSerial.print("batteryLvl: ");
  BTSerial.println(batt.level());
}

// initalize and check SD card
void initSDCard() {

  // Set the output pins for the DAC control.
  pinMode(CSpin, OUTPUT);
  pinMode(MOSIpin, OUTPUT);
  pinMode(MISOpin, OUTPUT);
  pinMode(SCKpin, OUTPUT);
  
  if (!card.init()) {         //play with 8 MHz spi (default faster!)  
    Serial.println("Card init. failed!");  // Something went wrong, lets print out why
    // sdErrorCheck();
    // while(1);                            // then 'halt' - do nothing!
  }

  // Now we will look for a FAT partition!
  uint8_t part;
  for (part = 0; part < 5; part++) {     // we have up to 5 slots to look in
    if (vol.init(card, part)) 
      break;                             // we found one, lets bail
  }
  if (part == 5) {                       // if we ended up not finding one  :(
    Serial.println("No valid FAT partition!");
    // sdErrorCheck();      // Something went wrong, lets print out why
    // while(1);                            // then 'halt' - do nothing!
  }
  
  // Let's tell the user about what we found
  Serial.println("Using partition ");
  Serial.print(part, DEC);
  Serial.println(", type is FAT");
  Serial.println(vol.fatType(),DEC);     // FAT16 or FAT32?

  // Open the root of the SD Card
  if (!root.openRoot(vol)) {
    Serial.println("Couldn't open root");
  }
}

// playSound reads a file in the root directory and plays it over the speaker
void playSound(char soundFile[12]) {
  FatReader file;

  if (!file.open(root, soundFile)) {       // open the file in the directory
      Serial.println("file.open failed");  // something went wrong :(
      while(1);                            // halt
  }

  // create wave object for the file and play if no error
  if (wave.create(file))
  {
    wave.play();
  } else {
    BTSerial.println("Failed to read file ");
    BTSerial.print(soundFile);
  }

  if (wave.isplaying)
  {
    BTSerial.println("Audio playing.");
  } else {
    BTSerial.println("Audio not playing.");
  }
  // Go back to the begining of the filesystem
  root.rewind();
}