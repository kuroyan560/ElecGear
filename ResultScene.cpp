#include "ResultScene.h"
#include"BaseLayout.h"
#include"Score.h"
#include"Combo.h"
#include"Easing.h"
#include"SoundEmitter.h"

#define NUM_FONT_WIDTH 30
#define NUM_FONT_HEIGHT 40
#define NUMBERS_X 385

#define TOTAL_SCORE_EXPAND_RATE 1.5f

#define APPEAR_SPAN 30
#define ITEM_APPEAR_TOTAL_TIME 50

#define APPEAR_SE_DELAY 10

#define FIXED_OFFSET_Y 50

//		SetDrawBright(255, 44, 104);
const int RED[3]{ 255,44,105 };
const int GREEN[3]{ 120,255,169 };

bool ResultScene::ArrowHitCheck()
{
	Vec2 mousePos = Input::GetMousePos();
	if (arrowX + 23 + 10 < mousePos.x)return false;
	if (mousePos.x < arrowX - 10)return false;
	if (arrowY + 99 + 10 < mousePos.y)return false;
	if (mousePos.y < arrowY - 10)return false;
	if (arrowY + 30 < mousePos.y && mousePos.y < arrowY + 99 - 30)return false;
	return true;
}

ResultScene::ResultScene(SceneChanger* changer)
	:BaseScene(changer)
{
	cutin.shortCut = true;
	LoadDivGraph("resource/resultNum.png", 10, 10, 1, NUM_FONT_WIDTH, NUM_FONT_HEIGHT, numFont);

	appearSE = SoundEmitter::Instance()->LoadSound("resource/slashSE_1.mp3",220);
	ynAppearSE = SoundEmitter::Instance()->LoadSound("resource/slash_gauge_heal_2.mp3", 200);
	selectSE = SoundEmitter::Instance()->LoadSound("resource/maxSlash.mp3",200);
	enterSE = SoundEmitter::Instance()->LoadSound("resource/entryPointDead.mp3",200);
}

void ResultScene::Initialize()
{
	BaseLayout::Instance()->Initialize(nullptr);
	BaseLayout::Instance()->MeterUpdate(0.0f);
	//ChangeFont("x14y24pxHeadUpDaisy", DX_CHARSET_DEFAULT);
	timer = 0;
	nowSelect = YES;
	change = false;
	ynArrowAngle = 0;

	//スコア
	score.digit = std::to_string(Score::Instance()->GetScore()).length();
	sprintf_s(score.splitNum, sizeof(score.splitNum), "%d", Score::Instance()->GetScore());
	for (int i = 0; i < 10; ++i) {
		score.splitNum[i] -= 48;
	}

	//倒した敵の数
	killEnemy.digit = std::to_string(Score::Instance()->GetEnemyKillCount()).length();
	sprintf_s(killEnemy.splitNum, sizeof(killEnemy.splitNum), "%d", Score::Instance()->GetEnemyKillCount());
	for (int i = 0; i < 10; ++i) {
		killEnemy.splitNum[i] -= 48;
	}

	//最大コンボ
	maxCombo.digit = std::to_string(Combo::Instance()->GetMaxConbo()).length();
	sprintf_s(maxCombo.splitNum, sizeof(maxCombo.splitNum), "%d", Combo::Instance()->GetMaxConbo());
	for (int i = 0; i < 10; ++i) {
		maxCombo.splitNum[i] -= 48;
	}

	timer = 0;
	for (int i = 0; i < ITEM_NUM; ++i) {
		itemTimer[i] = -1;
	}
	itemTimer[0] = 0;
	appearTimer = 0;
	
	selectChange = false;
	screenScanlineOffsetY = 0.0f;
	scanlineOffsetY = 0;

	appearSEDelayTimer = APPEAR_SE_DELAY;
}

void ResultScene::Finalize()
{
}

