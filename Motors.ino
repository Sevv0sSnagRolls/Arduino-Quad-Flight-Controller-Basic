/*
 * who knows.......... 
 */

#include "PinMap.h"

//void motorInit(){
//  return;
//}

int ESC1roll = -1;
int ESC1pitch = 1;
int ESC1_signal_vec[4] = {ESC1roll, ESC1pitch, ESC1roll, ESC1pitch];

int ESC2roll = 1;
int ESC2pitch = 1;
int ESC2_signal_vec[4] = {ESC2roll, ESC2pitch, ESC2roll, ESC2pitch];

int ESC3roll = 1;
int ESC3pitch = -1;
int ESC3_signal_vec[4] = {ESC3roll, ESC3pitch, ESC3roll, ESC3pitch];

int ESC4roll = -1;
int ESC4pitch = -1;
int ESC4_signal_vec[4] = {ESC4roll, ESC4pitch, ESC4roll, ESC4pitch];

float ESC_ControlSignals[4];

void multiplyAndSumSignals(){
  ESC_ControlSignal[0] = throttle;
  ESC_ControlSignal[1] = throttle;
  ESC_ControlSignal[2] = throttle;
  ESC_ControlSignal[3] = throttle; 
  
  for(int i = 0, i<=sizeof(ControlSignal), i++){
    ESC_ControlSignal[0] += ESC1_signal_vec[i]*ControlSignal[i];
    ESC_ControlSignal[1] += ESC2_signal_vec[i]*ControlSignal[i];
    ESC_ControlSignal[2] += ESC3_signal_vec[i]*ControlSignal[i];
    ESC_ControlSignal[3] += ESC4_signal_vec[i]*ControlSignal[i];
  }

  ESC_ControlSignal[0] = (ESC_ControlSignal[0] - 1000)/(256/1000);
  ESC_ControlSignal[1] = (ESC_ControlSignal[0] - 1000)/(256/1000);
  ESC_ControlSignal[2] = (ESC_ControlSignal[0] - 1000)/(256/1000);
  ESC_ControlSignal[3] = (ESC_ControlSignal[0] - 1000)/(256/1000);
}

void motorController(){
  //Run the Control Function
  PIDController();
  multiplyAndSumSignals();
  analogWrite(ESC1, ESC_ControlSignal[0]);
  analogWrite(ESC1, ESC_ControlSignal[1]);
  analogWrite(ESC1, ESC_ControlSignal[2]);
  analogWrite(ESC1, ESC_ControlSignal[3]);
}
