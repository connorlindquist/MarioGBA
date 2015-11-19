//Connor Lindquist
#include <stdlib.h>
#include "myLib.h"
#include "game.h"

int main() {
	REG_DISPCTL = MODE3 | BG2_ENABLE;
	int isReset = 1;

	while(1) {
		__key_prev = __key_curr; // save the previous key hit
		__key_curr = ~BUTTONS & BUTTON_MASK; // get the current key hit
		waitForVblank();
		if (KEY_HIT(BUTTON_START)) {
			switch(state) {
			case TITLE: // start the game
				state = NORMAL;
				startGame();
				break;
			case GAMEOVER: // reset
				reset();
				break;
			}
		} else if (KEY_HIT(BUTTON_SELECT) || isReset) {
			reset(); // reset whenever select is hit or at the start
			isReset = 0;
		} else if (state == NORMAL) {
			if (KEY_HIT(BUTTON_UP) || KEY_HELD(BUTTON_UP)) {
				moveUp();
				draw();
			} else if (KEY_HIT(BUTTON_DOWN) || KEY_HELD(BUTTON_DOWN)) {
				moveDown();
				draw();
			} else if (KEY_HIT(BUTTON_LEFT) || KEY_HELD(BUTTON_LEFT)) {
				moveLeft();
				draw();
			} else if (KEY_HIT(BUTTON_RIGHT) || KEY_HELD(BUTTON_RIGHT)) {
				moveRight();
				draw();
			} else if (KEY_HIT(BUTTON_A)) {
				lowerSpeed();
			}
			checkSpawn();
			draw();
			collision();
			moveItemsDown();
		}
		refresh();
	}
}