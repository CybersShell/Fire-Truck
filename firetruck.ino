///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This program is what will be used to communicate to communicate to the bluetooth module and how the bluetooth module      //
// (HM-10) will communicate and control other parts of the firetruck with includes the servo, speakers, and water pump.      //
// NOTE: to see if the circuit board and bluetooth module are communicating, use the serial monitor to check for input.      //
// Also note that the serial monitor does not send line endings to the HM-10. Programmed by Colby McClure at SMAP.           //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Links the header file
  #include "firetruck.h"
 
void setup() 
{  
  // Sets up the baud rate of the Arduino at 9600
    Serial.begin(baud);

  // Debug message in case the serial connection could not be established
    if(!Serial.available()) {
      Serial.println("Connection with serial not established");
      Serial.println(""); 
    }

  // Debug message
    Serial.println("Serial started at 9600"); 
    Serial.println("");

  // Displays information about the sketch file and what day it got uploaded
    Serial.print("Sketch:   ");   Serial.println(__FILE__);
    Serial.print("Uploaded: ");   Serial.println(__DATE__);
    Serial.println(" ");
 
  // Sets up the baud rate of the HM-10 bluetooth module and prints a debug message
    BTSerial.begin(baud);  
    Serial.println("BTserial started at 9600");
    Serial.println("");
    if(!BTSerial.available()) {
      Serial.println("Bluetooth connection not established");
      Serial.println(""); 
    }

  // Attaches the servo object to the correct servo pin and prints debug message in case it does not connect
    Servo1.attach(servoPin);
    if(!Servo1.attach(servoPin)) {
      Serial.println("Servo connection not established"); 
      Serial.println("");
    }

  // Initialize the SPI bus ports and begin connection
    pinMode(10, INPUT);
    pinMode(11, INPUT);
    pinMode(12, INPUT);
    pinMode(13, INPUT);
    pinMode(9, OUTPUT);
    SPI.begin();
  
  // Sets up the speaker pin
    audio.speakerPin = speakerPin;

  // Debug message in case the card does not get read
    if (!SD.begin(SDpin)) {
      Serial.println("Card cannot be read");
      return;  
    } else {
      Serial.println("Card has been read"); 
    }
  
  // The following code is to make sure that the speaker works, so it will play a clown horn to test that 
    audio.setVolume(7);
    audio.play("6.wav");
    if(!audio.isPlaying()) {
      Serial.println("Audio is not playing");
      Serial.println("");
    } else {
      Serial.println("Audio is playing");
      Serial.println(""); 
    }
}
 
void loop()
{

  // Read from the Bluetooth module and send to the Arduino serial monitor
    BTSerial.listen();
    if (BTSerial.available() > 0)
    {
      appData = BTSerial.read();
      inData = String(appData); 
      Serial.write(appData);
    }

  // Read from the Serial Monitor and send to the Bluetooth module
    if (Serial.available())
    {
        appData = Serial.read();
        BTSerial.write(appData);  
        if (NL) { Serial.print(">");  NL = false; }
        Serial.write(appData);
        if (appData == 10) { NL = true; }
    }

  // Turn the servo right
    if (inData == "R") {
      digitalWrite(servoPin, HIGH);
      Servo1.write(180); 
      delay(500);
      inData == "null";
    } 

  // Turn the servo left
    if (inData == "L") {
        digitalWrite(servoPin, HIGH);
        Servo1.write(0);
        delay(500);
        inData == "null";
    }
  
  // Turn the servo right
    if (inData == "R2") {
      digitalWrite(servoPin, HIGH);
      Servo1.writeMicroseconds(2000);
      delay(500);
      inData == "null";
    }

  // Turns the servo left
    if (inData == "L2") {
      digitalWrite(servoPin, HIGH);
      Servo1.writeMicroseconds(1000);
      delay(500);
      inData == "null";
    }
  
  // Plays a soundbyte
    if(inData == "Test") {
      audio.play("1.wav");
      if(audio.isPlaying() == 1) {
        Serial.println();
        Serial.println("Audio played successfully");
      } else {
        Serial.println();
        Serial.println("Audio file not found"); 
      }
      delay(500); 
      Serial.println("Read");
    }
}