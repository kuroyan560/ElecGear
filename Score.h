#pragma once
#include"Singleton.h"
#include"Combo.h"

class Score : public Singleton<Score>
{
private:
	friend class Singleton<Score>;
	//�X�R�A�֘A�Ɏg�p
	int font[10];											//�t�H���g
	float fontSize = FONT_SIZE;								//�t�H���g�T�C�Y
	float digit;											//�X�R�A�̌���
	int score = 123456;										//�X�R�A
	float changeScore;
	bool scoreAddFlag;
	int addScoreBuff = 0;									//�X�R�A�ɉ��Z����l�����߂Ă����o�b�t�@�[
	int addScoreBuff2 = 0;									//�X�R�A�ɉ��Z����l�����߂Ă����o�b�t�@�[
	char splitScore[25];									//�X�R�A�𕪊��������̎󂯎M

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