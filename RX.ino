#include "pinMap.h"
/*
 * Initialise by setting up interrupts for control inputs
 * Digital Pins are set to interupt on RISING because:
 * RX signal is as follows, the pin will constatly be low until a signal is recieved and then 
 * it will go high. The duration of how long its high for is what matters
 *          ______________
 * 
 * _________              ______________
*/

void readRX(){
  for (int i = 0; i<channels; i++){       // run through each RC channel
    int CH = i+1;
    RC_in[i] = RC_decode(CH);             // decode receiver channel and apply failsafe
  throttle = RC_in[0];
}
