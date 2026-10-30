#include <Arduino.h>
#include <Wire.h>
#include <Keypad.h>         

#define a 2
#define b 3
#define c 4
#define d 5                  

#define ROW1 22
#define ROW2 23
#define ROW3 24
#define ROW4 25
#define COL1 26
#define COL2 27
#define COL3 28            

int steps = 0;
boolean clockwise = true;            

const byte ROWS = 4;
const byte COLS = 3;
            
char lift[ROWS][COLS] = {
        {'1', '2', '3'},
        {'4', '5', '6'},
        {'7', '8', '9'},
        {'*', '0', '#'}
        };
            
byte rowPins[ROWS] = {ROW1, ROW2, ROW3, ROW4};
byte colPins[COLS] = {COL1, COL2, COL3};
            
Keypad liftKeypad = Keypad( makeKeymap(lift), rowPins, colPins, ROWS, COLS);
            

const int CONNECTED_SLAVES = 5;
int floorButtonUp[CONNECTED_SLAVES];
int floorButtonDown[CONNECTED_SLAVES];
int floorButtonElevator[CONNECTED_SLAVES];
int stopButton;
boolean liftAvailable[CONNECTED_SLAVES];
            
int currentFloor = 0;
int floorStopTime = 4000;
            
boolean movingUp = true;
boolean moveUp = false;
boolean moveDown = false;
            
void setup() {
      Serial.begin(2000000);
      Wire.begin(); 
            

     pinMode(a, OUTPUT);
     pinMode(b, OUTPUT);
     pinMode(c, OUTPUT);
     pinMode(d, OUTPUT);
            

   for (int i = 0; i < CONNECTED_SLAVES; i++) {
        liftAvailable[i] = false;
        floorButtonUp[i] = 0;
        floorButtonDown[i] = 0;
         }
            

  liftKeypad.addEventListener(keypadEvent);
            }
            
void loop() {
    char liftKey = liftKeypad.getKey();
    SendDataToAllFloors();
    checkMoveLift();
    moveLift();                                 
         }
            

void SendDataToAllFloors() {
              
   for (int i = 0; i < CONNECTED_SLAVES; i++) {
   int e = 0;
   Wire.requestFrom(i + 8, 1); 
            
   while (Wire.available()) {
   e = Wire.read();
   liftAvailable[i] = e & 1;
   if (e & 1) {
    currentFloor = i;
    }
    floorButtonUp[i] = e >> 1 & 1;
    floorButtonDown[i] = e >> 2 & 1;
            }
          }
    for (int j = 0; j < CONNECTED_SLAVES; j++) {
    Wire.beginTransmission(j + 8);
    Wire.write(currentFloor);
    Wire.endTransmission();
       }
   }
            

void keypadEvent(KeypadEvent liftKey) {

  switch(liftKey){
    case '0':
    floorButtonElevator[0] = 1;
    break;
    case '1':
    floorButtonElevator[1] = 1;
    break;
    case '2':
    floorButtonElevator[2] = 1;
    break;
    case '3':
    floorButtonElevator[3] = 1;
    break;
    case '4':
    floorButtonElevator[4] = 1;
    break;
    case '*':
    stopButton = 1;
    break;
               }
   }

