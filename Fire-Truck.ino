///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                           
// This program is what will be used to communicate to the Arduino board which will operate and control the other parts of   
// the firetruck including the motor controller, water pump, speakers, bluetooth module (HM-10), and micro SD card adapter.  
// NOTE: To see if the circuit board and bluetooth module are communicating, use the serial monitor to check for input.      
// Also note that the serial monitor does not send line endings to the HM-10. Programmed by Colby McClure and Andrew Woodlee at SMAP.
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

  // Uncomment when servo is used
  
  // Attaches the servo object to the correct servo pin and prints debug message in case it does not connect (Commented out until servo gets used)
    Servo1.attach(servoPin);
    delay(500);
    if(!Servo1.attached()) {
      Serial.println("Servo connection not established"); 
      Serial.println("");
    }
    


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

  // Start battery sensing
  batt.begin(5000, 3.2, &sigmoidal);
  batt.onDemand(ACTIVATION_PIN, LOW);

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
  // Turn the servo right
  if (data == 'R') {
    digitalWrite(servoPin, HIGH);
    Servo1.write(180); 
    delay(500);
    data = NULL;
  }
  // // Turn the servo left
  if (data == 'L') {
    digitalWrite(servoPin, HIGH);
    Servo1.write(0);
    delay(500);
    data = NULL;
  }
  // TODO: decide on how to implement backwards and forwards
  if (data == '1')
  {
    engageMotors(26, "forward");
  }
  else if (data == '2')
  {
    engageMotors(51, "forward");
  }
  else if (data == '3')
  {
    engageMotors(77, "forward");
  }
  else if (data == '4')
  {
    engageMotors(102, "forward");
  }
  else if (data == '5')
  {
    engageMotors(128, "forward");
  }
  else if (data == '6')
  {
    engageMotors(153, "forward");
  }
  else if (data == '7')
  {
    engageMotors(179, "forward");
  }
  else if (data == '8')
  {
    engageMotors(205, "forward");
  }
  else if (data == '9')
  {
    engageMotors(255, "forward");
  }
  
}



// Function that sets up bluetooth
void BTsetup(int baud) {
  BTSerial.begin(baud);
  Serial.println("BTserial started at 9600");
  Serial.println("");
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
  data = NULL;
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
  data = NULL;
}

// Function used for pausing playback
void stopPlayback() {
  audio.stopPlayback();
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
void forward(int speed) {
  analogWrite(enA, speed); 
  analogWrite(enB, speed);
  digitalWrite(in1, HIGH); 
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

// Function that moves the DC motors counter-clockwise/backward
void backward(int speed) {
  analogWrite(enA, speed); 
  analogWrite(enB, speed);
  digitalWrite(in1, LOW); 
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
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
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

// printVoltage calculates and sends the battery level over the Bluetooth connection 
void printVoltage() {
  // activate relay to sense battery
  // digitalWrite(ACTIVATION_PIN, HIGH);
  uint8_t batteryLvl = batt.level();
  // digitalWrite(ACTIVATION_PIN, LOW); 

  Serial.print("batteryLvl: ");
  Serial.println(batteryLvl);
}