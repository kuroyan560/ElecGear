#include "EnemyAppearEffect.h"

EnemyAppearEffect::EnemyAppearEffect()
{
	count = 0;
	alpha = 255;
	size = MAX_SIZE;
	flag = false;
}

void EnemyAppearEffect::Generate(Vec2 pos)
{
	this->pos = pos;
	count = 0;
	alpha = 255;
	size = MAX_SIZE;
	flag = true;
}

void EnemyAppearEffect::Update()
{
	count++;
	size -= SIZE_SPEED;
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	alpha -= ALPHA_SPEED;

	if (alpha <= 0) {
		flag = false;
	}
}

void EnemyAppearEffect::Draw(Vec2 scroll)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	if (count % 2 == 0) {
		DrawCircle(pos.x - scroll.x, pos.y - scroll.y, size, GetColor(255, 44, 105), false, 3);
		DrawCircle(pos.x - scroll.x, pos.y - scroll.y, size - 10.0, GetColor(255, 44, 105), false, 3);
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
}

bool EnemyAppearEffect::EndEffect()
{
	if (alpha <= 0)
		return true;
	else
		return false;
}
