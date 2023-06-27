/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
/* Definitions for blink01 */
osThreadId_t blink01Handle;
const osThreadAttr_t blink01_attributes = {
  .name = "blink01",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for blink02 */
osThreadId_t blink02Handle;
const osThreadAttr_t blink02_attributes = {
  .name = "blink02",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

void StartBlink01(void *argument);
void StartBlink02(void *argument);

//GPIO Initialization
void configureLED(void)
{
	RCC->AHB1ENR |=(1UL<<3);
	GPIOD->MODER &= ~(0xFFUL<<12*2);
	GPIOD->MODER |= (0x55UL<<12*2);
}
void msDelay(int msTime)
{
	//Assume for loop take 12 clock cycles and system clock is 16MHz
int Time=msTime*1333;
	for(int i=0;i<Time;i++);
}
int main(void)
{

  HAL_Init();
  configureLED();

  osKernelInitialize();
  blink01Handle = osThreadNew(StartBlink01, NULL, &blink01_attributes);

  /* creation of blink02 */
  blink02Handle = osThreadNew(StartBlink02, NULL, &blink02_attributes);

  /* Start scheduler */
  osKernelStart();

  while (1)
  {
  }
}



void StartBlink01(void *argument)
{

	  for(;;)
	   {
		 GPIOD->ODR ^= (0x1UL<<12);
		 msDelay(2000);
	     osDelay(2000);
	   }

}

/* USER CODE END Header_StartBlink02 */
void StartBlink02(void *argument)
{

  for(;;)
  {
	  GPIOD->ODR ^= (0x2UL<<12);;
	  osDelay(200);
  }

}



