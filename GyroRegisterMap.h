// REGISTER MAPPING
//--------------------------------------------------------------------------
#define MPU 0x68
// ACCELEROMETER
#define ACCEL_CONFIG 0x1C
#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x4F
#define ACCEL_ZOUT_L 0x40
// GYROSCOPE
#define GYRO_CONFIG 0x1B
#define GYRO_XOUT_H 0x43
#define GYRO_XOUT_L 0x44
#define GYRO_YOUT_H 0x45
#define GYRO_YOUT_L 0x46
#define GYRO_ZOUT_H 0x47
#define GYRO_ZOUT_L 0x48
// TEMPERATURE SENSOR
#define TEMP_OUT_H 0x41
#define TEMP_OUT_L 0x42

// CONFIGURATION
#define CONFIG 0x1A
#define GYRO_CONFIG 0x1B  //FS_SEL for bits 3 & 4; Write byte-> [X, X, X, FS_SEL[1}, FS_SEL[0], X, X, X]
#define ACCEL_CONFIG 0x1C //AFS_SEL for bits 3 & 4; Write byte-> [X, X, X, AFS_SEL[1}, AFS_SEL[0], X, X, X]



//--------------------------------------------------------------------------------------------------------
//SETTINGS
/*
 * Based of passing selection to only bits 3&4 of config registers use hexidecimals between these
 * MIN 00000000 <= byte <= 00011000 MAX == 0 <= 1*2^4 + 1*2^3 + + 1*2^2 + + 1*2^1 + + 1*2^0 = 24

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
 * Write byte-> [X, X, X, FS_SEL[1}, FS_SEL[0], X, X, X]
 */
#define FS_SEL B00010000  //(00010000 -> +/- 1000DEG/SEC RANGE)

/*
 * +/- 8g/sec range, this is based off the fact the props max power ~= 2*weight of craft, 
 * and unaccounted for circular motion accelerations
 * 
 * Write byte-> [X, X, X, AFS_SEL[1}, AFS_SEL[0], X, X, X]
 */
#define AFS_SEL B00010000



 /* Digital low-pass filter configuration.
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
 * 
 * CONFIG BYTE -> [ X, X, X, X, X, DLPF[2], DLPF[1], DLPF[0] ]
 */
#define DLPFSetting B00000010 //98HZ, 2.8ms update
//unsigned long Delay[8] = {980, 1900, 2800, 4800, 8300, 13400, 18600};
