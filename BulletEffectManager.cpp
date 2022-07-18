#include "BulletEffectManager.h"
#include"Easing.h"

void BulletEffectManager::Init()
{
	color = GetColor(255, 44, 105);
	//color = GetColor(120, 255, 169);
	aliveEffects.clear();
}

void BulletEffectManager::Update()
{
	for (auto itr : aliveEffects) {
		int time = TOTAL_TIME - itr->timer;
		itr->alpha = Easing::QuintIn(time, TOTAL_TIME, 230.0f, 0);
		itr->raidus = Easing::ExpOut(time, TOTAL_TIME, 0.0f, itr->maxRadius);
		itr->thickness = Easing::CircOut(time, TOTAL_TIME, THICKNESS_MAX, 3);
		itr->timer--;
	}

	//Šù‚ÉŽ€‚ñ‚Å‚¢‚½‚çÁ‚·
	aliveEffects.remove_if(
		[](BulletHit* x)	{
			return x->timer <= 0;
		});
}

void BulletEffectManager::Draw(Vec2 scrollAmount)
{
	for (auto itr : aliveEffects) {
		SetDrawBlendMode(DX_BLENDMODE_ADD, itr->alpha);
		DrawCircle(itr->hitPos.x - scrollAmount.x, itr->hitPos.y - scrollAmount.y, itr->raidus, color,false,itr->thickness);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void BulletEffectManager::Emit(Vec2 pos)
{
	for (int i = 0; i < BULLET_MAX;++i) {
		if (0 < effects[i].timer)continue;

		effects[i].alpha = 230.0f;
		effects[i].raidus = 0.0f;
		effects[i].hitPos = pos;
		effects[i].timer = TOTAL_TIME;
		effects[i].thickness = THICKNESS_MAX;
		effects[i].maxRadius = GetRand(RADIUS_RAND_MAX - RADIUS_RAND_MIN) + RADIUS_RAND_MIN;
		aliveEffects.push_front(&effects[i]);
		break;
	}
}