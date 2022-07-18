#include "CharacterLogin.h"
#include"Easing.h"
using namespace Easing;
#include"DxLib.h"
#include"Constants.h"

#include"Vec2.h"
#include"SoundEmitter.h"

//�ʏ�ver
const int BOX_EXPAND_TOTAL_TIME = 60;	//�L����I���܂ł̎���
const int WAIT_TOTAL_TIME = 30;				//�L���肫�������Ƃ̑ҋ@����
const int BOX_NARROW_TOTAL_TIME = 50;	//�����Ȃ��Ȃ�܂ł̎���
//�V���[�g�J�b�gver
const int BOX_EXPAND_TOTAL_TIME_SHORT = 12;	//�L����I���܂ł̎���
const int WAIT_TOTAL_TIME_SHORT = 19;				//�L���肫�������Ƃ̑ҋ@����
const int BOX_NARROW_TOTAL_TIME_SHORT = 10;	//�����Ȃ��Ȃ�܂ł̎���

//�ʏ�ver
const float MAX_EXPAND_SPEED = 30.0f;		//�g��X�s�[�h
//�V���[�g�J�b�gver
const float MAX_EXPAND_SPEED_SHORT = 150.0f;		//�g��X�s�[�h

const int MAX_BLACK_ALPHA = 255;	//���w�i�̃A���t�@

const int EME_GREEN = GetColor(120, 255, 169);		//�R���Z�v�g�J���[�̃G�������h�O���[��
const int BLUE = GetColor(42, 2, 99);							//�R���Z�v�g�J���[�̍��F

const float MAX_GREEN_BAND_SPEED = 1.6f;	//�Αт̊g��X�s�[�h

const int CHARA_WIDTH = 968;	//�L�����N�^�[����

//�ʏ�ver
const float CHARA_APPEAR_SPEED = -58.0f;	//�L�����N�^�[�ړ��̍ő�X�s�[�h
//�V���[�g�J�b�gver
const float CHARA_APPEAR_SPEED_SHORT = -390.0f;	//�L�����N�^�[�ړ��̍ő�X�s�[�h

const float CHARA_MIN_SPEED = -2.0f;	//�L�����N�^�[�ړ��̍ŏ��X�s�[�h
const float CHARA_DISAPEEAR_SPEED = -200.0f;

const int LOGIN_WIDTH = 218;	//���O�C����������
const int LOGIN_HEIGHT = 84;	//���O�C�������c��

//�ʏ�ver
const float LOGIN_APPEAR_SPEED = -29.0f;	//���O�C�������o��X�s�[�h
//�V���[�g�J�b�gver
const float LOGIN_APPEAR_SPEED_SHORT = -175.0f;	//���O�C�������o��X�s�[�h

const float LOGIN_MIN_SPEED = -0.6f;	//���O�C�������ŏ��X�s�[�h

const int RED_LINE_WIDTH = WIN_WIDTH;	//�Ԃ����C������
const int RED_LINE_HEIGHT = 190;	//�Ԃ����C���c��

