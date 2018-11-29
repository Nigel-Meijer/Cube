#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

uint32_t tickCounter = 0;
uint32_t delayCounter = 0;
void SysTick_Handler(void)
{
	if(delayCounter != 0)
		delayCounter--;
	tickCounter++;
}

void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
  while (1)
  {
  }
}

void MemManage_Handler(void)
{
  while (1)
  {
  }
}

void BusFault_Handler(void)
{
  while (1)
  {
  }
}

void UsageFault_Handler(void)
{
  while (1)
  {
  }
}


void SVC_Handler(void)
{

}


void DebugMon_Handler(void)
{

}


void PendSV_Handler(void)
{

}
