#pragma once
#include"SceneTransition.h"
class GameFinish : public SceneTransition
{
	int finishGraph;
	float finishY;

	int appearTimer;
	int waitTimer;
	int disappearTimer;

	int newSceneAppearTimer;

	bool complete = false;
	bool changeSceneTrigger;

	int blackAlpha;

public:
	GameFinish();
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

