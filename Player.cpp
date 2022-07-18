#include "Player.h"
#include"Easing.h"
#include"SoundEmitter.h"

#define PLAYER_ARM_POS -43 * CAMERA_RATE
#define ARM_ANGLE(angle) angle + DX_PI_F / 2.0f
#define REACTION_AMOUNT 20

//�{�^�������󋵃T�[�N��
#define MAX_SLASH_CIRCLE_RADIUS_MAX 128
#define MAX_SLASH_CIRCLE_THICKNESS_MAX 16

//�a���Q�[�W
#define SLASH_GAUGE_RADIUS 64
#define SLASH_GAUGE_TOTAL_TIME 20
#define NON_SLASH_TIMER 100

Player::Player()
{

	/*--�v���C���[�̉摜�����[�h--*/
	playerHeadGraph = LoadGraph("resource/player_character_head.png");
	playerArmGraph = LoadGraph("resource/player_character_arm.png");
	slashStartGraph = LoadGraph("resource/slash_start.png");
	LoadDivGraph("resource/slash.png", 5, 5, 1, 450, 450, slashGraph);
	slashEndGraph = LoadGraph("resource/slash_end.png");

	itemGetGraph = LoadGraph("resource/buffEffect.png");

	damagedSE = SoundEmitter::Instance()->LoadSound("resource/playerDamagedSE.mp3", 150);

	slashSE[0] = SoundEmitter::Instance()->LoadSound("resource/slashSE_3.mp3", 130);
	slashSE[1] = SoundEmitter::Instance()->LoadSound("resource/slashSE_2.mp3", 130);
	slashSE[2] = SoundEmitter::Instance()->LoadSound("resource/slashSE_1.mp3", 130);

	maxSlashSE = SoundEmitter::Instance()->LoadSound("resource/maxSlash.mp3", 200);
	slashRangeGraph = LoadGraph("resource/slash_range.png");
	slashGaugeGraph = LoadGraph("resource/slash_gauge.png");

	slashGaugeHealSE[0] = SoundEmitter::Instance()->LoadSound("resource/slash_gauge_heal_1.mp3",150);
	slashGaugeHealSE[1] = SoundEmitter::Instance()->LoadSound("resource/slash_gauge_heal_2.mp3",150);
	slashGaugeHealSE[2] = SoundEmitter::Instance()->LoadSound("resource/slash_gauge_heal_3.mp3",150);
}

void Player::Init()
{
	/*--��{�I�ȏ���������--*/
	hp = PLAYER_HP;
	pos = MAP_CENTER;
	pos.x += 150;
	size = 64 * CAMERA_RATE;
	speed = 0;
	vel = {};
	playerAngle = 0;
	hitTimer = 0;
	scrollAmount.Init();
	scrollAmount.x += 150;
	bulletStates = BULLET_GENERATE_STATES_NORMAL;
	//�����ɕK�v�Ȋe�ϐ���������
	reactionTime = 0;
	reactionTimer = 0;
	armReactionPos = 0;
	isReaction = false;
	//�a���ɕK�v�Ȋe�ϐ���������
	isSlash = false;
	slashFrame = 0;
	slashRadius = SLASH_RADIUS * CAMERA_RATE;
	slashCooltime = 0;
	canSlash = true;
	//slashDeathCount = 0;
	slashInvincibleTime = 0;
	isSlashAnime = false;
	slashAnimeNumber = 0;
	slashAnimeTimer = 0;
	slashMoveBuff.Init();

	move = { 0.0f,0.0f };
	moveBuff.Init();
	orbit.Init();

	slashGauge = CAN_SLASH_MAX_COUNT;
	slashGaugeTimer = 0;
	slashButtonFrame = 0;
	buttonInputRate = 0.0f;

	for (int i = 0; i < CAN_SLASH_MAX_COUNT; ++i) {
		slashGaugeDrawTimer[i] = SLASH_GAUGE_TOTAL_TIME;
	}
	nonSlashTimer = 0;
}

