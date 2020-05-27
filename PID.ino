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

float Kp_roll = 1;
float Kp_pitch = 1;
float Kp_rollRate = 1;
float Kp_pitchRate = 1;
float Kp[4] = {Kp_roll, Kp_pitch, Kp_rollRate, Kp_pitchRate};

float Kd_roll = 0.1;
float Kd_pitch = 0.1;
float Kd_rollRate = 0.1;
float Kd_pitchRate = 0.1;
float Kd[4] = {Kd_roll, Kd_pitch, Kd_rollRate, Kd_pitchRate};

float errorPrevious_roll;
float errorPrevious_pitch;
float errorPrevious_rollRate;
float errorPrevious_pitchRate;
float errorPrevious[4] = {errorPrevious_roll, errorPrevious_pitch, errorPrevious_rollRate, errorPrevious_pitchRate};

float cSignal;
float controlSignal[4];

unsigned long timePIDUpdate, timePIDPrevUpdate;
float dtPID;

int n = sizeof(Kp);

float PD(float Kp, float Kd, float desiredState, float estimatedState, float errorPrevious, float dt){
  error = estimatedState - desiredState;
  cSignal = Kp*error + Kd*(error - errorPrevious)/(dt); 
  errorPrevious = error;
  return cSignal
}

void PIDController(){
  dtPID = timePIDUpdate - timePIDPrevUpdate;
  for(int i = 0, i<=n, i++){
    controlSignal[i] = PD(Kp[i], Kd[i], 0, state[i], errorPrevious[i], dtPID);
  }
  timePIDPrevUpdate = timePIDUpdate;
}
