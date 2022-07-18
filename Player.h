#pragma once
#include "Vec2.h"
#include"PlayerOrbit.h"

class Player {
public:
	int hp;

	/*プレイヤー基本情報*/
	Vec2 pos;							//プレイヤーの座標
	Vec2 vel;							//プレイヤーの移動する角度
	Vec2 move;
	Vec2 moveBuff;
	float size;							//プレイヤーのサイズ
	float speed;						//プレイヤーの移動速度
	float playerAngle;					//弾を射出する方向
	int playerHeadGraph;				//プレイヤー頭の画像
	int playerArmGraph;					//プレイヤーの腕の画像
	int itemGetGraph;					//アイテムゲットエフェクト画像
	int hitTimer;						//あたった時に操作不能&無敵時間になるタイマー
	int bulletStates;					//弾のステータス

	int damagedSE;

	////float gauge;						//プレイヤーのゲージ
	//float hpGauge;

	int slashSE[3];	//斬撃SE
	int maxSlashSE;	//斬撃の移動量最大
	int slashRangeGraph;	//斬撃半円
	int slashGaugeGraph;	//斬撃ゲージ
	int slashGaugeDrawTimer[3];	//斬撃ゲージのレート
	int nonSlashTimer;
	int slashGaugeHealSE[3];

	/*--アイテムゲットエフェクト--*/
	static const int ITEM_GET_TOTAL_TIME = 60;
	int itemGetTimer;

	/*--反動に必要な変数--*/
	float reactionTime;					//反動が起きている(戻る時間も含めて)合計時間
	float reactionTimer;				//反動が起きている(戻る時間も含めて)時間
	float armReactionPos;				//反動が起きた時に動いた量を加算する値
	bool isReaction;					//反動が起きているかどうか
	/*--斬撃に必要な変数--*/
	bool isSlash;						//斬撃をしているかどうかのフラグ
	bool canSlash;						//斬撃ができるかどうかのフラグ
	int slashCooltime;					//斬撃ができるようになるまでのクールタイム
	//int slashDeathCount;				//斬撃中に殺した数
	int slashFrame;						//斬撃のフレーム数
	float slashRadius;					//斬撃の半径
	int slashInvincibleTime;			//斬撃中の無敵時間
	bool isSlashAnime;					//斬撃アニメーションを描画するかどうか
	int slashAnimeNumber;				//斬撃のアニメーションのアニメーションナンバー
	int slashAnimeTimer;				//斬撃のアニメーションナンバーを変えるタイマー
	Vec2 slashMoveBuff;					//斬撃時の移動量を保存
	//（新規追加）
	int slashGauge;
	int slashGaugeTimer;
	int slashButtonFrame;
	float buttonInputRate;

	Vec2 scrollAmount;					//移動した量

	/*--軌道--*/
	PlayerOrbit orbit;

	/*--斬撃--*/
	int slashStartGraph;
	int slashGraph[5];					//斬撃の画像
	int slashEndGraph;

	bool slashFinishTrigger = false;

public:
	//コンストラクタ
	Player();

	//初期化処理
	void Init();
	//更新処理
	void Update(bool isShoot, int shootDelay);
	//描画処理
	void Draw();

	//あたったときの処理
	void Hit();

	void EmitItemGetEffect();
};