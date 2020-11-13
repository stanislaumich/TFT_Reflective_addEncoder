#include <Arduino.h>
uint8_t beep_sm;// если масса на другом пине, если не нужен то -1 в ините вторым параметром
uint8_t pinbeep;// собственно сам пин
//--------------------BEEP---------------------------
void beep(int t1,int t2){
   unsigned long h;
   h=millis();
   while (millis()-h<(unsigned int)(t1)){
    digitalWrite(pinbeep,HIGH);
    delayMicroseconds(t2);
    digitalWrite(pinbeep,LOW);
    delayMicroseconds(t2);
   }
   digitalWrite(pinbeep,LOW);
 }
void shortbeep(void){
  beep(125,150);
 }
void longbeep(void){
  beep(250,250);
 }
void dshortbeep(void){
  shortbeep();
  delay(50);
  shortbeep(); 
 } 
 void initBeep(uint8_t a, uint8_t b)
 {
  pinbeep=a;
  beep_sm=b;
  if(b!=-1){
      pinMode(beep_sm,OUTPUT);
      digitalWrite(beep_sm,LOW); 
  }  
  pinMode(pinbeep,OUTPUT); 
  digitalWrite(pinbeep,LOW);
 }
//-----------------------------------------------------------