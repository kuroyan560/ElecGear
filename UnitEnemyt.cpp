#include "UnitEnemyt.h"

UnitEnemy::UnitEnemy()
{
	/*��{���*/
	pos.Init();
	angle = 0;
	speed = 0;
	hp = 0;
	size = 0;
	isAlive = 0;
	states = 0;

	/*���G�̏��*/
	addAngle = 0;
}

void UnitEnemy::Init()
{
	isAlive = false;
}

void UnitEnemy::Generate(Vec2 pos, float angle, int states)
{
	/*��{���*/
	this->pos = pos;
	this->angle = angle;
	speed = UNIT_ENEMY_SPEED;
	hp = UNIT_ENEMY_HP;
	size = UNIT_ENEMY_SIZE;
	isAlive = true;
	moved.Init();

	/*�X�e�[�^�X�ɂ���ĕ�����*/
	this->states = states;
	switch (states) {
	case ENEMY_UNIT_STATES_WALL:

		break;

	case ENEMY_UNIT_STATES_ROUND:

		//����ɕK�v�Ȋe�ϐ���������
		addAngle = 0;


		break;
	}
}

void UnitEnemy::Update()
{
	//�X�e�[�^�X�ɉ����ď�����ς���
	switch (states) {
	case ENEMY_UNIT_STATES_WALL:

		//�ړ�������
		pos += Vec2(cos(angle) * speed, sin(angle) * speed);

		break;

	case ENEMY_UNIT_STATES_ROUND:

		//�p�x���X�V����
		if (addAngle < UNIT_ENEMY_ROUND_ADDANGLE) {
			addAngle += 0.01f;
			angle += 0.01f;
		}

		//�ړ�������
		pos += Vec2(cos(angle) * speed, sin(angle) * speed);

		break;
	}

	//�ړ������ʂ𑫂�
	moved += Vec2(fabs(cos(angle) * speed), fabs(sin(angle) * speed));

	//�}�b�v�T�C�Y�̒��n���ړ�������폜 20000�͒萔.h��g�ݍ��񂾂�萔�ɂ���
	if (moved.x >= MAP_RADIUS_3_TIMES || moved.y >= MAP_RADIUS_3_TIMES) {
		Init();
	}
}

void UnitEnemy::Draw(Vec2 scrollAmount, int enemyCircleOuter, int enemyCircleInner, int enemyDirection)
{
	//�G��`��
	DrawExtendGraph(pos.x - size - scrollAmount.x, pos.y - size - scrollAmount.y, pos.x + size - scrollAmount.x, pos.y + size - scrollAmount.y, enemyCircleOuter, TRUE);
	float scale = (size / (UNIT_ENEMY_HP / hp));
	DrawExtendGraph(pos.x - scale - scrollAmount.x, pos.y - scale - scrollAmount.y, pos.x + scale - scrollAmount.x, pos.y + scale - scrollAmount.y, enemyCircleInner, TRUE);
	//DrawExtendGraph(pos.x - (size / hp) - scrollAmount.x, pos.y - (size / hp) - scrollAmount.y, pos.x + (size / hp) - scrollAmount.x, pos.y + (size / hp) - scrollAmount.y, enemyCircleInner, TRUE);
	//�G�̌��� �G���W��������Ă�����ɑ傫�������������ʒu�ɕ`��
	DrawRotaGraph(pos.x + (cos(angle) * (size + size / 2.0f)) - scrollAmount.x, pos.y + (sin(angle) * (size + size / 2.0f)) - scrollAmount.y, CAMERA_RATE, angle - 0.35f, enemyDirection, TRUE);
}

/*--�}�l�[�W���[�̊֐�--*/
UnitEnemyManager::UnitEnemyManager()
{
	isAlive = false;
}

void UnitEnemyManager::Init()
{
	for (int i = 0; i < UNIT_ENEMY_COUNT; ++i) {
		unitEnemy[i].isAlive = false;
	}
	isAlive = false;
	direction = 0;
	warningAlpha = 0;
	warningAlphaChange = true;
}

