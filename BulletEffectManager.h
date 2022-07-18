#pragma once
#include"Vec2.h"
#include"Singleton.h"
#include<forward_list>
class BulletEffectManager : public Singleton<BulletEffectManager>
{
	friend class Singleton<BulletEffectManager>;

	int color;

	const int TOTAL_TIME = 30;
	const float RADIUS_RAND_MAX = 100;
	const float RADIUS_RAND_MIN = 60;
	const int THICKNESS_MAX = 30;
	struct BulletHit
	{
		Vec2 hitPos;
		float alpha;
		float raidus;
		float thickness;
		float maxRadius;
		int timer = 0;
	};

	BulletHit effects[BULLET_MAX];
	std::forward_list<BulletHit*>aliveEffects;

public:
	void Init();
	void Update();
	void Draw(Vec2 scrollAmount);
	void Emit(Vec2 pos);
};