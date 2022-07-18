#pragma once
#include"SceneTransition.h"
class GameFinish : public SceneTransition
{
	int finishGraph;
	float finishY;

	int appearTimer;
	int waitTimer;
	int disappearTimer;

	int newSceneAppearTimer;

	bool complete = false;
	bool changeSceneTrigger;

	int blackAlpha;

public:
	GameFinish();
	//更新
	void Update()override;
	//描画
	virtual void Draw()override;
	//遷移開始
	virtual void Start()override;
	//シーンチェンジをする瞬間のトリガー判定
	virtual bool ChangeTrigger()override;
	//遷移が終了したかどうか
	virtual bool End()override;
};

