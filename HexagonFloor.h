#pragma once
#include"Vec2.h"
#include<vector>
#include<forward_list>
#include"DxLib.h"
class HexagonFloor
{
	struct Hexagon
	{
		float x;
		float y;

		//float r = 255.0f;
		//float g = 255.0f;
		//float b = 255.0f;

		float alpha = 255.0f;
		int timer;
		bool listIn = false;

		int blend = DX_BLENDMODE_ALPHA;
	};

	const float OFFSET_X = 26.0f * 0.6f;
	const int FLASH_ALPHA = 130;
	const int DEFAULT_ALPHA = 40;
	//const int HIT_CHECK_RADIUS = 40;
	const int FLASH_TOTAL_TIME = 50;

	std::vector<std::vector<Hexagon>> hexagons;
	std::forward_list<Hexagon*>flashHexagons;
	int hexagonGraph;
	int flashGraph;

public:
	HexagonFloor();
	void Init();
	void Update(Vec2 playerPos, float playerSize);
	void Draw(Vec2 scrollAmount);
};