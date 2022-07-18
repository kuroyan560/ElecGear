#pragma once
#include <cmath>
#include <cstdio>
#include <string>
#include "DxLib.h"
#include "Constants.h"

extern const int WIN_WIDTH;
extern const int WIN_HEIGHT;

struct Vec2 {
	float x;		// �v�fx
	float y;		// �v�fy
	// �R���X�g���N�^
	Vec2(float x, float y) : x(x), y(y) { }
	// �f�t�H���g�R���X�g���N�^
	Vec2() : x(x = 0), y(y = 0) { }
	// �v�f�̏�����
	void Init(float x = 0, float y = 0) {
		this->x = x;
		this->y = y;
	}
	// +���Z�q
	Vec2 operator + (const Vec2& rhs) {
		return Vec2(x + rhs.x, y + rhs.y);
	}
	// -���Z�q
	Vec2 operator - (const Vec2& rhs) {
		return Vec2(x - rhs.x, y - rhs.y);
	}
	// =���Z�q
	Vec2 operator = (const Vec2& rhs) {
		return Vec2(x = rhs.x, y = rhs.y);
	}
	// +=���Z�q
	void operator += (const Vec2& rhs) {
		x += rhs.x, y += rhs.y;
	}
	// -=���Z�q
	void operator -= (const Vec2& rhs) {
		x -= rhs.x, y -= rhs.y;
	}
	// ==���Z�q
	bool operator == (const Vec2& rhs) {
		return x == rhs.x && y == rhs.y;
	}
	// !=���Z�q
	bool operator != (const Vec2& rhs) {
		return !(x == rhs.x) && !(y == rhs.y);
	}
	// ��_�Ԃ̋��������߂�
	float Distance(const Vec2& rhs) {
		return sqrtf(powf(x - rhs.x, 2) + powf(y - rhs.y, 2));
	}
	// �����𑫂�
	Vec2 Add(float X, float Y) {
		x += X;
		y += Y;
		return Vec2(x, y);
	}
	// ������������
	Vec2 Times(float X, float Y) {
		x *= X;
		y *= Y;
		return Vec2(x, y);
	}
	// ����
	float Dot(const Vec2& rhs) {
		return x * rhs.x + y * rhs.y;
	}
	// �O��
	float Cross(const Vec2& rhs) {
		return x * rhs.y - y * rhs.x;
	}
	// �x�N�g���̒��������߂�
	float Length() {
		return sqrtf(Dot(*this));
	}
	// ���K��
	void Normalize() {
		float len = Length();
		x /= len;
		y /= len;
	}
};

class Input
{
private:
	static char keys[256];			//�ŐV�t���[���̃L�[�̉������
	static char oldKeys[256];		//��t���[���O�̃L�[�̉������
	static int mouseX;				//�}�E�X��X���W
	static int oldMouseX;
	static int mouseY;				//�}�E�X��Y���W
	static int oldMouseY;
	static int mouseBottom;			//�}�E�X�{�^���̉������
	static int oldMouseBottom;		//��t���[���O�̃}�E�X�{�^���̉������
	static XINPUT_STATE joypad;		//�W���C�p�b�h���
	static XINPUT_STATE oldjoypad;	//��t���[���O�̃W���C�p�b�h���
	static bool controller;

public:
	//�X�V����
	static void Update()
	{
		//�O�t���[���̃L�[����ۑ�����B
		for (int i = 0; i < 256; i++) {
			oldKeys[i] = keys[i];
		}
		//�ŐV�̃L�[���������擾�B
		GetHitKeyStateAll(keys);
		//�ŐV�̃}�E�X���W���擾�B
		oldMouseX = mouseX;
		oldMouseY = mouseY;
		GetMousePoint(&mouseX, &mouseY);
		//�O�t���[���̃}�E�X�{�^���̏���ۑ�����B
		oldMouseBottom = mouseBottom;
		//�ŐV�̃}�E�X�{�^�����������擾�B
		mouseBottom = GetMouseInput();
		//�O�t���[���̃W���C�p�b�h����ۑ�����B
		oldjoypad = joypad;
		//�ŐV�̃W���C�p�b�h�����擾�B
		GetJoypadXInputState(DX_INPUT_PAD1, &joypad);

		if (controller) {
			if (mouseX - oldMouseX || mouseY - oldMouseY || mouseBottom) {
				controller = false;
			}
		}
		else
		{
			bool flag = false;
			for (int i = 0; i < 16; ++i) {
				if (joypad.Buttons[i]) {
					flag = true;
					break;
				}
			}
			if (!flag && (joypad.LeftTrigger || joypad.RightTrigger ||  joypad.ThumbLX != oldjoypad.ThumbLX
				|| joypad.ThumbLY != oldjoypad.ThumbLY || joypad.ThumbRX != oldjoypad.ThumbRX || joypad.ThumbRY != oldjoypad.ThumbRY))
			{
				flag = true;
			}

			if (flag)controller = true;
		}
	}

