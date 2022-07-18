#pragma once
#include"SceneTransition.h"
class CharacterLogin : public SceneTransition
{
	//���Ԍv���p
	int boxExpandTimer = 0;
	int waitTimer = 0;
	int boxNarrowTimer = 0;

	//���݂̏�ԃt���O
	const enum { NONE,EXPAND, WAIT, NARROW };
	int flag = 0;

	//�J�b�g�l�p�̍���
	float boxHeight = 0.0f;
	//�g��X�s�[�h
	float expandSpeed = 0.0f;
	//�ő�̍����ۑ��p
	float maxHeight = 0.0f;

	//���w�i�̃A���t�@
	float blackAlpha = 0.0f;

	//�O���[���㉺�т̍���
	float bandHeight = 0.0f;
	//�����̏㏸�X�s�[�h
	float bandSpeed = 0.0f;
	//�ő�̍����ۑ��p
	float maxBandHeight = 0.0f;


//�摜
	//�L�����摜
	int characterHandle;
	//�L����X���W
	float charaX;
	//�L�����ړ��X�s�[�h
	float charaMoveX;

	//�Ԃ����C��
	int redLineHandle;
	float lineMaskWidthSpeed;
	//
	float lineMaskAppearWidth;
	//�Ԃ����C�����B���l�p�̉���
	int lineMaskWidth;

	//Login����
	int loginHandle;
	//Login����Y���W
	float loginY;
	//Login�����ړ��X�s�[�h
	float loginMoveY;
	//Login�����ŏI�ʒu�ۑ��p
	float loginAppearY;

	int bgmIntro;
	int bgmShortIntro;

	//�摜���J�b�g�C���^�ɂ��蔲���ĕ`��
	void DrawCutInGraph(int x, int rectX, int width, int height, int graphHandle);
	void DrawCutInGraph(int x, int y, int rectX, int width, int height, int graphHandle);

public:

	bool shortCut = false;
	CharacterLogin();
	~CharacterLogin();

	//�X�V
	void Update()override;
	//�`��
	virtual void Draw()override;
	//�J�ڊJ�n
	virtual void Start()override;
	//�V�[���`�F���W������u�Ԃ̃g���K�[����
	virtual bool ChangeTrigger()override;
	//�J�ڂ��I���������ǂ���
	virtual bool End()override;
};

