#include "UnitEnemyt.h"

UnitEnemy::UnitEnemy()
{
	/*基本情報*/
	pos.Init();
	angle = 0;
	speed = 0;
	hp = 0;
	size = 0;
	isAlive = 0;
	states = 0;

	/*回る敵の情報*/
	addAngle = 0;
}

void UnitEnemy::Init()
{
	isAlive = false;
}

void UnitEnemy::Generate(Vec2 pos, float angle, int states)
{
	/*基本情報*/
	this->pos = pos;
	this->angle = angle;
	speed = UNIT_ENEMY_SPEED;
	hp = UNIT_ENEMY_HP;
	size = UNIT_ENEMY_SIZE;
	isAlive = true;
	moved.Init();

	/*ステータスによって分ける*/
	this->states = states;
	switch (states) {
	case ENEMY_UNIT_STATES_WALL:

		break;

	case ENEMY_UNIT_STATES_ROUND:

		//周回に必要な各変数を初期化
		addAngle = 0;


		break;
	}
}

void UnitEnemy::Update()
{
	//ステータスに応じて処理を変える
	switch (states) {
	case ENEMY_UNIT_STATES_WALL:

		//移動させる
		pos += Vec2(cos(angle) * speed, sin(angle) * speed);

		break;

	case ENEMY_UNIT_STATES_ROUND:

		//角度を更新する
		if (addAngle < UNIT_ENEMY_ROUND_ADDANGLE) {
			addAngle += 0.01f;
			angle += 0.01f;
		}

		//移動させる
		pos += Vec2(cos(angle) * speed, sin(angle) * speed);

		break;
	}

	//移動した量を足す
	moved += Vec2(fabs(cos(angle) * speed), fabs(sin(angle) * speed));

	//マップサイズの直系分移動したら削除 20000は定数.hを組み込んだら定数にする
	if (moved.x >= MAP_RADIUS_3_TIMES || moved.y >= MAP_RADIUS_3_TIMES) {
		Init();
	}
}

void UnitEnemy::Draw(Vec2 scrollAmount, int enemyCircleOuter, int enemyCircleInner, int enemyDirection)
{
	//敵を描画
	DrawExtendGraph(pos.x - size - scrollAmount.x, pos.y - size - scrollAmount.y, pos.x + size - scrollAmount.x, pos.y + size - scrollAmount.y, enemyCircleOuter, TRUE);
	float scale = (size / (UNIT_ENEMY_HP / hp));
	DrawExtendGraph(pos.x - scale - scrollAmount.x, pos.y - scale - scrollAmount.y, pos.x + scale - scrollAmount.x, pos.y + scale - scrollAmount.y, enemyCircleInner, TRUE);
	//DrawExtendGraph(pos.x - (size / hp) - scrollAmount.x, pos.y - (size / hp) - scrollAmount.y, pos.x + (size / hp) - scrollAmount.x, pos.y + (size / hp) - scrollAmount.y, enemyCircleInner, TRUE);
	//敵の向き 敵座標から向いてる方向に大きさ分動かした位置に描画
	DrawRotaGraph(pos.x + (cos(angle) * (size + size / 2.0f)) - scrollAmount.x, pos.y + (sin(angle) * (size + size / 2.0f)) - scrollAmount.y, CAMERA_RATE, angle - 0.35f, enemyDirection, TRUE);
}

