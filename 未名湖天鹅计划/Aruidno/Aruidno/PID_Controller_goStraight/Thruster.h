#ifndef THRUSTER_H_
#define THRUSTER_H_

int For_and_Rev_pin = 16;
int Left_and_Right_pin = 17; //Setpoint increasing or decreasing

int Left_Thruster_pin = 9;
int Right_Thruster_pin = 10;

double* Thruster()
{
   //Parameters
  double Thruster_temp = 0;
  double Direction_temp = 0;
//  double Direction_temp = 0;

  double Thruster_Out = 0;
  double Direction_Out = 0;
//  double Direction_Out = 0;

//  double Test_left_in = 0;
//  double Test_right_in = 0;

//  double Left_Thruster = 0;
//  double Right_Thruster = 0;

  double* Return_Value = new double[2];
 
//  Thruster_In = pulseIn(For_And_Rev,HIGH);
//  Direction_In =  pulseIn(Left_And_Rig,HIGH);
  
  Thruster_temp = map(pulseIn(For_and_Rev_pin,HIGH),994.0,1970.0,1000.0,2000.0);
//  Direction_temp = map(pulseIn(Left_and_Right_pin,HIGH),994.0,1970.0,1000.0,2000.0);
  Direction_temp = pulseIn(Left_and_Right_pin,HIGH);

  
//  Serial.print(Direction_temp);
  
  Thruster_Out = Thruster_temp / 200.0;
//  Direction_Out = Direction_temp / 200.0;

//  Left_Thruster = Thruster_Out + 7.5 - Direction_Out;
//  Right_Thruster = Thruster_Out + Direction_Out - 7.5;
  
//  if(Left_Thruster > 10)
//    Left_Thruster = 10;
//
//  if(Right_Thruster > 10)
//    Right_Thruster = 10;
//  
//  if(Left_Thruster < 5)
//    Left_Thruster = 5;
//
//  if(Right_Thruster < 5)
//    Right_Thruster = 5;

  if( Thruster_Out > 10)
    Thruster_Out = 10;
  if(Thruster_Out < 5)
    Thruster_Out = 5;
    
  Return_Value[0] = 200 * Thruster_Out;
  
  if( Direction_temp > 1500)
    Return_Value[1] = 1;    //Left
  else if(Direction_temp < 1460) 
    Return_Value[1] = 2;    //Right
  else
    Return_Value[1] = 0;    //keep
  
  return Return_Value;

  delete[] Return_Value;
  }


#endif
