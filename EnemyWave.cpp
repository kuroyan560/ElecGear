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

		//�G�𐶐�����
		if (info.appearFrame == frame) {
			//�p�x��0��������nullptr�A�l�������Ă��炻�̒l������
			float* angleBuff = nullptr;
			if (info.angle == 0) {
			}
			else {
				angleBuff = &info.angle;
			}
			//�ڕW�ʒu��{0,0}��������nullptr�A�l�������Ă����炻�̒l������
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

			//�G�𐶐����鏈��
			EnemyManager::Instance()->GenerateEnemy(info.type, info.pos, hp, angleBuff, targetBuff);
			info.appear = true;
			
			//�G�o���G�t�F�N�g�𐶐�
			EnemyAppearEffectEmitter::Instance()->Generate(info.pos);
		}
	}
	if (frame == waveLife) {
		isAlive = false;
	}
	frame++;

	//�G�����ׂĎ���ł����玟�̃E�F�[�u��
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
	//angle��nullptr����Ȃ�������angle���ǉ�
	if (angle != 0) {
		info.angle = angle;
	}
	else {
		info.angle = 0;
	}
	//targetPos�ɒl�������Ă�����ǉ�
	if (targetPos.x == 0 && targetPos.y == 0) {
		info.targetPos = {};
	}
	else {
		info.targetPos = targetPos;
	}
	info.hp = hp;


	infos.emplace_back(info);

	//�o�ꂪ��Ԓx���G�̃t���[�����E�F�[�u�̎���
	if (waveLife < appearFrame)waveLife = appearFrame;
}
