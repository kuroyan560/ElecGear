#include "Player.h"
#include"Easing.h"
#include"SoundEmitter.h"

#define PLAYER_ARM_POS -43 * CAMERA_RATE
#define ARM_ANGLE(angle) angle + DX_PI_F / 2.0f
#define REACTION_AMOUNT 20

//ボタン押下状況サークル
#define MAX_SLASH_CIRCLE_RADIUS_MAX 128
#define MAX_SLASH_CIRCLE_THICKNESS_MAX 16

//斬撃ゲージ
#define SLASH_GAUGE_RADIUS 64
#define SLASH_GAUGE_TOTAL_TIME 20
#define NON_SLASH_TIMER 100

Player::Player()
{

	/*--プレイヤーの画像をロード--*/
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
	/*--基本的な情報を初期化--*/
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
	//反動に必要な各変数を初期化
	reactionTime = 0;
	reactionTimer = 0;
	armReactionPos = 0;
	isReaction = false;
	//斬撃に必要な各変数を初期化
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

	//hittimerが1以上だったら処理を行わない
	if (hitTimer <= PLAYER_HIT_TIMER - PLAYER_HIT_TIMER / 4.0f) {

		/*-----プレイヤーの移動速度の更新処理-----*/
		if (Input::Controller()) {	//コントローラー
			speed = (float)PLAYER_MAX_SPEED * (int)Input::GetJoyRightTrigger();
		}
		else { //マウス
			if (Input::isMouseBottom(MOUSE_INPUT_LEFT)) {
				speed = (float)PLAYER_MAX_SPEED;
			}
			else {
				speed = 0.0f;
			}
			//マウスホイールの回転量を得る
			//int mouseRot = GetMouseWheelRotVol();
			////ホイールがプラス方向に回転していて、自機の移動速度が最大値より小さかったら
			//if (mouseRot > 0 && speed < PLAYER_MAX_SPEED) {
			//	speed = PLAYER_MAX_SPEED;
			//	//speed += PLAYER_CHANGE_SPEED;
			//	////最大値を超えないようにする
			//	//if (speed > PLAYER_MAX_SPEED) {
			//	//	speed = PLAYER_MAX_SPEED;
			//	//}
			//}
			////ホイールがマイナス方向に回転していて、0以上だったら
			//if (mouseRot < 0 && speed > 0) {
			//	speed = 0;
			//	//speed -= PLAYER_CHANGE_SPEED;
			//	////0未満にならないようにする
			//	//if (speed < 0) {
			//	//	speed = 0;
			//	//}
			//}
		}

		/*-----プレイヤーの角度の更新処理-----*/
		if (Input::Controller()) {
			Vec2 leftStick = Input::GetJoyLeftStick();
			if (leftStick.x || leftStick.y) {
				vel = leftStick;
				vel.y *= -1;	//Yが逆なので反転
				vel.Normalize();

				//プレイヤーの角度も更新
				playerAngle = atan2(vel.y, vel.x);
			}
		}
		else {
				//マウス座標を取得
				Vec2 mousePos = Input::GetMousePos();
				//マウス座標にスクロール値を足す
				mousePos += scrollAmount;
				if (size / 2 < (mousePos - pos).Length()) {
					//マウス座標とプレイヤー座標から角度を取得
					vel = Vec2(mousePos.x - pos.x, mousePos.y - pos.y);
					vel.Normalize();

					//プレイヤーの角度も更新
					playerAngle = atan2(vel.y, vel.x);
				}
		}

		/*-----プレイヤーの移動の更新処理-----*/
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
		/*--斬撃の更新処理--*/
		if (!isSlash) {
			//斬撃ゲージ
			if (slashGauge < CAN_SLASH_MAX_COUNT) {
				slashGaugeTimer++;
				if (slashGaugeTimer == SLASH_GAUGE_HEAL_TIME) {
					slashGaugeDrawTimer[slashGauge] = 0;
					SoundEmitter::Instance()->Play(slashGaugeHealSE[slashGauge], DX_PLAYTYPE_BACK);
					slashGauge++;	//斬撃ゲージ回復
					slashGaugeTimer = 0;
				}
			}
			//斬撃入力処理
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
		//斬撃の生成処理
		if (slashInput && slashButtonFrame && canSlash) {
			isSlash = true;
			slashFrame = 0;
			canSlash = false;
			slashCooltime = 0;
			//slashDeathCount = 0;
			slashInvincibleTime = SLASH_FRAME + 5;	//斬撃時間+nフレームを無敵時間にする
			//アニメーションに必要な各変数を初期化する
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
		//斬撃の更新処理を行う
		if (isSlash) {
			slashFrame++;
			//一定フレームに達したらフラグを折る
			if (slashFrame >= SLASH_FRAME) {
				isSlash = false;
				//一定数倒していたら続けて斬撃を出せるようにする
				/*if (slashDeathCount >= SLASH_DEATH_COUNT) {
					canSlash = true;
					slashDeathCount = 0;
				}*/
				canSlash = true;
				slashFinishTrigger = true;
			}
			//斬撃時の移動処理 移動は9Fまで
			if (slashFrame < 9) {
				float moveAmount = (SLASH_MOVE_AMOUNT_MAX - SLASH_MOVE_AMOUNT_MIN) * buttonInputRate + SLASH_MOVE_AMOUNT_MIN;
				slashMoveBuff += Vec2(-cosf(playerAngle + DX_PI_F/* / 2.0f*/) * moveAmount,
					-sinf(playerAngle + DX_PI_F /*/ 2.0f*/) * moveAmount);
			}
		}
		//斬撃のクールタイムの更新
		if (!isSlash && !canSlash) {
			slashCooltime++;
			//クールタイムが一定数に達したら斬撃可能状態にする
			if (slashCooltime >= SLASH_COOLTIME) {
				canSlash = true;
			}
		}
		//斬撃の無敵時間の更新
		if (slashInvincibleTime > 0) {
			slashInvincibleTime--;
		}

		//斬撃アニメーションの更新
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

		//斬撃時に移動させう処理
		Vec2 slashBuff = Vec2(slashMoveBuff.x / 5.0f, slashMoveBuff.y / 5.0f);
		//移動させる
		pos += slashBuff;
		moveBuff += slashBuff;
		slashMoveBuff -= slashBuff;
	}

	//軌道の更新
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

	//ゲージがマックスになっていたら弾をway弾にする
	//if (gauge >= PLAYER_GAUGE_MAX) {
	//	gauge = 0;
	//	//bulletStates = BULLET_GENERATE_STATES_WAY;
	//}

	/*--弾を撃っていたら反動の処理を行う--*/
	if (isShoot) {
		isReaction = true;
		reactionTime = shootDelay;
		armReactionPos = REACTION_AMOUNT;
	}
	//反動の更新処理
	if (isReaction) {
		reactionTimer++;
		//リアクションタイマーがリアクションの合計時間の半分より小さかったら、後ろに下げる処理をする
		armReactionPos -= armReactionPos * (reactionTimer / reactionTime);

		//リアクションタイマーがリアクション合計時間以上になったら初期化
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

	//判定円
	//if (isSlash) {
	//	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	//	DrawCircle(pos.x - scrollAmount.x, pos.y - scrollAmount.y, SLASH_RADIUS, GetColor(255, 0, 0));
	//	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//}

	//斬撃アニメフラグが立っていたら
	if (isSlashAnime) {
			//ビームサーベルを出す
			DrawRotaGraph3(pos.x - scrollAmount.x,
				pos.y - scrollAmount.y,
				225 , 225 ,			//プレイヤーを回転させる画像上での中心座標
				CAMERA_RATE, CAMERA_RATE,
				playerAngle + DX_PI_F / 2.0f,
				slashGraph[slashAnimeNumber], true);
	}
	else if (isSlash && slashFrame < SLASH_FRAME)
	{
		//ビームサーベル余韻
		DrawRotaGraph3(pos.x - scrollAmount.x,
			pos.y - scrollAmount.y,
			225 , 225 ,			//プレイヤーを回転させる画像上での中心座標
			CAMERA_RATE, CAMERA_RATE,
			playerAngle + DX_PI_F / 2.0f,
			slashEndGraph, true);
	}
	else if (0 < slashButtonFrame)
	{
		//半円描画
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 130);
		DrawRotaGraph(pos.x - scrollAmount.x, pos.y - scrollAmount.y, CAMERA_RATE, ARM_ANGLE(playerAngle), slashRangeGraph, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		//ビームサーベルを取り出し
		DrawRotaGraph3(pos.x - scrollAmount.x,
			pos.y - scrollAmount.y,
			225, 225,			//プレイヤーを回転させる画像上での中心座標
			CAMERA_RATE, CAMERA_RATE,
			playerAngle + DX_PI_F / 2.0f,
			slashStartGraph, true);
	}
	//通常状態だったら
	else {
		DrawRotaGraph(pos.x - scrollAmount.x + (cosf(ARM_ANGLE(playerAngle)) * PLAYER_ARM_POS) + (cosf(ARM_ANGLE(playerAngle)) * armReactionPos),
			pos.y - scrollAmount.y + (sinf(ARM_ANGLE(playerAngle)) * PLAYER_ARM_POS) + (sinf(ARM_ANGLE(playerAngle)) * armReactionPos),
			CAMERA_RATE, playerAngle, playerArmGraph, true);
		//DrawRotaGraph2(pos.x - scrollAmount.x + cosf(ARM_ANGLE(playerAngle)) * armReactionPos,
		//	pos.y - scrollAmount.y + sinf(ARM_ANGLE(playerAngle)) * armReactionPos,
		//	24.5f, 46,			//プレイヤーを回転させる画像上での中心座標
		//	1, playerAngle, playerArmGraph, TRUE);
	}
	DrawRotaGraph(pos.x - scrollAmount.x, pos.y - scrollAmount.y, CAMERA_RATE, ARM_ANGLE(playerAngle), playerHeadGraph, true);

	SetDrawBright(0xFF, 0xFF, 0xFF);

	//ボタン押下状況サークル
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
