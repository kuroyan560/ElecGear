#include "GaugeItemManager.h"

void GaugeItemManager::Init()
{
	for (int i = 0; i < ITEM_MAX_COUNT; ++i) {
		items[i].Init();
	}
}

void GaugeItemManager::Update(Vec2 playerPos)
{
	//��������
	generateTimer++;
	if(generateTimer >= GAUGEITEM_GENERATE_TIMER){
		generateTimer = 0;

		for(int i = 0; i < ITEM_MAX_COUNT; ++i){
			if(items[i].isAlive == true) continue;

			items[i].Generate();
			break;
		}
	}

	//�X�V����
	for (int i = 0; i < ITEM_MAX_COUNT; ++i) {
		if (items[i].isAlive == false) continue;

		items[i].Update(playerPos);
	}
}

void GaugeItemManager::Draw(Vec2 scrollAmount)
{
	//�`�揈��
	for (int i = 0; i < ITEM_MAX_COUNT; ++i) {
		if (items[i].isAlive == false) continue;

		items[i].Draw(itemGraph, scrollAmount);
	}
}

void GaugeItemManager::AllDelete()
{
	//���ׂẴA�C�e�����폜��Ԃɂ���
	for (int i = 0; i < ITEM_MAX_COUNT; ++i) {
		if (items[i].isAlive == false) continue;

		items[i].Delete();;
	}
}