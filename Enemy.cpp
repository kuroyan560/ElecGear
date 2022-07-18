#include "Enemy.h"
#include"EnemyHitEffectManager.h"
#include"EnemyManager.h"
#include"Easing.h"
#include"Combo.h"

#define ENEMY_GENERATE_TIMER 50.0f

Enemy::Enemy(ENEMY_TYPE type) : EnemyInterFace(type)
{
	pos = {};
	speed = 0;
	size = 30 * CAMERA_RATE;
	angle = 0;
	isAlive = false;
	hp = ENEMY_HP;
}
void Enemy::Init()
{
	isAlive = false;

	isEscape = false;
}
void Enemy::Draw(Vec2 scrollAmount, int enemyCircleOuter, int enemyCircleInner, int enemyDirection, int warningGraph, Vec2 playerPos)
{
	//��������������A���t�@�u�����h��������
	if (!isGenerate) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((ENEMY_GENERATE_TIMER - generateTimer) / ENEMY_GENERATE_TIMER) * 255.0f);
	}

	//�G��`��
	DrawExtendGraph(pos.x - size - scrollAmount.x, pos.y - size - scrollAmount.y, pos.x + size - scrollAmount.x, pos.y + size - scrollAmount.y, enemyCircleOuter, TRUE);
	float scale = (size / (ENEMY_HP / hp));
	DrawExtendGraph(pos.x - scale - scrollAmount.x, pos.y - scale - scrollAmount.y, pos.x + scale - scrollAmount.x, pos.y + scale - scrollAmount.y, enemyCircleInner, TRUE);
	//DrawExtendGraph(pos.x - (size / hp) - scrollAmount.x, pos.y - (size / hp) - scrollAmount.y, pos.x + (size / hp) - scrollAmount.x, pos.y + (size / hp) - scrollAmount.y, enemyCircleInner, TRUE);
	//�G�̌��� �G���W��������Ă�����ɑ傫�������������ʒu�ɕ`��
	DrawRotaGraph(pos.x + (cos(angle) * (size + size / 2.0f)) - scrollAmount.x, pos.y + (sin(angle) * (size + size / 2.0f)) - scrollAmount.y, CAMERA_RATE, angle - 0.35f, enemyDirection, TRUE);

	int graphSize = 25;
	Vec2 buff;
	Vec2 scrollBuff;
	Vec2 scrollWidth = Vec2(550 - 160 + 35, -250 - 160);
	Vec2 scrollHeigh = Vec2(305 + 15, -305 - 15);

	//�댯�M����`��

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	//���S�Ƃ̋��������߂�
	buff = Vec2(pos.x - playerPos.x, pos.y - playerPos.y);

	//�X�N���[���ʂɏ��������
	scrollBuff = buff;
	if (buff.x > scrollWidth.x) scrollBuff.x = scrollWidth.x;
	if (buff.x < scrollWidth.y) scrollBuff.x = scrollWidth.y;
	if (buff.y > scrollHeigh.x) scrollBuff.y = scrollHeigh.x;
	if (buff.y < scrollHeigh.y) scrollBuff.y = scrollHeigh.y;

	//�댯�M������ʓ��Ɏ��܂��Ă�����\�����Ȃ�
	if (scrollBuff.x < scrollWidth.x && scrollBuff.x > scrollWidth.y && scrollBuff.y < scrollHeigh.x && scrollBuff.y > scrollHeigh.y) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0);
	}

	DrawExtendGraph(HALF_WIN_WIDTH - graphSize + scrollBuff.x - 160,
		HALF_WIN_HEIGHT - graphSize + scrollBuff.y,
		HALF_WIN_WIDTH + graphSize + scrollBuff.x - 160,
		HALF_WIN_HEIGHT + graphSize + scrollBuff.y,
		warningGraph, TRUE);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
