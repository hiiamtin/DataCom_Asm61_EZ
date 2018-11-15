#include"text.h"
#include <string.h>
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_MCP4725.h>
Adafruit_MCP4725 dac;

#define defaultFreq 1700

#define Ampmax 550
#define Ampmin 300
#define errorThreshold 90

int freqMap[] = {100,300,500,700};
unsigned long delays[4];
String dataStr[] = {"00", "01", "11", "00"};
bool isUp = false;
bool isStarted = false;
int freq = 0;
String str = "";
unsigned long stopTime = 0;
unsigned long timeout = 0;
long data = 0;
int recievedTime = 0;
int dataCounter = 0;
bool isFoundStartFlag = false;
long dataout;
Servo myservo;

void setup() {
  Serial.begin(115200);
  
  myservo.attach(9);
  dac.begin(0x64);
  for (int i = 0; i < 4; i++) {
    delays[i] = (1000000 / freqMap[i] - 1000000 / defaultFreq) / 4;
  }
    Serial.flush();
}

void loop() {
  if (Serial.available() > 0) {
    String inString = Serial.readStringUntil('\n');
      dataout = inString.toInt();
      inString = "";
    }
   if(dataout>3){
    for (int k = 24; k > 0; k -= 2) {
      int tmp = dataout & 3;
      for (int i = 0; i < freqMap[tmp]/2; i++) {
        dac.setVoltage(2047, false);
        delayMicroseconds(delays[tmp]);
        dac.setVoltage(4095, false);
        delayMicroseconds(delays[tmp]);
        dac.setVoltage(2047, false);
        delayMicroseconds(delays[tmp]);
        dac.setVoltage(0, false);
        delayMicroseconds(delays[tmp]);
      }
      //Serial.println(tmp);
      dataout >>= 2;
    }
   }
   else{
     if (dataout == 1){
      myservo.write(45);  
      delay(1000);
    } 
    else if (dataout == 2){
      myservo.write(0);  
      delay(1000);
    }
    else if (dataout == 3){
      //digitalWrite(led, HIGH);
      myservo.write(90);  
      delay(1000);
   }
   dataout = 0;
   }
  
  int amp = analogRead(A0);
  if(micros() - timeout > 1000000){
    dataCounter=0;
            recievedTime=0;
            data = 0;
  }
  if (isStarted) {
    if (isUp) {

      if (amp < Ampmin) {
        isUp = false;
        freq++;
      }
    } else {
      if (amp > Ampmax) {
        isUp = true;
      }
    }
    if (micros() > stopTime) {
      for (int i = 0; i < 4; i++) {
    if (freq*2 > freqMap[i] - errorThreshold && freq*2 < freqMap[i] + errorThreshold) {
      long temp = i;
      data |= (temp << (dataCounter *2));
      dataCounter++;
      timeout = micros();
      recievedTime++;
    }
      }
    freq = 0;
      isStarted = false;
      isUp = false;
      if(recievedTime == 12)
      {
           // char pp[20];
           //sprintf(pp,"%x",data);
           //Serial.println(pp);
           // String Serialdata = String(data);
           // Serialdata.concat("D");
            Serial.print((String)data);
            dataCounter=0;
            recievedTime=0;
            data = 0;
      }
      }
  } else {
    if (amp > Ampmax) {
      // Serial.println("Started recieved");
      isStarted = true;
      stopTime = micros() + 500000;
      isUp = true;
    }
  }
}
