#include "Target.h"

void Target::Initialize() {
	position_ = {0,0};
	textureHandle_ = TextureManager::GetInstance()->Load("project/gamedata/resources/Time.png");

	sprite_.reset(new CreateSprite());
	Vector4 leftTop = { float(-kWidth/2),float(-kHeight),0.0f,1.0f };
	Vector4 rightBottom = { float(kWidth/2),float(0),0.0f,1.0f };
	sprite_->Initialize(leftTop, rightBottom);
}

void Target::Update() {
	position_ = offset_;
	//position_.y -= 64;
}

void Target::SetWorldTransform(const WorldTransform& worldTransform, const ViewProjection& viewProjection) {
	Matrix4x4 matViewport =
		MakeViewportMatrix(0, 0, WinApp::kClientWidth, WinApp::kClientHeight, 0, 1.0f);
	worldTransform.translation_;
	Vector3 screen = TransformN(worldTransform.translation_, Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), matViewport));
	offset_.x = int(screen.num[0]);
	offset_.y = int(screen.num[1]);
}

void Target::Draw() {
	Transform transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{float(position_.x),float(position_.y),0.0f} };
	Transform uv = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	Vector4 color{ 1.0f,1.0f,1.0f,1.0f };
	sprite_->Draw(transform, uv, { 1.0f,1.0f,1.0f,1.0f }, textureHandle_);
}