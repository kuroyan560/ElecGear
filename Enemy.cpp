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
	//生成中だったらアルファブレンドをかける
	if (!isGenerate) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((ENEMY_GENERATE_TIMER - generateTimer) / ENEMY_GENERATE_TIMER) * 255.0f);
	}

	//敵を描画
	DrawExtendGraph(pos.x - size - scrollAmount.x, pos.y - size - scrollAmount.y, pos.x + size - scrollAmount.x, pos.y + size - scrollAmount.y, enemyCircleOuter, TRUE);
	float scale = (size / (ENEMY_HP / hp));
	DrawExtendGraph(pos.x - scale - scrollAmount.x, pos.y - scale - scrollAmount.y, pos.x + scale - scrollAmount.x, pos.y + scale - scrollAmount.y, enemyCircleInner, TRUE);
	//DrawExtendGraph(pos.x - (size / hp) - scrollAmount.x, pos.y - (size / hp) - scrollAmount.y, pos.x + (size / hp) - scrollAmount.x, pos.y + (size / hp) - scrollAmount.y, enemyCircleInner, TRUE);
	//敵の向き 敵座標から向いてる方向に大きさ分動かした位置に描画
	DrawRotaGraph(pos.x + (cos(angle) * (size + size / 2.0f)) - scrollAmount.x, pos.y + (sin(angle) * (size + size / 2.0f)) - scrollAmount.y, CAMERA_RATE, angle - 0.35f, enemyDirection, TRUE);

	int graphSize = 25;
	Vec2 buff;
	Vec2 scrollBuff;
	Vec2 scrollWidth = Vec2(550 - 160 + 35, -250 - 160);
	Vec2 scrollHeigh = Vec2(305 + 15, -305 - 15);

	//危険信号を描画

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	//中心との距離を求める
	buff = Vec2(pos.x - playerPos.x, pos.y - playerPos.y);

	//スクロール量に上限をつける
	scrollBuff = buff;
	if (buff.x > scrollWidth.x) scrollBuff.x = scrollWidth.x;
	if (buff.x < scrollWidth.y) scrollBuff.x = scrollWidth.y;
	if (buff.y > scrollHeigh.x) scrollBuff.y = scrollHeigh.x;
	if (buff.y < scrollHeigh.y) scrollBuff.y = scrollHeigh.y;

	//危険信号が画面内に収まっていたら表示しない
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

	//プレイヤーとの判定かつ生成中だったら当たり判定を行わない
	if (player && !isGenerate) {
		return false;
	}

	//当たり判定を行う
	float enemyBulletDistance = pos.Distance(this->pos);
	if (enemyBulletDistance <= size + this->size) {
		hit = true;

		if (player)	//プレイヤー
		{
			this->Init();	//削除
			//EnemyManager::Instance()->PlayDeadSE();
			EnemyHitEffectManager::Instance()->DamagedEffect(this->pos);
			//コンボを初期化
			Combo::Instance()->Damage();
		}
		else	//弾
		{
			//敵のHPを削る
			this->hp--;
			EnemyManager::Instance()->PlayDamageSE();
			//敵のHPが0以下になったら死亡
			if (this->hp <= 0) {
				this->Init();	//削除
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

		//斬撃半径内に敵が入っていたら角度を求める
		float bulletAngle = angle - DX_PI_F / 1.95f;
		Vec2 enemyVec = this->pos - pos;
		enemyVec.Normalize();
		float enemyAngle = atan2(enemyVec.y, enemyVec.x);
		//弾射出角度 - 敵との角度の絶対値ががPI / 2.0fより小さかったら当たっている
		if (fabs(bulletAngle - enemyAngle) <= DX_PI_F / 2.0f * 1.1) {
			hit = true;
			//敵のHPを削る
			this->hp -= ENEMY_HP / 2;
		}
	}

	if (hit)
	{
		//敵のHPが0以下になったら死亡
		if (this->hp <= 0) {
			this->Init();	//削除
			EnemyManager::Instance()->PlayDeadSE();
			EnemyHitEffectManager::Instance()->DeadEffect(this->pos);
			deathCount++;
		}
		else
		{
			EnemyHitEffectManager::Instance()->DamagedEffect(this->pos);
			EnemyManager::Instance()->PlayDamageSE();
			//当たった数を更新
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

/*--プレイヤー追尾--*/
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
	/*-----角度の更新処理-----*/
	//float playerAngle = atan2(playerPos.y - pos.y, playerPos.x - pos.x);
	////現在の角度の移動ベクトル
	//Vec2 nowVec = Vec2(cosf(angle) * speed, sinf(angle) * speed);
	////プレイヤーとの角度の移動ベクトル
	//Vec2 playerVec = Vec2(cosf(playerAngle) * speed, sinf(playerAngle) * speed);
	////外積を計算
	//float crossProduct = nowVec.Cross(playerVec);
	////外積の計算結果から角度を調整
	//if (crossProduct > 0)
	//{
	//	angle += ENEMY_CHANGE_ANGLE;
	//}
	//else if (crossProduct < 0)
	//{
	//	angle -= ENEMY_CHANGE_ANGLE;

	//生成状態だったら更新処理を行う
	if (isGenerate) {

		if (hitStanTimer) {
			hitStanTimer--;
		}
		else
		{
			if (!isEscape) {
				Vec2 toVec = playerPos - pos;
				toVec.Normalize();

				//円外にいるとき元素奥
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
			/*-----脱出モードがonになったら中心とは逆方向に逃げるため、角度を再計算する-----*/
			else {
				angle = atan2(pos.y - playerPos.y, pos.x - playerPos.x);
				pos += Vec2(cosf(angle) * speed, sinf(angle) * speed);	//逃走
			}
		}

		//hpが0になっていたら死亡
		if (hp <= 0) {
			Init();
		}

		//敵がプレイヤーからマップ直径分移動したら初期化
		if (pos.Distance(playerPos) >= MAP_RADIUS_2_TIMES) {
			Init();
		}
	}

	//生成していなかったらタイマーを更新
	else {
		generateTimer--;
		if (generateTimer <= 0) {
			isGenerate = true;
		}
	}
}

/*--生成された角度に真っすぐ--*/
void StraightEnemy::Generate(Vec2 generatePos, Vec2 playerPos, int* hp, float* angle, Vec2* targetPos)
{
	pos = generatePos;
	//pos = Vec2(GetRand(WIN_WIDTH), GetRand(WIN_HEIGHT));
	speed = GetRand(STRAIGHT_ENEMY_MAX_SPEED - STRAIGHT_ENEMY_MIN_SPEED) + STRAIGHT_ENEMY_MIN_SPEED;
	speed *= 1.5f;
	//angleに値が入っていなかったらプレイヤーの方向に飛ばす
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
	//生成状態だったら更新処理を行う
	if (isGenerate) {

		if (hitStanTimer) {
			hitStanTimer--;
		}
		else
		{
			pos += Vec2(cosf(angle) * speed, sinf(angle) * speed);	//逃走
		}

		//hpが0になっていたら死亡
		if (hp <= 0) {
			Init();
		}

		//敵がプレイヤーからマップ直径分移動したら初期化
		if (pos.Distance(playerPos) >= MAP_RADIUS_2_TIMES) {
			Init();
		}
	}

	//生成していなかったらタイマーを更新
	else {
		generateTimer--;
		if (generateTimer <= 0) {
			isGenerate = true;
		}
	}
}

/*--指定された位置まで移動--*/
void DestinationEnemy::Generate(Vec2 generatePos, Vec2 playerPos, int* hp, float* angle, Vec2* targetPos)
{
	pos = generatePos;
	//pos = Vec2(GetRand(WIN_WIDTH), GetRand(WIN_HEIGHT));
	speed = DESTIANTION_ENEMY_SPEED;
	//angleに値が入っていなかったらプレイヤーの方向に飛ばす
	if (angle == nullptr) {
		this->angle = atan2(playerPos.y - pos.y, playerPos.x - pos.x);
	}
	else {
		this->angle = *angle;
	}
	//targetPosの値がnullptrじゃなかったら値を入れる
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
	//生成状態だったら更新処理を行う
	if (isGenerate) {


		if (hitStanTimer) {
			hitStanTimer--;
		}
		else
		{
			//脱出モードじゃなかったら
			if (!isEscape) {
				//移動する方向を求める
				Vec2 moveVec = Vec2(targetPos.x - pos.x, targetPos.y - pos.y);
				moveVec.Normalize();
				//移動する量を求める
				float distance = targetPos.Distance(pos);
				//移動速度を更新
				speed = DESTIANTION_ENEMY_SPEED * (distance / distanceDef);
				//下限を設ける
				if (speed <= 2 && distance >= 1.0f) {
					speed = 2.0f;
				}

				//移動させる
				if (fabs(distance) > 1) {
					pos += Vec2(moveVec.x * speed, moveVec.y * speed);
				}

				//プレイヤーの方向を向かせる
				angle = atan2f(playerPos.y - pos.y, playerPos.x - pos.x);
			}
			else {
				//中心とは逆方向のアングルを求める
				angle = atan2f(pos.y - HALF_WIN_HEIGHT, pos.x - HALF_WIN_WIDTH);
				//移動させる
				pos += Vec2(cosf(angle) * speed, sinf(angle) * speed);
			}
		}

		//脱出するまでのタイマーを加算する
		escapeTimer++;
		if (escapeTimer >= DESTINATION_ENEMY_ESCAPE_TIME) {
			speed = 5.0f;
			isEscape = true;
		}

		//hpが0になっていたら死亡
		if (hp <= 0) {
			Init();
		}

		//敵がプレイヤーからマップ直径分移動したら初期化
		if (pos.Distance(playerPos) >= MAP_RADIUS_2_TIMES) {
			Init();
		}
	}

	//生成していなかったらタイマーを更新
	else {
		generateTimer--;
		if (generateTimer <= 0) {
			isGenerate = true;
		}
	}
}

/*--無限型に移動--*/
void InfinityEnemy::Generate(Vec2 generatePos, Vec2 playerPos, int* hp, float* angle, Vec2* targetPos)
{
	pos = Vec2(HALF_WIN_WIDTH, HALF_WIN_HEIGHT);
	//angleに値が入っていなかったらプレイヤーの方向に飛ばす
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

	//回転に必要な変数を初期化
	rotationAngle = 0;
	rotationAngleAmount = 0;
	isRotationLeft = true;

	isGenerate = false;
	generateTimer = ENEMY_GENERATE_TIMER - 1;
}
void InfinityEnemy::Update(Vec2 playerPos)
{
	//生成状態だったら更新処理を行う
	if (isGenerate) {

		if (hitStanTimer) {
			hitStanTimer--;
		}
		else
		{
			//どっち側を回転しているか
			if (isRotationLeft) {
				//回転量を加算
				rotationAngle += INFINITY_ENEMY_ROTATION_ADD;
				rotationAngleAmount += INFINITY_ENEMY_ROTATION_ADD;
				//回転させる
				pos = Vec2(cosf(rotationAngle) * INFINITY_ENEMY_ROTATION_RADIUS + INFINITY_ENEMY_ROTATION_LEFT.x,
					sinf(rotationAngle) * INFINITY_ENEMY_ROTATION_RADIUS + INFINITY_ENEMY_ROTATION_LEFT.y);
				//一周分回転したら
				if (rotationAngleAmount >= DX_TWO_PI_F) {
					rotationAngle = DX_PI_F;	//右側の中心から見ると、画面中心は左側なので
					rotationAngleAmount = 0;
					isRotationLeft = false;
				}
				//敵の向いている方向を更新
				angle = atan2(playerPos.y - pos.y, playerPos.x - pos.x);
			}
			else {
				//右側を回転している場合

				//回転量を加算
				rotationAngle -= INFINITY_ENEMY_ROTATION_ADD;
				rotationAngleAmount += INFINITY_ENEMY_ROTATION_ADD;
				//回転量が0以下になったら
				if (rotationAngle < 0) {
					rotationAngle = DX_TWO_PI_F;
				}
				//回転させる
				pos = Vec2(cosf(rotationAngle) * INFINITY_ENEMY_ROTATION_RADIUS + INFINITY_ENEMY_ROTATION_RIGHT.x,
					sinf(rotationAngle) * INFINITY_ENEMY_ROTATION_RADIUS + INFINITY_ENEMY_ROTATION_RIGHT.y);
				//一周分回転したら
				if (rotationAngleAmount >= DX_TWO_PI_F) {
					rotationAngle = 0;
					rotationAngleAmount = 0;
					isRotationLeft = true;
				}
				//敵の向いている方向を更新
				angle = atan2(playerPos.y - pos.y, playerPos.x - pos.x);
			}
		}

		//hpが0になっていたら死亡
		if (hp <= 0) {
			Init();
		}

		//敵がプレイヤーからマップ直径分移動したら初期化
		if (pos.Distance(playerPos) >= MAP_RADIUS_2_TIMES) {
			Init();
		}


	}
	//生成していなかったらタイマーを更新
	else {
		generateTimer--;
		if (generateTimer <= 0) {
			isGenerate = true;
		}
	}
}

/*--生成した位置に停止--*/
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
	//生成状態だったら更新処理を行う
	if (isGenerate) {

		escapeCount++;
		if (escapeCount >= STANDBY_ENEMY_ESCAPE_COUNT) {
			speed = 2.0f;
			isEscape = true;
		}

		//常にプレイヤーの方向を見るようにする
		angle = atan2(playerPos.y - pos.y, playerPos.x - pos.x);

		//hpが0になっていたら死亡
		if (hp <= 0) {
			Init();
		}

		//敵がプレイヤーからマップ直径分移動したら初期化
		if (pos.Distance(playerPos) >= MAP_RADIUS_2_TIMES) {
			Init();
		}


	}
	//生成していなかったらタイマーを更新
	else {
		generateTimer--;
		if (generateTimer <= 0) {
			isGenerate = true;
		}
	}
}

/*--ハイスコアの敵--*/
void HighScoreEnemy::Generate(Vec2 generatePos, Vec2 playerPos, int* hp, float* angle, Vec2* targetPos)
{
	//座標を初期化
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
	//生成状態だったら更新処理を行う
	if (isGenerate) {

		if (hitStanTimer) {
			hitStanTimer--;
		}
		else
		{
			//反時計回りに回転させる
			rotationAngle += HIGHTSCORE_ENEMY_ROTATION_ADD;
			//一周したら
			if (rotationAngle >= DX_TWO_PI_F) {
				rotationAngle = 0;
				rotationCount++;
				//既定の回数回転したら
				if (rotationCount >= HIGHTSCORE_ENEMY_ROTATION_COUNT) {
					isRadiusMinus = false;
				}
			}
			//小さくなるフラグが立っていたら半径を小さくする
			if (isRadiusMinus) {
				//最低値より大きかったら小さくする
				if (rotationRadius > HIGHTSCORE_ENEMY_RADIUS_MIN) {
					rotationRadius -= HIGHTSCORE_ENEMY_RADIUS_CHANGE;
					//最低値より小さくなっていたら押し戻す
					if (rotationRadius < HIGHTSCORE_ENEMY_RADIUS_MIN) {
						rotationRadius = HIGHTSCORE_ENEMY_RADIUS_MIN;
					}
				}
			}
			//小さくなるフラグが立っていなかったら半径を置きくする
			else {
				//どんどん大きくする
				rotationRadius += HIGHTSCORE_ENEMY_RADIUS_CHANGE;
			}

			//移動させる
			pos = Vec2(cosf(rotationAngle) * rotationRadius + HALF_WIN_WIDTH,
				sinf(rotationAngle) * rotationRadius + HALF_WIN_HEIGHT);
		}

		//プレイヤーの方向を向くようにする
		angle = atan2f(playerPos.y - pos.y, playerPos.x - pos.x);

		//hpが0になっていたら死亡
		if (hp <= 0) {
			Init();
		}

		//敵がプレイヤーからマップ直径分移動したら初期化
		if (pos.Distance(playerPos) >= MAP_RADIUS_2_TIMES) {
			Init();
		}
	}
	//生成していなかったらタイマーを更新
	else {
		generateTimer--;
		if (generateTimer <= 0) {
			isGenerate = true;
		}
	}
}
