#include "bsp_system.h"

uint8_t key_flag=0;
uint8_t key_temp=0;
uint32_t key_tick=0;	//按键计时


uint8_t Key_Scan(void)
{
	uint8_t Key_Val=0;
	
	if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)==GPIO_PIN_RESET)
    Key_Val=1;
   
	if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)==GPIO_PIN_RESET)
    Key_Val=2;
    
	if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)==GPIO_PIN_RESET)
    Key_Val=3;
	
	if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==GPIO_PIN_RESET)
    Key_Val=4;
	
	return Key_Val;
	
}

void Key_Long_Short_Process(void)
{
    
	key_val=Key_Scan();
    key_down=key_val&(key_val^key_old);
    key_up=~key_val&(key_val^key_old);
    key_old=key_val;
		
	if(key_down)
		key_tick=0;

	if(key_tick<700)	//短按
	{
		if(key_up==1)
		{

		}
		
		if(key_up==2)
		{
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET);
		}
		if(key_up==3)
		{


		}
		
		if(key_up==4)
		{
			
		}
	}
	
	else				//长按
	{
		if(key_up==1)
		{
		
		}
		
		if(key_up==2)
		{
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_RESET);
		}
		if(key_up==3)
		{


		}
		
		if(key_up==4)
		{
			
		}	
	}
}

void Key_Process(void)
{
	
	key_val=Key_Scan();
    key_down=key_val&(key_val^key_old);
    key_up=~key_val&(key_val^key_old);
    key_old=key_val;
}

void Key_Double_Single_Process(void)
{
		
	
	key_val=Key_Scan();
    key_down=key_val&(key_val^key_old);
    key_up=~key_val&(key_val^key_old);
    key_old=key_val;
	
	if(key_up)
	{
		key_temp=key_up;
		if(key_flag==0)
		{
			key_tick=0;
			key_flag=1;
		}
		else
		{
			key_flag=0;			//按下两次后开启下次计时
		}
		
	}
	if(key_flag==1)
	{
		if(key_tick<350)
		{
			if(key_down==1&&key_temp==1)		//双击
			{
				
			}
			else if(key_down==2&&key_temp==2)
			{
				
				
			}
			else if(key_down==3&&key_temp==3)
			{

			}
			else if(key_down==4&&key_temp==4)
			{

			}
		}
		else
		{
			if(key_temp==1)					//单击
			{
				page=(page+1)%3;
			}
			if(key_temp==2)
			{
				if(page==1||page==2)
				line=(line+1)%2;
			}
			if(key_temp==3)
			{
				if(page==3)
				if(line==0)
					printf("Hellw Wrold");
				
			}
			if(key_temp==4)
			{

			}
			key_flag=0;				//开启下次计时
		}
	}
	
}
