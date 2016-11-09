
#include "tpl_os.h"
#include "Arduino.h"
#include "casseBrique.h"

U8GLIB_NHD_C12864 u8g(SCK, MOSI, CS, A0, RST);

void setup() {
	u8g.setRot180();
	u8g.setContrast(0);
	u8g.setFont(u8g_font_6x12);
	u8g.setFontRefHeightText();
	u8g.setFontPosTop();

	menu_redraw_required = true;
	game_redraw_required = false;

	myPaddle = new Paddle();
	initPaddle();
	myBall = new Ball();
	initBall();
	myScreen = new Screen();
	initScreen();
	score = 0;
	highscore = 0;
}

void initPaddle() {
	myPaddle->x = 40;
	myPaddle->y = 60;
	myPaddle->width = 27;
	myPaddle->height = 3;
	myPaddle->movement = 10;
}

void initBall() {
	myBall->x = 30;
	myBall->y = 55;
	myBall->radius = 2;
	myBall->movX = 5;
	myBall->movY = -5;
}

void initScreen() {
	myScreen->width = u8g.getWidth() -1;
	myScreen->score_h = u8g.getFontAscent() - u8g.getFontDescent();
	myScreen->game_h = u8g.getHeight() - myScreen->score_h;
}

TASK(periodicTask) {
	key = get_joystick_key(analogRead(0));
	switch (STATE) {
	case IS_MENU :
		updateMenu();
		break;
	case IS_GAME :
		updateGame();
		break;
	}
	if (menu_redraw_required || game_redraw_required) {
		u8g.firstPage();
		do {
			if (STATE == IS_MENU)	drawMenu();
			else					drawGame();
		} while(u8g.nextPage());
		menu_redraw_required = false;
		game_redraw_required = false;
	}
}

void drawMenu() {
	uint8_t h, fh;
	u8g_uint_t w;

	fh = u8g.getFontAscent() - u8g.getFontDescent();
	h = u8g.getHeight();
	w = u8g.getWidth();
	u8g.setDefaultForegroundColor();
	u8g.drawStr(20, 5, "HIGHSCORE : ");
	u8g.setPrintPos(100, 5);
	u8g.print(highscore);
	char* title = "CASSE-BRIQUE";
	char* play = "PLAY";

	int posX, posY, width, height;
	posX = w/2 - u8g.getStrWidth(title)/2 -5;
	posY = h/2 - (1.5*fh);
	width = u8g.getStrWidth(title) + 10;
	height = 3 * fh;
	u8g.drawBox(posX, posY, width, height);
	u8g.setDefaultBackgroundColor();
	u8g.drawStr(w/2 - u8g.getStrWidth(title)/2, h/2 - fh, title);
	u8g.drawStr(w/2 - u8g.getStrWidth(play)/2, h/2, play);
}

void updateMenu() {
	if (key == CLICK) {
		STATE = IS_GAME;
		menu_redraw_required = false;
		game_redraw_required = true;
	}
	key = NONE;
}

void updateGame() {
	moveBall();
	score++;
	switch (key) {
	case LEFT :
		if (myPaddle->x > 0)
			myPaddle->x -= myPaddle->movement;
		break;
	case RIGHT :
		if (myPaddle->x < myScreen->width - myPaddle->width)
			myPaddle->x += myPaddle->movement;
		break;
	}
	game_redraw_required = true;
	key = NONE;
}

void moveBall() {
	if (myBall->x + myBall->movX < 0
		|| myBall->x + myBall->movX > myScreen->width) {
		myBall->movX = -myBall->movX;
	}
	if (myBall->y + myBall->movY < myScreen->score_h) {
		myBall->movY = -myBall->movY;
	}
	if (myBall->y + myBall->movY > myScreen->game_h + myScreen->score_h
		&& myBall->x + myBall->movX < myPaddle->x + myPaddle->width
		&& myBall->x + myBall->movX > myPaddle->x) {
		myBall->movY = -myBall->movY;
	}
	if (myBall->y + myBall->movY > myScreen->game_h + myScreen->score_h) {
		STATE = IS_MENU;
		initPaddle();
 		initBall();
 		if (score > highscore) {
 			highscore = score;
 		}
 		score = 0;
	    menu_redraw_required = true;
	    game_redraw_required = false;
	}
	myBall->x += myBall->movX;
	myBall->y += myBall->movY;
}

void drawGame() {
	u8g.setDefaultForegroundColor();
	u8g.drawBox(0, 0, myScreen->width +1, myScreen->score_h);
	
	u8g.drawLine(0,
				 myScreen->score_h,
				 0,
				 myScreen->game_h + myScreen->score_h);
	u8g.drawLine(0, 
				 myScreen->score_h,
				 myScreen->width, 
				 myScreen->score_h);
	u8g.drawLine(myScreen->width,
				 myScreen->score_h,
				 myScreen->width,
				 myScreen->game_h + myScreen->score_h);
	u8g.drawBox(myPaddle->x,
				myPaddle->y,
				myPaddle->width,
				myPaddle->height);
	u8g.drawCircle(myBall->x,
				   myBall->y,
				   myBall->radius);
	u8g.setDefaultBackgroundColor();
	char* scoreDisplay = "SCORE : ";
	u8g.drawStr(myScreen->width/2 - u8g.getStrWidth(scoreDisplay),
				0,
				scoreDisplay);
	u8g.setPrintPos(myScreen->width/2 +9, 0);
	u8g.print(score);
}

// Converti l'input en variable Joystick
// L'input est une valeur comprise entre 0 et 1023
// Il correspond à la valeur retournée par le potentiomètre
int get_joystick_key(unsigned int input) {
    if (input < 100) return LEFT;
    else  if (input < 300) return CLICK;
    else  if (input < 500) return BOTTOM;
    else  if (input < 700) return RIGHT;
    else  if (input < 900) return TOP;    
    else  return NONE;
}