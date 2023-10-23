#include"GameTitleScene.h"
#include "../MapManager.h"
void GameTitleScene::Initialize(){
	//CJEngine
	CJEngine_ = CitrusJunosEngine::GetInstance();
	
	//Input
	input_ = Input::GetInstance();


	// デバッグカメラの初期化
	debugCamera_ = DebugCamera::GetInstance();
	debugCamera_->initialize();
	debugCamera_->SetCamera({ 18.0f,39.0f,-24.0f }, { -5.0f,0.0f,0.0f });
	viewProjection_.Initialize();
	viewProjection_.translation_ = debugCamera_->GetViewProjection()->translation_;
	viewProjection_.rotation_ = debugCamera_->GetViewProjection()->rotation_;
	viewProjection_.UpdateMatrix();

	//Map
	MapManager::GetInstance()->Initialize();
	//Unit
	unit_.reset(new Unit());
	unit_->Initialize();
}

void GameTitleScene::Update(){
	if (input_->PressKey(DIK_N)) {
		sceneNo = 1;
	}

	ImGui::Begin("debug");
	ImGui::Text("GameTitleScene");
	ImGui::Text("nextScene:pressKey N");
	ImGui::End();

	MapManager::GetInstance()->Update();
	unit_->Update();
}

void GameTitleScene::Draw(){
#pragma region 3Dオブジェクト描画
	CJEngine_->PreDraw3D();
	MapManager::GetInstance()->Draw(viewProjection_);
	//player_->Draw(viewProjection_);
	unit_->Draw(viewProjection_);
#pragma endregion
}

void GameTitleScene::Finalize() {
	
}