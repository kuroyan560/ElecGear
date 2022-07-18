#include "CircleEnemy.h"
#include"EnemyHitEffectManager.h"
#include"EnemyManager.h"
#include "Combo.h"

/*-----����G�p�̊֐�-----*/
CircleEnemys::CircleEnemys(ENEMY_TYPE type) : EnemyInterFace(type)
{
	head = {};
	for (int i = 0; i < CIRCLE_ENEMY_FOLLOWING_COUNT; ++i) {
		following[i] = {};
	}
	states = 0;
	statesTimer = 0;
	isAlive = false;
	speed = 0;
}
void CircleEnemys::Init()
{
	head.isAlive = false;
	for (int i = 0; i < CIRCLE_ENEMY_FOLLOWING_COUNT; ++i) {
		following[i].isAlive = false;
	}
	isAlive = false;

	speed = CIRCLE_ENEMY_SPEED;
}
void CircleEnemys::Draw(Vec2 scrollAmount, int enemyCircleOuter, int enemyCircleInner, int enemyDirection, int warningGraph, Vec2 playerPos)
{
	//�擪�������Ă���ΐ擪��`��
	DrawExtendGraph(head.pos.x - head.size - scrollAmount.x, head.pos.y - head.size - scrollAmount.y, head.pos.x + head.size - scrollAmount.x, head.pos.y + head.size - scrollAmount.y, enemyCircleOuter, TRUE);
	//�G�̌��� �G���W��������Ă�����ɑ傫�������������ʒu�ɕ`��
	DrawRotaGraph(head.pos.x + (cos(head.angle) * (head.size + head.size / 2.0f)) - scrollAmount.x, head.pos.y + (sin(head.angle) * (head.size + head.size / 2.0f)) - scrollAmount.y, CAMERA_RATE, head.angle - 0.35f, enemyDirection, TRUE);
	if (head.isAlive) {
		float a = CIRCLE_ENEMY_COUNT_ALL;
		float buff = ((float)aliveCount / (float)a);
		float scale = (head.size * buff) + 5;
		DrawExtendGraph(head.pos.x - scale - scrollAmount.x, head.pos.y - scale - scrollAmount.y, head.pos.x + scale - scrollAmount.x, head.pos.y + scale - scrollAmount.y, enemyCircleInner, TRUE);
	}

	//�㑱�B��`��
	for (int i = 0; i < CIRCLE_ENEMY_FOLLOWING_COUNT; ++i) {
		//�������Ă��Ȃ���Ε`�悵�Ȃ�
		if (following[i].isAlive == false) continue;

		DrawExtendGraph(following[i].pos.x - following[i].size - scrollAmount.x, following[i].pos.y - following[i].size - scrollAmount.y, following[i].pos.x + following[i].size - scrollAmount.x, following[i].pos.y + following[i].size - scrollAmount.y, enemyCircleOuter, TRUE);

		float a = CIRCLE_ENEMY_COUNT_ALL;
		float buff = ((float)aliveCount / (float)a);
		float scale = (head.size * buff) + 5;
		DrawExtendGraph(following[i].pos.x - scale - scrollAmount.x, following[i].pos.y - scale - scrollAmount.y, following[i].pos.x + scale - scrollAmount.x, following[i].pos.y + scale - scrollAmount.y, enemyCircleInner, TRUE);
	}
}
bool CircleEnemys::HitCheck(Vec2 pos, float size, bool player, bool* addScore)
{
	bool hit = false;

	//�擪�Ƃ̓����蔻��
	float enemyBulletDistance = pos.Distance(head.pos);
	if (enemyBulletDistance <= size + head.size) {
		Hit(player);
		hit = true;
		//�R���{��������
		if (player) {
			Combo::Instance()->Damage();
		}
		else {
			Combo::Instance()->Hit(30);
		}
	}

	//�㑱�Ƃ̓����蔻��
	for (int follow = 0; follow < CIRCLE_ENEMY_FOLLOWING_COUNT; ++follow) {
		//�������Ă��Ȃ�������X�L�b�v
		if (following[follow].isAlive == false) continue;

		float enemyBulletDistance = pos.Distance(following[follow].pos);
		if (enemyBulletDistance <= size + following[follow].size) {
			//�G���S
			Hit(player);
			hit = true;
			//�R���{��������
			if (player) {
				Combo::Instance()->Damage();
			}
			else {
				Combo::Instance()->Hit(30);
			}
		}
	}

	if (addScore != nullptr && hit)*addScore = true;

	if (hit) {
		EnemyHitEffectManager::Instance()->DamagedEffect(head.pos);
		for (int follow = 0; follow < CIRCLE_ENEMY_FOLLOWING_COUNT; ++follow) {
			//�������Ă��Ȃ�������X�L�b�v
			if (following[follow].isAlive == false) continue;
			EnemyHitEffectManager::Instance()->DamagedEffect(following[follow].pos);
		}
	}

	return hit;
}
bool CircleEnemys::HitCheckSlash(Vec2 pos, float angle, int& deathCount, int& hitCount)
{
	if (slashed)return false;
	bool hit = false;

	//�擪�Ƃ̓����蔻��
	if (head.pos.Distance(pos) <= SLASH_RADIUS + head.size) {

		//�a�����a���ɓG�������Ă�����p�x�����߂�
		float bulletAngle = angle - DX_PI_F / 1.95f;
		Vec2 enemyVec = head.pos - pos;
		enemyVec.Normalize();
		float enemyAngle = atan2(enemyVec.y, enemyVec.x);
		//�e�ˏo�p�x - �G�Ƃ̊p�x�̐�Βl����PI / 2.0f��菬���������瓖�����Ă���
		if (fabs(bulletAngle - enemyAngle) <= DX_PI_F / 2.0f) {
			//�G���폜
			Hit(false);
			hit = true;
			deathCount++;
		}
	}

	//�㑱�Ƃ̓����蔻��
	for (int follow = 0; follow < CIRCLE_ENEMY_FOLLOWING_COUNT; ++follow) {
		//�������Ă��Ȃ�������X�L�b�v
		if (following[follow].isAlive == false) continue;

		if (following[follow].pos.Distance(pos) <= SLASH_RADIUS + following[follow].size) {

			//�a�����a���ɓG�������Ă�����p�x�����߂�
			float bulletAngle = angle - DX_PI_F / 1.95f;
			Vec2 enemyVec = following[follow].pos - pos;
			enemyVec.Normalize();
			float enemyAngle = atan2(enemyVec.y, enemyVec.x);
			//�e�ˏo�p�x - �G�Ƃ̊p�x�̐�Βl����PI / 2.0f��菬���������瓖�����Ă���
			if (fabs(bulletAngle - enemyAngle) <= DX_PI_F / 2.0f) {
				//�G���폜
				Hit(false);
				hit = true;
				//�������������X�V
				hitCount++;
			}
		}
	}

	if (hit)slashed = true;

	return hit;
}
void CircleEnemys::Escape()
{
	head.isEscape = true;
	originalSpeed += ENEMY_ECPACE_SPEED;
	speed += ENEMY_ECPACE_SPEED;
}
void CircleEnemys::HitAccel()
{
	//�{������ׂ��ړ����x���X�V
	if (aliveCount > 0) {
		float buff = CIRCLE_ENEMY_COUNT_ALL - aliveCount;
		originalSpeed = CIRCLE_ENEMY_SPEED / (float)(buff / CIRCLE_ENEMY_COUNT_ALL) + CIRCLE_ENEMY_SPEED_DEF;
	}

	//�X�s�[�h�����Z
	speed = CIRCLE_ENEMY_HIT_ACCEL + originalSpeed;

	//�E�o���[�h�������瑬�x���グ��
	if (head.isEscape) {
		originalSpeed += 0.5f;
		speed += 0.5f;
	}
}
void CircleEnemys::Hit(bool player)
{
	//��납�珇�Ԃɍ폜���Ă���
	if (aliveCount > 1) {
		for (int i = CIRCLE_ENEMY_FOLLOWING_COUNT - 1; i >= 0; --i) {
			//���łɎ���ł����珈�����΂�
			if (following[i].isAlive == false) continue;

			//�E��
			following[i].Init();
			EnemyManager::Instance()->PlayDamageSE();
			EnemyHitEffectManager::Instance()->DamagedEffect(following[i].pos);
			break;
		}
	}
	else {
		//�E��
		head.Init();
		if (!player) {
			EnemyManager::Instance()->PlayDeadSE();
		}
		EnemyHitEffectManager::Instance()->DeadEffect(head.pos);
	}

	//���������Ċm�F���A���ׂĎ���ł�����傫�ȃt���O��܂�
	aliveCount = 0;
	if (head.isAlive == true) aliveCount++;
	for (int i = 0; i < CIRCLE_ENEMY_FOLLOWING_COUNT; ++i) {
		if (following[i].isAlive == false) continue;
		aliveCount++;
	}
	//aliveCount��0�ȉ��������炷�ׂĎ���ł���Ƃ������ƂȂ̂ŏ�����
	if (aliveCount <= 0) {
		Init();
	}

	//����������
	HitAccel();
}
/*-----�擪�p�̊֐�-----*/
void CircleEnemyHead::Init()
{
	isAlive = false;

	isEscape = false;
}
void CircleEnemyHead::Generate(Vec2 generatePos)
{
	//�ʒu�����߂�
	pos = generatePos;
	//pos = Vec2(GetRand(WIN_WIDTH), GetRand(WIN_HEIGHT));
	//���̑�����������
	size = 30 * CAMERA_RATE;
	angle = 0;
	radius = 0;
	isAlive = true;
	hp = CIRCLE_ENEMY_HP;
	isEscape = false;
}
void CircleEnemyHead::Update(int states, Vec2 playerPos, float speed)
{
	//���[�J���ϐ���錾
	float playerAngle = 0;
	Vec2 nowVec = {};
	Vec2 playerVec = {};
	float crossProduct = 0;

	//�n���ꂽ�X�e�[�^�X�ɂ���ď�����ς���
	switch (states)
	{
	case CIRCLE_ENEMY_STATES_ROCKON:

		/*-----�p�x�̍X�V����-----*/
		playerAngle = atan2(playerPos.y - pos.y, playerPos.x - pos.x);
		//���݂̊p�x�̈ړ��x�N�g��
		nowVec = Vec2(cosf(angle) * speed, sinf(angle) * speed);
		//�v���C���[�Ƃ̊p�x�̈ړ��x�N�g��
		playerVec = Vec2(cosf(playerAngle) * speed, sinf(playerAngle) * speed);
		//�O�ς��v�Z
		crossProduct = nowVec.Cross(playerVec);
		//�O�ς̌v�Z���ʂ���p�x�𒲐�
		if (crossProduct > 0)
		{
			angle += CIRCLE_ENEMY_CHANGE_ANGLE;
		}
		else if (crossProduct < 0)
		{
			angle -= CIRCLE_ENEMY_CHANGE_ANGLE;
		}

		/*-----�E�o���[�h��on�ɂȂ����璆�S�Ƃ͋t�����ɓ����邽�߁A�p�x���Čv�Z����-----*/
		if (isEscape) {
			angle = atan2(pos.y - playerPos.y, pos.x - playerPos.x);
		}

		/*-----�ړ��̍X�V����-----*/
		pos += Vec2(cosf(angle) * speed, sinf(angle) * speed);

		break;

	case CIRCLE_ENEMY_STATES_ROUND:

		//���݂̈ʒu���璆�S�ւ̔��a�Ɗp�x�����߂�
		radius = pos.Distance(Vec2(WIN_WIDTH / 2.0f, WIN_HEIGHT / 2.0f));
		angle = atan2(pos.y - WIN_HEIGHT / 2.0f, pos.x - WIN_WIDTH / 2.0f);
		//�p�x���X�V���Ĉړ�������
		angle += 0.005f;
		pos = Vec2(cos(angle) * radius + WIN_WIDTH / 2.0f, sin(angle) * radius + WIN_HEIGHT / 2.0f);

		break;

	default:
		break;
	}

	//hp��0�������珉����
	if (hp <= 0) {
		Init();
	}
}
/*-----�㑱�p�̊֐�-----*/
void CircleEnemyFollowing::Init()
{
	isAlive = false;
}
void CircleEnemyFollowing::Generate(Vec2 prevPos)
{
	//�ЂƂO�̓G�̈ʒu�ƒ��S�_�Ƃ̋��������߂�
	float radius = prevPos.Distance(Vec2(WIN_WIDTH / 2.0f, WIN_HEIGHT / 2.0f));
	//�O�̓G�ƒ��S�Ƃ̊p�x�����߂�
	float angle = atan2(prevPos.y - WIN_HEIGHT / 2.0f, prevPos.x - WIN_WIDTH / 2.0f);
	//������]�������ʒu�ɐ�������
	angle -= 0.1f;
	pos.Init();
	pos += Vec2(cos(angle) * radius + WIN_WIDTH / 2.0f, sin(angle) * radius + WIN_HEIGHT / 2.0f);
	//���̑�����������
	size = 30 * CAMERA_RATE;
	isAlive = true;
	hp = CIRCLE_ENEMY_HP;
}
void CircleEnemyFollowing::Update(Vec2 prevPos, float speed)
{
	//�O�̓G��ǂ�������
	Vec2 forwardVec = Vec2(prevPos.x - pos.x, prevPos.y - pos.y);
	forwardVec.Normalize();

	pos += Vec2(forwardVec.x * speed, forwardVec.y * speed);

	//�O�ƏՓ˂��Ă����炨�����ǂ�
	float distance = pos.Distance(prevPos);
	if (distance <= size * 2 + size / 2.0f) {
		//�Ԃ����Ă�����Ԃ����Ă��镪���������߂�
		float pushAmount = size * 2 + size / 2.0f - distance;
		//�����߂��p�x�����߂�
		Vec2 pushVec = Vec2(pos.x - prevPos.x, pos.y - prevPos.y);
		pushVec.Normalize();
		//�����߂�
		pos += Vec2(pushVec.x * pushAmount, pushVec.y * pushAmount);
	}

	//hp��0�������珉����
	if (hp <= 0) {
		Init();
	}

}

