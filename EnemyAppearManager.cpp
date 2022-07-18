#include "EnemyAppearManager.h"

EnemyAppearManager::EnemyAppearManager()
{
	/*---ここでウェーブ生成---*/
	//第一ウェーブの生成
	EnemyWave_Frame waveBuff(0);
	waveBuff.Init();
	//最初に画面中心から一定距離離れた位置に五体の目的地敵を出現
	for (int i = 0; i < 5; ++i) {
		float generateAngle = DX_TWO_PI_F * (i / 5.0f);
		Vec2 generatePos = Vec2(cosf(generateAngle) * MAP_RADIUS + HALF_WIN_WIDTH, sinf(generateAngle) * MAP_RADIUS + HALF_WIN_HEIGHT);
		Vec2 targetPos = Vec2(cosf(generateAngle) * (MAP_RADIUS / 2.0f) + HALF_WIN_WIDTH, sinf(generateAngle) * (MAP_RADIUS / 2.0f) + HALF_WIN_HEIGHT);
		waveBuff.AddInfo(NORMAL_ENEMY_DESTINATION, 30, generatePos, 0, 0, targetPos);
	}
	//一定フレーム経過後に画面中心から一定距離離れた位置に五体の目的地敵を出現
	for (int i = 0; i < 5; ++i) {
		float generateAngle = DX_TWO_PI_F * (i / 5.0f) + DX_PI_F;
		Vec2 generatePos = Vec2(cosf(generateAngle) * MAP_RADIUS + HALF_WIN_WIDTH, sinf(generateAngle) * MAP_RADIUS + HALF_WIN_HEIGHT);
		Vec2 targetPos = Vec2(cosf(generateAngle) * (MAP_RADIUS / 2.0f + MAP_RADIUS / 4.0f) + HALF_WIN_WIDTH, sinf(generateAngle) * (MAP_RADIUS / 2.0f + MAP_RADIUS / 4.0f) + HALF_WIN_HEIGHT);
		waveBuff.AddInfo(NORMAL_ENEMY_DESTINATION, 300, generatePos, 0, 0, targetPos);
	}

	waves.emplace_back(waveBuff);

	//第二ウェーブの生成
	EnemyWave_Frame waveBuff2(600);
	waveBuff2.Init();
	//まず最初にレア的を一定間隔で出す
	waveBuff2.AddInfo(NORMAL_ENEMY_HIGHSCORE, 0, Vec2(HALF_WIN_WIDTH + MAP_RADIUS, HALF_WIN_HEIGHT));		//レア的の引数は型名と開始フレーム以外意味を持たないです
	waveBuff2.AddInfo(NORMAL_ENEMY_HIGHSCORE, 120, Vec2(HALF_WIN_WIDTH + MAP_RADIUS, HALF_WIN_HEIGHT));
	waveBuff2.AddInfo(NORMAL_ENEMY_HIGHSCORE, 240, Vec2(HALF_WIN_WIDTH + MAP_RADIUS, HALF_WIN_HEIGHT));
	waveBuff2.AddInfo(NORMAL_ENEMY_HIGHSCORE, 360, Vec2(HALF_WIN_WIDTH + MAP_RADIUS, HALF_WIN_HEIGHT));
	waveBuff2.AddInfo(NORMAL_ENEMY_HIGHSCORE, 480, Vec2(HALF_WIN_WIDTH + MAP_RADIUS, HALF_WIN_HEIGHT));

	waves.emplace_back(waveBuff2);

	//第三ウェーブの生成
	EnemyWave_Frame waveBuff3(1200);
	waveBuff3.Init();
	//中心に向かって真っすぐ動く敵を90度間隔で回転させながら出す
	waveBuff3.AddInfo(NORMAL_ENEMY_STRAIGHT, 60, Vec2(HALF_WIN_WIDTH, HALF_WIN_HEIGHT - MAP_RADIUS), 6, DX_PI_F / 2.0f);
	waveBuff3.AddInfo(NORMAL_ENEMY_STRAIGHT, 60, Vec2(HALF_WIN_WIDTH - MAP_RADIUS, HALF_WIN_HEIGHT), 6, 0.001f);
	waveBuff3.AddInfo(NORMAL_ENEMY_STRAIGHT, 60, Vec2(HALF_WIN_WIDTH, HALF_WIN_HEIGHT + MAP_RADIUS), 6, DX_PI_F + DX_PI_F / 2.0f);
	waveBuff3.AddInfo(NORMAL_ENEMY_STRAIGHT, 60, Vec2(HALF_WIN_WIDTH + MAP_RADIUS, HALF_WIN_HEIGHT), 6, DX_PI_F);
	waves.emplace_back(waveBuff3);

	//第四ウェーブの生成
	EnemyWave_Frame waveBuff4(1800);
	waveBuff4.Init();
	//上下に小さな壁のような敵を出す
	for (int i = 0; i < 5; ++i) {
		Vec2 generatePos = Vec2((HALF_WIN_WIDTH - 2.0f * (90.0f * CAMERA_RATE)) + (90.0f * CAMERA_RATE) * i, HALF_WIN_HEIGHT - MAP_RADIUS);
		waveBuff4.AddInfo(NORMAL_ENEMY_DESTINATION, 0, generatePos, 0, 0, Vec2(generatePos.x, generatePos.y + MAP_RADIUS / 2.0f));
	}
	for (int i = 0; i < 5; ++i) {
		Vec2 generatePos = Vec2((HALF_WIN_WIDTH - 2.0f * (90.0f * CAMERA_RATE)) + (90.0f * CAMERA_RATE) * i, HALF_WIN_HEIGHT + MAP_RADIUS);
		waveBuff4.AddInfo(NORMAL_ENEMY_DESTINATION, 0, generatePos, 0, 0, Vec2(generatePos.x, generatePos.y - MAP_RADIUS / 2.0f));
	}
	//左右からは壁を中心に向かって移動させて出す
	for (int i = 0; i < 3; ++i) {
		Vec2 generatePos = Vec2(HALF_WIN_WIDTH - (MAP_RADIUS * 0.5f), ((HALF_WIN_HEIGHT - 1.0f * (90.0f * CAMERA_RATE)) + (90.0f * CAMERA_RATE) * i));
		waveBuff4.AddInfo(NORMAL_ENEMY_STRAIGHT, 300, generatePos, 6);
	}
	for (int i = 0; i < 3; ++i) {
		Vec2 generatePos = Vec2(HALF_WIN_WIDTH + (MAP_RADIUS * 0.5f), ((HALF_WIN_HEIGHT - 1.0f * (90.0f * CAMERA_RATE)) + (90.0f * CAMERA_RATE) * i));
		waveBuff4.AddInfo(NORMAL_ENEMY_STRAIGHT, 300, generatePos, 6);
	}

	waves.emplace_back(waveBuff4);

	//第五ウェーブの生成
	EnemyWave_Frame waveBuff5(2400);
	waveBuff5.Init();
	//左右に円を描くように追尾敵を生成
	for (int i = 0; i < 3; ++i) {
		Vec2 centerPos = Vec2(HALF_WIN_WIDTH - MAP_RADIUS * 0.75f, HALF_WIN_HEIGHT);
		Vec2 generatePos = Vec2(cosf(DX_TWO_PI_F * ((float)i / 3.0f)) * 100.0f + centerPos.x,
			sinf(DX_TWO_PI_F * ((float)i / 3.0f)) * 100.0f + centerPos.y);
		waveBuff5.AddInfo(NORMAL_ENEMY_TRACKING, 400, generatePos);
	}
	for (int i = 0; i < 3; ++i) {
		Vec2 centerPos = Vec2(HALF_WIN_WIDTH + MAP_RADIUS * 0.75f, HALF_WIN_HEIGHT);
		Vec2 generatePos = Vec2(cosf(DX_TWO_PI_F * ((float)i / 3.0f)) * 100.0f + centerPos.x,
			sinf(DX_TWO_PI_F * ((float)i / 3.0f)) * 100.0f + centerPos.y);
		waveBuff5.AddInfo(NORMAL_ENEMY_TRACKING, 420, generatePos);
	}
	//上下に蛇敵も生成
	waveBuff5.AddInfo(CIRCLE_ENEMY_TRACKING, 400, Vec2(HALF_WIN_WIDTH, HALF_WIN_HEIGHT - MAP_RADIUS * 0.75f));
	waveBuff5.AddInfo(CIRCLE_ENEMY_TRACKING, 400, Vec2(HALF_WIN_WIDTH, HALF_WIN_HEIGHT + MAP_RADIUS * 0.75f));

	waves.emplace_back(waveBuff5);

	//追尾敵が強いので、1Wave分空きを作る

	//第七ウェーブの生成
	EnemyWave_Frame waveBuff7(3600);
	waveBuff7.Init();
	//このウェーブは第7ウェーブの準備段階
	waveBuff7.AddInfo(NORMAL_ENEMY_HIGHSCORE, 120, Vec2(HALF_WIN_WIDTH + MAP_RADIUS, HALF_WIN_HEIGHT));
	waveBuff7.AddInfo(NORMAL_ENEMY_HIGHSCORE, 240, Vec2(HALF_WIN_WIDTH + MAP_RADIUS, HALF_WIN_HEIGHT));
	waveBuff7.AddInfo(NORMAL_ENEMY_HIGHSCORE, 360, Vec2(HALF_WIN_WIDTH + MAP_RADIUS, HALF_WIN_HEIGHT));
	waveBuff7.AddInfo(NORMAL_ENEMY_HIGHSCORE, 480, Vec2(HALF_WIN_WIDTH + MAP_RADIUS, HALF_WIN_HEIGHT));
	waveBuff7.AddInfo(NORMAL_ENEMY_HIGHSCORE, 600, Vec2(HALF_WIN_WIDTH + MAP_RADIUS, HALF_WIN_HEIGHT));

	waveBuff7.AddInfo(NORMAL_ENEMY_HIGHSCORE, 60, Vec2(HALF_WIN_WIDTH + MAP_RADIUS, HALF_WIN_HEIGHT));
	waveBuff7.AddInfo(NORMAL_ENEMY_HIGHSCORE, 180, Vec2(HALF_WIN_WIDTH + MAP_RADIUS, HALF_WIN_HEIGHT));
	waveBuff7.AddInfo(NORMAL_ENEMY_HIGHSCORE, 300, Vec2(HALF_WIN_WIDTH + MAP_RADIUS, HALF_WIN_HEIGHT));
	waveBuff7.AddInfo(NORMAL_ENEMY_HIGHSCORE, 420, Vec2(HALF_WIN_WIDTH + MAP_RADIUS, HALF_WIN_HEIGHT));
	waveBuff7.AddInfo(NORMAL_ENEMY_HIGHSCORE, 540, Vec2(HALF_WIN_WIDTH + MAP_RADIUS, HALF_WIN_HEIGHT));

	waves.emplace_back(waveBuff7);

	//第八ウェーブの生成
	EnemyWave_Frame waveBuff8(4200);
	waveBuff8.Init();
	//無限型移動敵を生成
	waveBuff8.AddInfo(NORMAL_ENEMT_INFINITY, 200, Vec2(HALF_WIN_WIDTH, WIN_HEIGHT));

	waves.emplace_back(waveBuff8);

	//第九ウェーブも敵が大量に出てくるので1waveおやすみ

	//第十ウェーブの生成
	EnemyWave_Frame waveBuff10(5400);
	waveBuff10.Init();
	//左右から来る壁を生成
	for (int i = 0; i < 5; ++i) {
		Vec2 centerPos = Vec2(HALF_WIN_WIDTH - (MAP_RADIUS * 0.75f), (((HALF_WIN_HEIGHT)-3.0f * (90.0f * CAMERA_RATE))) + 45.0f);
		Vec2 generatePos = Vec2(centerPos.x, centerPos.y + (90.0f * CAMERA_RATE) * i);
		waveBuff10.AddInfo(NORMAL_ENEMY_STRAIGHT, 30, generatePos, 6, 0.0001f);
	}
	for (int i = 0; i < 5; ++i) {
		Vec2 centerPos = Vec2(HALF_WIN_WIDTH + (MAP_RADIUS * 0.75f), (((HALF_WIN_HEIGHT)-3.0f * (90.0f * CAMERA_RATE))) + 45.0f);
		Vec2 generatePos = Vec2(centerPos.x, centerPos.y + (90.0f * CAMERA_RATE) * i);
		waveBuff10.AddInfo(NORMAL_ENEMY_STRAIGHT, 30, generatePos, 6, atan2f(0, -10.0f));
	}
	//上下から来る敵を左右生成
	for (int i = 0; i < 5; ++i) {
		Vec2 generatePos = Vec2((HALF_WIN_WIDTH - 3.0f * (90.0f * CAMERA_RATE)) + (90.0f * CAMERA_RATE) * i + 45.0f, HALF_WIN_HEIGHT - MAP_RADIUS * 0.75f);
		waveBuff10.AddInfo(NORMAL_ENEMY_STRAIGHT, 30, generatePos, 6, atan2f(10.0f, 0));
	}
	for (int i = 0; i < 5; ++i) {
		Vec2 generatePos = Vec2((HALF_WIN_WIDTH - 3.0f * (90.0f * CAMERA_RATE)) + (90.0f * CAMERA_RATE) * i + 45.0f, HALF_WIN_HEIGHT + MAP_RADIUS * 0.75f);
		waveBuff10.AddInfo(NORMAL_ENEMY_STRAIGHT, 30, generatePos, 6, atan2f(-10, 0));
	}
	//円を描くように敵を生成
	for(int i = 0; i < 20; ++i){
		waveBuff10.AddInfo(NORMAL_ENEMY_STRAIGHT, 400, Vec2(HALF_WIN_WIDTH, HALF_WIN_HEIGHT), 4, DX_TWO_PI_F * ((float)i / 19.0f));
	}

	waves.emplace_back(waveBuff10);

	//第十一ウェーブの生成
	EnemyWave_Frame waveBuff11(6000);
	waveBuff11.Init();
	//左上から一定間隔ごとにプレイヤーの方向にまっすぐ敵を発射
	Vec2 buff11CenterPos = Vec2(HALF_WIN_WIDTH - MAP_RADIUS / 2.0f, HALF_WIN_HEIGHT - MAP_RADIUS / 2.0f);
	waveBuff11.AddInfo(NORMAL_ENEMY_STRAIGHT, 0, buff11CenterPos, 4);
	waveBuff11.AddInfo(NORMAL_ENEMY_STRAIGHT, 180, buff11CenterPos, 4);
	waveBuff11.AddInfo(NORMAL_ENEMY_STRAIGHT, 420, buff11CenterPos, 4);
	waveBuff11.AddInfo(NORMAL_ENEMY_STRAIGHT, 520, buff11CenterPos, 4);
	//左下から一定間隔ごとにプレイヤーの方向にまっすぐ敵を発射
	buff11CenterPos = Vec2(HALF_WIN_WIDTH - MAP_RADIUS / 2.0f, HALF_WIN_HEIGHT + MAP_RADIUS / 2.0f);;
	waveBuff11.AddInfo(NORMAL_ENEMY_STRAIGHT, 0, buff11CenterPos, 4);
	waveBuff11.AddInfo(NORMAL_ENEMY_STRAIGHT, 60, buff11CenterPos, 4);
	waveBuff11.AddInfo(NORMAL_ENEMY_STRAIGHT, 260, buff11CenterPos, 4);
	waveBuff11.AddInfo(NORMAL_ENEMY_STRAIGHT, 420, buff11CenterPos, 4);
	waveBuff11.AddInfo(NORMAL_ENEMY_STRAIGHT, 520, buff11CenterPos, 4);
	//右下から一定間隔ごとにプレイヤーの方向にまっすぐ敵を発射
	buff11CenterPos = Vec2(HALF_WIN_WIDTH + MAP_RADIUS / 2.0f, HALF_WIN_HEIGHT + MAP_RADIUS / 2.0f);
	waveBuff11.AddInfo(NORMAL_ENEMY_STRAIGHT, 0, buff11CenterPos, 4);
	waveBuff11.AddInfo(NORMAL_ENEMY_STRAIGHT, 180, buff11CenterPos, 4);
	waveBuff11.AddInfo(NORMAL_ENEMY_STRAIGHT, 420, buff11CenterPos, 4);
	waveBuff11.AddInfo(NORMAL_ENEMY_STRAIGHT, 520, buff11CenterPos, 4);
	//右上から一定間隔ごとにプレイヤーの方向にまっすぐ敵を発射
	buff11CenterPos = Vec2(HALF_WIN_WIDTH + MAP_RADIUS / 2.0f, HALF_WIN_HEIGHT - MAP_RADIUS / 2.0f);;
	waveBuff11.AddInfo(NORMAL_ENEMY_STRAIGHT, 0, buff11CenterPos, 4);
	waveBuff11.AddInfo(NORMAL_ENEMY_STRAIGHT, 60, buff11CenterPos, 4);
	waveBuff11.AddInfo(NORMAL_ENEMY_STRAIGHT, 280, buff11CenterPos, 4);
	waveBuff11.AddInfo(NORMAL_ENEMY_STRAIGHT, 420, buff11CenterPos, 4);
	waveBuff11.AddInfo(NORMAL_ENEMY_STRAIGHT, 520, buff11CenterPos, 4);

	waves.emplace_back(waveBuff11);

	//第十二ウェーブの生成		ラストウェーブ
	EnemyWave_Frame waveBuff12(6600);
	waveBuff12.Init();
	//ひたすら上からくる壁を生成
	Vec2 buff12StartPos = Vec2(HALF_WIN_WIDTH - MAP_RADIUS, HALF_WIN_HEIGHT - MAP_RADIUS);
	for (int i = 0; i < 30; ++i) {
		waveBuff12.AddInfo(NORMAL_ENEMY_STRAIGHT, 0, Vec2(buff12StartPos.x + (90.0f * CAMERA_RATE) * i, buff12StartPos.y), 1, atan2f(10, 0));
	}
	for (int i = 0; i < 30; ++i) {
		waveBuff12.AddInfo(NORMAL_ENEMY_STRAIGHT, 200, Vec2(buff12StartPos.x + (90.0f * CAMERA_RATE) * i, buff12StartPos.y), 1, atan2f(10, 0));
	}
	for (int i = 0; i < 30; ++i) {
		waveBuff12.AddInfo(NORMAL_ENEMY_STRAIGHT, 400, Vec2(buff12StartPos.x + (90.0f * CAMERA_RATE) * i, buff12StartPos.y), 1, atan2f(10, 0));
	}
	for (int i = 0; i < 30; ++i) {
		waveBuff12.AddInfo(NORMAL_ENEMY_STRAIGHT, 600, Vec2(buff12StartPos.x + (90.0f * CAMERA_RATE) * i, buff12StartPos.y), 1, atan2f(10, 0));
	}
	for (int i = 0; i < 30; ++i) {
		waveBuff12.AddInfo(NORMAL_ENEMY_STRAIGHT, 800, Vec2(buff12StartPos.x + (90.0f * CAMERA_RATE) * i, buff12StartPos.y), 1, atan2f(10, 0));
	}
	for (int i = 0; i < 30; ++i) {
		waveBuff12.AddInfo(NORMAL_ENEMY_STRAIGHT, 1000, Vec2(buff12StartPos.x + (90.0f * CAMERA_RATE) * i, buff12StartPos.y), 1, atan2f(10, 0));
	}
	for (int i = 0; i < 30; ++i) {
		waveBuff12.AddInfo(NORMAL_ENEMY_STRAIGHT, 1200, Vec2(buff12StartPos.x + (90.0f * CAMERA_RATE) * i, buff12StartPos.y), 1, atan2f(10, 0));
	}
	waves.emplace_back(waveBuff12);
}

void EnemyAppearManager::Init()
{
	for (auto& wave : waves) {
		wave.Init();
	}
	frame = 0;
}

void EnemyAppearManager::Update()
{
	for (auto& wave : waves) {
		if (wave.isAlive) {	//既に起動していたら
			wave.Update();	//ウェーブの更新
		}
		else
		{
			if (wave.OnFlag(frame)) {	//起動フラグがOnなら
				wave.Generate();	//起動
			}
		}
	}
	frame++;
}
