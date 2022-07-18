#pragma once
#include"Singleton.h"
#include"Combo.h"

class Score : public Singleton<Score>
{
private:
	friend class Singleton<Score>;
	//スコア関連に使用
	int font[10];											//フォント
	float fontSize = FONT_SIZE;								//フォントサイズ
	float digit;											//スコアの桁数
	int score = 123456;										//スコア
	float changeScore;
	bool scoreAddFlag;
	int addScoreBuff = 0;									//スコアに加算する値をためておくバッファー
	int addScoreBuff2 = 0;									//スコアに加算する値をためておくバッファー
	char splitScore[25];									//スコアを分割した時の受け皿

	int enemyKillCount = 123;

	Score();

public:
	void Init();
	void Update();
	void Draw();
	void Add(int scoreAmount = 100, int num = 1);
	void GameSceneEnd();

	int GetEnemyKillCount() { return enemyKillCount; }
	int GetScore() { return score; }
};