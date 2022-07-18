#pragma once
#include "DxLib.h"
#include "Vec2.h"

const int ALPHA_SPEED = 15;
const float SIZE_SPEED = 3.0f;
const float MAX_SIZE = 80.0f;

class EnemyAppearEffect
{
private:
	Vec2 pos;					//����������W
	int count;					//�J�E���^�[ �������������邽�߂Ɏg�p����
	int alpha;					//�A���t�@�l
	float size;					//�T�C�Y

public:
	bool flag;					//�������Ă��邩�̃t���O

public:
	EnemyAppearEffect();
	void Generate(Vec2 pos);
	void Update();
	void Draw(Vec2 scrool);
	bool EndEffect();
};

