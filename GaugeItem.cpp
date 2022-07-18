#include "GaugeItem.h"

GaugeItem::GaugeItem()
{
	pos.Init();
	radius = 0;
	alpha = 0;
	angle = 0;
	playerDistance = 0;
	states = 0;
	isAlive = false;
	isExpantion = false;
}

void GaugeItem::Init()
{
	isAlive = false;
	isCollision = false;
}

void GaugeItem::Generate()
{
	pos = Vec2(GetRand(MAP_RADIUS_2_TIMES) - MAP_RADIUS, GetRand(MAP_RADIUS_2_TIMES) - MAP_RADIUS);
	radius = ITEM_SIZE;
	alpha = 255;
	angle = 0;
	playerDistance = 0;
	deleteTimer = 0;
	states = ITEM_STATES_IDOL;
	isAlive = true;
	isExpantion = true;
	isCollision = true;
}

void GaugeItem::Update(Vec2 playerPos)
{

	//状態に応じて処理を分ける
	switch (states) {
	case ITEM_STATES_IDOL:
		//アイテムがIDOLの場合は削除タイマーを加算

		deleteTimer++;
		if (deleteTimer >= GAUGEITEM_DELETE_TIMER) {
			deleteTimer = 0;
			states = ITEM_STATES_DISAPPEAR;
		}

		break;

	case ITEM_STATES_OBTAINING:
		//アイテムが入手された状態なら

		//当たり判定を無効化
		isCollision = false;

		//半径が拡大するモードだったら拡大する処理を行う
		if (isExpantion) {
			//回転させる
			angle += ITEM_ROTATION_ANGLE_ADD;
			//プレイヤーとの距離を計算
			playerDistance += ITEM_ROTATION_ADD_RADIUS;
			//移動させる
			pos = Vec2(cosf(angle) * playerDistance + playerPos.x, sinf(angle) * playerDistance + playerPos.y);
			//プレイヤーとの距離が一定以上だったら半径を小さくする状態へ
			if (playerDistance >= ITEM_ROTATION_MAX_RADIUS) {
				isExpantion = false;
			}
		}
		//半径が縮小するモードだったら縮小する処理を行う
		else {
			//回転させる
			angle += ITEM_ROTATION_ANGLE_ADD * 2.0f;
			//プレイヤーとの距離を計算
			if (playerDistance > 0) {
				playerDistance -= ITEM_ROTATION_MINUS_RADIUS;
			}
			else {
				playerDistance = 0;
			}
			//移動させる
			pos = Vec2(cosf(angle) * playerDistance + playerPos.x, sinf(angle) * playerDistance + playerPos.y);
			//アルファ値を下げる
			alpha -= 10;
			//半径を小さくする
			radius -= 1;
			//半径かアルファ値が0以下になったらフラグを折る
			if (alpha <= 0 || radius <= 0) {
				isAlive = false;
			}
		}
		break;

	case ITEM_STATES_DISAPPEAR:
		//ほかのアイテムが入手されて消える状態なら

		//当たり判定を無効化
		isCollision = false;
		//アルファ値を小さくする
		alpha -= 10;
		//半径を小さくする
		radius -= 1;
		//半径かアルファ値が0以下になったらフラグを折る
		if (alpha <= 0 || radius <= 0) {
			isAlive = false;
		}
	}
}

void GaugeItem::Draw(int itemGraph, Vec2 scrollAmount)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

	DrawExtendGraph(pos.x - radius - scrollAmount.x, pos.y - radius - scrollAmount.y, pos.x + radius - scrollAmount.x, pos.y + radius - scrollAmount.y, itemGraph, TRUE);
	DrawCircle(pos.x - scrollAmount.x, pos.y - scrollAmount.y, radius + (radius * 0.3f), GetColor(0x78, 0xFF, 0xA9), FALSE, 3);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GaugeItem::Hit(Vec2 playerPos)
{
	//引数のplayerPosはいったん何なんだろう

	//当たり判定を無効化
	isCollision = false;
	//ステータスを変える
	states = ITEM_STATES_OBTAINING;
}

void GaugeItem::Delete()
{

	//当たり判定を無効化
	isCollision = false;
	states = ITEM_STATES_DISAPPEAR;
}
