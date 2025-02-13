
#ifndef __BSP_SYSTEM_H
#define __BSP_SYSTEM_H

#include "main.h"
#include "Scheduler.h"
#include "LED.h"
#include "Key.h"
#include "stdio.h"
#include "Uart.h"

#include "usart.h"
#include "gpio.h"
#include "ADC.h"
#include "stdio.h"
#include "string.h"

extern uint8_t RxData_Handle_Flag;
extern uint8_t Rx_Data;
extern uint8_t page;
extern uint8_t key_val,key_down,key_up,key_old;
extern uint8_t ucled;       //亮灯
extern uint32_t key_tick;   //用于按键计时ms
extern uint8_t line;
extern uint16_t count1,count2;
extern uint16_t ADC_Value1;
extern uint16_t ADC_Value2;
extern float k;

#endif
