#include "EnemyManager.h"
#include"Enemy.h"
#include"CircleEnemy.h"
#include"SoundEmitter.h"

EnemyManager::EnemyManager()
{
	//�摜�ǂݍ���
	enemyCircleOuter = LoadGraph("resource/enemy_circle_outer.png");
	enemyCircleInner = LoadGraph("resource/enemy_circle_inner.png");
	enemyDirection = LoadGraph("resource/enemy_direction.png");
	warningGraph = LoadGraph("resource/warning.png");

	enemyDamagedSE = SoundEmitter::Instance()->LoadSound("resource/enemyDamagedSE.mp3", 150);
	enemyDeadSE = SoundEmitter::Instance()->LoadSound("resource/enemyDeadSE.mp3", 150);

	//�e�^�C�}�[��������
	//generateNormalTimer = 0;
	//generateCircleTimer = 0;
	unitTimer = 0;

	//�e�X�e�[�^�X��������
	states = ENEMY_STATES_NORMAL;
	unitStates = 0;
	unitWallDirection = 0;

	//�G�̎�ޕ��̔z�񐶐�
	enemys.resize(ENEMY_TYPE_NUM);
	//�G�̎�ނɉ����Đ�������N���X�Ƃ��̐����ς��
	//�ʏ�̒ǔ��G
	for (int i = 0; i < ENEMY_MAX[NORMAL_ENEMY_TRACKING]; ++i)
	{
		enemys[NORMAL_ENEMY_TRACKING].emplace_back(new TrackingEnemy);
	}
	//�ʏ�̒��i�G
	for (int i = 0; i < ENEMY_MAX[NORMAL_ENEMY_STRAIGHT]; ++i)
	{
		enemys[NORMAL_ENEMY_STRAIGHT].emplace_back(new StraightEnemy);
	}
	//�ʏ�̎w�肳�ꂽ�Ƃ���܂ňړ��G
	for (int i = 0; i < ENEMY_MAX[NORMAL_ENEMY_DESTINATION]; ++i)
	{
		enemys[NORMAL_ENEMY_DESTINATION].emplace_back(new DestinationEnemy);
	}
	//�����̌`�Ɉړ�����G
	for (int i = 0; i < ENEMY_MAX[NORMAL_ENEMT_INFINITY]; ++i)
	{
		enemys[NORMAL_ENEMT_INFINITY].emplace_back(new InfinityEnemy);
	}
	//���������ꏊ�őҋ@�G
	for (int i = 0; i < ENEMY_MAX[NORMAL_ENEMY_STANDBY]; ++i)
	{
		enemys[NORMAL_ENEMY_STANDBY].emplace_back(new StandByEnemy);
	}
	//�n�C�X�R�A�G
	for (int i = 0; i < ENEMY_MAX[NORMAL_ENEMY_HIGHSCORE]; ++i)
	{
		enemys[NORMAL_ENEMY_HIGHSCORE].emplace_back(new HighScoreEnemy);
	}
	//�ǔ��֓G
	for (int i = 0; i < ENEMY_MAX[CIRCLE_ENEMY_TRACKING]; ++i)
	{
		enemys[CIRCLE_ENEMY_TRACKING].emplace_back(new TrackingCircleEnemys);
	}
}

EnemyManager::~EnemyManager()
{
	for (int i = 0; i < enemys.size(); ++i) {
		for (int j = 0; j < enemys[i].size(); ++j) {
			delete enemys[i][j];
		}
	}
}

void EnemyManager::Update()
{
	enemyDamageSEPlay = false;
	enemyDeadSEPlay = false;

	//���݂̃X�e�[�^�X�ɉ����ď�����ς���
	switch (states) {
	case ENEMY_STATES_NORMAL:

		//�ʏ��Ԃł̍X�V����
		NormalStatesEnemyUpdate();
		//�ʏ펞�Ƀ^�C�}�[���X�V
		//unitTimer++;

		break;

	case ENEMY_STATES_UNIT:

		//UNIT�X�e�[�^�X�ł̍X�V�������s��
		UnitStatesUpdate();

		break;
	}

	//���[�J���ϐ� �ǂ𐶐�����ʒu
	int direction = 0;
	int directionBuff = 0;

	//�^�C�}�[�����l�ɒB������J��
	if (unitTimer >= ENEMY_UNIT_TIMER || Input::isKeyTrigger(KEY_INPUT_0)) {
		unitTimer = 0;
		//���ݐ������Ă���G��E�o��Ԃɂ���
		EscapeNormalEnemy();
		//�X�e�[�^�X���X�V
		states = ENEMY_STATES_UNIT;
		//�����_���Ō��߂�
		if (GetRand(2) == 0) {
			unitStates = ENEMY_UNIT_STATES_ROUND;
		}
		else {
			unitStates = ENEMY_UNIT_STATES_WALL;
		}

		//unitStates�ɉ����Đ����������s��
		switch (unitStates) {
		case ENEMY_UNIT_STATES_WALL:

			//��������
			for (int count = 0; count < 2; ++count) {
				for (int i = 0; i < UNIT_ENEMYS_COUNT; ++i) {
					if (unitEnemys[i].isAlive == true) continue;

					//���񃋁[�v���͂��̂܂ܕǂ��o���ʒu���v�Z����
					if (count == 0) {
						direction = GetRand(3);
					}
					else {
						//���łɐ������Ă�����̈ӊO�ɂȂ�܂Ń��[�v
						do {
							directionBuff = GetRand(3);
						} while (direction == directionBuff);

						direction = directionBuff;
					}

					unitEnemys[i].Generate(ENEMY_UNIT_STATES_WALL, direction);
					break;
				}
			}

			break;
		case ENEMY_UNIT_STATES_ROUND:

			//��������
			for (int i = 0; i < UNIT_ENEMYS_COUNT; ++i) {
				if (unitEnemys[i].isAlive == true) continue;

				unitEnemys[i].Generate(ENEMY_UNIT_STATES_ROUND);
				break;
			}

			break;
		}
	}
}

