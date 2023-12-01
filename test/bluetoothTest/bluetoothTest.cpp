#include <Arduino.h>
#include <SoftwareSerial.h>

const int btRxPin = A4;
const int btTxPin = A3;
SoftwareSerial BTSerial(btRxPin, btTxPin);


void setup() {
  // pinMode(btRxPin, INPUT);
  // pinMode(btTxPin, OUTPUT);
  // Open serial port
  Serial.begin(9600);
  // begin bluetooth serial port communication
  BTSerial.begin(9600);

  // Empty loop to stop execution until serial is connected
  while (!Serial) { }
  
}

// Now for the loop

void loop() {
  Serial.println("Sending Bluetooth Message...");
  delay(500);
  BTSerial.write("Testing...");
}