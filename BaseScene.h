#pragma once
#include"SceneChanger.h"
#include"Vec2.h"

class BaseScene
{
protected:
	SceneChanger* sceneChanger;

public:
	BaseScene(SceneChanger* changer) :sceneChanger(changer) {};
	virtual ~BaseScene() {};
	virtual void Initialize() = 0;
	virtual void Finalize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
};