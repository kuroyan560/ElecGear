#pragma once
#include "GaugeItem.h"
#include "Singleton.h"

class GaugeItemManager : public Singleton<GaugeItemManager> {
public:
	GaugeItem items[ITEM_MAX_COUNT];		//アイテム配列
	int itemGraph;							//アイテム画像
	int generateTimer;						//生成するまでのタイマー

private:
	friend Singleton<GaugeItemManager>;
	GaugeItemManager() {
		itemGraph = LoadGraph("resource/heal_item.png");
		generateTimer = 0;
	}

public:

	//初期化処理
	void Init();
	//更新処理
	void Update(Vec2 playerpos);
	//描画処理
	void Draw(Vec2 scrollAmount);

	//すべてのアイテムを消滅状態にする
	void AllDelete();
};