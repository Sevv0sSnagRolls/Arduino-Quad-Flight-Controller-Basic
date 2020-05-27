/*
 * Aims to capture the timing of each component of the system and send requests/commands to each subsystem based of selected/constrained timings
 * 
 * Gyroscope
 * DLPF and onboard processing means there is a time between when measurements are ready, this is defined in the gyro module
 * 
 * Motors
 * Motors should not be updated too often due to the PWM on each of the control pins, the scheduler will take care of this
 * 
 * Control System
 * The frequency of the calculation and input of the control system can be altered
 */

// void scheduler(){
//  timeNow = micros();
//  elaspedTimeIMUReadings = (timeNow - timePrevIMU);
//  
//  //Read gyro when new data is ready
//  if(elapsedTimeIMUReadings > DelayIMU){
//    timePrevIMU = timeNow; 
//    ReadIMUData();  
//  }
//
//  if(elapsedTimeIMUReadings > DelayIMU){
//    
//  }
//
//  if(elapsedTimeIMUReadings > DelayIMU){
//    
//  }
// 
// }
