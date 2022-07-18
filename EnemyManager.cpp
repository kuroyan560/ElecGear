#include "EnemyManager.h"
#include"Enemy.h"
#include"CircleEnemy.h"
#include"SoundEmitter.h"

EnemyManager::EnemyManager()
{
	//画像読み込み
	enemyCircleOuter = LoadGraph("resource/enemy_circle_outer.png");
	enemyCircleInner = LoadGraph("resource/enemy_circle_inner.png");
	enemyDirection = LoadGraph("resource/enemy_direction.png");
	warningGraph = LoadGraph("resource/warning.png");

	enemyDamagedSE = SoundEmitter::Instance()->LoadSound("resource/enemyDamagedSE.mp3", 150);
	enemyDeadSE = SoundEmitter::Instance()->LoadSound("resource/enemyDeadSE.mp3", 150);

	//各タイマーを初期化
	//generateNormalTimer = 0;
	//generateCircleTimer = 0;
	unitTimer = 0;

	//各ステータスを初期化
	states = ENEMY_STATES_NORMAL;
	unitStates = 0;
	unitWallDirection = 0;

	//敵の種類分の配列生成
	enemys.resize(ENEMY_TYPE_NUM);
	//敵の種類に応じて生成するクラスとその数が変わる
	//通常の追尾敵
	for (int i = 0; i < ENEMY_MAX[NORMAL_ENEMY_TRACKING]; ++i)
	{
		enemys[NORMAL_ENEMY_TRACKING].emplace_back(new TrackingEnemy);
	}
	//通常の直進敵
	for (int i = 0; i < ENEMY_MAX[NORMAL_ENEMY_STRAIGHT]; ++i)
	{
		enemys[NORMAL_ENEMY_STRAIGHT].emplace_back(new StraightEnemy);
	}
	//通常の指定されたところまで移動敵
	for (int i = 0; i < ENEMY_MAX[NORMAL_ENEMY_DESTINATION]; ++i)
	{
		enemys[NORMAL_ENEMY_DESTINATION].emplace_back(new DestinationEnemy);
	}
	//無限の形に移動する敵
	for (int i = 0; i < ENEMY_MAX[NORMAL_ENEMT_INFINITY]; ++i)
	{
		enemys[NORMAL_ENEMT_INFINITY].emplace_back(new InfinityEnemy);
	}
	//生成した場所で待機敵
	for (int i = 0; i < ENEMY_MAX[NORMAL_ENEMY_STANDBY]; ++i)
	{
		enemys[NORMAL_ENEMY_STANDBY].emplace_back(new StandByEnemy);
	}
	//ハイスコア敵
	for (int i = 0; i < ENEMY_MAX[NORMAL_ENEMY_HIGHSCORE]; ++i)
	{
		enemys[NORMAL_ENEMY_HIGHSCORE].emplace_back(new HighScoreEnemy);
	}
	//追尾蛇敵
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

	//現在のステータスに応じて処理を変える
	switch (states) {
	case ENEMY_STATES_NORMAL:

		//通常状態での更新処理
		NormalStatesEnemyUpdate();
		//通常時にタイマーを更新
		//unitTimer++;

		break;

	case ENEMY_STATES_UNIT:

		//UNITステータスでの更新処理を行う
		UnitStatesUpdate();

		break;
	}

	//ローカル変数 壁を生成する位置
	int direction = 0;
	int directionBuff = 0;

	//タイマーが一定値に達したら遷移
	if (unitTimer >= ENEMY_UNIT_TIMER || Input::isKeyTrigger(KEY_INPUT_0)) {
		unitTimer = 0;
		//現在生存している敵を脱出状態にする
		EscapeNormalEnemy();
		//ステータスを更新
		states = ENEMY_STATES_UNIT;
		//ランダムで決める
		if (GetRand(2) == 0) {
			unitStates = ENEMY_UNIT_STATES_ROUND;
		}
		else {
			unitStates = ENEMY_UNIT_STATES_WALL;
		}

		//unitStatesに応じて生成処理を行う
		switch (unitStates) {
		case ENEMY_UNIT_STATES_WALL:

			//生成する
			for (int count = 0; count < 2; ++count) {
				for (int i = 0; i < UNIT_ENEMYS_COUNT; ++i) {
					if (unitEnemys[i].isAlive == true) continue;

					//初回ループ時はそのまま壁を出す位置を計算する
					if (count == 0) {
						direction = GetRand(3);
					}
					else {
						//すでに生成してあるもの意外になるまでループ
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

			//生成する
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

	//隊列敵を描画
	for (int i = 0; i < UNIT_ENEMYS_COUNT; ++i) {
		if (unitEnemys[i].isAlive == false) continue;
		unitEnemys[i].Draw(scrollAmount, enemyCircleOuter, enemyCircleInner, enemyDirection, warningGraph, *playerPos);
	}
}

void EnemyManager::NormalStatesEnemyUpdate()
{
	/*-----敵を生成の更新処理-----*/
	//generateNormalTimer++;
	if (generateNormalTimer >= ENEMY_GENERATE_TIMER) {
		generateNormalTimer = 0;

		int angle = GetRand(359);
		Vec2 pos;
		pos.x = ENEMY_GENERATE_RADIUS * cos(angle * PI);
		pos.y = ENEMY_GENERATE_RADIUS * sin(angle * PI);

		//敵を生成する
		GenerateEnemy(NORMAL_ENEMY_TRACKING, pos + MAP_CENTER);
	}

	//generateCircleTimer++;
	if (generateCircleTimer >= CIRCLE_ENEMY_GENERATE_TIMER) {
		generateCircleTimer = 0;
		//生成する
		GenerateEnemy(CIRCLE_ENEMY_TRACKING, Vec2(GetRand(WIN_WIDTH), GetRand(WIN_HEIGHT)));
	}

	/*-----敵の更新処理-----*/
	for (int i = 0; i < enemys.size(); ++i) {
		for (int j = 0; j < enemys[i].size(); ++j) {
			if (enemys[i][j]->isAlive == false)continue;
			enemys[i][j]->Update(*playerPos);
		}
	}

	//unitEnemysを更新する
	for (int i = 0; i < UNIT_ENEMYS_COUNT; ++i) {
		if (unitEnemys[i].isAlive == false) continue;

		unitEnemys[i].Update();
	}

	//仮でENTERが押されたら敵が全て初期化されるようにする
	if (Input::isKeyTrigger(KEY_INPUT_RETURN)) {
		Init();
	}
}

void EnemyManager::Init()
{
	//各タイマーを初期化
	generateNormalTimer = 0;
	generateCircleTimer = 0;

	//各ステータスを初期化
	states = ENEMY_STATES_NORMAL;
	unitStates = 0;

	//敵をすべて初期化
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
	//UNITのステータスに応じて処理を分ける
	switch (unitStates) {
	case ENEMY_UNIT_STATES_WALL:

		break;

	case ENEMY_UNIT_STATES_ROUND:

		break;
	}

	//UNITステータスでの通常の敵の更新処理を行う(移動のみ)
	UnitStatesEnemyUpdate();

	//unitEnemysを更新する
	for (int i = 0; i < UNIT_ENEMYS_COUNT; ++i) {
		if (unitEnemys[i].isAlive == false) continue;

		unitEnemys[i].Update();
	}

	//すべてのunitEnemysのフラグがfalseだったら通常の敵モードへ移行
	int alivecount = 0;
	for (int i = 0; i < UNIT_ENEMYS_COUNT; ++i) {
		if (unitEnemys[i].isAlive == false) continue;

		alivecount++;
	}
	//すべての敵グループが死んでいたら
	if (alivecount <= 0) {
		states = ENEMY_STATES_NORMAL;
	}

	//すべての敵がescapeしていたら通常的モードへ移行
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
	/*-----敵の更新処理-----*/
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

	//仮でENTERが押されたら敵が全て初期化されるようにする
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
	//重複して再生しないようにする
	if (!enemyDamageSEPlay) {
		SoundEmitter::Instance()->Play(enemyDamagedSE, DX_PLAYTYPE_BACK);
		enemyDamageSEPlay = true;
	}
}

void EnemyManager::PlayDeadSE()
{
	//重複して再生しないようにする
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
