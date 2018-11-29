#include "stm32f4xx.h"
#include "../Utils/delay.h"

void Led_Config();
void SPI_Config();
void latch();
void Init_Timer();
void Demo_Single_Led();

void ClearBuffer();
void TIM7_Init_CubeHandler();
void CubeBufferWriteHandler();


#define FALSE 				0
#define TRUE 				!(FALSE)

#define BUFFER_SIZE  		8 * 8

//uint8_t cubeBuffer[4][3][BUFFER_SIZE];
struct cubeFrameBuffer
{
	uint8_t cubeBuffer[4][3][BUFFER_SIZE];
};

struct Settings{
	uint8_t DisplayFrameBuffer;
	uint8_t WriteFrameBuffer;
};

struct cubeFrameBuffer Buffer[2];
struct Settings settings;

typedef enum {
	TOP,
	BOTTOM,
	NORTH,
	EAST,
	SOUTH,
	WEST
} Direction;

typedef enum{
	NONE =		0x00,
	BLUE = 		0x01,
	GREEN = 	0x02,
	LIGHTBLUE = 0x03, // green blue
	RED = 		0x04,
	PURPLE =	0x05, // Red blue
	YELLOW = 	0x06, // green red
	WHITE =		0x07
} Color;

typedef enum{
	CLEAR =		0x00,
	COPY = 		0x01
} BufferSwitchCommand;

void WritePixel(uint8_t x, uint8_t y, uint8_t z, Color color, uint8_t brightness);
void SwitchBuffer(BufferSwitchCommand command);

struct pixel
{
	uint8_t x;
	uint8_t y;
	uint8_t z;
};

int main(void)
{
	// set systick to millisecond
 	SysTick_Config(SystemCoreClock/1000);

 	// Init Frame buffers
 	settings.DisplayFrameBuffer = 0;
 	settings.WriteFrameBuffer = 0;


	// Clear the buffer
//	uint8_t var = 0;
//	for(int i = 0; i < BUFFER_SIZE/2; i++)
//	{
////		cubeBuffer[0][i] = 0xFF; // Red
////		cubeBuffer[1][i] = 0xFF; // green
////		cubeBuffer[2][i] = 0x0F; // blue
//
//		cubeBuffer[0][i] = 0xE2; // Red
//		cubeBuffer[1][i] = 0x74; // green
//		cubeBuffer[2][i] = 0x3A; // blue
//		//cubeBuffer[0][BUFFER_SIZE-i] = 0xDB;
//		//cubeBuffer[1][BUFFER_SIZE-i] = 0x6D;
//		var++;
//	}
//	for(int i = BUFFER_SIZE/2; i < BUFFER_SIZE; i++)
//	{
//		cubeBuffer[0][i] = 0x0F; // Red
//		cubeBuffer[1][i] = 0xFF; // green
//		cubeBuffer[2][i] = 0xF0; // blue
//
//		//cubeBuffer[0][i] = 0xE2; // Red
//		//cubeBuffer[1][i] = 0x74; // green
//		//cubeBuffer[2][i] = 0x3A; // blue
//		//cubeBuffer[0][BUFFER_SIZE-i] = 0xDB;
//		//cubeBuffer[1][BUFFER_SIZE-i] = 0x6D;
//		var++;
//	}
//	for(int i = 0; i < 24; i++)
//	{
//		//cubeBuffer[0][i] = 0xf0; // Red
//		//cubeBuffer[1][i] = 0xff; // Red
//		//cubeBuffer[2][i] = 0xff; // Red
//	}

//	for(int x = 4; x < 8; x++)
//	{
//		for(int i = 0; i < 8; i++)
//		{
//			WritePixel(x,i,i,GREEN);
//			//WritePixel(x,7-i,i,RED);
//		}
//	}
	//cubeBuffer[0][0] = 0xFF;
//	WritePixel(0, 0, 2, BLUE);
//
//	WritePixel(2, 1, 0, RED);
//	WritePixel(4, 2, 0, GREEN);
//	cubeBuffer[1][33] = 0x55; // green


	Led_Config();
	SPI_Config();
	Init_Timer();
	TIM7_Init_CubeHandler();


	ClearBuffer();



//	WritePixel(0,0,0,RED,15);
//	WritePixel(1,0,0,RED,14);
//	WritePixel(2,0,0,RED,13);
//	WritePixel(3,0,0,RED,12);
//	WritePixel(4,0,0,RED,11);
//	WritePixel(5,0,0,RED,10);
//	WritePixel(6,0,0,RED,9);
//	WritePixel(7,0,0,RED,8);
//	WritePixel(0,1,0,RED,7);
//	WritePixel(1,1,0,RED,6);
//	WritePixel(2,1,0,RED,5);
//	WritePixel(3,1,0,RED,4);
//	WritePixel(4,1,0,RED,3);
//	WritePixel(5,1,0,RED,2);
//	WritePixel(6,1,0,RED,1);
//	WritePixel(7,1,0,RED,0);


uint8_t varc = 0;
  while (1)
  {
	  static uint8_t countvar = 0;
	  static uint8_t updown = 1;

	  //ClearBuffer();
	  for (uint8_t x = 0; x < 8; ++x) {
		  for (uint8_t y = 0; y < 8; ++y) {
			  for (uint8_t z = 0; z < 8; ++z) {
				  WritePixel(x,y,z,RED,15);
			  }

		  }
	  }
	  //WritePixel(0,0,0,RED,countvar);
	  DelayInMs(100);
	  //cubeBuffer[0][varc++] = 0xFF; // Red

	  if(updown == 1)
	  {
		  countvar++;
		  if(countvar == 16)
			  updown = 0;
  	  }
  	  else
  	  {
  		  countvar--;
		  if(countvar == 0)
			  updown = 1;
	  }
	  SwitchBuffer(CLEAR);
  }
}

