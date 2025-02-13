#include "bsp_system.h"

uint8_t High=0,Low=0;
void RxData_Handle(void)
{
	if(RxData_Handle_Flag==1)
	{
		k=High-'0'+(Low-'0')/10.0;
		RxData_Handle_Flag=0;
		printf("ok\n");
	}
	else	
		return;
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef*huart)
{
	static uint8_t State=0;
		
		if(huart==&huart1)
	{
		
		if(State==0)
		{
			if(Rx_Data=='k')
				State++;
		}
		else if(State==1)
		{
			High=Rx_Data;
			State++;			
		}
		else if(State==2)
		{
			if(Rx_Data=='.')
				State++;
		}
		else if(State==3)
		{
			Low=Rx_Data;
			State++;
		}
		else if(State==4)
		{
			if(Rx_Data=='\n')
			{
				RxData_Handle_Flag=1;
				State=0;
			}
		}
		HAL_UART_Receive_IT(&huart1,&Rx_Data,1);
	}

}


