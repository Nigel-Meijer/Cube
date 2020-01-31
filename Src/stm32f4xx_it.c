#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

#include "../Cube/CubeHandler.h"
#include "../Controller/nes.h"

uint32_t tick = 0;
uint32_t delayCounter = 0;
void SysTick_Handler(void)
{
	if(delayCounter != 0)
		delayCounter--;
	tick++;
}

void TIM7_IRQHandler()
{
    // Checks whether the TIM7 interrupt has occurred or not
    if (TIM_GetITStatus(TIM7, TIM_IT_Update))
    {
    	CubeBufferWriteHandler();

        // Clears the TIM7 interrupt pending bit
        TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
    }
}

void TIM6_DAC_IRQHandler()
{
    // Checks whether the TIM7 interrupt has occurred or not
    if (TIM_GetITStatus(TIM6, TIM_IT_Update))
    {
    	ControllerHandler();

        // Clears the TIM7 interrupt pending bit
        TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
    }
}

void TIM8_UP_TIM13_IRQHandler()
{
    // Checks whether the TIM7 interrupt has occurred or not
    if (TIM_GetITStatus(TIM13, TIM_IT_Update))
    {
    	ControllerBitReadHandler();

        // Clears the TIM7 interrupt pending bit
        TIM_ClearITPendingBit(TIM13, TIM_IT_Update);
    }

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