bool Enemy::HitCheck(Vec2 pos, float size, bool player, bool* addScore)
{
	bool hit = false;

	//�v���C���[�Ƃ̔��肩�������������瓖���蔻����s��Ȃ�
	if (player && !isGenerate) {
		return false;
	}

	//�����蔻����s��
	float enemyBulletDistance = pos.Distance(this->pos);
	if (enemyBulletDistance <= size + this->size) {
		hit = true;

		if (player)	//�v���C���[
		{
			this->Init();	//�폜
			//EnemyManager::Instance()->PlayDeadSE();
			EnemyHitEffectManager::Instance()->DamagedEffect(this->pos);
			//�R���{��������
			Combo::Instance()->Damage();
		}
		else	//�e
		{
			//�G��HP�����
			this->hp--;
			EnemyManager::Instance()->PlayDamageSE();
			//�G��HP��0�ȉ��ɂȂ����玀�S
			if (this->hp <= 0) {
				this->Init();	//�폜
				EnemyManager::Instance()->PlayDeadSE();
				EnemyHitEffectManager::Instance()->DeadEffect(this->pos);
				if (addScore != nullptr)*addScore = true;
			}
			else
			{
				EnemyHitEffectManager::Instance()->DamagedEffect(this->pos);
			}
		}
	}

	if (hit) {
		hitStanTimer = TRACKING_ENEMY_DAMAGE_STAN_TIME;
	}

	return hit;
}
bool Enemy::HitCheckSlash(Vec2 pos, float angle, int& deathCount, int& hitCount)
{
	if (slashed)
	{
		return false;
	}

	bool hit = false;

	if (this->pos.Distance(pos) <= SLASH_RADIUS + size) {

		//�a�����a���ɓG�������Ă�����p�x�����߂�
		float bulletAngle = angle - DX_PI_F / 1.95f;
		Vec2 enemyVec = this->pos - pos;
		enemyVec.Normalize();
		float enemyAngle = atan2(enemyVec.y, enemyVec.x);
		//�e�ˏo�p�x - �G�Ƃ̊p�x�̐�Βl����PI / 2.0f��菬���������瓖�����Ă���
		if (fabs(bulletAngle - enemyAngle) <= DX_PI_F / 2.0f * 1.1) {
			hit = true;
			//�G��HP�����
			this->hp -= ENEMY_HP / 2;
		}
	}

	if (hit)
	{
		//�G��HP��0�ȉ��ɂȂ����玀�S
		if (this->hp <= 0) {
			this->Init();	//�폜
			EnemyManager::Instance()->PlayDeadSE();
			EnemyHitEffectManager::Instance()->DeadEffect(this->pos);
			deathCount++;
		}
		else
		{
			EnemyHitEffectManager::Instance()->DamagedEffect(this->pos);
			EnemyManager::Instance()->PlayDamageSE();
			//�������������X�V
			hitCount++;
		}
		slashed = true;
	}
	return hit;
}
void Enemy::Escape()
{
	isEscape = true;
	speed += ENEMY_ECPACE_SPEED;
}

