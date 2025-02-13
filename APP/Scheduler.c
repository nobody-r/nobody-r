#include "Scheduler.h"

typedef struct 
{
	void (*task_func)(void);
	uint32_t rate_ms;
	uint32_t last_run;
}	task_t;

uint8_t task_num;


static task_t task_Schedule[]=
{
	
	{LED,0,0}
	
};

void scheduler_Init(void)
{
	task_num=sizeof(task_Schedule)/sizeof(task_t);
	
}

void scheduler_Run(void)
{
	for(uint8_t i=0;i<task_num;i++)
	{
		uint32_t now_time=HAL_GetTick();
		if(now_time>=task_Schedule[i].rate_ms+task_Schedule[i].last_run)
		{
			task_Schedule[i].last_run=now_time;
			task_Schedule[i].task_func();
				
		}
	}
	
	
}