void UnitEnemyManager::Generate(int states, int direction)
{
	//���[�J���ϐ���錾
	float angle = 0;
	Vec2 generatePos;

	//states�ɂ���Đ�������l��ς���
	switch (states) {
	case ENEMY_UNIT_STATES_WALL:

		//��������p�x�������_���Ō��߂�
		this->direction = direction;
		//��������p�x�ɉ����Đ�������ʒu���ς���
		generatePos.Init();
		switch ((int)direction) {
		case 0:
			//�p�x���v�Z
			angle = direction * (DX_PI_F / 2.0f);
			//���ォ�牺�Ɍ����Đ�������B�G�̐i�s�����͉E
			generatePos.Init((WIN_WIDTH / 2.0f) - MAP_RADIUS - (MAP_RADIUS / 2.0f), (WIN_HEIGHT / 2.0f) - MAP_RADIUS);
			//�ǂ̓G�𐶐�����
			for (int i = 0; i < MAP_RADIUS_2_TIMES / UNIT_ENEMY_SIZE; ++i) {
				for (int j = 0; j < UNIT_ENEMY_COUNT; ++j) {
					//�����ς݂�������X�L�b�v����
					if (unitEnemy[j].isAlive == true) continue;

					unitEnemy[j].Generate(Vec2(generatePos.x, generatePos.y + (UNIT_ENEMY_SIZE * 3 * i)), angle, ENEMY_UNIT_STATES_WALL);
					break;
				}
			}
			break;
		case 1:
			//�p�x���v�Z
			angle = direction * (DX_PI_F / 2.0f);
			//�E�ォ�獶�Ɍ����Đ�������B�G�̐i�s�����͉�
			generatePos.Init((WIN_WIDTH / 2.0f) + MAP_RADIUS, (WIN_HEIGHT / 2.0f) - MAP_RADIUS - (MAP_RADIUS / 2.0f));
			//�ǂ̓G�𐶐�����
			for (int i = 0; i < MAP_RADIUS_2_TIMES / UNIT_ENEMY_SIZE; ++i) {
				for (int j = 0; j < UNIT_ENEMY_COUNT; ++j) {
					//�����ς݂�������X�L�b�v����
					if (unitEnemy[j].isAlive == true) continue;

					unitEnemy[j].Generate(Vec2(generatePos.x - (UNIT_ENEMY_SIZE * 3 * i), generatePos.y), angle, ENEMY_UNIT_STATES_WALL);
					break;
				}
			}
			break;
		case 2:
			//�p�x���v�Z
			angle = direction * (DX_PI_F / 2.0f);
			//�E�������Ɍ����Đ�������B�G�̐i�s�����͍�
			generatePos.Init((WIN_WIDTH / 2.0f) + MAP_RADIUS + (MAP_RADIUS / 2.0f), (WIN_HEIGHT / 2.0f) + MAP_RADIUS);
			//�ǂ̓G�𐶐�����
			for (int i = 0; i < MAP_RADIUS_2_TIMES / UNIT_ENEMY_SIZE; ++i) {
				for (int j = 0; j < UNIT_ENEMY_COUNT; ++j) {
					//�����ς݂�������X�L�b�v����
					if (unitEnemy[j].isAlive == true) continue;

					unitEnemy[j].Generate(Vec2(generatePos.x, generatePos.y - (UNIT_ENEMY_SIZE * 3 * i)), angle, ENEMY_UNIT_STATES_WALL);
					break;
				}
			}
			break;
		case 3:
			//�p�x���v�Z
			angle = direction * (DX_PI_F / 2.0f);
			//�������獶�Ɍ����Đ�������B�G�̐i�s�����͏�
			generatePos.Init((WIN_WIDTH / 2.0f) - MAP_RADIUS, (WIN_HEIGHT / 2.0f) + MAP_RADIUS + (MAP_RADIUS / 2.0f));
			//�ǂ̓G�𐶐�����
			for (int i = 0; i < MAP_RADIUS_2_TIMES / UNIT_ENEMY_SIZE; ++i) {
				for (int j = 0; j < UNIT_ENEMY_COUNT; ++j) {
					//�����ς݂�������X�L�b�v����
					if (unitEnemy[j].isAlive == true) continue;

					unitEnemy[j].Generate(Vec2(generatePos.x + (UNIT_ENEMY_SIZE * 3 * i), generatePos.y), angle, ENEMY_UNIT_STATES_WALL);
					break;
				}
			}
			break;
		}

		break;

	case ENEMY_UNIT_STATES_ROUND:

		break;
	}

	isAlive = true;
	isEscape = true;

	warningAlphaChange = true;
	warningAlphaTimer = 0;

	roundEnemyGenerateTimer = 0;
	roundEnemyGenerateCount = 0;

	this->states = states;
}

