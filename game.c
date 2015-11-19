//Connor Lindquist
#include <stdlib.h>
#include <stdio.h>
#include "myLib.h"
#include "game.h"
#include "font.h"
#include "start_screen.h"
#include "game_over.h"
#include "mario.h"
#include "coin.h"
#include "bomb.h"
#include "mushroom.h"

static int speed;
static int frame;
static char scoreString[41];
static char livesString[41];
static char speedString[41];
static char playAgain[41];

int playerSpeed = 1;
int currentScore=0;
int lives = 3;
int state;

PLAYER currentPlayer;
ITEM spawned[];

void refresh() {
	switch(state) {
	case TITLE:
		frame++;
		break;
	case NORMAL:
		if (frame == speed) {
			frame = 0;
		} else {
			frame++;
		}
		break;
	case GAMEOVER:
		frame++;
		break;
	}
}

//Clears entities on screen and shows end screen
void endGame() {
	state = GAMEOVER;
	for(int i=0; i<=10; i++) {
		ITEM* a = &spawned[i];
		a->x=SCREEN_WIDTH*2;
		a->y=SCREEN_HEIGHT*2;
		a->alive = 0;
	}
	currentPlayer.x = SCREEN_WIDTH*2;
	currentPlayer.y = SCREEN_HEIGHT*2;
	draw();
	drawRect3(0,0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);
	drawImage3(0, 0, GAME_OVER_WIDTH, GAME_OVER_HEIGHT, game_over);
	sprintf(scoreString, "Final Score: %d", currentScore);
	drawString(SCREEN_HEIGHT/2 - 15, SCREEN_WIDTH/2 - 45, scoreString, WHITE);
	sprintf(playAgain, "Press 'select' to play again");
	drawString(SCREEN_HEIGHT/2, SCREEN_WIDTH/2 - 85, playAgain, WHITE);
}

//Initializes score strings, entities
void startGame() {
	state = NORMAL;
	currentPlayer.x = SCREEN_HEIGHT - MARIO_HEIGHT - BOTTOM_BUFFER - 5;
	currentPlayer.y = SCREEN_WIDTH/2 - 20;
	drawRect3(0, 0, 240, 160, CYAN);
	drawRect3(SCREEN_HEIGHT - BOTTOM_BUFFER, 0 , SCREEN_WIDTH, BOTTOM_BUFFER, BLACK);
	drawImage3(currentPlayer.x, currentPlayer.y, MARIO_WIDTH, MARIO_HEIGHT, mario);
	sprintf(scoreString, "Score: %d", currentScore);
	drawString(SCREEN_HEIGHT - BOTTOM_BUFFER + BOTTOM_BUFFER/5, 5, scoreString, GREEN);
	sprintf(livesString, "Lives: %d", lives);
	drawString(SCREEN_HEIGHT - BOTTOM_BUFFER + BOTTOM_BUFFER/5, SCREEN_WIDTH - 60, livesString, GREEN);
	sprintf(speedString, "Speed: %d", playerSpeed);
	drawString(SCREEN_HEIGHT - BOTTOM_BUFFER + BOTTOM_BUFFER/5, SCREEN_WIDTH/3 + 20, speedString, GREEN);
	srand(frame);
	frame = 0;
	initSpawned();
	spawnCoin();
	spawnMushroom();
}

void reset() {
	state = TITLE;
	currentScore = 0;
	lives = 3;
	playerSpeed = 1;
	drawImage3(0, 0, START_SCREEN_WIDTH, START_SCREEN_HEIGHT, start_screen);
}

//Moves mario left
void moveLeft() {
	drawRect3(currentPlayer.x, currentPlayer.y, MARIO_WIDTH, MARIO_HEIGHT, CYAN);
	currentPlayer.y = currentPlayer.y - playerSpeed;
	drawImage3(currentPlayer.x , currentPlayer.y, MARIO_WIDTH, MARIO_HEIGHT, mario);
}

