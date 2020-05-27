#include "GyroRegisterMap.h"
#include <Wire.h>

// REGISTER DATA VARIABLES
//--------------------------------------------------------------------------
//Unecessary to capture individual bytes sent from slave in variables to combine
//more efficient to directly combine
int16_t ACCEL_XOUT_RAW, ACCEL_YOUT_RAW, ACCEL_ZOUT_RAW;
int16_t GYRO_XOUT_RAW, GYRO_YOUT_RAW, GYRO_ZOUT_RAW;
int16_t TEMP_RAW;
//--------------------------------------------------------------------------


float LSB_GYRO; 
float LSB_ACCEL;

//--------------------------------------------------------------------------
void setupIMU() {
  
  Wire.begin();                           
  //400kHz Clock.
  Wire.setClock(400000);

  //Wake up Arduino
  Wire.beginTransmission(MPU);            //begin, Send the slave adress (in this case 68)              
  Wire.write(0x6B);                       //make the reset (place a 0 into the 6B register)
  Wire.write(0x00);
  Wire.endTransmission(true);             //end the transmission

  //MPU 6050 Datasheet states power on sequence ramps up to VDD in 100ms
  delay(100);
  
  /** Get digital low-pass filter configuration.
 * The DLPF_CFG parameter sets the digital low pass filter configuration. It
 * also determines the internal sampling rate used by the device as shown in
 * the table below.
 *
 * Note: The accelerometer output rate is 1kHz. This means that for a Sample
 * Rate greater than 1kHz, the same accelerometer sample may be output to the
 * FIFO, DMP, and sensor registers more than once.
 *
 * <pre>
 *          |   ACCELEROMETER    |           GYROSCOPE
 * DLPF_CFG | Bandwidth | Delay  | Bandwidth | Delay  | Sample Rate
 * ---------+-----------+--------+-----------+--------+-------------
 * 0        | 260Hz     | 0ms    | 256Hz     | 0.98ms | 8kHz
 * 1        | 184Hz     | 2.0ms  | 188Hz     | 1.9ms  | 1kHz
 * 2        | 94Hz      | 3.0ms  | 98Hz      | 2.8ms  | 1kHz
 * 3        | 44Hz      | 4.9ms  | 42Hz      | 4.8ms  | 1kHz
 * 4        | 21Hz      | 8.5ms  | 20Hz      | 8.3ms  | 1kHz
 * 5        | 10Hz      | 13.8ms | 10Hz      | 13.4ms | 1kHz
 * 6        | 5Hz       | 19.0ms | 5Hz       | 18.6ms | 1kHz
 * 7        |   -- Reserved --   |   -- Reserved --   | Reserved
  */
  Wire.beginTransmission(MPU);
  Wire.write(CONFIG);
  Wire.write(DLPFSetting); 
  Wire.endTransmission(true);


  //Configure Gyroscope and Accelerometer
  
  /*
   * Now in order to obtain the gyro data in degrees/seconds we have to divide first
     by 32.8 because that's the value that the datasheet gives us for a 1000dps range

                GYROSCOPE
   * FS_SEL | FullScaleRange | LSB Sensitivity  | 
   * -------+----------------+------------------+
   * 0      | +/-250deg/sec  |       131        | 
   * 1      | +/-500deg/sec  |        65.5      |
   * 2      | +/-1000deg/sec |        32.8      |
   * 3      | +/-2000deg/sec |        16.4      | 
   * 
   *           ACCELEROMETER
   * AFS_SEL | FullScaleRange | LSB Sensitivity  | 
   * -------+----------------+------------------+
   * 0      | +/-2g/sec      |       16384      | 
   * 1      | +/-4g/sec      |        8192      |
   * 2      | +/-8g/sec      |        4096      |
   * 3      | +/-16g/sec     |        2048      | 
   * 
   */
  Wire.beginTransmission(MPU);          
  Wire.write(GYRO_CONFIG);                
  Wire.write(FS_SEL);                
  Wire.endTransmission(true);
  LSB_GYRO = 32.5;             

  Wire.beginTransmission(MPU);           
  Wire.write(ACCEL_CONFIG);                
  Wire.write(AFS_SEL);                
  Wire.endTransmission(true);             
  LSB_ACCEL = 4096;
  
  //Run Error And Setup Functions for the MPU
  //----------------------------
  IMUInitalErrorCalculation();

}

void updateBodyFixedIMU(){
      readIMURawData();  
      rollRate = (GYRO_XOUT_RAW/LSB_GYRO) - rollRateInitalError; 
      pitchRate = (GYRO_YOUT_RAW/LSB_GYRO) - pitchRateInitalError;
      accel_x = ACCEL_XOUT_RAW/LSB_ACCEL;
      accel_y = ACCEL_YOUT_RAW/LSB_ACCEL;
      accel_z = ACCEL_ZOUT_RAW/LSB_ACCEL;
}

