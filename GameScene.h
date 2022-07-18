#pragma once
#include "BaseScene.h"
#include "Player.h"
#include "ItemManager.h"
#include "GaugeItemManager.h"
#include"SceneTransition.h"
#include"HexagonFloor.h"
#include"Timer.h"
#include"Score.h"
#include"GameFinish.h"

class GameScene :public BaseScene
{
	/*--画像データ--*/
	int screenFrameGraph;									//スクリーンのフレームの画像
	int meterGraph;											//メーターの画像
	int meterIndicaterGraph;								//メーターの針の画像
	int playerDevGraph[7];									//プレイヤーの連番
	int charactorGraph;										//プレイヤーの通常時の画像
	int charactorCloseGraph;								//プレイヤーの瞬き時の画像
	int charactorDamageGraph;								//プレイヤーのダメージ時の画像
	int charactorDamageGraph_default;
	int charactorDamageGraph_close;
	int buffScreen;											//プレイヤーのダメージ時に使用するスクリーン
	int gaugeInnerGraph;									//ゲージの外側の画像
	int gaugeInnerGraph_damaged;
	int gaugeOuterGraph;									//ゲージの内側の画像
	
	//ダメージを受けたときのフレーム
	int damagedFrameGraph;
	int damagedFrameFlashAngle;

	/*--各変数--*/
	Player player;
	float playerShake = 0;									//プレイヤーがヒット時にシェイクする値

	//スピード計に使用
	float meterIndicaterAngle = 0;							//メーターの針の角度

	//BGM
	int bgm;

	//アイテム回収時のエフェクト
	int itemGetSE;
	int finishSE;

	//シーン遷移
	GameFinish gameFinishTrans;

	//床
	HexagonFloor hexagonFloor;

	//ゲームの時間計測
	Timer gameTimer;

public:
	//コンストラクタ
	GameScene(SceneChanger* changer);

	//初期化
	void Initialize()override;
	//終了処理
	void Finalize()override;
	//更新
	void Update()override;
	//描画
	void Draw()override;
};

