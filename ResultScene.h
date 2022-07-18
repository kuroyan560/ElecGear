#pragma once
#include"BaseScene.h"
#include"CharacterLogin.h"

class ResultScene :public BaseScene
{
	int debug = LoadGraph("resource/layout_result.png");

	CharacterLogin cutin;
	SceneTransition sceneTrans;

	int logoutBack = LoadGraph("resource/logout_back.png");
	int logoutStr = LoadGraph("resource/logout_str.png");
	int logoutScanLine = LoadGraph("resource/logout_scanline.png");
	float scanlineOffsetY = 0;

	const int WAIT_TIME = 20;
	int timer;
	int returnTitleNoise = LoadGraph("resource/returnTitleNoise.png");
	bool change;

	//int layout = LoadGraph("resource/layout_result.png");
	int resultChara = LoadGraph("resource/result_chara.png");
	int screenShadow = LoadGraph("resource/screen_shadow.png");
	int resultScanLine = LoadGraph("resource/result_scanline.png");
	float screenScanlineOffsetY = 0;

	//リザルト内容
	int numFont[10];
	int scoreGraph = LoadGraph("resource/resultStr_score.png");
	int killEnemyGraph = LoadGraph("resource/resultStr_killenemy.png");
	int maxComboGraph = LoadGraph("resource/resultStr_maxcombo.png");
	int thkForPlayGraph = LoadGraph("resource/thkForPlay.png");
	int retryGraph = LoadGraph("resource/resultStr_retry.png");
	int ynGraph[2] = {
		LoadGraph("resource/resultStr_yes.png"),
		LoadGraph("resource/resultStr_no.png")
	};

	int hashTagGraph = LoadGraph("resource/hashtag.png");

	const enum { YES, NO };
	int nowSelect;
	bool selectChange;
	int ynArrowGraph = LoadGraph("resource/ynArrow.png");
	int ynArrowAngle;

	struct Numbers {
		int digit;
		char splitNum[15];
	};
	Numbers score;
	Numbers killEnemy;
	Numbers maxCombo;

	int appearTimer;
	const enum {
		SCORE,KILLENEMY,MAXCOMBO,THK,RETRY,ITEM_NUM
	};
	int itemTimer[ITEM_NUM];

	int appearSE;
	int appearSEDelayTimer;
	int ynAppearSE;
	int selectSE;
	int enterSE;

	float arrowX = 296;
	float arrowY;

	bool ArrowHitCheck();

public:
	//コンストラクタ
	ResultScene(SceneChanger* changer);

	//初期化
	void Initialize()override;
	//終了処理
	void Finalize()override;
	//更新
	void Update()override;
	//描画
	void Draw()override;
};

