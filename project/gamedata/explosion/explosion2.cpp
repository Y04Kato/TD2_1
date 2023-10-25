#include "explosion2.h"

void Explosion2::Initialize() {
	input_ = Input::GetInstance();
	model_ = Model::CreateModelFromObj("project/gamedata/resources/explosion", "ExplosionBomb.obj");
	worldTransform_.Initialize();
	worldTransform_.scale_ = { 0.2f,0.2f,0.2f };
	modelMaterial_ = { 1.0f,1.0f,1.0f,1.0f };

	directionalLight_ = { {1.0f,1.0f,1.0f,1.0f},{0.0f,-1.0f,0.0f},1.0f };

	audio_ = Audio::GetInstance();

	soundData1_ = audio_->SoundLoadWave("project/gamedata/resources/explosion2.wav");
}

void Explosion2::Update() {
	if (explosionTimer <= 40) {
		if (isExplosion == true) {
			worldTransform_.scale_.num[0] += 0.1f;
			worldTransform_.scale_.num[1] += 0.1f;
			worldTransform_.scale_.num[2] += 0.1f;
			worldTransform_.rotation_.num[1] += 0.1f;
			//modelMaterial_.num[3] -= 0.02f;
		}
	}
	else {
		explosionTimer = 0;
		worldTransform_.scale_ = { 0.2f,0.2f,0.2f };
		modelMaterial_ = { 1.0f,1.0f,1.0f,1.0f };
		isExplosion = false;
	}

	if (isExplosion == true) {
		explosionTimer++;
	}

	worldTransform_.UpdateMatrix();
}

void Explosion2::Draw(const ViewProjection& viewProjection) {
	if (isExplosion == true) {
		model_->Draw(worldTransform_, viewProjection, modelMaterial_, directionalLight_);
	}
}

void Explosion2::ExplosionFlagTrue() {
	audio_->SoundPlayWave(soundData1_, 1.0f, false);
	isExplosion = true;
}

void Explosion2::SetExplosion(const WorldTransform& worldTransform) {
	worldTransform_.translation_ = worldTransform.translation_;
	worldTransform_.scale_ = { 0.2f,0.2f,0.2f };
	modelMaterial_ = { 1.0f,1.0f,1.0f,1.0f };
}