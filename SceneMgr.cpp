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

	//�����ŃV�[���ǉ�
	scene.push_back(new StartScene(this));
	scene.push_back(new TitleScene(this));
	scene.push_back(new GameScene(this));
	scene.push_back(new ResultScene(this));

	//�ŏ��̃V�[��������
	scene[nowScene]->Initialize();

	//�V�[���J�ڃC���X�^���X����
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
	if (nextScene != NONE_SCENE && sceneTransition != nullptr && sceneTransition->ChangeTrigger())	//���̃V�[�����Z�b�g����Ă�����
	{
		scene[nowScene]->Finalize();	//���݂̃V�[���̏I������
		scene[nextScene]->Initialize(); //���̃V�[���̏���������

		nowScene = nextScene;		//���݂̃V�[�����X�V
		nextScene = NONE_SCENE;	//���̃V�[������ɂ���
	}

	SoundEmitter::Instance()->Update();

	//���݃V�[���J�ڒ���
	if (sceneTransition != nullptr)
	{
		sceneTransition->Update();	//�V�[���J�ڂ̍X�V

		if (sceneTransition->End())	//�V�[���J�ڂ��I��������
		{
			sceneTransition = nullptr;
		}
	}
	else
	{
		scene[nowScene]->Update();	//���݃Z�b�g����Ă���V�[���̍X�V
	}
}

void SceneMgr::Draw()
{
	scene[nowScene]->Draw();//���݃Z�b�g����Ă���V�[���̕`��

	//���݃V�[���J�ڒ���
	if (sceneTransition != nullptr)
	{
		sceneTransition->Draw();	//�V�[���J�ڂ̕`��
	}

	if (!Input::Controller()) {
		DrawRotaGraph(Input::GetMousePos().x, Input::GetMousePos().y, 0.15f, 0.0f, mouseCursor, true);
	}
}

void SceneMgr::ChangeScene(SCENE_NAME NextScene, SceneTransition* SceneTransition)
{
	nextScene = NextScene;
	sceneTransition = SceneTransition;
	sceneTransition->Start();	//�V�[���J�ڃX�^�[�g
}