#include <Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial BTSerial(A4, A5); // RX | TX
char data;

// char arrays for data
char dirL[3];
char dirR[3];

void setup()
{

    Serial.begin(9600); //Default Baud for comm, it may be different for your Module. Serial.println("The bluetooth gates are open.\n Connect to HC-05 from any other bluetooth device with 1234 as pairing key!.");
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {

    // Turn on the LED if a character is available. 
    if (Serial.available()){
        data = Serial.read();

        if (data == 's') {
            Serial.readBytesUntil('X', dirL, 2);
            double dirLNum = strtod(dirL, nullptr);
            Serial.println(dirL);
            Serial.println(dirLNum);
            // Serial.findUntil(dirR, "X");
        }
        
    };
    delay(5000);
    digitalWrite(LED_BUILTIN, LOW);
}