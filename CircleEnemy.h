#pragma once
#include "Vec2.h"
#include"EnemyInterFace.h"

//周回敵の先頭
class CircleEnemyHead{
public:
	Vec2 pos;							//敵の座標
	float size;							//敵のサイズ
	float angle;						//中心点からの角度 これを更新して回転していく
	float radius;						//中心点からの距離 これで円の中のどの辺を周回するかを決める
	bool isAlive;						//敵が生存しているかのフラグ
	int hp;								//HP

	bool isEscape;						//脱出モードフラグ これが立っているときは中心から逆方向に逃げる

	//初期化処理
	void Init();
	//生成処理
	void Generate(Vec2 generatePos);
	//更新処理
	void Update(int states, Vec2 playerPos, float speed);
};
//後続の周回敵
class CircleEnemyFollowing{
public:
	Vec2 pos;							//敵の座標
	float size;							//敵のサイズ
	bool isAlive;						//敵が生存しているかのフラグ
	int hp;								//HP
	
	//初期化処理
	void Init();
	//生成処理
	void Generate(Vec2 prevPos);
	//更新処理
	void Update(Vec2 prevPos, float speed);
};

enum CIRCLE_ENEMY_STATES{
	CIRCLE_ENEMY_STATES_ROCKON,			//プレイヤーを追尾する状態
	CIRCLE_ENEMY_STATES_ROUND			//うろうろモード
};

//周回敵管理クラス
class CircleEnemys : public EnemyInterFace{
public:
	CircleEnemyHead head;				//周回敵グループの先頭
	CircleEnemyFollowing following[CIRCLE_ENEMY_FOLLOWING_COUNT];	//後続

	float speed;						//移動速度
	float originalSpeed;				//本来あるべき移動速度
	int aliveCount;						//生存数
	int states;							//周回敵のステータス
	int statesTimer;					//ステータスが変わるまでのタイマー

	//コンストラクタ
	CircleEnemys(ENEMY_TYPE type);

	//初期化処理
	void Init()override;
	//生成処理
	virtual void Generate(Vec2 generatePos, Vec2 playerPos,int* hp = nullptr, float* angle = nullptr, Vec2* targetPos = nullptr)override = 0;
	//更新処理
	virtual void Update(Vec2 playerPos)override = 0;
	//描画処理
	void Draw(Vec2 scrollAmount, int enemyCircleOuter, int enemyCircleInner, int enemyDirection, int warningGraph, Vec2 playerPos)override;
	//当たり判定
	bool HitCheck(Vec2 pos, float size, bool player, bool* addScore)override;
	bool HitCheckSlash(Vec2 pos, float angle, int& deathCount, int& hitCount)override;
	//逃走
	void Escape()override;

	//本来あるべき速度を更新
	void HitAccel();
	//hitした時の処理
	void Hit(bool player);
};

/*--プレイヤー追尾--*/
class TrackingCircleEnemys : public CircleEnemys 
{
public:
	TrackingCircleEnemys() : CircleEnemys(CIRCLE_ENEMY_TRACKING) {};
	void Generate(Vec2 generatePos, Vec2 playerPos, int* hp = nullptr, float* angle = nullptr, Vec2* targetPos = nullptr)override;
	void Update(Vec2 playerPos)override;
};