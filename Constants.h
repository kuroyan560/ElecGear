#pragma once

//�}�N�����߂ɂ��P�񂾂��R���p�C�������悤�ɂ���
#ifndef _CONSTANTS
#define _CONSTANTS

/*--�E�B���h�E�������֘A--*/
extern const char TITLE[];							//�E�B���h�E�ɕ\������閼�O
extern const int WIN_WIDTH;								//�E�B���h�E�̕�
extern const int HALF_WIN_WIDTH;			//�E�B���h�E�̕��̔���
extern const int WIN_HEIGHT;								//�E�B���h�E�̍���
extern const int HALF_WIN_HEIGHT;			//�E�B���h�E�̍����̔���

/*--�J����--*/
#define CAMERA_RATE 0.75f

/*--�}�b�v�Ɣw�i�֘A--*/
#define MAP_CENTER Vec2(WIN_WIDTH / 2.0f - 150, WIN_HEIGHT / 2.0f)
#define MAP_RADIUS 1000 * CAMERA_RATE															//�}�b�v�̔��a
#define MAP_RADIUS_2_TIMES MAP_RADIUS + MAP_RADIUS								//�}�b�v���a��2�{
#define MAP_RADIUS_3_TIMES MAP_RADIUS + MAP_RADIUS + MAP_RADIUS					//�}�b�v���a��3�{
#define BACKGROUND_LEFTUP_POS Vec2(-MAP_RADIUS * 1, -MAP_RADIUS * 1)			//�w�i�̊J�n�n�_
#define BACKGROUND_TILE_SIZE Vec2(53,46)										//�w�i�̃^�C���̃T�C�Y
#define BACKGROUND_TILE_COUNT_X (MAP_RADIUS * 5.0f) / BACKGROUND_TILE_SIZE.x	//�w�i�̃^�C����X�����̐�
#define BACKGROUND_TILE_COUNT_Y (MAP_RADIUS * 3.0f) / BACKGROUND_TILE_SIZE.y	//�w�i�̃^�C����Y�����̐�

/*--�t�H���g�֘A--*/
#define FONT_SIZE 64

/*---��---*/
#define PI 3.14159265359f

//�V�[���`�F���W�C���^�[�t�F�[�X���ۃN���X
typedef enum
{
	START_SCENE,TITLE_SCENE, GAME_SCENE, RESULT_SCENE, NONE_SCENE
}SCENE_NAME;

//�ŏ��̃V�[���i�f�o�b�O�p�j
extern const SCENE_NAME INIT_SCENE;

/*--�G���--*/
typedef enum
{
	NORMAL_ENEMY_TRACKING,					//�ʏ�̓G�A�ǔ�
	NORMAL_ENEMY_STRAIGHT,					//�ʏ�̓G�A���������p�x�ɂ܂������ړ�����
	NORMAL_ENEMY_DESTINATION,				//�ʏ�̓G�A�w�肵���ʒu�܂ňړ�
	NORMAL_ENEMT_INFINITY,					//�ʏ�̓G�A�����^�Ɉړ����|�����ƓG���ʂɐ����A�������Ă���600F�̊ԓ|����Ȃ������瓦����
	NORMAL_ENEMY_STANDBY,					//�ʏ�̓G�A���������ʒu�ɑҋ@�A�������Ă���600F�̊ԓ|����Ȃ������瓦����
	NORMAL_ENEMY_HIGHSCORE,					//�ʏ�̓G�A�悭�킩��Ȃ����������Ă��班�������瓦����@�n�C�X�R�A
	CIRCLE_ENEMY_TRACKING,					//�֏�̓G�A�ǔ�
	ENEMY_TYPE_NUM							//�G�̎�ނ̐�
}ENEMY_TYPE;

extern const int ENEMY_MAX[ENEMY_TYPE_NUM];	//�G�̍ő吔

