/*
 * who knows.......... 
 */

//#include "PinMap.h"

void motorInit(){
  ESC1.attach(ESC1Pin); 
  ESC2.attach(ESC2Pin);
  ESC3.attach(ESC3Pin);
  ESC4.attach(ESC4Pin);

  ESC1.writeMicroseconds(1000);
  ESC2.writeMicroseconds(1000);
  ESC3.writeMicroseconds(1000);
  ESC4.writeMicroseconds(1000);

  delay(10);
}

int ESC1roll = -1;
int ESC1pitch = 1;
int ESC1_signal_vec[4] = {ESC1roll, ESC1pitch, ESC1roll, ESC1pitch};

int ESC2roll = 1;
int ESC2pitch = 1;
int ESC2_signal_vec[4] = {ESC2roll, ESC2pitch, ESC2roll, ESC2pitch};

int ESC3roll = 1;
int ESC3pitch = -1;
int ESC3_signal_vec[4] = {ESC3roll, ESC3pitch, ESC3roll, ESC3pitch};

int ESC4roll = -1;
int ESC4pitch = -1;
int ESC4_signal_vec[4] = {ESC4roll, ESC4pitch, ESC4roll, ESC4pitch};

float ESC_ControlSignal[4];

int minControlSignal = 1110;
int maxControlSignal = 1800;

void multiplyAndSumSignals(){
  ESC_ControlSignal[0] = throttle;
  ESC_ControlSignal[1] = throttle;
  ESC_ControlSignal[2] = throttle;
  ESC_ControlSignal[3] = throttle; 

  for(int i = 0; i <= 1; i++){
    ESC_ControlSignal[0] += ESC1_signal_vec[i]*controlSignal[i];
    ESC_ControlSignal[1] += ESC2_signal_vec[i]*controlSignal[i];
    ESC_ControlSignal[2] += ESC3_signal_vec[i]*controlSignal[i];
    ESC_ControlSignal[3] += ESC4_signal_vec[i]*controlSignal[i];
   }

  for(int i = 0; i <= 3; i++){
   if( ESC_ControlSignal[i] < minControlSignal){
      ESC_ControlSignal[i] = minControlSignal;
   }
   else if( ESC_ControlSignal[i] > maxControlSignal){
      ESC_ControlSignal[i] = maxControlSignal;
   }
  }
}

void motorController(){
  //Run the Control Function
  PIDController();
  multiplyAndSumSignals();
  ESC1.writeMicroseconds(ESC_ControlSignal[0]);
  ESC2.writeMicroseconds(ESC_ControlSignal[1]);
  ESC3.writeMicroseconds(ESC_ControlSignal[2]);
  ESC4.writeMicroseconds(ESC_ControlSignal[3]);
  Serial.print(" Motor Controls ");
  Serial.print(ESC_ControlSignal[0]);
  Serial.print(" | ");
  Serial.print(ESC_ControlSignal[1]);
  Serial.print(" | ");
  Serial.print(ESC_ControlSignal[2]);
  Serial.print(" | ");
  Serial.println(ESC_ControlSignal[3]);
}
