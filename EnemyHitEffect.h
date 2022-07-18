#pragma once
#include "Vec2.h"

class EnemyHitEffect{
public:
	Vec2 pos;				//座標
	float radius;			//半径
	float alpha;			//アルファ値
	bool isAlive;			//生存しているかのフラグ

public:
	//コンストラクタ
	EnemyHitEffect();

	//初期化処理
	void Init();
	//生成処理
	void Generate(Vec2 pos);
	//更新処理
	void Update();
	//描画処理
	void Draw(Vec2 scrollAmount, int graph);
};