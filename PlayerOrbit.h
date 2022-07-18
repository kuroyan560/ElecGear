#pragma once
#include"Vec2.h"
#include"Constants.h"
class PlayerOrbit
{
	const float WIDTH = 24.0f;
	const float HEIGHT = 28.0f;
	int graph;
	struct Orbit
	{
		float alpha;
		Vec2 pos;
		bool isAlive;
		float angle;
	};
	Orbit orbits[PLAYER_ORBIT_MAX];

	int emitTimer = 0;

public:
	PlayerOrbit();
	void Init();
	void Update(bool move, Vec2 playerPos, float angle);
	void Draw(Vec2 scrollAmount);
};