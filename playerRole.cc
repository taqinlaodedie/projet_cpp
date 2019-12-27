#include "playerRole.hh"

PlayerRole::PlayerRole(int x, int y, RoleType type, std::vector<SDL_Texture*>& vec): Role(x, y, type)
{
	currentDir = UP;
	setTextures(vec);
}

void PlayerRole::draw(SDL_Renderer* renderer)
{
	int width, height;
	SDL_QueryTexture(textures[currentDir], NULL, NULL, &width, &height);
	SDL_Rect rect = {
		_x, _y, width, height
	};

	SDL_RenderCopy(renderer, textures[currentDir], NULL, &rect);
}

void PlayerRole::setTextures(std::vector<SDL_Texture*>& vec)
{
	std::vector<SDL_Texture*>::const_iterator it = vec.cbegin();
	for (; it != vec.cend(); ++it)
	{
		textures.push_back(*it);
	}
}

PlayerRole::~PlayerRole()
{
	// Ne rien faire
}