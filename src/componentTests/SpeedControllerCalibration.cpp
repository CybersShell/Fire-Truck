/*

Coded by Marjan Olesch

Sketch from Insctructables.com

*/
#include <Arduino.h>
#include <PWMServo.h>





int value = 0; // initialize the variables you need



PWMServo firstESC; // you can control 2 or more servos simultaneously



void setup() {



 firstESC.attach(9);   // attached to pin 9

 Serial.begin(9600);   // start serial at 9600 baud



}



void loop() {



//First connect your ESC WITHOUT arming. Then open the serial and follow the instructions.



 firstESC.write(value);



 if(Serial.available()) {
   value = Serial.parseInt();   // parse an integer from serial
   Serial.write(value);
 }




}