void EnemyManager::Draw(Vec2 scrollAmount)
{
	/*for (int i = 0; i < ENEMY_COUNT; ++i) {
		if (normalEnemys[i].isAlive == false) continue;
		normalEnemys[i].Draw(scrollAmount, enemyCircleOuter, enemyCircleInner, enemyDirection);
	}
	for (int i = 0; i < CIRCLE_ENEMYS_COUNT; ++i) {
		if (circleEnemys[i].isAlive == false) continue;
		circleEnemys[i].Draw(scrollAmount, enemyCircleOuter, enemyCircleInner, enemyDirection);
	}*/
	for (int i = 0; i < enemys.size(); ++i) {
		for (int j = 0; j < enemys[i].size(); ++j) {
			if (enemys[i][j]->isAlive == false)continue;
			enemys[i][j]->Draw(scrollAmount, enemyCircleOuter, enemyCircleInner, enemyDirection, warningGraph, *playerPos);
		}
	}

	//����G��`��
	for (int i = 0; i < UNIT_ENEMYS_COUNT; ++i) {
		if (unitEnemys[i].isAlive == false) continue;
		unitEnemys[i].Draw(scrollAmount, enemyCircleOuter, enemyCircleInner, enemyDirection, warningGraph, *playerPos);
	}
}

void EnemyManager::NormalStatesEnemyUpdate()
{
	/*-----�G�𐶐��̍X�V����-----*/
	//generateNormalTimer++;
	if (generateNormalTimer >= ENEMY_GENERATE_TIMER) {
		generateNormalTimer = 0;

		int angle = GetRand(359);
		Vec2 pos;
		pos.x = ENEMY_GENERATE_RADIUS * cos(angle * PI);
		pos.y = ENEMY_GENERATE_RADIUS * sin(angle * PI);

		//�G�𐶐�����
		GenerateEnemy(NORMAL_ENEMY_TRACKING, pos + MAP_CENTER);
	}

	//generateCircleTimer++;
	if (generateCircleTimer >= CIRCLE_ENEMY_GENERATE_TIMER) {
		generateCircleTimer = 0;
		//��������
		GenerateEnemy(CIRCLE_ENEMY_TRACKING, Vec2(GetRand(WIN_WIDTH), GetRand(WIN_HEIGHT)));
	}

	/*-----�G�̍X�V����-----*/
	for (int i = 0; i < enemys.size(); ++i) {
		for (int j = 0; j < enemys[i].size(); ++j) {
			if (enemys[i][j]->isAlive == false)continue;
			enemys[i][j]->Update(*playerPos);
		}
	}

	//unitEnemys���X�V����
	for (int i = 0; i < UNIT_ENEMYS_COUNT; ++i) {
		if (unitEnemys[i].isAlive == false) continue;

		unitEnemys[i].Update();
	}

	//����ENTER�������ꂽ��G���S�ď����������悤�ɂ���
	if (Input::isKeyTrigger(KEY_INPUT_RETURN)) {
		Init();
	}
}

void EnemyManager::Init()
{
	//�e�^�C�}�[��������
	generateNormalTimer = 0;
	generateCircleTimer = 0;

	//�e�X�e�[�^�X��������
	states = ENEMY_STATES_NORMAL;
	unitStates = 0;

	//�G�����ׂď�����
	/*for (int i = 0; i < ENEMY_COUNT; ++i) {
		if (normalEnemys[i].isAlive == false) continue;
		normalEnemys[i].Init();
	}
	for (int i = 0; i < CIRCLE_ENEMYS_COUNT; ++i) {
		if (circleEnemys[i].isAlive == false) continue;
		circleEnemys[i].Init();
	}*/
	for (int i = 0; i < enemys.size(); ++i) {
		for (int j = 0; j < enemys[i].size(); ++j) {
			enemys[i][j]->Init();
		}
	}

	for (int i = 0; i < UNIT_ENEMYS_COUNT; ++i) {
		unitEnemys[i].Init();
	}
}

