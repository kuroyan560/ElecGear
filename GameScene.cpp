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
	//各画像をロード
	screenFrameGraph = LoadGraph("resource/screen_frame.png");
	meterGraph = LoadGraph("resource/meter.png");
	meterIndicaterGraph = LoadGraph("resource/meter_indicater.png");
	LoadDivGraph("resource/character_card_default.png", 7, 7, 1, 350, 500, playerDevGraph);		//プレイヤーの連番
	charactorGraph = playerDevGraph[0];
	charactorCloseGraph = playerDevGraph[3];
	charactorDamageGraph = LoadGraph("resource/character_card_damage.png");
	charactorDamageGraph_default = LoadGraph("resource/character_card_damage_default.png");
	charactorDamageGraph_close = LoadGraph("resource/character_card_damage_default_close.png");
	gaugeInnerGraph = LoadGraph("resource/gauge_inner.png");
	gaugeInnerGraph_damaged = LoadGraph("resource/gauge_inner_damaged.png");
	gaugeOuterGraph = LoadGraph("resource/gauge_outer.png");
	damagedFrameGraph = LoadGraph("resource/damaged_frame.png");
	//スクリーンを生成
	buffScreen = MakeScreen(350, 500, TRUE);

	//BGM
	bgm = SoundEmitter::Instance()->LoadSound("resource/bgm.wav", BGM_VOLUME);

	itemGetSE = SoundEmitter::Instance()->LoadSound("resource/itemGetSE.mp3", 150);
	finishSE = SoundEmitter::Instance()->LoadSound("resource/endSE.mp3", 200);

	//プレイヤーの座標ポインタ設定
	EnemyManager::Instance()->playerPos = &player.pos;

	//タイマーの更新
	gameTimer.Init(99);
}

