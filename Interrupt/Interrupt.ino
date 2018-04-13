#include "analogComp.h"
volatile int st = 0;
int startT;
int endT;
void setup() {
  //Serial.begin(9600);
  pinMode(A16,INPUT);
  analogWriteResolution(12);
  analogWrite(A14,0.18*4096/3.3);
 analogComparator.setOn(3, 2);
 Serial.begin(9600);
 analogComparator.enableInterrupt(pulse, RISING);
 //delay(1000);
 //Serial.println("Comparator enabled");
}
 
void loop() {
  if(st==1){
    analogComparator.setOff();
    //analogComparator.disableInterrupt();
    startT = millis();
    endT=0;
    while(endT<5000){
      endT = millis()-startT;
    }
    analogComparator.setOn(3, 2);
    analogComparator.enableInterrupt(pulse, RISING);
    st =0;
  }
  //Serial.println(analogRead(A16));
 // delay(100);
  
 // Serial.println("Ping");
}

void pulse(){
  
  st = 1;
  Serial.println("PULSE");
}

