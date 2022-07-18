#pragma once
#include "Vec2.h"

class EnemyHitEffect{
public:
	Vec2 pos;				//���W
	float radius;			//���a
	float alpha;			//�A���t�@�l
	bool isAlive;			//�������Ă��邩�̃t���O

public:
	//�R���X�g���N�^
	EnemyHitEffect();

	//����������
	void Init();
	//��������
	void Generate(Vec2 pos);
	//�X�V����
	void Update();
	//�`�揈��
	void Draw(Vec2 scrollAmount, int graph);
};