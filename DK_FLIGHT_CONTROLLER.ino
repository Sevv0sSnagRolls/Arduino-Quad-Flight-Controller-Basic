#include "pinMap.h"
#include <Servo.h>


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

///FUNCTION DEFS///
void setupIMU();
void updateBodyFixedIMU();
void readIMURawData();
void IMUInitalErrorCalculation();

void motorInit();
void multiplyAndSumSignals();
void motorController();

float PD(float Kp, float Kd, float errorC, float errorPreviousC, float dt);
void PIDController();

void RXSHUTDOWN();
void readRX();
void RXWaitForMinThrottle();

void pciSetup(byte pin);
void pwmPIN_to_port();
void setup_pwmRead();
boolean RC_avail();
float calibrate(float Rx, int Min, int Mid, int Max);
boolean FAILSAFE(int CH);
void print_RCpwm();
void print_decimal2percentage(float dec);
boolean PWM_read(int CH);
float RC_decode(int CH);

float intergrator(float X, float Xd, float dt);
float complementaryFilter(float alpha, float currentAngleEstimation, float acceleration);
void stateEstimation();

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
Servo ESC1; 
Servo ESC2; 
Servo ESC3; 
Servo ESC4; 
  
//RX
const int channels = 1;                   // specify the number of receiver channels
float RC_in[channels];                    // an array to store the calibrated input from receiver 
float throttle;

//PID
float controlSignal[4];
//--------------------------------------------------------------------------


void setup() {
  Serial.begin(115200);  
  motorInit();
  setupIMU();  
  setup_pwmRead();

  RXWaitForMinThrottle();
}

void loop() { 

  timeNowReceiver = micros();
  elapsedTimeRXReadings = timeNowReceiver - timePrevReceiver;
  if(RC_avail() || (elapsedTimeRXReadings > RX_FRAME_RATE ) ){
    readRX();
    timePrevReceiver = timeNowReceiver;
//    Serial.print("timeRX: ");
//    Serial.println(elapsedTimeMotorControl);
  }
  
  timeNowIMU = micros();
  elapsedTimeIMUReadings = (timeNowIMU - timePrevIMU);
  if(elapsedTimeIMUReadings > IMUDelay){ 
    stateEstimation();  
    timePrevIMU = timeNowIMU;
  }
  
  //Move------------------------------------------------------
  timeNowMotors = micros();
  elapsedTimeMotorControl = (timeNowMotors - timePrevMotors);
  if(elapsedTimeMotorControl > motorUpdatingPeriod){ 
    motorController();  
    timePrevMotors = timeNowMotors;
//    Serial.print("timeMotors: ");
//    Serial.println(elapsedTimeMotorControl);
  }

  //Failsafe
  if(throttle < 1000){
    ESC1.writeMicroseconds(500);
    ESC2.writeMicroseconds(500);
    ESC3.writeMicroseconds(500);
    ESC4.writeMicroseconds(500);
    RXSHUTDOWN();
  }
  
}
