#include <Arduino.h>
#include <Wire.h>

void receiveData();

void setup()
{
    Wire.begin();       // join i2c bus
    Serial.begin(9600); // start serial for output
}

void receiveData()
{
    int address = 8;
    int bytesToRead = 6;
    Wire.requestFrom(address, bytesToRead);
    while (Wire.available())
    {
        char data = Wire.read();
        Serial.print(data);
    }
    delay(500);
}

void loop()
{
    receiveData();
}