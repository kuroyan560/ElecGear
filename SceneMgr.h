#pragma once
#include"SceneChanger.h"
#include<vector>

class BaseScene;
class SceneTransition;

class SceneMgr : public SceneChanger
{
private:
    SCENE_NAME nowScene;     //現在のシーン
    SCENE_NAME nextScene;    //次のシーン

    std::vector<BaseScene*> scene;    //シーン配列
    bool gameEnd;   //終了フラグ

    SceneTransition* sceneTransition = nullptr; //シーン遷移

    int mouseCursor;

public:
    SceneMgr();
    ~SceneMgr();
    void Update();
    void Draw();

    void ChangeScene(SCENE_NAME NextScene,SceneTransition* SceneTransition)override;

    void EndGame()override { gameEnd = true; }
    bool GameEndFlag() { return gameEnd; }
};

