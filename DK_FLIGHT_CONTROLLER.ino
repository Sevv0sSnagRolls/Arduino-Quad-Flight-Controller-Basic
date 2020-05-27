#include "pinMap.h"


/*
 * Crital Program Infomation
 * 
 * Before Use of this Flight Controller, you must perform a few setup functions and update the flight controller with
 * values from those setup functions
 * 
 * IMU
 * Need to run an IMU zero test, this will give you the specific offset parameters related to your IMU unit. These are indivudalised
 * for each unit based off build differences. You will then need to update the offset parameters in the Gyroscope config header
 * 
 * RX
 * Need to run tests on your specific TX/RX Setup, you are looking to find:
 * - Average Microseconds of signal for CHANNEL LOW, CHANNEL MID, CHANNEL HIGH for each Channel in use 
 *   and then update the parameters in the pwmRead file
 * - The frame rate of your reciever, does it operate on 25000us or 22100us etc and update the RX_FRAME_RATE_VARIABLE ACCORDINGLY
 * - 
 */





//------------------------------------------------------------------------
//--TIME SCHEDULING VARIABLES--------------------------------------------
//------------------------------------------------------------------------
unsigned long timeNow;


//IMU
unsigned long timeNowIMU, timePrevIMU;
float elapsedTimeIMUReadings;
//!!!!!!This variable needs to be updated in the setup based off 
//the DLPF config etc!!!!!
unsigned long IMUDelay = 2800;


//MOTORS
unsigned long timeNowMotors, timePrevMotors;
float elapsedTimeMotorControl;
//Time between updates to PWM signals on Motor pins in microseconds
unsigned long motorUpdatingPeriod = 50000;

//RX
unsigned long timeNowReceiver, timePrevReceiver;
float elapsedTimeRXReadings;
unsigned long RX_FRAME_RATE = 25000; //microseconds

//------------------------------------------------------------------------


//------------------------------------------------------------------------
//--GLOBAL VARIABLES-------------------------------------------------------
//-------------------------------------------------------------------------

//IMU
float accel_x, accel_y, accel_z;                     
float roll, pitch, yaw;        
float rollRateInitalError, pitchRateInitalError;               
float rollRate, pitchRate;
float state[4] = {roll, pitch, rollRate, pitchRate};

//MOTORS

//RX
const int channels = 1;                   // specify the number of receiver channels
float RC_in[channels];                    // an array to store the calibrated input from receiver 
float throttle;
//--------------------------------------------------------------------------


void setup() {
  Serial.begin(115200);  
  setupIMU();  
  setup_pwmRead();
  
}

void loop() { 
 
  timeNowIMU = micros();
  //Sense
  elapsedTimeIMUReadings = (timeNowIMU - timePrevIMU);
  //Read gyro when new data is ready
  if(elapsedTimeIMUReadings > IMUDelay){ 
    stateEstimation();  
    timePrevIMU = timeNowIMU;
  }
  
  //Move------------------------------------------------------
  timeNowReceiver = micros();
  elapsedTimeRXReadings = timeNowReceiver - timePrevReceiver;
  if(RC_avail() || (elapsedTimeRXReadings > RX_FRAME_RATE ) ){
    readRX();
    timePrevReceiver = timeNowReceiver;
  }

  //Move------------------------------------------------------
  timeNowMotors = micros();
  elapsedTimeIMUReadings = (timeNowMotors - timePrevMotors);
  if(elapsedTimeMotorControl > motorUpdatingPeriod){ 
    stateEstimation();  
    timePrevMotors = timeNowMotors;
  }

    
}
