#include "nes.h"

Controller controller[NUMBER_OF_CONTROLLERS];
uint8_t currentControllerRead = 0;

void ControllerInit()
{
	// Init Controllers
	for (uint8_t i = 0; i < NUMBER_OF_CONTROLLERS; ++i) {
		ClearController(i);
	}

	// Init Spi controller 1
	//SPI4_Controller_Config();
	Gpio_Init_Controller();
	TIM6_InitControllerHandler();
	TIM13_InitBitRead();

}

void TIM6_InitControllerHandler()
{ // Init on ~240HZ
	 // Enable clock for TIM6
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

	    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	    TIM_TimeBaseInitStruct.TIM_Prescaler = 1000;
	    TIM_TimeBaseInitStruct.TIM_Period = 290;
	    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;

	    // TIM6 initialize
	    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStruct);
	    // Enable TIM6 interrupt
	    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
	    // Start TIM6
	    TIM_Cmd(TIM6, ENABLE);

	    // Nested vectored interrupt settings
	    // TIM6 interrupt is most important (PreemptionPriority and
	    // SubPriority = 0)
	    NVIC_InitTypeDef NVIC_InitStruct;
	    NVIC_InitStruct.NVIC_IRQChannel = TIM6_DAC_IRQn;
	    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	    NVIC_Init(&NVIC_InitStruct);
}

void TIM13_InitBitRead()
{
	 // Enable clock for TIM13
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);

	    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	    TIM_TimeBaseInitStruct.TIM_Prescaler = 7;
	    TIM_TimeBaseInitStruct.TIM_Period = 10 - 1;
	    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;

	    // TIM13 initialize
	    TIM_TimeBaseInit(TIM13, &TIM_TimeBaseInitStruct);
	    // Enable TIM13 interrupt
	    TIM_ITConfig(TIM13, TIM_IT_Update, ENABLE);
	    // Start TIM13
	    TIM_Cmd(TIM13, DISABLE);

	    // Nested vectored interrupt settings
	    // TIM13 interrupt is most important (PreemptionPriority and
	    // SubPriority = 0)
	    NVIC_InitTypeDef NVIC_InitStruct;
	    NVIC_InitStruct.NVIC_IRQChannel = TIM8_UP_TIM13_IRQn;
	    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	    NVIC_Init(&NVIC_InitStruct);
}

void ControllerBitReadHandler()
{
	// sequence: 0.latchup 1.latchdown 2.read 3.clockup 4.clockdown 5.read 6.clockup 7.clockdown 8.read.........
	// LatchUp 	 = 0
	// LatchDown = 1
	// Read      = 2,5, 8,11,14,17,20,23
	// ClkUp     = 3,6, 9,12,15,18,21
	// ClkDown	 = 4,7,10,13,16,19,22

	static uint8_t state = 0;
	static uint8_t bitPosition = 0;
	static uint8_t readByte = 0;

	if(state == 0) // Latch Rising
	{
		GPIOE->ODR |= GPIO_Pin_6;
	}
	else if(state == 1)	// Latch Falling
	{
		GPIOE->ODR &= ~GPIO_Pin_6;
	}
	else
	{
		uint8_t test = state % 3;
		uint8_t bit = 0;
		switch (test) {
			case 0:
				GPIOE->ODR |= GPIO_Pin_2;
				break;
			case 1:
				GPIOE->ODR &= ~GPIO_Pin_2;
				break;
			case 2:
				bit = GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5);//GPIOE->IDR & GPIO_Pin_5;
				if(bit == 1){
		}

				readByte |= (GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5) << 7-bitPosition);

				bitPosition++;
				break;
			default:
				break;
		}
	}

	state++;
	if(state > 24 - 1)
	{
		TIM_Cmd(TIM13, DISABLE);
		HandleControllerData(readByte, currentControllerRead);
		state = 0;
		bitPosition = 0;
		readByte = 0;
	}
}

