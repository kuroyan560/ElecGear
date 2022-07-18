#pragma once
#include "Vec2.h"

class Item{
public:
	Vec2 pos;						//���W
	float radius;					//���a
	float alpha;					//�v���C���[�̎�������邮���]����Ƃ��ɂ��񂾂񔖂����邽�߂Ɏg�p����
	float angle;					//�v���C���[�̎������]����Ƃ��̊p�x
	float playerDistance;			//�v���C���[�̎������]����Ƃ��Ɏg���v���C���[�Ƃ̋���
	int states;						//�A�C�e�������݂ǂ������󋵉���\���X�e�[�^�X
	int id;							//�ǂ̃A�C�e������ID
	bool isAlive;					//�������Ă��邩
	bool isCollision;				//�����蔻����s�����ǂ���
	bool isExpantion;				//���a���g�傷�郂�[�h���ǂ���

public:
	//�R���X�g���N�^
	Item();
	//����������
	void Init();
	//��������
	void Generate(int itemID, Vec2 generatePos);
	//�X�V����
	void Update(Vec2 playerPos);
	//�`�揈��
	void Draw(int itemGraph, Vec2 scrollAmount);
	//�v���C���[�ƏՓ˂����Ƃ��̏���
	void Hit(Vec2 playerPos);
	//�ق��̃A�C�e�����v���C���[�ƏՓ˂����Ƃ��̏���
	void Delete();
};
