/*
 * Snake.c
 *
 *  Created on: Dec 3, 2018
 *      Author: Gebruiker
 */

#include "Snake.h"
#include "stdlib.h"

struct StartData start[4] = {
		{ .startDirection = DIR_NORTH, .startPosition.position.x = 3, .startPosition.position.y = 3, .startPosition.position.z = 0, .color = RED},
		{ .startDirection = DIR_SOUTH, .startPosition.position.x = 4, .startPosition.position.y = 3, .startPosition.position.z = 7, .color = GREEN},
		{ .startDirection = DIR_EAST, .startPosition.position.x = 0, .startPosition.position.y = 3, .startPosition.position.z = 4, .color = BLUE},
		{ .startDirection = DIR_WEST, .startPosition.position.x = 7, .startPosition.position.y = 3, .startPosition.position.z = 3, .color = YELLOW},
};

Apples apples[MAX_NUMBER_OF_APPLES];

void PopPush(uint8_t playerID, uint8_t x, uint8_t y, uint8_t z, uint8_t extend)
{
	struct Tail *prev = player[playerID].queue.front;
	player[playerID].queue.front = player[playerID].queue.front + 1;
	player[playerID].queue.front->position.x = x;
	player[playerID].queue.front->position.y = y;
	player[playerID].queue.front->position.z = z;
	player[playerID].queue.front->prev = prev;

	if(!extend)
	{
		player[playerID].queue.back = player[playerID].queue.back + 1;
		player[playerID].queue.back->prev = NULL;
	}
}

void DrawSnakes()
{
	// Loop Through the players
	for (int playerID = 0; playerID < NUMBER_OF_CONTROLLERS; ++playerID) {
		// Skip if player is dead or not playing.
		if(player[playerID].isPlaying == 1 && player[playerID].isDead == 0)
		{
			// Is player connected
			if(IsControllerConnected(playerID))
			{
				int tailLength = 0;
				struct Tail *tail = player[playerID].queue.front;
				// Loop through snake and draw it.
				while(tail != NULL)
				{
					uint8_t brightness = (tailLength) ? 15 : 31;
					WritePixel(tail->position.x, tail->position.y, tail->position.z, player[playerID].color, brightness);
					tail = tail->prev;
					tailLength++;
				}
			}
		}
	}
}

void PlayersInit()
{
	// Make sure the controllers are updated once before running this init.
	DelayInMs(50);

	settings.PrevUpdateTick = tick;

	for (int playerID = 0; playerID < NUMBER_OF_CONTROLLERS; ++playerID) {
		player[playerID].color 					= start[playerID].color;
		player[playerID].direction 				= start[playerID].startDirection;
		player[playerID].movementSpeed 			= 2;
		player[playerID].timeUntilNextMovement 	= 1 / player[playerID].movementSpeed;
		player[playerID].queue.front 			= player[playerID].tail;
		player[playerID].queue.back 			= player[playerID].tail;
		player[playerID].queue.back->prev 		= 0;
		player[playerID].tail[0] 				= start[playerID].startPosition;

		player[playerID].position				= start[playerID].startPosition.position;
		player[playerID].isDead 				= 0;

		// If controller is connected we assume they are playing
		if(IsControllerConnected(playerID))
			player[playerID].isPlaying = 1;
		else
			player[playerID].isPlaying = 0;
	}

}

void SnakeLogic()
{
	// Time that has past since last update
	float timeSinceLastUpdate = (float)(tick - settings.PrevUpdateTick)/1000;

	for (uint8_t playerID = 0; playerID < NUMBER_OF_CONTROLLERS; ++playerID) {
		// Skip if player is dead or not playing.
		if(player[playerID].isPlaying == 1 && player[playerID].isDead == 0)
		{
			//Update time for when player needs to move again.
			player[playerID].timeUntilNextMovement -= timeSinceLastUpdate;

			if(player[playerID].timeUntilNextMovement < 0)
			{
				//Update the player direction.
				Buttons button = GetLastPressedButton(playerID);
				if(button != BUTTON_NONE)
				{
					switch (button) {
						case BUTTON_UP: 	player[playerID].direction = DIR_NORTH; break;
						case BUTTON_LEFT: 	player[playerID].direction = DIR_WEST;	break;
						case BUTTON_DOWN: 	player[playerID].direction = DIR_SOUTH; break;
						case BUTTON_RIGHT:  player[playerID].direction = DIR_EAST;	break;
						case BUTTON_A:		player[playerID].direction = DIR_DOWN;	break;
						case BUTTON_B:  	player[playerID].direction = DIR_UP;	break;
						default: break;
					}
				}

				// Player has moved reset the "timeUntilNextMovement"
				player[playerID].timeUntilNextMovement = 1 / player[playerID].movementSpeed;

				// Update the player position.
				int x = 0, y = 0, z = 0;
				switch (player[playerID].direction) {
					case DIR_NORTH: z = 1; break;
					case DIR_SOUTH: z = -1; break;
					case DIR_EAST:  x = 1;	break;
					case DIR_WEST: 	x = -1;	break;
					case DIR_DOWN:	y = 1;	break;
					case DIR_UP:  	y = -1;	break;
					default: break;
				}


				// Check for if player hit wall.
				if(CheckCollisionWithWall(playerID, x, y, z))
				{
					KillPlayer(playerID);
				}

				if(CheckCollisionWithPlayers(playerID, x, y, z))
				{
					KillPlayer(playerID);
				}

				// If player is dead there is no need to add the new position.
				if(player[playerID].isDead == FALSE)
				{
					uint8_t extend = 0;

					if(State_GetCurrentState() == STATE_TRON)
					{
						extend = 1;
					}
					else if(State_GetCurrentState() == STATE_SNAKE)
					{
						extend = CheckCollisionWithApple(playerID, x, y, z);
					}




					// Update the snake.
					PopPush(	playerID,
								floor(player[playerID].queue.front->position.x + x),
								floor(player[playerID].queue.front->position.y + y),
								floor(player[playerID].queue.front->position.z + z),
								extend );
				}
			}
		}
	}
}

