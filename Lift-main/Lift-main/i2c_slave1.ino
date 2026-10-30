#include <Arduino.h>
#include <Wire.h>
            
const int IRSensorPin = 2;
const int GreenLED = 3;
const int RedLED = 4;
            
const int dataPin = 13;
const int latchPin = 12;
const int clockPin = 11;
            
const int buttonUpPin = 6;
const int buttonUpLED = 7;
const int buttonDownPin = 8;
const int buttonDownLED = 9;
            

int liftAvailable, buttonUp, buttonDown;
            
int datArray[] = {252, 96, 218, 242, 102};
            
void setup() {
pinMode(buttonUpPin, INPUT);
pinMode(buttonDownPin, INPUT);
pinMode(IRSensorPin, INPUT);
               
pinMode(GreenLED, OUTPUT);
pinMode(RedLED, OUTPUT);
pinMode(buttonUpLED, OUTPUT);
pinMode(buttonDownLED, OUTPUT);
            
pinMode(latchPin,OUTPUT);
pinMode(dataPin, OUTPUT);
pinMode(clockPin,OUTPUT);
            

Wire.begin(12);
Wire.onRequest(requestEvent);
Wire.onReceive(receiveEvent);
            
}
            
void loop() {
static unsigned long startTime = millis();
            
 if(!digitalRead(buttonUpPin)){
           digitalWrite(buttonUpLED, HIGH);
           buttonUp = 1;
              }
 if(!digitalRead(buttonDownPin)){
           digitalWrite(buttonDownLED, HIGH);
           buttonDown = 1;
             }
               

if(!digitalRead(IRSensorPin)){
         liftAvailable = 1;
             if(millis() - startTime > 1500){
                 digitalWrite(GreenLED, HIGH);
                 digitalWrite(RedLED, LOW);
                 digitalWrite(buttonUpLED, LOW);
                 buttonUp = 0;
                 digitalWrite(buttonDownLED, LOW);
                 buttonDown = 0;
                }
            } else{
               liftAvailable = 0;
               digitalWrite(GreenLED, LOW);
               digitalWrite(RedLED, HIGH);
               startTime = millis();
              }
            }

void ShowOn7Segment(int number){
          digitalWrite(latchPin, LOW);
          shiftOut(dataPin,clockPin,LSBFIRST,datArray[number]);
          digitalWrite(latchPin, HIGH);
            }

 void requestEvent(){
            int data = B00000000;
            data += B1 * liftAvailable;
            data += B10 * buttonUp;
            data += B100 * buttonDown;
            Wire.write(data);
        }
            
           
  void receiveEvent(int howMany){
               int liftPos = Wire.read();
               ShowOn7Segment(liftPos);
            }
          
