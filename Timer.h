#pragma once
#include "Constants.h"
#include "DxLib.h"
#include "Vec2.h"
#include <stdio.h>

const float TIMER_SPEED = 0.1000000f;		//Œ¸­—Ê
const float MAX_TIMER = 100.0;				//Å‘åŠÔ
const int TIMER_FRAME = 3;					//‰½ƒtƒŒ[ƒ€‚Å0.1•b‚Æ‚·‚é‚©


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
