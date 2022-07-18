#pragma once
#include "EnemyHitEffect.h"
#include "Singleton.h"
#include<forward_list>

class EnemyHitEffectManager : public Singleton<EnemyHitEffectManager>{
private:
	friend Singleton<EnemyHitEffectManager>;

	struct Explosion
	{
		static const int TOTAL_TIME_RAND_MIN = 10;
		static const int TOTAL_TIME_RAND_MAX = 30;
		static const int RADIUS_RAND_MAX = 50;
		static const int RADIUS_RAND_MIN = 30;
		static const int MAX_ALPHA = 190;
		Vec2 pos;
		float radius;
		int timer;
		float alpha;
		float maxRadius;
		int totalTime;
	};
	Explosion explosions[EFFECT_ENEMYDEAD_EXPLOSION_COUNT_MAX];
	std::forward_list<Explosion*>aliveExplosions;

	struct ExplosionEmitter
	{
		static const int EMITTER_LIFE = 60;
		static const int EMIT_SPAN = 5;
		static const int EMIT_POS_RAND_RANGE = 30;
		int timer;
		Vec2 pos;
	};
	int color;

	ExplosionEmitter explosionEmitter[EFFECT_ENEMYDEAD_EMITTER_COUNT_MAX];
	std::forward_list<ExplosionEmitter*>aliveExplosionEmitters;

	EnemyHitEffect enemyDamagedEffects[EFFECT_ENEMYHIT_COUNT_MAX];
	std::forward_list<EnemyHitEffect*>aliveEnemyDamagedEffects;
	int outerGraph;

	//�R���X�g���N�^
	EnemyHitEffectManager();

public:
	//����������
	void Init();
	//�_���[�W�G�t�F�N�g��������
	void DamagedEffect(Vec2 generatePos);
	//���S�G�t�F�N�g��������
	void DeadEffect(Vec2 generatePos);
	//�X�V����
	void Update();
	//�`�揈��
	void Draw(Vec2 scrollAmount);

};