#include <IMU.h>

class MYIMU{
public:

  cIMU IMU;
  uint8_t  err_code;
  
  double Read_Angle()
  { 
    double yawangle = 0;
    static uint32_t tTime[3];
    static uint32_t imu_time = 0;
    
//    Serial.print(" ");
  
    if( (millis()-tTime[0]) >= 500 )
    {
      tTime[0] = millis();
    }

    tTime[2] = micros();
    if( IMU.update() > 0 ) imu_time = micros()-tTime[2];

    if( (millis()-tTime[1]) >= 0 )
    {
      tTime[1] = millis();
      yawangle = IMU.rpy[2];

    }

    if( Serial.available() )
    {
      char Ch = Serial.read();

      if( Ch == '1' )
      {
//        Serial.println("ACC Cali Start");
        IMU.SEN.acc_cali_start();
        while( IMU.SEN.acc_cali_get_done() == false )
        {
          IMU.update();
        }
//        Serial.print("ACC Cali End ");
      }
    }

    return yawangle;
    }
};
