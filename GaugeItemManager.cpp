#include "GaugeItemManager.h"

void GaugeItemManager::Init()
{
	for (int i = 0; i < ITEM_MAX_COUNT; ++i) {
		items[i].Init();
	}
}

void GaugeItemManager::Update(Vec2 playerPos)
{
	//生成処理
	generateTimer++;
	if(generateTimer >= GAUGEITEM_GENERATE_TIMER){
		generateTimer = 0;

		for(int i = 0; i < ITEM_MAX_COUNT; ++i){
			if(items[i].isAlive == true) continue;

			items[i].Generate();
			break;
		}
	}

	//更新処理
	for (int i = 0; i < ITEM_MAX_COUNT; ++i) {
		if (items[i].isAlive == false) continue;

		items[i].Update(playerPos);
	}
}

void GaugeItemManager::Draw(Vec2 scrollAmount)
{
	//描画処理
	for (int i = 0; i < ITEM_MAX_COUNT; ++i) {
		if (items[i].isAlive == false) continue;

		items[i].Draw(itemGraph, scrollAmount);
	}
}

void GaugeItemManager::AllDelete()
{
	//すべてのアイテムを削除状態にする
	for (int i = 0; i < ITEM_MAX_COUNT; ++i) {
		if (items[i].isAlive == false) continue;

		items[i].Delete();;
	}
}