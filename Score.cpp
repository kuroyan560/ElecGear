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
	//�R���{�̍X�V����        �߂�Ƃ��͂����܂�
	Combo::Instance()->Update();

	//�R���{���Ȃ�changeScore�ɉ��Z
	if (!Combo::Instance()->ComboEndCheak() > 0 && addScoreBuff) {
		////�ύX����X�R�A
		//changeScore += addScoreBuff / 10.0f;
		//float nowChangeScore = addScoreBuff / 10.0f;
		//addScoreBuff -= nowChangeScore;
		//if (addScoreBuff < 0)
		//	addScoreBuff = 0;
	}
	//�R���{���r�؂ꂽ��
	else if (!scoreAddFlag && addScoreBuff > 0 && Combo::Instance()->ComboEndCheak()) {
		addScoreBuff = addScoreBuff * Combo::Instance()->GetScoreMagnification();
		if (addScoreBuff > 0) {
			Combo::Instance()->Clear();
			scoreAddFlag = true;
			//�T�C�Y��傫������
			fontSize = FONT_SIZE + 10.0f;
		}
	}

	//�r�؂ꂽ�ꍇ�Ascore�ɉ��Z
	if (scoreAddFlag) {
		//�ύX����X�R�A
		float changeScore = addScoreBuff / 10.0f;
		score += changeScore;
		addScoreBuff -= changeScore;

		if (addScoreBuff <= 0) {
			addScoreBuff = 0;
			changeScore = 0;
			scoreAddFlag = false;
		}
	}
	//�X�R�A�ɏ��������
	if (score >= 1000000000) {
		score = 999999999;
	}
	//�����𒲂ׂ�
	digit = std::to_string(score).length();
	//�X�R�A�𕪊�
	sprintf_s(splitScore, sizeof(splitScore), "%d", score);
	for (int i = 0; i < 10; ++i) {
		splitScore[i] -= 48;
	}

	//�t�H���g�T�C�Y���߂Â���
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
