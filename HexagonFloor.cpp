#include "HexagonFloor.h"
#include"DxLib.h"
#include"Constants.h"
#include"Easing.h"

HexagonFloor::HexagonFloor()
{
	hexagonGraph = LoadGraph("resource/hexagon.png");
	flashGraph = LoadGraph("resource/hexagon_flash.png");

	for (int width = 0; width < BACKGROUND_TILE_COUNT_X; ++width) {
		//‰¡‚Ì”‚ª‹ô”‚¾‚Á‚½‚ç‚½‚Ä‚ð­‚µ‚¸‚ç‚·
		float zure = 0;
		if (width % 2 == 0) {
			zure = BACKGROUND_TILE_SIZE.y / 2.0f;
		}
		hexagons.emplace_back();

		for (int height = 0; height < BACKGROUND_TILE_COUNT_Y; ++height) {
			Hexagon hexagon;
			hexagon.x = BACKGROUND_LEFTUP_POS.x + (width * (BACKGROUND_TILE_SIZE.x - OFFSET_X));
			hexagon.y = BACKGROUND_LEFTUP_POS.y + (height * BACKGROUND_TILE_SIZE.y) + zure;
			hexagons[width].emplace_back(hexagon);
		}
	}
}

void HexagonFloor::Init()
{
	for (int x = 0; x < hexagons.size(); ++x) {
		for (int y = 0; y < hexagons[x].size(); ++y) {
			//hexagons[x][y].r = 255.0f;
			//hexagons[x][y].g = 255.0f;
			//hexagons[x][y].b = 255.0f;
			hexagons[x][y].alpha = DEFAULT_ALPHA;
			hexagons[x][y].blend = DX_BLENDMODE_ALPHA;
		}
	}
}

void HexagonFloor::Update(Vec2 playerPos, float playerSize)
{
	playerSize *= 0.6f;
	for (auto itr : flashHexagons) {
		int time = FLASH_TOTAL_TIME - itr->timer;

		//itr->r = Easing::CircIn(time, FLASH_TOTAL_TIME, 120, 255);
		//itr->b = Easing::CircIn(time, FLASH_TOTAL_TIME, 169, 255);
		//itr->alpha = Easing::CircInOut(time, FLASH_TOTAL_TIME, FLASH_ALPHA, (float)DEFAULT_ALPHA);
		itr->alpha = Easing::CircIn(time, FLASH_TOTAL_TIME, FLASH_ALPHA, (float)DEFAULT_ALPHA);
		itr->timer--;
		if (itr->timer <= 0) {
			itr->listIn = false;
			itr->blend = DX_BLENDMODE_ALPHA;
		}
	}
	//Šù‚ÉŽ€‚ñ‚Å‚¢‚½‚çÁ‚·
	flashHexagons.remove_if(
		[](Hexagon* x) {
			return x->timer <= 0;
		});


	Vec2 poses[4] = {
	{playerPos.x - playerSize / 2,playerPos.y - playerSize / 2 },	//¶ã
	{playerPos.x + playerSize / 2,playerPos.y - playerSize / 2},//‰Eã
	{playerPos.x - playerSize / 2,playerPos.y + playerSize / 2 },	//¶‰º
	{playerPos.x + playerSize / 2,playerPos.y + playerSize / 2},//‰E‰º
	};

	int indexX;
	int indexY;

	for (int i = 0; i < 4; ++i)
	{
		indexX = poses[i].x / (BACKGROUND_TILE_SIZE.x - OFFSET_X);
		indexX -= BACKGROUND_LEFTUP_POS.x / (BACKGROUND_TILE_SIZE.x - OFFSET_X);

		float zure = 0;
		if (indexX % 2 == 0)zure = BACKGROUND_TILE_SIZE.y / 2.0f;
		indexY = (poses[i].y + zure) / BACKGROUND_TILE_SIZE.y;
		indexY -= (BACKGROUND_LEFTUP_POS.y + zure) / BACKGROUND_TILE_SIZE.y;

		Hexagon* target = &hexagons[indexX][indexY];
		target->alpha = FLASH_ALPHA;
		//target->r = 120;
		//target->g = 255;
		//target->b = 169;
		target->timer = FLASH_TOTAL_TIME;
		if (!target->listIn)
		{
			flashHexagons.push_front(target);
			target->listIn = true;
			target->blend = DX_BLENDMODE_ALPHA;
		}
	}
}

void HexagonFloor::Draw(Vec2 scrollAmount)
{
	/*float oldR = 0.0f;
	float oldG = 0.0f;
	float oldB = 0.0f;*/
	float oldA = 0.0f;
	int oldBlend = DX_BLENDMODE_MUL;
	for (int x = 0; x < hexagons.size(); ++x) {
		for (int y = 0; y < hexagons[x].size(); ++y) {

			/*if (oldR != hexagons[x][y].r || oldG != hexagons[x][y].g || oldB != hexagons[x][y].b) {
				SetDrawBright(hexagons[x][y].r, hexagons[x][y].g, hexagons[x][y].b);
			}*/
			if (oldA != hexagons[x][y].alpha || oldBlend != hexagons[x][y].blend) {
				SetDrawBlendMode(hexagons[x][y].blend, hexagons[x][y].alpha);
			}

			if(hexagons[x][y].listIn)
			{
				DrawGraph(hexagons[x][y].x - scrollAmount.x, hexagons[x][y].y - scrollAmount.y, flashGraph, true);
			}
			else
			{
				DrawGraph(hexagons[x][y].x - scrollAmount.x, hexagons[x][y].y - scrollAmount.y, hexagonGraph, true);
			}

			/*oldR = hexagons[x][y].r;
			oldG = hexagons[x][y].g;
			oldB = hexagons[x][y].b;*/
			oldA = hexagons[x][y].alpha;
			oldBlend = hexagons[x][y].blend;
		}
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetDrawBright(255, 255, 255);
}