#include "SceneMgr.h"
#include"DxLib.h"
#include"TitleScene.h"
#include"GameScene.h"
#include"CharacterLogin.h"
#include"SoundEmitter.h"
#include"ResultScene.h"
#include"StartScene.h"

SceneMgr::SceneMgr()
{
	nextScene = NONE_SCENE;
	nowScene = INIT_SCENE;

	//ここでシーン追加
	scene.push_back(new StartScene(this));
	scene.push_back(new TitleScene(this));
	scene.push_back(new GameScene(this));
	scene.push_back(new ResultScene(this));

	//最初のシーン初期化
	scene[nowScene]->Initialize();

	//シーン遷移インスタンス生成
	sceneTransition = new CharacterLogin;

	mouseCursor = LoadGraph("resource/mouseCursor.png");
}

SceneMgr::~SceneMgr()
{
	scene[nowScene]->Finalize();
	for (int i = 0; i < scene.size(); i++)
	{
		delete scene[i];
	}
}

void SceneMgr::Update()
{
	if (nextScene != NONE_SCENE && sceneTransition != nullptr && sceneTransition->ChangeTrigger())	//次のシーンがセットされていたら
	{
		scene[nowScene]->Finalize();	//現在のシーンの終了処理
		scene[nextScene]->Initialize(); //次のシーンの初期化処理

		nowScene = nextScene;		//現在のシーンを更新
		nextScene = NONE_SCENE;	//次のシーンを空にする
	}

	SoundEmitter::Instance()->Update();

	//現在シーン遷移中か
	if (sceneTransition != nullptr)
	{
		sceneTransition->Update();	//シーン遷移の更新

		if (sceneTransition->End())	//シーン遷移が終了したら
		{
			sceneTransition = nullptr;
		}
	}
	else
	{
		scene[nowScene]->Update();	//現在セットされているシーンの更新
	}
}

void SceneMgr::Draw()
{
	scene[nowScene]->Draw();//現在セットされているシーンの描画

	//現在シーン遷移中か
	if (sceneTransition != nullptr)
	{
		sceneTransition->Draw();	//シーン遷移の描画
	}

	if (!Input::Controller()) {
		DrawRotaGraph(Input::GetMousePos().x, Input::GetMousePos().y, 0.15f, 0.0f, mouseCursor, true);
	}
}

void SceneMgr::ChangeScene(SCENE_NAME NextScene, SceneTransition* SceneTransition)
{
	nextScene = NextScene;
	sceneTransition = SceneTransition;
	sceneTransition->Start();	//シーン遷移スタート
}