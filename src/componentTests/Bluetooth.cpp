#include <Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial EEBlue(A4, A5); // RX | TX

void setup()
{

    EEBlue.begin(9600); //Default Baud for comm, it may be different for your Module. Serial.println("The bluetooth gates are open.\n Connect to HC-05 from any other bluetooth device with 1234 as pairing key!.");
    pinMode(LED_BUILTIN, OUTPUT);

}

void loop() {

    // Turn on the LED if a character is available. 
    if (EEBlue.available()){
        digitalWrite(LED_BUILTIN, HIGH);
    };

    delay(5000);
    digitalWrite(LED_BUILTIN, LOW);
}