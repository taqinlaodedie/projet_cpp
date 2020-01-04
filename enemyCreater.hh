#include "game.hh"

class EnemyCreater
{
public:
	EnemyCreater(Game &g, std::vector<std::vector<SDL_Texture *>> &vec);
	~EnemyCreater();
	void create();
private:
	Game &game;
	std::vector<std::vector<SDL_Texture *>> &vec;
};