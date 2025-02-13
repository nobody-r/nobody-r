#include "bsp_system.h"



void LED_Disp(uint8_t ucled)
{
	HAL_GPIO_WritePin(GPIOC,0xff<<8,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);//全部熄灭
	
	HAL_GPIO_WritePin (GPIOC,ucled<<8,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
}

void LED_Toggle(uint8_t ucled)
{
	
	HAL_GPIO_TogglePin(GPIOC,ucled<<8);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
}

void LED(void)
{
	LED_Disp(0x01);
}
