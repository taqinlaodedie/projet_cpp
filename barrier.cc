#include "barrier.hh"

Barrier::Barrier(int x, int y, BarrierType type, SDL_Texture* texture): _x(x), _y(y), type(type), tex(texture)
{
	switch(type) {
		case NORMAL_BARRIER:
			barrierLength = SQUARE_LENGTH;
			break;
		case STEEL_BARRIER:
			barrierLength = SQUARE_LENGTH;
			break;
		case GRASS_BARRIER:
			barrierLength = SQUARE_LENGTH * 2;
			break;
		case WATER_BARRIER:
			barrierLength = SQUARE_LENGTH * 2;
			break;
		default:
			break;
	}
}

void Barrier::draw(SDL_Renderer* renderer)
{
	int width = 0, height = 0;
	SDL_QueryTexture(tex, NULL, NULL, &width, &height);
	SDL_Rect rect = {
		_x, _y, width, height
	};

	SDL_RenderCopy(renderer, tex, NULL, &rect);
}

Barrier::~Barrier()
{
	// Ne rien faire
}