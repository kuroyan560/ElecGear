#pragma once
#include "Singleton.h"
#include "DxLib.h"
#include "Vec2.h"
#include <vector>

using namespace std;

const int CONBO_TIMER = 120;				//�R���{���r�؂��t���[����
const int MAX_MAGNIFICATION = 5.0f;			//�ő�{��
const int MAX_HIT = 1;						//���̓���������{�����Z���邩
const float ADD_MAGNIFICAION = 0.05f;		//�{�����Z��
const Vec2 comboGaugeSize = Vec2(51, 316);	//�R���{�Q�[�W�摜�̃T�C�Y

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
	vector<ComboInfo> underAddScore;	//pair<���Z�����l�A���Z������>�̃R���e�i size�����Ɉړ����Ȃ���`�悷��
	float underAddScoreSize = 44;			//underAddScoreSize�̃t�H���g�T�C�Y
	int addAllScore = 0;					//���Z����鑍�X�R�A
	float bairituSize = 44;
	float allScoreSize = 118.0f / 2.0f;


	int comboGaugeOuterGraph;				//�R���{�Q�[�W�̊O���̉摜
	int comboGaugeInnerGraph;				//�R���{�Q�[�W�̓����̉摜
	int comboGaugeFont[14];					//�R���{�Q�[�W�̃t�H���g
	int comboGraph;							//combo�Ə����Ă���摜

	float comboFontSize = 118;		//�摜�T�C�Y
	float scoreFontSize = 22;		//�摜�T�C�Y�̔���
	char splitAllScore[25]{};				//�������������X�R�A�̎󂯎M
	char splitScore[8]{};					//�����������X�R�A�̎󂯎M


public:
	int maxCombo = 0;						//�ő�R���{��

private:
	Combo() {
		comboGaugeOuterGraph = LoadGraph("resource/combo_gauge_outer.png");
		comboGaugeInnerGraph = LoadGraph("resource/combo_gauge_inner.png");
		comboGraph = LoadGraph("resource/combo.png");
		LoadDivGraph("resource/font_combo.png", 13, 13, 1, 128, 128, comboGaugeFont);
	};

