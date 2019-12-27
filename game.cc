#include "game.hh"
#include <unistd.h>

Game::Game(std::vector<std::vector<SDL_Texture *>> &vec,
		std::vector<SDL_Texture*> &pTexs,
		SDL_Texture *bulletTex, 
		std::vector<SDL_Texture *> &blastTexs,  
		std::vector<SDL_Texture *> &barriersTexs):
			pRole(nullptr), vec(vec), pTexs(pTexs), bulletTex(bulletTex), 
			blastTexs(blastTexs), barriersTexs(barriersTexs)
{
	srand(unsigned (time(NULL)));
	this->barriersTexs = barriersTexs;
	gameLevel = 1;
	started = false;
	playerScore = 3;
	finish = false;
}

void Game::newGame()
{
	pRole = new PlayerRole(4 * SQUARE_LENGTH * 2, 8 * SQUARE_LENGTH * 2, PLAYER, pTexs);

	// Construire un jeu
	NormalEnemy *normalEnemy= new NormalEnemy(4 * SQUARE_LENGTH * 2, 3 * SQUARE_LENGTH * 2, NORMAL_ENEMY, vec);
	StrongEnemy *strongEnemy = new StrongEnemy(5 * SQUARE_LENGTH * 2, 5 * SQUARE_LENGTH * 2, STRONG_ENEMY, vec);

	// Donner une direction 
	normalEnemy->setCurrentDir(RoleMoveDir(rand() % 4));
	strongEnemy->setCurrentDir(RoleMoveDir(rand() % 4));
	eRoles.push_back(normalEnemy);
	eRoles.push_back(strongEnemy);

	// Creer des obstacles
	/*steelBarriers.push_back(new Barrier(6 * SQUARE_LENGTH * 2, 6 * SQUARE_LENGTH * 2, STEEL_BARRIER, barriersTexs[1]));
	normalBarriers.push_back(new Barrier(7 * SQUARE_LENGTH * 2, 7 * SQUARE_LENGTH * 2, NORMAL_BARRIER, barriersTexs[0]));
	normalBarriers.push_back(new Barrier(7 * SQUARE_LENGTH * 2 + SQUARE_LENGTH, 7 * SQUARE_LENGTH * 2, NORMAL_BARRIER, barriersTexs[0]));*/
}

// Le niveau du jeu
int Game::getGameLevel() const
{
	return gameLevel;
}

int Game::getScore() const
{
	if(pRole)
		return playerScore;
	else
		return 0;
}

void Game::loadStage(int lv)
{
	// Pas encore develope
}

void Game::handleInput(RoleMoveDir dir)
{
	if (started)
	{
		if(!pRole)
			return ;
		Role *player = dynamic_cast<Role *>(pRole);
		player->setCurrentDir(dir);

		// S'il n'y a pas de collision
		if(!collisionScreen(player, dir) && !collisionRole(player) && !collisionBarrier(player))
		{
			player->move(dir);
		}	
	}
}

void Game::attack()
{
	if (started)
	{
		if(!pRole)
			return ;
		Role *player = dynamic_cast<Role *>(pRole);

		if(player->canAttack())
		{
			Bullet *bullet = new Bullet(player->getX() + SQUARE_LENGTH - BULLET_LENGTH / 2, player->getY() + SQUARE_LENGTH - BULLET_LENGTH / 2, 
				player->currentDir, bulletTex, player);
			pBullets.push_back(bullet);
			
			// Interdir l'attaque
			player->enableAttack(false);
		}
	}
}

