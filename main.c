/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bsp_system.h"
#include "i2c.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
RTC_TimeTypeDef RTC_time;
RTC_DateTypeDef RTC_date;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint8_t key_val=0,key_down=0,key_up=0,key_old=0;
uint32_t tick=0;        //计时ms
uint8_t ucled=0x00;
char Lcd_buff[21];
char tx_buff[20];
uint8_t Rx_Data=0;

uint8_t RxData_Handle_Flag=0;
uint8_t Tx_Flag=0;

uint8_t page=0;         //页面
uint8_t line=0;
uint16_t count1=0,count2=0;

uint8_t Led_State=1;	//默认on

uint16_t ADC_Value1;
uint16_t ADC_Value2;

uint16_t Fre1,Fre2;
uint8_t duty1=10,duty2=10;

uint8_t hour=0,minute=0,second=0;

float k=0.1;
float Voltage=0;
float Set_k=0;

char Led_Text[4]="ON";
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

int fputc(int ch, FILE *f) {
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
	return ch;
}

	
void LCD_default_configuration(void)
{
	//LCD_Clear(Black);
	LCD_SetBackColor(Black);
	LCD_SetTextColor(White);
}

void LCD_Highlight(void)
{
  LCD_SetBackColor(Green);
	LCD_SetTextColor(White);
}

void LCD_Proc(void)
{
	LCD_default_configuration();
  if (page==0)
  {
		
		if(count1==0)
		{
			LCD_Clear(Black);
			count1=1;
			count2=0;
		}
    LCD_default_configuration();
	 
			sprintf(Lcd_buff,"  V1:%.2f",Voltage);
		 LCD_DisplayStringLine(Line3,(uint8_t*)Lcd_buff);
			sprintf(Lcd_buff,"  k:%.2f",k);
		 LCD_DisplayStringLine(Line4,(uint8_t*)Lcd_buff);
		
	if(Led_State==0)
	sprintf(Led_Text,"OFF");
	else
	sprintf(Led_Text,"ON ");
	
			sprintf(Lcd_buff,"  LED:%s",Led_Text);
			LCD_DisplayStringLine(Line5,(uint8_t*)Lcd_buff);
			sprintf(Lcd_buff,"  T:%02d-%02d-%02d",hour,minute,second);
			LCD_DisplayStringLine(Line6,(uint8_t*)Lcd_buff);
		
  }
	if(page==1)
	{
		if(count2==0)
		{
			LCD_Clear(Black);
			count2=1;
			count1=0;
		}														//清屏
		LCD_default_configuration();
			sprintf(Lcd_buff,"       Setting");
			LCD_DisplayStringLine(Line2,(uint8_t*)Lcd_buff);
		if(line==0)
			LCD_Highlight();
		else
			LCD_default_configuration();
			
			sprintf(Lcd_buff,"%02d",hour);
			LCD_DisplayString(Line4,319-80,(uint8_t*)Lcd_buff);
		
		LCD_default_configuration();
		sprintf(Lcd_buff,"-");
			LCD_DisplayString(Line4,319-112,(uint8_t*)Lcd_buff);
		
		if(line==1)
			LCD_Highlight();
		 else
			 LCD_default_configuration();

			sprintf(Lcd_buff,"%02d",minute);
			LCD_DisplayString(Line4,319-128,(uint8_t*)Lcd_buff);
		 
			LCD_default_configuration();
		 sprintf(Lcd_buff,"-");
			LCD_DisplayString(Line4,319-160,(uint8_t*)Lcd_buff);
		 
		 if(line==2)
			 LCD_Highlight();
		 else
			 LCD_default_configuration();

		 sprintf(Lcd_buff,"%02d",second);
			LCD_DisplayString(Line4,319-176,(uint8_t*)Lcd_buff);
		
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
	uint8_t byteData[4];
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

	LCD_Init();
	scheduler_Init();
	LCD_Clear(Black);

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_TIM16_Init();
  MX_TIM17_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */

	HAL_UART_Receive_IT(&huart1,&Rx_Data,1);
	
	HAL_ADCEx_Calibration_Start(&hadc1,ADC_SINGLE_ENDED);
	HAL_ADCEx_Calibration_Start(&hadc2,ADC_SINGLE_ENDED);
	
	I2CInit();
	
		k=EEPROM_Read(1)/10.0;
		//memcpy(&k, byteData, sizeof(float));
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		
		HAL_RTC_GetTime(&hrtc,&RTC_time,RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc,&RTC_date,RTC_FORMAT_BIN);
		HAL_ADC_Start(&hadc2);
		//Key_Long_Short_Process();
		//Key_Double_Single_Process();
		Key_Process();
		RxData_Handle();
		
		if(key_down==1)
		{
			if(Led_State==1)
				Led_State=0;
			else
				Led_State=1;
		}
		
		else if(key_down==2)
		{
			page=(page+1)%2;
		}
		else if(key_down==3)
		{
			if(page==1)
			line=(line+1)%3;
		}
		else if(key_down==4)
		{
			if(page==1)
			{
			if(line==0)
				hour=(hour+1)%25;
			else if(line==1)
				minute=(minute+1)%61;
			else if(line==2)
				second=(second+1)%61;
			}
		}
		
	ADC_Value1=HAL_ADC_GetValue(&hadc2);
	Voltage=ADC_Value1*3.3/4096;	
		LCD_Proc();
	
		if(Voltage>3.3*k)
		{
			if(Led_State==1)
			{
				if(tick<=200)
				LED_Disp(0x01);
				else if(tick>200&&tick<400)
					LED_Disp(0x00);
				else if(tick>=400)
					tick=0;
			}
		}
		else 
			LED_Disp(0x00);
		if(Led_State==0)
			LED_Disp(0x00);

		
		
		if(RTC_time.Hours==hour&&RTC_time.Minutes==minute&&RTC_time.Seconds==second)
		{
			Tx_Flag=1;
		}
		if(Tx_Flag==1)
		{
printf("%.2f+%.1f+%02d%02d%02d\n",Voltage,k,hour,minute,second);
		Tx_Flag=0;
			HAL_Delay(500);
		}
		//memcpy(byteData, &k, sizeof(float));
		EEPROM_Write(1,k*10);
		
  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
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
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV3;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
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
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
