#include "BulletManager.h"

void BulletManager::Init()
{
	for (int i = 0; i < BULLET_MAX; ++i) {
		bullet[i].Init();
	}
}

void BulletManager::Update(Vec2 playerPos, float angle, Vec2 scroolAmount, bool canShot, int bulletGenerateStates)
{

	/*--ステータスに応じて弾を生成する時間を変える--*/
	switch (bulletGenerateStates) {
	case BULLET_GENERATE_STATES_NORMAL:
		bulletGenerateTimerDef = BULLET_GENERATE_NORMAL_TIMER;

		break;
	case BULLET_GENERATE_STATES_WAY:
		bulletGenerateTimerDef = BULLET_GENERATE_WAY_TIMER;
		//いったんここでwayの生成数を固定にしておくが、後々レベルアップする必要が出てきたときにレベルに応じて変えられるようにする
		wayGenerateCount = 3;

		break;
	case BULLET_GENERATE_STATES_MACHINEGUN:
		bulletGenerateTimerDef = BULLET_GENERATE_MACHINGUN_TIMER;

		break;
	}

	/*--弾を生成する処理--*/
	/*if (playerDamageTimer <= 0) {*/
	if(canShot){

		bulletGenerateTimer++;
		if (bulletGenerateTimer >= bulletGenerateTimerDef) {
			bulletGenerateTimer = 0;

			//現在の生成する弾のステータスに応じて処理を変える
			switch (bulletGenerateStates) {
			case BULLET_GENERATE_STATES_NORMAL:
				//通常の弾を生成
				for (int i = 0; i < BULLET_MAX; ++i) {
					if (bullet[i].isAlive == true) continue;
					bullet[i].Generate(playerPos, angle);
					break;
				}

				break;
			case BULLET_GENERATE_STATES_WAY:
				//way弾を生成
				for (int i = 0; i < wayGenerateCount; ++i) {
					for (int j = 0; j < BULLET_MAX; ++j) {
						if (bullet[j].isAlive == true) continue;
						bullet[j].Generate(playerPos, (angle - (wayGenerateCount / 2) * BULLET_WAY_MACHINGUN_ANGLE) + (BULLET_WAY_MACHINGUN_ANGLE * i));
						break;
					}
				}


				break;
			case BULLET_GENERATE_STATES_MACHINEGUN:
				//マシンガンの弾を生成
				for (int i = 0; i < BULLET_MAX; ++i) {
					if (bullet[i].isAlive == true) continue;
					bullet[i].Generate(playerPos, angle);
					break;
				}


				break;
			}
		}
	}

	/*--弾の更新処理--*/
	for (int i = 0; i < BULLET_MAX; i++) {
		if (bullet[i].isAlive == false) continue;

		bullet[i].Update(scroolAmount);
	}
}

void BulletManager::Draw(Vec2 scroolAmount)
{
	for (int i = 0; i < BULLET_MAX; i++) {
		//生成していなかったら処理を飛ばす
		if (bullet[i].isAlive == false) continue;

		bullet[i].Draw(scroolAmount, playerBulletGraph);
	}
}
