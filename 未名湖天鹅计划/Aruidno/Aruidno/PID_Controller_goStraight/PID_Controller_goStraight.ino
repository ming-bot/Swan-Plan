//在使用时候不要用Serial,否则OPenCR会出问题
#include "OPENCR_IMU.h"
#include "Thruster.h"
#include <PID_v1.h>
//SetOutLimits(0,255)
//SampleTime = 100ms
//Output > 0 右边推进器加
//Output < 0 左边推进器加

int LED_Pin = 8;
char Read;

//IMU Declaration
//顺时针变小，逆时针变大
MYIMU OPENCR_IMU;

double Setpoint = 0;
double Input = 0;
double Output = 0;
double Kp = 1;
double Ki = 0;
double Kd = 0;


PID Straight_PID(&Input,&Output,&Setpoint,Kp,Ki,Kd,DIRECT);


void PWM(int PWM_Pin,double time_high_level) //100hz
{
  digitalWrite(PWM_Pin,HIGH);
  delayMicroseconds(time_high_level);
  digitalWrite(PWM_Pin,LOW);
  delayMicroseconds(10000 - time_high_level);
  }


void setup() {
  //PID Part
  Straight_PID.SetMode(AUTOMATIC);
  Straight_PID.SetOutputLimits(-40,40);
  pinMode(For_and_Rev_pin, INPUT);
  pinMode(Left_and_Right_pin,INPUT);

  pinMode(Left_Thruster_pin, OUTPUT);
  pinMode(Right_Thruster_pin, OUTPUT);
  pinMode(LED_Pin,OUTPUT);

//  for (int i =0;i < 220;i++)
//  {
//    PWM(Left_Thruster_pin,2000);
//    PWM(Right_Thruster_pin,2000);
//    }
//
//   for (int i =0;i < 120;i++)
//  {
//    PWM(Left_Thruster_pin,1000);
//    PWM(Right_Thruster_pin,1000);
//    }
  
  Serial.begin(9600);
  OPENCR_IMU.IMU.begin();
  // put your setup code here, to run once:

}

void Set_Setpoint(double & point,double dir)
{
  switch(int(dir))
  {
    case 1:
      point = point + 0.5;
      break;
    case 2:
      point = point - 0.5;
      break;
    case 0:
      point = point;
      break;
    }
  }


//int Left_Thruster_pin = 9;
//int Right_Thruster_pin = 10;
void Send_PWM(double PWM_OUT,double Output)
{
  Read = Serial.read();
  if(Read == 'O')
  {
    digitalWrite(LED_Pin,HIGH);
   }
  else if(Read == 'S')
   {
    digitalWrite(LED_Pin,LOW);
   }
   
  if(Output < 0)
  {
    PWM(Right_Thruster_pin,PWM_OUT);
    PWM(Left_Thruster_pin,PWM_OUT + 10*abs(Output));
//    Serial.print(PWM_OUT);
//    Serial.print(" ");
//    Serial.print(PWM_OUT + 10 *abs(Output));
    }

  else if(Output > 0)
  {
    PWM(Left_Thruster_pin,PWM_OUT);
    PWM(Right_Thruster_pin,PWM_OUT + 10*abs(Output));
//    Serial.print(PWM_OUT);
//    Serial.print(" ");
//    Serial.print(PWM_OUT + 10 *abs(Output));
    }
  else
    {
      PWM(Left_Thruster_pin,PWM_OUT);
      PWM(Right_Thruster_pin,PWM_OUT);
//      Serial.print(PWM_OUT);
//      Serial.print(" ");
//      Serial.print(PWM_OUT + 10 * Output);
      }
  }

void loop() {
//  double PWM_Left = 0;
//  double PWM_Right = 0;

   
  double PWM_OUT_LEFT = 0;
  double PWM_OUT_RIGHT = 0;
  double* PWM_value = new double[2];
  PWM_value = Thruster();
  Input = OPENCR_IMU.Read_Angle();
  Set_Setpoint(Setpoint,PWM_value[1]);
  Straight_PID.Compute();
  Send_PWM(PWM_value[0],Output);
//  Serial.print("Input YawAngle: ");
//  Serial.print(Input);
//  Serial.print(" ");
//  Serial.print(Setpoint);
//  Serial.print(" ");
//  Serial.println(Output);
  delete [] PWM_value;
  // put your main code here, to run repeatedly:

}
