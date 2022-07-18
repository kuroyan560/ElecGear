#pragma once
#include "Bullet.h"
#include "Singleton.h"

class BulletManager : public Singleton<BulletManager>
{
private:

	friend Singleton<BulletManager>;

	int playerBulletGraph;						//’e‚Ì‰æ‘œ

	BulletManager() {
		playerBulletGraph = LoadGraph("resource/player_bullet.png");
	};

public:

	Bullet bullet[BULLET_MAX];					//’e
	int bulletGenerateTimer = 0;				//’e‚ğ¶¬‚·‚é
	int bulletGenerateTimerDef = 0;				//‹…‚ğ¶¬‚·‚éãŒÀŠÔ
	int wayGenerateCount = 0;					//way’e‚ğ¶¬‚·‚é”

public:

	//‰Šú‰»ˆ—
	void Init();
	//XVˆ—
	void Update(Vec2 myPos, float angle, Vec2 scroolAmount, bool canShot, int bulletGenerateStates);
	//•`‰æˆ—
	void Draw(Vec2 scroolAmount);
};

