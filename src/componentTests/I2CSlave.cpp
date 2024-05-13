#include <Arduino.h>
#include <Wire.h>

void receiveEvent(int bytes);

void setup()
{
    Wire.begin(8);                // join the I2C bus with address 8
    Wire.onReceive(receiveEvent); // call receiveEvent when data is received
}

void loop()
{
    delay(100);
}

void receiveEvent(int bytes)
{
    Wire.write("hello "); // respond with message of 6 bytes as expected by master
}