void SwitchBuffer(BufferSwitchCommand command)
{
	// First switch the buffers
	uint8_t temp = settings.DisplayFrameBuffer;
	settings.WriteFrameBuffer =settings.DisplayFrameBuffer;
	settings.DisplayFrameBuffer = temp;

	switch (command) {
		case CLEAR:

			//ClearBuffer();
			break;
		case COPY:
			//memcpy(Buffer[settings.WriteFrameBuffer].cubeBuffer, Buffer[settings.DisplayFrameBuffer].cubeBuffer, 10);
			break;
		default:
			break;
	}
}


void Demo_Sweep_In_From(Direction side, Color color)
{
	static uint8_t step = 0;
	switch (side) {
		case TOP:
			for (uint8_t y = 0; y < 8; ++y) {
				for (uint8_t x = 0; x < 8; ++x) {
					for (uint8_t z = 0; z < 8; ++z) {
						WritePixel(x,y,z,RED,15);
					}
				}
				DelayInMs(100);
			}
			break;
		default:
			break;
	}
}

void WritePixel(uint8_t x, uint8_t y, uint8_t z, Color color, uint8_t brightness)
{
	if(x > 7) return;
	if(y > 7) return;
	if(z > 7) return;
	if(brightness > 15) return;

	uint8_t Z1 = (0x01 << z);
	uint8_t column = y*8+x;

	if((color & RED) == RED)
	{
		for (uint8_t i = 0; i < 4; ++i) {
			uint8_t test = (1 << i);
			uint8_t test2 = brightness & (1 << i);
			if(test == test2)
			{
				Buffer[settings.WriteFrameBuffer].cubeBuffer[i][0][column] |= Z1;
			}
		}
	}
	if((color & GREEN) == GREEN)
	{
		Buffer[settings.WriteFrameBuffer].cubeBuffer[0][1][column] |= Z1;
	}
	if((color & BLUE) == BLUE)
	{
		Buffer[settings.WriteFrameBuffer].cubeBuffer[0][2][column] |= Z1;
	}
}

void Demo_Single_Led()
{
	static int ledCurrent = 0;
	static int colorCurrent = 0;

	if(ledCurrent != 512)
		ledCurrent++;
	else
	{
		ledCurrent = 0;
		if(colorCurrent != 2)
			colorCurrent++;
		else
			colorCurrent = 0;
	}

	uint8_t tempshift = ledCurrent % 8;
	uint8_t ledToDisplay = (0x01 << tempshift);
	uint8_t selectedColumn = ledCurrent / 8;

	ClearBuffer();
	Buffer[settings.WriteFrameBuffer].cubeBuffer[0][colorCurrent][selectedColumn] = ledToDisplay;
}

void ClearBuffer()
{
	for(int buffer = 0; buffer < 4; buffer++)
	{
		for(int i = 0; i < BUFFER_SIZE; i++)
		{
			Buffer[settings.WriteFrameBuffer].cubeBuffer[buffer][0][i] = 0x00;
			Buffer[settings.WriteFrameBuffer].cubeBuffer[buffer][1][i] = 0x00;
			Buffer[settings.WriteFrameBuffer].cubeBuffer[buffer][2][i] = 0x00;
		}
	}
}