uint8_t CheckCollisionWithWall(uint8_t playerID,int x,int y,int z)
{
	int x1 = player[playerID].queue.front->position.x + x;
	int y1 = player[playerID].queue.front->position.y + y;
	int z1 = player[playerID].queue.front->position.z + z;

	if(x1 > 7 || x1 < 0) { return 1; }
	if(y1 > 7 || y1 < 0) { return 1; }
	if(z1 > 7 || z1 < 0) { return 1; }

	return 0;
}
uint8_t CheckCollisionWithPlayers(uint8_t playerID,int x,int y,int z)
{

	Position newPos = { .x = player[playerID].queue.front->position.x + x,
						.y = player[playerID].queue.front->position.y + y,
						.z = player[playerID].queue.front->position.z + z};

	for (uint8_t otherPlayer = 0; otherPlayer < NUMBER_OF_CONTROLLERS; ++otherPlayer) {

		if(player[otherPlayer].isDead == 0 && player[otherPlayer].isPlaying == 1)
		{
			struct Tail *tail = player[otherPlayer].queue.front;
			// Loop through snake and check for collision.
			while(tail != NULL)
			{
				if(newPos.x == tail->position.x && newPos.y == tail->position.y && newPos.z == tail->position.z)
				{
					return 1;
				}
				tail = tail->prev;
			}
		}
	}
	return 0;
}

uint8_t CheckCollisionWithApple(uint8_t playerID,int x,int y,int z)
{
	Position newPos = { .x = player[playerID].queue.front->position.x + x,
						.y = player[playerID].queue.front->position.y + y,
						.z = player[playerID].queue.front->position.z + z};

	for (int i = 0; i < MAX_NUMBER_OF_APPLES; ++i) {
		if(apples[i].active == TRUE)
		{
			if(newPos.x == apples[i].position.x && newPos.y == apples[i].position.y && newPos.z == apples[i].position.z)
			{
				// Disable apple.
				apples[i].active = 0;
				CreateApple(1);
				return 1;
			}
		}
	}
	return 0;
}

void CreateApple(uint8_t ammount)
{
	for (int i = 0; i < ammount; ++i)
	{
		uint16_t SelectedApple = 999;
		for (int i = 0; i < MAX_NUMBER_OF_APPLES; ++i) {
			if(apples[i].active == FALSE)
			{
				SelectedApple = i;
				break;
			}
		}

		if(SelectedApple != 999)
		{
			apples[SelectedApple].position.x= rand() % 8;
			apples[SelectedApple].position.y= rand() % 8;
			apples[SelectedApple].position.z= rand() % 8;
			apples[SelectedApple].active = 1;
		}
	}
}

void ClearApples()
{
	for (int i = 0; i < MAX_NUMBER_OF_APPLES; ++i) {
		apples[i].active = 0;
	}
}

void DrawApples()
{

	uint8_t r = rand() % 16;
	uint8_t g = rand() % 16;
	uint8_t b = rand() % 16;

	for (int i = 0; i < MAX_NUMBER_OF_APPLES; ++i) {
		if(apples[i].active == TRUE)
		{
			WritePixel(apples[i].position.x ,apples[i].position.y, apples[i].position.z, RED, r);
			WritePixel(apples[i].position.x ,apples[i].position.y, apples[i].position.z, GREEN, g);
			WritePixel(apples[i].position.x ,apples[i].position.y, apples[i].position.z, BLUE, b);
		}
	}
}

void KillPlayer(uint8_t playerID)
{
	player[playerID].isDead = 1;
}

void SnakeInit()
{
	PlayersInit();
	ClearApples();
	if(State_GetCurrentState() == STATE_SNAKE)
	{
		CreateApple(2);
	}
}

void SnakeLoop()
{
	SnakeLogic();
	DrawSnakes();
	DrawApples();
	if(settings.debugMode)
	{
		if(player[0].isDead)
			WritePixel(4,0,0,WHITE,15);
		if(player[1].isDead)
			WritePixel(5,0,0,WHITE,15);
		if(player[2].isDead)
			WritePixel(6,0,0,WHITE,15);
		if(player[3].isDead)
			WritePixel(7,0,0,WHITE,15);
	}
	settings.PrevUpdateTick = tick;
}
