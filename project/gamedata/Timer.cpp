#include "Timer.h"

void Timer::Initialize() {
	numberTextureHandle_ = TextureManager::GetInstance()->Load("project/gamedata/resources/verticalNumbers.png");
	timeTextureHandle_ = TextureManager::GetInstance()->Load("project/gamedata/resources/Time.png");
	timerSprite_.reset(new CreateSprite());
	Vector4 leftTopt = { float(kLeft + (-2 * (kSpace + kWidth))),float(kTop),0.0f,1.0f };
	Vector4 rightBottomt = { float(kLeft + kWidth*2 + (-2 * (kSpace + kWidth))),float(kTop + kHeight),0.0f,1.0f };
	timerSprite_->Initialize(leftTopt,rightBottomt);
	for (int index = 0; index < kDigits; index++) {
		Sprites_[index].reset(new CreateSprite());
		Vector4 leftTop = { float(kLeft + (index * (kSpace + kWidth))),float(kTop),0.0f,1.0f };
		Vector4 rightBottom = { float(kLeft + kWidth + (index * (kSpace + kWidth))),float(kTop + kHeight),0.0f,1.0f };
		Sprites_[index]->Initialize(leftTop, rightBottom);
	}
}

void Timer::Draw() {
	int32_t drawScore;
	Transform transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	Transform uv = { {1.0f,1.0f / 10.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	timerSprite_->Draw(transform, transform, {1.0f,1.0f,1.0f,1.0f},timeTextureHandle_);
	for (int index = 0; index < kDigits; index++) {
		drawScore = time_ / int(std::pow(10, kDigits - index - 1));
		drawScore %= 10;
		uv.translate.num[1] = 0.1f * float(drawScore);
		Sprites_[index]->Draw(transform, uv, { 1.0f,1.0f,1.0f,1.0f }, numberTextureHandle_);
	}
}
