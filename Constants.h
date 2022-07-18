#pragma once

//マクロ命令により１回だけコンパイルされるようにする
#ifndef _CONSTANTS
#define _CONSTANTS

/*--ウィンドウ初期化関連--*/
extern const char TITLE[];							//ウィンドウに表示される名前
extern const int WIN_WIDTH;								//ウィンドウの幅
extern const int HALF_WIN_WIDTH;			//ウィンドウの幅の半分
extern const int WIN_HEIGHT;								//ウィンドウの高さ
extern const int HALF_WIN_HEIGHT;			//ウィンドウの高さの半分

/*--カメラ--*/
#define CAMERA_RATE 0.75f

/*--マップと背景関連--*/
#define MAP_CENTER Vec2(WIN_WIDTH / 2.0f - 150, WIN_HEIGHT / 2.0f)
#define MAP_RADIUS 1000 * CAMERA_RATE															//マップの半径
#define MAP_RADIUS_2_TIMES MAP_RADIUS + MAP_RADIUS								//マップ半径の2倍
#define MAP_RADIUS_3_TIMES MAP_RADIUS + MAP_RADIUS + MAP_RADIUS					//マップ半径の3倍
#define BACKGROUND_LEFTUP_POS Vec2(-MAP_RADIUS * 1, -MAP_RADIUS * 1)			//背景の開始地点
#define BACKGROUND_TILE_SIZE Vec2(53,46)										//背景のタイルのサイズ
#define BACKGROUND_TILE_COUNT_X (MAP_RADIUS * 5.0f) / BACKGROUND_TILE_SIZE.x	//背景のタイルのX方向の数
#define BACKGROUND_TILE_COUNT_Y (MAP_RADIUS * 3.0f) / BACKGROUND_TILE_SIZE.y	//背景のタイルのY方向の数

/*--フォント関連--*/
#define FONT_SIZE 64

/*---π---*/
#define PI 3.14159265359f

//シーンチェンジインターフェース抽象クラス
typedef enum
{
	START_SCENE,TITLE_SCENE, GAME_SCENE, RESULT_SCENE, NONE_SCENE
}SCENE_NAME;

//最初のシーン（デバッグ用）
extern const SCENE_NAME INIT_SCENE;

/*--敵情報--*/
typedef enum
{
	NORMAL_ENEMY_TRACKING,					//通常の敵、追尾
	NORMAL_ENEMY_STRAIGHT,					//通常の敵、生成した角度にまっすぐ移動する
	NORMAL_ENEMY_DESTINATION,				//通常の敵、指定した位置まで移動
	NORMAL_ENEMT_INFINITY,					//通常の敵、無限型に移動し倒されると敵を大量に生成、生成してから600Fの間倒されなかったら逃げる
	NORMAL_ENEMY_STANDBY,					//通常の敵、生成した位置に待機、生成してから600Fの間倒されなかったら逃げる
	NORMAL_ENEMY_HIGHSCORE,					//通常の敵、よくわからない動きをしてから少ししたら逃げる　ハイスコア
	CIRCLE_ENEMY_TRACKING,					//蛇状の敵、追尾
	ENEMY_TYPE_NUM							//敵の種類の数
}ENEMY_TYPE;

extern const int ENEMY_MAX[ENEMY_TYPE_NUM];	//敵の最大数

/*--敵スピード--*/	//円から離れているほどスピード遅い（←ゆっくり外から出現）
#define OUT_OF_MAP_SPEED_RATE_MIN 0.9
#define ENEMY_GENERATE_RADIUS MAP_RADIUS * 1.5f

/*--蛇敵関係--*/
#define CIRCLE_ENEMY_HP 1										//周回敵のHP
#define CIRCLE_ENEMY_FOLLOWING_COUNT 8							//後続敵の数
#define CIRCLE_ENEMY_COUNT_ALL CIRCLE_ENEMY_FOLLOWING_COUNT + 1	//周回敵グループの総数
#define CIRCLE_ENEMY_STATES_DELAY 300							//周回敵のステータスが変わるまでの時間
#define CIRCLE_ENEMY_SPEED 5 * CAMERA_RATE									//周回敵の移動速度
#define CIRCLE_ENEMY_SPEED_DEF 2 * CAMERA_RATE								//周回敵の移動速度の下限
#define CIRCLE_ENEMY_HIT_ACCEL 5								//ヒット時の加速
#define CIRCLE_ENEMY_CHANGE_ANGLE 0.03f							//周回敵が追尾するときの追尾量
#define CIRCLE_ENEMYS_COUNT 5									//周回敵の最大数
#define CIRCLE_ENEMY_GENERATE_TIMER 480							//周回敵を生成するタイマー

