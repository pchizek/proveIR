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

#define DETECT_THRESHOLD 1600
const int adcAdj[4] = {300,0,0,200}; 

volatile long tStart;
volatile int startPin = 0;
volatile int endPin = 2;

float tStop;
int32_t readSum;

void setup() {
  
  // Set up serial
  Serial.begin(9600);

  Serial.println(">> CALIBRATING...");
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
    }

    float avg = readSum/10;
    Serial.print("Pin ");
    Serial.print(i);
    Serial.print(" 6 read Avg:");
    Serial.println(avg);
    avg = 0;
    readSum = 0;
  }

  // Set up timer interrupts
  Timer1.initialize(1200);
  Serial.println(">> READY");  
  Timer1.attachInterrupt(checkStart);
  
}

void loop() {
  char comnd = Sread();

  switch( comnd ){
    case 'r':
    case 'R':

      Timer1.detachInterrupt();

      Serial.println(">> TIMER RESET");

      Timer1.attachInterrupt(checkStart);
      break;

    case 's':
    case 'S':

      Timer1.detachInterrupt();
      Serial.println(">> START");
      tStart = millis();
      Serial.println(tStart);
      
      Serial.print("Pin ");
      Serial.println(startPin);
      delay(1);
      Timer1.attachInterrupt(checkEnd);
      break;
      
    case 'e':
    case 'E':

      tStop = millis() - tStart; 
      tStop = tStop/1000;
      
      Serial.println("---------------------");
      Serial.print("Time: ");
      Serial.print(tStop);
      Serial.println(" Sec");

      Serial.print("Pin ");
      Serial.println(endPin);
      Serial.println("---------------------");
      delay(100);
    
      Timer1.attachInterrupt(checkStart);
      break;

    case 'c':
    case 'C':

    Timer1.detachInterrupt();

    Serial.println(">> CALIBRATING...");
    
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
      }

    float avg = readSum/10;
    Serial.print("Pin ");
    Serial.print(i);
    Serial.print(" 6 read Avg:");
    Serial.println(avg);
    avg = 0;
    readSum = 0;
    
    }

    Serial.println("READY");
    Timer1.attachInterrupt(checkStart);
    break;
    
    default: ;
    
  }
  
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
  
  if ((adcSum - adcAdj[startPin]) > DETECT_THRESHOLD){
    
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
  
  if ((adcSum - adcAdj[endPin]) > DETECT_THRESHOLD){
    
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

char Sread(void){
  while(Serial.available()<1){}
  return Serial.read();
}
