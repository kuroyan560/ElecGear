#include "ItemManager.h"

void ItemManager::Init()
{
	for (int i = 0; i < ITEM_MAX_COUNT; ++i) {
		items[i].Init();
	}
}

void ItemManager::Generate(Vec2 playerPos)
{
	//画面上にあるほかのアイテムを全部削除モードにしてから生成
	for(int i = 0; i < ITEM_MAX_COUNT; ++i){
		if(items[i].isAlive == false) continue;

		items[i].Delete();
	}

	//プレイヤーの位置から一定以上離れた位置に生成
	Vec2 generatePos = Vec2(playerPos.x + (cosf(DX_TWO_PI_F / (float)GetRand(10)) * 300.0f),
		playerPos.y + (sinf(DX_TWO_PI_F / (float)GetRand(10)) * 300.0f));

	//二種類のアイテムを生成
	for (int i = 0; i < ITEM_MAX_COUNT; ++i) {
		if (items[i].isAlive == true) continue;

		items[i].Generate(ITEM_ID_MACHINEGUN, generatePos);
		break;
	}
	for (int i = 0; i < ITEM_MAX_COUNT; ++i) {
		if (items[i].isAlive == true) continue;

		items[i].Generate(ITEM_ID_WAY, Vec2(generatePos.x + ITEM_SIZE * 3.0f, generatePos.y));
		break;
	}
}

void ItemManager::Update(Vec2 playerPos)
{
	//更新処理
	for (int i = 0; i < ITEM_MAX_COUNT; ++i) {
		if (items[i].isAlive == false) continue;

		items[i].Update(playerPos);
	}
}

void ItemManager::Draw(Vec2 scrollAmount)
{
	//描画処理
	for (int i = 0; i < ITEM_MAX_COUNT; ++i) {
		if (items[i].isAlive == false) continue;

		items[i].Draw(itemGraph[items[i].id], scrollAmount);
	}
}

void ItemManager::AllDelete()
{
	//すべてのアイテムを削除状態にする
	for (int i = 0; i < ITEM_MAX_COUNT; ++i) {
		if (items[i].isAlive == false) continue;

		items[i].Delete();;
	}
}
