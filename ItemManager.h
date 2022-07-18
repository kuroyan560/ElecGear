#pragma once
#include "Item.h"
#include "Singleton.h"

class ItemManager : public Singleton<ItemManager> {
public:
	Item items[ITEM_MAX_COUNT];		//�A�C�e���z��
	int itemGraph[2];				//�A�C�e���摜�̔z��

private:
	friend Singleton<ItemManager>;
	ItemManager() {
		itemGraph[0] = LoadGraph("resource/waybullet_item.png");
		itemGraph[1] = LoadGraph("resource/machinegun_item.png");
	}

public:

	//����������
	void Init();
	//��������
	void Generate(Vec2 playerPos);
	//�X�V����
	void Update(Vec2 playerpos);
	//�`�揈��
	void Draw(Vec2 scrollAmount);

	//���ׂẴA�C�e�������ŏ�Ԃɂ���
	void AllDelete();
};