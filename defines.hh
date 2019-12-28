#pragma once

// Definir les valeurs constantes

// Variables de fenetre
#define WINDOW_CAPTION		"Risk in smoke"	// title
#define WINDOW_WIDTH		980				// largeur de fenetre
#define WINDOW_HEIGHT		780				// hauteur de fenetre

// Zone de jeu
#define GAME_LEFT_AREA		0
#define GAME_RIGHT_AREA		780
#define GAME_TOP_AREA		0
#define GAME_BOTTOM_AREA	780

// La longeur d'une carre
#define SQUARE_LENGTH		30

// Le grandeur et la vitesse initiale d'une balle 
#define INIT_BULLET_SPEED	15
#define BULLET_LENGTH		15

// Position des enemies
#define COL1_POSITION		(780 + 70)
#define COL2_POSITION		(780 + 100)
#define ROWY_POSITION		80

// Positions des informations du jeu
#define SCORE_X_POSITION	(780 + 35)
#define SCORE_Y_POSITION	30
#define LIFE_POSITION		550
#define GAME_LEVEL_POSITION	60
#define SMOKE_X_POSITION	(780 - 60)
#define SMOKE_Y_POSITION	0

// Position des elements interface
#define TITLE_X_POS			200
#define TITLE_Y_POS			100
#define START_X_POS			408
#define START_Y_POS			300
#define START_WIDTH			165
#define START_HEIGHT		52