void Game::draw(SDL_Renderer* renderer)
{
	if(started)
	{
		// Movement des enemies
		static int enemyCanMove = 0;
		static int newDir = SQUARE_LENGTH * (rand() % 3 + 4);
	
		// Obstacles
		drawBarriers(renderer);

		// Joueur
		if(pRole)
		{
			Role *player = dynamic_cast<Role *>(pRole);
			player->draw(renderer);
		}

		// Enemies
		enemyCanMove++;
		newDir--;
		for (std::list<NormalEnemy *>::const_iterator it = eRoles.cbegin(); it != eRoles.cend(); it++) {
			(*it)->draw(renderer);
			//std::cout << (*it)->getX() << std::endl;

			// Attaquer aleatoirement
			if((*it)->attack()) {
				if((*it)->canAttack()) {
					Bullet *bullet = new Bullet((*it)->getX() + SQUARE_LENGTH - BULLET_LENGTH / 2, (*it)->getY() + SQUARE_LENGTH - BULLET_LENGTH / 2, 
						(*it)->currentDir, bulletTex, (*it));
					eBullets.push_back(bullet);

					(*it)->enableAttack(false);
				}
			}
		}
		if(enemyCanMove > SQUARE_LENGTH - 2)
		{
			for(std::list<NormalEnemy *>::const_iterator it = eRoles.cbegin(); it != eRoles.cend(); it++)
			{
				
				// S'il n'y a pas de collision pendant certain temps, changer de sens
				if (!collisionScreen(*it, (*it)->currentDir) && !collisionRole(*it) && !collisionBarrier(*it)) {
					(*it)->move((*it)->currentDir);
					// Changer de sens
					if (newDir <= 0) {
						(*it)->setCurrentDir(RoleMoveDir(rand() % 4));
					}
				}
				else {
					// Changer de sens s'il y a des collisions
					(*it)->setCurrentDir(RoleMoveDir(rand() % 4));
				}
			}
			enemyCanMove = 0;
			if(newDir <= 0) {
				newDir = SQUARE_LENGTH * (rand() % 3 + 4);
			}
		}

		// Balles du joueur
		draw(pBullets, renderer, WALL);
		draw(pBullets, renderer, E_ROLE);

		// Balles des enemies
		draw(eBullets, renderer, WALL);
		draw(eBullets, renderer, P_ROLE);

		// Obstacles
		draw(pBullets, renderer, BARRIER);

		// Balles
		draw(pBullets, renderer, NOT_ALL);
	}
}

void Game::drawBarriers(SDL_Renderer *renderer)
{
	drawBarriers(renderer, steelBarriers);
	drawBarriers(renderer, normalBarriers);
	drawBarriers(renderer, waterBarriers);
	drawBarriers(renderer, grassBarriers);
}

void Game::drawBarriers(SDL_Renderer *renderer, std::list<Barrier *>& barriers)
{
	for (std::list<Barrier *>::const_iterator it = barriers.cbegin(); it != barriers.cend(); it++)
	{
		if (*it){
			(*it)->draw(renderer);
		}
	}
}

// Afficher les balles et detecter les collision
void Game::draw(std::list<Bullet *> &bullets, SDL_Renderer *renderer, CollisionWithBulletType type)
{
	switch(type)
	{
		case E_ROLE:	// Collision de balle avec enemie
			if(pRole)
				handleCollision(pBullets, renderer, pRole);
			break;
		case P_ROLE:	// collision de balle avec joueur
			if(pRole)
				handleCollision(renderer, pRole);
			break;
		case WALL:		// avec l'ecran
			for(std::list<Bullet *>::const_iterator it = bullets.cbegin(); it != bullets.cend(); ) {
				bool res = (*it)->moveSelf();
				if(res) {
					// Explosion
					blast(blastTexs, *it, renderer);
					delete (*it);
					it = bullets.erase(it);
					continue;
				}
				else {
					(*it)->draw(renderer);
					it++;
				}
			}
			break;
		case BARRIER:	// avec obstacle
			if (pRole)
				handleCollision(pBullets, PLAYER, renderer);
			handleCollision(eBullets, NORMAL_ENEMY, renderer);
			break;
		case NOT_ALL:
			break;
		default:
			break;
	}
}

// Tuer enemie
void Game::handleCollision(std::list<Bullet *> &bullets, SDL_Renderer *renderer, Role *pRole)
{
	for(std::list<Bullet *>::const_iterator item = bullets.cbegin(); item != bullets.cend(); ) {
		bool sign = false;
		for(std::list<NormalEnemy *>::const_iterator it = eRoles.cbegin(); it != eRoles.cend(); ) {
			if(item == pBullets.cend())
				break;
			// collision avec role
			if(collisionBullet(*item, *it)) {
				sign = true;
				// balle expoite
				blast(blastTexs, *item, renderer);
				delete (*item);
				item = pBullets.erase(item);
				pRole->enableAttack(true);
				playerScore += (*it)->getAttack();;
				// Supprimer enemie
				delete (*it);
				it = eRoles.erase(it);
				std::cout << "Enemy is hitted" << std::endl;
				std::cout << "Score: " << playerScore << std::endl;
				break;
			}
			else {
				it++;
			}
		}
		if(!sign) {
			(*item)->draw(renderer);
			item++;
		}
	}
}

