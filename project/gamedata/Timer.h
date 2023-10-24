#pragma once
#include <stdint.h>
#include <array>
#include <list>
#include "VectorInt.h"
#include <memory>
#include "components/manager/TextureManager.h"
#include "components/2d/CreateSprite.h"
class Timer {
public:
	void Initialize();
	//ロード時間短縮用
	//void ShortInitialize();
	//void Update();
	void Draw();

	void SetNowTime(int nowTime) { time_ = nowTime; };
	void SetInitialTime(int initialtime) { initialTime_ = initialtime; };
private:

	int32_t time_;
	//初期時間
	int32_t initialTime_;
	//スコア桁数
	static const int32_t kDigits = 3;


	//kaisiiti
	const int32_t kLeft = 140;
	const int32_t kTop = 30;

	//一桁ごとのサイズ
	const int32_t kWidth = 64;
	const int32_t kHeight = 64;
	//桁ごとの空間
	const int32_t kSpace = 0;

	std::unique_ptr<CreateSprite> timerSprite_;
	
	std::array<std::unique_ptr<CreateSprite>, kDigits> Sprites_;
	//std::list<std::function<Manager>> CommandList_;
	uint32_t numberTextureHandle_ = 0;
	uint32_t timeTextureHandle_ = 0;
};