void Player::Update(bool isShoot, int shootDelay)
{
	slashFinishTrigger = false;

	if (0 < itemGetTimer) {
		itemGetTimer--;
	}

	for (int i = 0; i < slashGauge; ++i) {
		if (slashGaugeDrawTimer[i] < SLASH_GAUGE_TOTAL_TIME) {
			slashGaugeDrawTimer[i]++;
		}
	}

	if (slashGauge == 3 && nonSlashTimer < NON_SLASH_TIMER) {
		nonSlashTimer++;
		if (nonSlashTimer == NON_SLASH_TIMER) {
			for (int i = 0; i < slashGauge; ++i) {
				slashGaugeDrawTimer[i] = 0;
			}
		}
	}

	//hittimer��1�ȏゾ�����珈�����s��Ȃ�
	if (hitTimer <= PLAYER_HIT_TIMER - PLAYER_HIT_TIMER / 4.0f) {

		/*-----�v���C���[�̈ړ����x�̍X�V����-----*/
		if (Input::Controller()) {	//�R���g���[���[
			speed = (float)PLAYER_MAX_SPEED * (int)Input::GetJoyRightTrigger();
		}
		else { //�}�E�X
			if (Input::isMouseBottom(MOUSE_INPUT_LEFT)) {
				speed = (float)PLAYER_MAX_SPEED;
			}
			else {
				speed = 0.0f;
			}
			//�}�E�X�z�C�[���̉�]�ʂ𓾂�
			//int mouseRot = GetMouseWheelRotVol();
			////�z�C�[�����v���X�����ɉ�]���Ă��āA���@�̈ړ����x���ő�l��菬����������
			//if (mouseRot > 0 && speed < PLAYER_MAX_SPEED) {
			//	speed = PLAYER_MAX_SPEED;
			//	//speed += PLAYER_CHANGE_SPEED;
			//	////�ő�l�𒴂��Ȃ��悤�ɂ���
			//	//if (speed > PLAYER_MAX_SPEED) {
			//	//	speed = PLAYER_MAX_SPEED;
			//	//}
			//}
			////�z�C�[�����}�C�i�X�����ɉ�]���Ă��āA0�ȏゾ������
			//if (mouseRot < 0 && speed > 0) {
			//	speed = 0;
			//	//speed -= PLAYER_CHANGE_SPEED;
			//	////0�����ɂȂ�Ȃ��悤�ɂ���
			//	//if (speed < 0) {
			//	//	speed = 0;
			//	//}
			//}
		}

		/*-----�v���C���[�̊p�x�̍X�V����-----*/
		if (Input::Controller()) {
			Vec2 leftStick = Input::GetJoyLeftStick();
			if (leftStick.x || leftStick.y) {
				vel = leftStick;
				vel.y *= -1;	//Y���t�Ȃ̂Ŕ��]
				vel.Normalize();

				//�v���C���[�̊p�x���X�V
				playerAngle = atan2(vel.y, vel.x);
			}
		}
		else {
				//�}�E�X���W���擾
				Vec2 mousePos = Input::GetMousePos();
				//�}�E�X���W�ɃX�N���[���l�𑫂�
				mousePos += scrollAmount;
				if (size / 2 < (mousePos - pos).Length()) {
					//�}�E�X���W�ƃv���C���[���W����p�x���擾
					vel = Vec2(mousePos.x - pos.x, mousePos.y - pos.y);
					vel.Normalize();

					//�v���C���[�̊p�x���X�V
					playerAngle = atan2(vel.y, vel.x);
				}
		}

		/*-----�v���C���[�̈ړ��̍X�V����-----*/
		/*Vec2 toMove = Vec2(vel.x * speed, vel.y * speed);
		move.x = Easing::Lerp(move.x, toMove.x, 0.05f);
		move.y = Easing::Lerp(move.y, toMove.y, 0.05f);
		pos += move;
		scrollAmount.x += move.x;
		scrollAmount.y += move.y;*/
		Vec2 toMove = Vec2(vel.x * (speed), vel.y * (speed));
		float lerpRate = Easing::Lerp(0.05f, 0.2f, speed / PLAYER_MAX_SPEED);
		move.x = Easing::Lerp(move.x, toMove.x, lerpRate);
		move.y = Easing::Lerp(move.y, toMove.y, lerpRate);
		pos += move;

		moveBuff += move;
		Vec2 buff = {};
		buff.x = Easing::Lerp(buff.x, moveBuff.x, 0.2f);
		buff.y = Easing::Lerp(buff.y, moveBuff.y, 0.2f);
		scrollAmount += buff;
		moveBuff -= buff;

		bool slashInput = Input::Controller() ? Input::isJoyBottom(XINPUT_BUTTON_A) : Input::isKey(KEY_INPUT_SPACE);
		if (slashInput) {
			if (nonSlashTimer == NON_SLASH_TIMER) {
				for (int i = 0; i < slashGauge; ++i) {
					slashGaugeDrawTimer[i] = 0;
				}
			}
			nonSlashTimer = 0;
		}
		/*--�a���̍X�V����--*/
		if (!isSlash) {
			//�a���Q�[�W
			if (slashGauge < CAN_SLASH_MAX_COUNT) {
				slashGaugeTimer++;
				if (slashGaugeTimer == SLASH_GAUGE_HEAL_TIME) {
					slashGaugeDrawTimer[slashGauge] = 0;
					SoundEmitter::Instance()->Play(slashGaugeHealSE[slashGauge], DX_PLAYTYPE_BACK);
					slashGauge++;	//�a���Q�[�W��
					slashGaugeTimer = 0;
				}
			}
			//�a�����͏���
			if (slashGauge && hitTimer <= 0 && slashInput && slashButtonFrame < SLASH_BUTTON_FRAME_MAX){
				slashButtonFrame++;
				if (slashButtonFrame == SLASH_BUTTON_FRAME_MAX) {
					SoundEmitter::Instance()->Play(maxSlashSE, DX_PLAYTYPE_BACK);
				}
				buttonInputRate = (float)slashButtonFrame / (float)SLASH_BUTTON_FRAME_MAX;
				if (1.0f < buttonInputRate)buttonInputRate = 1.0f;
			}
		}

		slashInput = Input::Controller() ? !Input::isJoyBottom(XINPUT_BUTTON_A) : !Input::isKey(KEY_INPUT_SPACE);
		//�a���̐�������
		if (slashInput && slashButtonFrame && canSlash) {
			isSlash = true;
			slashFrame = 0;
			canSlash = false;
			slashCooltime = 0;
			//slashDeathCount = 0;
			slashInvincibleTime = SLASH_FRAME + 5;	//�a������+n�t���[���𖳓G���Ԃɂ���
			//�A�j���[�V�����ɕK�v�Ȋe�ϐ�������������
			isSlashAnime = true;
			slashAnimeTimer = 0;
			slashAnimeNumber = 0;

			slashGauge--;
			slashButtonFrame = 0;

			if (CheckSoundMem(slashSE[2])) {
				SoundEmitter::Instance()->Play(slashSE[0], DX_PLAYTYPE_BACK);
			}
			else if (CheckSoundMem(slashSE[1])) {
				SoundEmitter::Instance()->Play(slashSE[2], DX_PLAYTYPE_BACK);
			}
			else if(CheckSoundMem(slashSE[0])) {
				SoundEmitter::Instance()->Play(slashSE[1], DX_PLAYTYPE_BACK);
			}
			else {
				SoundEmitter::Instance()->Play(slashSE[0], DX_PLAYTYPE_BACK);
			}
		}
		//�a���̍X�V�������s��
		if (isSlash) {
			slashFrame++;
			//���t���[���ɒB������t���O��܂�
			if (slashFrame >= SLASH_FRAME) {
				isSlash = false;
				//��萔�|���Ă����瑱���Ďa�����o����悤�ɂ���
				/*if (slashDeathCount >= SLASH_DEATH_COUNT) {
					canSlash = true;
					slashDeathCount = 0;
				}*/
				canSlash = true;
				slashFinishTrigger = true;
			}
			//�a�����̈ړ����� �ړ���9F�܂�
			if (slashFrame < 9) {
				float moveAmount = (SLASH_MOVE_AMOUNT_MAX - SLASH_MOVE_AMOUNT_MIN) * buttonInputRate + SLASH_MOVE_AMOUNT_MIN;
				slashMoveBuff += Vec2(-cosf(playerAngle + DX_PI_F/* / 2.0f*/) * moveAmount,
					-sinf(playerAngle + DX_PI_F /*/ 2.0f*/) * moveAmount);
			}
		}
		//�a���̃N�[���^�C���̍X�V
		if (!isSlash && !canSlash) {
			slashCooltime++;
			//�N�[���^�C������萔�ɒB������a���\��Ԃɂ���
			if (slashCooltime >= SLASH_COOLTIME) {
				canSlash = true;
			}
		}
		//�a���̖��G���Ԃ̍X�V
		if (slashInvincibleTime > 0) {
			slashInvincibleTime--;
		}

		//�a���A�j���[�V�����̍X�V
		if (isSlashAnime) {
			slashAnimeTimer++;
			if (slashAnimeTimer >= 3) {
				slashAnimeTimer = 0;
				slashAnimeNumber++;
				if (slashAnimeNumber > 5) {
					isSlashAnime = false;
				}
			}
		}

		//�a�����Ɉړ�����������
		Vec2 slashBuff = Vec2(slashMoveBuff.x / 5.0f, slashMoveBuff.y / 5.0f);
		//�ړ�������
		pos += slashBuff;
		moveBuff += slashBuff;
		slashMoveBuff -= slashBuff;
	}

	//�O���̍X�V
	if (isSlash) {
		Vec2 slashBuff = Vec2(slashMoveBuff.x / 5.0f, slashMoveBuff.y / 5.0f);
		orbit.Update(PLAYER_ORBIT_EMIT_MOVE_LENGTH < move.Length(), pos, ARM_ANGLE(atan2(slashBuff.y, slashBuff.x)));
	}
	else {
		orbit.Update(PLAYER_ORBIT_EMIT_MOVE_LENGTH < move.Length(), pos, ARM_ANGLE(atan2(move.y, move.x)));
	}

	if (hitTimer > 0) {
		hitTimer--;
	}

	//�Q�[�W���}�b�N�X�ɂȂ��Ă�����e��way�e�ɂ���
	//if (gauge >= PLAYER_GAUGE_MAX) {
	//	gauge = 0;
	//	//bulletStates = BULLET_GENERATE_STATES_WAY;
	//}

	/*--�e�������Ă����甽���̏������s��--*/
	if (isShoot) {
		isReaction = true;
		reactionTime = shootDelay;
		armReactionPos = REACTION_AMOUNT;
	}
	//�����̍X�V����
	if (isReaction) {
		reactionTimer++;
		//���A�N�V�����^�C�}�[�����A�N�V�����̍��v���Ԃ̔�����菬����������A���ɉ����鏈��������
		armReactionPos -= armReactionPos * (reactionTimer / reactionTime);

		//���A�N�V�����^�C�}�[�����A�N�V�������v���Ԉȏ�ɂȂ����珉����
		if (reactionTimer >= reactionTime) {
			isReaction = false;
			reactionTimer = 0;
			armReactionPos = 0;
		}
	}
}