// Tuer joueur
void Game::handleCollision(SDL_Renderer *renderer, Role *pRole)
{
	for(std::list<Bullet *>::const_iterator it = eBullets.cbegin(); it != eBullets.cend(); ) {
		if(collisionBullet(*it, pRole)) {
			std::cout << "Player is hitted" << std::endl;
			NormalEnemy *item = dynamic_cast<NormalEnemy *>((*it)->getRole());
			playerScore -= (item->getAttack());
			std::cout << "Score: " << playerScore << std::endl;
			blast(blastTexs, *it, renderer);
			(*it)->getRole()->enableAttack(true);
			delete (*it);
			it = eBullets.erase(it);
			if(playerScore <= 0) {
				finish = true;
				std::cout << "You lose" << std::endl;
			}
		}
		else {
			(*it)->draw(renderer);
			it++;
		}
	}
}

// Collision entre balle et obstacle
void Game::handleCollision(std::list<Bullet *> &bullets, RoleType type, SDL_Renderer *renderer)
{
	for(std::list<Bullet *>::const_iterator it = bullets.cbegin(); it != bullets.cend(); ) {
		bool sign = false;
		// Obstacle normale doit etre supprime
		for(std::list<Barrier *>::const_iterator item = normalBarriers.cbegin(); item != normalBarriers.cend(); ) {
			if(collisionBarrier(*item, *it)) {
				sign = true;
				delete *item;
				item = normalBarriers.erase(item);

				for(std::list<Barrier *>::const_iterator item2 = normalBarriers.cbegin(); item2 != normalBarriers.cend(); ) {
					if(collisionBarrier(*item2, *it)) {
						delete (*item2);
						item2 = normalBarriers.erase(item2);
						break;
					}
					else {
						++item2;
					}
				}
				// Supprimer la balle
				blast(blastTexs, *it, renderer);
				(*it)->getRole()->enableAttack(true);
				delete (*it);
				it = bullets.erase(it);
				break;
			}
			else {
				item++;
			}
		}
		if(!sign) {
			(*it)->draw(renderer);
			it++;
		}
	}

	// Steel barriers
	for (std::list<Bullet *>::const_iterator it = bullets.cbegin(); it != bullets.cend(); ) {
		bool sign = false;
		for(std::list<Barrier *>::const_iterator item = steelBarriers.cbegin(); item != steelBarriers.cend(); ) {
			// Obstacle reste
			if(collisionBarrier(*item, *it)) {
				sign = true;
				blast(blastTexs, *it, renderer);
				(*it)->getRole()->enableAttack(true);
				delete (*it);
				it = bullets.erase(it);
				break;
			}
			else {
				item++;
			}
		}
		if (!sign) {
			(*it)->draw(renderer);
			++it;
		}
	}
}

// Explotion de balle
void Game::blast(std::vector<SDL_Texture *> &texs, Bullet *bullet, SDL_Renderer *renderer)
{
	for(std::vector<SDL_Texture *>::const_iterator it = texs.cbegin(); it != texs.cend(); it++) {
		int width = 0, height = 0;
		SDL_QueryTexture(*it, NULL, NULL, &width, &height);
		SDL_Rect rect = { bullet->getX() - width / 2, bullet->getY() - height / 2, width, height };

		SDL_RenderCopy(renderer, *it, NULL, &rect);
	}
}


