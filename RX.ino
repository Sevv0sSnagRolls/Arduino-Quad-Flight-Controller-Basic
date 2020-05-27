//#include "pinMap.h"
/*
 * Initialise by setting up interrupts for control inputs
 * Digital Pins are set to interupt on RISING because:
 * RX signal is as follows, the pin will constatly be low until a signal is recieved and then 
 * it will go high. The duration of how long its high for is what matters
 *          ______________
 * 
 * _________              ______________
*/





//void throttleCalculation(){
//  throttle = (RC_in[0] - 1000)
//}

void readRX(){
  for (int i = 0; i<channels; i++){       // run through each RC channel
    int CH = i+1;
    RC_in[i] = RC_decode(CH);
  }
  throttle = RC_in[0];
//  Serial.print("throttle : ");
//  Serial.println(throttle);
}

void RXWaitForMinThrottle(){
  Serial.println("waiting for throttle arming sequence");
  Serial.println("Set Throttle to Max");
  Serial.println("...");
  unsigned long now, prev;
  while(throttle < 1800){
    now = millis();  
    if(RC_avail()){
      readRX();
      prev = now;
    }
  }
  Serial.println("Set Throttle to Min");
  while(throttle > 1100){
    now = millis(); 
    if(RC_avail()){
      readRX();
      prev = now;
    }
  }
  Serial.println("Armed");
}

void RXSHUTDOWN(){
  Serial.println("waiting for throttle arming sequence");
  Serial.println("Set Throttle to Max");
  Serial.println("...");
  unsigned long now, prev;
  while(throttle < 1800){
    now = millis();  
    if(RC_avail()){
      readRX();
      prev = now;
    }
  }
  Serial.println("Set Throttle to Min");
  while(throttle > 1100){
    now = millis(); 
    if(RC_avail()){
      readRX();
      prev = now;
    }
  }
  Serial.println("Set Throttle to Max");
  Serial.println("...");
  while(throttle < 1800){
    now = millis();  
    if(RC_avail()){
      readRX();
      prev = now;
    }
  }
  Serial.println("Armed");
}
