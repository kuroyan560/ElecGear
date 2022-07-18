#pragma once
#include"Constants.h"
#include<vector>
#include"Vec2.h"
class EnemyWave
{
protected:
	struct Info
	{
		ENEMY_TYPE type;		//登場する敵の種別
		int appearFrame;		//ウェーブ起動してから何フレーム目で登場するか
		bool appear = false;	//登場させたか
		Vec2 pos;				//登場位置
		Vec2 targetPos = {};	//移動させたい位置
		float angle = 0;		//移動させる角度
		int hp = 0;				//hp
	};

	std::vector<Info>infos;
	int frame;	//起動してからのフレーム数
	int waveLife = 0;	//ウェーブの寿命（最後の敵を登場させたら死ぬ）

public:
	void Init() { isAlive = false; }
	bool isAlive = false;	//起動中か
	void Generate();
	void Update();
	void AddInfo(ENEMY_TYPE type, int appearFrame, Vec2 pos, int hp = 0, float angle = 0, Vec2 targetPos = {});

	virtual bool OnFlag(int frame) = 0;
};

//ゲームスタートしてからのフレーム数で起動するウェーブ
class EnemyWave_Frame : public EnemyWave
{
	int startFrame;	//ゲームスタートしたから< startFrame >目で起動する
public:
	EnemyWave_Frame(int onFrame) :startFrame(onFrame) {};
	bool OnFlag(int frame)override { return startFrame == frame; }
};