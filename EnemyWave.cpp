#include "EnemyWave.h"
#include"EnemyManager.h"
#include "EnemyAppearEffectEmitter.h"

void EnemyWave::Generate()
{
	isAlive = true;
	frame = 0;
	for (auto info : infos) {
		info.appear = false;
	}
}

void EnemyWave::Update()
{
	for (auto info : infos) {
		if (info.appear == true)continue;

		//敵を生成する
		if (info.appearFrame == frame) {
			//角度が0だったらnullptr、値が入ってたらその値を入れる
			float* angleBuff = nullptr;
			if (info.angle == 0) {
			}
			else {
				angleBuff = &info.angle;
			}
			//目標位置が{0,0}だったらnullptr、値が入っていたらその値を入れる
			Vec2* targetBuff = nullptr;
			if (info.targetPos.x == 0 && info.targetPos.y == 0) {
			}
			else {
				targetBuff = &info.targetPos;
			}
			int* hp = nullptr;
			if(info.hp == 0){
			}else{
				hp = &info.hp;
			}

			//敵を生成する処理
			EnemyManager::Instance()->GenerateEnemy(info.type, info.pos, hp, angleBuff, targetBuff);
			info.appear = true;
			
			//敵出現エフェクトを生成
			EnemyAppearEffectEmitter::Instance()->Generate(info.pos);
		}
	}
	if (frame == waveLife) {
		isAlive = false;
	}
	frame++;

	//敵がすべて死んでいたら次のウェーブへ
	//if(EnemyManager::Instance()->isAllEnemyDead()){
		//isAlive = false;
	//}

}

void EnemyWave::AddInfo(ENEMY_TYPE type, int appearFrame, Vec2 pos, int hp, float angle, Vec2 targetPos)
{
	Info info;
	info.type = type;
	info.appearFrame = appearFrame;
	info.pos = pos;
	//angleがnullptrじゃなかったらangleも追加
	if (angle != 0) {
		info.angle = angle;
	}
	else {
		info.angle = 0;
	}
	//targetPosに値が入っていたら追加
	if (targetPos.x == 0 && targetPos.y == 0) {
		info.targetPos = {};
	}
	else {
		info.targetPos = targetPos;
	}
	info.hp = hp;


	infos.emplace_back(info);

	//登場が一番遅い敵のフレームがウェーブの寿命
	if (waveLife < appearFrame)waveLife = appearFrame;
}
