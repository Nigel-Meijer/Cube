/*
 * Snake.h
 *
 *  Created on: Dec 3, 2018
 *      Author: Gebruiker
 */

#ifndef SNAKE_SNAKE_H_
#define SNAKE_SNAKE_H_

#include "stm32f4xx.h"
#include "../Settings/Settings.h"
#include "../Controller/nes.h"
#include "../Cube/cube.h"
#include "math.h"
#include "../StateMachine/StateMachine.h"

#define MAX_SNAKE_SIZE  		512
#define MAX_NUMBER_OF_APPLES  	100

typedef struct Position{
	uint8_t x;
	uint8_t y;
	uint8_t z;
} Position;

typedef struct Apple{
	Position position;
	uint8_t active;
} Apples;

struct Tail{
	struct Position position;
	struct Tail *prev;
};

struct StartData{
	struct Tail startPosition;
	Direction startDirection;
	Color color;
};

struct Queue {
	struct Tail *front;
	struct Tail *back;
};

struct Player{
	float movementSpeed;
	float timeUntilNextMovement;
	Direction direction;
	Color color;
	struct Position position;

	struct Queue queue;
	struct Tail tail[MAX_SNAKE_SIZE];
	uint8_t isPlaying;
	uint8_t isDead;
} player[4];

void SnakeInit();
void SnakeLoop();
void SnakeLogic();
void DrawSnakes();
void DrawApples();
void PlayersInit();
void PopPush(uint8_t playerID, uint8_t x, uint8_t y, uint8_t z, uint8_t extend);
uint8_t CheckCollisionWithWall(uint8_t playerID,int x,int y,int z);
uint8_t CheckCollisionWithPlayers(uint8_t playerID,int x,int y,int z);
uint8_t CheckCollisionWithApple(uint8_t playerID,int x,int y,int z);
void KillPlayer(uint8_t playerID);
void CreateApple(uint8_t ammount);
void ClearApples();
uint32_t extern tick;



#endif /* SNAKE_SNAKE_H_ */