/*--�v���C���[�ǔ�--*/
void TrackingEnemy::Generate(Vec2 generatePos, Vec2 playerPos, int* hp, float* angle, Vec2* targetPos)
{
	pos = generatePos;
	//pos = Vec2(GetRand(WIN_WIDTH), GetRand(WIN_HEIGHT));
	speed = GetRand(TRACKING_ENEMY_MAX_SPEED - TRACKING_ENEMY_MIN_SPEED) + TRACKING_ENEMY_MIN_SPEED;
	speed *= 1.5f;
	this->angle = atan2(playerPos.y - pos.y, playerPos.x - pos.x);
	move = playerPos - pos;
	move.Normalize();
	move.x *= speed;
	move.y *= speed;
	isAlive = true;
	this->hp = ENEMY_HP;

	isEscape = false;
	hitStanTimer = 0;
	slashed = false;

	isGenerate = false;
	generateTimer = ENEMY_GENERATE_TIMER - 1;
}
void TrackingEnemy::Update(Vec2 playerPos)
{
	/*-----�p�x�̍X�V����-----*/
	//float playerAngle = atan2(playerPos.y - pos.y, playerPos.x - pos.x);
	////���݂̊p�x�̈ړ��x�N�g��
	//Vec2 nowVec = Vec2(cosf(angle) * speed, sinf(angle) * speed);
	////�v���C���[�Ƃ̊p�x�̈ړ��x�N�g��
	//Vec2 playerVec = Vec2(cosf(playerAngle) * speed, sinf(playerAngle) * speed);
	////�O�ς��v�Z
	//float crossProduct = nowVec.Cross(playerVec);
	////�O�ς̌v�Z���ʂ���p�x�𒲐�
	//if (crossProduct > 0)
	//{
	//	angle += ENEMY_CHANGE_ANGLE;
	//}
	//else if (crossProduct < 0)
	//{
	//	angle -= ENEMY_CHANGE_ANGLE;

	//������Ԃ�������X�V�������s��
	if (isGenerate) {

		if (hitStanTimer) {
			hitStanTimer--;
		}
		else
		{
			if (!isEscape) {
				Vec2 toVec = playerPos - pos;
				toVec.Normalize();

				//�~�O�ɂ���Ƃ����f��
				float rate = ((MAP_CENTER - pos).Length() - MAP_RADIUS) / (ENEMY_GENERATE_RADIUS - MAP_RADIUS);
				if (rate < 0)rate = 0;
				if (1 < rate)rate = 1;
				//rate = 1.0f - rate;
				toVec.x *= speed;
				toVec.y *= speed;

				move.x = Easing::Lerp(move.x, toVec.x, 0.08f) * Easing::ExpOut(rate, 1.0f, 1.0f, OUT_OF_MAP_SPEED_RATE_MIN);
				move.y = Easing::Lerp(move.y, toVec.y, 0.08f) * Easing::ExpOut(rate, 1.0f, 1.0f, OUT_OF_MAP_SPEED_RATE_MIN);

				pos += move;

				angle = atan2(move.y, move.x);
			}
			/*-----�E�o���[�h��on�ɂȂ����璆�S�Ƃ͋t�����ɓ����邽�߁A�p�x���Čv�Z����-----*/
			else {
				angle = atan2(pos.y - playerPos.y, pos.x - playerPos.x);
				pos += Vec2(cosf(angle) * speed, sinf(angle) * speed);	//����
			}
		}

		//hp��0�ɂȂ��Ă����玀�S
		if (hp <= 0) {
			Init();
		}

		//�G���v���C���[����}�b�v���a���ړ������珉����
		if (pos.Distance(playerPos) >= MAP_RADIUS_2_TIMES) {
			Init();
		}
	}

	//�������Ă��Ȃ�������^�C�}�[���X�V
	else {
		generateTimer--;
		if (generateTimer <= 0) {
			isGenerate = true;
		}
	}
}

/*--�������ꂽ�p�x�ɐ^������--*/
void StraightEnemy::Generate(Vec2 generatePos, Vec2 playerPos, int* hp, float* angle, Vec2* targetPos)
{
	pos = generatePos;
	//pos = Vec2(GetRand(WIN_WIDTH), GetRand(WIN_HEIGHT));
	speed = GetRand(STRAIGHT_ENEMY_MAX_SPEED - STRAIGHT_ENEMY_MIN_SPEED) + STRAIGHT_ENEMY_MIN_SPEED;
	speed *= 1.5f;
	//angle�ɒl�������Ă��Ȃ�������v���C���[�̕����ɔ�΂�
	if (angle == nullptr) {
		this->angle = atan2(playerPos.y - pos.y, playerPos.x - pos.x);
	}
	else {
		this->angle = *angle;
	}
	move = playerPos - pos;
	move.Normalize();
	move.x *= speed;
	move.y *= speed;
	isAlive = true;
	if (hp == nullptr) {
		this->hp = 1;
	}
	else {
		this->hp = *hp;
	}

	isEscape = false;
	hitStanTimer = 0;
	slashed = false;

	isGenerate = false;
	generateTimer = ENEMY_GENERATE_TIMER - 1;
}
void StraightEnemy::Update(Vec2 playerPos)
{
	//������Ԃ�������X�V�������s��
	if (isGenerate) {

		if (hitStanTimer) {
			hitStanTimer--;
		}
		else
		{
			pos += Vec2(cosf(angle) * speed, sinf(angle) * speed);	//����
		}

		//hp��0�ɂȂ��Ă����玀�S
		if (hp <= 0) {
			Init();
		}

		//�G���v���C���[����}�b�v���a���ړ������珉����
		if (pos.Distance(playerPos) >= MAP_RADIUS_2_TIMES) {
			Init();
		}
	}

	//�������Ă��Ȃ�������^�C�}�[���X�V
	else {
		generateTimer--;
		if (generateTimer <= 0) {
			isGenerate = true;
		}
	}
}

