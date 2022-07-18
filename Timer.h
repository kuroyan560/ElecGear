#pragma once
#include "Constants.h"
#include "DxLib.h"
#include "Vec2.h"
#include <stdio.h>

const float TIMER_SPEED = 0.1000000f;		//������
const float MAX_TIMER = 100.0;				//�ő厞��
const int TIMER_FRAME = 3;					//���t���[����0.1�b�Ƃ��邩


class Timer
{
private:
	Vec2 pos;
	int timer;
	float nowTimer;
	int font[11];
	float size;

	char strNum[5];

public:
	Timer();
	void Init(int maxTime);
	void Update();
	void Draw();
	bool EndTimer();
};
