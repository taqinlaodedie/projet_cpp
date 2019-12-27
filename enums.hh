#pragma once

// Definir tous les enums

// Les variables d'obstacles
enum BarrierType
{
	NORMAL_BARRIER,	
	STEEL_BARRIER,
	GRASS_BARRIER,
	WATER_BARRIER
};
enum BarrierDir
{
	VERTICAL,
	HORIZONTAL
};

// Les variables de personnages
enum RoleMoveDir
{
	LEFT,
	UP,
	RIGHT,
	DOWN,
	NONE
};
enum RoleType
{
	NORMAL_ENEMY,
	STRONG_ENEMY,
	PLAYER,
};

// Les collisions
enum CollisionWithBulletType
{
	E_ROLE,	// Avec enemie
	P_ROLE,	// Avec joueur
	WALL,
	BARRIER,
	NOT_ALL
};