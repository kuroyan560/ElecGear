#pragma once
#include "Vec2.h"

class Bullet
{
public:
	Vec2 pos;			//弾の現在位置
	float angle;		//玉の移動する方向
	bool isAlive;		//生存フラグ
	int timer;

public:
	//コンストラクタ
	Bullet();
	//初期化処理
	void Init();
	//生成処理
	void Generate(Vec2 playerPos, float angle);
	//更新処理
	void Update(Vec2 scroolAmount);
	//描画処理
	void Draw(Vec2 scroolAmount, int playerBulletGraph);

	void Hit();
};