/*---�v���C���[�ǔ�---*/
void TrackingCircleEnemys::Generate(Vec2 generatePos, Vec2 playerPos, int* hp, float* angle, Vec2* targetPos)
{
	//�܂��擪�𐶐�����
	head.Generate(generatePos);
	//�����Č㑱�𐶐�����
	following[0].Generate(head.pos);
	for (int i = 1; i < CIRCLE_ENEMY_FOLLOWING_COUNT - 1; ++i) {
		following[i].Generate(following[i - 1].pos);
	}
	//�t���O�𗧂Ă�
	isAlive = true;
	aliveCount = CIRCLE_ENEMY_COUNT_ALL;

	//speed�������� �����G�����Ȃ��Ȃ�΂Ȃ�قǑ����Ȃ�
	speed = CIRCLE_ENEMY_SPEED / (CIRCLE_ENEMY_COUNT_ALL / CIRCLE_ENEMY_COUNT_ALL) + CIRCLE_ENEMY_SPEED_DEF;
	originalSpeed = speed;

	slashed = false;
}
void TrackingCircleEnemys::Update(Vec2 playerPos)
{
	/*-----�X�e�[�^�X�̍X�V-----*/
	statesTimer++;
	if (statesTimer >= CIRCLE_ENEMY_STATES_DELAY) {
		statesTimer = 0;
		if (states == CIRCLE_ENEMY_STATES_ROCKON) {
			//states = CIRCLE_ENEMY_STATES_ROUND;
		}
		else {
			states = CIRCLE_ENEMY_STATES_ROCKON;
		}
	}

	/*-----�擪�̍X�V-----*/
	head.Update(states, playerPos, speed);

	/*-----�㑱�̍X�V-----*/
	following[0].Update(head.pos, speed);
	for (int i = 1; i < CIRCLE_ENEMY_FOLLOWING_COUNT - 1; ++i) {
		following[i].Update(following[i - 1].pos, speed);
	}

	/*-----�����m�F-----*/
	aliveCount = 0;
	if (head.isAlive == true) aliveCount++;
	for (int i = 0; i < CIRCLE_ENEMY_FOLLOWING_COUNT; ++i) {
		if (following[i].isAlive == false) continue;
		aliveCount++;
	}
	//aliveCount��0�ȉ��������炷�ׂĎ���ł���Ƃ������ƂȂ̂ŏ�����
	if (aliveCount <= 0) {
		Init();
	}

	//�{������ׂ��X�s�[�h - ���݂̃X�s�[�h�̐�Βl��1�ȏゾ������߂Â���
	if (fabs(originalSpeed - speed) >= 1) {
		//+-�ɕ����Ē���
		if (speed > 0) {
			speed -= 1;
		}
		else {
			speed += 1;
		}
	}

}