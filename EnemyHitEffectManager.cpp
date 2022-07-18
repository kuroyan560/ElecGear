#include "EnemyHitEffectManager.h"
#include"Easing.h"

EnemyHitEffectManager::EnemyHitEffectManager()
{
	outerGraph = LoadGraph("resource/enemy_circle_outer.png");
}

void EnemyHitEffectManager::Init()
{
	//for (int i = 0; i < EFFECT_ENEMYHIT_COUNT_MAX; ++i) {
	//	enemyDamagedEffects[i].Init();
	//}
	aliveEnemyDamagedEffects.clear();
	aliveExplosions.clear();
	aliveExplosionEmitters.clear();
	color = GetColor(255, 44, 105);
}

void EnemyHitEffectManager::DamagedEffect(Vec2 generatePos)
{
	for (int i = 0; i < EFFECT_ENEMYHIT_COUNT_MAX; ++i) {
		if (enemyDamagedEffects[i].isAlive) continue;

		enemyDamagedEffects[i].Generate(generatePos);
		aliveEnemyDamagedEffects.push_front(&enemyDamagedEffects[i]);
		break;
	}
}

void EnemyHitEffectManager::DeadEffect(Vec2 generatePos)
{
	for (int i = 0; i < EFFECT_ENEMYDEAD_EMITTER_COUNT_MAX; ++i) {
		if (0 < explosionEmitter[i].timer)continue;
		explosionEmitter[i].pos = generatePos;
		explosionEmitter[i].timer = ExplosionEmitter::EMITTER_LIFE;
		aliveExplosionEmitters.push_front(&explosionEmitter[i]);
		break;
	}
}

void EnemyHitEffectManager::Update()
{
	//ダメージエフェクト
	for (auto itr : aliveEnemyDamagedEffects) {
		itr->Update();
	}
	//既に死んでいたら消す
	aliveEnemyDamagedEffects.remove_if(
		[](EnemyHitEffect* x) {
			return !x->isAlive;
		});

	//死亡エフェクトエミッター
	for (auto itr : aliveExplosionEmitters) {
		if (itr->timer % ExplosionEmitter::EMIT_SPAN == 0) {
			//爆発
			for (int i = 0; i < EFFECT_ENEMYDEAD_EXPLOSION_COUNT_MAX; ++i) {
				if (0 < explosions[i].timer)continue;
				explosions[i].pos = itr->pos;
				explosions[i].pos.x += GetRand(ExplosionEmitter::EMIT_POS_RAND_RANGE * 2) - ExplosionEmitter::EMIT_POS_RAND_RANGE;
				explosions[i].pos.y += GetRand(ExplosionEmitter::EMIT_POS_RAND_RANGE * 2) - ExplosionEmitter::EMIT_POS_RAND_RANGE;
				explosions[i].radius = 0.0f;
				explosions[i].totalTime = GetRand(Explosion::TOTAL_TIME_RAND_MAX - Explosion::TOTAL_TIME_RAND_MIN) + Explosion::TOTAL_TIME_RAND_MIN;
				explosions[i].timer = explosions[i].totalTime;
				explosions[i].alpha = Explosion::MAX_ALPHA;
				explosions[i].maxRadius = GetRand(Explosion::RADIUS_RAND_MAX - Explosion::RADIUS_RAND_MIN) + Explosion::RADIUS_RAND_MIN;
				aliveExplosions.push_front(&explosions[i]);
				break;
			}
		}
		itr->timer--;
	}
	//既に死んでいたら消す
	aliveExplosionEmitters.remove_if(
		[](ExplosionEmitter* x) {
			return x->timer == 0;
		});

	//死亡爆発エフェクト
	for (auto itr : aliveExplosions) {
		int time = itr->totalTime - itr->timer;
		itr->radius = Easing::ExpOut(time, itr->totalTime, 0.0f, itr->maxRadius);
		itr->alpha = Easing::CircIn(time, itr->totalTime, Explosion::MAX_ALPHA, 0.0f);
		itr->timer--;
	}
	//既に死んでいたら消す
	aliveExplosions.remove_if(
		[](Explosion* x) {
			return x->timer == 0;
		});
}

void EnemyHitEffectManager::Draw(Vec2 scrollAmount)
{
	for (auto itr : aliveExplosions) {
		SetDrawBlendMode(DX_BLENDMODE_ADD, itr->alpha);
		DrawCircle(itr->pos.x - scrollAmount.x, itr->pos.y - scrollAmount.y, itr->radius, color);
	}
	for (auto itr : aliveEnemyDamagedEffects) {
		itr->Draw(scrollAmount, outerGraph);
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
