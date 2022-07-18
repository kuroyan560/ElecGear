#include "Timer.h"

Timer::Timer()
{
	pos = { 50,80 };
	LoadDivGraph("resource/font.png", 11, 11, 1, 128, 128, font);
	size = 0.5f;
}

void Timer::Init(int maxTime)
{
	timer = TIMER_FRAME;
	nowTimer = maxTime;
}

void Timer::Update()
{
	if (timer <= 0) {
		timer = TIMER_FRAME;
		nowTimer -= TIMER_SPEED;
	}
	else
		timer--;

	if (nowTimer >= 0.0)
		sprintf_s(strNum, sizeof(strNum), "%d", (int)(nowTimer * 10.0000f));
}

void Timer::Draw()
{
	if (nowTimer >= 100)
		for (int i = 0; i < 4; i++) {
			if (i == 3)
				DrawRotaGraph(pos.x + FONT_SIZE / 1.6 * i, pos.y, size, 0.0f, font[strNum[i] - 48], true);
			else
				DrawRotaGraph(pos.x + FONT_SIZE / 2.0 * i, pos.y, size, 0.0f, font[strNum[i] - 48], true);
		}
	else if (nowTimer >= 10) {
		for (int i = 1; i < 4; i++) {
			if (i == 3)
				DrawRotaGraph(pos.x + FONT_SIZE / 1.6 * i, pos.y, size, 0.0f, font[strNum[i - 1] - 48], true);
			else
				DrawRotaGraph(pos.x + FONT_SIZE / 2.0 * i, pos.y, size, 0.0f, font[strNum[i - 1] - 48], true);
		}
	}

	else if (nowTimer >= 1) {
		for (int i = 2; i < 4; i++) {
			if (i == 3)
				DrawRotaGraph(pos.x + FONT_SIZE / 1.6 * i, pos.y, size, 0.0f, font[strNum[i - 2] - 48], true);
			else
				DrawRotaGraph(pos.x + FONT_SIZE / 2.0 * i, pos.y, size, 0.0f, font[strNum[i - 2] - 48], true);
		}
	}

	else {
		for (int i = 2; i < 4; i++) {
			if (i == 3)
				DrawRotaGraph(pos.x + FONT_SIZE / 1.6 * i, pos.y, size, 0.0f, font[strNum[i - 3] - 48], true);
			else
				DrawRotaGraph(pos.x + FONT_SIZE / 2.0 * i, pos.y, size, 0.0f, font[0], true);
		}
	}
	DrawRotaGraph(pos.x + FONT_SIZE / 2.0 * 3, pos.y, size, 0.0f, font[10], true);
}

bool Timer::EndTimer()
{
	if (nowTimer <= 0)
		return true;
	else
		return false;
}
