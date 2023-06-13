// Library that may be used later
  #include <Wire.h>

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
  const int rx = 8;
  const int tx = 9;
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
  const int in1 = 2;
  const int in2 = 4;
  const int in3 = 6;
  const int in4 = 7; 

// The constants used for controlling the enabling pins
  const int enA = 3;
  const int enB = 5;

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
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);
  
  // Setup for enable pins
    pinMode(enA, OUTPUT);
    pinMode(enB, OUTPUT); 

  // Turn off the motors as their initial state
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
  }

// Create the SD card object that will play the sounds
  TMRpcm audio;
  //const int speakerPin = 9;
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
     analogWrite(enA, 100);
     analogWrite(enB, 100);
    
   //Test the DC motors
     digitalWrite(in1, HIGH);
     digitalWrite(in2, LOW);
     delay(5000);

     digitalWrite(in3, HIGH);
     digitalWrite(in4, LOW);
     delay(5000);

     digitalWrite(in1, LOW);
     digitalWrite(in2, HIGH); 
     delay(5000); 

     digitalWrite(in3, LOW);
     digitalWrite(in4, HIGH);
     delay(5000);
  }

// Function to test the DC motors by slowly raising their speed over time
  void motorTest2() {
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);

// Accelerate from zero to maximum speed
	  for (int i = 0; i < 256; i++) {
		  analogWrite(enA, i);
		  analogWrite(enB, i);
		  delay(20);
	  }
	
// Decelerate from maximum speed to zero
	  for (int i = 255; i >= 0; --i) {
		  analogWrite(enA, i);
		  analogWrite(enB, i);
	    delay(20);
	  }
  }

  void forward() {
    analogWrite(enA, 255);
    analogWrite(enB, 255);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH); 
    digitalWrite(in4, LOW); 
  }

  void backward() {
    analogWrite(enA, 255);
    analogWrite(enB, 255);
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  }

  void brake() {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW); 
    inData == "null"; 
  }