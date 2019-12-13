#pragma once

#include <SDL.h>        
#include <SDL_image.h>  
#include <iostream>

class Window {
private:
	// Les images
	SDL_Window *win;
	SDL_Renderer *renderer;
	SDL_Surface *coverImage;

	// Les variables gestion de jeu
	int quit;
	SDL_Event event;
	bool mouse_on_button_start;
public:
	Window();
	~Window();
	void start();
};