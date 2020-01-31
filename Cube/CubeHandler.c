/*
 * CubeHandler.c
 *
 *  Created on: Dec 3, 2018
 *      Author: Gebruiker
 */

#include "../Cube/CubeHandler.h"

void CubeHandlerInit()
{
	SPI1_CubeHandler_Config();
	TIM7_InitCubeHandler();
}

void TIM7_InitCubeHandler()
{ // Init on 24KHZ
  // 8 layers * 100Hz
	 // Enable clock for TIM7
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

	    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	    TIM_TimeBaseInitStruct.TIM_Prescaler = 6 - 1;
	    TIM_TimeBaseInitStruct.TIM_Period = 486 - 1;
	    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;

	    // TIM7 initialize
	    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseInitStruct);
	    // Enable TIM7 interrupt
	    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
	    // Start TIM7
	    TIM_Cmd(TIM7, ENABLE);

	    // Nested vectored interrupt settings
	    // TIM7 interrupt is most important (PreemptionPriority and
	    // SubPriority = 0)
	    NVIC_InitTypeDef NVIC_InitStruct;
	    NVIC_InitStruct.NVIC_IRQChannel = TIM7_IRQn;
	    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	    NVIC_Init(&NVIC_InitStruct);
}

void CubeBufferWriteHandler()
{
	GPIO_SetBits(GPIOE,GPIO_Pin_4);

	static uint8_t cycle = 1;
	uint8_t bufferToRead = 0;

    if (cycle == 16) {
    	bufferToRead = 0;
    }
    else if ((cycle == 8 || cycle == 24)) {
    	bufferToRead = 1;
    }
    else if (cycle == 4 || cycle == 12 || cycle == 20 || cycle == 28) {
    	bufferToRead = 2;
    }
    else if (cycle == 2 || cycle == 6 || cycle == 10 || cycle == 14 || cycle == 18 || cycle == 22 || cycle == 26 || cycle == 30) {
    	bufferToRead = 3;
    }
    else
    {
    	bufferToRead = 4;
    }


//    if (cycle == 8) {
//    	bufferToRead = 0;
//    }
//    else if ((cycle == 4 || cycle == 12)) {
//    	bufferToRead = 1;
//    }
//    else if (cycle == 2 || cycle == 6 || cycle == 10 || cycle == 14) {
//    	bufferToRead = 2;
//    }
//    else if (cycle == 1 || cycle == 3 || cycle == 5 || cycle == 7 || cycle == 9 || cycle == 11 || cycle == 13 || cycle == 15) {
//    	bufferToRead = 3;
//    }
//    else
//    {
//    	bufferToRead = 4;
//    }


	static uint8_t currentLayer = 0;

	SPI_I2S_SendData(SPI1,0x01 << currentLayer);
	SPI_I2S_SendData(SPI1,0x01 << currentLayer);

	for(int color = 0; color < 3; color++)
	{
		for(int i = 0; i < 8; i++)
		{
			SPI_I2S_SendData(SPI1, Buffer[settings.DisplayFrameBuffer].cubeBuffer[bufferToRead][color][BUFFER_SIZE-1- (currentLayer*8) -i ]);
		}
	}
	SPI1_Latch();

	currentLayer++;
	if(currentLayer > 7)
	{
		currentLayer = 0;
		cycle++;
		if(cycle > 31)
			cycle =1;
	}
	GPIO_ResetBits(GPIOE,GPIO_Pin_4);
}
