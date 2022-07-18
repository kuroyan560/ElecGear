#pragma once
#include "Singleton.h"
#include "UnitEnemyt.h"
#include"Constants.h"
#include<vector>

class EnemyInterFace;

//敵全体の動きを管理するステータス
enum ENEMY_STATES{
	ENEMY_STATES_NORMAL,			//通常時の敵の処理
	ENEMY_STATES_UNIT				//隊列を組んだ敵を出現させるときの処理
};

class EnemyManager : public Singleton<EnemyManager> {
private:
	friend Singleton<EnemyManager>;

	int enemyCircleOuter;							//敵の外側の画像
	int enemyCircleInner;							//敵の内側の画像
	int enemyDirection;								//敵の進行方向の画像
	int warningGraph;								//注意マーク

	int enemyDamagedSE = -1;
	bool enemyDamageSEPlay = false;
	int enemyDeadSE = -1;
	bool enemyDeadSEPlay = false;

public:
	Vec2* playerPos = nullptr;

	/*--ステータスなどの管理変数--*/
	int states;										//敵全体のモード管理用変数
	int unitTimer;									//unit敵状態に遷移するまでのタイマー

	/*--通常時処理する敵の変数--*/
	//TrackingEnemy normalEnemys[ENEMY_COUNT];				//敵配列
	int generateNormalTimer;						//敵を生成するタイマー
	//CircleEnemys circleEnemys[CIRCLE_ENEMYS_COUNT];	//周回敵の配列
	int generateCircleTimer;						//周回敵を生成するタイマー
	std::vector<std::vector<EnemyInterFace*>>enemys;

	/*--隊列敵など特殊な敵の変数--*/
	UnitEnemyManager unitEnemys[UNIT_ENEMYS_COUNT];	//隊列敵
	int unitStates;									//隊列敵のステータス
	int unitWallDirection;							//壁の敵の枠方角

private:
	//コンストラクタ
	EnemyManager();

public:
	~EnemyManager();

	//初期化処理
	void Init();
	//更新処理
	void Update();
	//描画処理
	void Draw(Vec2 scrollAmount);

	//通常状態での敵の更新処理
	void NormalStatesEnemyUpdate();


	//UNIT状態での敵の更新処理
	void UnitStatesEnemyUpdate();
	//UNIT状態での全体の更新処理
	void UnitStatesUpdate();

	//現在生存している敵を脱出モードにする
	void EscapeNormalEnemy();

	//敵の登場
	void GenerateEnemy(ENEMY_TYPE type, Vec2 pos, int* hp = nullptr, float* angle = nullptr, Vec2* targetPos = nullptr);

	//ダメージSE
	void PlayDamageSE();
	//死亡SE
	void PlayDeadSE();

	void ResetSlashedFlag();

	//すべての敵が死んでいるかを確認 死んでいたらtrue
	bool isAllEnemyDead();
};