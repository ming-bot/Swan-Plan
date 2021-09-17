/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t NumberCount = 0;
uint8_t ReceiveResult = 0;
uint8_t receive_loop = 0;

uint8_t high[4];//调试使用

uint32_t capture_Buf[3] = {0}; 
uint8_t capture_Cnt_8 = 0;    
uint8_t capture_Cnt_12 = 0;
uint32_t high_time_left_right;		
uint32_t high_time_back_front;
uint16_t left_right[2] = {0, 2000};
uint16_t back_front[2] = {0, 2000};
uint16_t back_front_PWM = 0;
uint16_t left_right_pwm = 0;
	
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	
	if(TIM12 == htim->Instance)
	{
		switch(capture_Cnt_12){
			case 1:
				capture_Buf[0] = HAL_TIM_ReadCapturedValue(&htim12,TIM_CHANNEL_1);
				__HAL_TIM_SET_CAPTUREPOLARITY(&htim12,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING); 
				capture_Cnt_12++;
				break;
			case 2:
				capture_Buf[1] = HAL_TIM_ReadCapturedValue(&htim12,TIM_CHANNEL_1);
				HAL_TIM_IC_Stop_IT(&htim12,TIM_CHANNEL_1); 
			if(capture_Buf[1]>capture_Buf[0])
				high_time_back_front = capture_Buf[1] - capture_Buf[0];
						
			if(high_time_back_front > back_front[0])
			{
				back_front[0] = high_time_back_front;
			}
			if(high_time_back_front < back_front[1])
			{
				back_front[1] = high_time_back_front;
			}
			
			back_front_PWM = 1300 + (500/(back_front[0] - back_front[1])) * (high_time_back_front - back_front[1]);
			__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1,back_front_PWM);
			__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2,back_front_PWM);
			
			uint32_t high_time = high_time_back_front;
			high[3] = high_time%10+'0';
			high_time /=10;
			high[2] = high_time%10+'0';
			high_time /=10;
			high[1] = high_time%10+'0';
			high_time /=10;
			high[0] = high_time%10+'0';
			high_time /=10;
			HAL_UART_Transmit(&huart1, high,4, 0x100);
			HAL_UART_Transmit(&huart1, "\r\n",2, 0x100);
			__HAL_TIM_SET_CAPTUREPOLARITY(&htim12, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
			HAL_TIM_IC_Start_IT(&htim12, TIM_CHANNEL_1);
			capture_Cnt_12=1; 
		}
	}
		
		if(TIM8 == htim->Instance)
	{
		switch(capture_Cnt_8){
			case 1:
				capture_Buf[0] = HAL_TIM_ReadCapturedValue(&htim8,TIM_CHANNEL_1);
				__HAL_TIM_SET_CAPTUREPOLARITY(&htim8,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING); 
				capture_Cnt_8++;
				break;
			case 2:
				capture_Buf[1] = HAL_TIM_ReadCapturedValue(&htim8,TIM_CHANNEL_1);
				HAL_TIM_IC_Stop_IT(&htim8,TIM_CHANNEL_1); 
			if(capture_Buf[1]>capture_Buf[0])
				high_time_left_right = capture_Buf[1] - capture_Buf[0];
			if(high_time_left_right>1800)
			{
				high_time_left_right = 1800;
			}
			 else if(high_time_left_right<1200)
			{
				high_time_left_right = 1200;
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
				__HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_3,left_right_pwm);
				__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2,left_right_pwm);

			
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
			HAL_UART_Transmit(&huart1, "    ",4, 0x100);
			
			
			 high_time =left_right[1];
			high[3] = high_time%10+'0';
			high_time /=10;
			high[2] = high_time%10+'0';
			high_time /=10;
			high[1] = high_time%10+'0';
			high_time /=10;
			high[0] = high_time%10+'0';
			high_time /=10;
			HAL_UART_Transmit(&huart1, high,4, 0x100);
			HAL_UART_Transmit(&huart1, "    ",4, 0x100);
			
			high_time =left_right[0];
			high[3] = high_time%10+'0';
			high_time /=10;
			high[2] = high_time%10+'0';
			high_time /=10;
			high[1] = high_time%10+'0';
			high_time /=10;
			high[0] = high_time%10+'0';
			high_time /=10;
			HAL_UART_Transmit(&huart1, high,4, 0x100);
			HAL_UART_Transmit(&huart1, "    ",4, 0x100);
			
				high_time =high_time_left_right;
			high[3] = high_time%10+'0';
			high_time /=10;
			high[2] = high_time%10+'0';
			high_time /=10;
			high[1] = high_time%10+'0';
			high_time /=10;
			high[0] = high_time%10+'0';
			high_time /=10;
			HAL_UART_Transmit(&huart1, high,4, 0x100);
			HAL_UART_Transmit(&huart1, "    ",4, 0x100);
			
			HAL_UART_Transmit(&huart1, "\r\n",2, 0x100);
			__HAL_TIM_SET_CAPTUREPOLARITY(&htim8, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
			HAL_TIM_IC_Start_IT(&htim8, TIM_CHANNEL_1);
			capture_Cnt_8=1; 
		}
	
	}
	
}


void USART1_IRQHandler(void)  
{
    volatile uint8_t receive;
    //receive interrupt 接收中断
    if(huart1.Instance->SR & UART_FLAG_RXNE)
    {
        receive = huart1.Instance->DR;
			  if(receive == 'b'){
					NumberCount = 0;
					ReceiveResult = 0;
					receive_loop = 0;
				}
				else if(receive == 'e'){
					receive_loop = 1;
					/*
					change the struct data code
					*/
				}
				else{
					int add_number = receive - '0';
					int j = 1;
					int weight = 1;
					NumberCount ++ ;
					for( j = 1; j < NumberCount; j ++){
						weight = weight * 10;
					}
					ReceiveResult += weight * add_number;
					
				}
    }
		
    //idle interrupt 空闲中断
    else if(huart1.Instance->SR & UART_FLAG_IDLE)
    {
        receive = huart1.Instance->DR;
        HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET);
    }

}





/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_TIM8_Init();
  MX_TIM12_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
    HAL_TIM_Base_Start(&htim8);
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);
		HAL_TIM_Base_Start(&htim1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
		
		
    HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_RESET);
    //enable receive interrupt and idle interrupt
    //使能接收中断和空闲中断
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);  //receive interrupt
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);  //idle interrupt

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	//get begin,open the rasing take;开始检测上升沿
	__HAL_TIM_SET_CAPTUREPOLARITY(&htim8, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
		HAL_TIM_IC_Start_IT(&htim8, TIM_CHANNEL_1);
		__HAL_TIM_SET_CAPTUREPOLARITY(&htim12, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
		HAL_TIM_IC_Start_IT(&htim12, TIM_CHANNEL_1);
		capture_Cnt_8 = 1;
		capture_Cnt_12 = 1;
		//
		

	  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
        //send data by usart
        //串口发送数据
//        HAL_UART_Transmit(&huart1, "RoboMaster\r\n", 12, 100);
//        HAL_Delay(100);
//        HAL_UART_Transmit(&huart6, "RoboMaster\r\n", 12, 100);
//        HAL_Delay(100);
//		__HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_3,left_right_pwm);
//				__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2,left_right_pwm);
			HAL_Delay(100);
			HAL_UART_Transmit(&huart1, "shdksdjks\r\n", 12, 100);
		
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 6;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
