#pragma once
#include "Vec2.h"
#include"EnemyInterFace.h"

//����G�̐擪
class CircleEnemyHead{
public:
	Vec2 pos;							//�G�̍��W
	float size;							//�G�̃T�C�Y
	float angle;						//���S�_����̊p�x ������X�V���ĉ�]���Ă���
	float radius;						//���S�_����̋��� ����ŉ~�̒��̂ǂ̕ӂ����񂷂邩�����߂�
	bool isAlive;						//�G���������Ă��邩�̃t���O
	int hp;								//HP

	bool isEscape;						//�E�o���[�h�t���O ���ꂪ�����Ă���Ƃ��͒��S����t�����ɓ�����

	//����������
	void Init();
	//��������
	void Generate(Vec2 generatePos);
	//�X�V����
	void Update(int states, Vec2 playerPos, float speed);
};
//�㑱�̎���G
class CircleEnemyFollowing{
public:
	Vec2 pos;							//�G�̍��W
	float size;							//�G�̃T�C�Y
	bool isAlive;						//�G���������Ă��邩�̃t���O
	int hp;								//HP
	
	//����������
	void Init();
	//��������
	void Generate(Vec2 prevPos);
	//�X�V����
	void Update(Vec2 prevPos, float speed);
};

enum CIRCLE_ENEMY_STATES{
	CIRCLE_ENEMY_STATES_ROCKON,			//�v���C���[��ǔ�������
	CIRCLE_ENEMY_STATES_ROUND			//���낤�냂�[�h
};

//����G�Ǘ��N���X
class CircleEnemys : public EnemyInterFace{
public:
	CircleEnemyHead head;				//����G�O���[�v�̐擪
	CircleEnemyFollowing following[CIRCLE_ENEMY_FOLLOWING_COUNT];	//�㑱

	float speed;						//�ړ����x
	float originalSpeed;				//�{������ׂ��ړ����x
	int aliveCount;						//������
	int states;							//����G�̃X�e�[�^�X
	int statesTimer;					//�X�e�[�^�X���ς��܂ł̃^�C�}�[

	//�R���X�g���N�^
	CircleEnemys(ENEMY_TYPE type);

	//����������
	void Init()override;
	//��������
	virtual void Generate(Vec2 generatePos, Vec2 playerPos,int* hp = nullptr, float* angle = nullptr, Vec2* targetPos = nullptr)override = 0;
	//�X�V����
	virtual void Update(Vec2 playerPos)override = 0;
	//�`�揈��
	void Draw(Vec2 scrollAmount, int enemyCircleOuter, int enemyCircleInner, int enemyDirection, int warningGraph, Vec2 playerPos)override;
	//�����蔻��
	bool HitCheck(Vec2 pos, float size, bool player, bool* addScore)override;
	bool HitCheckSlash(Vec2 pos, float angle, int& deathCount, int& hitCount)override;
	//����
	void Escape()override;

	//�{������ׂ����x���X�V
	void HitAccel();
	//hit�������̏���
	void Hit(bool player);
};

/*--�v���C���[�ǔ�--*/
class TrackingCircleEnemys : public CircleEnemys 
{
public:
	TrackingCircleEnemys() : CircleEnemys(CIRCLE_ENEMY_TRACKING) {};
	void Generate(Vec2 generatePos, Vec2 playerPos, int* hp = nullptr, float* angle = nullptr, Vec2* targetPos = nullptr)override;
	void Update(Vec2 playerPos)override;
};