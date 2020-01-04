#pragma once

#include <vector>
#include "enums.hh"
#include "defines.hh"
#include <SDL.h>        
#include <SDL_image.h>  

class Barrier
{
public:
	Barrier(int x, int y, BarrierType type, SDL_Texture *texture);
	~Barrier(void);

public:
	void draw(SDL_Renderer *renderer);

	int getX() const { return _x; }
	int getY() const { return _y; }
	BarrierType getType() const { return type; }
	int getBarrierLength() const { return barrierLength; }

private:
	int _x;
	int _y;
	int barrierLength;
	BarrierType type;
	SDL_Texture* tex;
};