//Moves mario right
void moveRight() {
	drawRect3(currentPlayer.x, currentPlayer.y, MARIO_WIDTH, MARIO_HEIGHT, CYAN);
	currentPlayer.y = currentPlayer.y + playerSpeed;
	drawImage3(currentPlayer.x , currentPlayer.y, MARIO_WIDTH, MARIO_HEIGHT, mario);
}

//Moves mario up
void moveUp() {
	drawRect3(currentPlayer.x, currentPlayer.y, MARIO_WIDTH, MARIO_HEIGHT, CYAN);
	currentPlayer.x = currentPlayer.x - playerSpeed;
	drawImage3(currentPlayer.x , currentPlayer.y, MARIO_WIDTH, MARIO_HEIGHT, mario);
}

//Moves mario down
void moveDown() {
	drawRect3(currentPlayer.x, currentPlayer.y, MARIO_WIDTH, MARIO_HEIGHT, CYAN);
	currentPlayer.x = currentPlayer.x + playerSpeed;
	drawImage3(currentPlayer.x , currentPlayer.y, MARIO_WIDTH, MARIO_HEIGHT, mario);
}

//Draws mario
void drawPlayer() {
	int x = currentPlayer.x;
	int y = currentPlayer.y;
	drawImage3(x, y, MARIO_WIDTH, MARIO_HEIGHT, mario);
}

//Random spawn chance
void checkSpawn() {
	int random_number = rand() % 200 + 1;
	if(random_number==1 || random_number==2) {
		spawnCoin();
	} else if (random_number==3 || random_number==4) {
		spawnBomb();
	} else if (random_number==4 || random_number==5) {
		spawnMushroom();
	}
}

//Spawns bomb
void spawnBomb() {
	for(int i=0; i<10; i++) {
		ITEM* a = &spawned[i];
		if(!a->alive){
			a->alive = 1;
			break;
		}
	}
	draw();
}

//Spawns coin
void spawnCoin() {
	for(int i=5; i<10; i++) {
		ITEM* a = &spawned[i];
		if(!a->alive){
			a->alive = 1;
			break;
		}
	}
	draw();
}

//Spawns mushroom
void spawnMushroom() {
	ITEM* a = &spawned[10];
	if(!a->alive){
		a->alive=1;
	}
	draw();
}

//Checks edge collisions and entity collisions
void collision() {
	if(lives==0){
		endGame();
	} else {
		if(currentPlayer.x == 0 || currentPlayer.y == 0 || currentPlayer.y == SCREEN_WIDTH - MARIO_WIDTH || currentPlayer.x == SCREEN_HEIGHT - MARIO_HEIGHT - BOTTOM_BUFFER || currentPlayer.x == 1 || currentPlayer.y == 1 || currentPlayer.y == SCREEN_WIDTH - MARIO_WIDTH - 1 || currentPlayer.x == SCREEN_HEIGHT - MARIO_HEIGHT - BOTTOM_BUFFER + 1){
			lives--;
			updateValues();
			home();
		}
		for(int i=0; i<=10; i++) {
			ITEM* a = &spawned[i];
			if(a->alive){ 
				int valX = abs(abs(currentPlayer.x) - abs(a->x));
				int valY = abs(abs(currentPlayer.y) - abs(a->y));
				if(valX < 10 && valY < 10) {
					if(a->type==0) {
						lives--;
						a->alive=0;
						draw();
						a->x = 0;
						a->y = rand() % SCREEN_WIDTH + 1; 
						updateValues();
						home();
					} else if(a->type==1) {
						currentScore++;
						a->alive=0;
						draw();
						a->x = 0;
						a->y = rand() % SCREEN_WIDTH + 1; 
						updateValues();
						break;
					} else if(a->type==2) {
						if(playerSpeed<2){
							playerSpeed++;
						}
						a->alive=0;
						draw();
						a->x = 0;
						a->y = rand() % SCREEN_WIDTH + 1;
						updateValues();
						break;
					}							
				}
			}
		}
	}
}