void CubeBufferWriteHandler()
{
    /////////////////////////
	static uint8_t cycle = 1;
	uint8_t bufferToRead = 0;

    if (cycle == 1) {
    	bufferToRead = 0;
    }
    else if ((cycle == 2 || cycle == 3)) {
    	bufferToRead = 1;
    }
    else if (cycle >= 4 && cycle <= 7) {
    	bufferToRead = 2;
    }
    else if (cycle >= 8 && cycle <= 15) {
    	bufferToRead = 3;
    }
	///////////////////////////

	static uint8_t currentLayer = 0;

	for(int i = 0; i < 26; i++)
	{
		SPI_I2S_SendData(SPI1,0x00);
		while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) == RESET);
	}
	latch();
	for (int i = 0; i < 600; ++i) {;	}
		SPI_I2S_SendData(SPI1,0x01 << currentLayer);
		while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) == RESET);
		SPI_I2S_SendData(SPI1,0x01 << currentLayer);
		while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) == RESET);
		for(int color = 0; color < 3; color++)
		{
			for(int i = 0; i < 8; i++)
			{
				SPI_I2S_SendData(SPI1, Buffer[settings.DisplayFrameBuffer].cubeBuffer[bufferToRead][color][BUFFER_SIZE-1- (currentLayer*8) -i ]);
				while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) == RESET);
			}
		}
		latch();

	currentLayer++;
	if(currentLayer > 7)
	{
		currentLayer = 0;
		cycle++;
		if(cycle > 15)
			cycle =1;
	}
}

void latch()
{
	GPIO_SetBits(GPIOA, GPIO_Pin_4);
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);

	//for(int i = 0; i< 16120; i++) {;} // 100hz
	//for(int i = 0; i< 21000; i++) {;} // 75hz
	//for(int i = 0; i< 25000; i++) {;} // 62.5hz
	//for(int i = 0; i< 32500; i++) {;} // 50hz
	//for(int i = 0; i< 12000000; i++) {;} // 1sec per laag
}

void Led_Config()
{
	GPIO_InitTypeDef GPIO_InitDef;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

	GPIO_InitDef.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
	//Initialize pins
	GPIO_Init(GPIOG, &GPIO_InitDef);

	GPIO_SetBits(GPIOG, GPIO_Pin_13 | GPIO_Pin_14);
}

void SPI_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	//Initialize pins
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;

	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_Init(SPI1, &SPI_InitStructure);

	SPI_Cmd(SPI1, ENABLE);
}

void TIM2_IRQHandler()
{
    // Checks whether the TIM2 interrupt has occurred or not
    if (TIM_GetITStatus(TIM2, TIM_IT_Update))
    {
        // Toggle orange LED (GPIO13)
        //GPIO_ToggleBits(GPIOG, GPIO_Pin_13);
        //Demo_Single_Led();

        // Clears the TIM2 interrupt pending bit
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}


void Init_Timer()
{
	 // Enable clock for TIM2
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	    // TIM2 initialization overflow every 500ms
	    // TIM2 by default has clock of 84MHz
	    // Here, we must set value of prescaler and period,
	    // so update event is 0.5Hz or 500ms
	    // Update Event (Hz) = timer_clock / ((TIM_Prescaler + 1) *
	    // (TIM_Period + 1))
	    // Update Event (Hz) = 84MHz / ((4199 + 1) * (9999 + 1)) = 2 Hz
	    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	    TIM_TimeBaseInitStruct.TIM_Prescaler = 4199;
	    TIM_TimeBaseInitStruct.TIM_Period = 500; //org 9999
	    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;

	    // TIM2 initialize
	    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	    // Enable TIM2 interrupt
	    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	    // Start TIM2
	    TIM_Cmd(TIM2, ENABLE);

	    // Nested vectored interrupt settings
	    // TIM2 interrupt is most important (PreemptionPriority and
	    // SubPriority = 0)
	    NVIC_InitTypeDef NVIC_InitStruct;
	    NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	    NVIC_Init(&NVIC_InitStruct);
}

void TIM7_Init_CubeHandler()
{ // Init on 1600HZ
	 // Enable clock for TIM7
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

	    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	    TIM_TimeBaseInitStruct.TIM_Prescaler = 10;
	    TIM_TimeBaseInitStruct.TIM_Period = 656;
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

void TIM7_IRQHandler()
{
    // Checks whether the TIM7 interrupt has occurred or not
    if (TIM_GetITStatus(TIM7, TIM_IT_Update))
    {
    	GPIO_SetBits(GPIOG, GPIO_Pin_13);
    	CubeBufferWriteHandler();
  	  	GPIO_ResetBits(GPIOG, GPIO_Pin_13);

        // Clears the TIM2 interrupt pending bit
        TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
    }
}
