#pragma once
#include"DxLib.h"
#include"Vec2.h"

extern const Vec2 ENTRY_POINT_POS;
extern const float ENTRY_POINT_STR_FLOAT_HEIGT;
extern const float ENTRY_POINT_FLOATING_SPEED;
extern const int ENTRY_POINT_HP;

struct EntryPoint
{
	//エントリーポイント
	int entryInner = LoadGraph("resource/entryPoint_inner.png");
	int entryOuter = LoadGraph("resource/entryPoint_outer.png");
	int entryStr = LoadGraph("resource/entryPoint_str.png");

	float entryStrOffsetY = 0.0f;	//文字オフセットY
	float entryFloatAngle = 0.0f;	//文字浮遊

	//エントリーポイントHP
	int entryPointHp;

	//ヒットエフェクト
	int expandTimer = 0;
	float hexagonScaleInner = 1.0f;
	float hexagonScaleOuter = 1.0f;
	bool dead = false;
	float hexagonOuterAngle = 0.0f;
	float alpha = 255;

	//音
	int hitSE;
	int deadSE;

	bool slashed = false;

	bool OutOfScreen(Vec2 pos);

public:
	EntryPoint();
	void Init();
	void Damage();
	void Update();
	void Draw(Vec2 scrollAmount);

	bool CanVisible(Vec2 scrollAmount);
};

