#pragma once
#define DIRECTINPUT_VERSION		0x0800  //DirectInputのバージョン指定
#include <dinput.h>
#include <Xinput.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib,"xinput.lib")
#include "WinApp.h"
#include <array>

class Input{
public:
	static Input* GetInstance();

	void Initialize();

	void Update();

	bool TriggerKey(BYTE keyNumber) const;

	bool PressKey(BYTE keyNumber)const;

	bool ReleaseKey(BYTE keyNumber)const;

	Input(const Input& obj) = delete;
	Input& operator=(const Input& obj) = delete;

	static bool GetJoystickState(uint8_t stickNo, XINPUT_STATE& out);


private:
	Input() = default;
	~Input() = default;

	Microsoft::WRL::ComPtr <IDirectInput8> directInput_ = nullptr;
	Microsoft::WRL::ComPtr <IDirectInputDevice8> keyboard_ = nullptr;
	std::array<BYTE, 256> key_;
	std::array<BYTE, 256> preKey_;

	uint32_t deadzoneLeft = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
	uint32_t deadzoneRight = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;

};

