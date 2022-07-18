#include "TitleScene.h"
#include"Vec2.h"
#include"BulletManager.h"
#include"BaseLayout.h"
#include"SoundEmitter.h"
#include"Constants.h"
#include"Easing.h"

const int ENEMY_REVIVE_TIME = 60;

const int MODE_SIMULATE_X = 982;
const int MODE_SIMULATE_Y = 24;
const int MODE_SIMULATE_WIDTH = 379;
const int MODE_SIMULATE_HEIGHT = 45;
const int MODE_SIMULATE_SCROLL_WIDTH = 260;
const int MODE_SIMULATE_SCROLL_MAX = -MODE_SIMULATE_WIDTH + (MODE_SIMULATE_WIDTH - MODE_SIMULATE_SCROLL_WIDTH);

//チュートリアル
const int TUOTRIAL_WIDTH = 379;
const int TUTORIAL_APPEAR_X = 546 + TUOTRIAL_WIDTH / 2.0f;
const int TUTORIAL_APPEAR_Y = 429 + 267.0f / 2.0f;
//const int HANDLE_TUTORIAL_CLEAR_INPUT_FRAME = 60;
//const int ACCEL_TUTORIAL_CLEAR_INPUT_FRAME = 60;
//const int SLASH_TUTORIAL_CLEAR_INPUT_COUNT = 5;
const int TUTORIAL_PROCEED_MAX[TitleScene::TUTORIAL_NUM] = {
	60,60,5
};
const int TUTORIAL_EXPAND_TOTAL_TIME = 15;

const float TUTORIAL_CENTER_X = 492;
const float TUTORIAL_OFFSET_X = 265 + 30;

void TitleScene::DrawRightScrollGraph(int x, int y, int width, int height,int scrollWidth, int scrollX, int graphHandle)
{
	if (0 < scrollX)
	{
		int offsetX = width - scrollX;
		int visibleWidth = offsetX;
		if (scrollWidth < visibleWidth)
		{
			visibleWidth = scrollWidth;
		}
		DrawRectGraph(x, y, width - offsetX, 0, visibleWidth, height, graphHandle, true);
	}
	else
	{
		int offsetX = width + scrollX;
		int visibleWidth = scrollWidth + scrollX;
		DrawRectGraph(x + width - offsetX, y, 0, 0, visibleWidth, height, graphHandle, true);
	}
}

TitleScene::TitleScene(SceneChanger* changer) :BaseScene(changer)
{
	titleBGM = SoundEmitter::Instance()->LoadSound("resource/bgm_title.wav", TITLE_BGM_VOLUME);
	tutorialProceed = 0;
	tutorialTimer = TUTORIAL_EXPAND_TOTAL_TIME;
	tutorialExp = 1.0f;
	expand = false;
	tutorialInput = true;

	tutorialSkip = false;

	tutorialSE = SoundEmitter::Instance()->LoadSound("resource/tutorialclearSE.mp3", 120);
	tutorialOnOffSE = SoundEmitter::Instance()->LoadSound("resource/tutorialonoffSE.mp3", 150);
}

