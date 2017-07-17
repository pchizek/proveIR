/*
 *  Sharp IR Timing Gate
 *  Patrick Chizek
 *  Developed for PROVE Outreach
 *  
 *  Using:
 *  
 *  TimerOne Library library by Paul Stoffregen
 *  https://github.com/PaulStoffregen/TimerOne
 *  
 */

#include <TimerOne.h>

//#define DEBUG

int           threshold = 160;
int           reads = 8;      //Make sure this number is the same as the number of times analogRead() is called  
const int     adcAdj[4] = {0,0,0,0}; 

volatile long tStart;
volatile int  startPin = 0;
volatile int  endPin = 2;

volatile bool timerEn = true;

float         tStop;
int32_t       readSum;

void setup() {
  
  // Set up serial
  Serial.begin(9600);

  Serial.println("Reading...");
  for (int i = 0; i<4; i++){

    for (int j= 0;j<10;j++){
    readSum += analogRead(i);
    delayMicroseconds(10);
    readSum += analogRead(i);
    delayMicroseconds(10);
    readSum += analogRead(i);
    delayMicroseconds(10);
    readSum += analogRead(i);
    delayMicroseconds(10);
    readSum += analogRead(i);
    delayMicroseconds(10);
    readSum += analogRead(i);
    delayMicroseconds(10);
    readSum += analogRead(i);  
    delayMicroseconds(10);
    readSum += analogRead(i);    
    }

    float avg = readSum/(reads*10);
    Serial.print("Pin A");
    Serial.print(i);
    Serial.print(" ");
    Serial.print(String(reads));
    Serial.print(" read Avg: ");
    Serial.println(avg);
    avg = 0;
    readSum = 0;
  }

  // Set up timer interrupts
  Timer1.initialize(1200);
  Serial.println("Ready");  
  Timer1.attachInterrupt(checkStart);
  
}

void loop() {
  String cmd = cmdSerialRead();
  Serial.println("---------------------");
  Serial.print(">>");
  Serial.println(cmd);
    if(cmd == "reset"){
      
      Timer1.detachInterrupt();

      Serial.println("Timer Reset");

      Timer1.attachInterrupt(checkStart);
    }

    else if(cmd == "start"){

      Timer1.detachInterrupt();
      Serial.println("Timer Started.");
      tStart = millis();

      #ifdef DEBUG
        Serial.println(tStart);
      
        Serial.print("Pin ");
        Serial.println(startPin);
      #endif
        
      delay(1);
      Timer1.attachInterrupt(checkEnd);
    }
          
    else if(cmd == "end"){

      tStop = millis() - tStart; 
      tStop = tStop/1000;
      
      Serial.print("Time: ");
      Serial.print(tStop);
      Serial.println(" Sec");

      #ifdef DEBUG
        Serial.print("Pin ");
        Serial.println(endPin);
      #endif
      
      delay(100);
    
      Timer1.attachInterrupt(checkStart);
    }

    else if(cmd == "burst"){
 
      Timer1.detachInterrupt();

      Serial.println("Reading...");
    
      for (int i = 0; i<4; i++){

          for (int j= 0;j<10;j++){
          readSum += analogRead(i);
          delayMicroseconds(10);
          readSum += analogRead(i);
          delayMicroseconds(10);
          readSum += analogRead(i);
          delayMicroseconds(10);
          readSum += analogRead(i);
          delayMicroseconds(10);
          readSum += analogRead(i);
          delayMicroseconds(10);
          readSum += analogRead(i);
          delayMicroseconds(10);
          readSum += analogRead(i);  
          delayMicroseconds(10);
          readSum += analogRead(i);    
        }

      float avg = readSum/(reads*10);
      Serial.print("Pin A");
      Serial.print(i);
      Serial.print(" ");
      Serial.print(String(reads));
      Serial.print(" read Avg: ");
      Serial.println(avg);
      avg = 0;
      readSum = 0;
    
      }

      if (timerEn){
        Serial.println("Ready");
        Timer1.attachInterrupt(checkStart);
      }

    }
    
    else if(cmd == "toggle"){
      
      if (timerEn){
        Timer1.detachInterrupt();
        timerEn = false;
        Serial.println("Timer Disabled.");
      }

      else{
        Timer1.attachInterrupt(checkStart);
        timerEn = true;
        Serial.println("Timer Enabled.");
      }      

    }
    
    else if(cmd == "set"){
    
      char val[6];
      String param = cmdSerialRead();
      cmdSerialRead().toCharArray(val,6);

      if (atof(val) > 0){
                  
        if(param == "threshold"){
            threshold = val;
        }
        else{
            Serial.println(">> Invalid Command.");
        }
      }
         
      else{
        Serial.println(">> Invalid Value.");
      }
    }
  Serial.println("---------------------");
}


void checkStart(){
  
  Timer1.detachInterrupt();
  
  int adcSum = 0;
  
  adcSum += analogRead(startPin);
  delayMicroseconds(10);
  adcSum += analogRead(startPin);
  delayMicroseconds(10);
  adcSum += analogRead(startPin);
  delayMicroseconds(10);
  adcSum += analogRead(startPin);
  delayMicroseconds(10);
  adcSum += analogRead(startPin);
  delayMicroseconds(10);
  adcSum += analogRead(startPin);
  delayMicroseconds(10);
  adcSum += analogRead(startPin);
  delayMicroseconds(10);
  adcSum += analogRead(startPin);
  
  if ((adcSum - adcAdj[startPin]) > threshold){
    
    Serial.println("START");
    tStart = millis();
    Serial.println(tStart);

    Serial.print("Pin ");
    Serial.println(startPin);
    delay(1);
    
    Timer1.attachInterrupt(checkEnd);
  }
  else{
    Timer1.attachInterrupt(checkStart);
  }

  startPin ^= 1;
  
}

void checkEnd(){
  
  Timer1.detachInterrupt();
  
  int adcSum = 0;
  
  adcSum += analogRead(endPin);
  delayMicroseconds(10);
  adcSum += analogRead(endPin);
  delayMicroseconds(10);
  adcSum += analogRead(endPin);
  delayMicroseconds(10);
  adcSum += analogRead(endPin);
  delayMicroseconds(10);
  adcSum += analogRead(endPin);
  delayMicroseconds(10);
  adcSum += analogRead(endPin);
  delayMicroseconds(10);
  adcSum += analogRead(endPin);
  delayMicroseconds(10);
  adcSum += analogRead(endPin);
  
  if ((adcSum - adcAdj[endPin]) > threshold){
    
    float tGate = millis() - tStart; 
    tGate = tGate/1000;
    
    Serial.println("---------------------");
    Serial.print("Time: ");
    Serial.print(tGate);
    Serial.println(" Sec");

    Serial.print("Pin ");
    Serial.println(endPin);
    Serial.println("---------------------");
    delay(100);
    
    Timer1.attachInterrupt(checkStart);
  }
  else{
    Timer1.attachInterrupt(checkEnd);
  }

  endPin ^= 1;
}

String cmdSerialRead(void){
  char in;
  String out;
  for (int i=0;i<31;i++) {  
    while(Serial.available()<1){}
    in = Serial.read();

    if (in == '\r'){
      //Serial.println(in);
      return out;
    }

    if (in != '\n'){
      out += String(in);
    }

    if (in == ' '){
      //Serial.println(in);
      return out;
    }

  }
  
}
