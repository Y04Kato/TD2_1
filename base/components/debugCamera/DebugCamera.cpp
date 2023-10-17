#include "DebugCamera.h"

DebugCamera* DebugCamera::GetInstance() {
	static DebugCamera instance;
	return &instance;
}

void DebugCamera::initialize() {
	viewProjection_.Initialize();
	viewProjection_.translation_ = { 0,0,-30 };
	viewProjection_.rotation_ = { 0,0,0 };
	input_ = Input::GetInstance();
}

void DebugCamera::Update() {

	ImGui::Begin("DebugCamera");
	ImGui::SliderFloat3("rotation", viewProjection_.rotation_.num, -100, 100);
	ImGui::SliderFloat3("translation", viewProjection_.translation_.num, -50, 50);
	ImGui::End();
	viewProjection_.UpdateMatrix();
}

void DebugCamera::ShakeCamera(int shakePower) {
	viewProjection_.translation_.num[0] += (rand() % shakePower - shakePower / 2+ rand() / (float)RAND_MAX)/5;
	viewProjection_.translation_.num[1] += (rand() % shakePower - shakePower / 2 + rand() / (float)RAND_MAX) / 5;
	viewProjection_.translation_.num[2] += (rand() % shakePower - shakePower / 2+ rand() / (float)RAND_MAX) / 5;
}

void DebugCamera::SetCamera(Vector3 translation, Vector3 rotation) {
	viewProjection_.translation_ = translation;
	viewProjection_.rotation_ = rotation;
}