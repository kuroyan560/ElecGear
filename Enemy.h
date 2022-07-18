#pragma once
#include"EnemyInterFace.h"
#include"Vec2.h"
class Enemy : public EnemyInterFace
{
protected:
	/*敵基本情報*/
	Vec2 pos;							//敵の座標
	float speed;						//敵の移動速度
	float size;							//敵のサイズ
	float angle;						//敵の移動する角度
	Vec2 move;

	bool isEscape;						//脱出モードフラグ これが立っているときは中心から逆方向に逃げる

	int hitStanTimer;
	
	int generateTimer;					//生成するまでの時間
	bool isGenerate;					//生成しているかどうか

public:
	Enemy(ENEMY_TYPE type);
	//初期化処理
	virtual void Init()override;
	//生成処理
	virtual void Generate(Vec2 pos, Vec2 playerPos, int* hp = nullptr, float* angle = nullptr, Vec2* targetPos = nullptr) = 0;
	//更新処理
	virtual void Update(Vec2 playerPos)override = 0;
	//描画処理
	void Draw(Vec2 scrollAmount, int enemyCircleOuter, int enemyCircleInner, int enemyDirection, int warningGraph, Vec2 playerPos)override;
	//当たり判定
	virtual bool HitCheck(Vec2 pos, float size, bool player, bool* addScore)override;
	virtual bool HitCheckSlash(Vec2 pos, float angle, int& deathCount, int& hitCount)override;
	//逃走
	void Escape()override;
};

/*--プレイヤー追尾--*/
class TrackingEnemy : public Enemy {
public:
	TrackingEnemy() :Enemy(NORMAL_ENEMY_TRACKING) {};
	void Generate(Vec2 generatePos, Vec2 playerPos, int* hp = nullptr, float* angle = nullptr, Vec2* targetPos = nullptr)override;
	void Update(Vec2 playerPos)override;
};

/*--まっすぐ動く敵--*/
class StraightEnemy : public Enemy {
public:
	StraightEnemy() :Enemy(NORMAL_ENEMY_STRAIGHT) {};
	void Generate(Vec2 generatePos, Vec2 playerPos, int* hp = nullptr, float* angle = nullptr, Vec2* targetPos = nullptr)override;
	void Update(Vec2 playerPos)override;
};

/*--指定の位置まで移動する敵--*/
class DestinationEnemy : public Enemy {
public:
	Vec2 targetPos;							//目的地
	float distanceDef;						//目的地までの初期の距離
	int escapeTimer;						//脱出するまでのタイマー
	DestinationEnemy() : Enemy(NORMAL_ENEMY_DESTINATION) {};
	void Generate(Vec2 generatePos, Vec2 playerPos, int* hp = nullptr, float* angle = nullptr, Vec2* targetPos = nullptr)override;
	void Update(Vec2 playerPos)override;
};

/*--無限の形で移動し倒されたら大量の敵を出す敵--*/
class InfinityEnemy : public Enemy {
public:
	float rotationAngle;					//回転に必要なアングル
	float rotationAngleAmount;				//回転した量
	bool isRotationLeft;					//どっち側を回転しているか
	InfinityEnemy() : Enemy(NORMAL_ENEMT_INFINITY) {};
	//このクラスでは生成する際にあらかじめ決まった値を入れるので、引数は適当でよい
	void Generate(Vec2 generatePos, Vec2 playerPos, int* hp = nullptr, float* angle = nullptr, Vec2* targetPos = nullptr)override;
	void Update(Vec2 playerPos)override;
};

/*--生成した位置で停止させておく敵--*/
class StandByEnemy :public Enemy {
public:
	int escapeCount = 0;					//脱出するまでのタイマー

	StandByEnemy() :Enemy(NORMAL_ENEMY_STANDBY) {};
	void Generate(Vec2 generatePos, Vec2 playerPos, int* hp = nullptr, float* angle = nullptr, Vec2* targetPos = nullptr)override;
	void Update(Vec2 playerPos)override;
};

/*--ハイスコアの敵--*/
class HighScoreEnemy : public Enemy {
public:
	int rotationCount;						//脱出するまで回転する数
	float rotationRadius;					//回転する半径
	float rotationAngle;					//角度
	bool isRadiusMinus;						//半径を小さくするかどうかのフラグ

	HighScoreEnemy() :Enemy(NORMAL_ENEMY_HIGHSCORE) {};
	//このクラスでは生成する際にあらかじめ決まった値を入れるので、引数は適当でよい
	void Generate(Vec2 generatePos, Vec2 playerPos, int* hp = nullptr, float* angle = nullptr, Vec2* targetPos = nullptr)override;
	void Update(Vec2 playerPos)override;
};