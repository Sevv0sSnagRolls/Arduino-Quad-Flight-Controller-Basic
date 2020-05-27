


// TIME SCHEDULING VARIABLES
//--------------------------------------------------------------------------
unsigned long timeNow, timeNowIMU, timePrevIMU;
float elapsedTimeIMUReadings;



//This variable needs to be updated in the setup based off the DLPF config etc
unsigned long IMUDelay = 2800;

void setup() {
  Serial.begin(115200);  
  setupIMU();  

}

void loop() { 
  timeNow = micros();
  elapsedTimeIMUReadings = (timeNow - timePrevIMU);
  
  //Read gyro when new data is ready
  if(elapsedTimeIMUReadings > IMUDelay){ 
    updateBodyFixedIMU();  
    timePrevIMU = timeNow;
  }

}
