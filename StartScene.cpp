#include "StartScene.h"
#include"Easing.h"
#define FADE_IN_TOTAL_TIME 30
#define WAIT_TOTAL_TIME 90
#define FADE_OUT_TOTAL_TIME 40

StartScene::StartScene(SceneChanger* changer) :BaseScene(changer)
{
	graph = LoadGraph("resource/startscreen.png");
}

void StartScene::Initialize()
{
	alpha = 0.0f;
	fadeInTimer = 0;
	waitTimer = WAIT_TOTAL_TIME;
	fadeOutTimer = FADE_OUT_TOTAL_TIME;
	skip = false;
}

void StartScene::Finalize()
{
}

void StartScene::Update()
{
	if (!skip) {
		if (fadeInTimer < FADE_IN_TOTAL_TIME) {
			fadeInTimer++;
			alpha = Easing::Linear(fadeInTimer, FADE_IN_TOTAL_TIME, 0.0f, 255.0f);
			if (fadeInTimer == FADE_IN_TOTAL_TIME) {
				waitTimer = 0;
			}
		}
		if (waitTimer < WAIT_TOTAL_TIME) {
			waitTimer++;
			if (waitTimer == WAIT_TOTAL_TIME) {
				fadeOutTimer = 0;
			}
		}
		if (fadeOutTimer < FADE_OUT_TOTAL_TIME) {
			fadeOutTimer++;
			alpha = Easing::Linear(fadeOutTimer, FADE_IN_TOTAL_TIME, 255.0f, 0.0f);
			if (fadeOutTimer == FADE_OUT_TOTAL_TIME) {
				skip = true;
				fadeOutTimer = 0;
			}
		}
		else {
			if ((Input::Controller() && Input::isJoyBottomTrigger(XINPUT_BUTTON_A))
				|| (!Input::Controller() && (Input::isKeyTrigger(KEY_INPUT_SPACE) || (Input::isMouseBottomTrigger(MOUSE_INPUT_LEFT))))) {
				skip = true;
				fadeOutTimer = 0;
			}
		}
	}
	else {
		if (fadeOutTimer < FADE_OUT_TOTAL_TIME) {
			fadeOutTimer++;
			if (fadeOutTimer == FADE_OUT_TOTAL_TIME) {
				sceneChanger->ChangeScene(TITLE_SCENE, &sceneTrans);
			}
		}
	}
}

void StartScene::Draw()
{
	if (!skip) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawGraph(0, 0, graph, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}
