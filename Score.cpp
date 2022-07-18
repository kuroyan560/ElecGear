#include "Score.h"
#include"Constants.h"
#include"DxLib.h"
#include<string>

Score::Score()
{
	LoadDivGraph("resource/font.png", 10, 10, 1, 128, 128, font);

	score = 0;
	changeScore = 0;
	scoreAddFlag = false;
	addScoreBuff = 0;
	addScoreBuff2 = 0;
	enemyKillCount = 0;
}

void Score::Init()
{
	score = 0;
	changeScore = 0;
	scoreAddFlag = false;
	addScoreBuff = 0;
	addScoreBuff2 = 0;
	enemyKillCount = 0;
}

void Score::Update()
{
	//コンボの更新処理        戻るときはここまで
	Combo::Instance()->Update();

	//コンボ中ならchangeScoreに加算
	if (!Combo::Instance()->ComboEndCheak() > 0 && addScoreBuff) {
		////変更するスコア
		//changeScore += addScoreBuff / 10.0f;
		//float nowChangeScore = addScoreBuff / 10.0f;
		//addScoreBuff -= nowChangeScore;
		//if (addScoreBuff < 0)
		//	addScoreBuff = 0;
	}
	//コンボが途切れたら
	else if (!scoreAddFlag && addScoreBuff > 0 && Combo::Instance()->ComboEndCheak()) {
		addScoreBuff = addScoreBuff * Combo::Instance()->GetScoreMagnification();
		if (addScoreBuff > 0) {
			Combo::Instance()->Clear();
			scoreAddFlag = true;
			//サイズを大きくする
			fontSize = FONT_SIZE + 10.0f;
		}
	}

	//途切れた場合、scoreに加算
	if (scoreAddFlag) {
		//変更するスコア
		float changeScore = addScoreBuff / 10.0f;
		score += changeScore;
		addScoreBuff -= changeScore;

		if (addScoreBuff <= 0) {
			addScoreBuff = 0;
			changeScore = 0;
			scoreAddFlag = false;
		}
	}
	//スコアに上限をつける
	if (score >= 1000000000) {
		score = 999999999;
	}
	//桁数を調べる
	digit = std::to_string(score).length();
	//スコアを分割
	sprintf_s(splitScore, sizeof(splitScore), "%d", score);
	for (int i = 0; i < 10; ++i) {
		splitScore[i] -= 48;
	}

	//フォントサイズを近づける
	fontSize += (FONT_SIZE - fontSize) / 10.0f;
}

void Score::Draw()
{
	for (int i = 0; i < digit; ++i) {
		/*int zure = i * (fontSize / 2.0f) + (fontSize / 2.0f) * (digit / 2.0f);
		DrawExtendGraph(WIN_WIDTH - 310 - fontSize / 2.0f + zure,
			0,
			WIN_WIDTH - 310 + fontSize / 2.0f + zure,
			0 + fontSize,
			font[splitScore[i]], TRUE);*/

		float halfFontSize = fontSize / 2.0f;
		Vec2 startPos = Vec2(WIN_WIDTH - 160 - ((digit / 2.0f) * (halfFontSize)), 0);
		DrawExtendGraph(startPos.x - halfFontSize + (halfFontSize * i),
			startPos.y,
			startPos.x + halfFontSize + (halfFontSize * i),
			startPos.y + fontSize,
			font[splitScore[i]], TRUE);
	}
}

void Score::Add(int scoreAmount, int num)
{
	addScoreBuff += scoreAmount * num;
	enemyKillCount++;
}

void Score::GameSceneEnd()
{
	score += addScoreBuff * Combo::Instance()->GetScoreMagnification();
}