void ResultScene::Update()
{
	ynArrowAngle++;
	if (180 <= ynArrowAngle){
		ynArrowAngle = 0;
	}

	if (change && timer < WAIT_TIME) {
		timer++;
		if (timer == WAIT_TIME) {
			if (nowSelect == YES) {
				sceneChanger->ChangeScene(GAME_SCENE, &cutin);
			}
			else {
				sceneChanger->ChangeScene(TITLE_SCENE, &sceneTrans);
			}
		}
	}

	if (0 < appearSEDelayTimer) {
		appearSEDelayTimer--;
		if (appearSEDelayTimer == 0)	SoundEmitter::Instance()->Play(appearSE, DX_PLAYTYPE_BACK);
	}
	
	if (itemTimer[RETRY] == ITEM_APPEAR_TOTAL_TIME && (Input::isJoyBottomTrigger(XINPUT_BUTTON_A) || Input::isKeyTrigger(KEY_INPUT_SPACE)))
	{
		timer = 0;
		change = true;
		SoundEmitter::Instance()->Play(enterSE, DX_PLAYTYPE_BACK);
	}

	//LogOut欄走査線
	scanlineOffsetY += 0.3f;
	if (10 < scanlineOffsetY)scanlineOffsetY = 0;

	screenScanlineOffsetY += 0.3f;
	if (20 < screenScanlineOffsetY)screenScanlineOffsetY = 0;

	BaseLayout::Instance()->MeterUpdate(0.0f);

	for (int i = 0; i < ITEM_NUM; ++i) {
		if (0 <= itemTimer[i] && itemTimer[i] < ITEM_APPEAR_TOTAL_TIME) {
			itemTimer[i]++;
			if (i == RETRY && itemTimer[i] == ITEM_APPEAR_TOTAL_TIME) {
				SoundEmitter::Instance()->Play(ynAppearSE, DX_PLAYTYPE_BACK);
			}
		}
	}

	appearTimer++;
	if (appearTimer % APPEAR_SPAN == 0) {
		if (appearTimer / APPEAR_SPAN < ITEM_NUM) {
			itemTimer[appearTimer / APPEAR_SPAN] = 0;
			appearSEDelayTimer = APPEAR_SE_DELAY;
		}
	}

	arrowY = 373 + 6 * sin((PI / 180) * ynArrowAngle) + 100;

	bool flag = Input::Controller() ? (Input::GetJoyLeftStick().y < -0.8f || 0.8f < Input::GetJoyLeftStick().y) : (Input::isMouseBottomTrigger(MOUSE_INPUT_LEFT) && ArrowHitCheck());
	if (!selectChange && flag) {
		nowSelect = nowSelect == YES ? NO : YES;
		selectChange = true;
		SoundEmitter::Instance()->Play(selectSE, DX_PLAYTYPE_BACK);
	}
	else if (selectChange && Input::GetJoyLeftStick().y == 0.0f) {
		selectChange = false;
	}
}

