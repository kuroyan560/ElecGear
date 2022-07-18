#include "PlayerOrbit.h"
#include"DxLib.h"

PlayerOrbit::PlayerOrbit()
{
	graph = LoadGraph("resource/player_orbit.png");
}

void PlayerOrbit::Init()
{
	for (int i = 0; i < PLAYER_ORBIT_MAX; ++i) {
		orbits[i].isAlive = false;
	}
}

void PlayerOrbit::Update(bool move, Vec2 playerPos, float angle)
{
	//各orbitの更新
	for (int i = 0; i < PLAYER_ORBIT_MAX; ++i) {
		if (!orbits[i].isAlive)continue;
		orbits[i].alpha -= PLAYER_ORBIT_ALPHA_CHANGE_SPEED;
		if (orbits[i].alpha <= 0)orbits[i].isAlive = false;
	}

	//エミット
	if (move) {
		emitTimer++;
		if (emitTimer == PLAYER_ORBIT_EMIT_SPAN) {
			emitTimer = 0;

			for (int i = 0; i < PLAYER_ORBIT_MAX; ++i) {
				if (orbits[i].isAlive)continue;

				orbits[i].isAlive = true;
				orbits[i].alpha = 255;
				orbits[i].angle = angle;
				orbits[i].pos = playerPos;
				break;
			}
		}
	}
}

void PlayerOrbit::Draw(Vec2 scrollAmount)
{
	for (int i = 0; i < PLAYER_ORBIT_MAX; ++i) {
		if (!orbits[i].isAlive)continue;

		//SetDrawBlendMode(DX_BLENDMODE_ADD, orbits[i].alpha);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, orbits[i].alpha);
		//DrawGraph(orbits[i].pos.x - scrollAmount.x, orbits[i].pos.y - scrollAmount.y, graph, true);
		DrawRotaGraph(orbits[i].pos.x - scrollAmount.x, orbits[i].pos.y - scrollAmount.y, 1.0f, orbits[i].angle, graph, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}