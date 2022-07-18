#pragma once
#include "Vec2.h"
#include"PlayerOrbit.h"

class Player {
public:
	int hp;

	/*�v���C���[��{���*/
	Vec2 pos;							//�v���C���[�̍��W
	Vec2 vel;							//�v���C���[�̈ړ�����p�x
	Vec2 move;
	Vec2 moveBuff;
	float size;							//�v���C���[�̃T�C�Y
	float speed;						//�v���C���[�̈ړ����x
	float playerAngle;					//�e���ˏo�������
	int playerHeadGraph;				//�v���C���[���̉摜
	int playerArmGraph;					//�v���C���[�̘r�̉摜
	int itemGetGraph;					//�A�C�e���Q�b�g�G�t�F�N�g�摜
	int hitTimer;						//�����������ɑ���s�\&���G���ԂɂȂ�^�C�}�[
	int bulletStates;					//�e�̃X�e�[�^�X

	int damagedSE;

	////float gauge;						//�v���C���[�̃Q�[�W
	//float hpGauge;

	int slashSE[3];	//�a��SE
	int maxSlashSE;	//�a���̈ړ��ʍő�
	int slashRangeGraph;	//�a�����~
	int slashGaugeGraph;	//�a���Q�[�W
	int slashGaugeDrawTimer[3];	//�a���Q�[�W�̃��[�g
	int nonSlashTimer;
	int slashGaugeHealSE[3];

	/*--�A�C�e���Q�b�g�G�t�F�N�g--*/
	static const int ITEM_GET_TOTAL_TIME = 60;
	int itemGetTimer;

	/*--�����ɕK�v�ȕϐ�--*/
	float reactionTime;					//�������N���Ă���(�߂鎞�Ԃ��܂߂�)���v����
	float reactionTimer;				//�������N���Ă���(�߂鎞�Ԃ��܂߂�)����
	float armReactionPos;				//�������N�������ɓ������ʂ����Z����l
	bool isReaction;					//�������N���Ă��邩�ǂ���
	/*--�a���ɕK�v�ȕϐ�--*/
	bool isSlash;						//�a�������Ă��邩�ǂ����̃t���O
	bool canSlash;						//�a�����ł��邩�ǂ����̃t���O
	int slashCooltime;					//�a�����ł���悤�ɂȂ�܂ł̃N�[���^�C��
	//int slashDeathCount;				//�a�����ɎE������
	int slashFrame;						//�a���̃t���[����
	float slashRadius;					//�a���̔��a
	int slashInvincibleTime;			//�a�����̖��G����
	bool isSlashAnime;					//�a���A�j���[�V������`�悷�邩�ǂ���
	int slashAnimeNumber;				//�a���̃A�j���[�V�����̃A�j���[�V�����i���o�[
	int slashAnimeTimer;				//�a���̃A�j���[�V�����i���o�[��ς���^�C�}�[
	Vec2 slashMoveBuff;					//�a�����̈ړ��ʂ�ۑ�
	//�i�V�K�ǉ��j
	int slashGauge;
	int slashGaugeTimer;
	int slashButtonFrame;
	float buttonInputRate;

	Vec2 scrollAmount;					//�ړ�������

	/*--�O��--*/
	PlayerOrbit orbit;

	/*--�a��--*/
	int slashStartGraph;
	int slashGraph[5];					//�a���̉摜
	int slashEndGraph;

	bool slashFinishTrigger = false;

public:
	//�R���X�g���N�^
	Player();

	//����������
	void Init();
	//�X�V����
	void Update(bool isShoot, int shootDelay);
	//�`�揈��
	void Draw();

	//���������Ƃ��̏���
	void Hit();

	void EmitItemGetEffect();
};