void GameScene::Initialize()
{
	//ゲーム開始後、経過フレーム測定開始
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
	//敵登場管理更新
	EnemyAppearManager::Instance()->Update();

	//イントロ終わったら音楽ループ再生
	if (!SoundEmitter::Instance()->NowPlayBGM()) {
		SoundEmitter::Instance()->Play(bgm, DX_PLAYTYPE_LOOP, true);
	}

	/*-----プレイヤーの更新処理-----*/
	if (0 < player.hp) {
		player.Update(BulletManager::Instance()->bulletGenerateTimer == 0, BulletManager::Instance()->bulletGenerateTimerDef);
	}

	/*-----プレイヤー弾の更新処理-----*/
	BulletManager::Instance()->Update(player.pos, player.playerAngle, player.scrollAmount, player.hitTimer <= 0 && player.slashButtonFrame <= 0, player.bulletStates);

	/*-----敵の更新処理-----*/
	EnemyManager::Instance()->Update();
	EnemyAppearManager::Instance()->Update();
	EnemyAppearEffectEmitter::Instance()->Update();

	/*-----タイマーの更新処理-----*/
	gameTimer.Update();

	/*-----アイテムの更新処理*/
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

	/*-----当たり判定処理-----*/
#pragma region 当たり判定

	/*プレイヤーの押し戻し処理*/
	//マップの中心とプレイヤーの距離
	float playerMapDistance = player.pos.Distance(Vec2(HALF_WIN_WIDTH, HALF_WIN_HEIGHT));
	//距離がマップの半径よりも大きかったら
	if (playerMapDistance > MAP_RADIUS) {
		//はみ出ている距離を求める
		float dev = playerMapDistance - MAP_RADIUS;
		//プレイヤーからマップの中心への方向ベクトルを求める
		Vec2 playerToMap = Vec2((WIN_WIDTH / 2.0f) - player.pos.x, (WIN_HEIGHT / 2.0f) - player.pos.y);
		playerToMap.Normalize();

		//押し戻す
		player.pos += Vec2(playerToMap.x * dev, playerToMap.y * dev);
		//スクロール量を更新
		player.scrollAmount.x += playerToMap.x * dev;
		player.scrollAmount.y += playerToMap.y * dev;
	}

	/*弾と敵の当たり判定*/
	for (int bullet = 0; bullet < BULLET_MAX; ++bullet) {
		//生存していなかったら処理をスキップ
		if (BulletManager::Instance()->bullet[bullet].isAlive == false) continue;

		for (int type = 0; type < ENEMY_TYPE_NUM; ++type) {
			for (int enemy = 0; enemy < ENEMY_MAX[type]; ++enemy) {
				//生存していなかったら処理をスキップ
				if (EnemyManager::Instance()->enemys[type][enemy]->isAlive == false) continue;

				bool addScoreFlag = false;    //スコア加算フラグ
				//当たり判定を行う
				if (EnemyManager::Instance()->enemys[type][enemy]->HitCheck(BulletManager::Instance()->bullet[bullet].pos, BULLET_SIZE, false, &addScoreFlag)) {
					//弾を消す
					BulletManager::Instance()->bullet[bullet].Hit();
					//addScoreFlagは殺したときにtrueになるので、敵を殺していたら
					if (addScoreFlag) {
						Score::Instance()->Add(BULLET_KILL_SCORE, 1);
						//コンボを更新
						Combo::Instance()->Hit(BULLET_KILL_SCORE);
					}
					else {
						Score::Instance()->Add(BULLET_HIT_SCORE, 1);
						//コンボを更新
						Combo::Instance()->Hit(BULLET_HIT_SCORE);
					}

					//殺した敵がinfinityEnemyだったら大量の敵を生成する
					if (type == NORMAL_ENEMT_INFINITY && EnemyManager::Instance()->enemys[type][enemy]->hp <= 0) {
						//一周で生成する量
						float roundCount = 20.0f;
						//3列
						int hp = 4;
						for (int line = 0; line < 2; ++line) {
							//一周分の敵
							for (int count = 0; count < roundCount; ++count) {
								Vec2 generatePos = Vec2(cosf(DX_TWO_PI_F * ((float)count / roundCount)) * (MAP_RADIUS * 0.75f + (line * 50)) + HALF_WIN_WIDTH,
									sinf(DX_TWO_PI_F * (count / roundCount)) * (MAP_RADIUS * 0.75f + (line * 50)) + HALF_WIN_HEIGHT);
								float generateAngle = atan2f(HALF_WIN_HEIGHT - generatePos.y, HALF_WIN_WIDTH - generatePos.x);
								//生成
								EnemyManager::Instance()->GenerateEnemy(NORMAL_ENEMY_STRAIGHT, generatePos, &hp, &generateAngle);
								//生成エフェクトも生成
								EnemyAppearEffectEmitter::Instance()->Generate(generatePos);
							}
						}
					}
				}
			}
		}
	}

	/*斬撃と敵の当たり判定*/
	if (player.isSlash) {
		for (int type = 0; type < ENEMY_TYPE_NUM; ++type) {
			for (int enemy = 0; enemy < ENEMY_MAX[type]; ++enemy) {
				//生存していなかったら処理をスキップ
				if (EnemyManager::Instance()->enemys[type][enemy]->isAlive == false) continue;

				//当たり判定を行う
				int deathCount = 0;
				int hitCount = 0;
				if (EnemyManager::Instance()->enemys[type][enemy]->HitCheckSlash(player.pos, player.playerAngle, deathCount, hitCount)) {

					//スコアを加算する
					Score::Instance()->Add(SLASH_KILL_SCORE, deathCount);
					Score::Instance()->Add(SLASH_HIT_SCORE, hitCount);
					//コンボを更新
					if (deathCount > 0) Combo::Instance()->Hit(SLASH_KILL_SCORE);
					if (hitCount > 0) Combo::Instance()->Hit(SLASH_HIT_SCORE);
					//斬撃撃破数を加算する
					//player.slashDeathCount += deathCount;



					//殺した敵がinfinityEnemyだったら大量の敵を生成する
					if (type == NORMAL_ENEMT_INFINITY && EnemyManager::Instance()->enemys[type][enemy]->hp <= 0) {
						//一周で生成する量
						float roundCount = 20.0f;
						//3列
						for (int line = 0; line < 2; ++line) {
							//一周分の敵
							int hp = 4;
							for (int count = 0; count < roundCount; ++count) {
								Vec2 generatePos = Vec2(cosf(DX_TWO_PI_F * ((float)count / roundCount)) * (MAP_RADIUS * 0.75f + (line * 50)) + HALF_WIN_WIDTH,
									sinf(DX_TWO_PI_F * (count / roundCount)) * (MAP_RADIUS * 0.75f + (line * 50)) + HALF_WIN_HEIGHT);
								float generateAngle = atan2f(HALF_WIN_HEIGHT - generatePos.y, HALF_WIN_WIDTH - generatePos.x);
								//生成
								EnemyManager::Instance()->GenerateEnemy(NORMAL_ENEMY_STRAIGHT, generatePos, &hp, &generateAngle);
								//生成エフェクトも生成
								EnemyAppearEffectEmitter::Instance()->Generate(generatePos);
							}
						}
					}
				}
			}
		}
	}

	/*プレイヤーと敵の当たり判定*/
	//プレイヤーが無敵時間中だったら当たり判定を行わない
	if (player.hitTimer <= 0 && player.slashInvincibleTime <= 0) {
		for (int type = 0; type < ENEMY_TYPE_NUM; ++type) {
			for (int enemy = 0; enemy < ENEMY_MAX[type]; ++enemy) {
				//生存していなかったら処理をスキップ
				if (EnemyManager::Instance()->enemys[type][enemy]->isAlive == false) continue;

				//当たり判定を行う
				if (EnemyManager::Instance()->enemys[type][enemy]->HitCheck(player.pos, player.size / 2.0f, true)) {
					//プレイヤーを操作不能&当たり判定なし状態へ
					player.Hit();
					//プレイヤーのシェイク量を更新
					playerShake = PLAYER_SHAKE_DEF;
					//プレイヤーの斬撃系を初期化
					player.isSlash = false;
					player.isSlashAnime = false;
					player.slashInvincibleTime = 0;

					//殺した敵がinfinityEnemyだったら大量の敵を生成する
					if (type == NORMAL_ENEMT_INFINITY) {
						//一周で生成する量
						float roundCount = 20.0f;
						//3列
						for (int line = 0; line < 2; ++line) {
							//一周分の敵
							for (int count = 0; count < roundCount; ++count) {
								Vec2 generatePos = Vec2(cosf(DX_TWO_PI_F * ((float)count / roundCount)) * (MAP_RADIUS * 0.75f + (line * 50)) + HALF_WIN_WIDTH,
									sinf(DX_TWO_PI_F * (count / roundCount)) * (MAP_RADIUS * 0.75f + (line * 50)) + HALF_WIN_HEIGHT);
								float generateAngle = atan2f(HALF_WIN_HEIGHT - generatePos.y, HALF_WIN_WIDTH - generatePos.x);
								int hp = 4;
								//生成
								EnemyManager::Instance()->GenerateEnemy(NORMAL_ENEMY_STRAIGHT, generatePos, &hp, &generateAngle);
								//生成エフェクトも生成
								EnemyAppearEffectEmitter::Instance()->Generate(generatePos);
							}
						}
					}
				}
			}
		}
	}

	/*プレイヤーとアイテムの当たり判定*/
	for (int i = 0; i < ITEM_MAX_COUNT; ++i) {
		if (GaugeItemManager::Instance()->items[i].isAlive == false || GaugeItemManager::Instance()->items[i].isCollision == false) continue;

		//当たり判定を行う
		float distance = player.pos.Distance(GaugeItemManager::Instance()->items[i].pos);
		if (distance <= player.size / 2.0f + GaugeItemManager::Instance()->items[i].radius) {
			//ヒットしたアイテムをヒットしたモードにする
			GaugeItemManager::Instance()->items[i].states = ITEM_STATES_OBTAINING;
			//プレイヤーのゲージを更新
			//player.gauge += 5;

			SoundEmitter::Instance()->Play(itemGetSE, DX_PLAYTYPE_BACK);
			player.EmitItemGetEffect();
			if (PLAYER_HP_WARING < player.hp) {
				damagedFrameFlashAngle = 0.0f;
			}
		}
	}

#pragma endregion

	/* ---- - メーターの針の角度を求める---- - */
		//プレイヤーの現在速度を0~1の範囲にする
	/*float playerSpeedPer = player.speed / PLAYER_MAX_SPEED;*/
	float playerSpeedPer = player.move.Length() / (PLAYER_MAX_SPEED * 0.98);
	//プレイヤーがヒットしたときは移動速度が0担っているので、針の位置も0にする
	//if (player.hitTimer >= PLAYER_HIT_TIMER / 2.0f) {
	//	playerSpeedPer = 0;
	//}
	//メーターが最も左の時の角度が-0.922,最も右の時の角度が0.907なので、一旦値の範囲を0~1.829にする
	playerSpeedPer = playerSpeedPer * 1.829f;
	//-1.022を引いて値の範囲を正しく設定する
	playerSpeedPer -= 0.907f;
	meterIndicaterAngle = playerSpeedPer;

	/*-----スコアの描画用のデータを更新-----*/
	//addScoreBuffに値が入ってたらscoreに足す
	Score::Instance()->Update();

	/*---エネミーに弾が当たったときのエフェクト---*/
	EnemyHitEffectManager::Instance()->Update();


	//プレイヤーのシェイク量を更新
	if (playerShake > 0) {
		playerShake -= 0.5f;
	}

	//タイトルシーンへ
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
	/*---------------描画処理---------------*/
	//背景の描画処理
	hexagonFloor.Draw(player.scrollAmount);
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	//for (int width = 0; width < BACKGROUND_TILE_COUNT_X; ++width) {
	//	//横の数が偶数だったらたてを少しずらす
	//	float zure = 0;
	//	if (width % 2 == 0) {
	//		zure = BACKGROUND_TILE_SIZE.y / 2.0f;
	//	}
	//	for (int height = 0; height < BACKGROUND_TILE_COUNT_Y; ++height) {
	//		//描画する
	//		float drawX = BACKGROUND_LEFTUP_POS.x + (width * (BACKGROUND_TILE_SIZE.x - 35.0f)) - player.scrollAmount.x;
	//		float drawY = BACKGROUND_LEFTUP_POS.y + (height * BACKGROUND_TILE_SIZE.y) + zure - player.scrollAmount.y;
	//		DrawGraph(drawX, drawY, backGroundGraph, true);
	//	}
	//}
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//マップの描画処理
	DrawCircle(HALF_WIN_WIDTH - player.scrollAmount.x, HALF_WIN_HEIGHT - player.scrollAmount.y, MAP_RADIUS, GetColor(0x78, 0xFF, 0xA9), FALSE, 5);

	//敵の描画処理
	EnemyAppearEffectEmitter::Instance()->Draw(player.scrollAmount);
	EnemyManager::Instance()->Draw(player.scrollAmount);
	BulletManager::Instance()->Draw(player.scrollAmount);

	//敵にヒットしたときのエフェクト
	EnemyHitEffectManager::Instance()->Draw(player.scrollAmount);

	BulletEffectManager::Instance()->Draw(player.scrollAmount);

	//アイテムの描画処理
	//ItemManager::Instance()->Draw(player.scrollAmount);
	GaugeItemManager::Instance()->Draw(player.scrollAmount);

	//プレイヤーの描画処理
	player.Draw();

	//スクリーンフレーム画像の描画処理
	//DrawExtendGraph(0 - 50, 0 - 50, WIN_WIDTH + 50, WIN_HEIGHT + 50, screenFrameGraph, TRUE);
	DrawGraph(-50 + (GetRand(playerShake) - playerShake) * 2, -50 + (GetRand(playerShake) - playerShake / 2.0f) * 2, screenFrameGraph, TRUE);
	DrawGraph(WIN_WIDTH - 330, WIN_HEIGHT - 180, meterGraph, TRUE);
	//メーターの針の描画
	if (player.move.Length() / PLAYER_MAX_SPEED < 0.95f) {
		DrawRotaGraph(WIN_WIDTH - 165, WIN_HEIGHT - 30, 1, meterIndicaterAngle, meterIndicaterGraph, TRUE);
	}
	else {
		DrawRotaGraph(WIN_WIDTH - 165, WIN_HEIGHT - 30, 1, meterIndicaterAngle + ((GetRand(100) / 1000.0f) - 0.05f), meterIndicaterGraph, TRUE);
	}

	//キャラクターを表示
	if (player.hitTimer > PLAYER_HIT_TIMER / 2.0f) {
		//ぼやけさせる
		SetDrawScreen(buffScreen);
		ClearDrawScreen();
		DrawGraph(0, 0, charactorDamageGraph, TRUE);
		SetDrawScreen(DX_SCREEN_BACK);

		//ちょっと赤くする
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

	//プレイヤーのゲージを更新
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

	//スコアを描画
	Score::Instance()->Draw();

	//コンボを描画
	Combo::Instance()->Draw();

	//タイマーの描画


	//ダメージフレーム
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
