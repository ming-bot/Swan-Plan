//Pin

int For_And_Rev = 16; //port2 up_for down_Rev
int Left_And_Rig = 17; //port4 up_left down_right

int Left_Thruster_pin = 9;
int Right_Thruster_pin = 10;

//Parameter Reference
double Thruster_In = 0;
double Direction_In = 0; 

double Thruster_temp = 0;
double Direction_temp = 0;

double Thruster_Out = 0;
double Direction_Out = 0;

double Test_left_in = 0;
double Test_right_in = 0;

double time_high_left = 0;
double time_high_right = 0;


double Left_Thruster = 0;
double Right_Thruster = 0;

void PWM(int PWM_Pin,double time_high_level) //100hz
{
  digitalWrite(PWM_Pin,HIGH);
  delayMicroseconds(time_high_level);
  digitalWrite(PWM_Pin,LOW);
  delayMicroseconds(10000 - time_high_level);
  }


void setup()
{  
  pinMode(For_And_Rev,INPUT);
  pinMode(Left_And_Rig,INPUT);

  pinMode(Left_Thruster_pin,OUTPUT);
  pinMode(Right_Thruster_pin,OUTPUT);

  //油门最高点确认音 2ms最高油门
//  for(int i =0;i < 110; i ++)
//  {
//    PWM(Left_Thruster_pin,2000.0);
//    PWM(Right_Thruster_pin,2000.0);
//    }
//
//  for(int i = 0;i<55;i++)
//  {
//    PWM(Left_Thruster_pin,1000.0);
//    PWM(Right_Thruster_pin,1000.0);
//    }

  Serial.begin(9600);
}
void loop()
{
  Thruster_In = pulseIn(For_And_Rev,HIGH);
  Direction_In =  pulseIn(Left_And_Rig,HIGH);
  
  Thruster_temp = map(Thruster_In,994.0,1980.0,1000.0,2000.0);
  Direction_temp = map(Direction_In,980.0,1970.0,1000.0,2000.0);
  
  Thruster_Out = Thruster_temp / 200.0;
  Direction_Out = Direction_temp / 200.0;

  Left_Thruster = Thruster_Out + 7.5 - Direction_Out;
  Right_Thruster = Thruster_Out + Direction_Out - 7.5;
  
  if(Left_Thruster > 10)
    Left_Thruster = 10;

  if(Right_Thruster > 10)
    Right_Thruster = 10;
  
  if(Left_Thruster < 5)
    Left_Thruster = 5;

  if(Right_Thruster < 5)
    Right_Thruster = 5;
    
  time_high_left = 200 * Left_Thruster;
  time_high_right = 200 * Right_Thruster;
  
  PWM(Left_Thruster_pin,time_high_left);
  PWM(Right_Thruster_pin,time_high_right);
 
  
  Serial.print(" Left_high_time_1 = ");
  Serial.print( Left_Thruster );
  Serial.print(" Right_high_time_2 = ");
  Serial.println(Right_Thruster);
 
}
