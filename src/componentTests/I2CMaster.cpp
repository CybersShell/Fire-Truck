#include <Arduino.h>
#include <Wire.h>


void setup() {
  Wire.begin();
}

byte x = 0;

void loop() {
  Wire.beginTransmission(8);
  Wire.write("x is ");      
  Wire.write(x);            
  Wire.endTransmission();  

  x++;
  delay(500);
}