void EnemyManager::EscapeNormalEnemy()
{
	/*for (int i = 0; i < enemys.size(); ++i) {
		for (int j = 0; j < enemys[i].size(); ++j) {
			if (enemys[i][j]->isAlive == false)continue;
			enemys[i][j]->Escape();
		}
	}*/
	//for (int i = 0; i < ENEMY_COUNT; ++i) {
	//	if (normalEnemys[i].isAlive == false) continue;
	//	normalEnemys[i].isEscape = true;
	//	normalEnemys[i].speed += ENEMY_ECPACE_SPEED;
	//}
	//for (int i = 0; i < CIRCLE_ENEMYS_COUNT; ++i) {
	//	if (circleEnemys[i].isAlive == false) continue;
	//	circleEnemys[i].head.isEscape = true;
	//	circleEnemys[i].originalSpeed += ENEMY_ECPACE_SPEED;
	//	circleEnemys[i].speed += ENEMY_ECPACE_SPEED;
	//}
}

void EnemyManager::UnitStatesUpdate()
{
	//UNIT�̃X�e�[�^�X�ɉ����ď����𕪂���
	switch (unitStates) {
	case ENEMY_UNIT_STATES_WALL:

		break;

	case ENEMY_UNIT_STATES_ROUND:

		break;
	}

	//UNIT�X�e�[�^�X�ł̒ʏ�̓G�̍X�V�������s��(�ړ��̂�)
	UnitStatesEnemyUpdate();

	//unitEnemys���X�V����
	for (int i = 0; i < UNIT_ENEMYS_COUNT; ++i) {
		if (unitEnemys[i].isAlive == false) continue;

		unitEnemys[i].Update();
	}

	//���ׂĂ�unitEnemys�̃t���O��false��������ʏ�̓G���[�h�ֈڍs
	int alivecount = 0;
	for (int i = 0; i < UNIT_ENEMYS_COUNT; ++i) {
		if (unitEnemys[i].isAlive == false) continue;

		alivecount++;
	}
	//���ׂĂ̓G�O���[�v������ł�����
	if (alivecount <= 0) {
		states = ENEMY_STATES_NORMAL;
	}

	//���ׂĂ̓G��escape���Ă�����ʏ�I���[�h�ֈڍs
	int escapeCount = 0;
	for (int i = 0; i < UNIT_ENEMYS_COUNT; ++i) {
		if (unitEnemys[i].isEscape == false) continue;

		escapeCount++;
	}
	if (escapeCount <= 0) {
		states = ENEMY_STATES_NORMAL;
	}
}

void EnemyManager::UnitStatesEnemyUpdate()
{
	/*-----�G�̍X�V����-----*/
	/*for (int i = 0; i < ENEMY_COUNT; ++i) {
		if (normalEnemys[i].isAlive == false) continue;
		normalEnemys[i].Update(playerPos);
	}

	for (int i = 0; i < CIRCLE_ENEMYS_COUNT; ++i) {
		if (circleEnemys[i].isAlive == false) continue;
		circleEnemys[i].Update(playerPos);
	}*/
	for (int i = 0; i < enemys.size(); ++i) {
		for (int j = 0; j < enemys[i].size(); ++j) {
			if (enemys[i][j]->isAlive == false)continue;
			enemys[i][j]->Update(*playerPos);
		}
	}

	//����ENTER�������ꂽ��G���S�ď����������悤�ɂ���
	if (Input::isKeyTrigger(KEY_INPUT_RETURN)) {
		Init();
	}
}

void EnemyManager::GenerateEnemy(ENEMY_TYPE type, Vec2 pos, int* hp, float* angle, Vec2* targetPos)
{
	for (int i = 0; i < enemys[type].size(); ++i) {
		if (enemys[type][i]->isAlive == true)continue;
		enemys[type][i]->Generate(pos, *playerPos, hp, angle, targetPos);
		break;
	}
}

void EnemyManager::PlayDamageSE()
{
	//�d�����čĐ����Ȃ��悤�ɂ���
	if (!enemyDamageSEPlay) {
		SoundEmitter::Instance()->Play(enemyDamagedSE, DX_PLAYTYPE_BACK);
		enemyDamageSEPlay = true;
	}
}

void EnemyManager::PlayDeadSE()
{
	//�d�����čĐ����Ȃ��悤�ɂ���
	if (!enemyDeadSEPlay) {
		SoundEmitter::Instance()->Play(enemyDeadSE, DX_PLAYTYPE_BACK);
		enemyDeadSEPlay = true;
	}
}

void EnemyManager::ResetSlashedFlag()
{
	for (int i = 0; i < enemys.size(); ++i) {
		for (int j = 0; j < enemys[i].size(); ++j) {
			if (enemys[i][j]->isAlive == false)continue;
			enemys[i][j]->slashed = false;
		}
	}
}

bool EnemyManager::isAllEnemyDead()
{
	int aliveCount = 0;

	for (int i = 0; i < ENEMY_TYPE_NUM; ++i) {
		for (int j = 0; j < ENEMY_MAX[i]; ++j)
		{
			if (enemys[i][j]->isAlive) aliveCount++;
		}
	}

	if (aliveCount == 0) {
		return true;
	}
	return false;
}
