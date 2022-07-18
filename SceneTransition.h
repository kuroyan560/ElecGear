#pragma once

//シーン遷移基底クラス
class SceneTransition
{
public:
	//仮想関数デストラクタ
	virtual ~SceneTransition() {};
	//更新
	virtual void Update() {};
	//描画
	virtual void Draw() {};
	//遷移開始
	virtual void Start() {};
	//シーンチェンジをする瞬間のトリガー判定
	virtual bool ChangeTrigger() { return true; }
	//遷移が終了したかどうか
	virtual bool End() { return true; }
};