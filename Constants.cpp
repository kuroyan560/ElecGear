#include"Constants.h"

/*--ウィンドウ初期化関連--*/
const char TITLE[] = "エレギア";
const int WIN_WIDTH = 1280;
const int HALF_WIN_WIDTH = WIN_WIDTH / 2.0;
const int WIN_HEIGHT = 720;
const int HALF_WIN_HEIGHT = WIN_HEIGHT / 2.0f;

//最初のシーン（デバッグ用）
const SCENE_NAME INIT_SCENE = START_SCENE;

/*--敵情報--*/
const int ENEMY_MAX[ENEMY_TYPE_NUM] = { 70,150,20,1,20,20,40 };	//敵の最大数

/*--プレイヤー弾関係--*/
const int BULLET_GENERATE_NORMAL_TIMER = 5;					//通常弾を生成するときのタイマー
const int BULLET_GENERATE_WAY_TIMER = 20;						//WAY弾を生成するときのタイマー
const int BULLET_GENERATE_MACHINGUN_TIMER = 5;					//MACHINGUN弾を生成するときのタイマー
const float BULLET_WAY_MACHINGUN_ANGLE = PI / 16.0f;		//Way弾を生成するときの角度
const int BULLET_SIZE = 5 * CAMERA_RATE;
const int BULLET_SPEED_MIN = 3;
const int BULLET_SPEED_MAX = 21;
const int BULLET_SPEED_TO_MAX_TIME = 14;
const int R = 255;
const int G = 255;
const int B = 255;

/*--音楽関係--*/
//const int TITLE_BGM_VOLUME = 180;
//const int BGM_VOLUME = 140;
const int TITLE_BGM_VOLUME = 50;
const int BGM_VOLUME = 50;