/*--�w�肳�ꂽ�ʒu�܂ňړ�--*/
void DestinationEnemy::Generate(Vec2 generatePos, Vec2 playerPos, int* hp, float* angle, Vec2* targetPos)
{
	pos = generatePos;
	//pos = Vec2(GetRand(WIN_WIDTH), GetRand(WIN_HEIGHT));
	speed = DESTIANTION_ENEMY_SPEED;
	//angle�ɒl�������Ă��Ȃ�������v���C���[�̕����ɔ�΂�
	if (angle == nullptr) {
		this->angle = atan2(playerPos.y - pos.y, playerPos.x - pos.x);
	}
	else {
		this->angle = *angle;
	}
	//targetPos�̒l��nullptr����Ȃ�������l������
	if (targetPos == nullptr) {
		this->targetPos = Vec2(HALF_WIN_WIDTH, HALF_WIN_HEIGHT);
		distanceDef = pos.Distance(Vec2(HALF_WIN_WIDTH, HALF_WIN_HEIGHT));
	}
	else {
		this->targetPos = *targetPos;
		distanceDef = pos.Distance(*targetPos);
	}
	move = playerPos - pos;
	move.Normalize();
	move.x *= speed;
	move.y *= speed;
	isAlive = true;
	this->hp = ENEMY_HP;

	isEscape = false;
	hitStanTimer = 0;
	slashed = false;
	escapeTimer = 0;

	isGenerate = false;
	generateTimer = ENEMY_GENERATE_TIMER - 1;
}
void DestinationEnemy::Update(Vec2 playerPos)
{
	//������Ԃ�������X�V�������s��
	if (isGenerate) {


		if (hitStanTimer) {
			hitStanTimer--;
		}
		else
		{
			//�E�o���[�h����Ȃ�������
			if (!isEscape) {
				//�ړ�������������߂�
				Vec2 moveVec = Vec2(targetPos.x - pos.x, targetPos.y - pos.y);
				moveVec.Normalize();
				//�ړ�����ʂ����߂�
				float distance = targetPos.Distance(pos);
				//�ړ����x���X�V
				speed = DESTIANTION_ENEMY_SPEED * (distance / distanceDef);
				//������݂���
				if (speed <= 2 && distance >= 1.0f) {
					speed = 2.0f;
				}

				//�ړ�������
				if (fabs(distance) > 1) {
					pos += Vec2(moveVec.x * speed, moveVec.y * speed);
				}

				//�v���C���[�̕�������������
				angle = atan2f(playerPos.y - pos.y, playerPos.x - pos.x);
			}
			else {
				//���S�Ƃ͋t�����̃A���O�������߂�
				angle = atan2f(pos.y - HALF_WIN_HEIGHT, pos.x - HALF_WIN_WIDTH);
				//�ړ�������
				pos += Vec2(cosf(angle) * speed, sinf(angle) * speed);
			}
		}

		//�E�o����܂ł̃^�C�}�[�����Z����
		escapeTimer++;
		if (escapeTimer >= DESTINATION_ENEMY_ESCAPE_TIME) {
			speed = 5.0f;
			isEscape = true;
		}

		//hp��0�ɂȂ��Ă����玀�S
		if (hp <= 0) {
			Init();
		}

		//�G���v���C���[����}�b�v���a���ړ������珉����
		if (pos.Distance(playerPos) >= MAP_RADIUS_2_TIMES) {
			Init();
		}
	}

	//�������Ă��Ȃ�������^�C�}�[���X�V
	else {
		generateTimer--;
		if (generateTimer <= 0) {
			isGenerate = true;
		}
	}
}

