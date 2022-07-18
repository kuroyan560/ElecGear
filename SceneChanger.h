#pragma once
#include"Constants.h"

class SceneTransition;
class SceneChanger
{
public:
	virtual ~SceneChanger() {};
	virtual void ChangeScene(SCENE_NAME NextScene, SceneTransition* SceneTransition) = 0;
	virtual void EndGame() = 0;
};