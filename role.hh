#pragma once

#include <SDL.h>        
#include <SDL_image.h> 
#include "enums.hh" 
#include "defines.hh"
#include <vector>

class Role {
protected:
	int _x;
	int _y;
	int speed;	// Vitesse de balle
	RoleType type;
	std::vector<SDL_Texture *> textures;
	int distance;	// Distance de chaque mouvement
	bool isCanAttack;	// Pouvoir attaquer
public:
	RoleMoveDir currentDir;	// Sens de mouvement
	
	Role(int x, int y, RoleType type);
	virtual void draw(SDL_Renderer *renderer) = 0;
	virtual ~Role();

	void setX(int x) { this->_x = x; }
	void setY(int y) { this->_y = y; }
	int getX() const { return this->_x; }
	int getY() const { return this->_y; }
	int getDistance() const { return this->distance; }

	virtual void move(RoleMoveDir dir);
	void setCurrentDir(RoleMoveDir dir);
	bool canAttack() const { return this->isCanAttack; }
	void enableAttack(bool val) { this->isCanAttack = val; }
	bool attack();
};