#pragma once
#include "Bullet.h"
#include "Singleton.h"

class BulletManager : public Singleton<BulletManager>
{
private:

	friend Singleton<BulletManager>;

	int playerBulletGraph;						//弾の画像

	BulletManager() {
		playerBulletGraph = LoadGraph("resource/player_bullet.png");
	};

public:

	Bullet bullet[BULLET_MAX];					//弾
	int bulletGenerateTimer = 0;				//弾を生成する
	int bulletGenerateTimerDef = 0;				//球を生成する上限時間
	int wayGenerateCount = 0;					//way弾を生成する数

public:

	//初期化処理
	void Init();
	//更新処理
	void Update(Vec2 myPos, float angle, Vec2 scroolAmount, bool canShot, int bulletGenerateStates);
	//描画処理
	void Draw(Vec2 scroolAmount);
};

