#include "GaugeItem.h"

GaugeItem::GaugeItem()
{
	pos.Init();
	radius = 0;
	alpha = 0;
	angle = 0;
	playerDistance = 0;
	states = 0;
	isAlive = false;
	isExpantion = false;
}

void GaugeItem::Init()
{
	isAlive = false;
	isCollision = false;
}

void GaugeItem::Generate()
{
	pos = Vec2(GetRand(MAP_RADIUS_2_TIMES) - MAP_RADIUS, GetRand(MAP_RADIUS_2_TIMES) - MAP_RADIUS);
	radius = ITEM_SIZE;
	alpha = 255;
	angle = 0;
	playerDistance = 0;
	deleteTimer = 0;
	states = ITEM_STATES_IDOL;
	isAlive = true;
	isExpantion = true;
	isCollision = true;
}

void GaugeItem::Update(Vec2 playerPos)
{

	//��Ԃɉ����ď����𕪂���
	switch (states) {
	case ITEM_STATES_IDOL:
		//�A�C�e����IDOL�̏ꍇ�͍폜�^�C�}�[�����Z

		deleteTimer++;
		if (deleteTimer >= GAUGEITEM_DELETE_TIMER) {
			deleteTimer = 0;
			states = ITEM_STATES_DISAPPEAR;
		}

		break;

	case ITEM_STATES_OBTAINING:
		//�A�C�e�������肳�ꂽ��ԂȂ�

		//�����蔻��𖳌���
		isCollision = false;

		//���a���g�傷�郂�[�h��������g�傷�鏈�����s��
		if (isExpantion) {
			//��]������
			angle += ITEM_ROTATION_ANGLE_ADD;
			//�v���C���[�Ƃ̋������v�Z
			playerDistance += ITEM_ROTATION_ADD_RADIUS;
			//�ړ�������
			pos = Vec2(cosf(angle) * playerDistance + playerPos.x, sinf(angle) * playerDistance + playerPos.y);
			//�v���C���[�Ƃ̋��������ȏゾ�����甼�a�������������Ԃ�
			if (playerDistance >= ITEM_ROTATION_MAX_RADIUS) {
				isExpantion = false;
			}
		}
		//���a���k�����郂�[�h��������k�����鏈�����s��
		else {
			//��]������
			angle += ITEM_ROTATION_ANGLE_ADD * 2.0f;
			//�v���C���[�Ƃ̋������v�Z
			if (playerDistance > 0) {
				playerDistance -= ITEM_ROTATION_MINUS_RADIUS;
			}
			else {
				playerDistance = 0;
			}
			//�ړ�������
			pos = Vec2(cosf(angle) * playerDistance + playerPos.x, sinf(angle) * playerDistance + playerPos.y);
			//�A���t�@�l��������
			alpha -= 10;
			//���a������������
			radius -= 1;
			//���a���A���t�@�l��0�ȉ��ɂȂ�����t���O��܂�
			if (alpha <= 0 || radius <= 0) {
				isAlive = false;
			}
		}
		break;

	case ITEM_STATES_DISAPPEAR:
		//�ق��̃A�C�e�������肳��ď������ԂȂ�

		//�����蔻��𖳌���
		isCollision = false;
		//�A���t�@�l������������
		alpha -= 10;
		//���a������������
		radius -= 1;
		//���a���A���t�@�l��0�ȉ��ɂȂ�����t���O��܂�
		if (alpha <= 0 || radius <= 0) {
			isAlive = false;
		}
	}
}

void GaugeItem::Draw(int itemGraph, Vec2 scrollAmount)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

	DrawExtendGraph(pos.x - radius - scrollAmount.x, pos.y - radius - scrollAmount.y, pos.x + radius - scrollAmount.x, pos.y + radius - scrollAmount.y, itemGraph, TRUE);
	DrawCircle(pos.x - scrollAmount.x, pos.y - scrollAmount.y, radius + (radius * 0.3f), GetColor(0x78, 0xFF, 0xA9), FALSE, 3);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GaugeItem::Hit(Vec2 playerPos)
{
	//������playerPos�͂������񉽂Ȃ񂾂낤

	//�����蔻��𖳌���
	isCollision = false;
	//�X�e�[�^�X��ς���
	states = ITEM_STATES_OBTAINING;
}

void GaugeItem::Delete()
{

	//�����蔻��𖳌���
	isCollision = false;
	states = ITEM_STATES_DISAPPEAR;
}