/*--�G�X�s�[�h--*/	//�~���痣��Ă���قǃX�s�[�h�x���i���������O����o���j
#define OUT_OF_MAP_SPEED_RATE_MIN 0.9
#define ENEMY_GENERATE_RADIUS MAP_RADIUS * 1.5f

/*--�֓G�֌W--*/
#define CIRCLE_ENEMY_HP 1										//����G��HP
#define CIRCLE_ENEMY_FOLLOWING_COUNT 8							//�㑱�G�̐�
#define CIRCLE_ENEMY_COUNT_ALL CIRCLE_ENEMY_FOLLOWING_COUNT + 1	//����G�O���[�v�̑���
#define CIRCLE_ENEMY_STATES_DELAY 300							//����G�̃X�e�[�^�X���ς��܂ł̎���
#define CIRCLE_ENEMY_SPEED 5 * CAMERA_RATE									//����G�̈ړ����x
#define CIRCLE_ENEMY_SPEED_DEF 2 * CAMERA_RATE								//����G�̈ړ����x�̉���
#define CIRCLE_ENEMY_HIT_ACCEL 5								//�q�b�g���̉���
#define CIRCLE_ENEMY_CHANGE_ANGLE 0.03f							//����G���ǔ�����Ƃ��̒ǔ���
#define CIRCLE_ENEMYS_COUNT 5									//����G�̍ő吔
#define CIRCLE_ENEMY_GENERATE_TIMER 480							//����G�𐶐�����^�C�}�[

/*--�ʏ�̓G�֌W--*/
#define ENEMY_CHANGE_ANGLE 0.03f * CAMERA_RATE								//�G���ǔ�����ۂɉ�]����p�x
#define ENEMY_HP 10.0f											//�G��HP
#define ENEMY_COUNT 70											//�G�̍ő吔
#define ENEMY_GENERATE_TIMER 60									//�G�𐶐�����^�C�}�[
#define ENEMY_UNIT_TIMER 900									//����G���[�h�Ɉړ�����܂ł̃^�C�}�[
#define ENEMY_ECPACE_SPEED 0.5f * CAMERA_RATE									//�G�������鎞�̃X�s�[�h

//�ǔ��G	
#define	TRACKING_ENEMY_MIN_SPEED 3 * CAMERA_RATE
#define TRACKING_ENEMY_MAX_SPEED 4 * CAMERA_RATE
#define TRACKING_ENEMY_DAMAGE_STAN_TIME 2

//�܂������G
#define	STRAIGHT_ENEMY_MIN_SPEED 2.5f * CAMERA_RATE
#define STRAIGHT_ENEMY_MAX_SPEED 3.0f * CAMERA_RATE
#define STRAIGHT_ENEMY_DAMAGE_STAN_TIME 2

//�ړI�n�܂ňړ��G
#define DESTIANTION_ENEMY_SPEED 10
#define DESTINATION_ENEMY_DAMAGE_STAN_TIME 2
#define DESTINATION_ENEMY_ESCAPE_TIME 400

//�����^�Ɉړ�����G
#define INFINITY_ENEMY_ROTATION_RADIUS 100
#define INFINITY_ENEMY_ROTATION_LEFT Vec2(HALF_WIN_WIDTH - INFINITY_ENEMY_ROTATION_RADIUS, HALF_WIN_HEIGHT)
#define INFINITY_ENEMY_ROTATION_RIGHT Vec2(HALF_WIN_WIDTH + INFINITY_ENEMY_ROTATION_RADIUS, HALF_WIN_HEIGHT)
#define INFINITY_ENEMY_ROTATION_ADD 0.05f

//���������ʒu�ɒ�~����G
#define STANDBY_ENEMY_ESCAPE_COUNT 600