/*--通常の敵関係--*/
#define ENEMY_CHANGE_ANGLE 0.03f * CAMERA_RATE								//敵が追尾する際に回転する角度
#define ENEMY_HP 10.0f											//敵のHP
#define ENEMY_COUNT 70											//敵の最大数
#define ENEMY_GENERATE_TIMER 60									//敵を生成するタイマー
#define ENEMY_UNIT_TIMER 900									//隊列敵モードに移動するまでのタイマー
#define ENEMY_ECPACE_SPEED 0.5f * CAMERA_RATE									//敵が逃げる時のスピード

//追尾敵	
#define	TRACKING_ENEMY_MIN_SPEED 3 * CAMERA_RATE
#define TRACKING_ENEMY_MAX_SPEED 4 * CAMERA_RATE
#define TRACKING_ENEMY_DAMAGE_STAN_TIME 2

//まっすぐ敵
#define	STRAIGHT_ENEMY_MIN_SPEED 2.5f * CAMERA_RATE
#define STRAIGHT_ENEMY_MAX_SPEED 3.0f * CAMERA_RATE
#define STRAIGHT_ENEMY_DAMAGE_STAN_TIME 2

//目的地まで移動敵
#define DESTIANTION_ENEMY_SPEED 10
#define DESTINATION_ENEMY_DAMAGE_STAN_TIME 2
#define DESTINATION_ENEMY_ESCAPE_TIME 400

//無限型に移動する敵
#define INFINITY_ENEMY_ROTATION_RADIUS 100
#define INFINITY_ENEMY_ROTATION_LEFT Vec2(HALF_WIN_WIDTH - INFINITY_ENEMY_ROTATION_RADIUS, HALF_WIN_HEIGHT)
#define INFINITY_ENEMY_ROTATION_RIGHT Vec2(HALF_WIN_WIDTH + INFINITY_ENEMY_ROTATION_RADIUS, HALF_WIN_HEIGHT)
#define INFINITY_ENEMY_ROTATION_ADD 0.05f

//生成した位置に停止する敵
#define STANDBY_ENEMY_ESCAPE_COUNT 600

//ハイスコアの敵
#define HIGHTSCORE_ENEMY_RADIUS_MIN MAP_RADIUS / 2.0f			//ハイスコアの敵の回転半径の最小量
#define HIGHTSCORE_ENEMY_RADIUS_CHANGE 10.0f					//ハイスコアの敵の回転半径が小さくなる量
#define HIGHTSCORE_ENEMY_ROTATION_COUNT 3						//ハイスコアの敵の回転量が大きくなっていくまでの回転数
#define HIGHTSCORE_ENEMY_ROTATION_ADD 0.02f						//ハイスコアの敵の回転量

/*--隊列敵関係--*/
#define UNIT_ENEMYS_COUNT 10									//隊列敵のグループの最大数
#define UNIT_ENEMY_HP 1											//隊列敵のHP
#define UNIT_ENEMY_SPEED 3 * CAMERA_RATE						//隊列敵の移動速度
#define UNIT_ENEMY_SIZE 30 * CAMERA_RATE						//隊列敵の大きさ
#define UNIT_ENEMY_COUNT 80										//隊列敵の最大数
#define UNIT_ENEMY_ROUND_COUNT 15.0f							//中心から広がる敵の一周の数
#define UNIT_ENEMY_ROUND_GENERATE_COUNT 3						//中心から広がる敵がワンセットで沸く数
#define UNIT_ENEMY_ROUND_GENERATE_TIMER 90						//中心から広がる敵が沸くまでの遅延
#define UNIT_ENEMY_ROUND_ADDANGLE PI / 2.0f * CAMERA_RATE		//中心から広がる敵の回転する限界量

/* --敵エフェクト関係-- */
#define EFFECT_ENEMYHIT_COUNT_MAX 50                            //敵被弾エフェクトの最大数
#define EFFECT_ENEMYDEAD_EXPLOSION_COUNT_MAX 100
#define EFFECT_ENEMYDEAD_EMITTER_COUNT_MAX 50			//爆発エフェクトエミッター最大数

/*--プレイヤー関係--*/
#define PLAYER_HP 100
#define PLAYER_HP_DAMAGE 5
#define PLAYER_HP_WARING PLAYER_HP / 2
#define PLAYER_HP_ITEM_HEAL 5
#define PLAYER_SHAKE_DEF 10										//プレイヤーがヒット時にシェイクする最大量
#define PLAYER_MAX_SPEED 13 * CAMERA_RATE									//プレイヤーの移動速度の最大値
#define PLAYER_CHANGE_SPEED 0.5f * CAMERA_RATE								//プレイヤーの移動速度を変える量
#define PLAYER_HIT_TIMER 60									//当たり判定がないときの時間
//#define PLAYER_GAUGE_MAX 30										//プレイヤーのゲージの最大値
#define PLAYER_GAUGE_ADD_ENEMY 1								//プレイヤーのゲージに加算する値
#define PLAYER_GAUGE_SIZE Vec2(243, 40)							//プレイヤーのゲージのサイズ

