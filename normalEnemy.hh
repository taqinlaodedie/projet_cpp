#pragma once

// Une enemie est une enemie normale par defaut
#include "role.hh"

class NormalEnemy: public Role
{
public:
	NormalEnemy(int x, int y, RoleType type, std::vector<std::vector<SDL_Texture *>> &vec);
	virtual void draw(SDL_Renderer* renderer);
	int getAttack() const { return attackVal; }
	virtual ~NormalEnemy();
private:
	void setTextures(std::vector<std::vector<SDL_Texture*>>& vec);
protected:
	int attackVal;
};