#include "GameScene.h"
#include "EnemyManager.h"
#include "BulletManager.h"
#include"Constants.h"
#include"SoundEmitter.h"
#include"EnemyInterFace.h"
#include"EnemyAppearManager.h"
#include"EnemyHitEffectManager.h"
#include"BulletEffectManager.h"
#include"Easing.h"
#include"EnemyAppearEffectEmitter.h"
#include"Combo.h"
#include"ScoreManager.h"

GameScene::GameScene(SceneChanger* changer)
	:BaseScene(changer)
{
	//�e�摜�����[�h
	screenFrameGraph = LoadGraph("resource/screen_frame.png");
	meterGraph = LoadGraph("resource/meter.png");
	meterIndicaterGraph = LoadGraph("resource/meter_indicater.png");
	LoadDivGraph("resource/character_card_default.png", 7, 7, 1, 350, 500, playerDevGraph);		//�v���C���[�̘A��
	charactorGraph = playerDevGraph[0];
	charactorCloseGraph = playerDevGraph[3];
	charactorDamageGraph = LoadGraph("resource/character_card_damage.png");
	charactorDamageGraph_default = LoadGraph("resource/character_card_damage_default.png");
	charactorDamageGraph_close = LoadGraph("resource/character_card_damage_default_close.png");
	gaugeInnerGraph = LoadGraph("resource/gauge_inner.png");
	gaugeInnerGraph_damaged = LoadGraph("resource/gauge_inner_damaged.png");
	gaugeOuterGraph = LoadGraph("resource/gauge_outer.png");
	damagedFrameGraph = LoadGraph("resource/damaged_frame.png");
	//�X�N���[���𐶐�
	buffScreen = MakeScreen(350, 500, TRUE);

	//BGM
	bgm = SoundEmitter::Instance()->LoadSound("resource/bgm.wav", BGM_VOLUME);

	itemGetSE = SoundEmitter::Instance()->LoadSound("resource/itemGetSE.mp3", 150);
	finishSE = SoundEmitter::Instance()->LoadSound("resource/endSE.mp3", 200);

	//�v���C���[�̍��W�|�C���^�ݒ�
	EnemyManager::Instance()->playerPos = &player.pos;

	//�^�C�}�[�̍X�V
	gameTimer.Init(99);
}

void GameScene::Initialize()
{
	//�Q�[���J�n��A�o�߃t���[������J�n
	EnemyAppearManager::Instance()->Init();

	EnemyHitEffectManager::Instance()->Init();

	hexagonFloor.Init();

	BulletEffectManager::Instance()->Init();

	player.Init();

	damagedFrameFlashAngle = 0;

	BulletManager::Instance()->Init();

	gameTimer.Init(99);

	Combo::Instance()->Init();

	Score::Instance()->Init();

	EnemyManager::Instance()->Init();

	GaugeItemManager::Instance()->Init();


}

void GameScene::Finalize()
{
	SoundEmitter::Instance()->StopNowBGM();
}

