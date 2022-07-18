#pragma once
#include"BaseScene.h"
#include"SceneTransition.h"
class StartScene : public BaseScene
{
private:
	float alpha;
	int graph;

	int fadeInTimer;
	int waitTimer;
	int fadeOutTimer;

	bool skip;

	SceneTransition sceneTrans;

public:
	//コンストラクタ
	StartScene(SceneChanger* changer);

	//初期化
	void Initialize()override;
	//終了処理
	void Finalize()override;
	//更新
	void Update()override;
	//描画
	void Draw()override;
};

