/*
 * CubeHandler.h
 *
 *  Created on: Dec 3, 2018
 *      Author: Gebruiker
 */

#ifndef CUBE_CUBEHANDLER_H_
#define CUBE_CUBEHANDLER_H_

#include "stm32f4xx.h"
#include "../Settings/Settings.h"
#include "../Drivers/SPI/Spi1-CubeHandler.h"

void CubeHandlerInit();
void TIM7_InitCubeHandler();
void CubeBufferWriteHandler();

#endif /* CUBE_CUBEHANDLER_H_ */
