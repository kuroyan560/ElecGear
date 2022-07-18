#include "EntryPoint.h"
#include"Constants.h"
#include"Easing.h"
#include"SoundEmitter.h"

const Vec2 ENTRY_POINT_POS = { (float)HALF_WIN_WIDTH,-150.0f };
const float ENTRY_POINT_STR_FLOAT_HEIGT = 8.0f;
const float ENTRY_POINT_FLOATING_SPEED = PI / 180.0f * 3;
const int ENTRY_POINT_HP = 10;

const int EXPAND_TOTAL_TIME = 20;
const float EXPAND_MAX_SCALE = 3.0f;
const int DEAD_EXPAND_TOTAL_TIME = 30;
const float DEAD_EXPAND_SCALE = 100.0f;

const float OUTER_ANGLE_MAX = PI * 2 * 2;

const int HEXAGON_SIZE = 120;

bool EntryPoint::OutOfScreen(Vec2 pos)
{
	if (pos.x + 64 < 0)return false;
	if (WIN_WIDTH < pos.x - 64)return false;
	if (pos.y + 64 < 0)return false;
	if (WIN_HEIGHT < pos.y - 64)return false;
	return true;
}

void EntryPoint::Init()
{
	entryPointHp = ENTRY_POINT_HP;
	expandTimer = 0;
	hexagonScaleOuter = 1.0f;
	hexagonScaleInner = 1.0f;
	dead = false;
	hexagonOuterAngle = 0.0f;
}

EntryPoint::EntryPoint()
{
	hitSE = SoundEmitter::Instance()->LoadSound("resource/entryPointHit.mp3", 150);
	deadSE = SoundEmitter::Instance()->LoadSound("resource/entryPointDead.mp3", 150);
}

void EntryPoint::Update()
{
	//エントリーポイントSTR
	entryFloatAngle += ENTRY_POINT_FLOATING_SPEED;
	if (PI * 2 <= entryFloatAngle)
	{
		entryFloatAngle -= PI * 2;
	}
	entryStrOffsetY = sin(entryFloatAngle) * ENTRY_POINT_STR_FLOAT_HEIGT;

	if (0 < expandTimer)
	{
		expandTimer--;

		int time;
		int totalTime;

		if (dead)
		{
			time = DEAD_EXPAND_TOTAL_TIME - expandTimer;
			totalTime = DEAD_EXPAND_TOTAL_TIME;
			hexagonScaleInner = Easing::CircIn(time, totalTime, 1.0f, DEAD_EXPAND_SCALE);
		}
		else
		{
			time = EXPAND_TOTAL_TIME - expandTimer;
			totalTime = EXPAND_TOTAL_TIME;
			hexagonScaleInner = Easing::BackOut(time, totalTime, EXPAND_MAX_SCALE, (float)entryPointHp / (float)ENTRY_POINT_HP);
			hexagonScaleOuter = Easing::BackOut(time, totalTime, EXPAND_MAX_SCALE, 1.0f);
			hexagonOuterAngle = Easing::ElasticOut(time, totalTime, OUTER_ANGLE_MAX, 0.0f);
		}
		alpha = Easing::BackInOut(time, totalTime, 50, 255);
	}
}

void EntryPoint::Damage()
{
	entryPointHp--;
	if (entryPointHp == 0) 
	{
		SoundEmitter::Instance()->Play(deadSE, DX_PLAYTYPE_BACK);
		dead = true;
		expandTimer = DEAD_EXPAND_TOTAL_TIME;
		hexagonScaleOuter = 1.0f;
		hexagonScaleInner = 1.0f;
	}
	else
	{
		SoundEmitter::Instance()->Play(hitSE, DX_PLAYTYPE_BACK);
		expandTimer = EXPAND_TOTAL_TIME;
		hexagonScaleInner = EXPAND_MAX_SCALE;
		hexagonScaleOuter = EXPAND_MAX_SCALE;
	}

	hexagonOuterAngle = OUTER_ANGLE_MAX;
	alpha = 50;
}

void EntryPoint::Draw(Vec2 scrollAmount)
{
	Vec2 pos;
	pos.x = ENTRY_POINT_POS.x - scrollAmount.x;
	pos.y = ENTRY_POINT_POS.y - scrollAmount.y;

	//エントリーポイント
	//float hpRate = (float)entryPointHp / (float)ENTRY_POINT_HP;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawRotaGraph(pos.x, pos.y, hexagonScaleInner, -hexagonOuterAngle, entryInner, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawRotaGraph(pos.x, pos.y, hexagonScaleOuter, hexagonOuterAngle, entryOuter, true);

	//エントリーポイントSTR
	DrawRotaGraph(pos.x, pos.y - 100 + entryStrOffsetY, 1.0f, 0.0f, entryStr, true);
}

bool EntryPoint::CanVisible(Vec2 scrollAmount)
{
	Vec2 pos;
	pos.x = ENTRY_POINT_POS.x - scrollAmount.x;
	pos.y = ENTRY_POINT_POS.y - scrollAmount.y;
	if (pos.x + HEXAGON_SIZE / 2 < 34)return false;
	if(935 < pos.x - HEXAGON_SIZE / 2)return false;
	if (pos.y + HEXAGON_SIZE / 2 < 12)return false;
	if (705 < pos.y - HEXAGON_SIZE / 2)return false;

	return true;
}
