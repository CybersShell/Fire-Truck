///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This program is what will be used to communicate to the Arduino board which will operate and control the other parts of   //
// the firetruck including the motor controller, water pump, speakers, bluetooth module (HM-10), and micro SD card adapter.  //
// NOTE: To see if the circuit board and bluetooth module are communicating, use the serial monitor to check for input.      //
// Also note that the serial monitor does not send line endings to the HM-10. Programmed by Colby McClure at SMAP.           //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Links the header file
  #include "firetruck_v1.4.h"
 
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
    BTsetup(baud); 

  // Attaches the servo object to the correct servo pin and prints debug message in case it does not connect (Commented out until servo gets used)
    /*Servo1.attach(servoPin);
    if(!Servo1.attach(servoPin)) {
      Serial.println("Servo connection not established"); 
      Serial.println("");
    }*/

  // Initialize the SPI bus ports and begin connection
    initSPI();
  
  // Sets up the speaker pin
    audio.speakerPin = speakerPin;
    pinMode(speakerPin, OUTPUT);

  // Debug message in case the card does not get read
    if (!SD.begin(SDpin)) {
      Serial.println("Card cannot be read");
      return;  
    } else {
      Serial.println("Card has been read"); 
      Serial.println("");
    }

  // Function that will initialize the DC motors
    initDC(); 
  
  // The following code is to make sure that the speaker works, so it will play a clown horn to test
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

  // If else statements that'll call the specific function if the condition gets met
    if (inData == "P") {
      stop();
    } else if(inData == "A") {
      soundbyte1();
    } else if (inData == "B") {
      soundbyte2();
    } else if (inData == "M") {
      motorTest(); 
    }

  // Turn the servo right (Commented out until servo is used)
    /*if (inData == "R") {
      digitalWrite(servoPin, HIGH);
      Servo1.write(180); 
      delay(500);
      inData == "null";
    }*/ 

  // Turn the servo left (Commented out until servo is used) 
    /*if (inData == "L") {
        digitalWrite(servoPin, HIGH);
        Servo1.write(0);
        delay(500);
        inData == "null";
    }*/

}