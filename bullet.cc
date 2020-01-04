#include "bullet.hh"

Bullet::Bullet(int x, int y, RoleMoveDir dir, SDL_Texture *texture, Role *role): 
	_x(x), _y(y), dir(dir), tex(texture), role(role)
{
	bulletSpeed = INIT_BULLET_SPEED;
}

void Bullet::draw(SDL_Renderer *renderer)
{
	int width = 0, height = 0;
	SDL_QueryTexture(tex, NULL, NULL, &width, &height);
	SDL_Rect rect = { _x, _y, width, height };

	SDL_RenderCopy(renderer, tex, NULL, &rect);
}

void Bullet::move()
{
	switch (dir)
	{
	case LEFT:
		_x -= bulletSpeed;
		break;
	case UP:
		_y -= bulletSpeed;
		break;
	case RIGHT:
		_x += bulletSpeed;
		break;
	case DOWN:
		_y += bulletSpeed;
		break;
	case NONE:
		break;
	default:
		break;
	}
}

// Bouger si la balle est dans l'ecran
bool Bullet::moveSelf()
{
	if (!collisionScreen()){
		move();
		return false;
	}
	else{
		role->enableAttack(true);
		return true;
	}
}

// Detecter si la balle depasse l'ecran
bool Bullet::collisionScreen()
{
	bool res = false;
	switch (dir)
	{
	case LEFT:
		{
			if ((_x - bulletSpeed) < GAME_LEFT_AREA)
				res = true;
			else
				res = false;
		}
		break;
	case UP:
		{
			if ((_y - bulletSpeed) < GAME_TOP_AREA)
				res = true;
			else
				res = false;
		}
		break;
	case RIGHT:
		{
			if ((_x + bulletSpeed + BULLET_LENGTH) > GAME_RIGHT_AREA)
				res = true;
			else
				res = false;
		}
		break;
	case DOWN:
		{
			if ((_y + bulletSpeed + BULLET_LENGTH) > GAME_BOTTOM_AREA)
				res = true;
			else
				res = false;
		}
		break;
	case NONE:
		break;
	default:
		break;
	}

	return res;
}

Bullet::~Bullet()
{
	// Ne rien faire
}