#include "normalEnemy.hh"

class StrongEnemy: public NormalEnemy
{
public:
	StrongEnemy(int x, int y, RoleType type, std::vector<std::vector<SDL_Texture *>> &vec): 
		NormalEnemy(x, y, type, vec) { this->attackVal = 2; }
};