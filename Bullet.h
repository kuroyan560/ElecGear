#pragma once
#include "Vec2.h"

class Bullet
{
public:
	Vec2 pos;			//�e�̌��݈ʒu
	float angle;		//�ʂ̈ړ��������
	bool isAlive;		//�����t���O
	int timer;

public:
	//�R���X�g���N�^
	Bullet();
	//����������
	void Init();
	//��������
	void Generate(Vec2 playerPos, float angle);
	//�X�V����
	void Update(Vec2 scroolAmount);
	//�`�揈��
	void Draw(Vec2 scroolAmount, int playerBulletGraph);

	void Hit();
};

