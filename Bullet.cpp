#include "Bullet.h"
#include"BulletEffectManager.h"
#include"Easing.h"

Bullet::Bullet()
{
	angle = 0;
	isAlive = false;
}

void Bullet::Init()
{
	isAlive = false;
}

void Bullet::Generate(Vec2 myPos, float angle)
{
	//�����̊p�x�̓v���C���[�̐i�s�����̊p�x�Ȃ̂ŁA90�x���炷
	this->angle = angle - DX_PI_F / 1.95f;
	pos = Vec2(myPos.x + cos(this->angle) * 54, myPos.y + sin(this->angle) * 54);      //�Z�b�g
	isAlive = true;
	timer = 0;
}

void Bullet::Update(Vec2 scroolAmount)
{
	pos.x += Easing::CircIn(timer, BULLET_SPEED_TO_MAX_TIME,BULLET_SPEED_MIN,BULLET_SPEED_MAX) * cos(this->angle);
	pos.y += Easing::CircIn(timer, BULLET_SPEED_TO_MAX_TIME, BULLET_SPEED_MIN, BULLET_SPEED_MAX) * sin(this->angle);
	//��ʊO�֏o���珉��������
	if (pos.x < scroolAmount.x || pos.x > WIN_WIDTH + scroolAmount.x || pos.y < scroolAmount.y || pos.y > WIN_HEIGHT + scroolAmount.y) {
		Init();
	}

	if (timer < BULLET_SPEED_TO_MAX_TIME) {
		timer++;
	}
}

void Bullet::Draw(Vec2 scroolAmount, int playerBulletGraph)
{
	//DrawCircle(pos.x - scroolAmount.x, pos.y - scroolAmount.y, BULLET_SIZE, Color, true);
	DrawRotaGraph(pos.x - scroolAmount.x, pos.y - scroolAmount.y, CAMERA_RATE, angle, playerBulletGraph, TRUE);
}

void Bullet::Hit()
{
	isAlive = false;
	BulletEffectManager::Instance()->Emit(pos);
}
