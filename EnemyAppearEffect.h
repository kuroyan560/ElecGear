#pragma once
#include "DxLib.h"
#include "Vec2.h"

const int ALPHA_SPEED = 15;
const float SIZE_SPEED = 3.0f;
const float MAX_SIZE = 80.0f;

class EnemyAppearEffect
{
private:
	Vec2 pos;					//生成する座標
	int count;					//カウンター ちかちかさせるために使用する
	int alpha;					//アルファ値
	float size;					//サイズ

public:
	bool flag;					//生存しているかのフラグ

public:
	EnemyAppearEffect();
	void Generate(Vec2 pos);
	void Update();
	void Draw(Vec2 scrool);
	bool EndEffect();
};

