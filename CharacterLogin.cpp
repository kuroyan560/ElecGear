#include "CharacterLogin.h"
#include"Easing.h"
using namespace Easing;
#include"DxLib.h"
#include"Constants.h"

#include"Vec2.h"
#include"SoundEmitter.h"

//通常ver
const int BOX_EXPAND_TOTAL_TIME = 60;	//広がり終わるまでの時間
const int WAIT_TOTAL_TIME = 30;				//広がりきったあとの待機時間
const int BOX_NARROW_TOTAL_TIME = 50;	//見えなくなるまでの時間
//ショートカットver
const int BOX_EXPAND_TOTAL_TIME_SHORT = 12;	//広がり終わるまでの時間
const int WAIT_TOTAL_TIME_SHORT = 19;				//広がりきったあとの待機時間
const int BOX_NARROW_TOTAL_TIME_SHORT = 10;	//見えなくなるまでの時間

//通常ver
const float MAX_EXPAND_SPEED = 30.0f;		//拡大スピード
//ショートカットver
const float MAX_EXPAND_SPEED_SHORT = 150.0f;		//拡大スピード

const int MAX_BLACK_ALPHA = 255;	//黒背景のアルファ

const int EME_GREEN = GetColor(120, 255, 169);		//コンセプトカラーのエメラルドグリーン
const int BLUE = GetColor(42, 2, 99);							//コンセプトカラーの紺色

const float MAX_GREEN_BAND_SPEED = 1.6f;	//緑帯の拡大スピード

const int CHARA_WIDTH = 968;	//キャラクター横幅

//通常ver
const float CHARA_APPEAR_SPEED = -58.0f;	//キャラクター移動の最大スピード
//ショートカットver
const float CHARA_APPEAR_SPEED_SHORT = -390.0f;	//キャラクター移動の最大スピード

const float CHARA_MIN_SPEED = -2.0f;	//キャラクター移動の最小スピード
const float CHARA_DISAPEEAR_SPEED = -200.0f;

const int LOGIN_WIDTH = 218;	//ログイン文字横幅
const int LOGIN_HEIGHT = 84;	//ログイン文字縦幅

//通常ver
const float LOGIN_APPEAR_SPEED = -29.0f;	//ログイン文字登場スピード
//ショートカットver
const float LOGIN_APPEAR_SPEED_SHORT = -175.0f;	//ログイン文字登場スピード

const float LOGIN_MIN_SPEED = -0.6f;	//ログイン文字最小スピード

const int RED_LINE_WIDTH = WIN_WIDTH;	//赤いライン横幅
const int RED_LINE_HEIGHT = 190;	//赤いライン縦幅

//通常ver
const float LINE_MASK_WIDTH_APPEAR_SPEED = -75.0f;	//赤いライン見え始め
//ショートカットver
const float LINE_MASK_WIDTH_APPEAR_SPEED_SHORT = -465.0f;	//赤いライン見え始め

const float LINE_MASK_WIDTH_MIN_SPEED = -2.0f;

void CharacterLogin::DrawCutInGraph(int x,int rectX, int width, int height, int graphHandle)
{
	DrawRectGraph(x, (WIN_HEIGHT - boxHeight) / 2, rectX, height / 2 - boxHeight / 2, width, boxHeight, graphHandle, true);
}

void CharacterLogin::DrawCutInGraph(int x, int y, int rectX, int width, int height, int graphHandle)
{
	if (height < boxHeight)
	{
		float lowY = HALF_WIN_HEIGHT - boxHeight / 2;
		float upY = HALF_WIN_HEIGHT + boxHeight / 2;
		float graphLowY = y;
		float graphUpY = y + height;

		if (lowY < graphUpY && graphLowY < upY)	//カットイン範囲内にいる
		{
			if (graphUpY < upY && lowY < graphLowY)	//カットイン範囲に入りきっている
			{
				DrawGraph(x, y, graphHandle, true);
			}
			else if (graphLowY < upY && upY < graphUpY)	//カットイン範囲から画像の下側がはみ出ている
			{
				DrawRectGraph(x, y, rectX, 0, width, upY - graphLowY, graphHandle, true);
			}
		}

		//映らない
	}
	else
	{
	}
}

CharacterLogin::CharacterLogin()
{
	characterHandle = LoadGraph("resource/cut_in_character.png");
	redLineHandle = LoadGraph("resource/cut_in_line.png");
	loginHandle = LoadGraph("resource/cut_in_login.png");

	bgmIntro = SoundEmitter::Instance()->LoadSound("resource/bgm_intro.wav", BGM_VOLUME);
	bgmShortIntro = SoundEmitter::Instance()->LoadSound("resource/bgm_intro_short.wav", BGM_VOLUME);
}

CharacterLogin::~CharacterLogin()
{
}

