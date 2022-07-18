#include "WayBullet.h"

WayBullet::WayBullet()
{
	for (int i = 0; i < MAXWAY; i++) {
		angle[i] = 0;
		bulletFlag[i] = false;
	}

	Color = GetColor(255, 255, 0);

	isAlive = false;
}

void WayBullet::Generate(Vec2 myPos, float angle)
{
	float waySpace = WAYANGLE / 180.0f * DX_PI_F / MAXWAY; //way�̊Ԋu�T�C�Y�i�ʓx�@�j

	for (int i = 0; i < MAXWAY; i++) {
		if (!bulletFlag[i]) {
			bulletPos[i] = Vec2(myPos.x + cos(angle - DX_PI_F / 1.8f) * 54, myPos.y + sin(angle - DX_PI_F / 1.8f) * 54);      //�Z�b�g
			this->angle[i] = angle - DX_PI_F / 2 - (WAYANGLE / 180.0f * DX_PI_F / 2) + (waySpace * i);
			bulletFlag[i] = true;
		}
	}
	isAlive = true;
}

void WayBullet::Update(Vec2 scroolAmount)
{
	for (int i = 0; i < MAXWAY; i++) {
		if (bulletFlag[i]) {
			bulletPos[i].x += /*BULLET_SPEED*/10 * cos(this->angle[i]);
			bulletPos[i].y += /*BULLET_SPEED*/10 * sin(this->angle[i]);
			if (bulletPos[i].x < scroolAmount.x || bulletPos[i].x > WIN_WIDTH + scroolAmount.x || bulletPos[i].y < scroolAmount.y || bulletPos[i].y > WIN_HEIGHT + scroolAmount.y) {
				bulletFlag[i] = false; //��ʊO
			}
		}
	}

	int aliveCount = 0;
	for(int i = 0; i < MAXWAY; ++i){
		if(bulletFlag[i] == false) continue;
		aliveCount++;
	}
	if(aliveCount <= 0){
		isAlive = false;
	}
}

void WayBullet::Draw(Vec2 scroolAmount, int playerBulletGraph)
{
	for (int i = 0; i < MAXWAY; i++) {
		if (bulletFlag[i]) {
			DrawCircle(bulletPos[i].x - scroolAmount.x, bulletPos[i].y - scroolAmount.y, BULLET_SIZE, Color, true);
			DrawRotaGraph(bulletPos[i].x - scroolAmount.x, bulletPos[i].y - scroolAmount.y, 1, angle[i], playerBulletGraph, TRUE);
		}
	}
}