void Player::Draw()
{
	orbit.Draw(scrollAmount);

	if (hitTimer > 0) {
		SetDrawBright(0xFF, 0x0F, 0x0F);
	}

	//����~
	//if (isSlash) {
	//	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	//	DrawCircle(pos.x - scrollAmount.x, pos.y - scrollAmount.y, SLASH_RADIUS, GetColor(255, 0, 0));
	//	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//}

	//�a���A�j���t���O�������Ă�����
	if (isSlashAnime) {
			//�r�[���T�[�x�����o��
			DrawRotaGraph3(pos.x - scrollAmount.x,
				pos.y - scrollAmount.y,
				225 , 225 ,			//�v���C���[����]������摜��ł̒��S���W
				CAMERA_RATE, CAMERA_RATE,
				playerAngle + DX_PI_F / 2.0f,
				slashGraph[slashAnimeNumber], true);
	}
	else if (isSlash && slashFrame < SLASH_FRAME)
	{
		//�r�[���T�[�x���]�C
		DrawRotaGraph3(pos.x - scrollAmount.x,
			pos.y - scrollAmount.y,
			225 , 225 ,			//�v���C���[����]������摜��ł̒��S���W
			CAMERA_RATE, CAMERA_RATE,
			playerAngle + DX_PI_F / 2.0f,
			slashEndGraph, true);
	}
	else if (0 < slashButtonFrame)
	{
		//���~�`��
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 130);
		DrawRotaGraph(pos.x - scrollAmount.x, pos.y - scrollAmount.y, CAMERA_RATE, ARM_ANGLE(playerAngle), slashRangeGraph, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		//�r�[���T�[�x�������o��
		DrawRotaGraph3(pos.x - scrollAmount.x,
			pos.y - scrollAmount.y,
			225, 225,			//�v���C���[����]������摜��ł̒��S���W
			CAMERA_RATE, CAMERA_RATE,
			playerAngle + DX_PI_F / 2.0f,
			slashStartGraph, true);
	}
	//�ʏ��Ԃ�������
	else {
		DrawRotaGraph(pos.x - scrollAmount.x + (cosf(ARM_ANGLE(playerAngle)) * PLAYER_ARM_POS) + (cosf(ARM_ANGLE(playerAngle)) * armReactionPos),
			pos.y - scrollAmount.y + (sinf(ARM_ANGLE(playerAngle)) * PLAYER_ARM_POS) + (sinf(ARM_ANGLE(playerAngle)) * armReactionPos),
			CAMERA_RATE, playerAngle, playerArmGraph, true);
		//DrawRotaGraph2(pos.x - scrollAmount.x + cosf(ARM_ANGLE(playerAngle)) * armReactionPos,
		//	pos.y - scrollAmount.y + sinf(ARM_ANGLE(playerAngle)) * armReactionPos,
		//	24.5f, 46,			//�v���C���[����]������摜��ł̒��S���W
		//	1, playerAngle, playerArmGraph, TRUE);
	}
	DrawRotaGraph(pos.x - scrollAmount.x, pos.y - scrollAmount.y, CAMERA_RATE, ARM_ANGLE(playerAngle), playerHeadGraph, true);

	SetDrawBright(0xFF, 0xFF, 0xFF);

	//�{�^�������󋵃T�[�N��
	if (0 < slashButtonFrame && slashButtonFrame < SLASH_BUTTON_FRAME_MAX) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, Easing::QuadIn(slashButtonFrame, SLASH_BUTTON_FRAME_MAX, 0, 200));
		DrawCircle(pos.x - scrollAmount.x, pos.y - scrollAmount.y,
			Easing::QuartIn(slashButtonFrame, SLASH_BUTTON_FRAME_MAX, MAX_SLASH_CIRCLE_RADIUS_MAX, 0), GetColor(120, 255, 169),
			false, Easing::QuadIn(slashButtonFrame, SLASH_BUTTON_FRAME_MAX, 0, MAX_SLASH_CIRCLE_THICKNESS_MAX));
	}

	if (itemGetTimer) {
		SetDrawBlendMode(DX_BLENDMODE_ADD, Easing::CubicIn(ITEM_GET_TOTAL_TIME - itemGetTimer, itemGetTimer, 255, 0));
		DrawRotaGraph(pos.x - scrollAmount.x, pos.y - scrollAmount.y, CAMERA_RATE, 0.0f, itemGetGraph, true);
	}

	for (int i = 0; i < slashGauge; ++i) {
		if (nonSlashTimer == NON_SLASH_TIMER)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, Easing::CircIn(slashGaugeDrawTimer[i], SLASH_GAUGE_TOTAL_TIME, 255, 0));
			float angle = (PI * 2 / 3.0f) * i - PI / 2;
			DrawRotaGraph(pos.x - scrollAmount.x + SLASH_GAUGE_RADIUS * cos(angle), pos.y - scrollAmount.y + SLASH_GAUGE_RADIUS * sin(angle),
				Easing::BackIn(slashGaugeDrawTimer[i], SLASH_GAUGE_TOTAL_TIME, 1.0f, 0.0f) * CAMERA_RATE, angle + PI / 2, slashGaugeGraph, true);
		}
		else
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, Easing::ExpOut(slashGaugeDrawTimer[i], SLASH_GAUGE_TOTAL_TIME, 0, 255));
			float angle = (PI * 2 / 3.0f) * i - PI / 2;
			DrawRotaGraph(pos.x - scrollAmount.x + SLASH_GAUGE_RADIUS * cos(angle), pos.y - scrollAmount.y + SLASH_GAUGE_RADIUS * sin(angle),
				Easing::BackOut(slashGaugeDrawTimer[i], SLASH_GAUGE_TOTAL_TIME, 0, 1.0f) * CAMERA_RATE, angle + PI / 2, slashGaugeGraph, true);
		}
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Player::Hit()
{
	hitTimer = PLAYER_HIT_TIMER;
	SoundEmitter::Instance()->Play(damagedSE, DX_PLAYTYPE_BACK);
	slashButtonFrame = 0;
	hp -= PLAYER_HP_DAMAGE;
	if (hp < 0)hp = 0;
}

void Player::EmitItemGetEffect()
{
	itemGetTimer = ITEM_GET_TOTAL_TIME;
	hp += PLAYER_HP_ITEM_HEAL;
	if (PLAYER_HP < hp)hp = PLAYER_HP;
}
