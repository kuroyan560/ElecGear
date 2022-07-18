#pragma once
#include "Vec2.h"
#include "Bullet.h"

const int MAXWAY = 10;
const int WAYANGLE = 60;

class WayBullet
{
public:
	Vec2 bulletPos[MAXWAY];
	float angle[MAXWAY];
	unsigned int Color;
	bool bulletFlag[MAXWAY];
	bool isAlive;

public:
	WayBullet();
	void Generate(Vec2 myPos, float angle);
	void Update(Vec2 scroolAmount);
	void Draw(Vec2 scroolAmount, int playerBulletGraph);
};
