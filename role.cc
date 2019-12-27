#include "role.hh"

Role::Role(int x, int y, RoleType type): _x(x), _y(y), type(type)
{
	speed = 0;
	distance = SQUARE_LENGTH;
	currentDir = NONE;
	isCanAttack = true;
}

void Role::move(RoleMoveDir dir)
{
	switch(dir) {
		case LEFT:
			_x -= SQUARE_LENGTH;
			break;
		case UP:
			_y -= SQUARE_LENGTH;
			break;
		case RIGHT:
			_x += SQUARE_LENGTH;
			break;
		case DOWN:
			_y += SQUARE_LENGTH;
			break;
		case NONE:
			break;
		default:
			break;
	}
}

void Role::setCurrentDir(RoleMoveDir dir)
{
	switch (dir)
	{
	case LEFT:
		currentDir = LEFT;
		break;
	case UP:
		currentDir = UP;
		break;
	case RIGHT:
		currentDir = RIGHT;
		break;
	case DOWN:
		currentDir = DOWN;
		break;
	case NONE:
		break;
	default:
		break;
	}
}

// Generer un temps aleatoire entre deux attaques
bool Role::attack()
{
	static int attackTime = rand() % 100;
	attackTime--;
	if (attackTime < 0)
	{
		attackTime = rand() % 100;
		return true;
	}
	else{
		return false;
	}
}

Role::~Role()
{
	// Ne rien faire
}