//�n�C�X�R�A�̓G
#define HIGHTSCORE_ENEMY_RADIUS_MIN MAP_RADIUS / 2.0f			//�n�C�X�R�A�̓G�̉�]���a�̍ŏ���
#define HIGHTSCORE_ENEMY_RADIUS_CHANGE 10.0f					//�n�C�X�R�A�̓G�̉�]���a���������Ȃ��
#define HIGHTSCORE_ENEMY_ROTATION_COUNT 3						//�n�C�X�R�A�̓G�̉�]�ʂ��傫���Ȃ��Ă����܂ł̉�]��
#define HIGHTSCORE_ENEMY_ROTATION_ADD 0.02f						//�n�C�X�R�A�̓G�̉�]��

/*--����G�֌W--*/
#define UNIT_ENEMYS_COUNT 10									//����G�̃O���[�v�̍ő吔
#define UNIT_ENEMY_HP 1											//����G��HP
#define UNIT_ENEMY_SPEED 3 * CAMERA_RATE						//����G�̈ړ����x
#define UNIT_ENEMY_SIZE 30 * CAMERA_RATE						//����G�̑傫��
#define UNIT_ENEMY_COUNT 80										//����G�̍ő吔
#define UNIT_ENEMY_ROUND_COUNT 15.0f							//���S����L����G�̈���̐�
#define UNIT_ENEMY_ROUND_GENERATE_COUNT 3						//���S����L����G�������Z�b�g�ŕ�����
#define UNIT_ENEMY_ROUND_GENERATE_TIMER 90						//���S����L����G�������܂ł̒x��
#define UNIT_ENEMY_ROUND_ADDANGLE PI / 2.0f * CAMERA_RATE		//���S����L����G�̉�]������E��

/* --�G�G�t�F�N�g�֌W-- */
#define EFFECT_ENEMYHIT_COUNT_MAX 50                            //�G��e�G�t�F�N�g�̍ő吔
#define EFFECT_ENEMYDEAD_EXPLOSION_COUNT_MAX 100
#define EFFECT_ENEMYDEAD_EMITTER_COUNT_MAX 50			//�����G�t�F�N�g�G�~�b�^�[�ő吔

/*--�v���C���[�֌W--*/
#define PLAYER_HP 100
#define PLAYER_HP_DAMAGE 5
#define PLAYER_HP_WARING PLAYER_HP / 2
#define PLAYER_HP_ITEM_HEAL 5
#define PLAYER_SHAKE_DEF 10										//�v���C���[���q�b�g���ɃV�F�C�N����ő��
#define PLAYER_MAX_SPEED 13 * CAMERA_RATE									//�v���C���[�̈ړ����x�̍ő�l
#define PLAYER_CHANGE_SPEED 0.5f * CAMERA_RATE								//�v���C���[�̈ړ����x��ς����
#define PLAYER_HIT_TIMER 60									//�����蔻�肪�Ȃ��Ƃ��̎���
//#define PLAYER_GAUGE_MAX 30										//�v���C���[�̃Q�[�W�̍ő�l
#define PLAYER_GAUGE_ADD_ENEMY 1								//�v���C���[�̃Q�[�W�ɉ��Z����l
#define PLAYER_GAUGE_SIZE Vec2(243, 40)							//�v���C���[�̃Q�[�W�̃T�C�Y

/* --�a���֌W-- */
#define SLASH_RADIUS 250 * CAMERA_RATE                                        //�a���̍U���͈�(�v���C���[���S�̔��a)
#define SLASH_FRAME 14                                            //�a�����s���t���[����
#define SLASH_COOLTIME 45                                        //�a�����ł���悤�ɂȂ�܂ł̃t���[����
//#define SLASH_DEATH_COUNT 3                                        //�����Ďa�����o����悤�ɂ��邽�߂̌��j��
#define CAN_SLASH_MAX_COUNT 3	//�A���Ŏa���񐔁i�a���X�^�~�i�̍ő�l�j
#define SLASH_GAUGE_HEAL_TIME 60	//�a���Q�[�W�P���񕜂܂ł̎���
#define SLASH_BUTTON_FRAME_MAX 20	//�{�^�������ʍő�擾�t���[��
#define SLASH_MOVE_AMOUNT_MIN 5 * CAMERA_RATE	//�a���̈ړ��ʍŏ�
#define SLASH_MOVE_AMOUNT_MAX 50 * CAMERA_RATE	//�a���̈ړ��ʍő�

