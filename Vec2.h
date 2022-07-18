#pragma once
#include <cmath>
#include <cstdio>
#include <string>
#include "DxLib.h"
#include "Constants.h"

extern const int WIN_WIDTH;
extern const int WIN_HEIGHT;

struct Vec2 {
	float x;		// 要素x
	float y;		// 要素y
	// コンストラクタ
	Vec2(float x, float y) : x(x), y(y) { }
	// デフォルトコンストラクタ
	Vec2() : x(x = 0), y(y = 0) { }
	// 要素の初期化
	void Init(float x = 0, float y = 0) {
		this->x = x;
		this->y = y;
	}
	// +演算子
	Vec2 operator + (const Vec2& rhs) {
		return Vec2(x + rhs.x, y + rhs.y);
	}
	// -演算子
	Vec2 operator - (const Vec2& rhs) {
		return Vec2(x - rhs.x, y - rhs.y);
	}
	// =演算子
	Vec2 operator = (const Vec2& rhs) {
		return Vec2(x = rhs.x, y = rhs.y);
	}
	// +=演算子
	void operator += (const Vec2& rhs) {
		x += rhs.x, y += rhs.y;
	}
	// -=演算子
	void operator -= (const Vec2& rhs) {
		x -= rhs.x, y -= rhs.y;
	}
	// ==演算子
	bool operator == (const Vec2& rhs) {
		return x == rhs.x && y == rhs.y;
	}
	// !=演算子
	bool operator != (const Vec2& rhs) {
		return !(x == rhs.x) && !(y == rhs.y);
	}
	// 二点間の距離を求める
	float Distance(const Vec2& rhs) {
		return sqrtf(powf(x - rhs.x, 2) + powf(y - rhs.y, 2));
	}
	// 引数を足す
	Vec2 Add(float X, float Y) {
		x += X;
		y += Y;
		return Vec2(x, y);
	}
	// 引数をかける
	Vec2 Times(float X, float Y) {
		x *= X;
		y *= Y;
		return Vec2(x, y);
	}
	// 内積
	float Dot(const Vec2& rhs) {
		return x * rhs.x + y * rhs.y;
	}
	// 外積
	float Cross(const Vec2& rhs) {
		return x * rhs.y - y * rhs.x;
	}
	// ベクトルの長さを求める
	float Length() {
		return sqrtf(Dot(*this));
	}
	// 正規化
	void Normalize() {
		float len = Length();
		x /= len;
		y /= len;
	}
};

