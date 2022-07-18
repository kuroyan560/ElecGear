#include"Constants.h"

/*--�E�B���h�E�������֘A--*/
const char TITLE[] = "�G���M�A";
const int WIN_WIDTH = 1280;
const int HALF_WIN_WIDTH = WIN_WIDTH / 2.0;
const int WIN_HEIGHT = 720;
const int HALF_WIN_HEIGHT = WIN_HEIGHT / 2.0f;

//�ŏ��̃V�[���i�f�o�b�O�p�j
const SCENE_NAME INIT_SCENE = START_SCENE;

/*--�G���--*/
const int ENEMY_MAX[ENEMY_TYPE_NUM] = { 70,150,20,1,20,20,40 };	//�G�̍ő吔

/*--�v���C���[�e�֌W--*/
const int BULLET_GENERATE_NORMAL_TIMER = 5;					//�ʏ�e�𐶐�����Ƃ��̃^�C�}�[
const int BULLET_GENERATE_WAY_TIMER = 20;						//WAY�e�𐶐�����Ƃ��̃^�C�}�[
const int BULLET_GENERATE_MACHINGUN_TIMER = 5;					//MACHINGUN�e�𐶐�����Ƃ��̃^�C�}�[
const float BULLET_WAY_MACHINGUN_ANGLE = PI / 16.0f;		//Way�e�𐶐�����Ƃ��̊p�x
const int BULLET_SIZE = 5 * CAMERA_RATE;
const int BULLET_SPEED_MIN = 3;
const int BULLET_SPEED_MAX = 21;
const int BULLET_SPEED_TO_MAX_TIME = 14;
const int R = 255;
const int G = 255;
const int B = 255;

/*--���y�֌W--*/
//const int TITLE_BGM_VOLUME = 180;
//const int BGM_VOLUME = 140;
const int TITLE_BGM_VOLUME = 50;
const int BGM_VOLUME = 50;