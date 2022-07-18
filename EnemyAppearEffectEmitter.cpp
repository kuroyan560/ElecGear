#include "EnemyAppearEffectEmitter.h"

EnemyAppearEffectEmitter::EnemyAppearEffectEmitter()
{
}

void EnemyAppearEffectEmitter::Generate(Vec2 pos)
{
	for (int i = 0; i < MAX_EFFECT; i++) {
		if (enemyAppeareffect[i].flag) continue;
		enemyAppeareffect[i].Generate(pos);
		break;
	}
}

void EnemyAppearEffectEmitter::Update()
{
	for (int i = 0; i < MAX_EFFECT; i++) {
		if (!enemyAppeareffect[i].flag) continue;
		enemyAppeareffect[i].Update();
	}
}

void EnemyAppearEffectEmitter::Draw(Vec2 scroll)
{
	for (int i = 0; i < MAX_EFFECT; i++) {
		if (!enemyAppeareffect[i].flag) continue;
		enemyAppeareffect[i].Draw(scroll);
	}
}