	//�w��̃L�[�̉��������擾�B
	static bool isKey(int Key)
	{
		return keys[Key];
	}

	//�w��̃L�[�̃g���K�[���������擾�B
	static bool isKeyTrigger(int Key)
	{
		return keys[Key] && !oldKeys[Key];
	}

	//�}�E�X���W���擾�B
	static Vec2 GetMousePos()
	{
		Vec2 mousePos(mouseX, mouseY);
		return mousePos;
	}

	//�}�E�X�{�^���̉��������擾�B
	static bool isMouseBottom(int Mousebottom) {
		return mouseBottom & Mousebottom;
	}

	//�}�E�X�{�^���̃g���K�[���������擾�B
	static bool isMouseBottomTrigger(int MouseBottom) {
		return !(oldMouseBottom & MouseBottom) && mouseBottom & MouseBottom;
	}

	//�}�E�X�{�^���̃����[�X���������擾�B
	static bool isMouseBottomRelease(int MouseBottom) {
		return oldMouseBottom & MouseBottom && !(mouseBottom & MouseBottom);
	}

	//�E�X�e�B�b�N�̓��͏�Ԃ�0~1�̒l�Ŏ擾�B
	static Vec2 GetJoyRightStick() {
		Vec2 stick;
		//�f�b�h���C����݂���X�������̌X������擾����B
		if (fabs(joypad.ThumbRX) <= 3000) {
			stick.x = 0;
		}
		else {
			stick.x = joypad.ThumbRX / 32767;
		}
		//�f�b�h���C����݂���Y�������̌X������擾����B
		if (fabs(joypad.ThumbRY) <= 3000) {
			stick.y = 0;
		}
		else {
			stick.y = joypad.ThumbRY / 32767;
		}
		return stick;
	}

	//�E�X�e�B�b�N�����͂���Ă���p�x���擾�B
	static float GetJoyRightStickAngle() {
		Vec2 stick;
		//�f�b�h���C����݂���X�������̌X������擾����B
		if (fabs(joypad.ThumbRX) <= 1000) {
			stick.x = 0;
		}
		else {
			stick.x = joypad.ThumbRX / 32767.0f;
		}
		//�f�b�h���C����݂���Y�������̌X������擾����B
		if (fabs(joypad.ThumbRY) <= 1000) {
			stick.y = 0;
		}
		else {
			stick.y = joypad.ThumbRY / 32767.0f;
		}
		return -atan2(stick.y, stick.x);
	}

	//���X�e�B�b�N�̓��͏�Ԃ�0~1�̒l�Ŏ擾�B
	static Vec2 GetJoyLeftStick() {
		Vec2 stick;
		//�f�b�h���C����݂���X�������̌X������擾����B
		if (fabs(joypad.ThumbLX) <= 3000) {
			stick.x = 0;
		}
		else {
			stick.x = joypad.ThumbLX / 32767.0f;
		}
		//�f�b�h���C����݂���Y�������̌X������擾����B
		if (fabs(joypad.ThumbLY) <= 3000) {
			stick.y = 0;
		}
		else {
			stick.y = joypad.ThumbLY / 32767.0f;
		}
		return stick;
	}

	//�E�g���K�[�̌X�����0~1�̒l�Ŏ擾�B
	static float GetJoyRightTrigger() {
		float trigger = 0;
		////�f�b�h���C����݂���B
		//if (fabs(joypad.RightTrigger) <= 30) {
		//	trigger = 0;
		//}
		//else {
			trigger = (float)joypad.RightTrigger / 255.0f;
		//}
		return trigger;
	}

	//�W���C�p�b�h�{�^���̉��������擾�B
	static bool isJoyBottom(int JoyBottom) {
		return joypad.Buttons[JoyBottom];
	}

	//�W���C�p�b�h�{�^���̃g���K�[���������擾�B
	static bool isJoyBottomTrigger(int JoyBottom) {
		return joypad.Buttons[JoyBottom] && !oldjoypad.Buttons[JoyBottom];
	}

	//�W���C�p�b�h�{�^���̃����[�X���������擾�B
	static bool isJoyBottomRelease(int JoyBottom) {
		return !joypad.Buttons[JoyBottom] && oldjoypad.Buttons[JoyBottom];
	}

	static bool Controller() { return controller; }

	static bool MouseMove() { return mouseX != oldMouseX || mouseY != oldMouseY; }
};