void GameScene::Update()
{
	//�G�o��Ǘ��X�V
	EnemyAppearManager::Instance()->Update();

	//�C���g���I������特�y���[�v�Đ�
	if (!SoundEmitter::Instance()->NowPlayBGM()) {
		SoundEmitter::Instance()->Play(bgm, DX_PLAYTYPE_LOOP, true);
	}

	/*-----�v���C���[�̍X�V����-----*/
	if (0 < player.hp) {
		player.Update(BulletManager::Instance()->bulletGenerateTimer == 0, BulletManager::Instance()->bulletGenerateTimerDef);
	}

	/*-----�v���C���[�e�̍X�V����-----*/
	BulletManager::Instance()->Update(player.pos, player.playerAngle, player.scrollAmount, player.hitTimer <= 0 && player.slashButtonFrame <= 0, player.bulletStates);

	/*-----�G�̍X�V����-----*/
	EnemyManager::Instance()->Update();
	EnemyAppearManager::Instance()->Update();
	EnemyAppearEffectEmitter::Instance()->Update();

	/*-----�^�C�}�[�̍X�V����-----*/
	gameTimer.Update();

	/*-----�A�C�e���̍X�V����*/
	/*if (player.gauge >= PLAYER_GAUGE_MAX) {
		player.gauge = 0;
		ItemManager::Instance()->Generate(player.pos);
	}
	ItemManager::Instance()->Update(player.pos);*/

	GaugeItemManager::Instance()->Update(player.pos);

	BulletEffectManager::Instance()->Update();

	hexagonFloor.Update(player.pos, player.size);

	if (player.slashFinishTrigger) {
		EnemyManager::Instance()->ResetSlashedFlag();
	}

	/*-----�����蔻�菈��-----*/
#pragma region �����蔻��

	/*�v���C���[�̉����߂�����*/
	//�}�b�v�̒��S�ƃv���C���[�̋���
	float playerMapDistance = player.pos.Distance(Vec2(HALF_WIN_WIDTH, HALF_WIN_HEIGHT));
	//�������}�b�v�̔��a�����傫��������
	if (playerMapDistance > MAP_RADIUS) {
		//�͂ݏo�Ă��鋗�������߂�
		float dev = playerMapDistance - MAP_RADIUS;
		//�v���C���[����}�b�v�̒��S�ւ̕����x�N�g�������߂�
		Vec2 playerToMap = Vec2((WIN_WIDTH / 2.0f) - player.pos.x, (WIN_HEIGHT / 2.0f) - player.pos.y);
		playerToMap.Normalize();

		//�����߂�
		player.pos += Vec2(playerToMap.x * dev, playerToMap.y * dev);
		//�X�N���[���ʂ��X�V
		player.scrollAmount.x += playerToMap.x * dev;
		player.scrollAmount.y += playerToMap.y * dev;
	}

	/*�e�ƓG�̓����蔻��*/
	for (int bullet = 0; bullet < BULLET_MAX; ++bullet) {
		//�������Ă��Ȃ������珈�����X�L�b�v
		if (BulletManager::Instance()->bullet[bullet].isAlive == false) continue;

		for (int type = 0; type < ENEMY_TYPE_NUM; ++type) {
			for (int enemy = 0; enemy < ENEMY_MAX[type]; ++enemy) {
				//�������Ă��Ȃ������珈�����X�L�b�v
				if (EnemyManager::Instance()->enemys[type][enemy]->isAlive == false) continue;

				bool addScoreFlag = false;    //�X�R�A���Z�t���O
				//�����蔻����s��
				if (EnemyManager::Instance()->enemys[type][enemy]->HitCheck(BulletManager::Instance()->bullet[bullet].pos, BULLET_SIZE, false, &addScoreFlag)) {
					//�e������
					BulletManager::Instance()->bullet[bullet].Hit();
					//addScoreFlag�͎E�����Ƃ���true�ɂȂ�̂ŁA�G���E���Ă�����
					if (addScoreFlag) {
						Score::Instance()->Add(BULLET_KILL_SCORE, 1);
						//�R���{���X�V
						Combo::Instance()->Hit(BULLET_KILL_SCORE);
					}
					else {
						Score::Instance()->Add(BULLET_HIT_SCORE, 1);
						//�R���{���X�V
						Combo::Instance()->Hit(BULLET_HIT_SCORE);
					}

					//�E�����G��infinityEnemy���������ʂ̓G�𐶐�����
					if (type == NORMAL_ENEMT_INFINITY && EnemyManager::Instance()->enemys[type][enemy]->hp <= 0) {
						//����Ő��������
						float roundCount = 20.0f;
						//3��
						int hp = 4;
						for (int line = 0; line < 2; ++line) {
							//������̓G
							for (int count = 0; count < roundCount; ++count) {
								Vec2 generatePos = Vec2(cosf(DX_TWO_PI_F * ((float)count / roundCount)) * (MAP_RADIUS * 0.75f + (line * 50)) + HALF_WIN_WIDTH,
									sinf(DX_TWO_PI_F * (count / roundCount)) * (MAP_RADIUS * 0.75f + (line * 50)) + HALF_WIN_HEIGHT);
								float generateAngle = atan2f(HALF_WIN_HEIGHT - generatePos.y, HALF_WIN_WIDTH - generatePos.x);
								//����
								EnemyManager::Instance()->GenerateEnemy(NORMAL_ENEMY_STRAIGHT, generatePos, &hp, &generateAngle);
								//�����G�t�F�N�g������
								EnemyAppearEffectEmitter::Instance()->Generate(generatePos);
							}
						}
					}
				}
			}
		}
	}

	/*�a���ƓG�̓����蔻��*/
	if (player.isSlash) {
		for (int type = 0; type < ENEMY_TYPE_NUM; ++type) {
			for (int enemy = 0; enemy < ENEMY_MAX[type]; ++enemy) {
				//�������Ă��Ȃ������珈�����X�L�b�v
				if (EnemyManager::Instance()->enemys[type][enemy]->isAlive == false) continue;

				//�����蔻����s��
				int deathCount = 0;
				int hitCount = 0;
				if (EnemyManager::Instance()->enemys[type][enemy]->HitCheckSlash(player.pos, player.playerAngle, deathCount, hitCount)) {

					//�X�R�A�����Z����
					Score::Instance()->Add(SLASH_KILL_SCORE, deathCount);
					Score::Instance()->Add(SLASH_HIT_SCORE, hitCount);
					//�R���{���X�V
					if (deathCount > 0) Combo::Instance()->Hit(SLASH_KILL_SCORE);
					if (hitCount > 0) Combo::Instance()->Hit(SLASH_HIT_SCORE);
					//�a�����j�������Z����
					//player.slashDeathCount += deathCount;



					//�E�����G��infinityEnemy���������ʂ̓G�𐶐�����
					if (type == NORMAL_ENEMT_INFINITY && EnemyManager::Instance()->enemys[type][enemy]->hp <= 0) {
						//����Ő��������
						float roundCount = 20.0f;
						//3��
						for (int line = 0; line < 2; ++line) {
							//������̓G
							int hp = 4;
							for (int count = 0; count < roundCount; ++count) {
								Vec2 generatePos = Vec2(cosf(DX_TWO_PI_F * ((float)count / roundCount)) * (MAP_RADIUS * 0.75f + (line * 50)) + HALF_WIN_WIDTH,
									sinf(DX_TWO_PI_F * (count / roundCount)) * (MAP_RADIUS * 0.75f + (line * 50)) + HALF_WIN_HEIGHT);
								float generateAngle = atan2f(HALF_WIN_HEIGHT - generatePos.y, HALF_WIN_WIDTH - generatePos.x);
								//����
								EnemyManager::Instance()->GenerateEnemy(NORMAL_ENEMY_STRAIGHT, generatePos, &hp, &generateAngle);
								//�����G�t�F�N�g������
								EnemyAppearEffectEmitter::Instance()->Generate(generatePos);
							}
						}
					}
				}
			}
		}
	}

	/*�v���C���[�ƓG�̓����蔻��*/
	//�v���C���[�����G���Ԓ��������瓖���蔻����s��Ȃ�
	if (player.hitTimer <= 0 && player.slashInvincibleTime <= 0) {
		for (int type = 0; type < ENEMY_TYPE_NUM; ++type) {
			for (int enemy = 0; enemy < ENEMY_MAX[type]; ++enemy) {
				//�������Ă��Ȃ������珈�����X�L�b�v
				if (EnemyManager::Instance()->enemys[type][enemy]->isAlive == false) continue;

				//�����蔻����s��
				if (EnemyManager::Instance()->enemys[type][enemy]->HitCheck(player.pos, player.size / 2.0f, true)) {
					//�v���C���[�𑀍�s�\&�����蔻��Ȃ���Ԃ�
					player.Hit();
					//�v���C���[�̃V�F�C�N�ʂ��X�V
					playerShake = PLAYER_SHAKE_DEF;
					//�v���C���[�̎a���n��������
					player.isSlash = false;
					player.isSlashAnime = false;
					player.slashInvincibleTime = 0;

					//�E�����G��infinityEnemy���������ʂ̓G�𐶐�����
					if (type == NORMAL_ENEMT_INFINITY) {
						//����Ő��������
						float roundCount = 20.0f;
						//3��
						for (int line = 0; line < 2; ++line) {
							//������̓G
							for (int count = 0; count < roundCount; ++count) {
								Vec2 generatePos = Vec2(cosf(DX_TWO_PI_F * ((float)count / roundCount)) * (MAP_RADIUS * 0.75f + (line * 50)) + HALF_WIN_WIDTH,
									sinf(DX_TWO_PI_F * (count / roundCount)) * (MAP_RADIUS * 0.75f + (line * 50)) + HALF_WIN_HEIGHT);
								float generateAngle = atan2f(HALF_WIN_HEIGHT - generatePos.y, HALF_WIN_WIDTH - generatePos.x);
								int hp = 4;
								//����
								EnemyManager::Instance()->GenerateEnemy(NORMAL_ENEMY_STRAIGHT, generatePos, &hp, &generateAngle);
								//�����G�t�F�N�g������
								EnemyAppearEffectEmitter::Instance()->Generate(generatePos);
							}
						}
					}
				}
			}
		}
	}

	/*�v���C���[�ƃA�C�e���̓����蔻��*/
	for (int i = 0; i < ITEM_MAX_COUNT; ++i) {
		if (GaugeItemManager::Instance()->items[i].isAlive == false || GaugeItemManager::Instance()->items[i].isCollision == false) continue;

		//�����蔻����s��
		float distance = player.pos.Distance(GaugeItemManager::Instance()->items[i].pos);
		if (distance <= player.size / 2.0f + GaugeItemManager::Instance()->items[i].radius) {
			//�q�b�g�����A�C�e�����q�b�g�������[�h�ɂ���
			GaugeItemManager::Instance()->items[i].states = ITEM_STATES_OBTAINING;
			//�v���C���[�̃Q�[�W���X�V
			//player.gauge += 5;

			SoundEmitter::Instance()->Play(itemGetSE, DX_PLAYTYPE_BACK);
			player.EmitItemGetEffect();
			if (PLAYER_HP_WARING < player.hp) {
				damagedFrameFlashAngle = 0.0f;
			}
		}
	}

#pragma endregion

	/* ---- - ���[�^�[�̐j�̊p�x�����߂�---- - */
		//�v���C���[�̌��ݑ��x��0~1�͈̔͂ɂ���
	/*float playerSpeedPer = player.speed / PLAYER_MAX_SPEED;*/
	float playerSpeedPer = player.move.Length() / (PLAYER_MAX_SPEED * 0.98);
	//�v���C���[���q�b�g�����Ƃ��͈ړ����x��0�S���Ă���̂ŁA�j�̈ʒu��0�ɂ���
	//if (player.hitTimer >= PLAYER_HIT_TIMER / 2.0f) {
	//	playerSpeedPer = 0;
	//}
	//���[�^�[���ł����̎��̊p�x��-0.922,�ł��E�̎��̊p�x��0.907�Ȃ̂ŁA��U�l�͈̔͂�0~1.829�ɂ���
	playerSpeedPer = playerSpeedPer * 1.829f;
	//-1.022�������Ēl�͈̔͂𐳂����ݒ肷��
	playerSpeedPer -= 0.907f;
	meterIndicaterAngle = playerSpeedPer;

	/*-----�X�R�A�̕`��p�̃f�[�^���X�V-----*/
	//addScoreBuff�ɒl�������Ă���score�ɑ���
	Score::Instance()->Update();

	/*---�G�l�~�[�ɒe�����������Ƃ��̃G�t�F�N�g---*/
	EnemyHitEffectManager::Instance()->Update();


	//�v���C���[�̃V�F�C�N�ʂ��X�V
	if (playerShake > 0) {
		playerShake -= 0.5f;
	}

	//�^�C�g���V�[����
	if (gameTimer.EndTimer() || player.hp <= 0) {
		int combo = Combo::Instance()->GetMaxConbo();
		Score::Instance()->GameSceneEnd();
		sceneChanger->ChangeScene(RESULT_SCENE, &gameFinishTrans);
		SoundEmitter::Instance()->Play(finishSE, DX_PLAYTYPE_BACK);
		playerShake = 0;
	}
}


