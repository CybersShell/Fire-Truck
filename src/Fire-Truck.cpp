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
#include <firetruck.h>
#include <Arduino.h>

void playSound(char soundFile[12]);

void printVoltage();

void engageMotors(int speed, const char* dir);

void waterPump();

void initSC();

void initShield();

void BTsetup(int baud);

void forward();

void brake();

void setup() {
  // Sets up the baud rate of the Arduino at 9600
  // Serial.begin(baud);

  // Sets up the baud rate of the HM-10 bluetooth module and prints a debug message
  BTsetup(baud);
  
  // Attaches the servo object to the correct servo pin and prints debug message in case it does not connect (Commented out until servo gets used)
  Servo1.attach(servoPin);
  delay(500);
  
  // Function that will initialize the speed controller
  initSC();


  // Initialize the water pump port
  pinMode(A5, OUTPUT);
  digitalWrite(A5, HIGH);

  
  
  // Start battery sensing
  batt.begin(BOARD_REF_VOLTAGE, 3.2, &sigmoidal);
  batt.onDemand(ACTIVATION_PIN, HIGH);
  delay(500);

  // Initalize Wave Shield
  initShield();

  // Serial.println("Program started!");
}

void loop() {
  bool newData = false;


  currentTime = millis();

  // motors will stop after 30 * 1000 ms = 30000 ms = 30 s
  if (motorsMoving && (currentTime - timeMotorsEngaged >= 30000))
  {
    brake();
    motorsMoving = false;
  }
  if (wave.isplaying && (currentTime - timeSoundStarted >= timeToStopPlayingSound ))
  {
    // Stop playing and close the sound file and go back to the beginning of the filesystem
    wave.stop();
    file.close();
    root.rewind();
  }
  

  // Read from the Bluetooth module and send to the Arduino serial monitor
  BTSerial.listen();
  if(BTSerial.available())
  {
    data = BTSerial.read();
    newData = true;
  }
  
  if (newData)
  {  
    // If-else statements that'll call the specific function if the condition gets met
    if (data == 'P') {
      wave.stop();
      data = NULL;
    } else if (data == 'A') {
      playSound("3.wav");
      data = NULL;
    } else if (data == 'C') {
      playSound("4.wav");
      data = NULL;
    } else if (data == 'F') {
      engageMotors(255, "f");
    } else if (data == 'B') {
      engageMotors(255, "b");
    } else if (data == 'S') {
      // brake();
      // motorsMoving = false;
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
        if (BTSerial.available()) break;
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
          if (BTSerial.available()) break;
        }
      } else if (servoAngle > 89)
      {
        for (servoAngle; servoAngle > 90; servoAngle--)
        {
          Servo1.write(servoAngle);
          delay(servoDelay);
          if (BTSerial.available()) break;
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
        if (BTSerial.available()) break;
      }
      data = NULL;
    }

  }
  // TODO: decide on how to implement backwards and forwards

  
}

// Function that sets up bluetooth
void BTsetup(int baud) {
  BTSerial.begin(baud);
  // Serial.println("BTserial started at 9600");
  // Serial.println("");
}

// Function to initialize DC motors and start them in the "OFF" state
void initSC() {
  SpeedCon.attach(speedControllerPin, 700, 3000);  // attaches the servo on pin 9 to the servo object
  SpeedCon.write(1500);
}



// Function used for pausing playback
void stopPlayback() {
  wave.stop();
  data = NULL;
}

// Function for turning the water pump on and off
void waterPump() {
  digitalWrite(A5, LOW);
  delay(1000);
  digitalWrite(A5, HIGH);
}


// Function that moves the DC motors clockwise/forward
void forward(int speed) {
  // analogWrite(enA_B, speed);
  // digitalWrite(in1_3, HIGH);
  // digitalWrite(in2_4, LOW);
}

// Function that moves the DC motors counter-clockwise/backward
void backward(int speed) {
  // analogWrite(enA_B, speed);
  // digitalWrite(in1_3, LOW); 
  // digitalWrite(in2_4, HIGH);
}

// engageMotors changes the motors' speed to speed and directon to dir
void engageMotors(int speed, const char* dir) {
  if (strcmp(dir, "b")) {
    backward(speed);
  } else if (dir == "forward" || dir == "f") {
    forward(speed);
  }

  // motorsMoving = true;
  // timeMotorsEngaged = currentTime;
  // BTSerial.print("Time motors engaged: ");
  // BTSerial.println(timeMotorsEngaged);
  // data = NULL;
}

void brake() {
  // digitalWrite(in1_3, LOW);
  // digitalWrite(in2_4, LOW);
}

// printVoltage calculates and sends the battery level over the Bluetooth connection 
void printVoltage() {

  BTSerial.println();
  BTSerial.print("batteryLvl: ");
  BTSerial.println(batt.level());
}

// initalize and check Wave Shield and SD card
void initShield() {

  // Set the output pins for the DAC control.
  pinMode(SS, OUTPUT);
  pinMode(SCK, OUTPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(MISO, OUTPUT);
  
  // Serial.print("Freeram: ");
  // Serial.println(FreeRam());
  
  if (!card.init()) {         //play with 8 MHz spi (default faster!)  
    // Serial.println("Card init. failed!");  // Something went wrong, lets print out why
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
    // Serial.println("No valid FAT partition!");
    // sdErrorCheck();      // Something went wrong, lets print out why
    while(1);                            // then 'halt' - do nothing!
  }

  // Open the root of the SD Card
  if (!root.openRoot(vol)) {
    // Serial.println("Couldn't open root");
  }
}

// playSound reads a file in the root directory and plays it over the speaker
void playSound(char soundFile[12]) {
  // FatReader file;

  if (!file.open(root, soundFile)) {       // open the file in the directory
      // Serial.println("file.open failed");  // something went wrong :(
      while(1);                            // halt
  }

  // create wave object for the file and play if no error
  if (wave.create(file))
  {
    BTSerial.print("Freeram: ");
    BTSerial.println(FreeRam());
    wave.play();
    timeSoundStarted = millis();
    timeToStopPlayingSound = 10 * 1000;
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
}