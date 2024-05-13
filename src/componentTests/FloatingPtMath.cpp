#include <Arduino.h>

volatile float d;
void setup(){

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop (void)
{
  unsigned long opcount = 0;
  unsigned long starttime = millis();

  while (millis() - starttime < 1000) {
    float b,c; 
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)

    b = (float) random(100);
    c = (float) random(72);
    d = cos(b/c);
    opcount++;
  }
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
}