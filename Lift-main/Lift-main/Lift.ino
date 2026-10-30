#define DATA 13    
#define LATCH 12 
#define CLOCK 11
#define SENSOR1 2
#define SENSOR2 3
#define SENSOR3 4
#define SENSOR4 5
#define SENSOR5 6
// setting up a array named data, with the segments for 0 to  4
int data[] = {252, 96, 218, 242, 102};
void setup() {
  Serial.begin(9600);
  pinMode(LATCH, OUTPUT);
  pinMode(CLOCK, OUTPUT);
  pinMode(DATA, OUTPUT);
  pinMode(SENSOR1, INPUT);
  pinMode(SENSOR2, INPUT);
}
void loop() {
  int i;
  
    if (digitalRead(SENSOR1) == LOW)
    {
    digitalWrite(LATCH, LOW);
    int i = 0;
    shiftOut(DATA, CLOCK, LSBFIRST, data [i]);
    digitalWrite(LATCH, HIGH);
    Serial.println(0);
    delay(1000);
     }

     if (digitalRead(SENSOR2) == LOW)
    {
     digitalWrite(LATCH, LOW);
     int i = 1;
     shiftOut(DATA, CLOCK, LSBFIRST, data [i]);
     digitalWrite(LATCH, HIGH);
     Serial.println(1);
     delay(1000);
     }

     if (digitalRead(SENSOR3) == LOW)
    {
     digitalWrite(LATCH, LOW);
     int i = 2;
     shiftOut(DATA, CLOCK, LSBFIRST, data [i]);
     digitalWrite(LATCH, HIGH);
     Serial.println(2);
     delay(1000);
     }
     
     if (digitalRead(SENSOR4) == LOW)
    {
     digitalWrite(LATCH, LOW);
     int i = 3;
     shiftOut(DATA, CLOCK, LSBFIRST, data [i]);
     digitalWrite(LATCH, HIGH);
     Serial.println(3);
     delay(1000);
     }
     
     if (digitalRead(SENSOR5) == LOW)
    {
     digitalWrite(LATCH, LOW);
     int i = 4;
     shiftOut(DATA, CLOCK, LSBFIRST, data [i]);
     digitalWrite(LATCH, HIGH);
     Serial.println(4);
     delay(1000);
     }
}