void ControllerHandler()
{
	currentControllerRead++;

	if(currentControllerRead >= NUMBER_OF_CONTROLLERS)
	{
		currentControllerRead = 0;
	}

	switch (currentControllerRead) {
		case 0:
			GPIO_ResetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3);
			break;
		case 1:
			GPIO_SetBits(GPIOE, GPIO_Pin_0);
			GPIO_ResetBits(GPIOE, GPIO_Pin_1 | GPIO_Pin_3);
			break;
		case 2:
			GPIO_ResetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_3);
			GPIO_SetBits(GPIOE, GPIO_Pin_1);
			break;
		case 3:
			GPIO_ResetBits(GPIOE, GPIO_Pin_3);
			GPIO_SetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1);
			break;
		default:
			break;
	}


	TIM_Cmd(TIM13, ENABLE);
}

//void ControllerHandler()
//{
//	currentControllerRead++;
//	if(currentControllerRead >= NUMBER_OF_CONTROLLERS)
//	{
//		currentControllerRead = 0;
//	}
//	Read_Controller(currentControllerRead);
//
//}



Controller * GetController(uint8_t controllerID)
{
	return &controller[controllerID];
}

uint8_t IsControllerConnected(uint8_t controllerID)
{
	return controller[controllerID].Connected;
}

uint8_t IsButtonPressed(Buttons button, uint8_t controllerID)
{
	switch (button) {
		case BUTTON_A: 		return controller[controllerID].A; 		break;
		case BUTTON_B: 		return controller[controllerID].B; 		break;
		case BUTTON_START: 	return controller[controllerID].Start; 	break;
		case BUTTON_SELECT: return controller[controllerID].Select; break;
		case BUTTON_UP:		return controller[controllerID].Up; 	break;
		case BUTTON_DOWN:	return controller[controllerID].Down; 	break;
		case BUTTON_LEFT:	return controller[controllerID].Left; 	break;
		case BUTTON_RIGHT:	return controller[controllerID].Right; 	break;
		default:			return 0;								break;
	}
}

Buttons GetLastPressedButton(uint8_t controllerID)
{
	Buttons lastPressed = controller[controllerID].LastButtonPressed;
	LastPressedButtonClear(controllerID);
	return lastPressed;
}

Buttons GetLastPressedButtonWithoutClear(uint8_t controllerID)
{
	Buttons lastPressed = controller[controllerID].LastButtonPressed;
	return lastPressed;
}

void LastPressedButtonClear(uint8_t controllerID)
{
	controller[controllerID].LastButtonPressed = BUTTON_NONE;
}


void ClearController(uint8_t controllerID)
{
	controller[controllerID].A = 0;
	controller[controllerID].B = 0;
	controller[controllerID].Select = 0;
	controller[controllerID].Start = 0;
	controller[controllerID].Up = 0;
	controller[controllerID].Down = 0;
	controller[controllerID].Left = 0;
	controller[controllerID].Right = 0;
	controller[controllerID].Connected = 0;
	controller[controllerID].Connected = 0;
}

