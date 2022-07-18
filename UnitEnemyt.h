#pragma once
#include "Vec2.h"

//����G�̃X�e�[�^�X
enum ENEMY_UNIT_STATES{
	ENEMY_UNIT_STATES_ROUND,			//���S����ɂ��邮�������G
	ENEMY_UNIT_STATES_WALL,				//�ǂ�g��ňړ��������G
	ENEMY_UNIT_STATES_CLUSTER			//��莞�ԂŊg�U�������G
};

enum UNIT_ENEMY_DIRECTION{
	UNIT_ENEMY_DIRECTION_RIGHT,			//�ォ��
	UNIT_ENEMY_DIRECTION_DOWN,			//�E����
	UNIT_ENEMY_DIRECTION_LEFT,			//������
	UNIT_ENEMY_DIRECTION_UP				//������
};

class UnitEnemy{
public:
	/*��{���*/
	Vec2 pos;							//���W
	float angle;						//�ړ�����p�x
	float speed;						//�ړ����x
	Vec2 moved;							//�ړ�������
	int hp;								//HP
	int size;							//�傫��
	bool isAlive;						//�������Ă��邩�̃t���O
	int states;							//�G�̃X�e�[�^�X

	/*���G�̏��*/
	float addAngle;						//�p�x�ɉ��Z��������

	/*�ǂ̓G�̏��*/

public:
	//�R���X�g���N�^
	UnitEnemy();

	//����������
	void Init();
	//��������
	void Generate(Vec2 pos, float angle, int states);
	//�X�V����
	void Update();
	//�`�揈��
	void Draw(Vec2 scrollAmount, int enemyCircleOuter, int enemyCircleInner, int enemyDirection);
};

class UnitEnemyManager{
public:
	UnitEnemy unitEnemy[UNIT_ENEMY_COUNT];	//����G�̍ő吔
	bool isAlive;							//����G����������Ă��邩
	int direction;							//�ǂ������Ă��Ă������
	int warningAlpha;						//�댯�}�[�N�̃A���t�@�l
	bool warningAlphaChange;				//�댯�}�[�N�̃A���t�@�l���傫���Ȃ邩�������Ȃ邩
	float warningAlphaTimer;				//�댯�}�[�N�̃C�[�W���O�Ɏg���^�C�}�[
	int states;								//�G�̃X�e�[�^�X
	bool isEscape;							//�}�b�v�O�֏o�����ǂ����̃t���O
	
	/*--�~��̓G�p�̕ϐ�--*/
	int roundEnemyGenerateCount;			//�~��̓G������������
	int roundEnemyGenerateTimer;			//�~��̓G����������܂ł̒x���p

public:
	//�R���X�g���N�^
	UnitEnemyManager();

	//����������
	void Init();
	//��������
	void Generate(int states, int direction = 0);
	//�X�V����
	void Update();
	//�`�揈��
	void Draw(Vec2 scrollAmount, int enemyCircleOuter, int enemyCircleInner, int enemyDirection, int warningGraph, Vec2 playerPos);
};