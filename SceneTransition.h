#pragma once

//�V�[���J�ڊ��N���X
class SceneTransition
{
public:
	//���z�֐��f�X�g���N�^
	virtual ~SceneTransition() {};
	//�X�V
	virtual void Update() {};
	//�`��
	virtual void Draw() {};
	//�J�ڊJ�n
	virtual void Start() {};
	//�V�[���`�F���W������u�Ԃ̃g���K�[����
	virtual bool ChangeTrigger() { return true; }
	//�J�ڂ��I���������ǂ���
	virtual bool End() { return true; }
};