/*--マネージャーの関数--*/
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
	//ローカル変数を宣言
	float angle = 0;
	Vec2 generatePos;

	//statesによって生成する値を変える
	switch (states) {
	case ENEMY_UNIT_STATES_WALL:

		//生成する角度をランダムで決める
		this->direction = direction;
		//生成する角度に応じて生成する位置も変える
		generatePos.Init();
		switch ((int)direction) {
		case 0:
			//角度を計算
			angle = direction * (DX_PI_F / 2.0f);
			//左上から下に向けて生成する。敵の進行方向は右
			generatePos.Init((WIN_WIDTH / 2.0f) - MAP_RADIUS - (MAP_RADIUS / 2.0f), (WIN_HEIGHT / 2.0f) - MAP_RADIUS);
			//壁の敵を生成する
			for (int i = 0; i < MAP_RADIUS_2_TIMES / UNIT_ENEMY_SIZE; ++i) {
				for (int j = 0; j < UNIT_ENEMY_COUNT; ++j) {
					//生成済みだったらスキップする
					if (unitEnemy[j].isAlive == true) continue;

					unitEnemy[j].Generate(Vec2(generatePos.x, generatePos.y + (UNIT_ENEMY_SIZE * 3 * i)), angle, ENEMY_UNIT_STATES_WALL);
					break;
				}
			}
			break;
		case 1:
			//角度を計算
			angle = direction * (DX_PI_F / 2.0f);
			//右上から左に向けて生成する。敵の進行方向は下
			generatePos.Init((WIN_WIDTH / 2.0f) + MAP_RADIUS, (WIN_HEIGHT / 2.0f) - MAP_RADIUS - (MAP_RADIUS / 2.0f));
			//壁の敵を生成する
			for (int i = 0; i < MAP_RADIUS_2_TIMES / UNIT_ENEMY_SIZE; ++i) {
				for (int j = 0; j < UNIT_ENEMY_COUNT; ++j) {
					//生成済みだったらスキップする
					if (unitEnemy[j].isAlive == true) continue;

					unitEnemy[j].Generate(Vec2(generatePos.x - (UNIT_ENEMY_SIZE * 3 * i), generatePos.y), angle, ENEMY_UNIT_STATES_WALL);
					break;
				}
			}
			break;
		case 2:
			//角度を計算
			angle = direction * (DX_PI_F / 2.0f);
			//右下から上に向けて生成する。敵の進行方向は左
			generatePos.Init((WIN_WIDTH / 2.0f) + MAP_RADIUS + (MAP_RADIUS / 2.0f), (WIN_HEIGHT / 2.0f) + MAP_RADIUS);
			//壁の敵を生成する
			for (int i = 0; i < MAP_RADIUS_2_TIMES / UNIT_ENEMY_SIZE; ++i) {
				for (int j = 0; j < UNIT_ENEMY_COUNT; ++j) {
					//生成済みだったらスキップする
					if (unitEnemy[j].isAlive == true) continue;

					unitEnemy[j].Generate(Vec2(generatePos.x, generatePos.y - (UNIT_ENEMY_SIZE * 3 * i)), angle, ENEMY_UNIT_STATES_WALL);
					break;
				}
			}
			break;
		case 3:
			//角度を計算
			angle = direction * (DX_PI_F / 2.0f);
			//左下から左に向けて生成する。敵の進行方向は上
			generatePos.Init((WIN_WIDTH / 2.0f) - MAP_RADIUS, (WIN_HEIGHT / 2.0f) + MAP_RADIUS + (MAP_RADIUS / 2.0f));
			//壁の敵を生成する
			for (int i = 0; i < MAP_RADIUS_2_TIMES / UNIT_ENEMY_SIZE; ++i) {
				for (int j = 0; j < UNIT_ENEMY_COUNT; ++j) {
					//生成済みだったらスキップする
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
	/*-----statesがRoundだった場合、一定時間起きに敵グループを生成-----*/
	if (states == ENEMY_UNIT_STATES_ROUND) {
		//タイマーを加算
		roundEnemyGenerateTimer++;
		if (roundEnemyGenerateTimer >= UNIT_ENEMY_ROUND_GENERATE_TIMER) {
			roundEnemyGenerateTimer = 0;
			//生成した数が既定の数に達していなかったら
			if (roundEnemyGenerateCount < UNIT_ENEMY_ROUND_GENERATE_COUNT) {

				//敵を生成
				float angle = 0.01f * roundEnemyGenerateCount;
				Vec2 generatePos = Vec2(WIN_WIDTH / 2.0f, WIN_HEIGHT / 2.0f);
				//指定数生成する
				for (int i = 0; i < UNIT_ENEMY_ROUND_COUNT; ++i) {
					for (int j = 0; j < UNIT_ENEMY_COUNT; ++j) {
						//生成済みだったらスキップする
						if (unitEnemy[j].isAlive == true) continue;

						unitEnemy[j].Generate(Vec2(generatePos.x, generatePos.y), angle, ENEMY_UNIT_STATES_WALL);
						break;
					}
					//回転量を更新
					angle += DX_TWO_PI_F / UNIT_ENEMY_ROUND_COUNT;
				}

				//生成した数を加算
				roundEnemyGenerateCount++;
			}
		}
	}

	/*-----UnitEnemyの更新処理-----*/
	for (int i = 0; i < UNIT_ENEMY_COUNT; ++i) {
		if (unitEnemy[i].isAlive == false) continue;

		unitEnemy[i].Update();
	}

	/*-----すべての敵が死んでいたら大きなフラグを折る-----*/
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

	/*-----すべての敵がマップ外へ移動したら脱出したフラグを折る-----*/
	switch (states) {
	case ENEMY_UNIT_STATES_WALL:
		//壁の敵だった場合
		for (int i = 0; i < UNIT_ENEMY_COUNT; ++i) {
			if (unitEnemy[i].moved.x <= MAP_RADIUS + (MAP_RADIUS / 2.0f) && unitEnemy[i].moved.y <= MAP_RADIUS + (MAP_RADIUS / 2.0f)) continue;

			isEscape = false;
		}
		break;

	case ENEMY_UNIT_STATES_ROUND:
		//円状に広がる敵だった場合、最大生成数まで生成してないと処理を行わない
		if (roundEnemyGenerateCount >= UNIT_ENEMY_ROUND_GENERATE_COUNT) {
			for (int i = 0; i < UNIT_ENEMY_COUNT; ++i) {
				if (unitEnemy[i].moved.x <= MAP_RADIUS && unitEnemy[i].moved.y <= MAP_RADIUS) continue;

				isEscape = false;
			}
		}
		break;
	}

	/*-----危険信号のアルファ値を変える-----*/
	//危険信号のアルファ値を変える
	warningAlphaTimer += 0.01f;
	if (warningAlphaChange) {
		//このフラグがtrueだったらアルファ値を大きくしていく
		warningAlpha = 255 * (powf(1 - warningAlphaTimer, 3));
		if (warningAlpha <= 10) {
			warningAlphaTimer = 0;
			warningAlphaChange = false;
			warningAlpha = 0;
		}
	}
	else {
		//このフラグがfalseだったらアルファ値を小さくしていく
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
	//敵を描画
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

	//危険信号を描画
	if (isEscape) {

		switch (states) {

		case ENEMY_UNIT_STATES_ROUND:

			SetDrawBlendMode(DX_BLENDMODE_ALPHA, warningAlpha + 100);
			//中心との距離を求める
			buff = Vec2((WIN_WIDTH / 2.0f) - playerPos.x, (WIN_HEIGHT / 2.0f) - playerPos.y);

			//スクロール量に上限をつける
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
			//危険信号を描画				マジックナンバーは微調整
			switch (direction) {
			case UNIT_ENEMY_DIRECTION_LEFT:

				//右から壁がくる場合の描画処理を行う
				x = 900;
				y = 40 + graphSize;

				SetDrawBlendMode(DX_BLENDMODE_ALPHA, warningAlpha);
				for (int i = 0; i < 9; ++i) {
					DrawExtendGraph(x - graphSize / 2.0f, y - graphSize / 2.0f + (i * graphSize), x + graphSize / 2.0f, y + graphSize / 2.0f + (i * graphSize), warningGraph, TRUE);
				}

				break;

			case UNIT_ENEMY_DIRECTION_UP:

				//下から壁がくる場合
				x = 100;
				y = 670;

				SetDrawBlendMode(DX_BLENDMODE_ALPHA, warningAlpha);
				for (int i = 0; i < 890 / graphSize; ++i) {
					DrawExtendGraph(x - graphSize / 2.0f + (i * graphSize), y - graphSize / 2.0f, x + graphSize / 2.0f + (i * graphSize), y + graphSize / 2.0f, warningGraph, TRUE);
				}

				break;

			case UNIT_ENEMY_DIRECTION_RIGHT:

				//左から壁がくる場合
				x = 80;
				y = 40 + graphSize;

				SetDrawBlendMode(DX_BLENDMODE_ALPHA, warningAlpha);
				for (int i = 0; i < 9; ++i) {
					DrawExtendGraph(x - graphSize / 2.0f, y - graphSize / 2.0f + (i * graphSize), x + graphSize / 2.0f, y + graphSize / 2.0f + (i * graphSize), warningGraph, TRUE);
				}

				break;

			case UNIT_ENEMY_DIRECTION_DOWN:

				//上から壁がくる場合
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
