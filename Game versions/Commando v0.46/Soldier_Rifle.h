#ifndef __SOLDIER_RIFLE_H__
#define __SOLDIER_RIFLE_H__

#include "Enemy.h"
#include "Path.h"

class Enemy_SoldierRifle : public Enemy
{
private:
	float wave = -1.0f;
	bool going_up = true;

	int original_y = 0;
	int original_x = 0;

	Animation turn, center, left, more_left, right, more_right;

	int rand1 = 3, num_shots, space = 0;

public:

	Enemy_SoldierRifle(int x, int y);

	void Move();
};

#endif