/*--�����^�Ɉړ�--*/
void InfinityEnemy::Generate(Vec2 generatePos, Vec2 playerPos, int* hp, float* angle, Vec2* targetPos)
{
	pos = Vec2(HALF_WIN_WIDTH, HALF_WIN_HEIGHT);
	//angle�ɒl�������Ă��Ȃ�������v���C���[�̕����ɔ�΂�
	if (angle == nullptr) {
		this->angle = atan2(playerPos.y - pos.y, playerPos.x - pos.x);
	}
	else {
		this->angle = *angle;
	}
	move = playerPos - pos;
	move.Normalize();
	move.x *= speed;
	move.y *= speed;
	isAlive = true;
	this->hp = ENEMY_HP;

	isEscape = false;
	hitStanTimer = 0;
	slashed = false;

	//��]�ɕK�v�ȕϐ���������
	rotationAngle = 0;
	rotationAngleAmount = 0;
	isRotationLeft = true;

	isGenerate = false;
	generateTimer = ENEMY_GENERATE_TIMER - 1;
}
void InfinityEnemy::Update(Vec2 playerPos)
{
	//������Ԃ�������X�V�������s��
	if (isGenerate) {

		if (hitStanTimer) {
			hitStanTimer--;
		}
		else
		{
			//�ǂ���������]���Ă��邩
			if (isRotationLeft) {
				//��]�ʂ����Z
				rotationAngle += INFINITY_ENEMY_ROTATION_ADD;
				rotationAngleAmount += INFINITY_ENEMY_ROTATION_ADD;
				//��]������
				pos = Vec2(cosf(rotationAngle) * INFINITY_ENEMY_ROTATION_RADIUS + INFINITY_ENEMY_ROTATION_LEFT.x,
					sinf(rotationAngle) * INFINITY_ENEMY_ROTATION_RADIUS + INFINITY_ENEMY_ROTATION_LEFT.y);
				//�������]������
				if (rotationAngleAmount >= DX_TWO_PI_F) {
					rotationAngle = DX_PI_F;	//�E���̒��S���猩��ƁA��ʒ��S�͍����Ȃ̂�
					rotationAngleAmount = 0;
					isRotationLeft = false;
				}
				//�G�̌����Ă���������X�V
				angle = atan2(playerPos.y - pos.y, playerPos.x - pos.x);
			}
			else {
				//�E������]���Ă���ꍇ

				//��]�ʂ����Z
				rotationAngle -= INFINITY_ENEMY_ROTATION_ADD;
				rotationAngleAmount += INFINITY_ENEMY_ROTATION_ADD;
				//��]�ʂ�0�ȉ��ɂȂ�����
				if (rotationAngle < 0) {
					rotationAngle = DX_TWO_PI_F;
				}
				//��]������
				pos = Vec2(cosf(rotationAngle) * INFINITY_ENEMY_ROTATION_RADIUS + INFINITY_ENEMY_ROTATION_RIGHT.x,
					sinf(rotationAngle) * INFINITY_ENEMY_ROTATION_RADIUS + INFINITY_ENEMY_ROTATION_RIGHT.y);
				//�������]������
				if (rotationAngleAmount >= DX_TWO_PI_F) {
					rotationAngle = 0;
					rotationAngleAmount = 0;
					isRotationLeft = true;
				}
				//�G�̌����Ă���������X�V
				angle = atan2(playerPos.y - pos.y, playerPos.x - pos.x);
			}
		}

		//hp��0�ɂȂ��Ă����玀�S
		if (hp <= 0) {
			Init();
		}

		//�G���v���C���[����}�b�v���a���ړ������珉����
		if (pos.Distance(playerPos) >= MAP_RADIUS_2_TIMES) {
			Init();
		}


	}
	//�������Ă��Ȃ�������^�C�}�[���X�V
	else {
		generateTimer--;
		if (generateTimer <= 0) {
			isGenerate = true;
		}
	}
}

