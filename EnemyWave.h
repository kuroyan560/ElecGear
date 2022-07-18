#pragma once
#include"Constants.h"
#include<vector>
#include"Vec2.h"
class EnemyWave
{
protected:
	struct Info
	{
		ENEMY_TYPE type;		//�o�ꂷ��G�̎��
		int appearFrame;		//�E�F�[�u�N�����Ă��牽�t���[���ڂœo�ꂷ�邩
		bool appear = false;	//�o�ꂳ������
		Vec2 pos;				//�o��ʒu
		Vec2 targetPos = {};	//�ړ����������ʒu
		float angle = 0;		//�ړ�������p�x
		int hp = 0;				//hp
	};

	std::vector<Info>infos;
	int frame;	//�N�����Ă���̃t���[����
	int waveLife = 0;	//�E�F�[�u�̎����i�Ō�̓G��o�ꂳ�����玀�ʁj

public:
	void Init() { isAlive = false; }
	bool isAlive = false;	//�N������
	void Generate();
	void Update();
	void AddInfo(ENEMY_TYPE type, int appearFrame, Vec2 pos, int hp = 0, float angle = 0, Vec2 targetPos = {});

	virtual bool OnFlag(int frame) = 0;
};

//�Q�[���X�^�[�g���Ă���̃t���[�����ŋN������E�F�[�u
class EnemyWave_Frame : public EnemyWave
{
	int startFrame;	//�Q�[���X�^�[�g��������< startFrame >�ڂŋN������
public:
	EnemyWave_Frame(int onFrame) :startFrame(onFrame) {};
	bool OnFlag(int frame)override { return startFrame == frame; }
};