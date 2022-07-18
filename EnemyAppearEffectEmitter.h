#pragma once
#include "EnemyAppearEffect.h"
#include "Singleton.h"

const int MAX_EFFECT = 50;
//const int MAX_TIMER = 10;

class EnemyAppearEffectEmitter :public Singleton<EnemyAppearEffectEmitter>
{
private:
	friend Singleton<EnemyAppearEffectEmitter>;
	EnemyAppearEffectEmitter();

public:
	EnemyAppearEffect enemyAppeareffect[MAX_EFFECT];

public:
	void Generate(Vec2 pos);
	void Update();
	void Draw(Vec2 scroll);
};