void UnitEnemyManager::Update()
{
	/*-----states��Round�������ꍇ�A��莞�ԋN���ɓG�O���[�v�𐶐�-----*/
	if (states == ENEMY_UNIT_STATES_ROUND) {
		//�^�C�}�[�����Z
		roundEnemyGenerateTimer++;
		if (roundEnemyGenerateTimer >= UNIT_ENEMY_ROUND_GENERATE_TIMER) {
			roundEnemyGenerateTimer = 0;
			//����������������̐��ɒB���Ă��Ȃ�������
			if (roundEnemyGenerateCount < UNIT_ENEMY_ROUND_GENERATE_COUNT) {

				//�G�𐶐�
				float angle = 0.01f * roundEnemyGenerateCount;
				Vec2 generatePos = Vec2(WIN_WIDTH / 2.0f, WIN_HEIGHT / 2.0f);
				//�w�萔��������
				for (int i = 0; i < UNIT_ENEMY_ROUND_COUNT; ++i) {
					for (int j = 0; j < UNIT_ENEMY_COUNT; ++j) {
						//�����ς݂�������X�L�b�v����
						if (unitEnemy[j].isAlive == true) continue;

						unitEnemy[j].Generate(Vec2(generatePos.x, generatePos.y), angle, ENEMY_UNIT_STATES_WALL);
						break;
					}
					//��]�ʂ��X�V
					angle += DX_TWO_PI_F / UNIT_ENEMY_ROUND_COUNT;
				}

				//���������������Z
				roundEnemyGenerateCount++;
			}
		}
	}

	/*-----UnitEnemy�̍X�V����-----*/
	for (int i = 0; i < UNIT_ENEMY_COUNT; ++i) {
		if (unitEnemy[i].isAlive == false) continue;

		unitEnemy[i].Update();
	}

	/*-----���ׂĂ̓G������ł�����傫�ȃt���O��܂�-----*/
	if (!(states == ENEMY_UNIT_STATES_ROUND && roundEnemyGenerateCount < UNIT_ENEMY_ROUND_GENERATE_COUNT)) {
		int alivecount = 0;
		for (int i = 0; i < UNIT_ENEMY_COUNT; ++i) {
			if (unitEnemy[i].isAlive == false) continue;

			alivecount++;
		}
		if (alivecount <= 0) {
			isAlive = false;
		}
	}

	/*-----���ׂĂ̓G���}�b�v�O�ֈړ�������E�o�����t���O��܂�-----*/
	switch (states) {
	case ENEMY_UNIT_STATES_WALL:
		//�ǂ̓G�������ꍇ
		for (int i = 0; i < UNIT_ENEMY_COUNT; ++i) {
			if (unitEnemy[i].moved.x <= MAP_RADIUS + (MAP_RADIUS / 2.0f) && unitEnemy[i].moved.y <= MAP_RADIUS + (MAP_RADIUS / 2.0f)) continue;

			isEscape = false;
		}
		break;

	case ENEMY_UNIT_STATES_ROUND:
		//�~��ɍL����G�������ꍇ�A�ő吶�����܂Ő������ĂȂ��Ə������s��Ȃ�
		if (roundEnemyGenerateCount >= UNIT_ENEMY_ROUND_GENERATE_COUNT) {
			for (int i = 0; i < UNIT_ENEMY_COUNT; ++i) {
				if (unitEnemy[i].moved.x <= MAP_RADIUS && unitEnemy[i].moved.y <= MAP_RADIUS) continue;

				isEscape = false;
			}
		}
		break;
	}

	/*-----�댯�M���̃A���t�@�l��ς���-----*/
	//�댯�M���̃A���t�@�l��ς���
	warningAlphaTimer += 0.01f;
	if (warningAlphaChange) {
		//���̃t���O��true��������A���t�@�l��傫�����Ă���
		warningAlpha = 255 * (powf(1 - warningAlphaTimer, 3));
		if (warningAlpha <= 10) {
			warningAlphaTimer = 0;
			warningAlphaChange = false;
			warningAlpha = 0;
		}
	}
	else {
		//���̃t���O��false��������A���t�@�l�����������Ă���
		warningAlpha = -255 * (powf(warningAlphaTimer, 3)) + 255;
		if (warningAlpha >= 245) {
			warningAlphaTimer = 0;
			warningAlphaChange = true;
			warningAlpha = 255;
		}
	}
}