//�ʏ�ver
const float LINE_MASK_WIDTH_APPEAR_SPEED = -75.0f;	//�Ԃ����C�������n��
//�V���[�g�J�b�gver
const float LINE_MASK_WIDTH_APPEAR_SPEED_SHORT = -465.0f;	//�Ԃ����C�������n��

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

		if (lowY < graphUpY && graphLowY < upY)	//�J�b�g�C���͈͓��ɂ���
		{
			if (graphUpY < upY && lowY < graphLowY)	//�J�b�g�C���͈͂ɓ��肫���Ă���
			{
				DrawGraph(x, y, graphHandle, true);
			}
			else if (graphLowY < upY && upY < graphUpY)	//�J�b�g�C���͈͂���摜�̉������͂ݏo�Ă���
			{
				DrawRectGraph(x, y, rectX, 0, width, upY - graphLowY, graphHandle, true);
			}
		}

		//�f��Ȃ�
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
	//�g�咆
	if (flag == EXPAND)
	{
		//�v��
		boxExpandTimer--;

		//�X�V
		int total = shortCut ? BOX_EXPAND_TOTAL_TIME_SHORT : BOX_EXPAND_TOTAL_TIME;
		int time = total - boxExpandTimer;
		expandSpeed = CircOut(time, total, shortCut ? MAX_EXPAND_SPEED_SHORT : MAX_EXPAND_SPEED, 0.0f);
		blackAlpha = CircOut(time, total, 0.0f, MAX_BLACK_ALPHA);
		bandSpeed = CircOut(time, total, MAX_GREEN_BAND_SPEED, 0.0f);
		charaMoveX = CircOut(time, total, shortCut ? CHARA_APPEAR_SPEED_SHORT : CHARA_APPEAR_SPEED, CHARA_MIN_SPEED);
		loginMoveY = CircOut(time, total, shortCut ? LOGIN_APPEAR_SPEED_SHORT : LOGIN_APPEAR_SPEED, LOGIN_MIN_SPEED);
		lineMaskWidthSpeed = CircOut(time, total, shortCut ? LINE_MASK_WIDTH_APPEAR_SPEED_SHORT : LINE_MASK_WIDTH_APPEAR_SPEED, LINE_MASK_WIDTH_MIN_SPEED);

		//���̃t���O��
		if (boxExpandTimer == 0)
		{
			waitTimer = shortCut ? WAIT_TOTAL_TIME_SHORT : WAIT_TOTAL_TIME;
			flag = WAIT;
		}
	}
	//�ҋ@��
	else if (flag == WAIT)
	{
		//�v��
		waitTimer--;

		//���̃t���O��
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
	//�k����
	else if (flag == NARROW)
	{
		//�v��
		boxNarrowTimer--;

		//�X�V
		int total = shortCut ? BOX_NARROW_TOTAL_TIME_SHORT : BOX_NARROW_TOTAL_TIME;
		int time = total - boxNarrowTimer;
		boxHeight = BackIn(time, total, maxHeight, 0.0f);
		blackAlpha = BackIn(time, total, MAX_BLACK_ALPHA, 0.0f);
		bandHeight = CircIn(time, total, maxBandHeight, 0.0f);
		charaMoveX = QuintInOut(time, total, CHARA_MIN_SPEED, CHARA_DISAPEEAR_SPEED);
		loginY = BackIn(time, total, loginAppearY, -LOGIN_HEIGHT);
		lineMaskWidth = QuintInOut(time, total, lineMaskAppearWidth, 0.0f);

		//�I��
		if (boxNarrowTimer == 0)
		{
			flag = NONE;
			shortCut = true;	//��x�����ʏ�ver�Đ�
		}
	}


	if (flag != NONE)
	{
		//�l�p�̍������Z
		if (flag != NARROW)
		{
			boxHeight += expandSpeed;
			bandHeight += bandSpeed;

			loginY += loginMoveY;

			//�Ԃ����C�������Ă���
			lineMaskWidth += lineMaskWidthSpeed;
		}

		//�L�����ړ�
		charaX += charaMoveX;
	}
}

void CharacterLogin::Draw()
{
	//�f�o�b�O�p
	//DrawGraph(0, 0, backHandle, true);

	//���w�i
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, blackAlpha);
	DrawBox(0, 0, WIN_WIDTH, WIN_HEIGHT, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//�J�b�g�C�����̍��w�i
	DrawBox(0, HALF_WIN_HEIGHT - boxHeight / 2, WIN_WIDTH, HALF_WIN_HEIGHT + boxHeight / 2, BLUE, true);

//�J�b�g�C��
	//�J�b�g�C���l�p�̏㉺Y
	float upY = HALF_WIN_HEIGHT - boxHeight / 2.0f;
	float lowY = HALF_WIN_HEIGHT + boxHeight / 2.0f;

	//�Ԃ����C��
	DrawCutInGraph(0,359, 0, RED_LINE_WIDTH, RED_LINE_HEIGHT, redLineHandle);
	//�Ԃ����C����p�}�X�N
	DrawBox(0, upY, lineMaskWidth, lowY, BLUE, true);

	//�L�����N�^�[
	DrawCutInGraph(charaX, 0, CHARA_WIDTH, WIN_HEIGHT, characterHandle);

	//���O�C������
	DrawCutInGraph(76, loginY, 0, LOGIN_WIDTH, LOGIN_HEIGHT, loginHandle);

	//�㉺�O���[����
	DrawBox(0, upY, WIN_WIDTH, upY + bandHeight, EME_GREEN, true);
	DrawBox(0, lowY - bandHeight, WIN_WIDTH, lowY, EME_GREEN, true);
}

void CharacterLogin::Start()
{
	//���X�p�����[�^�̏����l
	flag = EXPAND;

	//�J�b�g�C���l�p
	boxHeight = 0;
	boxExpandTimer = shortCut ? BOX_EXPAND_TOTAL_TIME_SHORT : BOX_EXPAND_TOTAL_TIME;
	expandSpeed = shortCut ? MAX_EXPAND_SPEED_SHORT : MAX_EXPAND_SPEED;

	//�J�b�g�C���㉺�̑�
	bandHeight = 0.0f;
	bandSpeed = MAX_GREEN_BAND_SPEED;

	//���w�i�̃A���t�@
	blackAlpha = 0.0f;

	//�J�b�g�C�����摜�����l
	charaX = WIN_WIDTH;
	loginY = WIN_HEIGHT;

	//�Ԃ����C���B���l�p�̉���
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
