#include "enemyCreater.hh"

EnemyCreater::EnemyCreater(Game &g, std::vector<std::vector<SDL_Texture *>> &vec): game(g), vec(vec)
{
	// Ne rien faire
}

EnemyCreater::~EnemyCreater()
{
	// Ne rien faire
}

// Creer des enemies aleatoires
void EnemyCreater::create()
{
	static int timeSlot = 0;
	timeSlot--;
	if (timeSlot < 0)
	{
		timeSlot = rand() % 1000 + 100;
		switch(rand()%2) {
			case 0:
				game += new NormalEnemy(SMOKE_X_POSITION, SMOKE_Y_POSITION, NORMAL_ENEMY, vec);
				break;
			case 1:
				game += new StrongEnemy(SMOKE_X_POSITION, SMOKE_Y_POSITION, STRONG_ENEMY, vec);
				break;
			default:
				break;
		}
		std::cout << "enemy created" << std::endl;
	}
	//std::cout << timeSlot << std::endl;
}