void readIMURawData(){
      /*
       FUNCTION AIM:
       - UPDATES GLOBAL REGISTER DATA VARIABLES
        
      I2C Communication Example for each register reading:
        Wire.beginTransmission(MPU_ADDRESS);
        Wire.write(ACCEL_XOUT_H);
        Wire.endTransmission();
        Wire.requestFrom(MPU_ADDRESS,1,true);
        while(Wire.available() == 0);
        ACCEL_XOUT_H_RAW = Wire.read();
      This is highly cumbersome to write out for all tweleve bytes of data...
      
      Luckily design of MPU 6050 is clever and registers are in incremental sequence for accel and gyro
      So we can easily just read 6 registers at a time.
      This essentially boils down to a sequence of instructions like so:
      //ACCEL
      Wire.beginTransmission(MPU);
      Wire.write(ACCEL_XOUT_H);
      Wire.endTransmission(false);
      Wire.requestFrom(MPU,6, true);
      ACCEL_XOUT_H_RAW = Wire.read();
      ACCEL_XOUT_L_RAW = Wire.read();
      ACCEL_YOUT_H_RAW = Wire.read();
      ACCEL_YOUT_L_RAW = Wire.read();
      ACCEL_ZOUT_H_RAW = Wire.read();
      ACCEL_ZOUT_L_RAW = Wire.read();
      //GYRO
      Wire.beginTransmission(MPU);
      Wire.write(GYRO_XOUT_H);
      Wire.endTransmission(false);
      Wire.requestFrom(MPU,6, true);
      GYRO_XOUT_H_RAW = Wire.read();
      GYRO_XOUT_L_RAW = Wire.read();
      GYRO_YOUT_H_RAW = Wire.read();
      GYRO_YOUT_L_RAW = Wire.read();
      GYRO_ZOUT_H_RAW = Wire.read();
      GYRO_ZOUT_L_RAW = Wire.read(); 
      //Bit Shift and Combine Readings into 16bit data
      ACCEL_XOUT_RAW = ACCEL_XOUT_H_RAW<<8 | ACCEL_XOUT_L_RAW;    
      ACCEL_YOUT_RAW = ACCEL_YOUT_H_RAW<<8 | ACCEL_YOUT_L_RAW;
      ACCEL_ZOUT_RAW = ACCEL_ZOUT_H_RAW<<8 | ACCEL_ZOUT_L_RAW;
      GYRO_XOUT_RAW = GYRO_XOUT_H_RAW<<8 | GYRO_XOUT_L_RAW;    
      GYRO_YOUT_RAW = GYRO_YOUT_H_RAW<<8 | GYRO_YOUT_L_RAW;
      GYRO_ZOUT_RAW = GYRO_ZOUT_H_RAW<<8 | GYRO_ZOUT_L_RAW;
     */
     
      //Make code as efficient as possible by doing one call for all the data
      //Each Wire.read will have an Acknowledgement between master and slave so it moves on to
      //the next piece of data which is why the same call is made and the right shifted (high bits) 
      //are called first on the right side of the assignment (=) operator
      Wire.beginTransmission(MPU);
      Wire.write(ACCEL_XOUT_H);
      Wire.endTransmission(false);
      Wire.requestFrom(MPU, 14, true);
      ACCEL_XOUT_RAW = Wire.read()<<8 | Wire.read();       
      ACCEL_YOUT_RAW = Wire.read()<<8 | Wire.read();    
      ACCEL_ZOUT_RAW = Wire.read()<<8 | Wire.read();
      TEMP_RAW = Wire.read()<<8 | Wire.read();
      GYRO_XOUT_RAW = Wire.read()<<8 | Wire.read();    
      GYRO_YOUT_RAW = Wire.read()<<8 | Wire.read();
      GYRO_ZOUT_RAW = Wire.read()<<8 | Wire.read();
}


void IMUInitalErrorCalculation(){
  /*
   * Aim of function is to capture the error inherent with the sensor
   * 
   * Alters global error variables
   * 
   * Assumptions:
   *  - Assume Gyro/Quadcopter begins in a level position
   *  
   *  Arbitarily chose 200 iterations as that seemed to work.
   */

  int nSamples = 200;

  //Collect nSamples
  Serial.print("Calibrating Gyro");
  for(int i=0; i<nSamples; i++){
    readIMURawData();
    rollRateInitalError += (GYRO_XOUT_RAW/LSB_GYRO); 
    pitchRateInitalError += (GYRO_YOUT_RAW/LSB_GYRO);
    
    //wait for data to be updated based of delay from DLPF
    delayMicroseconds(IMUDelay);

    if( (i % nSamples/10) == 0 ){
      Serial.print("...");
      Serial.print(i*10);
      Serial.print("%...");
    }
  }
  Serial.println("Calibration Complete");
  //Average the data sampled
  rollRateInitalError /= 200;
  pitchRateInitalError /= 200;
}
