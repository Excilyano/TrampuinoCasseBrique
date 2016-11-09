#include "U8glib/src/U8glib.h"
#include "Paddle.h"
#include "Screen.h"
#include "Ball.h"

/***************************
 * Variables ports arduino *
 ***************************/

#define SCK   13
#define MOSI  11
#define CS    10
#define A0    9
#define RST   8
#define JOYSTICK 	A0

/**************************
 ***** Alias Joystick *****
 **************************/

#define NONE    -1
#define LEFT    0
#define CLICK   1
#define BOTTOM  2
#define RIGHT   3
#define TOP     4


/**************************
 **** Variables d'etat ****
 **************************/

#define IS_MENU 1
#define IS_GAME 2
uint8_t STATE = IS_MENU;
bool menu_redraw_required, game_redraw_required;
uint8_t key = NONE;

/**************************
 **** Elements de jeu *****
 **************************/

Paddle* myPaddle;
Ball* myBall;
Screen* myScreen;

int score, highscore;

/**************************
 ****** Signatures ********
 **************************/
void drawMenu();
void drawGame();
void initPaddle();
void initBall();
void initScreen();
void moveBall();
void updateMenu();
void updateGame();
int get_joystick_key(unsigned int);