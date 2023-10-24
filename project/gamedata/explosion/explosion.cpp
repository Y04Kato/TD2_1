#include "explosion.h"

void Explosion::Initialize() {
	input_ = Input::GetInstance();
	for (int i = 0; i < Max; i++) {
		model_[i] = Model::CreateModelFromObj("project/gamedata/resources/block", "block.obj");
		worldTransform_[i].Initialize();
		worldTransform_[i].scale_ = { 0.2f,0.2f,0.2f };
	}
	modelMaterial_ = { 1.0f,1.0f,1.0f,1.0f };

	directionalLight_ = { {1.0f,1.0f,1.0f,1.0f},{0.0f,-1.0f,0.0f},1.0f };

	audio_ = Audio::GetInstance();

	soundData1_ = audio_->SoundLoadWave("project/gamedata/resources/Explosion.wav");
}

void Explosion::Update(const WorldTransform& worldTransform) {
	for (int i = 0; i < Max; i++) {
		transform_[i].translate.num[1] += bounceSpeed_;
	}
	bounceSpeed_ += acceleration_;

	if (startFlag == true) {
		time_ += 0.05f;
		if (time_ >= 5.0f) {
			startFlag = false;
			drawFlag = false;
		}
	}

	if (testFlag == true) {
		for (int i = 0; i < Max; i++) {
			worldTransform_[i].translation_ = worldTransform.translation_;
			transform_[i].translate.num[0] = worldTransform.translation_.num[0] + rand() % 6 - 3 + rand() / (float)RAND_MAX;
			transform_[i].translate.num[1] = worldTransform.translation_.num[1];
			transform_[i].translate.num[2] = worldTransform.translation_.num[2] + rand() % 4 - 2 + rand() / (float)RAND_MAX;
		}
		testFlag = false;
	}
#ifdef _DEBUG
	ImGui::Begin("Explosion");
	ImGui::DragFloat("boundSpeed", &bounceSpeed_);
	ImGui::DragFloat("timer", &time_);
	ImGui::DragFloat3("movepoint", transform_->translate.num);
	ImGui::DragFloat3("translation", worldTransform_->translation_.num);
	ImGui::Text("%f", worldTransform.translation_.num[0]);
	ImGui::End();
#endif

	for (int i = 0; i < Max; i++) {
		worldTransform_[i].translation_.num[0] = (1.0f - time_) * worldTransform.translation_.num[0] + time_ * transform_[i].translate.num[0];
		worldTransform_[i].translation_.num[1] = transform_[i].translate.num[1];
		worldTransform_[i].translation_.num[2] = (1.0f - time_) * worldTransform.translation_.num[2] + time_ * transform_[i].translate.num[2];

		worldTransform_[i].UpdateMatrix();
	}
}

void Explosion::Draw(const ViewProjection& viewProjection) {
	if (drawFlag == true) {
		for (int i = 0; i < Max; i++) {
			model_[i]->Draw(worldTransform_[i], viewProjection, modelMaterial_, directionalLight_);
		}
	}
}

void Explosion::ExplosionFlagTrue(){
	audio_->SoundPlayWave(soundData1_);
	bounceSpeed_ = 1.2f;
	time_ = 0.0f;
	startFlag = true;
	testFlag = true;
	drawFlag = true;
}