//Initializes bombs, mushrooms, coins
void initSpawned() {
	for(int i=0; i<5; i++) {
		ITEM a;
		a.x = 0;
		a.y = rand() % SCREEN_WIDTH + 1; 
		a.type = 0; //bomb
		a.alive = 0;
		spawned[i] = a;
	}
	for(int i=5; i<10; i++) {
		ITEM a;
		a.x = 0;
		a.y = rand() % SCREEN_WIDTH + 1; 
		a.type = 1; //coin
		a.alive = 0;
		spawned[i] = a;
	}
	spawned[10].x = 0;
	spawned[10].y = rand() % SCREEN_WIDTH + 1;
	spawned[10].type = 2; //mushroom
	spawned[10].alive = 0;
}

//Draws bombs, coins, mushrooms
void draw() {
	for(int i=0; i<=10; i++) {
		ITEM* a = &spawned[i];
		if(!a->alive) {
			if(!a->type) {
				drawRect3(a->x, a->y, BOMB_WIDTH, BOMB_HEIGHT, CYAN);
			}else if(a->type) {
				drawRect3(a->x, a->y, COIN_WIDTH, COIN_HEIGHT, CYAN);
			} else {
				drawRect3(a->x, a->y, MUSHROOM_WIDTH, MUSHROOM_HEIGHT, CYAN);
			}			
		} else {
			if(!a->type) {
				drawImage3(a->x, a->y, BOMB_WIDTH, BOMB_HEIGHT, bomb);
			}else if(a->type==1) {
				drawImage3(a->x, a->y, COIN_WIDTH, COIN_HEIGHT, coin);
			} else {
				drawImage3(a->x, a->y, MUSHROOM_WIDTH, MUSHROOM_HEIGHT, mushroom);
			}	
		}
	}
	drawImage3(currentPlayer.x, currentPlayer.y, MARIO_WIDTH, MARIO_HEIGHT, mario);
	drawImage3(currentPlayer.x, currentPlayer.y, MARIO_WIDTH, MARIO_HEIGHT, mario);
}


//Moves items down screen
void moveItemsDown() {
	for(int i=0; i<=10; i++) {
		ITEM* a = &spawned[i];
		if(a->x == SCREEN_HEIGHT - BOTTOM_BUFFER - COIN_HEIGHT) {
			a->alive=0;
			draw();
			a->x = 0;
			a->y = rand() % SCREEN_WIDTH + 1;
		}
		if(a->alive){			
			if(!a->type) {
				drawRect3(a->x, a->y, BOMB_WIDTH, BOMB_HEIGHT, CYAN);
				a->x++;		
			} else {
				drawRect3(a->x, a->y, COIN_WIDTH, COIN_HEIGHT, CYAN);
				a->x++;
			}
		}
	}
	draw();
}

//Updates score, lives, speed on bottom of screen
void updateValues() {
	drawRect3(SCREEN_HEIGHT - BOTTOM_BUFFER, 0 , SCREEN_WIDTH, BOTTOM_BUFFER, BLACK);
	sprintf(scoreString, "Score: %d", currentScore);
	drawString(SCREEN_HEIGHT - BOTTOM_BUFFER + BOTTOM_BUFFER/5, 5, scoreString, GREEN);
	sprintf(livesString, "Lives: %d", lives);
	drawString(SCREEN_HEIGHT - BOTTOM_BUFFER + BOTTOM_BUFFER/5, SCREEN_WIDTH - 60, livesString, GREEN);
	sprintf(speedString, "Speed: %d", playerSpeed);
	drawString(SCREEN_HEIGHT - BOTTOM_BUFFER + BOTTOM_BUFFER/5, SCREEN_WIDTH/3 + 20, speedString, GREEN);
}


//Brings mario back to home when wall is it
void home() {
	drawRect3(currentPlayer.x, currentPlayer.y, MARIO_WIDTH, MARIO_HEIGHT, CYAN);
	currentPlayer.x = SCREEN_HEIGHT - MARIO_HEIGHT - BOTTOM_BUFFER - 5;
	currentPlayer.y = SCREEN_WIDTH/2 - 20;
	drawImage3(currentPlayer.x, currentPlayer.y, MARIO_WIDTH, MARIO_HEIGHT, mario);
	playerSpeed=1;
}


//Lowers speed value
void lowerSpeed() {
	if(playerSpeed>1){
		playerSpeed--;
	}
	updateValues();
}