#pragma once

#include <SDL.h>        
#include <SDL_image.h> 
#include <SDL_ttf.h> 
#include <iostream>
#include "game.hh"
#include <fstream>

class Window 
{
private:
	// Les images
	SDL_Window *win;
	SDL_Renderer *renderer;
	SDL_Surface *coverImage;
	SDL_Texture *texture;

	// Les variables gestion de jeu
	int quit;
	SDL_Event event;
	bool mouse_on_button_start;
	TTF_Font *font;
	// Texture des roles
	std::vector<std::vector<SDL_Texture *>> vec;
	std::vector<SDL_Texture *> pTexs;
	// Texture de balle
	SDL_Texture *bulletTex;
	// Texture d'explotion
	std::vector<SDL_Texture *> blastTexs;
	// Texture d'obstacles
	std::vector<SDL_Texture *> barrierTexs;
	// Le jeu
	Game* game;
	// Texture du fond
	SDL_Texture *bg;
	SDL_Texture *cover;
	// Texture du font
	SDL_Texture *fontTexs[5];
	SDL_Texture *gameTexs[5];
	// Bouton commencer
	bool mouseOnButton;		
	// Record du jeu
	std::ofstream record;
	// Charger le map
	std::ifstream mapInfo;

	void drawBackground();
	void drawCover();
	void drawGameInfo();
	void drawTitle();
	void drawButton();

	void destroyTexture();
public:
	Window();
	~Window();
	void start();
	void init();
	void loadResourceFile();
	void update();
	SDL_Texture *loadImage(std::string filePath);	
	SDL_Texture *loadGifImage(std::string filePath);
	void drawText(std::string message, int x, int y, int flag);
};