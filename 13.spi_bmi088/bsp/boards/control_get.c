#include"control_get.h"
#include "main.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "control_get.h"


uint8_t NumberCount = 0;
uint8_t ReceiveResult = 0;
uint8_t receive_loop = 0;

uint8_t high[4];//µ˜ ‘ π”√

uint32_t capture_Buf[2] = {0,0}; //left_right
uint32_t capture_buf[2] = {0,0};//back_front
uint32_t capture_Buf_12[2] = {0,0}; //channal_5
uint32_t capture_buf_12[2] = {0,0};//channal_6
extern uint8_t capture_Cnt_8[2];    
extern uint8_t capture_Cnt_12[2];
uint32_t high_time_left_right;		
uint32_t high_time_back_front;
uint32_t high_time_chanal_5;
uint32_t high_time_channal_6;
uint16_t left_right[2] = {0, 2000};
uint16_t back_front[2] = {0, 2000};
uint16_t back_front_PWM = 0;
uint16_t left_right_pwm = 0;

uint8_t channal_state_5 = 0;
uint8_t channal_state_6 = 0;
	

void task1()
{
	uint16_t x = back_front_PWM-1550;
	uint16_t y = left_right_pwm-1550;
	if(x<0)
		x=0;
	uint16_t left = 1300+x*2+y*2;
	uint16_t right = 1300+x*3-y*2;
	if(left>1800)
		left = 1800;
	if(left<1300)
		left = 1300;
	if(right>1800)
		right = 1800;
	if(right<1300)
		right = 1300;
	__HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_3,right);
	__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2,left);
	
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	
	if(TIM12 == htim->Instance)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
			switch(capture_Cnt_12[0]){
				case 1:
					capture_Buf_12[0] = HAL_TIM_ReadCapturedValue(&htim12,TIM_CHANNEL_1);
					__HAL_TIM_SET_CAPTUREPOLARITY(&htim12,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING); 
					capture_Cnt_12[0]++;
					break;
				case 2:
					capture_Buf_12[1] = HAL_TIM_ReadCapturedValue(&htim12,TIM_CHANNEL_1);
					HAL_TIM_IC_Stop_IT(&htim12,TIM_CHANNEL_1); 
				if(capture_Buf_12[1]>capture_Buf_12[0])
					high_time_back_front = capture_Buf_12[1] - capture_Buf_12[0];
				if(high_time_back_front<600)
				{
					channal_state_5 = 0;
				}
				else if(high_time_back_front<800)
				{
					channal_state_5 = 1;
				}
				else
				{					
					channal_state_5 = 2;
			}
//				uint32_t high_time = high_time_back_front;
			uint32_t high_time = channal_state_5;
				high[3] = high_time%10+'0';
				high_time /=10;
				high[2] = high_time%10+'0';
				high_time /=10;
				high[1] = high_time%10+'0';
				high_time /=10;
				high[0] = high_time%10+'0';
				high_time /=10;
					HAL_UART_Transmit(&huart1, "                     ",16, 0x100);
				HAL_UART_Transmit(&huart1, high,4, 0x100);
				
				HAL_UART_Transmit(&huart1, "\r\n",2, 0x100);
				__HAL_TIM_SET_CAPTUREPOLARITY(&htim12, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
				HAL_TIM_IC_Start_IT(&htim12, TIM_CHANNEL_1);
				capture_Cnt_12[0]=1; 
			}
		}
	
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
		{
			switch(capture_Cnt_12[1]){
				case 1:
					capture_buf_12[0] = HAL_TIM_ReadCapturedValue(&htim12,TIM_CHANNEL_2);
					__HAL_TIM_SET_CAPTUREPOLARITY(&htim12,TIM_CHANNEL_2,TIM_ICPOLARITY_FALLING); 
					capture_Cnt_12[1]++;
					break;
				case 2:
					capture_buf_12[1] = HAL_TIM_ReadCapturedValue(&htim12,TIM_CHANNEL_2);
					HAL_TIM_IC_Stop_IT(&htim12,TIM_CHANNEL_2); 
				if(capture_buf_12[1]>capture_buf_12[0])
					high_time_channal_6 = capture_buf_12[1] - capture_buf_12[0];
				if(high_time_channal_6<600)
				{
					channal_state_6 = 0;
				}
				else
					{
					channal_state_6 = 1;
					}
				if(channal_state_6 ==1)
				{
					left_right[0] = 0;
					left_right[1] = 2000;
					 back_front[0] = 0;
					back_front[1] = 2000;
				}
				//uint32_t high_time = high_time_channal_6;
				uint32_t high_time = channal_state_6;
				high[3] = high_time%10+'0';
				high_time /=10;
				high[2] = high_time%10+'0';
				high_time /=10;
				high[1] = high_time%10+'0';
				high_time /=10;
				high[0] = high_time%10+'0';
				high_time /=10;
					HAL_UART_Transmit(&huart1, "                   ",20, 0x100);
				HAL_UART_Transmit(&huart1, high,4, 0x100);
				HAL_UART_Transmit(&huart1, "\r\n",2, 0x100);
				__HAL_TIM_SET_CAPTUREPOLARITY(&htim12, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_RISING);
				HAL_TIM_IC_Start_IT(&htim12, TIM_CHANNEL_2);
				capture_Cnt_12[1]=1; 
			}
		}
	}
		
		if(TIM8 == htim->Instance)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
			switch(capture_Cnt_8[0]){
				case 1:
					capture_Buf[0] = HAL_TIM_ReadCapturedValue(&htim8,TIM_CHANNEL_1);
					__HAL_TIM_SET_CAPTUREPOLARITY(&htim8,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING); 
					capture_Cnt_8[0]++;
					break;
				case 2:
					capture_Buf[1] = HAL_TIM_ReadCapturedValue(&htim8,TIM_CHANNEL_1);
					HAL_TIM_IC_Stop_IT(&htim8,TIM_CHANNEL_1); 
				if(capture_Buf[1]>capture_Buf[0])
					high_time_left_right = capture_Buf[1] - capture_Buf[0];
				if(high_time_left_right>2100)
				{
					high_time_left_right = 2100;
				}
				 else if(high_time_left_right<900)
				{
					high_time_left_right = 900;
				}
				if(high_time_left_right>left_right[0])
				{
					left_right[0] = high_time_left_right;
				}
				if(high_time_left_right<left_right[1])
				{
					left_right[1] = high_time_left_right;
				}
				left_right_pwm = 1300 + (500* (high_time_left_right - left_right[1]))/(left_right[0] - left_right[1]);
				
				task1();
				//__HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_3,left_right_pwm);
				//__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2,left);
				
				uint32_t high_time =left_right_pwm;
				high[3] = high_time%10+'0';
				high_time /=10;
				high[2] = high_time%10+'0';
				high_time /=10;
				high[1] = high_time%10+'0';
				high_time /=10;
				high[0] = high_time%10+'0';
				high_time /=10;
				HAL_UART_Transmit(&huart1, high,4, 0x100);
				//HAL_UART_Transmit(&huart1, "    ",4, 0x100);
				
				
//				 high_time =left_right[1];
//				high[3] = high_time%10+'0';
//				high_time /=10;
//				high[2] = high_time%10+'0';
//				high_time /=10;
//				high[1] = high_time%10+'0';
//				high_time /=10;
//				high[0] = high_time%10+'0';
//				high_time /=10;
//				HAL_UART_Transmit(&huart1, high,4, 0x100);
//				HAL_UART_Transmit(&huart1, "    ",4, 0x100);
//				
//				high_time =left_right[0];
//				high[3] = high_time%10+'0';
//				high_time /=10;
//				high[2] = high_time%10+'0';
//				high_time /=10;
//				high[1] = high_time%10+'0';
//				high_time /=10;
//				high[0] = high_time%10+'0';
//				high_time /=10;
//				HAL_UART_Transmit(&huart1, high,4, 0x100);
//				HAL_UART_Transmit(&huart1, "    ",4, 0x100);
//				
//					high_time =high_time_left_right;
//				high[3] = high_time%10+'0';
//				high_time /=10;
//				high[2] = high_time%10+'0';
//				high_time /=10;
//				high[1] = high_time%10+'0';
//				high_time /=10;
//				high[0] = high_time%10+'0';
//				high_time /=10;
//				HAL_UART_Transmit(&huart1, high,4, 0x100);
//				HAL_UART_Transmit(&huart1, "    ",4, 0x100);
//				
				HAL_UART_Transmit(&huart1, "\r\n",2, 0x100);
				__HAL_TIM_SET_CAPTUREPOLARITY(&htim8, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
				HAL_TIM_IC_Start_IT(&htim8, TIM_CHANNEL_1);
				capture_Cnt_8[0]=1; 
			}
		}
		
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
		{
			switch(capture_Cnt_8[1]){
				case 1:
					capture_buf[0] = HAL_TIM_ReadCapturedValue(&htim8,TIM_CHANNEL_2);
					__HAL_TIM_SET_CAPTUREPOLARITY(&htim8,TIM_CHANNEL_2,TIM_ICPOLARITY_FALLING); 
					capture_Cnt_8[1]++;
					break;
				case 2:
					capture_buf[1] = HAL_TIM_ReadCapturedValue(&htim8,TIM_CHANNEL_2);
					HAL_TIM_IC_Stop_IT(&htim8,TIM_CHANNEL_2); 
				if(capture_buf[1]>capture_buf[0])
					high_time_back_front = capture_buf[1] - capture_buf[0];
				if(high_time_back_front>2100)
				{
					high_time_back_front = 2100;
				}
				 else if(high_time_back_front<900)
				{
					high_time_back_front = 900;
				}
				if(high_time_back_front>back_front[0])
				{
					back_front[0] = high_time_back_front;
				}
				if(high_time_back_front<back_front[1])
				{
					back_front[1] = high_time_back_front;
				}
				back_front_PWM = 1300 + (500* (high_time_back_front - back_front[1]))/(back_front[0] - back_front[1]);
				
				task1();
				//__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2,back_front_PWM);
				
				uint32_t high_time =back_front_PWM;
				high[3] = high_time%10+'0';
				high_time /=10;
				high[2] = high_time%10+'0';
				high_time /=10;
				high[1] = high_time%10+'0';
				high_time /=10;
				high[0] = high_time%10+'0';
				high_time /=10;
				HAL_UART_Transmit(&huart1, "             ",10, 0x100);
				HAL_UART_Transmit(&huart1, high,4, 0x100);
//				HAL_UART_Transmit(&huart1, "    ",4, 0x100);
//				
//				
//				 high_time =back_front[1];
//				high[3] = high_time%10+'0';
//				high_time /=10;
//				high[2] = high_time%10+'0';
//				high_time /=10;
//				high[1] = high_time%10+'0';
//				high_time /=10;
//				high[0] = high_time%10+'0';
//				high_time /=10;
//				HAL_UART_Transmit(&huart1, high,4, 0x100);
//				HAL_UART_Transmit(&huart1, "    ",4, 0x100);
//				
//				high_time =back_front[0];
//				high[3] = high_time%10+'0';
//				high_time /=10;
//				high[2] = high_time%10+'0';
//				high_time /=10;
//				high[1] = high_time%10+'0';
//				high_time /=10;
//				high[0] = high_time%10+'0';
//				high_time /=10;
//				HAL_UART_Transmit(&huart1, high,4, 0x100);
//				HAL_UART_Transmit(&huart1, "    ",4, 0x100);
//				
//					high_time =high_time_back_front;
//				high[3] = high_time%10+'0';
//				high_time /=10;
//				high[2] = high_time%10+'0';
//				high_time /=10;
//				high[1] = high_time%10+'0';
//				high_time /=10;
//				high[0] = high_time%10+'0';
//				high_time /=10;
//				HAL_UART_Transmit(&huart1, high,4, 0x100);
//				HAL_UART_Transmit(&huart1, "    ",4, 0x100);
				
				HAL_UART_Transmit(&huart1, "\r\n",2, 0x100);
				__HAL_TIM_SET_CAPTUREPOLARITY(&htim8, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_RISING);
				HAL_TIM_IC_Start_IT(&htim8, TIM_CHANNEL_2);
				capture_Cnt_8[1]=1; 
			}
		}
	 }
}



