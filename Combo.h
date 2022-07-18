#pragma once
#include "Singleton.h"
#include "DxLib.h"
#include "Vec2.h"
#include <vector>

using namespace std;

const int CONBO_TIMER = 120;				//コンボが途切れるフレーム数
const int MAX_MAGNIFICATION = 5.0f;			//最大倍率
const int MAX_HIT = 1;						//何体当たったら倍率加算するか
const float ADD_MAGNIFICAION = 0.05f;		//倍率加算量
const Vec2 comboGaugeSize = Vec2(51, 316);	//コンボゲージ画像のサイズ

const float ALLSCORE_SIZE = 118.0f / 2.0f;
const float BAIRITSU_SIZE = 44;
const float AMOUNT_SIZE = 22;

struct ComboInfo {
	float addScore = 0;
	int amount = 0;
	float amoutSize = 0;
};

class Combo :public Singleton<Combo>
{
private:
	friend Singleton<Combo>;

	int comboTimer = CONBO_TIMER;
	int combo = 0;
	int hitCount = 0;
	float scoreMagnification = 1.0f;
	bool damageFlag = false;
	vector<ComboInfo> underAddScore;	//pair<加算される値、加算される個数>のコンテナ size分下に移動しながら描画する
	float underAddScoreSize = 44;			//underAddScoreSizeのフォントサイズ
	int addAllScore = 0;					//加算される総スコア
	float bairituSize = 44;
	float allScoreSize = 118.0f / 2.0f;


	int comboGaugeOuterGraph;				//コンボゲージの外側の画像
	int comboGaugeInnerGraph;				//コンボゲージの内側の画像
	int comboGaugeFont[14];					//コンボゲージのフォント
	int comboGraph;							//comboと書いてある画像

	float comboFontSize = 118;		//画像サイズ
	float scoreFontSize = 22;		//画像サイズの半分
	char splitAllScore[25]{};				//分割した総合スコアの受け皿
	char splitScore[8]{};					//分割した下スコアの受け皿


public:
	int maxCombo = 0;						//最大コンボ数

private:
	Combo() {
		comboGaugeOuterGraph = LoadGraph("resource/combo_gauge_outer.png");
		comboGaugeInnerGraph = LoadGraph("resource/combo_gauge_inner.png");
		comboGraph = LoadGraph("resource/combo.png");
		LoadDivGraph("resource/font_combo.png", 13, 13, 1, 128, 128, comboGaugeFont);
	};

public:
	//初期化
	void Init() {
		comboTimer = 0;
		combo = 0;
		underAddScore.clear();
		hitCount = 0;
		scoreMagnification = 1.0f;
		addAllScore = 0;
		maxCombo = 0;
	}

	//初期化最大コンボ数は初期化しないバージョン
	void Clear() {
		comboTimer = 0;
		combo = 0;
		underAddScore.clear();
		hitCount = 0;
		scoreMagnification = 1.0f;
		addAllScore = 0;
	}

	//コンボ有効時間
	void Update() {
		if (comboTimer > 0)
			comboTimer--;

		//すべてのUIのサイズをデフォルトの量に近づける まずは総スコアから
		allScoreSize += (ALLSCORE_SIZE - allScoreSize) / 10.0f;
		//次は倍率
		bairituSize += (BAIRITSU_SIZE - bairituSize) / 10.0f;
		//下のスコアを順番に
		for (int i = 0; i < underAddScore.size(); ++i) {
			underAddScore[i].amoutSize += (AMOUNT_SIZE - underAddScore[i].amoutSize) / 10.0f;
		}

		if (combo > maxCombo) {
			maxCombo = combo;
		}
	}

	//当たった時に呼ぶ
	void Hit(float score) {
		comboTimer = CONBO_TIMER;
		combo++;
		hitCount++;
		addAllScore += score;
		if (hitCount % MAX_HIT == 0) {
			scoreMagnification += ADD_MAGNIFICAION;
			//倍率サイズを加算
			bairituSize = BAIRITSU_SIZE + 10.0f;
		}
		//総スコアサイズを加算
		allScoreSize = ALLSCORE_SIZE + 15.0f;

		//underAddScoreを更新する
		bool isHave = false;
		for (int i = 0; i < underAddScore.size(); ++i) {
			if (underAddScore[i].addScore == score) {
				isHave = true;
				underAddScore[i].amount++;
				if (underAddScore[i].amount >= 100) {
					underAddScore[i].amount = 99;
				}
				//したスコアサイズを加算
				underAddScore[i].amoutSize = AMOUNT_SIZE + 10.0f;
			}
		}
		if (isHave == false) {
			ComboInfo buff = {};
			buff.addScore = score;
			buff.amount = 1;
			buff.amoutSize = 22;
			underAddScore.push_back(buff);
		}
	}

