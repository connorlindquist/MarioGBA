//Connor Lindquist
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160
#define BOTTOM_BUFFER 15

#define TITLE 0
#define NORMAL 1
#define GAMEOVER 2

typedef struct {
	int x;
	int y;
	int type;
	int alive;
} ITEM;

typedef struct {
	int x;
	int y;
} PLAYER;

extern int state;
extern PLAYER currentPlayer;
extern ITEM spawned[11];

void refresh();
void reset();
void startGame();
void endGame();
void moveLeft();
void moveRight();
void moveUp();
void moveDown();
PLAYER genPlayer();
void drawPlayer();
void spawnCoin();
void spawnBomb();
void spawnMushroom();
void collision();
void checkSpawn();
void initSpawned();
void draw();
void moveItemsDown();
void updateValues();
void home();
void lowerSpeed();