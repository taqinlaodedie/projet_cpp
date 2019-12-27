#include "normalEnemy.hh"

NormalEnemy::NormalEnemy(int x, int y, RoleType type, std::vector<std::vector<SDL_Texture* >> &vec): Role(x, y, type)
{
	attackVal = 1;
	currentDir = RoleMoveDir(rand() % 4);
	setTextures(vec);
}

void NormalEnemy::setTextures(std::vector<std::vector<SDL_Texture*>> &vec)
{
	std::vector<SDL_Texture*>::const_iterator it = vec[type].begin();
	for (; it != vec[type].end(); ++it)
	{
		textures.push_back(*it);
	}
}

void NormalEnemy::draw(SDL_Renderer *renderer)
{
	int width = 0, height = 0;
	SDL_QueryTexture(textures[currentDir], NULL, NULL, &width, &height);
	SDL_Rect rect = {
		_x, _y, width, height
	};

	SDL_RenderCopy(renderer, textures[currentDir], NULL, &rect);
}

NormalEnemy::~NormalEnemy()
{
	// Ne rien faire
}