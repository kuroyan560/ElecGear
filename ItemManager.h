#pragma once
#include "Item.h"
#include "Singleton.h"

class ItemManager : public Singleton<ItemManager> {
public:
	Item items[ITEM_MAX_COUNT];		//アイテム配列
	int itemGraph[2];				//アイテム画像の配列

private:
	friend Singleton<ItemManager>;
	ItemManager() {
		itemGraph[0] = LoadGraph("resource/waybullet_item.png");
		itemGraph[1] = LoadGraph("resource/machinegun_item.png");
	}

public:

	//初期化処理
	void Init();
	//生成処理
	void Generate(Vec2 playerPos);
	//更新処理
	void Update(Vec2 playerpos);
	//描画処理
	void Draw(Vec2 scrollAmount);

	//すべてのアイテムを消滅状態にする
	void AllDelete();
};