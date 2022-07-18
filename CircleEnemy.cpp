#include "CircleEnemy.h"
#include"EnemyHitEffectManager.h"
#include"EnemyManager.h"
#include "Combo.h"

/*-----周回敵用の関数-----*/
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
	//先頭が生きていれば先頭を描画
	DrawExtendGraph(head.pos.x - head.size - scrollAmount.x, head.pos.y - head.size - scrollAmount.y, head.pos.x + head.size - scrollAmount.x, head.pos.y + head.size - scrollAmount.y, enemyCircleOuter, TRUE);
	//敵の向き 敵座標から向いてる方向に大きさ分動かした位置に描画
	DrawRotaGraph(head.pos.x + (cos(head.angle) * (head.size + head.size / 2.0f)) - scrollAmount.x, head.pos.y + (sin(head.angle) * (head.size + head.size / 2.0f)) - scrollAmount.y, CAMERA_RATE, head.angle - 0.35f, enemyDirection, TRUE);
	if (head.isAlive) {
		float a = CIRCLE_ENEMY_COUNT_ALL;
		float buff = ((float)aliveCount / (float)a);
		float scale = (head.size * buff) + 5;
		DrawExtendGraph(head.pos.x - scale - scrollAmount.x, head.pos.y - scale - scrollAmount.y, head.pos.x + scale - scrollAmount.x, head.pos.y + scale - scrollAmount.y, enemyCircleInner, TRUE);
	}

	//後続達を描画
	for (int i = 0; i < CIRCLE_ENEMY_FOLLOWING_COUNT; ++i) {
		//生存していなければ描画しない
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

	//先頭との当たり判定
	float enemyBulletDistance = pos.Distance(head.pos);
	if (enemyBulletDistance <= size + head.size) {
		Hit(player);
		hit = true;
		//コンボを初期化
		if (player) {
			Combo::Instance()->Damage();
		}
		else {
			Combo::Instance()->Hit(30);
		}
	}

	//後続との当たり判定
	for (int follow = 0; follow < CIRCLE_ENEMY_FOLLOWING_COUNT; ++follow) {
		//生存していなかったらスキップ
		if (following[follow].isAlive == false) continue;

		float enemyBulletDistance = pos.Distance(following[follow].pos);
		if (enemyBulletDistance <= size + following[follow].size) {
			//敵死亡
			Hit(player);
			hit = true;
			//コンボを初期化
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
			//生存していなかったらスキップ
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

	//先頭との当たり判定
	if (head.pos.Distance(pos) <= SLASH_RADIUS + head.size) {

		//斬撃半径内に敵が入っていたら角度を求める
		float bulletAngle = angle - DX_PI_F / 1.95f;
		Vec2 enemyVec = head.pos - pos;
		enemyVec.Normalize();
		float enemyAngle = atan2(enemyVec.y, enemyVec.x);
		//弾射出角度 - 敵との角度の絶対値ががPI / 2.0fより小さかったら当たっている
		if (fabs(bulletAngle - enemyAngle) <= DX_PI_F / 2.0f) {
			//敵を削除
			Hit(false);
			hit = true;
			deathCount++;
		}
	}

	//後続との当たり判定
	for (int follow = 0; follow < CIRCLE_ENEMY_FOLLOWING_COUNT; ++follow) {
		//生存していなかったらスキップ
		if (following[follow].isAlive == false) continue;

		if (following[follow].pos.Distance(pos) <= SLASH_RADIUS + following[follow].size) {

			//斬撃半径内に敵が入っていたら角度を求める
			float bulletAngle = angle - DX_PI_F / 1.95f;
			Vec2 enemyVec = following[follow].pos - pos;
			enemyVec.Normalize();
			float enemyAngle = atan2(enemyVec.y, enemyVec.x);
			//弾射出角度 - 敵との角度の絶対値ががPI / 2.0fより小さかったら当たっている
			if (fabs(bulletAngle - enemyAngle) <= DX_PI_F / 2.0f) {
				//敵を削除
				Hit(false);
				hit = true;
				//当たった数を更新
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
	//本来あるべき移動速度を更新
	if (aliveCount > 0) {
		float buff = CIRCLE_ENEMY_COUNT_ALL - aliveCount;
		originalSpeed = CIRCLE_ENEMY_SPEED / (float)(buff / CIRCLE_ENEMY_COUNT_ALL) + CIRCLE_ENEMY_SPEED_DEF;
	}

	//スピードを加算
	speed = CIRCLE_ENEMY_HIT_ACCEL + originalSpeed;

	//脱出モードだったら速度を上げる
	if (head.isEscape) {
		originalSpeed += 0.5f;
		speed += 0.5f;
	}
}
void CircleEnemys::Hit(bool player)
{
	//後ろから順番に削除していく
	if (aliveCount > 1) {
		for (int i = CIRCLE_ENEMY_FOLLOWING_COUNT - 1; i >= 0; --i) {
			//すでに死んでいたら処理を飛ばす
			if (following[i].isAlive == false) continue;

			//殺す
			following[i].Init();
			EnemyManager::Instance()->PlayDamageSE();
			EnemyHitEffectManager::Instance()->DamagedEffect(following[i].pos);
			break;
		}
	}
	else {
		//殺す
		head.Init();
		if (!player) {
			EnemyManager::Instance()->PlayDeadSE();
		}
		EnemyHitEffectManager::Instance()->DeadEffect(head.pos);
	}

	//生存数を再確認し、すべて死んでいたら大きなフラグを折る
	aliveCount = 0;
	if (head.isAlive == true) aliveCount++;
	for (int i = 0; i < CIRCLE_ENEMY_FOLLOWING_COUNT; ++i) {
		if (following[i].isAlive == false) continue;
		aliveCount++;
	}
	//aliveCountが0以下だったらすべて死んでいるということなので初期化
	if (aliveCount <= 0) {
		Init();
	}

	//加速させる
	HitAccel();
}
/*-----先頭用の関数-----*/
void CircleEnemyHead::Init()
{
	isAlive = false;

	isEscape = false;
}
void CircleEnemyHead::Generate(Vec2 generatePos)
{
	//位置を決める
	pos = generatePos;
	//pos = Vec2(GetRand(WIN_WIDTH), GetRand(WIN_HEIGHT));
	//その他情報を初期化
	size = 30 * CAMERA_RATE;
	angle = 0;
	radius = 0;
	isAlive = true;
	hp = CIRCLE_ENEMY_HP;
	isEscape = false;
}
void CircleEnemyHead::Update(int states, Vec2 playerPos, float speed)
{
	//ローカル変数を宣言
	float playerAngle = 0;
	Vec2 nowVec = {};
	Vec2 playerVec = {};
	float crossProduct = 0;

	//渡されたステータスによって処理を変える
	switch (states)
	{
	case CIRCLE_ENEMY_STATES_ROCKON:

		/*-----角度の更新処理-----*/
		playerAngle = atan2(playerPos.y - pos.y, playerPos.x - pos.x);
		//現在の角度の移動ベクトル
		nowVec = Vec2(cosf(angle) * speed, sinf(angle) * speed);
		//プレイヤーとの角度の移動ベクトル
		playerVec = Vec2(cosf(playerAngle) * speed, sinf(playerAngle) * speed);
		//外積を計算
		crossProduct = nowVec.Cross(playerVec);
		//外積の計算結果から角度を調整
		if (crossProduct > 0)
		{
			angle += CIRCLE_ENEMY_CHANGE_ANGLE;
		}
		else if (crossProduct < 0)
		{
			angle -= CIRCLE_ENEMY_CHANGE_ANGLE;
		}

		/*-----脱出モードがonになったら中心とは逆方向に逃げるため、角度を再計算する-----*/
		if (isEscape) {
			angle = atan2(pos.y - playerPos.y, pos.x - playerPos.x);
		}

		/*-----移動の更新処理-----*/
		pos += Vec2(cosf(angle) * speed, sinf(angle) * speed);

		break;

	case CIRCLE_ENEMY_STATES_ROUND:

		//現在の位置から中心への半径と角度を求める
		radius = pos.Distance(Vec2(WIN_WIDTH / 2.0f, WIN_HEIGHT / 2.0f));
		angle = atan2(pos.y - WIN_HEIGHT / 2.0f, pos.x - WIN_WIDTH / 2.0f);
		//角度を更新して移動させる
		angle += 0.005f;
		pos = Vec2(cos(angle) * radius + WIN_WIDTH / 2.0f, sin(angle) * radius + WIN_HEIGHT / 2.0f);

		break;

	default:
		break;
	}

	//hpが0だったら初期化
	if (hp <= 0) {
		Init();
	}
}
/*-----後続用の関数-----*/
void CircleEnemyFollowing::Init()
{
	isAlive = false;
}
void CircleEnemyFollowing::Generate(Vec2 prevPos)
{
	//ひとつ前の敵の位置と中心点との距離を求める
	float radius = prevPos.Distance(Vec2(WIN_WIDTH / 2.0f, WIN_HEIGHT / 2.0f));
	//前の敵と中心との角度を求める
	float angle = atan2(prevPos.y - WIN_HEIGHT / 2.0f, prevPos.x - WIN_WIDTH / 2.0f);
	//少し回転させた位置に生成する
	angle -= 0.1f;
	pos.Init();
	pos += Vec2(cos(angle) * radius + WIN_WIDTH / 2.0f, sin(angle) * radius + WIN_HEIGHT / 2.0f);
	//その他情報を初期化
	size = 30 * CAMERA_RATE;
	isAlive = true;
	hp = CIRCLE_ENEMY_HP;
}
void CircleEnemyFollowing::Update(Vec2 prevPos, float speed)
{
	//前の敵を追いかける
	Vec2 forwardVec = Vec2(prevPos.x - pos.x, prevPos.y - pos.y);
	forwardVec.Normalize();

	pos += Vec2(forwardVec.x * speed, forwardVec.y * speed);

	//前と衝突していたらおしもどす
	float distance = pos.Distance(prevPos);
	if (distance <= size * 2 + size / 2.0f) {
		//ぶつかっていたらぶつかっている分だけ押し戻す
		float pushAmount = size * 2 + size / 2.0f - distance;
		//押し戻す角度を求める
		Vec2 pushVec = Vec2(pos.x - prevPos.x, pos.y - prevPos.y);
		pushVec.Normalize();
		//押し戻す
		pos += Vec2(pushVec.x * pushAmount, pushVec.y * pushAmount);
	}

	//hpが0だったら初期化
	if (hp <= 0) {
		Init();
	}

}

/*---プレイヤー追尾---*/
void TrackingCircleEnemys::Generate(Vec2 generatePos, Vec2 playerPos, int* hp, float* angle, Vec2* targetPos)
{
	//まず先頭を生成する
	head.Generate(generatePos);
	//続いて後続を生成する
	following[0].Generate(head.pos);
	for (int i = 1; i < CIRCLE_ENEMY_FOLLOWING_COUNT - 1; ++i) {
		following[i].Generate(following[i - 1].pos);
	}
	//フラグを立てる
	isAlive = true;
	aliveCount = CIRCLE_ENEMY_COUNT_ALL;

	//speedを初期化 生存敵が少なくなればなるほど早くなる
	speed = CIRCLE_ENEMY_SPEED / (CIRCLE_ENEMY_COUNT_ALL / CIRCLE_ENEMY_COUNT_ALL) + CIRCLE_ENEMY_SPEED_DEF;
	originalSpeed = speed;

	slashed = false;
}
void TrackingCircleEnemys::Update(Vec2 playerPos)
{
	/*-----ステータスの更新-----*/
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

	/*-----先頭の更新-----*/
	head.Update(states, playerPos, speed);

	/*-----後続の更新-----*/
	following[0].Update(head.pos, speed);
	for (int i = 1; i < CIRCLE_ENEMY_FOLLOWING_COUNT - 1; ++i) {
		following[i].Update(following[i - 1].pos, speed);
	}

	/*-----生存確認-----*/
	aliveCount = 0;
	if (head.isAlive == true) aliveCount++;
	for (int i = 0; i < CIRCLE_ENEMY_FOLLOWING_COUNT; ++i) {
		if (following[i].isAlive == false) continue;
		aliveCount++;
	}
	//aliveCountが0以下だったらすべて死んでいるということなので初期化
	if (aliveCount <= 0) {
		Init();
	}

	//本来あるべきスピード - 現在のスピードの絶対値が1以上だったら近づける
	if (fabs(originalSpeed - speed) >= 1) {
		//+-に分けて調整
		if (speed > 0) {
			speed -= 1;
		}
		else {
			speed += 1;
		}
	}

}