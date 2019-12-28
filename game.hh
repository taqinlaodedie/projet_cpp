#pragma once

#include <iostream>
#include "enums.hh"
#include "defines.hh"
#include "playerRole.hh"
#include "normalEnemy.hh"
#include "strongEnemy.hh"
#include "barrier.hh"
#include "bullet.hh"
#include <list>
#include <vector>
#include <fstream>

class Game
{
public:
	bool started;	// Le jeu est commence

	Game(std::vector<std::vector<SDL_Texture *>> &vec,
		std::vector<SDL_Texture*> &pTexs,
		SDL_Texture *bulletTex, 
		std::vector<SDL_Texture *> &blastTexs,  
		std::vector<SDL_Texture *> &barriersTexs);
	~Game();

	void draw(SDL_Renderer* renderer);		// Afficher tous les elements
	void handleInput(RoleMoveDir dir);		// Gestion des entrees du sens
	void attack();							// Gestion des attaques
	void clear();							// Netoyer les elements
	void newGame();							// Initialiser la carte et les roles
	void loadStage(int lv);					// Charger les diffenrents niveau du jeu
	int getEnemyCount();					// Nombre d'enemies
	int getGameLevel() const;				// Niveau du jeu
	int getScore() const;					// Score du joueur
	bool gameOver() { return finish; }

	friend std::ifstream &operator<<(Game &g, std::ifstream &input);		// Charger le map du jeu en utilisant un fichier
	friend std::ofstream &operator<<(std::ofstream &output, const Game &g);	// Donner le score du jeu
	friend Game &operator+=(Game &g, NormalEnemy *e);						// Ajouter un nouvel enemi
private:
	PlayerRole *pRole;					// joueur
	std::list<NormalEnemy *> eRoles;	// list d'enemies

	// Obstacles
	std::list<Barrier *> steelBarriers;
	std::list<Barrier *> normalBarriers;
	std::list<Barrier *> waterBarriers;
	std::list<Barrier *> grassBarriers;

	// Textures (4 sens pour chaque role)
	std::vector<std::vector<SDL_Texture *>> &vec;	// Textures d'enemies
	std::vector<SDL_Texture *> &pTexs;				// Textures du joueurs
	SDL_Texture* &bulletTex;						// Texture de balle
	std::vector<SDL_Texture *> &blastTexs;			// Textures d'explosion 
	std::vector<SDL_Texture *> &barriersTexs;		// Textures d'obstacles

	// Les balles
	std::list<Bullet *> pBullets;	// balles du joueur	
	std::list<Bullet *> eBullets;	// balles de l'enemie

	int gameLevel;		// niveau du jeu
	int playerScore;	// Score du joueur

	bool finish;		// Game over si vrai

	// Afficher les differents elements
	void draw(std::list<Bullet *> &bullets, SDL_Renderer *renderer, CollisionWithBulletType type);
	void drawBarriers(SDL_Renderer *renderer);
	void drawBarriers(SDL_Renderer *renderer, std::list<Barrier *> &barriers);

	// Supprimer les elements du jeu
	void clear(std::list<Barrier *> &barriers);
	void clear(std::list<Bullet *> &bullets);

	// Detection des collisions
	bool collisionScreen(Role *role, RoleMoveDir dir);				// Collisions avec les bordes
	bool collisionRole(Role *role1, Role *role2);					// Collision entre les roles
	bool collisionRole(Role *role);
	bool collisionBullet(Bullet *bullet, Role *role);				// Collision entre role et balle
	bool collisionBullet(Bullet *eBullet, Bullet *pBullet);
	bool collisionBarrier(Barrier *barriers, Role *role);			// Collision entre role et obstacle
	bool collisionBarrier(Barrier *barriers, Bullet *bullet);		// Collision entre obstacle et balle
	bool collisionBarrier(Role *role);


	void getNextPosition(int* x, int* y, Role *role);	// Trouver la position apres un mouvement

	void handleCollision(std::list<Bullet *> &bullets, SDL_Renderer *renderer, Role *eRole);	// Collision entre balle et enemie
	void handleCollision(SDL_Renderer *renderer, Role *pRole);									// Collision entre balle et joueur
	void handleCollision(std::list<Bullet *> &bullets, RoleType type, SDL_Renderer *renderer);	// Collision entre balle et obstacle
	// Collision entre les balles
	void handleCollision(std::list<Bullet *> &eBullets, std::list<Bullet *> &pBullets, SDL_Renderer *renderer);

	// Exposions
	void blast(std::vector<SDL_Texture *> &texs, Bullet *bullet, SDL_Renderer *renderer);
};