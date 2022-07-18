#pragma once
#include"SceneTransition.h"
class CharacterLogin : public SceneTransition
{
	//時間計測用
	int boxExpandTimer = 0;
	int waitTimer = 0;
	int boxNarrowTimer = 0;

	//現在の状態フラグ
	const enum { NONE,EXPAND, WAIT, NARROW };
	int flag = 0;

	//カット四角の高さ
	float boxHeight = 0.0f;
	//拡大スピード
	float expandSpeed = 0.0f;
	//最大の高さ保存用
	float maxHeight = 0.0f;

	//黒背景のアルファ
	float blackAlpha = 0.0f;

	//グリーン上下帯の高さ
	float bandHeight = 0.0f;
	//高さの上昇スピード
	float bandSpeed = 0.0f;
	//最大の高さ保存用
	float maxBandHeight = 0.0f;


//画像
	//キャラ画像
	int characterHandle;
	//キャラX座標
	float charaX;
	//キャラ移動スピード
	float charaMoveX;

	//赤いライン
	int redLineHandle;
	float lineMaskWidthSpeed;
	//
	float lineMaskAppearWidth;
	//赤いラインを隠す四角の横幅
	int lineMaskWidth;

	//Login文字
	int loginHandle;
	//Login文字Y座標
	float loginY;
	//Login文字移動スピード
	float loginMoveY;
	//Login文字最終位置保存用
	float loginAppearY;

	int bgmIntro;
	int bgmShortIntro;

	//画像をカットイン型にくり抜いて描画
	void DrawCutInGraph(int x, int rectX, int width, int height, int graphHandle);
	void DrawCutInGraph(int x, int y, int rectX, int width, int height, int graphHandle);

public:

	bool shortCut = false;
	CharacterLogin();
	~CharacterLogin();

	//更新
	void Update()override;
	//描画
	virtual void Draw()override;
	//遷移開始
	virtual void Start()override;
	//シーンチェンジをする瞬間のトリガー判定
	virtual bool ChangeTrigger()override;
	//遷移が終了したかどうか
	virtual bool End()override;
};