void TitleScene::Initialize()
{
	BaseLayout::Instance()->Initialize(&player);
	BulletManager::Instance()->Init();
	reviveTimer = 0;
	//enemy.Generate(player.pos);
	modeSimulateScrollX = MODE_SIMULATE_WIDTH;

	SoundEmitter::Instance()->Play(titleBGM, DX_PLAYTYPE_LOOP, true);

	entryPoint.Init();

	hexagonFloor.Init();

	player.Init();

	nowTutorialGraph = Input::Controller() ? tutorialGraph_controller : tutorialGraph_keymou;
	nowTutorialSmallGraph = Input::Controller() ? tutorialGraph_controller_small : tutorialGraph_keymou_small;
}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
	nowTutorialGraph = Input::Controller() ? tutorialGraph_controller : tutorialGraph_keymou;
	nowTutorialSmallGraph = Input::Controller() ? tutorialGraph_controller_small : tutorialGraph_keymou_small;

	/*-----プレイヤーの更新処理-----*/
	player.Update(BulletManager::Instance()->bulletGenerateTimer == 0, BulletManager::Instance()->bulletGenerateTimerDef);

	/*-----プレイヤー弾の更新処理-----*/
	BulletManager::Instance()->Update(player.pos, player.playerAngle, player.scrollAmount, player.hitTimer <= 0 && player.slashButtonFrame <= 0, player.bulletStates);

	/*-----敵の更新処理-----*/
	//enemy.Update(player.pos);

	/*----敵の復活処理----*/
	//if (!enemy.isAlive){
	//	reviveTimer--;
	//	if (reviveTimer <= 0) {	//死んでから指定の時間が経過したら復活
	//		enemy.Generate(player.pos);
	//		printf("チュートリアル敵復活\n");
	//	}
	//}

	hexagonFloor.Update(player.pos, player.size);

	/*-----当たり判定処理-----*/
	/*プレイヤーの押し戻し処理*/
	//マップの中心とプレイヤーの距離
	float playerMapDistance = player.pos.Distance(Vec2(HALF_WIN_WIDTH, HALF_WIN_HEIGHT));
	//距離がマップの半径よりも大きかったら
	if (playerMapDistance > MAP_RADIUS) {
		//はみ出ている距離を求める
		float dev = playerMapDistance - MAP_RADIUS;
		//プレイヤーからマップの中心への方向ベクトルを求める
		Vec2 playerToMap = Vec2((WIN_WIDTH / 2.0f) - player.pos.x, (WIN_HEIGHT / 2.0f) - player.pos.y);
		playerToMap.Normalize();

		//押し戻す
		player.pos += Vec2(playerToMap.x * dev, playerToMap.y * dev);
		//スクロール量を更新
		player.scrollAmount.x += playerToMap.x * dev;
		player.scrollAmount.y += playerToMap.y * dev;
	}

	if (tutorialNum == TUTORIAL_NUM) {
#pragma region 当たり判定
		/*弾と敵の当たり判定*/
		for (int bullet = 0; bullet < BULLET_MAX; ++bullet) {
			//生存していなかったら処理をスキップ
			if (BulletManager::Instance()->bullet[bullet].isAlive == false) continue;

			if (0 < entryPoint.entryPointHp) {
				//当たり判定を行う
				float dist = BulletManager::Instance()->bullet[bullet].pos.Distance(ENTRY_POINT_POS);
				if (dist <= BULLET_SIZE + 40) {
					//弾を消す
					BulletManager::Instance()->bullet[bullet].isAlive = false;
					//エントリーポイントのHPを削る
					entryPoint.Damage();
					//エントリーポイントのHPが0以下になったらシーン遷移
					if (entryPoint.entryPointHp <= 0) {
						SoundEmitter::Instance()->FadeOutNowPlayBGM(2);
					}
				}
			}
		}

		if (player.slashFinishTrigger)entryPoint.slashed = false;

		if (player.isSlash && 0 < entryPoint.entryPointHp && !entryPoint.slashed) {
			Vec2 entryPointPos = ENTRY_POINT_POS;
			if (entryPointPos.Distance(player.pos) <= SLASH_RADIUS + 40) {
				//斬撃半径内に敵が入っていたら角度を求める
				float bulletAngle = player.playerAngle - DX_PI_F / 1.95f;
				Vec2 enemyVec = entryPointPos - player.pos;
				enemyVec.Normalize();
				float enemyAngle = atan2(enemyVec.y, enemyVec.x);
				//弾射出角度 - 敵との角度の絶対値ががPI / 2.0fより小さかったら当たっている
				if (fabs(bulletAngle - enemyAngle) <= DX_PI_F / 2.0f * 1.1) {
					entryPoint.slashed = true;
					//エントリーポイントのHPを削る
					entryPoint.Damage();
					//エントリーポイントのHPが0以下になったらシーン遷移
					if (entryPoint.entryPointHp <= 0) {
						SoundEmitter::Instance()->FadeOutNowPlayBGM(2);
					}
				}
			}
		}


		/*プレイヤーと敵の当たり判定*/
		//プレイヤーが無敵時間中だったら当たり判定を行わない
		//if (player.hitTimer <= 0) {
		//	if (enemy.isAlive != false){
		//		//当たり判定を行う
		//		float playerEnemyDistance = player.pos.Distance(enemy.pos);
		//		if (playerEnemyDistance <= player.size / 2.0f + enemy.size) {
		//			//敵を削除
		//			enemy.Init();
		//			//プレイヤーを操作不能&当たり判定なし状態へ
		//			player.Hit();
		//		}
		//	}
		//}
#pragma endregion
	}

	BaseLayout::Instance()->MeterUpdate(player.move.Length());

	//Mode-Simulateのスクロール更新
	modeSimulateScrollX--;
	if (modeSimulateScrollX == MODE_SIMULATE_SCROLL_MAX)
	{
		modeSimulateScrollX = MODE_SIMULATE_WIDTH;
	}

	//LogOut欄走査線
	scanlineOffsetY += 0.3f;
	if (10 < scanlineOffsetY)scanlineOffsetY = 0;

	entryPoint.Update();

	//チュートリアル
	if (tutorialTimer < TUTORIAL_EXPAND_TOTAL_TIME) {
		tutorialTimer++;
		if (!expand) {
			tutorialExp = Easing::BackIn(tutorialTimer, TUTORIAL_EXPAND_TOTAL_TIME, 1.0f, 0.0f);
			if (tutorialTimer == TUTORIAL_EXPAND_TOTAL_TIME && tutorialNum < TUTORIAL_NUM) {
				tutorialTimer = 0;
				expand = true;
				tutorialNum++;
				if (tutorialSkip) {
					tutorialNum = TUTORIAL_NUM;
				}
			}
		}
		else {
			tutorialExp = Easing::BackOut(tutorialTimer, TUTORIAL_EXPAND_TOTAL_TIME, 0.0f, 1.0f);

			if (tutorialTimer == TUTORIAL_EXPAND_TOTAL_TIME) {
				tutorialInput = true;
				tutorialProceed = 0;
			}
		}
		if (tutorialExp < 0.0f)tutorialExp = 0.0f;
	}

	if (tutorialInput) {
		if (tutorialNum == HANDLE ) {
			bool flag = Input::Controller() ?
				(Input::GetJoyLeftStick().x != 0.0f || Input::GetJoyLeftStick().y != 0.0f)
				: Input::MouseMove();
			if (flag) {
				tutorialProceed++;
				if (tutorialProceed == TUTORIAL_PROCEED_MAX[HANDLE]) {
					tutorialTimer = 0;
					expand = false;
					tutorialInput = false;
					tutorialBarWidthRate = 0.0f;
					SoundEmitter::Instance()->Play(tutorialSE, DX_PLAYTYPE_BACK);
				}
			}
		}
		else if (tutorialNum == ACCEL) {
			bool flag = Input::Controller() ?
				Input::GetJoyRightTrigger()
				: Input::isMouseBottom(MOUSE_INPUT_LEFT);
			if (flag) {
				tutorialProceed++;
				if (tutorialProceed == TUTORIAL_PROCEED_MAX[ACCEL]) {
					tutorialTimer = 0;
					expand = false;
					tutorialInput = false;
					tutorialBarWidthRate = 0.0f;
					SoundEmitter::Instance()->Play(tutorialSE, DX_PLAYTYPE_BACK);
				}
			}
		}
		else if (tutorialNum == SLASH) {
			bool flag = Input::Controller() ? Input::isJoyBottomTrigger(XINPUT_BUTTON_A) : Input::isKeyTrigger(KEY_INPUT_SPACE);
			if (flag) {
				tutorialProceed++;
				if (tutorialProceed - 1 == TUTORIAL_PROCEED_MAX[SLASH]) {
					tutorialTimer = 0;
					expand = false;
					tutorialInput = false;
					tutorialBarWidthRate = 0.0f;
					SoundEmitter::Instance()->Play(tutorialSE, DX_PLAYTYPE_BACK);
				}
			}
		}
	}


	bool tFlag = Input::Controller() ? Input::isJoyBottomTrigger(XINPUT_BUTTON_BACK) : Input::isKeyTrigger(KEY_INPUT_T);
	if (tFlag) {
		tutorialTimer = 0;
		if (tutorialNum == TUTORIAL_NUM)expand = !expand;
		else expand = false;
		tutorialInput = false;
		tutorialSkip = true;
		SoundEmitter::Instance()->Play(tutorialOnOffSE, DX_PLAYTYPE_BACK);
	}

	if (tutorialNum == TUTORIAL_NUM) {
		arrowAlphaAngle += PI / 180 * 2;
		if (PI < arrowAlphaAngle) {
			arrowAlphaAngle -= PI;
		}
	}

	//ゲームシーンへ
	if (Input::isKeyTrigger(KEY_INPUT_N))
	{
		SoundEmitter::Instance()->FadeOutNowPlayBGM(2);
	}
	if (!SoundEmitter::Instance()->NowPlayBGM())
	{
		sceneChanger->ChangeScene(GAME_SCENE, &cutin);
	}

	if (Input::isKey(KEY_INPUT_LEFT)) {
		x--;
	}
	else if (Input::isKey(KEY_INPUT_RIGHT)) {
		x++;
	}
	if (Input::isKey(KEY_INPUT_UP)) {
		y--;
	}
	else if (Input::isKey(KEY_INPUT_DOWN)) {
		y++;
	}
}