/* --斬撃関係-- */
#define SLASH_RADIUS 250 * CAMERA_RATE                                        //斬撃の攻撃範囲(プレイヤー中心の半径)
#define SLASH_FRAME 14                                            //斬撃を行うフレーム数
#define SLASH_COOLTIME 45                                        //斬撃ができるようになるまでのフレーム数
//#define SLASH_DEATH_COUNT 3                                        //続けて斬撃を出せるようにするための撃破数
#define CAN_SLASH_MAX_COUNT 3	//連続で斬れる回数（斬撃スタミナの最大値）
#define SLASH_GAUGE_HEAL_TIME 60	//斬撃ゲージ１つ分回復までの時間
#define SLASH_BUTTON_FRAME_MAX 20	//ボタン押下量最大取得フレーム
#define SLASH_MOVE_AMOUNT_MIN 5 * CAMERA_RATE	//斬撃の移動量最小
#define SLASH_MOVE_AMOUNT_MAX 50 * CAMERA_RATE	//斬撃の移動量最大

/*--プレイヤー軌道関係--*/
#define PLAYER_ORBIT_MAX 200	//プレイヤー軌道オブジェクトの最大数
#define PLAYER_ORBIT_ALPHA_CHANGE_SPEED 10	//プレイヤー軌道のアルファ値減少量
#define PLAYER_ORBIT_EMIT_SPAN 2	//プレイヤー軌道の排出スパン
#define PLAYER_ORBIT_EMIT_MOVE_LENGTH 0.1f		//軌道排出条件、移動量の大きさ

/*--アイテム関係--*/
#define ITEM_SIZE 30 * CAMERA_RATE											//アイテムのサイズ
#define ITEM_MAX_COUNT 30										//アイテムの最大量
#define ITEM_ROTATION_ANGLE_ADD PI / 16.0f * CAMERA_RATE					//アイテムがプレイヤーの周りを回転するときの回転量
#define ITEM_ROTATION_MAX_RADIUS 100 * CAMERA_RATE							//アイテムがプレイヤーの周りを回転するときの最大半径
#define ITEM_ROTATION_ADD_RADIUS 5 * CAMERA_RATE								//アイテムがプレイヤーの周りを回転するときの加算量
#define ITEM_ROTATION_MINUS_RADIUS 5 * CAMERA_RATE							//アイテムがプレイヤーの周りを回転するときの減算量
#define GAUGEITEM_DELETE_TIMER 600								//ゲージアイテムが自動削除されるまでの時間
#define GAUGEITEM_GENERATE_TIMER 60								//ゲージアイテムが生成されるまでの時間
//アイテムの種類
enum ITEM_ID {
	ITEM_ID_WAY,
	ITEM_ID_MACHINEGUN
};
//アイテムのステータス
enum ITEM_STATES {
	ITEM_STATES_IDOL,				//アイテムが生成されて待機している状態
	ITEM_STATES_OBTAINING,			//アイテムが入手されて回転している状態
	ITEM_STATES_DISAPPEAR			//ほかのアイテムが入手されたので消えていく状態
};

/*--プレイヤー弾関係--*/
static const int BULLET_MAX = 100;										//弾の最大数
extern const int BULLET_GENERATE_NORMAL_TIMER;					//通常弾を生成するときのタイマー
extern const int BULLET_GENERATE_WAY_TIMER;						//WAY弾を生成するときのタイマー
extern const int BULLET_GENERATE_MACHINGUN_TIMER;					//MACHINGUN弾を生成するときのタイマー
extern const float BULLET_WAY_MACHINGUN_ANGLE;		//Way弾を生成するときの角度
extern const int BULLET_SIZE;
extern const int BULLET_SPEED_MIN;
extern const int BULLET_SPEED_MAX;
extern const int BULLET_SPEED_TO_MAX_TIME;
extern const int R;
extern const int G;
extern const int B;

enum BULLET_GENERATE_STATES {
	BULLET_GENERATE_STATES_NORMAL,
	BULLET_GENERATE_STATES_WAY,
	BULLET_GENERATE_STATES_MACHINEGUN
};

/*--スコア関係--*/
#define BULLET_HIT_SCORE 100
#define SLASH_HIT_SCORE 300
#define BULLET_KILL_SCORE 1000
#define SLASH_KILL_SCORE 1000

/*--ダメージ関係--*/
#define PLAYER_HP_DAMAGE 15
#define PLAYER_HP_ITEM_HEAL 5

/*--音楽関係--*/
extern const int TITLE_BGM_VOLUME;
extern const int BGM_VOLUME;
#endif