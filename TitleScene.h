#pragma once
#include"BaseScene.h"
#include"Player.h"
#include"Enemy.h"
#include"CharacterLogin.h"
#include"EntryPoint.h"
#include"HexagonFloor.h"
class TitleScene:public BaseScene
{
public:
	const enum TUTORIAL { HANDLE, ACCEL, SLASH, TUTORIAL_NUM };
private:
	Player player;

	//�`���[�g���A���p�G(�P�݂̂̂ŉ������������)
	//int enemyCircleOuter = LoadGraph("resource/enemy_circle_outer_demo.png");							//�G�̊O���̉摜
	//int enemyCircleInner = LoadGraph("resource/enemy_circle_inner_demo.png");							//�G�̓����̉摜
	//int enemyDirection = LoadGraph("resource/enemy_direction.png");								//�G�̐i�s�����̉摜
	//int enemyStr = LoadGraph("resource/tutorial_enemy.png");										//�G�ł��邱�Ƃ����������摜
	//Enemy enemy;

	//����ł��畜������܂ł̃t���[��
	int reviveTimer;

	//�摜
	int modeSimulate = LoadGraph("resource/mode_simulate.png");
	int modeSimulateScrollX;

	int logoutBack = LoadGraph("resource/logout_back.png");
	int logoutStr = LoadGraph("resource/logout_str.png");
	int logoutScanLine = LoadGraph("resource/logout_scanline.png");
	float scanlineOffsetY = 0;

	int titleLogo = LoadGraph("resource/title.png");

	int titleBGM;

	void DrawRightScrollGraph(int x, int y, int width, int height, int scrollWidth, int scrollX,int graphHandle);

	CharacterLogin cutin;

	//�G���g���[�|�C���g
	EntryPoint entryPoint;

	//��
	HexagonFloor hexagonFloor;

	//�`���[�g���A��
	bool tutorial = true;
	int tutorialNum = HANDLE;

	int tutorialGraph_controller[TUTORIAL_NUM] = {
		LoadGraph("resource/tutorial_handle.png"),
		LoadGraph("resource/tutorial_accel.png"),
		LoadGraph("resource/tutorial_slash.png")
	};
	int tutorialGraph_controller_small[TUTORIAL_NUM] = {
		LoadGraph("resource/tutorial_handle_small.png"),
		LoadGraph("resource/tutorial_accel_small.png"),
		LoadGraph("resource/tutorial_slash_small.png")
	};

	int tutorialGraph_keymou[TUTORIAL_NUM] = {
	LoadGraph("resource/tutorial_handle_keymou.png"),
	LoadGraph("resource/tutorial_accel_keymou.png"),
	LoadGraph("resource/tutorial_slash_keymou.png")
	};
	int tutorialGraph_keymou_small[TUTORIAL_NUM] = {
		LoadGraph("resource/tutorial_handle_keymou_small.png"),
		LoadGraph("resource/tutorial_accel_keymou_small.png"),
		LoadGraph("resource/tutorial_slash_keymou_small.png")
	};
	int* nowTutorialGraph = nullptr;
	int* nowTutorialSmallGraph = nullptr;

	bool tutorialInput;
	float tutorialBarWidthRate = 0.0f;
	int tutorialProceed;
	int tutorialTimer;
	float tutorialExp;
	bool expand;
	bool tutorialSkip;

	int tutorial_skip = LoadGraph("resource/tutorial_skip.png");
	int tutorial_skip_keymou = LoadGraph("resource/tutorial_skip_keymou.png");
	int tutorial_on_off = LoadGraph("resource/tutorial_on_off.png");
	int tutorial_on_off_keymou = LoadGraph("resource/tutorial_on_off_keymou.png");

	int tutorialSE;
	int tutorialOnOffSE;

	int arrowGraph = LoadGraph("resource/arrow.png");
	float arrowAlphaAngle = 0.0f;

	int x = 0;
	int y = 0;

public:
	//�R���X�g���N�^
	TitleScene(SceneChanger* changer);

	//������
	void Initialize()override;
	//�I������
	void Finalize()override;
	//�X�V
	void Update()override;
	//�`��
	void Draw()override;
};

