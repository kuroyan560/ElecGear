#pragma once
#include"DxLib.h"
#include"Singleton.h"
class Vec2;
class Player;
class BaseLayout : public Singleton<BaseLayout>
{
	friend class Singleton<BaseLayout>;
	BaseLayout() {};

	int screenFrameGraph = LoadGraph("resource/screen_frame.png");								//�X�N���[���̃t���[���̉摜
	int meterGraph = LoadGraph("resource/meter.png");												//���[�^�[�̉摜
	int meterIndicaterGraph = LoadGraph("resource/meter_indicater.png");						//���[�^�[�̐j�̉摜
	int backGroundGraph = LoadGraph("resource/hexagon.png");									//�Z�p�`�̉摜

	int screenShadowGraph = LoadGraph("resource/screen_shadow.png");	//�X�N���[���A�i��Z�p�j

	//���[�^�[�̐j�̊p�x
	float meterIndicaterAngle = 0;

	Player* player = nullptr;

public:
	void Initialize(Player* player);
	void MeterUpdate(float playerMoveLength);
	void DrawHexagon();
	void DrawScreenFrame(float playerMoveLength);
};

