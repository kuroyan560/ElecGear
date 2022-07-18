#pragma once
#include "BaseScene.h"
#include "Player.h"
#include "ItemManager.h"
#include "GaugeItemManager.h"
#include"SceneTransition.h"
#include"HexagonFloor.h"
#include"Timer.h"
#include"Score.h"
#include"GameFinish.h"

class GameScene :public BaseScene
{
	/*--�摜�f�[�^--*/
	int screenFrameGraph;									//�X�N���[���̃t���[���̉摜
	int meterGraph;											//���[�^�[�̉摜
	int meterIndicaterGraph;								//���[�^�[�̐j�̉摜
	int playerDevGraph[7];									//�v���C���[�̘A��
	int charactorGraph;										//�v���C���[�̒ʏ펞�̉摜
	int charactorCloseGraph;								//�v���C���[�̏u�����̉摜
	int charactorDamageGraph;								//�v���C���[�̃_���[�W���̉摜
	int charactorDamageGraph_default;
	int charactorDamageGraph_close;
	int buffScreen;											//�v���C���[�̃_���[�W���Ɏg�p����X�N���[��
	int gaugeInnerGraph;									//�Q�[�W�̊O���̉摜
	int gaugeInnerGraph_damaged;
	int gaugeOuterGraph;									//�Q�[�W�̓����̉摜
	
	//�_���[�W���󂯂��Ƃ��̃t���[��
	int damagedFrameGraph;
	int damagedFrameFlashAngle;

	/*--�e�ϐ�--*/
	Player player;
	float playerShake = 0;									//�v���C���[���q�b�g���ɃV�F�C�N����l

	//�X�s�[�h�v�Ɏg�p
	float meterIndicaterAngle = 0;							//���[�^�[�̐j�̊p�x

	//BGM
	int bgm;

	//�A�C�e��������̃G�t�F�N�g
	int itemGetSE;
	int finishSE;

	//�V�[���J��
	GameFinish gameFinishTrans;

	//��
	HexagonFloor hexagonFloor;

	//�Q�[���̎��Ԍv��
	Timer gameTimer;

public:
	//�R���X�g���N�^
	GameScene(SceneChanger* changer);

	//������
	void Initialize()override;
	//�I������
	void Finalize()override;
	//�X�V
	void Update()override;
	//�`��
	void Draw()override;
};

