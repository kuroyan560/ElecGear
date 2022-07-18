#pragma once
#include "Bullet.h"
#include "Singleton.h"

class BulletManager : public Singleton<BulletManager>
{
private:

	friend Singleton<BulletManager>;

	int playerBulletGraph;						//�e�̉摜

	BulletManager() {
		playerBulletGraph = LoadGraph("resource/player_bullet.png");
	};

public:

	Bullet bullet[BULLET_MAX];					//�e
	int bulletGenerateTimer = 0;				//�e�𐶐�����
	int bulletGenerateTimerDef = 0;				//���𐶐�����������
	int wayGenerateCount = 0;					//way�e�𐶐����鐔

public:

	//����������
	void Init();
	//�X�V����
	void Update(Vec2 myPos, float angle, Vec2 scroolAmount, bool canShot, int bulletGenerateStates);
	//�`�揈��
	void Draw(Vec2 scroolAmount);
};