void HandleControllerData(uint8_t Data, uint8_t controllerID)
{
	// Data == 0 means controller not connected.
	if(Data == 0)
	{
		if(controller[controllerID].Connected == 0){
			return;
		}
		// Clear the pressed buttons.
		ClearController(controllerID);
		return;
	}

	// if button wasn't pressed before set it as last pressed
	if(!IsButtonPressed(BUTTON_A,controllerID) && !(Data & BUTTON_A))			{	controller[controllerID].LastButtonPressed = BUTTON_A;		}
	if(!IsButtonPressed(BUTTON_B,controllerID) && !(Data & BUTTON_B))			{	controller[controllerID].LastButtonPressed = BUTTON_B;		}
	if(!IsButtonPressed(BUTTON_SELECT,controllerID) && !(Data & BUTTON_SELECT))	{	controller[controllerID].LastButtonPressed = BUTTON_SELECT;	}
	if(!IsButtonPressed(BUTTON_START,controllerID) && !(Data & BUTTON_START))	{	controller[controllerID].LastButtonPressed = BUTTON_START;	}
	if(!IsButtonPressed(BUTTON_UP,controllerID) && !(Data & BUTTON_UP))			{	controller[controllerID].LastButtonPressed = BUTTON_UP;		}
	if(!IsButtonPressed(BUTTON_DOWN,controllerID) && !(Data & BUTTON_DOWN))		{	controller[controllerID].LastButtonPressed = BUTTON_DOWN;	}
	if(!IsButtonPressed(BUTTON_LEFT,controllerID) && !(Data & BUTTON_LEFT))		{	controller[controllerID].LastButtonPressed = BUTTON_LEFT;	}
	if(!IsButtonPressed(BUTTON_RIGHT,controllerID) && !(Data & BUTTON_RIGHT))	{	controller[controllerID].LastButtonPressed = BUTTON_RIGHT;	}

	// Update the current pressed buttons.
	controller[controllerID].A = 		(Data & BUTTON_A) ? 0 : 1;
	controller[controllerID].B = 		(Data & BUTTON_B) ? 0 : 1;
	controller[controllerID].Select = 	(Data & BUTTON_SELECT) ? 0 : 1;
	controller[controllerID].Start = 	(Data & BUTTON_START) ? 0 : 1;
	controller[controllerID].Up = 		(Data & BUTTON_UP) ? 0 : 1;
	controller[controllerID].Down = 	(Data & BUTTON_DOWN) ? 0 : 1;
	controller[controllerID].Left = 	(Data & BUTTON_LEFT) ? 0 : 1;
	controller[controllerID].Right = 	(Data & BUTTON_RIGHT) ? 0 : 1;

	// Data is received so controller is connected
	controller[controllerID].Connected = 1;
}

void Gpio_Init_Controller()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	// Init controller enable pins.
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_6 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOE, &GPIO_InitStructure);


	GPIO_ResetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6);


//	// Init controller enable pins.
//	GPIO_InitTypeDef GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(GPIOE, &GPIO_InitStructure);
//
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
//	GPIO_Init(GPIOE, &GPIO_InitStructure);
//
//	GPIO_ResetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_6);
}

void SPI4_Controller_Config()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	// INIT SPI on ~0.5MHZ

	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI4, ENABLE);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	//Initialize pins
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOE, GPIO_PinSource2, GPIO_AF_SPI4);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_SPI4);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;

	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_Init(SPI4, &SPI_InitStructure);

	// Enable receive interrupt
	SPI_I2S_ITConfig(SPI4, SPI_I2S_IT_RXNE, ENABLE);

	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	  NVIC_InitStructure.NVIC_IRQChannel = SPI4_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&NVIC_InitStructure);

	SPI_Cmd(SPI4, ENABLE);
}

void SPI4_IRQHandler()
{
	if(SPI_I2S_GetITStatus(SPI4,SPI_I2S_IT_RXNE) == SET)
	{
		uint8_t receivedData = SPI_I2S_ReceiveData(SPI4);
		HandleControllerData(receivedData,currentControllerRead);

		SPI_I2S_ClearITPendingBit(SPI4,SPI_I2S_IT_RXNE);
	}
}

void Read_Controller(uint8_t ControllerID)
{
	//TODO AddCode here to multiplex controller signals.
	switch (ControllerID) {
		case 0:
			GPIO_ResetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3);
			break;
		case 1:
			GPIO_SetBits(GPIOE, GPIO_Pin_0);
			GPIO_ResetBits(GPIOE, GPIO_Pin_1 | GPIO_Pin_3);
			break;
		case 2:
			GPIO_ResetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_3);
			GPIO_SetBits(GPIOE, GPIO_Pin_1);
			break;
		case 3:
			GPIO_ResetBits(GPIOE, GPIO_Pin_3);
			GPIO_SetBits(GPIOE, GPIO_Pin_0 | GPIO_Pin_1);
			break;
		default:
			break;
	}

	//Latch
	GPIO_SetBits(GPIOE,GPIO_Pin_6);
	GPIO_ResetBits(GPIOE,GPIO_Pin_6);

	//Send 1byte clock pulse.
	SPI_I2S_SendData(SPI4, 0x00);
}
