// Library used for serial communication on other digital pins of the Arduino
  #include <SoftwareSerial.h>

// Library used to control the servo
  #include <Servo.h>

// Library for connecting to the SD card adapter
  #include <SPI.h>

// Libraries necessary for Audio playback
  #include <TMRpcm.h>
  #include <SD.h>

// Creates the bluetooth object with the reciever and transmitter pins as the arguments
  const int rx = 7;
  const int tx = 8;
  SoftwareSerial BTSerial(rx, tx); 

// Function that sets up bluetooth
  void BTsetup(int baud) {
    BTSerial.begin(baud);  
    Serial.println("BTserial started at 9600");
    Serial.println("");
    if(BTSerial.available() < 0) {
      Serial.println("Bluetooth connection not established");
      Serial.println(""); 
    }
  }

// The constants that will be used with both DC motors
  const int motor1pin1 = 2;
  const int motor1pin2 = 3;
  const int motor2pin1 = 4;
  const int motor2pin2 = 5; 

// The constants used for controlling the enabling pins
  const int enable1 = 0;
  const int enable2 = 1;

// The constant used for what pin the Servo will be on (Commented out until servo is used) 
  // const int servoPin = 3;

// Creates the "Servo1" object
  Servo Servo1;

// Constants used for SPI bus ports
  const int CSpin = 10;
  const int SCKpin = 13;
  const int MOSIpin = 11;
  const int MISOpin = 12;

// Function used to initialize SPI bus ports
  void initSPI() {
    pinMode(CSpin, INPUT);
    pinMode(MOSIpin, INPUT);
    pinMode(MISOpin, INPUT);
    pinMode(SCKpin, INPUT);
    SPI.begin();
  }

// Function to initialize DC motors and start them in the "OFF" state
  void initDC() {
    // Setup for both of the DC motors
    pinMode(motor1pin1, OUTPUT);
    pinMode(motor1pin2, OUTPUT);
    pinMode(motor2pin1, OUTPUT);
    pinMode(motor2pin2, OUTPUT);
  
  // Setup for enable pins
    pinMode(enable1, OUTPUT);
    pinMode(enable2, OUTPUT); 

  // Turn off the motors as their initial state
    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, LOW);
    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, LOW);
  }

// Create the SD card object that will play the sounds
  TMRpcm audio;
  const int speakerPin = 9;
  const int SDpin = 10;

// Variables used for recieving and transmitting data
  String inData = "";
  char appData;
  boolean NL = true;

// Constant used for baud rate
  const int baud = 9600; 

// Function used to play the first soundbyte
  void soundbyte1() {
    audio.play("3.wav", 39);
      if(audio.isPlaying() == 1) {
        Serial.println("");
        Serial.println("Audio played successfully");
      } else {
        Serial.println("");
        Serial.println("Audio file not found"); 
      }
      inData = "null";
  }

// Function used fthe play the second soundbyte
  void soundbyte2() {
    audio.play("4.wav");
      if(audio.isPlaying() == 1) {
        Serial.println("");
        Serial.println("Audio played successfully"); 
      } else {
        Serial.println("");
        Serial.println("Audio file not found"); 
      }
      inData = "null";
  }

// Function used for pausing playback
  void stop() {
    audio.stopPlayback(); 
    inData == "null";
  }

// Function used for testing the DC motors 
  void motorTest() {
   // Set the speed for the motors
     analogWrite(enable1, 100);
     analogWrite(enable2, 100);
    
   //Test the DC motors
     digitalWrite(motor1pin1, HIGH);
     digitalWrite(motor1pin2, LOW);
     delay(2000);

     digitalWrite(motor2pin1, HIGH);
     digitalWrite(motor2pin2, LOW);
     delay(2000);

     digitalWrite(motor1pin1, LOW);
     digitalWrite(motor1pin2, HIGH); 
     delay(2000); 

     digitalWrite(motor2pin1, LOW);
     digitalWrite(motor2pin2, HIGH);
     delay(2000);
  }