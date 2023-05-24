/////////////////////////////////////////////////////////////////////////////////////////////////////
// This program uses hardware serial to talk to the host computer and                              //
// AltSoftSerial for communication with the bluetooth module. It essentially                       //
// sets up the "frequency" in which the computer can communicate with the HM-10 bluetooth module.  //
// Note that you can only communicate with the HM-10 bluetooth module using the serial monitor     //
// that is found on the top-right side of the Arduino IDE. What ever gets sent to the serial       //
// monitor gets sent to the HM-10 and anything recieved from the HM-10 gets copied to the          //
// serial monitor. Also note that the serial monitor does not send line endings to the HM-10.      //
// Will need to use "AT" commands to communicate to the HM-10. Typing "AT" into the serial monitor //
// will send back "OK" if there is an established connection to the HM-10 from the computer.       //
/////////////////////////////////////////////////////////////////////////////////////////////////////

// Library used for serial communication on other digital pins of the Arduino
  #include <SoftwareSerial.h>

// Library used to control the servo
  #include <Servo.h>
 
// Creates the bluetooth object with the reciever and transmitter pins as the arguments
  SoftwareSerial BTSerial(8, 9); 

// The constant used for what pin the Servo will be on
  const int servoPin = 3;

// Creates the Servo object
  Servo Servo1;

String inData = "";
char appData;
boolean NL = true;
 
void setup() 
{ 

  // Sets up the baud rate of the Arduino at 9600
    Serial.begin(9600);

  // Notifying message
    Serial.println("Serial started at 9600"); 

  // Displays information about the sketch file and what day it got uploaded
    Serial.print("Sketch:   ");   Serial.println(__FILE__);
    Serial.print("Uploaded: ");   Serial.println(__DATE__);
    Serial.println(" ");
 
  // Sets up the baud rate of the HM-10 bluetooth module and prints a debug message
    BTSerial.begin(9600);  
    Serial.println("BTserial started at 9600");
    Serial.println(" "); 

  // Attaches the Servo to its designated pin
    Servo1.attach(servoPin);
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

  // The first button mapped for the Servo
     if (inData == "F") {
      digitalWrite(servoPin, HIGH);
      Serial.println("Read");
      Servo1.write(0); 
      delay(500);
      Servo1.write(180); 
      delay(500);
    } 

}
