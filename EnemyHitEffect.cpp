#include "EnemyHitEffect.h"

EnemyHitEffect::EnemyHitEffect()
{
	pos.Init();
	radius = 0;
	alpha = 0;
	isAlive = false;
}

void EnemyHitEffect::Init()
{
	isAlive = false;
}

void EnemyHitEffect::Generate(Vec2 pos)
{
	this->pos = pos;
	radius = 30;
	alpha = 200;
	isAlive = true;
}

void EnemyHitEffect::Update()
{
	radius++;
	alpha -= 15;
	//アルファ値がマイナスになったら初期化
	if(alpha <= 0){
		Init();
	}
}

void EnemyHitEffect::Draw(Vec2 scrollAmount, int graph)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

	DrawExtendGraph(pos.x - radius - scrollAmount.x, pos.y - radius - scrollAmount.y,pos.x + radius - scrollAmount.x, pos.y + radius - scrollAmount.y,
		graph, TRUE);
}