public:
	//������
	void Init() {
		comboTimer = 0;
		combo = 0;
		underAddScore.clear();
		hitCount = 0;
		scoreMagnification = 1.0f;
		addAllScore = 0;
		maxCombo = 0;
	}

	//�������ő�R���{���͏��������Ȃ��o�[�W����
	void Clear() {
		comboTimer = 0;
		combo = 0;
		underAddScore.clear();
		hitCount = 0;
		scoreMagnification = 1.0f;
		addAllScore = 0;
	}

	//�R���{�L������
	void Update() {
		if (comboTimer > 0)
			comboTimer--;

		//���ׂĂ�UI�̃T�C�Y���f�t�H���g�̗ʂɋ߂Â��� �܂��͑��X�R�A����
		allScoreSize += (ALLSCORE_SIZE - allScoreSize) / 10.0f;
		//���͔{��
		bairituSize += (BAIRITSU_SIZE - bairituSize) / 10.0f;
		//���̃X�R�A�����Ԃ�
		for (int i = 0; i < underAddScore.size(); ++i) {
			underAddScore[i].amoutSize += (AMOUNT_SIZE - underAddScore[i].amoutSize) / 10.0f;
		}

		if (combo > maxCombo) {
			maxCombo = combo;
		}
	}

	//�����������ɌĂ�
	void Hit(float score) {
		comboTimer = CONBO_TIMER;
		combo++;
		hitCount++;
		addAllScore += score;
		if (hitCount % MAX_HIT == 0) {
			scoreMagnification += ADD_MAGNIFICAION;
			//�{���T�C�Y�����Z
			bairituSize = BAIRITSU_SIZE + 10.0f;
		}
		//���X�R�A�T�C�Y�����Z
		allScoreSize = ALLSCORE_SIZE + 15.0f;

		//underAddScore���X�V����
		bool isHave = false;
		for (int i = 0; i < underAddScore.size(); ++i) {
			if (underAddScore[i].addScore == score) {
				isHave = true;
				underAddScore[i].amount++;
				if (underAddScore[i].amount >= 100) {
					underAddScore[i].amount = 99;
				}
				//�����X�R�A�T�C�Y�����Z
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

	//�_���[�W�󂯂���
	void Damage() {
		damageFlag = true;
		//�{���T�C�Y�����Z
		bairituSize = BAIRITSU_SIZE + 10.0f;
		//���X�R�A�T�C�Y�����Z
		allScoreSize = ALLSCORE_SIZE + 15.0f;
	}

	//�m�F
	bool ComboEndCheak() {
		if (comboTimer <= 0 || damageFlag) {
			damageFlag = false;
			Clear();
			return true;
		}
		else
			return false;
	}

	//�R���{�擾�p
	int GetCombo() {
		return combo;
	}
	int GetMaxConbo() {
		return maxCombo;
	}

	//�X�R�A�{���擾�p
	float GetScoreMagnification() {
		return scoreMagnification;
	}

	//�f�o�b�O�p
	void Draw() {
		/*
		DrawFormatString(100, 600, GetColor(255, 255, 255), "���̃R���{��:%d", combo, true);
		DrawFormatString(100, 620, GetColor(255, 255, 255), "�r�؂��܂ł̎���:%d", comboTimer, true);
		DrawFormatString(100, 640, GetColor(255, 255, 255), "�~%.2f", scoreMagnification, true);

		DrawLine(100, 660, 100 + comboTimer * 2, 660, GetColor(200, 200, 200), 5);*/

		//�R���{�Q�[�W��`��
		DrawExtendGraph(HALF_WIN_WIDTH * 1.37f, 20, HALF_WIN_WIDTH * 1.37f + comboGaugeSize.x, 20 + comboGaugeSize.y, comboGaugeOuterGraph, TRUE);
		DrawExtendGraph(HALF_WIN_WIDTH * 1.37f, 20 + (comboGaugeSize.y * ((float)(CONBO_TIMER - comboTimer) / CONBO_TIMER)),
			HALF_WIN_WIDTH * 1.37f + comboGaugeSize.x, 20 + comboGaugeSize.y,
			comboGaugeInnerGraph, TRUE);

		//�R���{�Ə����Ă���摜��`��
		//DrawGraph(HALF_WIN_WIDTH, comboFontSize + 10, comboGraph, TRUE);


		SetDrawBright(255, 44, 104);

		//���X�R�A����`�� �X�V�����������ł����Ⴂ�܂�
		int disit = to_string(addAllScore).length();
		if (addAllScore == 0) {
			disit = 1;
		}
		//���X�R�A�𕪊�
		sprintf_s(splitAllScore, sizeof(splitAllScore), "%d", addAllScore);
		for (int i = 0; i < disit; ++i) {
			splitAllScore[i] -= 48;
		}
		//���X�R�A��`��
		Vec2 comboDrawPos = Vec2(HALF_WIN_WIDTH * 1.4f - (comboFontSize / 2.0f * disit), 80);
		float comboFontZure = (comboFontSize / 2.0f);
		for (int i = 0; i < disit; ++i) {
			comboFontZure = (comboFontSize / 2.0f * (i));
			DrawExtendGraph(comboDrawPos.x - allScoreSize + comboFontZure,	//������������
				comboDrawPos.y - allScoreSize,
				comboDrawPos.x + allScoreSize + comboFontZure,	//������������
				comboDrawPos.y + allScoreSize,
				comboGaugeFont[splitAllScore[i]], TRUE);
		}

		//�{����`��
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

		//�R���{�̉��ɂ��邿������ȃX�R�A��`��
		for (int i = 0; i < underAddScore.size(); ++i) {
			//�X�R�A�̌��������߂�
			int scoreDisit = to_string((int)underAddScore[i].addScore).length();
			//���̌��������߂�
			int amountDisit = to_string((int)underAddScore[i].amoutSize).length();
			//�X�R�A�𕪊�����
			sprintf_s(splitScore, sizeof(splitScore), "%d", (int)underAddScore[i].addScore);
			for (int j = 0; j < scoreDisit; ++j) {
				splitScore[j] -= 48;
			}
			float offsetXPos = 50;
			//�ŏ���"+"��`�悵�Ă���X�R�A��`�悷��
			Vec2 plusPosY = Vec2(250 - underAddScoreSize / 2.0f + (i * (scoreFontSize + 20.0f)), 250 + underAddScoreSize / 2.0f + (i * (scoreFontSize + 20.0f)));
			DrawExtendGraph(HALF_WIN_WIDTH * 1.03f - underAddScoreSize / 2.0f + offsetXPos,
				plusPosY.x,
				HALF_WIN_WIDTH * 1.03f + underAddScoreSize / 2.0f + offsetXPos,
				plusPosY.y,
				comboGaugeFont[10], TRUE);
			//�����ăX�R�A��`��
			for (int j = 0; j < scoreDisit; ++j) {
				DrawExtendGraph(HALF_WIN_WIDTH * 1.03f - underAddScoreSize / 2.0f + ((j + 1.0f) * scoreFontSize) + offsetXPos,
					plusPosY.x,
					HALF_WIN_WIDTH * 1.03f + underAddScoreSize / 2.0f + ((j + 1.0f) * scoreFontSize) + offsetXPos,
					plusPosY.y,
					comboGaugeFont[splitScore[j]], TRUE);
			}
			//������"*"��`��
			Vec2 xPosX = Vec2(HALF_WIN_WIDTH * 1.2f - underAddScoreSize / 4.0f, HALF_WIN_WIDTH * 1.2f + underAddScoreSize / 4.0f);
			DrawExtendGraph(xPosX.x + offsetXPos,
				plusPosY.x + scoreFontSize,
				xPosX.y + offsetXPos,
				plusPosY.y,
				comboGaugeFont[11], TRUE);
			//���𕪊�����
			sprintf_s(splitScore, sizeof(splitScore), "%d", (int)underAddScore[i].amount);
			for (int j = 0; j < scoreDisit; ++j) {
				splitScore[j] -= 48;
			}
			//�����Č���`��
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