// Detection de collision entre balle et obstacle
bool Game::collisionBarrier(Barrier *barriers, Bullet *bullet)
{
	int distance = bullet->getSpeed();

	int x = bullet->getX();
	int y = bullet->getY();

	RoleMoveDir dir = bullet->getDir();

	switch (dir) {
		case LEFT:
			x -= distance;
			break;
		case UP:
			y -= distance;
			break;
		case RIGHT:
			x += distance;
			break;
		case DOWN:
			y += distance;
			break;
		case NONE:
			break;
		default:
			break;
	}

	// Calculer le centre de balle
	int bulletCenterX = x + BULLET_LENGTH / 2;
	int bulletCenterY = y + BULLET_LENGTH / 2;

	// Calculer centre d'obstacle
	int barrierLength = barriers->getBarrierLength();
	int barrierCenterX = barriers->getX() + barrierLength / 2;
	int barrierCenterY = barriers->getY() + barrierLength / 2;

	if((abs(barrierCenterX - bulletCenterX) < (BULLET_LENGTH / 2 + barrierLength / 2)) && 
		(abs(barrierCenterY - bulletCenterY) < (BULLET_LENGTH / 2 + barrierLength / 2))) {
		return true;
	}
	return false;
}

// Collision entre role et obstace
bool Game::collisionBarrier(Barrier *barriers, Role *role)
{
	int distance = role->getDistance();
	int x = role->getX();
	int y = role->getY();
	RoleMoveDir dir = role->currentDir;

	switch(dir) {
		case LEFT:
			x -= distance;
			break;
		case UP:
			y -= distance;
			break;
		case RIGHT:
			x += distance;
			break;
		case DOWN:
			y += distance;
			break;
		case NONE:
			break;
		default:
			break;
	}

	int barrierLength = barriers->getBarrierLength();
	int barrierCenterX = barriers->getX() + barrierLength / 2;
	int barrierCenterY = barriers->getY() + barrierLength / 2;

	int roleCenterX = x + SQUARE_LENGTH;
	int roleCenterY = y + SQUARE_LENGTH;

	if((abs(roleCenterX - barrierCenterX) < (barrierLength / 2 + SQUARE_LENGTH)) &&
		(abs(roleCenterY - barrierCenterY) < (barrierLength / 2 + SQUARE_LENGTH))) {
		return true;
	}
	return false;
}

// Detection de collision entre balle et joueur
bool Game::collisionBullet(Bullet *bullet, Role *role)
{
	int Distance = bullet->getSpeed();
	int x = bullet->getX();
	int y = bullet->getY();
	RoleMoveDir dir = bullet->getDir();

	switch (dir) {
		case LEFT:
			x -= Distance;
			break;
		case UP:
			y -= Distance;
			break;
		case RIGHT:
			x += Distance;
			break;
		case DOWN:
			y += Distance;
			break;
		case NONE:
			break;
		default:
			break;
	}

	int bulletCenterX = x + BULLET_LENGTH / 2;
	int bulletCenterY = y + BULLET_LENGTH / 2;

	int roleCenterX = role->getX() + SQUARE_LENGTH;
	int roleCenterY = role->getY() + SQUARE_LENGTH;

	if((abs(roleCenterX - bulletCenterX) < (BULLET_LENGTH / 2 + SQUARE_LENGTH)) &&
		(abs(roleCenterY - bulletCenterY) < (BULLET_LENGTH / 2 + SQUARE_LENGTH))) {
		return true;
	}
	return false;
}


// Collision entre enemie et obstacle
bool Game::collisionBarrier(Role *role)
{
	// steel barrier
	for(std::list<Barrier *>::const_iterator it = steelBarriers.cbegin(); it != steelBarriers.cend(); it++) {
		if(collisionBarrier(*it, role))
			return true;
	}

	// normal barrier
	for(std::list<Barrier *>::const_iterator it = normalBarriers.cbegin(); it != normalBarriers.cend(); it++) {
		if(collisionBarrier(*it, role))
			return true;
	}

	// water barrier
	for (std::list<Barrier *>::const_iterator it = waterBarriers.cbegin(); it != waterBarriers.cend(); it++) {
		if (collisionBarrier(*it, role))
			return true;
	}

	return false;
}

// Detection de collision entre bord et role
bool Game::collisionScreen(Role *role, RoleMoveDir dir)
{
	int distance = SQUARE_LENGTH;
	int x = role->getX();
	int y = role->getY();

	bool res = false;
	switch(dir) {
		case LEFT:
			if ((x - distance) < GAME_LEFT_AREA)
				res = true;
			else
				res = false;
			break;
		case UP:
			if ((y - distance) < GAME_TOP_AREA)
				res = true;
			else
				res = false;
			break;
		case RIGHT:
			if ((x + SQUARE_LENGTH * 2 + distance) > GAME_RIGHT_AREA)
				res = true;
			else
				res = false;
			break;
		case DOWN:
			if ((y + SQUARE_LENGTH * 2 + distance) > GAME_BOTTOM_AREA)
				res = true;
			else
				res = false;
			break;
		case NONE:
			break;
		default:
			break;
	}

	return res;
}