void CharacterLogin::Update()
{
	//拡大中
	if (flag == EXPAND)
	{
		//計測
		boxExpandTimer--;

		//更新
		int total = shortCut ? BOX_EXPAND_TOTAL_TIME_SHORT : BOX_EXPAND_TOTAL_TIME;
		int time = total - boxExpandTimer;
		expandSpeed = CircOut(time, total, shortCut ? MAX_EXPAND_SPEED_SHORT : MAX_EXPAND_SPEED, 0.0f);
		blackAlpha = CircOut(time, total, 0.0f, MAX_BLACK_ALPHA);
		bandSpeed = CircOut(time, total, MAX_GREEN_BAND_SPEED, 0.0f);
		charaMoveX = CircOut(time, total, shortCut ? CHARA_APPEAR_SPEED_SHORT : CHARA_APPEAR_SPEED, CHARA_MIN_SPEED);
		loginMoveY = CircOut(time, total, shortCut ? LOGIN_APPEAR_SPEED_SHORT : LOGIN_APPEAR_SPEED, LOGIN_MIN_SPEED);
		lineMaskWidthSpeed = CircOut(time, total, shortCut ? LINE_MASK_WIDTH_APPEAR_SPEED_SHORT : LINE_MASK_WIDTH_APPEAR_SPEED, LINE_MASK_WIDTH_MIN_SPEED);

		//次のフラグへ
		if (boxExpandTimer == 0)
		{
			waitTimer = shortCut ? WAIT_TOTAL_TIME_SHORT : WAIT_TOTAL_TIME;
			flag = WAIT;
		}
	}
	//待機中
	else if (flag == WAIT)
	{
		//計測
		waitTimer--;

		//次のフラグへ
		if (waitTimer == 0)
		{
			boxNarrowTimer = shortCut ? BOX_NARROW_TOTAL_TIME_SHORT : BOX_NARROW_TOTAL_TIME;
			maxHeight = boxHeight;
			maxBandHeight = bandHeight;
			lineMaskAppearWidth = lineMaskWidth;
			flag = NARROW;

			loginAppearY = loginY;
		}
	}
	//縮小中
	else if (flag == NARROW)
	{
		//計測
		boxNarrowTimer--;

		//更新
		int total = shortCut ? BOX_NARROW_TOTAL_TIME_SHORT : BOX_NARROW_TOTAL_TIME;
		int time = total - boxNarrowTimer;
		boxHeight = BackIn(time, total, maxHeight, 0.0f);
		blackAlpha = BackIn(time, total, MAX_BLACK_ALPHA, 0.0f);
		bandHeight = CircIn(time, total, maxBandHeight, 0.0f);
		charaMoveX = QuintInOut(time, total, CHARA_MIN_SPEED, CHARA_DISAPEEAR_SPEED);
		loginY = BackIn(time, total, loginAppearY, -LOGIN_HEIGHT);
		lineMaskWidth = QuintInOut(time, total, lineMaskAppearWidth, 0.0f);

		//終了
		if (boxNarrowTimer == 0)
		{
			flag = NONE;
			shortCut = true;	//一度だけ通常ver再生
		}
	}


	if (flag != NONE)
	{
		//四角の高さ加算
		if (flag != NARROW)
		{
			boxHeight += expandSpeed;
			bandHeight += bandSpeed;

			loginY += loginMoveY;

			//赤いライン見えてくる
			lineMaskWidth += lineMaskWidthSpeed;
		}

		//キャラ移動
		charaX += charaMoveX;
	}
}

void CharacterLogin::Draw()
{
	//デバッグ用
	//DrawGraph(0, 0, backHandle, true);

	//黒背景
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, blackAlpha);
	DrawBox(0, 0, WIN_WIDTH, WIN_HEIGHT, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//カットイン内の紺背景
	DrawBox(0, HALF_WIN_HEIGHT - boxHeight / 2, WIN_WIDTH, HALF_WIN_HEIGHT + boxHeight / 2, BLUE, true);

//カットイン
	//カットイン四角の上下Y
	float upY = HALF_WIN_HEIGHT - boxHeight / 2.0f;
	float lowY = HALF_WIN_HEIGHT + boxHeight / 2.0f;

	//赤いライン
	DrawCutInGraph(0,359, 0, RED_LINE_WIDTH, RED_LINE_HEIGHT, redLineHandle);
	//赤いライン専用マスク
	DrawBox(0, upY, lineMaskWidth, lowY, BLUE, true);

	//キャラクター
	DrawCutInGraph(charaX, 0, CHARA_WIDTH, WIN_HEIGHT, characterHandle);

	//ログイン文字
	DrawCutInGraph(76, loginY, 0, LOGIN_WIDTH, LOGIN_HEIGHT, loginHandle);

	//上下グリーン帯
	DrawBox(0, upY, WIN_WIDTH, upY + bandHeight, EME_GREEN, true);
	DrawBox(0, lowY - bandHeight, WIN_WIDTH, lowY, EME_GREEN, true);
}

void CharacterLogin::Start()
{
	//諸々パラメータの初期値
	flag = EXPAND;

	//カットイン四角
	boxHeight = 0;
	boxExpandTimer = shortCut ? BOX_EXPAND_TOTAL_TIME_SHORT : BOX_EXPAND_TOTAL_TIME;
	expandSpeed = shortCut ? MAX_EXPAND_SPEED_SHORT : MAX_EXPAND_SPEED;

	//カットイン上下の帯
	bandHeight = 0.0f;
	bandSpeed = MAX_GREEN_BAND_SPEED;

	//黒背景のアルファ
	blackAlpha = 0.0f;

	//カットイン内画像初期値
	charaX = WIN_WIDTH;
	loginY = WIN_HEIGHT;

	//赤いライン隠す四角の横幅
	lineMaskWidth = WIN_WIDTH;
	lineMaskWidthSpeed = shortCut ? LINE_MASK_WIDTH_APPEAR_SPEED_SHORT : LINE_MASK_WIDTH_APPEAR_SPEED;

	if (shortCut)
	{
		SoundEmitter::Instance()->Play(bgmShortIntro, DX_PLAYTYPE_BACK, true);
	}
	else
	{
		SoundEmitter::Instance()->Play(bgmIntro, DX_PLAYTYPE_BACK, true);
	}
}

bool CharacterLogin::ChangeTrigger()
{
	return waitTimer == (shortCut ? WAIT_TOTAL_TIME_SHORT : WAIT_TOTAL_TIME) && flag == WAIT;
}

bool CharacterLogin::End()
{
	return flag == NONE;
}
