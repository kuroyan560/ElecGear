#pragma once
#include "GaugeItem.h"
#include "Singleton.h"

class GaugeItemManager : public Singleton<GaugeItemManager> {
public:
	GaugeItem items[ITEM_MAX_COUNT];		//�A�C�e���z��
	int itemGraph;							//�A�C�e���摜
	int generateTimer;						//��������܂ł̃^�C�}�[

private:
	friend Singleton<GaugeItemManager>;
	GaugeItemManager() {
		itemGraph = LoadGraph("resource/heal_item.png");
		generateTimer = 0;
	}

public:

	//����������
	void Init();
	//�X�V����
	void Update(Vec2 playerpos);
	//�`�揈��
	void Draw(Vec2 scrollAmount);

	//���ׂẴA�C�e�������ŏ�Ԃɂ���
	void AllDelete();
};