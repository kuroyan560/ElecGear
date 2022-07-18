#pragma once
#include "Singleton.h"
#include "UnitEnemyt.h"
#include"Constants.h"
#include<vector>

class EnemyInterFace;

//�G�S�̂̓������Ǘ�����X�e�[�^�X
enum ENEMY_STATES{
	ENEMY_STATES_NORMAL,			//�ʏ펞�̓G�̏���
	ENEMY_STATES_UNIT				//�����g�񂾓G���o��������Ƃ��̏���
};

class EnemyManager : public Singleton<EnemyManager> {
private:
	friend Singleton<EnemyManager>;

	int enemyCircleOuter;							//�G�̊O���̉摜
	int enemyCircleInner;							//�G�̓����̉摜
	int enemyDirection;								//�G�̐i�s�����̉摜
	int warningGraph;								//���Ӄ}�[�N

	int enemyDamagedSE = -1;
	bool enemyDamageSEPlay = false;
	int enemyDeadSE = -1;
	bool enemyDeadSEPlay = false;

public:
	Vec2* playerPos = nullptr;

	/*--�X�e�[�^�X�Ȃǂ̊Ǘ��ϐ�--*/
	int states;										//�G�S�̂̃��[�h�Ǘ��p�ϐ�
	int unitTimer;									//unit�G��ԂɑJ�ڂ���܂ł̃^�C�}�[

	/*--�ʏ펞��������G�̕ϐ�--*/
	//TrackingEnemy normalEnemys[ENEMY_COUNT];				//�G�z��
	int generateNormalTimer;						//�G�𐶐�����^�C�}�[
	//CircleEnemys circleEnemys[CIRCLE_ENEMYS_COUNT];	//����G�̔z��
	int generateCircleTimer;						//����G�𐶐�����^�C�}�[
	std::vector<std::vector<EnemyInterFace*>>enemys;

	/*--����G�ȂǓ���ȓG�̕ϐ�--*/
	UnitEnemyManager unitEnemys[UNIT_ENEMYS_COUNT];	//����G
	int unitStates;									//����G�̃X�e�[�^�X
	int unitWallDirection;							//�ǂ̓G�̘g���p

private:
	//�R���X�g���N�^
	EnemyManager();

public:
	~EnemyManager();

	//����������
	void Init();
	//�X�V����
	void Update();
	//�`�揈��
	void Draw(Vec2 scrollAmount);

	//�ʏ��Ԃł̓G�̍X�V����
	void NormalStatesEnemyUpdate();


	//UNIT��Ԃł̓G�̍X�V����
	void UnitStatesEnemyUpdate();
	//UNIT��Ԃł̑S�̂̍X�V����
	void UnitStatesUpdate();

	//���ݐ������Ă���G��E�o���[�h�ɂ���
	void EscapeNormalEnemy();

	//�G�̓o��
	void GenerateEnemy(ENEMY_TYPE type, Vec2 pos, int* hp = nullptr, float* angle = nullptr, Vec2* targetPos = nullptr);

	//�_���[�WSE
	void PlayDamageSE();
	//���SSE
	void PlayDeadSE();

	void ResetSlashedFlag();

	//���ׂĂ̓G������ł��邩���m�F ����ł�����true
	bool isAllEnemyDead();
};