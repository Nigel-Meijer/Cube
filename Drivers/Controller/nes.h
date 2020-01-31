/*
 * nes.h
 *
 *  Created on: Dec 2, 2018
 *      Author: Gebruiker
 */

#ifndef DRIVERS_CONTROLLER_NES_H_
#define DRIVERS_CONTROLLER_NES_H_

#include "stm32f4xx.h"
#include "../Settings/Settings.h"
#include "../Utils/delay.h"


typedef enum {
	BUTTON_NONE		= 0x00,	// The hex value is the corresponding bit send from the NES controller.
	BUTTON_A 		= 0x80,
	BUTTON_B 		= 0x40,
	BUTTON_SELECT 	= 0x20,
	BUTTON_START	= 0x10,
	BUTTON_UP		= 0x08,
	BUTTON_DOWN		= 0x04,
	BUTTON_LEFT		= 0x02,
	BUTTON_RIGHT	= 0x01
} Buttons;


//union {
//struct
//{
//uint8_t OnOff:1; //knop1
//uint8_t Play:1; //knop2
//uint8_t Stop:1; //knop3
//uint8_t Left:1; //knop4
//uint8_t Up:1; //knop5
//uint8_t Right:1; //knop6
//uint8_t Down:1; //knop7
//uint8_t Res:1;
//};
//uint8_t byte;
//}Buttons;

typedef struct {
	uint8_t A;
	uint8_t B;
	uint8_t Select;
	uint8_t Start;
	uint8_t Up;
	uint8_t Down;
	uint8_t Left;
	uint8_t Right;
	Buttons LastButtonPressed;
	uint8_t Connected;
} Controller;

void ControllerInit();
void TIM6_InitCubeHandler();
void SPI4_Controller_Config();
void Gpio_Init_Controller();

void Read_Controller(uint8_t ControllerID);
void HandleControllerData(uint8_t Data, uint8_t controllerID);
void ClearController(uint8_t controllerID);

void TIM6_InitControllerHandler();
void TIM13_InitBitRead();
void ControllerHandler();
void ControllerBitReadHandler();

uint8_t IsButtonPressed(Buttons button, uint8_t controllerID);
uint8_t IsControllerConnected(uint8_t controllerID);
Controller * GetController(uint8_t controllerID);
Buttons GetLastPressedButton(uint8_t controllerID);
Buttons GetLastPressedButtonWithoutClear(uint8_t controllerID);
void LastPressedButtonClear(uint8_t controllerID);


#endif /* DRIVERS_CONTROLLER_NES_H_ */
