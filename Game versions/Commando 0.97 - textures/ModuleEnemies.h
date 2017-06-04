#ifndef __ModuleEnemies_H__
#define __ModuleEnemies_H__

#include "Module.h"

#define MAX_ENEMIES 4000

enum ENEMY_TYPES
{
	NO_TYPE,
	SOLDIER_RIFLE,
	SOLDIER_GRENADE,
	SOLDIER_KNIFE,
	SOLDIER,
	SOLDIER_SHIELD,
	CRAZY_GREEN,
	PRISONER,
	PRISONERPOINTS,
	SOLDIERPRISONER,
	GRENADE,
	ITEM1,
	ITEM2,

	SOLDIER1,
	SOLDIER2,
	SOLDIER3
};

class Enemy;

struct EnemyInfo
{
	ENEMY_TYPES type = ENEMY_TYPES::NO_TYPE;
	int x, y;
};

class ModuleEnemies : public Module
{
public:

	ModuleEnemies();
	~ModuleEnemies();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);
	bool AddEnemy(ENEMY_TYPES type, int x, int y);
	int dieE;
	int cont;

	bool follow = false;

private:

	void SpawnEnemy(const EnemyInfo& info);

private:

	EnemyInfo queue[MAX_ENEMIES];
	Enemy* enemies[MAX_ENEMIES];

};

#endif // __ModuleEnemies_H__