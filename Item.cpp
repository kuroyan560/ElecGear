#include "Item.h"

Item::Item()
{
	pos.Init();
	radius = 0;
	alpha = 0;
	angle = 0;
	playerDistance = 0;
	states = 0;
	id = 0;
	isAlive = false;
	isExpantion = false;
}

void Item::Init()
{
	isAlive = false;
	isCollision = false;
}

void Item::Generate(int itemID, Vec2 generatePos)
{
	pos = generatePos;
	radius = ITEM_SIZE;
	alpha = 255;
	angle = 0;
	playerDistance = 0;
	states = ITEM_STATES_IDOL;
	id = itemID;
	isAlive = true;
	isExpantion = true;
	isCollision = true;
}

void Item::Update(Vec2 playerPos)
{

	//状態に応じて処理を分ける
	switch (states) {
	case ITEM_STATES_IDOL:
		//アイテムがIDOLの場合は何も処理を行わない

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

void Item::Draw(int itemGraph, Vec2 scrollAmount)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

	DrawExtendGraph(pos.x - radius - scrollAmount.x, pos.y - radius - scrollAmount.y, pos.x + radius - scrollAmount.x, pos.y + radius - scrollAmount.y, itemGraph, TRUE);
	DrawCircle(pos.x - scrollAmount.x, pos.y - scrollAmount.y, radius, GetColor(0x78, 0xFF, 0xA9), FALSE, 3);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Item::Hit(Vec2 playerPos)
{
	//引数のplayerPosはいったん何なんだろう

	//当たり判定を無効化
	isCollision = false;
	//ステータスを変える
	states = ITEM_STATES_OBTAINING;
}

void Item::Delete()
{

	//当たり判定を無効化
	isCollision = false;
	states = ITEM_STATES_DISAPPEAR;
}