	//ダメージ受けたら
	void Damage() {
		damageFlag = true;
		//倍率サイズを加算
		bairituSize = BAIRITSU_SIZE + 10.0f;
		//総スコアサイズを加算
		allScoreSize = ALLSCORE_SIZE + 15.0f;
	}

	//確認
	bool ComboEndCheak() {
		if (comboTimer <= 0 || damageFlag) {
			damageFlag = false;
			Clear();
			return true;
		}
		else
			return false;
	}

	//コンボ取得用
	int GetCombo() {
		return combo;
	}
	int GetMaxConbo() {
		return maxCombo;
	}

	//スコア倍率取得用
	float GetScoreMagnification() {
		return scoreMagnification;
	}

	//デバッグ用
	void Draw() {
		/*
		DrawFormatString(100, 600, GetColor(255, 255, 255), "今のコンボ数:%d", combo, true);
		DrawFormatString(100, 620, GetColor(255, 255, 255), "途切れるまでの時間:%d", comboTimer, true);
		DrawFormatString(100, 640, GetColor(255, 255, 255), "×%.2f", scoreMagnification, true);

		DrawLine(100, 660, 100 + comboTimer * 2, 660, GetColor(200, 200, 200), 5);*/

		//コンボゲージを描画
		DrawExtendGraph(HALF_WIN_WIDTH * 1.37f, 20, HALF_WIN_WIDTH * 1.37f + comboGaugeSize.x, 20 + comboGaugeSize.y, comboGaugeOuterGraph, TRUE);
		DrawExtendGraph(HALF_WIN_WIDTH * 1.37f, 20 + (comboGaugeSize.y * ((float)(CONBO_TIMER - comboTimer) / CONBO_TIMER)),
			HALF_WIN_WIDTH * 1.37f + comboGaugeSize.x, 20 + comboGaugeSize.y,
			comboGaugeInnerGraph, TRUE);

		//コンボと書いてある画像を描画
		//DrawGraph(HALF_WIN_WIDTH, comboFontSize + 10, comboGraph, TRUE);


		SetDrawBright(255, 44, 104);

		//総スコア数を描画 更新処理もここでしちゃいます
		int disit = to_string(addAllScore).length();
		if (addAllScore == 0) {
			disit = 1;
		}
		//総スコアを分割
		sprintf_s(splitAllScore, sizeof(splitAllScore), "%d", addAllScore);
		for (int i = 0; i < disit; ++i) {
			splitAllScore[i] -= 48;
		}
		//総スコアを描画
		Vec2 comboDrawPos = Vec2(HALF_WIN_WIDTH * 1.4f - (comboFontSize / 2.0f * disit), 80);
		float comboFontZure = (comboFontSize / 2.0f);
		for (int i = 0; i < disit; ++i) {
			comboFontZure = (comboFontSize / 2.0f * (i));
			DrawExtendGraph(comboDrawPos.x - allScoreSize + comboFontZure,	//無理やり微調整
				comboDrawPos.y - allScoreSize,
				comboDrawPos.x + allScoreSize + comboFontZure,	//無理やり微調整
				comboDrawPos.y + allScoreSize,
				comboGaugeFont[splitAllScore[i]], TRUE);
		}

		//倍率を描画
		int bairituDigit = to_string((int)((scoreMagnification) * 100.0f)).length();
		sprintf_s(splitAllScore, sizeof(splitAllScore), "%d", (int)((scoreMagnification + 0.01f) * 100.0f));
		for (int i = 0; i < bairituDigit; ++i) {
			splitAllScore[i] -= 48;
		}
		if (scoreMagnification <= 1.02f) {
			splitAllScore[2] = 0;
		}
		for (int i = 0; i < bairituDigit; ++i) {
			DrawExtendGraph(HALF_WIN_WIDTH * 1.17f + (bairituSize * 0.75f * i),
				150,
				HALF_WIN_WIDTH * 1.17f + bairituSize + (bairituSize * 0.75f * i),
				150 + bairituSize,
				comboGaugeFont[splitAllScore[i]], TRUE);
			if (i == 0) {
				DrawExtendGraph(HALF_WIN_WIDTH * 1.17f + (bairituSize * i) + bairituSize * 0.5f,
					150,
					HALF_WIN_WIDTH * 1.17f + bairituSize + (bairituSize * i) + bairituSize * 0.5f,
					150 + bairituSize,
					comboGaugeFont[12], TRUE);
				DrawExtendGraph(HALF_WIN_WIDTH * 1.17f + (bairituSize * i) - bairituSize * 0.75f,
					150,
					HALF_WIN_WIDTH * 1.17f + bairituSize + (bairituSize * i) - bairituSize * 0.75f,
					150 + bairituSize,
					comboGaugeFont[11], TRUE);
			}
		}

		SetDrawBright(120, 255, 169);

		//コンボの下にあるちっちゃなスコアを描画
		for (int i = 0; i < underAddScore.size(); ++i) {
			//スコアの桁数を求める
			int scoreDisit = to_string((int)underAddScore[i].addScore).length();
			//個数の桁数を求める
			int amountDisit = to_string((int)underAddScore[i].amoutSize).length();
			//スコアを分割する
			sprintf_s(splitScore, sizeof(splitScore), "%d", (int)underAddScore[i].addScore);
			for (int j = 0; j < scoreDisit; ++j) {
				splitScore[j] -= 48;
			}
			float offsetXPos = 50;
			//最初に"+"を描画してからスコアを描画する
			Vec2 plusPosY = Vec2(250 - underAddScoreSize / 2.0f + (i * (scoreFontSize + 20.0f)), 250 + underAddScoreSize / 2.0f + (i * (scoreFontSize + 20.0f)));
			DrawExtendGraph(HALF_WIN_WIDTH * 1.03f - underAddScoreSize / 2.0f + offsetXPos,
				plusPosY.x,
				HALF_WIN_WIDTH * 1.03f + underAddScoreSize / 2.0f + offsetXPos,
				plusPosY.y,
				comboGaugeFont[10], TRUE);
			//続いてスコアを描画
			for (int j = 0; j < scoreDisit; ++j) {
				DrawExtendGraph(HALF_WIN_WIDTH * 1.03f - underAddScoreSize / 2.0f + ((j + 1.0f) * scoreFontSize) + offsetXPos,
					plusPosY.x,
					HALF_WIN_WIDTH * 1.03f + underAddScoreSize / 2.0f + ((j + 1.0f) * scoreFontSize) + offsetXPos,
					plusPosY.y,
					comboGaugeFont[splitScore[j]], TRUE);
			}
			//続いて"*"を描画
			Vec2 xPosX = Vec2(HALF_WIN_WIDTH * 1.2f - underAddScoreSize / 4.0f, HALF_WIN_WIDTH * 1.2f + underAddScoreSize / 4.0f);
			DrawExtendGraph(xPosX.x + offsetXPos,
				plusPosY.x + scoreFontSize,
				xPosX.y + offsetXPos,
				plusPosY.y,
				comboGaugeFont[11], TRUE);
			//個数を分割する
			sprintf_s(splitScore, sizeof(splitScore), "%d", (int)underAddScore[i].amount);
			for (int j = 0; j < scoreDisit; ++j) {
				splitScore[j] -= 48;
			}
			//続いて個数を描画
			for (int j = 0; j < amountDisit; ++j) {
				DrawExtendGraph(xPosX.x + ((j + 1) * underAddScore[i].amoutSize) + offsetXPos,
					plusPosY.x + underAddScore[i].amoutSize / 2.0f,
					xPosX.y + ((j + 1) * underAddScore[i].amoutSize) + offsetXPos,
					plusPosY.y,
					comboGaugeFont[splitScore[j]], TRUE);
			}
		}
		SetDrawBright(255, 255, 255);

	}
};

