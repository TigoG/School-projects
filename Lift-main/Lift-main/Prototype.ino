#include <arduinio.h>
#define DATA 13    
#define LATCH 12 
#define CLOCK 11
#define SENSOR1 2
#define SENSOR2 3

const int buttonPin = 8;
const int ledPin =  9;
const int ir = 2;
const int Red = 10;
const int Green = 9;

int buttonState = 0;
int on;

void setup() {
  pinMode(ir, INPUT);
  pinMode(buttonPin, INPUT);
  pinMode(SENSOR1, INPUT);
  pinMode(SENSOR2, INPUT);
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(Red, OUTPUT);
  pinMode(Green, OUTPUT);
  pinMode(LATCH, OUTPUT);
  pinMode(CLOCK, OUTPUT);
  pinMode(DATA, OUTPUT);
}
int data[] = {252, 96, 218, 242, 102};

void loop() {
  int i;
  buttonState = digitalRead(buttonPin);

    if(digitalRead(Green) == HIGH){
        digitalWrite(Red, LOW);
      }
  
    if (digitalRead(SENSOR1) == LOW)
    {
    digitalWrite(LATCH, LOW);
    int i = 0;
    shiftOut(DATA, CLOCK, LSBFIRST, data [i]);
    digitalWrite(LATCH, HIGH);
    digitalWrite(Green, HIGH);
    Serial.println(0);
    delay(1000);
     }

    /*
     if (digitalRead(SENSOR2) == LOW)
    {
     digitalWrite(LATCH, LOW);
     int i = 1;
     shiftOut(DATA, CLOCK, LSBFIRST, data [i]);
     digitalWrite(LATCH, HIGH);
     Serial.println(1);
     digitalWrite(Green, HIGH);
     delay(1000);
     }
    */


  if (buttonState == PRESSED) {
    on = 1;
    digitalWrite(ledPin, HIGH);
  }


   if (i == on){
    digitalWrite(ledPin, LOW);
    Motor stop
   }
}
