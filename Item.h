#pragma once
#include "Vec2.h"

class Item{
public:
	Vec2 pos;						//座標
	float radius;					//半径
	float alpha;					//プレイヤーの周りをぐるぐる回転するときにだんだん薄くするために使用する
	float angle;					//プレイヤーの周りを回転するときの角度
	float playerDistance;			//プレイヤーの周りを回転するときに使うプレイヤーとの距離
	int states;						//アイテムが現在どういう状況下を表すステータス
	int id;							//どのアイテムかのID
	bool isAlive;					//生存しているか
	bool isCollision;				//当たり判定を行うかどうか
	bool isExpantion;				//半径が拡大するモードかどうか

public:
	//コンストラクタ
	Item();
	//初期化処理
	void Init();
	//生成処理
	void Generate(int itemID, Vec2 generatePos);
	//更新処理
	void Update(Vec2 playerPos);
	//描画処理
	void Draw(int itemGraph, Vec2 scrollAmount);
	//プレイヤーと衝突したときの処理
	void Hit(Vec2 playerPos);
	//ほかのアイテムがプレイヤーと衝突したときの処理
	void Delete();
};
