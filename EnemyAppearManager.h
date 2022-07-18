#pragma once
#include"EnemyWave.h"
#include"Singleton.h"
class EnemyAppearManager : public Singleton<EnemyAppearManager>
{
	friend class Singleton<EnemyAppearManager>;
	std::vector<EnemyWave_Frame>waves;
	int frame;

	EnemyAppearManager();
public:
	void Init();
	void Update();
};