void UnitEnemyManager::Draw(Vec2 scrollAmount, int enemyCircleOuter, int enemyCircleInner, int enemyDirection, int warningGraph, Vec2 playerPos)
{
	//�G��`��
	for (int i = 0; i < UNIT_ENEMY_COUNT; ++i) {
		if (unitEnemy[i].isAlive == false) continue;

		unitEnemy[i].Draw(scrollAmount, enemyCircleOuter, enemyCircleInner, enemyDirection);
	}

	int x = 0;
	int y = 0;
	int graphSize = 64;
	Vec2 buff;
	Vec2 scrollBuff;
	Vec2 scrollWidth = Vec2(550, -250);
	Vec2 scrollHeigh = Vec2(305, -305);

	//�댯�M����`��
	if (isEscape) {

		switch (states) {

		case ENEMY_UNIT_STATES_ROUND:

			SetDrawBlendMode(DX_BLENDMODE_ALPHA, warningAlpha + 100);
			//���S�Ƃ̋��������߂�
			buff = Vec2((WIN_WIDTH / 2.0f) - playerPos.x, (WIN_HEIGHT / 2.0f) - playerPos.y);

			//�X�N���[���ʂɏ��������
			scrollBuff = scrollAmount;
			if (scrollBuff.x > scrollWidth.x) scrollBuff.x = scrollWidth.x;
			if (scrollBuff.x < scrollWidth.y) scrollBuff.x = scrollWidth.y;
			if (scrollBuff.y > scrollHeigh.x) scrollBuff.y = scrollHeigh.x;
			if (scrollBuff.y < scrollHeigh.y) scrollBuff.y = scrollHeigh.y;

			DrawExtendGraph((WIN_WIDTH / 2.0f) - graphSize / 2.0f - scrollBuff.x,
				(WIN_HEIGHT / 2.0f) - graphSize / 2.0f - scrollBuff.y,
				(WIN_WIDTH / 2.0f) + graphSize / 2.0f - scrollBuff.x,
				(WIN_HEIGHT / 2.0f) + graphSize / 2.0f - scrollBuff.y,
				warningGraph, TRUE);

			break;


		case ENEMY_UNIT_STATES_WALL:
			//�댯�M����`��				�}�W�b�N�i���o�[�͔�����
			switch (direction) {
			case UNIT_ENEMY_DIRECTION_LEFT:

				//�E����ǂ�����ꍇ�̕`�揈�����s��
				x = 900;
				y = 40 + graphSize;

				SetDrawBlendMode(DX_BLENDMODE_ALPHA, warningAlpha);
				for (int i = 0; i < 9; ++i) {
					DrawExtendGraph(x - graphSize / 2.0f, y - graphSize / 2.0f + (i * graphSize), x + graphSize / 2.0f, y + graphSize / 2.0f + (i * graphSize), warningGraph, TRUE);
				}

				break;

			case UNIT_ENEMY_DIRECTION_UP:

				//������ǂ�����ꍇ
				x = 100;
				y = 670;

				SetDrawBlendMode(DX_BLENDMODE_ALPHA, warningAlpha);
				for (int i = 0; i < 890 / graphSize; ++i) {
					DrawExtendGraph(x - graphSize / 2.0f + (i * graphSize), y - graphSize / 2.0f, x + graphSize / 2.0f + (i * graphSize), y + graphSize / 2.0f, warningGraph, TRUE);
				}

				break;

			case UNIT_ENEMY_DIRECTION_RIGHT:

				//������ǂ�����ꍇ
				x = 80;
				y = 40 + graphSize;

				SetDrawBlendMode(DX_BLENDMODE_ALPHA, warningAlpha);
				for (int i = 0; i < 9; ++i) {
					DrawExtendGraph(x - graphSize / 2.0f, y - graphSize / 2.0f + (i * graphSize), x + graphSize / 2.0f, y + graphSize / 2.0f + (i * graphSize), warningGraph, TRUE);
				}

				break;

			case UNIT_ENEMY_DIRECTION_DOWN:

				//�ォ��ǂ�����ꍇ
				x = 100;
				y = 50;

				SetDrawBlendMode(DX_BLENDMODE_ALPHA, warningAlpha);
				for (int i = 0; i < 890 / graphSize; ++i) {
					DrawExtendGraph(x - graphSize / 2.0f + (i * graphSize), y - graphSize / 2.0f, x + graphSize / 2.0f + (i * graphSize), y + graphSize / 2.0f, warningGraph, TRUE);
				}

				break;
			}
		}
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
