/*
 * Implement a filter on the gyroscope calculations to get a better estimate as to the state of the craft
 * 
 * 
 */

unsigned long timeIMUupdate, timePrevIMUupdate;
float samplePeriodIMU;

//Currently using a simple Forward Euler Intergration
float intergrator(float X, float Xd, float dt){
  return X + Xd*dt;
}

float alpha = 0.01;
//Control Theory on the sizing of alpha
//float alpha(float tau, float dt){
//  return tau/(tau + dt)
//}

//Filter could be updated to Kalman, but complementary is what seems to be recommended by multirotor docs
float complementaryFilter(float alpha, float currentAngleEstimation, float acceleration){
  return (1-alpha)*currentAngleEstimation + alpha*acceleration;
}


void stateEstimation() {
    //Get new IMU Data
    updateBodyFixedIMU();
    
    //time in seconds from the exact time intergration is performed (NOW) since last state Estimation Update
    timeIMUupdate = micros();
    samplePeriodIMU = timeIMUupdate - timePrevIMUupdate;
    samplePeriodIMU /= 1000000;
    
    roll = complementaryFilter( alpha, intergrator(roll, rollRate, samplePeriodIMU), accel_y);
    pitch = complementaryFilter( alpha, intergrator(pitch, pitchRate, samplePeriodIMU), accel_y); 

    state[0] = roll;
    state[1] = pitch;
    state[2] = rollRate;
    state[3] = pitchRate;
//    Serial.print("State");
//    Serial.print(state[0]);
//    Serial.print(" | ");
//    Serial.print(state[1]);
//    Serial.print(" | ");
//    Serial.print(state[2]);
//    Serial.print(" | ");
//    Serial.println(state[3]);
//    
//    Serial.print(elapsedTimeIMUReadings);
//    Serial.print(" : ");
//    Serial.print(timeIMUupdate );
//    Serial.print(" : ");
//    Serial.print(timePrevIMUupdate);
//    Serial.print(" : ");
//    Serial.print(samplePeriodIMU);
//    Serial.print(" Roll: ");
//    Serial.print(roll);
//    Serial.print("   |   ");
//    Serial.print("Pitch: ");
//    Serial.print(pitch);
//    Serial.print("| X: ");
//    Serial.print(accel_x);
//    Serial.print("   |   ");
//    Serial.print("Y: ");
//    Serial.println(accel_y);

    timePrevIMUupdate = timeIMUupdate;
}
