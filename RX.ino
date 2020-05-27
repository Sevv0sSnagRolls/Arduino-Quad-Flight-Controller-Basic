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
//void RXinit(){
//  attachInterrupt( digitalPinToInterrupt(throttle), ISRthrottle, RISING );
//}
//
//ISRthrottle{
//  throttleFlag = 1;
//}
