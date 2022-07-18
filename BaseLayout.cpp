#include "BaseLayout.h"
#include"Vec2.h"
#include"Player.h"
#include"Constants.h"

const float MAX_METER_ANGLE = 1.207f;
const float MIN_METER_ANGLE = -1.122f;

void BaseLayout::Initialize(Player* player)
{
	meterIndicaterAngle = MIN_METER_ANGLE;
	this->player = player;
}

void BaseLayout::MeterUpdate(float playerMoveLength)
{
	/*-----���[�^�[�̐j�̊p�x�����߂�-----*/
	float playerSpeedPer = playerMoveLength / (PLAYER_MAX_SPEED * 0.98);
	//���[�^�[���ł����̎��̊p�x��-1.122,�ł��E�̎��̊p�x��1.207�Ȃ̂ŁA��U�l�͈̔͂�0~2.329�ɂ���
	playerSpeedPer = playerSpeedPer * 2.329f;
	//-1.122�������Ēl�͈̔͂𐳂����ݒ肷��
	playerSpeedPer -= 1.122f;
	meterIndicaterAngle = playerSpeedPer;
}

void BaseLayout::DrawHexagon()
{
	//�w�i�̕`�揈��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	for (int width = 0; width < BACKGROUND_TILE_COUNT_X; ++width) {
		//���̐��������������炽�Ă��������炷
		float zure = 0;
		if (width % 2 == 0) {
			zure = BACKGROUND_TILE_SIZE.y / 2.0f;
		}
		for (int height = 0; height < BACKGROUND_TILE_COUNT_Y; ++height) {
			//�`�悷��
			float drawX = BACKGROUND_LEFTUP_POS.x + (width * (BACKGROUND_TILE_SIZE.x - 35.0f)) - player->scrollAmount.x;
			float drawY = BACKGROUND_LEFTUP_POS.y + (height * BACKGROUND_TILE_SIZE.y) + zure - player->scrollAmount.y;
			DrawGraph(drawX, drawY, backGroundGraph, true);
		}
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void BaseLayout::DrawScreenFrame(float playerMoveLength)
{
	//�A
	DrawGraph(0, 0, screenShadowGraph, TRUE);

	//�X�N���[���t���[���摜�̕`�揈��
	//DrawExtendGraph(0, 0, WIN_WIDTH, WIN_HEIGHT, screenFrameGraph, TRUE);
	DrawGraph(-50, -50, screenFrameGraph, TRUE);
	DrawGraph(WIN_WIDTH - 330, WIN_HEIGHT - 180, meterGraph, TRUE);
	//���[�^�[�̐j�̕`��
	if (playerMoveLength / PLAYER_MAX_SPEED < 0.95f) {
		DrawRotaGraph(WIN_WIDTH - 165, WIN_HEIGHT - 30, 1, meterIndicaterAngle, meterIndicaterGraph, TRUE);
	}
	else {
		DrawRotaGraph(WIN_WIDTH - 165, WIN_HEIGHT - 30, 1, meterIndicaterAngle + ((GetRand(100) / 1000.0f) - 0.05f), meterIndicaterGraph, TRUE);
	}
}
