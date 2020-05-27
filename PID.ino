/*
 * Implement a PD Controller for the motion of the quadcopter based of off documents 
 * ........
 * 
 * 
 * Use a PD for each state variable 
 * roll
 * rollRate
 * pitch
 * pitchRate
 * 
 * xdotWorld
 * ydotWorld
 * zdotWorld
 * xWorld
 * yWorld
 * zWorld
 */

float Kp_roll = 25;
float Kp_pitch = 25;
float Kp_rollRate = 5;
float Kp_pitchRate = 5;
float Kp[4] = {Kp_roll, Kp_pitch, Kp_rollRate, Kp_pitchRate};

float Kd_roll = 4.1;
float Kd_pitch = 4.1;
float Kd_rollRate = 2.1;
float Kd_pitchRate = 2.1;
float Kd[4] = {Kd_roll, Kd_pitch, Kd_rollRate, Kd_pitchRate};

float error[4];
float errorPrevious[4];

float cSignal;

unsigned long timePIDUpdate, timePIDPrevUpdate;
float dtPID;

int n = sizeof(Kp);

float PD(float Kp, float Kd, float errorC, float errorPreviousC, float dt){
  cSignal = Kp*errorC + Kd*(errorC - errorPreviousC)/(dt); 
  return cSignal;
}

void PIDController(){
  timePIDUpdate = micros();
  dtPID = timePIDUpdate - timePIDPrevUpdate;
  for(int i = 0; i<=3; i++){
    error[i] = state[i] - 0.0;
    controlSignal[i] = PD(Kp[i], Kd[i], error[i], errorPrevious[i], dtPID);
    errorPrevious[i] = error[i];
  }
  timePIDPrevUpdate = timePIDUpdate;
//  Serial.print("PID");
//  Serial.print(controlSignal[0]);
//  Serial.print(" | ");
//  Serial.print(controlSignal[1]);
//  Serial.print(" | ");
//  Serial.print(controlSignal[2]);
//  Serial.print(" | ");
//  Serial.println(controlSignal[3]);
}