void checkMoveLift() {
     if ((floorButtonDown[currentFloor] || floorButtonUp[currentFloor] || floorButtonElevator[currentFloor]) && liftAvailable[currentFloor]) {
         if (movingUp && (floorButtonUp[currentFloor] || floorButtonElevator[currentFloor])) {
              moveUp = false;
              moveDown = false;
              delay(floorStopTime);
              floorButtonElevator[currentFloor] = 0;
              return;
    }
    else if (!movingUp && (floorButtonDown[currentFloor] || floorButtonElevator[currentFloor])) {
         moveUp = false;
         moveDown = false;
         delay(floorStopTime);
         floorButtonElevator[currentFloor] = 0;
         return;
     } 
     else if (movingUp) {
         boolean stopHere = true;
         for (int i = currentFloor + 1; i < (sizeof(floorButtonUp) / sizeof(int)); i++) {
         if (floorButtonUp[i] || floorButtonDown[i] || floorButtonElevator[i]) {
         stopHere = false;
         break;
           }
        }
            
      if (stopHere) {
         moveUp = false;
         moveDown = false;
         delay(floorStopTime);
         floorButtonElevator[currentFloor] = 0;
         return;
            }
        } else if (!movingUp) {
         boolean stopHere = true;
         for (int i = currentFloor - 1; i >= 0; i--) {
             if (floorButtonUp[i] || floorButtonDown[i] || floorButtonElevator[i]) {
                stopHere = false;
                break;
           }
       }
            
     if (stopHere) {
         moveUp = false;
         moveDown = false;
         delay(floorStopTime);
         floorButtonElevator[currentFloor] = 0;
         return;
         }
       }

    if (movingUp) {
      for (int i = currentFloor; i < (sizeof(floorButtonUp) / sizeof(int)); i++) {
        if (floorButtonUp[i] || floorButtonElevator[i]) {
          moveUp = true;
          moveDown = false;
          movingUp = true;
          return;
         }
       }
      for (int i = currentFloor; i < (sizeof(floorButtonUp) / sizeof(int)); i++) {
        if (floorButtonDown[i] || floorButtonElevator[i]) {
           moveUp = true;
           moveDown = false;
           movingUp = true;
           return;
          }
       }

      for (int i = currentFloor; i >= 0; i--) {
       if (floorButtonDown[i] || floorButtonElevator[i]) {
       moveUp = false;
       moveDown = true;
       movingUp = false;
       return;
          }
      }

      for (int i = currentFloor; i >= 0; i--) {
       if (floorButtonUp[i] || floorButtonElevator[i]) {
         moveUp = false;
         moveDown = true;
         movingUp = false;
         return;
            }
         }
    } 
    
    else if (!movingUp) {
        for (int i = currentFloor; i >= 0; i--) {
         if (floorButtonUp[i] || floorButtonElevator[i]) {
              moveUp = false;
              moveDown = true;
              movingUp = false;
              return;
              }
           }
        
        for (int i = currentFloor; i >= 0; i--) {
         if (floorButtonDown[i] || floorButtonElevator[i]) {
             moveUp = false;
             moveDown = true;
             movingUp = false;
             return;
                }
            }

        for (int i = currentFloor; i < (sizeof(floorButtonUp) / sizeof(int)); i++) {
         if (floorButtonDown[i] || floorButtonElevator[i]) {
             moveUp = true;
             moveDown = false;
             movingUp = true;
             return;
                }
             }

        for (int i = currentFloor; i < (sizeof(floorButtonUp) / sizeof(int)); i++) {
         if (floorButtonUp[i] || floorButtonElevator[i]) {
             moveUp = true;
             moveDown = false;
             movingUp = true;
             return;
                }
             }
      }
     moveUp = false;
     moveDown = false;
  }
}
          
 void moveLift() {
      if (moveUp) {
         liftController(true);
         } 
      else if (moveDown) {
         liftController(false);
         }
     }
            
void liftController(boolean up) {
     clockwise = !up;
    if (stopButton) {
        return;
        }
   for (int i = 0; i < 100; i++) {
        stepper();
        delay(2000);
        char liftKey = liftKeypad.getKey(); // Also check the input of the keypad while the elevator is moving
        }
   }
            
void stepper() {
     switch (steps) {
             case 0:
             digitalWrite(a, HIGH);
             digitalWrite(b, HIGH);
             digitalWrite(c, LOW);
             digitalWrite(d, LOW);
             break;
             case 1:
             digitalWrite(a, LOW);
             digitalWrite(b, HIGH);
             digitalWrite(c, HIGH);
             digitalWrite(d, LOW);
             break;
             case 2:
             digitalWrite(a, LOW);
             digitalWrite(b, LOW);
             digitalWrite(c, HIGH);
             digitalWrite(d, HIGH);
             break;
             case 3:
             digitalWrite(a, HIGH);
             digitalWrite(b, LOW);
             digitalWrite(c, LOW);
             digitalWrite(d, HIGH);
             break;
             default:
             digitalWrite(a, LOW);
             digitalWrite(b, LOW);
             digitalWrite(c, LOW);
             digitalWrite(d, LOW);
             break;
            }
            
    if (clockwise) {
        steps++;
        }
   else {
         steps--;
        }
            
   if (steps > 3) {
       steps = 0;
       } 
       else if (steps < 0) {
       steps = 3;
    }

  
}