void Game::getNextPosition(int* x, int* y, Role *role)
{
	int _x = role->getX();
	int _y = role->getY();

	int distance = role->getDistance();

	switch (role->currentDir)
	{
		case LEFT:
			_x -= distance;
			break;
		case UP:
			_y -= distance;
			break;
		case RIGHT:
			_x += distance;
			break;
		case DOWN:
			_y += distance;
			break;
		case NONE:
			break;
		default:
			break;
	}

	(*x) = _x;
	(*y) = _y;
}

// Detection de collision entre les roles
bool Game::collisionRole(Role *role1, Role *role2)
{
	int x1 = 0, y1 = 0;
	int x2 = 0, y2 = 0;
	getNextPosition(&x1, &y1, role1);

	x2 = role2->getX();
	y2 = role2->getY();

	if((abs(x2 - x1) < (SQUARE_LENGTH * 2)) && (abs(y2 - y1) < (SQUARE_LENGTH * 2))) {
		return true;
	}
	return false;
}

bool Game::collisionRole(Role *role)
{
	for(std::list<NormalEnemy *>::const_iterator it = eRoles.cbegin(); it != eRoles.cend(); it++)
	{
		if(role == (*it)) {
			continue;
		}
		else {
			if(collisionRole(role, *it))
				return true;
			if(pRole && role != pRole) {
				if(collisionRole(role, pRole))
					return true;
			}
		}
	}
	return false;
}

void Game::clear()
{
	clear(steelBarriers);
	clear(normalBarriers);
	clear(grassBarriers);
	clear(waterBarriers);

	for(std::list<NormalEnemy *>::const_iterator it = eRoles.cbegin(); it != eRoles.cend(); it++) {
		if(*it)
			delete (*it);
	}
	eRoles.clear();
}

void Game::clear(std::list<Barrier *> &barriers)
{
	for(std::list<Barrier *>::const_iterator it = barriers.cbegin(); it != barriers.cend(); it++) {
		if(*it)
			delete (*it);
	}
	barriers.clear();
}

void Game::clear(std::list<Bullet *> &bullets)
{
	for(std::list<Bullet*>::const_iterator it = bullets.cbegin(); it != bullets.cend(); it++) {
		if(*it)
			delete (*it);
	}
	bullets.clear();
}

Game::~Game()
{
	for(std::list<NormalEnemy *>::const_iterator it = eRoles.cbegin(); it != eRoles.cend(); it++) {
		if(*it)
			delete (*it);
	}
	eRoles.clear();
	if(pRole)
		delete pRole;

	clear(steelBarriers);
	clear(normalBarriers);
	clear(grassBarriers);
	clear(waterBarriers);

	clear(pBullets);
	clear(eBullets);
}

std::ifstream &operator<<(Game &g, std::ifstream &input)
{
	int x, y, type;
	std::string s;

	while(std::getline(input, s)) {
		sscanf(s.c_str(), "%d %d %d", &type, &x, &y);
		switch(type) {
			case 0:		// normal barrier
				g.normalBarriers.push_back(new Barrier(x, y, NORMAL_BARRIER, g.barriersTexs[0]));
				break;
			case 1:		// steel barrier
				g.steelBarriers.push_back(new Barrier(x, y, STEEL_BARRIER, g.barriersTexs[1]));
				break;
			case 2:		// grass barrier
				g.grassBarriers.push_back(new Barrier(x, y, GRASS_BARRIER, g.barriersTexs[2]));
				break;
			case 3:		// water barrier
				g.waterBarriers.push_back(new Barrier(x, y, WATER_BARRIER, g.barriersTexs[3]));
				break;
		}
	}

	return input;
}

std::ofstream &operator<<(std::ofstream &output, const Game &g)
{
	time_t now = time(0);

	output << ctime(&now) << "	Score: " << g.playerScore << std::endl;
	std::cout << "record finished" << std::endl;

	return output;
}