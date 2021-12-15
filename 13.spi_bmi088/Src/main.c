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
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "BMI088driver.h"
#include "control_get.h"
#include "Get_Angle.h"
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
/* Private user code ---------------------------------------------------------*/

/* USER CODE BEGIN PFP */
/* USER CODE BEGIN 0 */
//uint8_t NumberCount = 0;
//uint8_t ReceiveResult = 0;
//uint8_t receive_loop = 0;

//uint8_t high[4];//调试使用
//uint32_t capture_Buf[2] = {0,0}; //left_right
//uint32_t capture_buf[2] = {0,0};//back_front
//uint32_t capture_Buf_12[2] = {0,0}; //channal_5
//uint32_t capture_buf_12[2] = {0,0};//channal_6
uint8_t capture_Cnt_8[2] = {0,0};    
uint8_t capture_Cnt_12[2] = {0,0};
fp32 gyro[3], accel[3], temp, Euler_Angles[3];
float q[4] ;
//uint32_t high_time_left_right;		
//uint32_t high_time_back_front;
//uint32_t high_time_chanal_5;
//uint32_t high_time_channal_6;
//uint16_t left_right[2] = {0, 2000};
//uint16_t back_front[2] = {0, 2000};
//uint16_t back_front_PWM = 0;
//uint16_t left_right_pwm = 0;
//uint8_t channal_state_5 = 0;
//uint8_t channal_state_6 = 0;
//	

//void USART1_IRQHandler(void)  
//{
//    volatile uint8_t receive;
//    //receive interrupt 接收中断
//    if(huart1.Instance->SR & UART_FLAG_RXNE)
//    {
//        receive = huart1.Instance->DR;
//			  if(receive == 'b'){
//					NumberCount = 0;
//					ReceiveResult = 0;
//					receive_loop = 0;
//				}
//				else if(receive == 'e'){
//					receive_loop = 1;
//					/*
//					change the struct data code
//					*/
//				}
//				else{
//					int add_number = receive - '0';
//					int j = 1;
//					int weight = 1;
//					NumberCount ++ ;
//					for( j = 1; j < NumberCount; j ++){
//						weight = weight * 10;
//					
//					ReceiveResult += weight * add_number;
//					
//				}
//    }
//		
//    //idle interrupt 空闲中断
//    else if(huart1.Instance->SR & UART_FLAG_IDLE)
//    {
//        receive = huart1.Instance->DR;
//       // HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET);
//    }

//}




/* USER CODE END 0 */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
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
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_TIM10_Init();
  MX_TIM1_Init();
  MX_TIM8_Init();
  MX_TIM12_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	 /* USER CODE BEGIN 2 */
    HAL_TIM_Base_Start(&htim8);
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);
		HAL_TIM_Base_Start(&htim1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
		
		
//    HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET);
//    HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET);
//    HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_RESET);
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
		__HAL_TIM_SET_CAPTUREPOLARITY(&htim8, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_RISING);
		HAL_TIM_IC_Start_IT(&htim8, TIM_CHANNEL_2);
		__HAL_TIM_SET_CAPTUREPOLARITY(&htim12, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
		HAL_TIM_IC_Start_IT(&htim12, TIM_CHANNEL_1);
		__HAL_TIM_SET_CAPTUREPOLARITY(&htim12, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_RISING);
		HAL_TIM_IC_Start_IT(&htim12, TIM_CHANNEL_2);
		capture_Cnt_8[0] = 1;
		capture_Cnt_8[1] = 1;
		capture_Cnt_12[0] = 1;
		capture_Cnt_12[1] = 1;
		
    while(BMI088_init())
    {
        ;
    }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
				BMI088_read(gyro, accel, &temp);
		    MahonyAHRSupdateIMU(gyro[0], gyro[1], gyro[2], accel[0], accel[1], accel[2]);
        HAL_Delay(10);
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
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