/*--���������ʒu�ɒ�~--*/
void StandByEnemy::Generate(Vec2 generatePos, Vec2 playerPos, int* hp, float* angle, Vec2* targetPos)
{
	pos = generatePos;
	speed = 0.0f;
	isAlive = true;
	this->hp = ENEMY_HP;

	isGenerate = false;
	generateTimer = ENEMY_GENERATE_TIMER - 1;
}
void StandByEnemy::Update(Vec2 playerPos)
{
	//������Ԃ�������X�V�������s��
	if (isGenerate) {

		escapeCount++;
		if (escapeCount >= STANDBY_ENEMY_ESCAPE_COUNT) {
			speed = 2.0f;
			isEscape = true;
		}

		//��Ƀv���C���[�̕���������悤�ɂ���
		angle = atan2(playerPos.y - pos.y, playerPos.x - pos.x);

		//hp��0�ɂȂ��Ă����玀�S
		if (hp <= 0) {
			Init();
		}

		//�G���v���C���[����}�b�v���a���ړ������珉����
		if (pos.Distance(playerPos) >= MAP_RADIUS_2_TIMES) {
			Init();
		}


	}
	//�������Ă��Ȃ�������^�C�}�[���X�V
	else {
		generateTimer--;
		if (generateTimer <= 0) {
			isGenerate = true;
		}
	}
}

/*--�n�C�X�R�A�̓G--*/
void HighScoreEnemy::Generate(Vec2 generatePos, Vec2 playerPos, int* hp, float* angle, Vec2* targetPos)
{
	//���W��������
	pos = Vec2(HALF_WIN_WIDTH + MAP_RADIUS, HALF_WIN_HEIGHT);
	speed = 0;
	rotationCount = 0;
	rotationAngle = 0;
	rotationRadius = MAP_RADIUS;
	isRadiusMinus = true;
	hitStanTimer = 0;

	this->hp = 1;
	isAlive = true;

	isGenerate = false;
	generateTimer = ENEMY_GENERATE_TIMER - 1;
}
void HighScoreEnemy::Update(Vec2 playerPos)
{
	//������Ԃ�������X�V�������s��
	if (isGenerate) {

		if (hitStanTimer) {
			hitStanTimer--;
		}
		else
		{
			//�����v���ɉ�]������
			rotationAngle += HIGHTSCORE_ENEMY_ROTATION_ADD;
			//���������
			if (rotationAngle >= DX_TWO_PI_F) {
				rotationAngle = 0;
				rotationCount++;
				//����̉񐔉�]������
				if (rotationCount >= HIGHTSCORE_ENEMY_ROTATION_COUNT) {
					isRadiusMinus = false;
				}
			}
			//�������Ȃ�t���O�������Ă����甼�a������������
			if (isRadiusMinus) {
				//�Œ�l���傫�������珬��������
				if (rotationRadius > HIGHTSCORE_ENEMY_RADIUS_MIN) {
					rotationRadius -= HIGHTSCORE_ENEMY_RADIUS_CHANGE;
					//�Œ�l��菬�����Ȃ��Ă����牟���߂�
					if (rotationRadius < HIGHTSCORE_ENEMY_RADIUS_MIN) {
						rotationRadius = HIGHTSCORE_ENEMY_RADIUS_MIN;
					}
				}
			}
			//�������Ȃ�t���O�������Ă��Ȃ������甼�a��u��������
			else {
				//�ǂ�ǂ�傫������
				rotationRadius += HIGHTSCORE_ENEMY_RADIUS_CHANGE;
			}

			//�ړ�������
			pos = Vec2(cosf(rotationAngle) * rotationRadius + HALF_WIN_WIDTH,
				sinf(rotationAngle) * rotationRadius + HALF_WIN_HEIGHT);
		}

		//�v���C���[�̕����������悤�ɂ���
		angle = atan2f(playerPos.y - pos.y, playerPos.x - pos.x);

		//hp��0�ɂȂ��Ă����玀�S
		if (hp <= 0) {
			Init();
		}

		//�G���v���C���[����}�b�v���a���ړ������珉����
		if (pos.Distance(playerPos) >= MAP_RADIUS_2_TIMES) {
			Init();
		}
	}
	//�������Ă��Ȃ�������^�C�}�[���X�V
	else {
		generateTimer--;
		if (generateTimer <= 0) {
			isGenerate = true;
		}
	}
}
