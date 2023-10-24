#include "Fade.h"

Fade* Fade::GetInstance() {
	static Fade instance;
	return &instance;
}

void Fade::Initialize() {
	isFade_ = false;
	isFadePlay_ = false;
	textureHandle_ = TextureManager::GetInstance()->Load("project/gamedata/resources/White2x2.png");
	sprite_.reset(new CreateSprite());
	Vector4 leftTop = { 0.0f,0.0f,0.0f,1.0f };
	Vector4 rightBottom = { float(WinApp::kClientWidth),float(WinApp::kClientHeight),0.0f,1.0f };
	material_ = { 1.0f,1.0f,1.0f,0.0f };
	alpha_ = 0.0f;
	material_.num[3] = alpha_;
	sprite_->Initialize(leftTop, rightBottom);
	phase_ = 1;
}

void Fade::Update() {
	if (isFade_) {
		alpha_ += (1.0f / (kFadeLength / 2.0f)) * float(phase_);
		if (phase_ == 1 && alpha_ >= 0.999f) {
			//isFade_ = false;
			isFadePlay_ = false;
		}
		alpha_ = std::clamp(alpha_, 0.0f, 1.0f);
		material_.num[3] = alpha_;
		if (phase_ == -1 && alpha_ == 0.0f) {
			isFade_ = false;
			isFadePlay_ = false;
		}
	}
}

void Fade::Draw() {
	if (isFade_) {
		Transform transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
		sprite_->Draw(transform, transform, material_, textureHandle_);
	}
}

void Fade::FadeIn() {
	alpha_ = 0.0f;
	material_.num[3] = alpha_;
	phase_ = 1;
	isFade_ = true;
	isFadePlay_ = true;
}

void Fade::FadeOut() {
	alpha_ = 1.0f;
	material_.num[3] = alpha_;
	phase_ = -1;
	isFade_ = true;
	isFadePlay_ = true;
}