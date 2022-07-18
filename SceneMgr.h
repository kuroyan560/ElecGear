#pragma once
#include"SceneChanger.h"
#include<vector>

class BaseScene;
class SceneTransition;

class SceneMgr : public SceneChanger
{
private:
    SCENE_NAME nowScene;     //���݂̃V�[��
    SCENE_NAME nextScene;    //���̃V�[��

    std::vector<BaseScene*> scene;    //�V�[���z��
    bool gameEnd;   //�I���t���O

    SceneTransition* sceneTransition = nullptr; //�V�[���J��

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

