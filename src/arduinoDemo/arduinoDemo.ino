#include <TMRpcm.h>
#include <SD.h> 
#include <SPI.h> 

const int in1 = 2;
const int in2 = 4;
const int in3 = 6; 
const int in4 = 7;

const int enA = 3;
const int enB = 5; 

const int CSpin = 10; 
const int SCKpin = 13;
const int MOSIpin = 11; 
const int MISOpin = 12;
const int SDpin = 10;

TMRpcm audio; 
const int speakerPin = 9; 

const int water = A5; 

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600); 

  audio.setVolume(7);
  audio.play("6.wav");
  if (!audio.isPlaying()) {
    Serial.println("Audio is not playing");
    Serial.println("");
  } else {
    Serial.println("Audio is playing");
    Serial.println("");
  }

  pinMode(CSpin, INPUT);
  pinMode(MOSIpin, INPUT);
  pinMode(MISOpin, INPUT);
  pinMode(SCKpin, INPUT);
  SPI.begin();

  audio.speakerPin = speakerPin;
  pinMode(speakerPin, OUTPUT); 

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);

  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

  pinMode(water, OUTPUT);
  digitalWrite(water, HIGH);
  
}

void loop() {
  // put your main code here, to run repeatedly:

  analogWrite(enA, 255);
  analogWrite(enB, 255);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(3000); 

  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

  analogWrite(enA, 255);
  analogWrite(enB, 255);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(3000); 

  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

  digitalWrite(water, LOW);
  delay(5000);
  digitalWrite(water, HIGH);
  delay(2000); 

  audio.play("4.wav"); 

}
