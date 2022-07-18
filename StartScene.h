#pragma once
#include"BaseScene.h"
#include"SceneTransition.h"
class StartScene : public BaseScene
{
private:
	float alpha;
	int graph;

	int fadeInTimer;
	int waitTimer;
	int fadeOutTimer;

	bool skip;

	SceneTransition sceneTrans;

public:
	//�R���X�g���N�^
	StartScene(SceneChanger* changer);

	//������
	void Initialize()override;
	//�I������
	void Finalize()override;
	//�X�V
	void Update()override;
	//�`��
	void Draw()override;
};

