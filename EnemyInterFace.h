#pragma once
#include"Constants.h"
#include"Vec2.h"
class EnemyInterFace
{

protected:
	EnemyInterFace(ENEMY_TYPE type) :type(type) {};
	ENEMY_TYPE type;

public:
	int hp;
	bool isAlive;						//ìGÇ™ê∂ë∂ÇµÇƒÇ¢ÇÈÇ©ÇÃÉtÉâÉO
	bool slashed;
	virtual void Init() = 0;
	virtual void Generate(Vec2 generatePos, Vec2 playerPos, int* hp = nullptr, float* angle = nullptr, Vec2* targetPos = nullptr) = 0;	//ê∂ê¨
	virtual void Update(Vec2 playerPos) = 0;
	virtual void Draw(Vec2 scrollAmount, int enemyCircleOuter, int enemyCircleInner, int enemyDirection, int warningGraph, Vec2 playerPos) = 0;
	virtual bool HitCheck(Vec2 pos, float size, bool player,bool* addScore = nullptr) = 0;
	virtual bool HitCheckSlash(Vec2 pos, float angle, int& deathCount, int& hitCount) = 0;
	virtual void Escape() = 0;
};