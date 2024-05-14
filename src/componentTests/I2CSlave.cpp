#include <Arduino.h>
#include <Wire.h>

void I2C_RxHandler(int numBytes);

char data;

void setup()
{
    Serial.begin(9600);
    Wire.begin(8);                // join the I2C bus with address 8
    Wire.onReceive(I2C_RxHandler); // call receiveEvent when data is received
}

void loop()
{
    // Serial.println(data);
    delay(100);
}

// I2C_RxHandler handles bytes coming over the I2C protocol from the Arduino Master
void I2C_RxHandler(int numBytes)
{
  while (Wire.available())
  {
    // Read Any Received Data
    data = Wire.read();
    Serial.print(data);
  }
  Serial.println();
  delay(50);
}