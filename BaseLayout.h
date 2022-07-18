#pragma once
#include"DxLib.h"
#include"Singleton.h"
class Vec2;
class Player;
class BaseLayout : public Singleton<BaseLayout>
{
	friend class Singleton<BaseLayout>;
	BaseLayout() {};

	int screenFrameGraph = LoadGraph("resource/screen_frame.png");								//スクリーンのフレームの画像
	int meterGraph = LoadGraph("resource/meter.png");												//メーターの画像
	int meterIndicaterGraph = LoadGraph("resource/meter_indicater.png");						//メーターの針の画像
	int backGroundGraph = LoadGraph("resource/hexagon.png");									//六角形の画像

	int screenShadowGraph = LoadGraph("resource/screen_shadow.png");	//スクリーン陰（乗算用）

	//メーターの針の角度
	float meterIndicaterAngle = 0;

	Player* player = nullptr;

public:
	void Initialize(Player* player);
	void MeterUpdate(float playerMoveLength);
	void DrawHexagon();
	void DrawScreenFrame(float playerMoveLength);
};