/*--�v���C���[�O���֌W--*/
#define PLAYER_ORBIT_MAX 200	//�v���C���[�O���I�u�W�F�N�g�̍ő吔
#define PLAYER_ORBIT_ALPHA_CHANGE_SPEED 10	//�v���C���[�O���̃A���t�@�l������
#define PLAYER_ORBIT_EMIT_SPAN 2	//�v���C���[�O���̔r�o�X�p��
#define PLAYER_ORBIT_EMIT_MOVE_LENGTH 0.1f		//�O���r�o�����A�ړ��ʂ̑傫��

/*--�A�C�e���֌W--*/
#define ITEM_SIZE 30 * CAMERA_RATE											//�A�C�e���̃T�C�Y
#define ITEM_MAX_COUNT 30										//�A�C�e���̍ő��
#define ITEM_ROTATION_ANGLE_ADD PI / 16.0f * CAMERA_RATE					//�A�C�e�����v���C���[�̎������]����Ƃ��̉�]��
#define ITEM_ROTATION_MAX_RADIUS 100 * CAMERA_RATE							//�A�C�e�����v���C���[�̎������]����Ƃ��̍ő唼�a
#define ITEM_ROTATION_ADD_RADIUS 5 * CAMERA_RATE								//�A�C�e�����v���C���[�̎������]����Ƃ��̉��Z��
#define ITEM_ROTATION_MINUS_RADIUS 5 * CAMERA_RATE							//�A�C�e�����v���C���[�̎������]����Ƃ��̌��Z��
#define GAUGEITEM_DELETE_TIMER 600								//�Q�[�W�A�C�e���������폜�����܂ł̎���
#define GAUGEITEM_GENERATE_TIMER 60								//�Q�[�W�A�C�e�������������܂ł̎���
//�A�C�e���̎��
enum ITEM_ID {
	ITEM_ID_WAY,
	ITEM_ID_MACHINEGUN
};
//�A�C�e���̃X�e�[�^�X
enum ITEM_STATES {
	ITEM_STATES_IDOL,				//�A�C�e������������đҋ@���Ă�����
	ITEM_STATES_OBTAINING,			//�A�C�e�������肳��ĉ�]���Ă�����
	ITEM_STATES_DISAPPEAR			//�ق��̃A�C�e�������肳�ꂽ�̂ŏ����Ă������
};

/*--�v���C���[�e�֌W--*/
static const int BULLET_MAX = 100;										//�e�̍ő吔
extern const int BULLET_GENERATE_NORMAL_TIMER;					//�ʏ�e�𐶐�����Ƃ��̃^�C�}�[
extern const int BULLET_GENERATE_WAY_TIMER;						//WAY�e�𐶐�����Ƃ��̃^�C�}�[
extern const int BULLET_GENERATE_MACHINGUN_TIMER;					//MACHINGUN�e�𐶐�����Ƃ��̃^�C�}�[
extern const float BULLET_WAY_MACHINGUN_ANGLE;		//Way�e�𐶐�����Ƃ��̊p�x
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

/*--�X�R�A�֌W--*/
#define BULLET_HIT_SCORE 100
#define SLASH_HIT_SCORE 300
#define BULLET_KILL_SCORE 1000
#define SLASH_KILL_SCORE 1000

/*--�_���[�W�֌W--*/
#define PLAYER_HP_DAMAGE 15
#define PLAYER_HP_ITEM_HEAL 5

/*--���y�֌W--*/
extern const int TITLE_BGM_VOLUME;
extern const int BGM_VOLUME;
#endif