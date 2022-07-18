#include "EnemyAppearManager.h"

EnemyAppearManager::EnemyAppearManager()
{
	/*---�����ŃE�F�[�u����---*/
	//���E�F�[�u�̐���
	EnemyWave_Frame waveBuff(0);
	waveBuff.Init();
	//�ŏ��ɉ�ʒ��S�����苗�����ꂽ�ʒu�Ɍܑ̖̂ړI�n�G���o��
	for (int i = 0; i < 5; ++i) {
		float generateAngle = DX_TWO_PI_F * (i / 5.0f);
		Vec2 generatePos = Vec2(cosf(generateAngle) * MAP_RADIUS + HALF_WIN_WIDTH, sinf(generateAngle) * MAP_RADIUS + HALF_WIN_HEIGHT);
		Vec2 targetPos = Vec2(cosf(generateAngle) * (MAP_RADIUS / 2.0f) + HALF_WIN_WIDTH, sinf(generateAngle) * (MAP_RADIUS / 2.0f) + HALF_WIN_HEIGHT);
		waveBuff.AddInfo(NORMAL_ENEMY_DESTINATION, 30, generatePos, 0, 0, targetPos);
	}
	//���t���[���o�ߌ�ɉ�ʒ��S�����苗�����ꂽ�ʒu�Ɍܑ̖̂ړI�n�G���o��
	for (int i = 0; i < 5; ++i) {
		float generateAngle = DX_TWO_PI_F * (i / 5.0f) + DX_PI_F;
		Vec2 generatePos = Vec2(cosf(generateAngle) * MAP_RADIUS + HALF_WIN_WIDTH, sinf(generateAngle) * MAP_RADIUS + HALF_WIN_HEIGHT);
		Vec2 targetPos = Vec2(cosf(generateAngle) * (MAP_RADIUS / 2.0f + MAP_RADIUS / 4.0f) + HALF_WIN_WIDTH, sinf(generateAngle) * (MAP_RADIUS / 2.0f + MAP_RADIUS / 4.0f) + HALF_WIN_HEIGHT);
		waveBuff.AddInfo(NORMAL_ENEMY_DESTINATION, 300, generatePos, 0, 0, targetPos);
	}

	waves.emplace_back(waveBuff);

	//���E�F�[�u�̐���
	EnemyWave_Frame waveBuff2(600);
	waveBuff2.Init();
	//�܂��ŏ��Ƀ��A�I�����Ԋu�ŏo��
	waveBuff2.AddInfo(NORMAL_ENEMY_HIGHSCORE, 0, Vec2(HALF_WIN_WIDTH + MAP_RADIUS, HALF_WIN_HEIGHT));		//���A�I�̈����͌^���ƊJ�n�t���[���ȊO�Ӗ��������Ȃ��ł�
	waveBuff2.AddInfo(NORMAL_ENEMY_HIGHSCORE, 120, Vec2(HALF_WIN_WIDTH + MAP_RADIUS, HALF_WIN_HEIGHT));
	waveBuff2.AddInfo(NORMAL_ENEMY_HIGHSCORE, 240, Vec2(HALF_WIN_WIDTH + MAP_RADIUS, HALF_WIN_HEIGHT));
	waveBuff2.AddInfo(NORMAL_ENEMY_HIGHSCORE, 360, Vec2(HALF_WIN_WIDTH + MAP_RADIUS, HALF_WIN_HEIGHT));
	waveBuff2.AddInfo(NORMAL_ENEMY_HIGHSCORE, 480, Vec2(HALF_WIN_WIDTH + MAP_RADIUS, HALF_WIN_HEIGHT));

	waves.emplace_back(waveBuff2);

	//��O�E�F�[�u�̐���
	EnemyWave_Frame waveBuff3(1200);
	waveBuff3.Init();
	//���S�Ɍ������Đ^�����������G��90�x�Ԋu�ŉ�]�����Ȃ���o��
	waveBuff3.AddInfo(NORMAL_ENEMY_STRAIGHT, 60, Vec2(HALF_WIN_WIDTH, HALF_WIN_HEIGHT - MAP_RADIUS), 6, DX_PI_F / 2.0f);
	waveBuff3.AddInfo(NORMAL_ENEMY_STRAIGHT, 60, Vec2(HALF_WIN_WIDTH - MAP_RADIUS, HALF_WIN_HEIGHT), 6, 0.001f);
	waveBuff3.AddInfo(NORMAL_ENEMY_STRAIGHT, 60, Vec2(HALF_WIN_WIDTH, HALF_WIN_HEIGHT + MAP_RADIUS), 6, DX_PI_F + DX_PI_F / 2.0f);
	waveBuff3.AddInfo(NORMAL_ENEMY_STRAIGHT, 60, Vec2(HALF_WIN_WIDTH + MAP_RADIUS, HALF_WIN_HEIGHT), 6, DX_PI_F);
	waves.emplace_back(waveBuff3);

	//��l�E�F�[�u�̐���
	EnemyWave_Frame waveBuff4(1800);
	waveBuff4.Init();
	//�㉺�ɏ����ȕǂ̂悤�ȓG���o��
	for (int i = 0; i < 5; ++i) {
		Vec2 generatePos = Vec2((HALF_WIN_WIDTH - 2.0f * (90.0f * CAMERA_RATE)) + (90.0f * CAMERA_RATE) * i, HALF_WIN_HEIGHT - MAP_RADIUS);
		waveBuff4.AddInfo(NORMAL_ENEMY_DESTINATION, 0, generatePos, 0, 0, Vec2(generatePos.x, generatePos.y + MAP_RADIUS / 2.0f));
	}
	for (int i = 0; i < 5; ++i) {
		Vec2 generatePos = Vec2((HALF_WIN_WIDTH - 2.0f * (90.0f * CAMERA_RATE)) + (90.0f * CAMERA_RATE) * i, HALF_WIN_HEIGHT + MAP_RADIUS);
		waveBuff4.AddInfo(NORMAL_ENEMY_DESTINATION, 0, generatePos, 0, 0, Vec2(generatePos.x, generatePos.y - MAP_RADIUS / 2.0f));
	}
	//���E����͕ǂ𒆐S�Ɍ������Ĉړ������ďo��
	for (int i = 0; i < 3; ++i) {
		Vec2 generatePos = Vec2(HALF_WIN_WIDTH - (MAP_RADIUS * 0.5f), ((HALF_WIN_HEIGHT - 1.0f * (90.0f * CAMERA_RATE)) + (90.0f * CAMERA_RATE) * i));
		waveBuff4.AddInfo(NORMAL_ENEMY_STRAIGHT, 300, generatePos, 6);
	}
	for (int i = 0; i < 3; ++i) {
		Vec2 generatePos = Vec2(HALF_WIN_WIDTH + (MAP_RADIUS * 0.5f), ((HALF_WIN_HEIGHT - 1.0f * (90.0f * CAMERA_RATE)) + (90.0f * CAMERA_RATE) * i));
		waveBuff4.AddInfo(NORMAL_ENEMY_STRAIGHT, 300, generatePos, 6);
	}

	waves.emplace_back(waveBuff4);

	//��܃E�F�[�u�̐���
	EnemyWave_Frame waveBuff5(2400);
	waveBuff5.Init();
	//���E�ɉ~��`���悤�ɒǔ��G�𐶐�
	for (int i = 0; i < 3; ++i) {
		Vec2 centerPos = Vec2(HALF_WIN_WIDTH - MAP_RADIUS * 0.75f, HALF_WIN_HEIGHT);
		Vec2 generatePos = Vec2(cosf(DX_TWO_PI_F * ((float)i / 3.0f)) * 100.0f + centerPos.x,
			sinf(DX_TWO_PI_F * ((float)i / 3.0f)) * 100.0f + centerPos.y);
		waveBuff5.AddInfo(NORMAL_ENEMY_TRACKING, 400, generatePos);
	}
	for (int i = 0; i < 3; ++i) {
		Vec2 centerPos = Vec2(HALF_WIN_WIDTH + MAP_RADIUS * 0.75f, HALF_WIN_HEIGHT);
		Vec2 generatePos = Vec2(cosf(DX_TWO_PI_F * ((float)i / 3.0f)) * 100.0f + centerPos.x,
			sinf(DX_TWO_PI_F * ((float)i / 3.0f)) * 100.0f + centerPos.y);
		waveBuff5.AddInfo(NORMAL_ENEMY_TRACKING, 420, generatePos);
	}
	//�㉺�Ɏ֓G������
	waveBuff5.AddInfo(CIRCLE_ENEMY_TRACKING, 400, Vec2(HALF_WIN_WIDTH, HALF_WIN_HEIGHT - MAP_RADIUS * 0.75f));
	waveBuff5.AddInfo(CIRCLE_ENEMY_TRACKING, 400, Vec2(HALF_WIN_WIDTH, HALF_WIN_HEIGHT + MAP_RADIUS * 0.75f));

	waves.emplace_back(waveBuff5);

	//�ǔ��G�������̂ŁA1Wave���󂫂����

	//�掵�E�F�[�u�̐���
	EnemyWave_Frame waveBuff7(3600);
	waveBuff7.Init();
	//���̃E�F�[�u�͑�7�E�F�[�u�̏����i�K
	waveBuff7.AddInfo(NORMAL_ENEMY_HIGHSCORE, 120, Vec2(HALF_WIN_WIDTH + MAP_RADIUS, HALF_WIN_HEIGHT));
	waveBuff7.AddInfo(NORMAL_ENEMY_HIGHSCORE, 240, Vec2(HALF_WIN_WIDTH + MAP_RADIUS, HALF_WIN_HEIGHT));
	waveBuff7.AddInfo(NORMAL_ENEMY_HIGHSCORE, 360, Vec2(HALF_WIN_WIDTH + MAP_RADIUS, HALF_WIN_HEIGHT));
	waveBuff7.AddInfo(NORMAL_ENEMY_HIGHSCORE, 480, Vec2(HALF_WIN_WIDTH + MAP_RADIUS, HALF_WIN_HEIGHT));
	waveBuff7.AddInfo(NORMAL_ENEMY_HIGHSCORE, 600, Vec2(HALF_WIN_WIDTH + MAP_RADIUS, HALF_WIN_HEIGHT));

	waveBuff7.AddInfo(NORMAL_ENEMY_HIGHSCORE, 60, Vec2(HALF_WIN_WIDTH + MAP_RADIUS, HALF_WIN_HEIGHT));
	waveBuff7.AddInfo(NORMAL_ENEMY_HIGHSCORE, 180, Vec2(HALF_WIN_WIDTH + MAP_RADIUS, HALF_WIN_HEIGHT));
	waveBuff7.AddInfo(NORMAL_ENEMY_HIGHSCORE, 300, Vec2(HALF_WIN_WIDTH + MAP_RADIUS, HALF_WIN_HEIGHT));
	waveBuff7.AddInfo(NORMAL_ENEMY_HIGHSCORE, 420, Vec2(HALF_WIN_WIDTH + MAP_RADIUS, HALF_WIN_HEIGHT));
	waveBuff7.AddInfo(NORMAL_ENEMY_HIGHSCORE, 540, Vec2(HALF_WIN_WIDTH + MAP_RADIUS, HALF_WIN_HEIGHT));

	waves.emplace_back(waveBuff7);

	//�攪�E�F�[�u�̐���
	EnemyWave_Frame waveBuff8(4200);
	waveBuff8.Init();
	//�����^�ړ��G�𐶐�
	waveBuff8.AddInfo(NORMAL_ENEMT_INFINITY, 200, Vec2(HALF_WIN_WIDTH, WIN_HEIGHT));

	waves.emplace_back(waveBuff8);

	//���E�F�[�u���G����ʂɏo�Ă���̂�1wave���₷��

	//��\�E�F�[�u�̐���
	EnemyWave_Frame waveBuff10(5400);
	waveBuff10.Init();
	//���E���痈��ǂ𐶐�
	for (int i = 0; i < 5; ++i) {
		Vec2 centerPos = Vec2(HALF_WIN_WIDTH - (MAP_RADIUS * 0.75f), (((HALF_WIN_HEIGHT)-3.0f * (90.0f * CAMERA_RATE))) + 45.0f);
		Vec2 generatePos = Vec2(centerPos.x, centerPos.y + (90.0f * CAMERA_RATE) * i);
		waveBuff10.AddInfo(NORMAL_ENEMY_STRAIGHT, 30, generatePos, 6, 0.0001f);
	}
	for (int i = 0; i < 5; ++i) {
		Vec2 centerPos = Vec2(HALF_WIN_WIDTH + (MAP_RADIUS * 0.75f), (((HALF_WIN_HEIGHT)-3.0f * (90.0f * CAMERA_RATE))) + 45.0f);
		Vec2 generatePos = Vec2(centerPos.x, centerPos.y + (90.0f * CAMERA_RATE) * i);
		waveBuff10.AddInfo(NORMAL_ENEMY_STRAIGHT, 30, generatePos, 6, atan2f(0, -10.0f));
	}
	//�㉺���痈��G�����E����
	for (int i = 0; i < 5; ++i) {
		Vec2 generatePos = Vec2((HALF_WIN_WIDTH - 3.0f * (90.0f * CAMERA_RATE)) + (90.0f * CAMERA_RATE) * i + 45.0f, HALF_WIN_HEIGHT - MAP_RADIUS * 0.75f);
		waveBuff10.AddInfo(NORMAL_ENEMY_STRAIGHT, 30, generatePos, 6, atan2f(10.0f, 0));
	}
	for (int i = 0; i < 5; ++i) {
		Vec2 generatePos = Vec2((HALF_WIN_WIDTH - 3.0f * (90.0f * CAMERA_RATE)) + (90.0f * CAMERA_RATE) * i + 45.0f, HALF_WIN_HEIGHT + MAP_RADIUS * 0.75f);
		waveBuff10.AddInfo(NORMAL_ENEMY_STRAIGHT, 30, generatePos, 6, atan2f(-10, 0));
	}
	//�~��`���悤�ɓG�𐶐�
	for(int i = 0; i < 20; ++i){
		waveBuff10.AddInfo(NORMAL_ENEMY_STRAIGHT, 400, Vec2(HALF_WIN_WIDTH, HALF_WIN_HEIGHT), 4, DX_TWO_PI_F * ((float)i / 19.0f));
	}

	waves.emplace_back(waveBuff10);

	//��\��E�F�[�u�̐���
	EnemyWave_Frame waveBuff11(6000);
	waveBuff11.Init();
	//���ォ����Ԋu���ƂɃv���C���[�̕����ɂ܂������G�𔭎�
	Vec2 buff11CenterPos = Vec2(HALF_WIN_WIDTH - MAP_RADIUS / 2.0f, HALF_WIN_HEIGHT - MAP_RADIUS / 2.0f);
	waveBuff11.AddInfo(NORMAL_ENEMY_STRAIGHT, 0, buff11CenterPos, 4);
	waveBuff11.AddInfo(NORMAL_ENEMY_STRAIGHT, 180, buff11CenterPos, 4);
	waveBuff11.AddInfo(NORMAL_ENEMY_STRAIGHT, 420, buff11CenterPos, 4);
	waveBuff11.AddInfo(NORMAL_ENEMY_STRAIGHT, 520, buff11CenterPos, 4);
	//����������Ԋu���ƂɃv���C���[�̕����ɂ܂������G�𔭎�
	buff11CenterPos = Vec2(HALF_WIN_WIDTH - MAP_RADIUS / 2.0f, HALF_WIN_HEIGHT + MAP_RADIUS / 2.0f);;
	waveBuff11.AddInfo(NORMAL_ENEMY_STRAIGHT, 0, buff11CenterPos, 4);
	waveBuff11.AddInfo(NORMAL_ENEMY_STRAIGHT, 60, buff11CenterPos, 4);
	waveBuff11.AddInfo(NORMAL_ENEMY_STRAIGHT, 260, buff11CenterPos, 4);
	waveBuff11.AddInfo(NORMAL_ENEMY_STRAIGHT, 420, buff11CenterPos, 4);
	waveBuff11.AddInfo(NORMAL_ENEMY_STRAIGHT, 520, buff11CenterPos, 4);
	//�E��������Ԋu���ƂɃv���C���[�̕����ɂ܂������G�𔭎�
	buff11CenterPos = Vec2(HALF_WIN_WIDTH + MAP_RADIUS / 2.0f, HALF_WIN_HEIGHT + MAP_RADIUS / 2.0f);
	waveBuff11.AddInfo(NORMAL_ENEMY_STRAIGHT, 0, buff11CenterPos, 4);
	waveBuff11.AddInfo(NORMAL_ENEMY_STRAIGHT, 180, buff11CenterPos, 4);
	waveBuff11.AddInfo(NORMAL_ENEMY_STRAIGHT, 420, buff11CenterPos, 4);
	waveBuff11.AddInfo(NORMAL_ENEMY_STRAIGHT, 520, buff11CenterPos, 4);
	//�E�ォ����Ԋu���ƂɃv���C���[�̕����ɂ܂������G�𔭎�
	buff11CenterPos = Vec2(HALF_WIN_WIDTH + MAP_RADIUS / 2.0f, HALF_WIN_HEIGHT - MAP_RADIUS / 2.0f);;
	waveBuff11.AddInfo(NORMAL_ENEMY_STRAIGHT, 0, buff11CenterPos, 4);
	waveBuff11.AddInfo(NORMAL_ENEMY_STRAIGHT, 60, buff11CenterPos, 4);
	waveBuff11.AddInfo(NORMAL_ENEMY_STRAIGHT, 280, buff11CenterPos, 4);
	waveBuff11.AddInfo(NORMAL_ENEMY_STRAIGHT, 420, buff11CenterPos, 4);
	waveBuff11.AddInfo(NORMAL_ENEMY_STRAIGHT, 520, buff11CenterPos, 4);

	waves.emplace_back(waveBuff11);

	//��\��E�F�[�u�̐���		���X�g�E�F�[�u
	EnemyWave_Frame waveBuff12(6600);
	waveBuff12.Init();
	//�Ђ�����ォ�炭��ǂ𐶐�
	Vec2 buff12StartPos = Vec2(HALF_WIN_WIDTH - MAP_RADIUS, HALF_WIN_HEIGHT - MAP_RADIUS);
	for (int i = 0; i < 30; ++i) {
		waveBuff12.AddInfo(NORMAL_ENEMY_STRAIGHT, 0, Vec2(buff12StartPos.x + (90.0f * CAMERA_RATE) * i, buff12StartPos.y), 1, atan2f(10, 0));
	}
	for (int i = 0; i < 30; ++i) {
		waveBuff12.AddInfo(NORMAL_ENEMY_STRAIGHT, 200, Vec2(buff12StartPos.x + (90.0f * CAMERA_RATE) * i, buff12StartPos.y), 1, atan2f(10, 0));
	}
	for (int i = 0; i < 30; ++i) {
		waveBuff12.AddInfo(NORMAL_ENEMY_STRAIGHT, 400, Vec2(buff12StartPos.x + (90.0f * CAMERA_RATE) * i, buff12StartPos.y), 1, atan2f(10, 0));
	}
	for (int i = 0; i < 30; ++i) {
		waveBuff12.AddInfo(NORMAL_ENEMY_STRAIGHT, 600, Vec2(buff12StartPos.x + (90.0f * CAMERA_RATE) * i, buff12StartPos.y), 1, atan2f(10, 0));
	}
	for (int i = 0; i < 30; ++i) {
		waveBuff12.AddInfo(NORMAL_ENEMY_STRAIGHT, 800, Vec2(buff12StartPos.x + (90.0f * CAMERA_RATE) * i, buff12StartPos.y), 1, atan2f(10, 0));
	}
	for (int i = 0; i < 30; ++i) {
		waveBuff12.AddInfo(NORMAL_ENEMY_STRAIGHT, 1000, Vec2(buff12StartPos.x + (90.0f * CAMERA_RATE) * i, buff12StartPos.y), 1, atan2f(10, 0));
	}
	for (int i = 0; i < 30; ++i) {
		waveBuff12.AddInfo(NORMAL_ENEMY_STRAIGHT, 1200, Vec2(buff12StartPos.x + (90.0f * CAMERA_RATE) * i, buff12StartPos.y), 1, atan2f(10, 0));
	}
	waves.emplace_back(waveBuff12);
}

void EnemyAppearManager::Init()
{
	for (auto& wave : waves) {
		wave.Init();
	}
	frame = 0;
}

void EnemyAppearManager::Update()
{
	for (auto& wave : waves) {
		if (wave.isAlive) {	//���ɋN�����Ă�����
			wave.Update();	//�E�F�[�u�̍X�V
		}
		else
		{
			if (wave.OnFlag(frame)) {	//�N���t���O��On�Ȃ�
				wave.Generate();	//�N��
			}
		}
	}
	frame++;
}
