#include <Arduino.h>
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000);
  Serial.println("Starting");
  pinMode(18, INPUT);
  unsigned long startTime = micros();
  unsigned long currentTime = startTime;
  for(int i = 0; i < 512; i++){
    analogRead(18);
    while(micros()-currentTime < 67){ //makes sure that it samples every 67 us.
    }
    currentTime = micros();
  }
  unsigned long endTime = micros();
  delay(1000);
  Serial.println(endTime - startTime);
  
}

void loop() {
  // put your main code here, to run repeatedly:
 
// Serial.println("hello world"); 
}