void GameScene::Draw()
{
	/*---------------�`�揈��---------------*/
	//�w�i�̕`�揈��
	hexagonFloor.Draw(player.scrollAmount);
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	//for (int width = 0; width < BACKGROUND_TILE_COUNT_X; ++width) {
	//	//���̐��������������炽�Ă��������炷
	//	float zure = 0;
	//	if (width % 2 == 0) {
	//		zure = BACKGROUND_TILE_SIZE.y / 2.0f;
	//	}
	//	for (int height = 0; height < BACKGROUND_TILE_COUNT_Y; ++height) {
	//		//�`�悷��
	//		float drawX = BACKGROUND_LEFTUP_POS.x + (width * (BACKGROUND_TILE_SIZE.x - 35.0f)) - player.scrollAmount.x;
	//		float drawY = BACKGROUND_LEFTUP_POS.y + (height * BACKGROUND_TILE_SIZE.y) + zure - player.scrollAmount.y;
	//		DrawGraph(drawX, drawY, backGroundGraph, true);
	//	}
	//}
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//�}�b�v�̕`�揈��
	DrawCircle(HALF_WIN_WIDTH - player.scrollAmount.x, HALF_WIN_HEIGHT - player.scrollAmount.y, MAP_RADIUS, GetColor(0x78, 0xFF, 0xA9), FALSE, 5);

	//�G�̕`�揈��
	EnemyAppearEffectEmitter::Instance()->Draw(player.scrollAmount);
	EnemyManager::Instance()->Draw(player.scrollAmount);
	BulletManager::Instance()->Draw(player.scrollAmount);

	//�G�Ƀq�b�g�����Ƃ��̃G�t�F�N�g
	EnemyHitEffectManager::Instance()->Draw(player.scrollAmount);

	BulletEffectManager::Instance()->Draw(player.scrollAmount);

	//�A�C�e���̕`�揈��
	//ItemManager::Instance()->Draw(player.scrollAmount);
	GaugeItemManager::Instance()->Draw(player.scrollAmount);

	//�v���C���[�̕`�揈��
	player.Draw();

	//�X�N���[���t���[���摜�̕`�揈��
	//DrawExtendGraph(0 - 50, 0 - 50, WIN_WIDTH + 50, WIN_HEIGHT + 50, screenFrameGraph, TRUE);
	DrawGraph(-50 + (GetRand(playerShake) - playerShake) * 2, -50 + (GetRand(playerShake) - playerShake / 2.0f) * 2, screenFrameGraph, TRUE);
	DrawGraph(WIN_WIDTH - 330, WIN_HEIGHT - 180, meterGraph, TRUE);
	//���[�^�[�̐j�̕`��
	if (player.move.Length() / PLAYER_MAX_SPEED < 0.95f) {
		DrawRotaGraph(WIN_WIDTH - 165, WIN_HEIGHT - 30, 1, meterIndicaterAngle, meterIndicaterGraph, TRUE);
	}
	else {
		DrawRotaGraph(WIN_WIDTH - 165, WIN_HEIGHT - 30, 1, meterIndicaterAngle + ((GetRand(100) / 1000.0f) - 0.05f), meterIndicaterGraph, TRUE);
	}

	//�L�����N�^�[��\��
	if (player.hitTimer > PLAYER_HIT_TIMER / 2.0f) {
		//�ڂ₯������
		SetDrawScreen(buffScreen);
		ClearDrawScreen();
		DrawGraph(0, 0, charactorDamageGraph, TRUE);
		SetDrawScreen(DX_SCREEN_BACK);

		//������ƐԂ�����
		SetDrawBright(0xFF, 0xC0, 0xC0);
		GraphFilter(buffScreen, DX_GRAPH_FILTER_GAUSS, playerShake / 10.0f, playerShake / 10.0f);
		DrawGraph(WIN_WIDTH - 350 + (GetRand(playerShake) - playerShake / 2.0f), 12 + (GetRand(playerShake) - playerShake / 2.0f), buffScreen, TRUE);
		SetDrawBright(0xFF, 0xFF, 0xFF);
	}
	else {
		if (GetRand(100) % 99 == 0) {
			if (PLAYER_HP / 2 < player.hp) {
				DrawGraph(WIN_WIDTH - 350, 12, charactorCloseGraph, TRUE);
			}
			else {
				DrawGraph(WIN_WIDTH - 350, 12, charactorDamageGraph_close, TRUE);
			}
		}
		else {
			if (PLAYER_HP / 2 < player.hp) {
				DrawGraph(WIN_WIDTH - 350, 12, charactorGraph, TRUE);
			}
			else {
				DrawGraph(WIN_WIDTH - 350, 12, charactorDamageGraph_default, TRUE);
			}
		}
	}

	//�v���C���[�̃Q�[�W���X�V
	DrawGraph(WIN_WIDTH - 295, WIN_HEIGHT / 2.0f + 110, gaugeOuterGraph, TRUE);
	/*DrawExtendGraph(WIN_WIDTH - 290,
		WIN_HEIGHT / 2.0f + 110,
		(WIN_WIDTH - 290) + (PLAYER_GAUGE_SIZE.x * (player.gauge / PLAYER_GAUGE_MAX)),
		(WIN_HEIGHT / 2.0f + 110) + PLAYER_GAUGE_SIZE.y,
		gaugeInnerGraph, TRUE);*/
	/*if (0 < player.hitTimer) {
		DrawExtendGraph(WIN_WIDTH - 285.5,
			WIN_HEIGHT / 2.0f + 110,
			(WIN_WIDTH - 290) + (PLAYER_GAUGE_SIZE.x * ((float)(player.hp + PLAYER_HP_DAMAGE) / (float)PLAYER_HP)),
			(WIN_HEIGHT / 2.0f + 110) + PLAYER_GAUGE_SIZE.y,
			gaugeInnerGraph_damaged, TRUE);
	}*/
	if (0 < player.hp) {
		DrawExtendGraph(WIN_WIDTH - 285.5,
			WIN_HEIGHT / 2.0f + 110,
			(WIN_WIDTH - 290) + (PLAYER_GAUGE_SIZE.x * ((float)player.hp / (float)PLAYER_HP)),
			(WIN_HEIGHT / 2.0f + 110) + PLAYER_GAUGE_SIZE.y,
			gaugeInnerGraph, TRUE);
	}

	//�X�R�A��`��
	Score::Instance()->Draw();

	//�R���{��`��
	Combo::Instance()->Draw();

	//�^�C�}�[�̕`��


	//�_���[�W�t���[��
	if (player.hp <= PLAYER_HP_WARING) {
		damagedFrameFlashAngle += 5;
		if (180 < damagedFrameFlashAngle) {
			damagedFrameFlashAngle -= 180;
		}
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, sin(PI / 180 * damagedFrameFlashAngle) * (150 - 80) + 80);
		DrawGraph(0, 0, damagedFrameGraph, true);
	}
	else if (0 < player.hitTimer) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, Easing::CircOut(PLAYER_HIT_TIMER - player.hitTimer, PLAYER_HIT_TIMER, 255, 0));
		DrawGraph(0, 0, damagedFrameGraph, true);
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	gameTimer.Draw();

	//DrawFormatString(0, 0, GetColor(255, 255, 255), "damagedFrameFlashAngle %d", damagedFrameFlashAngle);
	Combo::Instance()->Draw();
}