void ResultScene::Draw()
{
	//DrawGraph(0, 0, debug, true);
	//DrawGraph(0, 0, layout, true);

	if (change && nowSelect == NO) {
		//DrawGraph(0, 0, returnTitleNoise, true);
		DrawBox(0, 0, 953, WIN_HEIGHT, GetColor(0, 0, 0), true);
	}

	//DrawString(0, 0, "123456 あおいえ SCORE ABC", GetColor(255, 44, 105));
	//DrawStringToHandle(0, 0, "123456 あおいえ SCORE ABC", GetColor(255, 44, 105),font);
	//if (!change) {
	//	DrawFormatStringToHandle(236, 297, GetColor(120, 255, 169), font, "TOTAL SCORE %d\n KILL ENEMY %d\n  MAX COMBO %d",
	//		Score::Instance()->GetScore(), Score::Instance()->GetEnemyKillCount(), Combo::Instance()->GetCombo());
	//}
	
	//リザルト
	if (!(change && nowSelect == NO)) {

		DrawGraph(30, screenScanlineOffsetY, resultScanLine, true);
		DrawGraph(0, 0, screenShadow, true);

		DrawGraph(0, 0, resultChara, true);

		float offsetX;

		//スコア
		SetDrawBright(RED[0], RED[1], RED[2]);
		if (0 <= itemTimer[SCORE]) {

			offsetX = Easing::ExpOut(itemTimer[SCORE], ITEM_APPEAR_TOTAL_TIME, WIN_WIDTH, 0.0f);
			//DrawGraph(49 + offsetX, 43, scoreGraph, true);
			DrawRotaGraph(49 + offsetX + 75 * TOTAL_SCORE_EXPAND_RATE, 43 + 20 * TOTAL_SCORE_EXPAND_RATE, TOTAL_SCORE_EXPAND_RATE, 0.0f, scoreGraph, true);

			for (int i = 0; i < score.digit; ++i) {
				//DrawGraph(NUMBERS_X + NUM_FONT_WIDTH * i + offsetX, 43, numFont[score.splitNum[i]], true);
				float x1 = NUMBERS_X + NUM_FONT_WIDTH * TOTAL_SCORE_EXPAND_RATE * i + offsetX;
				DrawExtendGraph(x1, 43, x1 + NUM_FONT_WIDTH * TOTAL_SCORE_EXPAND_RATE, 43 + NUM_FONT_HEIGHT * TOTAL_SCORE_EXPAND_RATE, numFont[score.splitNum[i]], true);
			}
		}

		SetDrawBright(GREEN[0], GREEN[1], GREEN[2]);
		//倒した敵の数
		if (0 <= itemTimer[KILLENEMY]) {
			offsetX = Easing::ExpOut(itemTimer[KILLENEMY], ITEM_APPEAR_TOTAL_TIME, WIN_WIDTH, 0.0f);
			DrawGraph(49 + offsetX, 103 + FIXED_OFFSET_Y, killEnemyGraph, true);
			for (int i = 0; i < killEnemy.digit; ++i) {
				DrawGraph(NUMBERS_X + NUM_FONT_WIDTH * i + offsetX, 103 + FIXED_OFFSET_Y, numFont[killEnemy.splitNum[i]], true);
			}
		}

		//最大コンボ
		if (0 <= itemTimer[MAXCOMBO]) {
			offsetX = Easing::ExpOut(itemTimer[MAXCOMBO], ITEM_APPEAR_TOTAL_TIME, WIN_WIDTH, 0.0f);
			DrawGraph(49 + offsetX, 162 + FIXED_OFFSET_Y, maxComboGraph, true);
			for (int i = 0; i < maxCombo.digit; ++i) {
				DrawGraph(NUMBERS_X + NUM_FONT_WIDTH * i + offsetX, 162 + FIXED_OFFSET_Y, numFont[maxCombo.splitNum[i]], true);
			}
		}
		SetDrawBright(255, 255, 255);

		if (0 <= itemTimer[THK]) {
			//Thank you ~
			DrawGraph(49 + Easing::ExpOut(itemTimer[THK], ITEM_APPEAR_TOTAL_TIME, WIN_WIDTH, 0.0f), 284, thkForPlayGraph, true);
		}

		if (0 <= itemTimer[RETRY]) {
			//RETRY?
			DrawGraph(49 + Easing::ExpOut(itemTimer[RETRY], ITEM_APPEAR_TOTAL_TIME, WIN_WIDTH, 0.0f), 404 + 100, retryGraph, true);
		}

		if (itemTimer[RETRY] == ITEM_APPEAR_TOTAL_TIME) {
			DrawGraph(45, 404, hashTagGraph, true);
			if (GetRand(150) % 149 != 0)
			{
				//矢印
				DrawGraph(arrowX, arrowY, ynArrowGraph, true);

				//Yes_ or No_
				DrawRotaGraph(311, 423 + 100, 1.0f, 0.0f, ynGraph[nowSelect], true);
			}
		}
	}

	//ログアウト画面
	DrawGraph(978, 95, logoutBack, true);
	DrawGraph(978, 95 + scanlineOffsetY, logoutScanLine, true);
	DrawGraph(978, 95, logoutStr, true);

	//スクリーンフレームの描画処理
	BaseLayout::Instance()->DrawScreenFrame(0.0f);

	//DrawFormatString(0, 0, GetColor(255, 255, 255), "x %d, y %d", x, y);
}