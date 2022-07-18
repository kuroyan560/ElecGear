#pragma once
#include"EnemyInterFace.h"
#include"Vec2.h"
class Enemy : public EnemyInterFace
{
protected:
	/*�G��{���*/
	Vec2 pos;							//�G�̍��W
	float speed;						//�G�̈ړ����x
	float size;							//�G�̃T�C�Y
	float angle;						//�G�̈ړ�����p�x
	Vec2 move;

	bool isEscape;						//�E�o���[�h�t���O ���ꂪ�����Ă���Ƃ��͒��S����t�����ɓ�����

	int hitStanTimer;
	
	int generateTimer;					//��������܂ł̎���
	bool isGenerate;					//�������Ă��邩�ǂ���

public:
	Enemy(ENEMY_TYPE type);
	//����������
	virtual void Init()override;
	//��������
	virtual void Generate(Vec2 pos, Vec2 playerPos, int* hp = nullptr, float* angle = nullptr, Vec2* targetPos = nullptr) = 0;
	//�X�V����
	virtual void Update(Vec2 playerPos)override = 0;
	//�`�揈��
	void Draw(Vec2 scrollAmount, int enemyCircleOuter, int enemyCircleInner, int enemyDirection, int warningGraph, Vec2 playerPos)override;
	//�����蔻��
	virtual bool HitCheck(Vec2 pos, float size, bool player, bool* addScore)override;
	virtual bool HitCheckSlash(Vec2 pos, float angle, int& deathCount, int& hitCount)override;
	//����
	void Escape()override;
};

/*--�v���C���[�ǔ�--*/
class TrackingEnemy : public Enemy {
public:
	TrackingEnemy() :Enemy(NORMAL_ENEMY_TRACKING) {};
	void Generate(Vec2 generatePos, Vec2 playerPos, int* hp = nullptr, float* angle = nullptr, Vec2* targetPos = nullptr)override;
	void Update(Vec2 playerPos)override;
};

/*--�܂����������G--*/
class StraightEnemy : public Enemy {
public:
	StraightEnemy() :Enemy(NORMAL_ENEMY_STRAIGHT) {};
	void Generate(Vec2 generatePos, Vec2 playerPos, int* hp = nullptr, float* angle = nullptr, Vec2* targetPos = nullptr)override;
	void Update(Vec2 playerPos)override;
};

/*--�w��̈ʒu�܂ňړ�����G--*/
class DestinationEnemy : public Enemy {
public:
	Vec2 targetPos;							//�ړI�n
	float distanceDef;						//�ړI�n�܂ł̏����̋���
	int escapeTimer;						//�E�o����܂ł̃^�C�}�[
	DestinationEnemy() : Enemy(NORMAL_ENEMY_DESTINATION) {};
	void Generate(Vec2 generatePos, Vec2 playerPos, int* hp = nullptr, float* angle = nullptr, Vec2* targetPos = nullptr)override;
	void Update(Vec2 playerPos)override;
};

/*--�����̌`�ňړ����|���ꂽ���ʂ̓G���o���G--*/
class InfinityEnemy : public Enemy {
public:
	float rotationAngle;					//��]�ɕK�v�ȃA���O��
	float rotationAngleAmount;				//��]������
	bool isRotationLeft;					//�ǂ���������]���Ă��邩
	InfinityEnemy() : Enemy(NORMAL_ENEMT_INFINITY) {};
	//���̃N���X�ł͐�������ۂɂ��炩���ߌ��܂����l������̂ŁA�����͓K���ł悢
	void Generate(Vec2 generatePos, Vec2 playerPos, int* hp = nullptr, float* angle = nullptr, Vec2* targetPos = nullptr)override;
	void Update(Vec2 playerPos)override;
};

/*--���������ʒu�Œ�~�����Ă����G--*/
class StandByEnemy :public Enemy {
public:
	int escapeCount = 0;					//�E�o����܂ł̃^�C�}�[

	StandByEnemy() :Enemy(NORMAL_ENEMY_STANDBY) {};
	void Generate(Vec2 generatePos, Vec2 playerPos, int* hp = nullptr, float* angle = nullptr, Vec2* targetPos = nullptr)override;
	void Update(Vec2 playerPos)override;
};

/*--�n�C�X�R�A�̓G--*/
class HighScoreEnemy : public Enemy {
public:
	int rotationCount;						//�E�o����܂ŉ�]���鐔
	float rotationRadius;					//��]���锼�a
	float rotationAngle;					//�p�x
	bool isRadiusMinus;						//���a�����������邩�ǂ����̃t���O

	HighScoreEnemy() :Enemy(NORMAL_ENEMY_HIGHSCORE) {};
	//���̃N���X�ł͐�������ۂɂ��炩���ߌ��܂����l������̂ŁA�����͓K���ł悢
	void Generate(Vec2 generatePos, Vec2 playerPos, int* hp = nullptr, float* angle = nullptr, Vec2* targetPos = nullptr)override;
	void Update(Vec2 playerPos)override;
};