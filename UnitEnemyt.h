#pragma once
#include "Vec2.h"

//隊列敵のステータス
enum ENEMY_UNIT_STATES{
	ENEMY_UNIT_STATES_ROUND,			//中心を基準にぐるぐる回る隊列敵
	ENEMY_UNIT_STATES_WALL,				//壁を組んで移動する隊列敵
	ENEMY_UNIT_STATES_CLUSTER			//一定時間で拡散する隊列敵
};

enum UNIT_ENEMY_DIRECTION{
	UNIT_ENEMY_DIRECTION_RIGHT,			//上から
	UNIT_ENEMY_DIRECTION_DOWN,			//右から
	UNIT_ENEMY_DIRECTION_LEFT,			//下から
	UNIT_ENEMY_DIRECTION_UP				//左から
};

class UnitEnemy{
public:
	/*基本情報*/
	Vec2 pos;							//座標
	float angle;						//移動する角度
	float speed;						//移動速度
	Vec2 moved;							//移動した量
	int hp;								//HP
	int size;							//大きさ
	bool isAlive;						//生存しているかのフラグ
	int states;							//敵のステータス

	/*回る敵の情報*/
	float addAngle;						//角度に加算した料量

	/*壁の敵の情報*/

public:
	//コンストラクタ
	UnitEnemy();

	//初期化処理
	void Init();
	//生成処理
	void Generate(Vec2 pos, float angle, int states);
	//更新処理
	void Update();
	//描画処理
	void Draw(Vec2 scrollAmount, int enemyCircleOuter, int enemyCircleInner, int enemyDirection);
};

class UnitEnemyManager{
public:
	UnitEnemy unitEnemy[UNIT_ENEMY_COUNT];	//隊列敵の最大数
	bool isAlive;							//隊列敵が生成されているか
	int direction;							//壁が迫ってきている方向
	int warningAlpha;						//危険マークのアルファ値
	bool warningAlphaChange;				//危険マークのアルファ値が大きくなるか小さくなるか
	float warningAlphaTimer;				//危険マークのイージングに使うタイマー
	int states;								//敵のステータス
	bool isEscape;							//マップ外へ出たかどうかのフラグ
	
	/*--円状の敵用の変数--*/
	int roundEnemyGenerateCount;			//円状の敵が生成した数
	int roundEnemyGenerateTimer;			//円状の敵が生成するまでの遅延用

public:
	//コンストラクタ
	UnitEnemyManager();

	//初期化処理
	void Init();
	//生成処理
	void Generate(int states, int direction = 0);
	//更新処理
	void Update();
	//描画処理
	void Draw(Vec2 scrollAmount, int enemyCircleOuter, int enemyCircleInner, int enemyDirection, int warningGraph, Vec2 playerPos);
};