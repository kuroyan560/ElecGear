#include "GameFinish.h"
#include"DxLib.h"
#include"Constants.h"
#include"Vec2.h"
#include"Easing.h"

#define FINISH_GRAPH_WIDTH 522.0f
#define FINISH_GRAPH_HEIGHT 101.0f
#define FINISH_GRAPH_X 223
#define FINISH_GRAPH_START_Y -FINISH_GRAPH_HEIGHT
#define FINISH_GRAPH_APPEAR_Y HALF_WIN_HEIGHT - FINISH_GRAPH_HEIGHT / 2.0f
#define FINISH_GRAPH_END_Y WIN_HEIGHT

#define FINISH_APPEAR_TOTAL_TIME 25
#define FINISH_WAIT_TOTAL_TIME 60
#define FINISH_DISAPPEAR_TOTAL_TIME 25

#define NEW_SCENE_APPEAR_TOTAL_TIME 30

GameFinish::GameFinish()
{
	finishGraph = LoadGraph("resource/finish.png");
}

void GameFinish::Update()
{
	changeSceneTrigger = false;
	if (appearTimer < FINISH_APPEAR_TOTAL_TIME) {
		appearTimer++;
		finishY = Easing::QuadOut(appearTimer, FINISH_APPEAR_TOTAL_TIME, FINISH_GRAPH_START_Y, FINISH_GRAPH_APPEAR_Y);
		if (appearTimer == FINISH_APPEAR_TOTAL_TIME) {
			waitTimer = 0;
		}
	}
	if (waitTimer < FINISH_WAIT_TOTAL_TIME) {
		waitTimer++;
		if (waitTimer == FINISH_WAIT_TOTAL_TIME) {
			disappearTimer = 0;
		}
	}
	if (disappearTimer < FINISH_DISAPPEAR_TOTAL_TIME) {
		disappearTimer++;
		finishY = Easing::BackIn(disappearTimer, FINISH_DISAPPEAR_TOTAL_TIME, FINISH_GRAPH_APPEAR_Y, FINISH_GRAPH_END_Y);
		blackAlpha = Easing::CircIn(disappearTimer, FINISH_DISAPPEAR_TOTAL_TIME, 0.0f, 255.0f);
		if (disappearTimer == FINISH_DISAPPEAR_TOTAL_TIME) {
			changeSceneTrigger = true;
			newSceneAppearTimer = 0;
		}
	}

	if (newSceneAppearTimer < NEW_SCENE_APPEAR_TOTAL_TIME) {
		newSceneAppearTimer++;
		blackAlpha = Easing::CircIn(newSceneAppearTimer, NEW_SCENE_APPEAR_TOTAL_TIME, 255.0f, 0.0f);
		if (newSceneAppearTimer == NEW_SCENE_APPEAR_TOTAL_TIME) {
			complete = true;
		}
	}
}

void GameFinish::Draw()
{
	DrawGraph(FINISH_GRAPH_X, finishY, finishGraph, true);

	if (blackAlpha) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, blackAlpha);
		DrawBox(0, 0, WIN_WIDTH, WIN_HEIGHT, GetColor(0, 0, 0), true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void GameFinish::Start()
{
	finishY = FINISH_GRAPH_START_Y;
	appearTimer = 0;
	waitTimer = FINISH_WAIT_TOTAL_TIME;
	disappearTimer = FINISH_DISAPPEAR_TOTAL_TIME;
	newSceneAppearTimer = NEW_SCENE_APPEAR_TOTAL_TIME;
	changeSceneTrigger = false;
	complete = false;

	blackAlpha = 0;
}

bool GameFinish::ChangeTrigger()
{
	return changeSceneTrigger;
}

bool GameFinish::End()
{
	return complete;
}