class Input
{
private:
	static char keys[256];			//最新フレームのキーの押下情報
	static char oldKeys[256];		//一フレーム前のキーの押下情報
	static int mouseX;				//マウスのX座標
	static int oldMouseX;
	static int mouseY;				//マウスのY座標
	static int oldMouseY;
	static int mouseBottom;			//マウスボタンの押下情報
	static int oldMouseBottom;		//一フレーム前のマウスボタンの押下情報
	static XINPUT_STATE joypad;		//ジョイパッド情報
	static XINPUT_STATE oldjoypad;	//一フレーム前のジョイパッド情報
	static bool controller;

public:
	//更新処理
	static void Update()
	{
		//前フレームのキー情報を保存する。
		for (int i = 0; i < 256; i++) {
			oldKeys[i] = keys[i];
		}
		//最新のキー押下情報を取得。
		GetHitKeyStateAll(keys);
		//最新のマウス座標を取得。
		oldMouseX = mouseX;
		oldMouseY = mouseY;
		GetMousePoint(&mouseX, &mouseY);
		//前フレームのマウスボタンの情報を保存する。
		oldMouseBottom = mouseBottom;
		//最新のマウスボタン押下情報を取得。
		mouseBottom = GetMouseInput();
		//前フレームのジョイパッド情報を保存する。
		oldjoypad = joypad;
		//最新のジョイパッド情報を取得。
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

	//指定のキーの押下情報を取得。
	static bool isKey(int Key)
	{
		return keys[Key];
	}

	//指定のキーのトリガー押下情報を取得。
	static bool isKeyTrigger(int Key)
	{
		return keys[Key] && !oldKeys[Key];
	}

	//マウス座標を取得。
	static Vec2 GetMousePos()
	{
		Vec2 mousePos(mouseX, mouseY);
		return mousePos;
	}

	//マウスボタンの押下情報を取得。
	static bool isMouseBottom(int Mousebottom) {
		return mouseBottom & Mousebottom;
	}

	//マウスボタンのトリガー押下情報を取得。
	static bool isMouseBottomTrigger(int MouseBottom) {
		return !(oldMouseBottom & MouseBottom) && mouseBottom & MouseBottom;
	}

	//マウスボタンのリリース押下情報を取得。
	static bool isMouseBottomRelease(int MouseBottom) {
		return oldMouseBottom & MouseBottom && !(mouseBottom & MouseBottom);
	}

	//右スティックの入力状態を0~1の値で取得。
	static Vec2 GetJoyRightStick() {
		Vec2 stick;
		//デッドラインを設けてX軸方向の傾き具合を取得する。
		if (fabs(joypad.ThumbRX) <= 3000) {
			stick.x = 0;
		}
		else {
			stick.x = joypad.ThumbRX / 32767;
		}
		//デッドラインを設けてY軸方向の傾き具合を取得する。
		if (fabs(joypad.ThumbRY) <= 3000) {
			stick.y = 0;
		}
		else {
			stick.y = joypad.ThumbRY / 32767;
		}
		return stick;
	}

	//右スティックが入力されている角度を取得。
	static float GetJoyRightStickAngle() {
		Vec2 stick;
		//デッドラインを設けてX軸方向の傾き具合を取得する。
		if (fabs(joypad.ThumbRX) <= 1000) {
			stick.x = 0;
		}
		else {
			stick.x = joypad.ThumbRX / 32767.0f;
		}
		//デッドラインを設けてY軸方向の傾き具合を取得する。
		if (fabs(joypad.ThumbRY) <= 1000) {
			stick.y = 0;
		}
		else {
			stick.y = joypad.ThumbRY / 32767.0f;
		}
		return -atan2(stick.y, stick.x);
	}

	//左スティックの入力状態を0~1の値で取得。
	static Vec2 GetJoyLeftStick() {
		Vec2 stick;
		//デッドラインを設けてX軸方向の傾き具合を取得する。
		if (fabs(joypad.ThumbLX) <= 3000) {
			stick.x = 0;
		}
		else {
			stick.x = joypad.ThumbLX / 32767.0f;
		}
		//デッドラインを設けてY軸方向の傾き具合を取得する。
		if (fabs(joypad.ThumbLY) <= 3000) {
			stick.y = 0;
		}
		else {
			stick.y = joypad.ThumbLY / 32767.0f;
		}
		return stick;
	}

	//右トリガーの傾き具合を0~1の値で取得。
	static float GetJoyRightTrigger() {
		float trigger = 0;
		////デッドラインを設ける。
		//if (fabs(joypad.RightTrigger) <= 30) {
		//	trigger = 0;
		//}
		//else {
			trigger = (float)joypad.RightTrigger / 255.0f;
		//}
		return trigger;
	}

	//ジョイパッドボタンの押下情報を取得。
	static bool isJoyBottom(int JoyBottom) {
		return joypad.Buttons[JoyBottom];
	}

	//ジョイパッドボタンのトリガー押下情報を取得。
	static bool isJoyBottomTrigger(int JoyBottom) {
		return joypad.Buttons[JoyBottom] && !oldjoypad.Buttons[JoyBottom];
	}

	//ジョイパッドボタンのリリース押下情報を取得。
	static bool isJoyBottomRelease(int JoyBottom) {
		return !joypad.Buttons[JoyBottom] && oldjoypad.Buttons[JoyBottom];
	}

	static bool Controller() { return controller; }

	static bool MouseMove() { return mouseX != oldMouseX || mouseY != oldMouseY; }
};