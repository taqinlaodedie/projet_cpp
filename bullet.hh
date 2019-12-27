#pragma once

#include "role.hh"

class Bullet
{
private:
	int _x;
	int _y;
	int bulletSpeed;	// Vitesse de balle	
	RoleMoveDir dir;	// Sens de balle
	SDL_Texture *tex;
	Role *role;	// Le role qui envoit cette balle
public:
	Bullet(int x, int y, RoleMoveDir dir, SDL_Texture* texture, Role *role);
	~Bullet();

	int getX() const { return _x; }
	int getY() const { return _y; }
	void setSpeed(int speed) { this->bulletSpeed = speed; }
	int getSpeed() const { return this->bulletSpeed; }
	RoleMoveDir getDir() const { return dir; }
	Role* getRole() { return role; }

	void draw(SDL_Renderer* renderer);
	void move();
	bool moveSelf();

	bool collisionScreen();	// Collision avec le bord
};