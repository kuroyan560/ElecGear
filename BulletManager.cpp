#include "BulletManager.h"

void BulletManager::Init()
{
	for (int i = 0; i < BULLET_MAX; ++i) {
		bullet[i].Init();
	}
}

void BulletManager::Update(Vec2 playerPos, float angle, Vec2 scroolAmount, bool canShot, int bulletGenerateStates)
{

	/*--�X�e�[�^�X�ɉ����Ēe�𐶐����鎞�Ԃ�ς���--*/
	switch (bulletGenerateStates) {
	case BULLET_GENERATE_STATES_NORMAL:
		bulletGenerateTimerDef = BULLET_GENERATE_NORMAL_TIMER;

		break;
	case BULLET_GENERATE_STATES_WAY:
		bulletGenerateTimerDef = BULLET_GENERATE_WAY_TIMER;
		//�������񂱂���way�̐��������Œ�ɂ��Ă������A��X���x���A�b�v����K�v���o�Ă����Ƃ��Ƀ��x���ɉ����ĕς�����悤�ɂ���
		wayGenerateCount = 3;

		break;
	case BULLET_GENERATE_STATES_MACHINEGUN:
		bulletGenerateTimerDef = BULLET_GENERATE_MACHINGUN_TIMER;

		break;
	}

	/*--�e�𐶐����鏈��--*/
	/*if (playerDamageTimer <= 0) {*/
	if(canShot){

		bulletGenerateTimer++;
		if (bulletGenerateTimer >= bulletGenerateTimerDef) {
			bulletGenerateTimer = 0;

			//���݂̐�������e�̃X�e�[�^�X�ɉ����ď�����ς���
			switch (bulletGenerateStates) {
			case BULLET_GENERATE_STATES_NORMAL:
				//�ʏ�̒e�𐶐�
				for (int i = 0; i < BULLET_MAX; ++i) {
					if (bullet[i].isAlive == true) continue;
					bullet[i].Generate(playerPos, angle);
					break;
				}

				break;
			case BULLET_GENERATE_STATES_WAY:
				//way�e�𐶐�
				for (int i = 0; i < wayGenerateCount; ++i) {
					for (int j = 0; j < BULLET_MAX; ++j) {
						if (bullet[j].isAlive == true) continue;
						bullet[j].Generate(playerPos, (angle - (wayGenerateCount / 2) * BULLET_WAY_MACHINGUN_ANGLE) + (BULLET_WAY_MACHINGUN_ANGLE * i));
						break;
					}
				}


				break;
			case BULLET_GENERATE_STATES_MACHINEGUN:
				//�}�V���K���̒e�𐶐�
				for (int i = 0; i < BULLET_MAX; ++i) {
					if (bullet[i].isAlive == true) continue;
					bullet[i].Generate(playerPos, angle);
					break;
				}


				break;
			}
		}
	}

	/*--�e�̍X�V����--*/
	for (int i = 0; i < BULLET_MAX; i++) {
		if (bullet[i].isAlive == false) continue;

		bullet[i].Update(scroolAmount);
	}
}

void BulletManager::Draw(Vec2 scroolAmount)
{
	for (int i = 0; i < BULLET_MAX; i++) {
		//�������Ă��Ȃ������珈�����΂�
		if (bullet[i].isAlive == false) continue;

		bullet[i].Draw(scroolAmount, playerBulletGraph);
	}
}
