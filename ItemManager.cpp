#include "ItemManager.h"

void ItemManager::Init()
{
	for (int i = 0; i < ITEM_MAX_COUNT; ++i) {
		items[i].Init();
	}
}

void ItemManager::Generate(Vec2 playerPos)
{
	//��ʏ�ɂ���ق��̃A�C�e����S���폜���[�h�ɂ��Ă��琶��
	for(int i = 0; i < ITEM_MAX_COUNT; ++i){
		if(items[i].isAlive == false) continue;

		items[i].Delete();
	}

	//�v���C���[�̈ʒu������ȏ㗣�ꂽ�ʒu�ɐ���
	Vec2 generatePos = Vec2(playerPos.x + (cosf(DX_TWO_PI_F / (float)GetRand(10)) * 300.0f),
		playerPos.y + (sinf(DX_TWO_PI_F / (float)GetRand(10)) * 300.0f));

	//���ނ̃A�C�e���𐶐�
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
	//�X�V����
	for (int i = 0; i < ITEM_MAX_COUNT; ++i) {
		if (items[i].isAlive == false) continue;

		items[i].Update(playerPos);
	}
}

void ItemManager::Draw(Vec2 scrollAmount)
{
	//�`�揈��
	for (int i = 0; i < ITEM_MAX_COUNT; ++i) {
		if (items[i].isAlive == false) continue;

		items[i].Draw(itemGraph[items[i].id], scrollAmount);
	}
}

void ItemManager::AllDelete()
{
	//���ׂẴA�C�e�����폜��Ԃɂ���
	for (int i = 0; i < ITEM_MAX_COUNT; ++i) {
		if (items[i].isAlive == false) continue;

		items[i].Delete();;
	}
}
