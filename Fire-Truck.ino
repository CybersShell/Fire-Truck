///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                           //
// This program is what will be used to communicate to the Arduino board which will operate and control the other parts of   //
// the firetruck including the motor controller, water pump, speakers, bluetooth module (HM-10), and micro SD card adapter.  //
// NOTE: To see if the circuit board and bluetooth module are communicating, use the serial monitor to check for input.      //
// Also note that the serial monitor does not send line endings to the HM-10. Programmed by Colby McClure at SMAP.           //
//                                                                                                                           //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// TODO: Change all Serial calls/objects to BTserial after testing??

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

  /*
  // Uncomment when servo is used
  
  // Attaches the servo object to the correct servo pin and prints debug message in case it does not connect (Commented out until servo gets used)
    Servo1.attach(servoPin);
    if(!Servo1.attach(servoPin)) {
      Serial.println("Servo connection not established"); 
      Serial.println("");
    }
    
  */

  // Initialize the SPI bus ports and begin connection
  initSPI();

  // Sets up the speaker pin
  audio.speakerPin = speakerPin;
  pinMode(speakerPin, OUTPUT);

  // Debug message in case the card does not get read
  if (!SD.begin(SDpin)) {
    Serial.println("Card cannot be read");
    Serial.println("");
    return;
  } else {
    Serial.println("Card has been read");
    Serial.println("");
  }

  // Function that will initialize the DC motors
  initDC();

  // Initialize the water pump port
  pinMode(water, OUTPUT);
  digitalWrite(water, HIGH); 

  // The following code is to make sure that the speaker works, so it will play a clown horn to test
  audio.setVolume(7);
  audio.play("6.wav");
  if (!audio.isPlaying()) {
    Serial.println("Audio is not playing");
    Serial.println("");
  } else {
    Serial.println("Audio is playing");
    Serial.println("");
  }
}

void loop() {

  currentTime = millis();

  // Read from the Bluetooth module and send to the Arduino serial monitor
  BTSerial.listen();
  while(BTSerial.available()>0)
  {
    appData = BTSerial.read();
    Serial.print(appData);
    data += appData;
    // if (BTSerial.available() == 0)
    // {
    //   Serial.println();
    // }

  // BTSerial.println(data);
  // if (appData == '\n')
  // {
    // Serial.print("Received: ");
    // Serial.println(data);


  // if (BTSerial.available() > 0) {
  //   Serial.print("Character received: ");
  //   appData = (char) BTSerial.read();
  //   inData = String(appData);
  //   Serial.write(appData);
  //   Serial.println();
  // }

    // // Read from the Serial Monitor and send to the Bluetooth module
    // if (Serial.available() > 0) {
    //   appData = Serial.read();
    //   BTSerial.write(appData);
    // }

    // If else statements that'll call the specific function if the condition gets met
    if (data == "P") {
      stop();
      data = "";
    } else if (data == "A") {
      soundbyte1();
      data = "";
    } else if (data == "C") {
      soundbyte2();
      data = "";
    } else if (data == "M") {
      motorTest();
      data = "";
    } else if (data == "T") {
      motorTest2();
      data = "";
    } else if (data == "F") {
      forward();
      motorsMoving = true;
      timeMotorsEngaged = currentTime;
      BTSerial.print("Time motors engaged: ");
      BTSerial.println(timeMotorsEngaged);
      data = "";
    } else if (data == "B") {
      backward();
      motorsMoving = true;
      timeMotorsEngaged = currentTime;
      BTSerial.print("Time motors engaged: ");
      BTSerial.println(timeMotorsEngaged);
      data = "";
    } else if (data == "S") {
      brake();
      motorsMoving = false;
      data = "";
    } else if (data == "W") {
      waterPump();
    } else if (data == "Volts") {
      printVoltage();
      data = "";
    }

    // Turn the servo right
    if (data == "R") {
      digitalWrite(servoPin, HIGH);
      Servo1.write(180); 
      delay(500);
      data = "";
    }

    // // Turn the servo left
    if (data == "L") {
      digitalWrite(servoPin, HIGH);
      Servo1.write(0);
      delay(500);
      data = "";
    }

    // if (appData == 13) {  
        // Serial.print("Received: ");
        // Serial.println(data);
        // data = "";
    // } 
      // data = "";
  
  }

  if (motorsMoving)
  {
    if (currentTime - timeMotorsEngaged >= timeToStopMotor)
    {
      BTSerial.println("Motors stopped");
      BTSerial.print("Time motors stopped: ");
      BTSerial.println(currentTime);
      brake();
      motorsMoving = false;
    }
  }
}

// Function that sets up bluetooth
void BTsetup(int baud) {
  BTSerial.begin(baud);
  Serial.println("BTserial started at 9600");
  Serial.println("");
  // if (BTSerial.available() < 0) {
  //   Serial.println("Bluetooth connection not established");
  //   Serial.println("");
  // }
}

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

// Function used to play the first soundbyte
void soundbyte1() {
  audio.play("3.wav", 39);
  if (audio.isPlaying() == 1) {
    Serial.println("");
    Serial.println("Audio played successfully");
  } else {
    Serial.println("");
    Serial.println("Audio file not found");
  }
  inData = "null";
}

// Function used to play the second soundbyte
void soundbyte2() {
  audio.play("4.wav");
  if (audio.isPlaying() == 1) {
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
  inData = "null";
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
  for (int i = 255; i > 0; --i) {
    analogWrite(enA, i); 
    analogWrite(enB, i);
    delay(20);
  }
}

// Function that moves the DC motors clockwise/forward
void forward() {
  analogWrite(enA, 255); 
  analogWrite(enB, 255);
  digitalWrite(in1, HIGH); 
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

// Function that moves the DC motors counter-clockwise/backward
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
}

// printVoltage sends the voltage to Bluetooth and Serial connections 
void printVoltage() 
{
  volts = readVcc();
  char fstr[8];
  dtostrf(volts, 5, 2, fstr);
  Serial.print("Voltage: ");
  Serial.println(fstr);
  Serial.println();

  BTSerial.print("Voltage: ");
  BTSerial.println(volts);
  data = "";
}