void TitleScene::Draw()
{
	//六角形床の描画処理
	//BaseLayout::Instance()->DrawHexagon();
	hexagonFloor.Draw(player.scrollAmount);

	//マップの描画処理
	DrawCircle(HALF_WIN_WIDTH - player.scrollAmount.x, HALF_WIN_HEIGHT - player.scrollAmount.y, MAP_RADIUS, GetColor(0x78, 0xFF, 0xA9), FALSE, 5);

	////敵の描画処理
	//if (enemy.isAlive) {
	//	enemy.Draw(player.scrollAmount, enemyCircleOuter, enemyCircleInner, enemyDirection);
	//	DrawRotaGraph(enemy.pos.x - player.scrollAmount.x, enemy.pos.y - player.scrollAmount.y + 50, 1.0f, 0.0f, enemyStr, true);
	//}
	BulletManager::Instance()->Draw(player.scrollAmount);

	//プレイヤーの描画処理
	player.Draw();

	if (tutorialNum == TUTORIAL_NUM && !(entryPoint.entryPointHp <= 0)) {
		entryPoint.Draw(player.scrollAmount);
	}

	//タイトルロゴ
	if (GetRand(100) % 99 != 0) {
		DrawGraph(36, 18, titleLogo, true);
	}

	//チュートリアル
	//DrawGraph(TUTORIAL_APPEAR_X, TUTORIAL_APPEAR_Y, tutorialGraph[tutorialNum], true);
	if (tutorialNum < TUTORIAL_NUM) {
		if (tutorialInput) {
			float rate = (float)tutorialProceed / (float)TUTORIAL_PROCEED_MAX[tutorialNum];
			if (1.0f < rate)rate = 1.0f;
			tutorialBarWidthRate = Easing::Lerp(tutorialBarWidthRate, rate, 0.1f);
			DrawBox(577, 403, 577 + 320 * tutorialBarWidthRate, 403 + 16, GetColor(255, 44, 105), true);
		}
		DrawRotaGraph(TUTORIAL_APPEAR_X, TUTORIAL_APPEAR_Y, tutorialExp, 0.0f, nowTutorialGraph[tutorialNum], true);

		//チュートリアルスキップ
		if (Input::Controller()) {
			DrawGraph(619, 21, tutorial_skip, true);
		}
		else {
			DrawGraph(619, 21, tutorial_skip_keymou, true);
		}
	}
	else {
		if (!entryPoint.CanVisible(player.scrollAmount)) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, sin(arrowAlphaAngle) * 255.0f);
			Vec2 vec = ENTRY_POINT_POS;
			vec -= player.pos;
			float angle = atan2(vec.y, vec.x);
			DrawRotaGraph(player.pos.x + cos(angle) * 120 - player.scrollAmount.x, player.pos.y + sin(angle) * 120 - player.scrollAmount.y,
				0.55f, angle, arrowGraph, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		else {
			arrowAlphaAngle = 0.0f;
		}

		DrawRotaGraph(TUTORIAL_CENTER_X - TUTORIAL_OFFSET_X, TUTORIAL_APPEAR_Y + 30, tutorialExp, 0.0f, nowTutorialSmallGraph[0], true);
		DrawRotaGraph(TUTORIAL_CENTER_X, TUTORIAL_APPEAR_Y + 30, tutorialExp, 0.0f, nowTutorialSmallGraph[1], true);
		DrawRotaGraph(TUTORIAL_CENTER_X + TUTORIAL_OFFSET_X, TUTORIAL_APPEAR_Y + 30, tutorialExp, 0.0f, nowTutorialSmallGraph[2], true);

		//チュートリアルスキップオンオフ
		if (Input::Controller()) {
			DrawGraph(619 - (366 - 304), 21, tutorial_on_off, true);
		}
		else {
			DrawGraph(619 - (366 - 304), 21, tutorial_on_off_keymou, true);
		}
	}



	if (entryPoint.entryPointHp <= 0) {
		entryPoint.Draw(player.scrollAmount);
	}

	//ログアウト画面
	DrawGraph(978,95, logoutBack, true);
	DrawGraph(978, 95 + scanlineOffsetY, logoutScanLine, true);
	DrawGraph(978, 95, logoutStr, true);

	//スクリーンフレームの描画処理
	BaseLayout::Instance()->DrawScreenFrame(player.move.Length());

	//Mode-Simulate文字の描画処理
	DrawRightScrollGraph(MODE_SIMULATE_X, MODE_SIMULATE_Y, 
		MODE_SIMULATE_WIDTH, MODE_SIMULATE_HEIGHT,MODE_SIMULATE_SCROLL_WIDTH, modeSimulateScrollX, modeSimulate);

	//DrawFormatString(0, 0, GetColor(255, 255, 255), "x %d, y %d\ntutorialProceed %d", x, y,tutorialProceed);
}
