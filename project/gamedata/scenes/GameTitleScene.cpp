#include"GameTitleScene.h"
#include "../MapManager.h"
#include "gamedata/Fade.h"

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
	Fade::GetInstance()->Initialize();
	isSceneChange_ = false;

	backGround_.reset(Model::CreateModelFromObj("project/gamedata/resources/BG", "BG.obj"));
	worldTransformBackGround_.Initialize();
	worldTransformBackGround_.translation_ = { 17.3f,-25.8f,-4.6f };
	worldTransformBackGround_.scale_ = { 58.0f,1.0f,38.5f };

	//ライト
	directionalLight_ = { {1.0f,1.0f,1.0f,1.0f},{0.0f,-1.0f,0.0f},1.0f };

}

void GameTitleScene::Update(){
	if (input_->PressKey(DIK_N) && !isSceneChange_) {
		//sceneNo = 1;
		if (!Fade::GetInstance()->IsFade()) {
			Fade::GetInstance()->FadeIn();
			isSceneChange_ = true;
		}
	}

	if (!isSceneChange_) {
		if (!Fade::GetInstance()->IsFade() && !
			Fade::GetInstance()->IsFadeEnd()) {
			Fade::GetInstance()->FadeOut();
		}
	}

	//遷移フラグが立ってフェードアニメーションが終わったら
	if (isSceneChange_ && !Fade::GetInstance()->IsFade()) {
		isSceneChange_ = false;
		sceneNo = 1;
	}
#ifdef _DEBUG
	ImGui::Begin("debug");
	ImGui::Text("GameTitleScene");
	ImGui::Text("nextScene:pressKey N");
	ImGui::End();
#endif

	MapManager::GetInstance()->Update();
	unit_->Update();
	Fade::GetInstance()->Update();
	worldTransformBackGround_.UpdateMatrix();
}

void GameTitleScene::Draw(){
#pragma region 3Dオブジェクト描画
	CJEngine_->PreDraw3D();
	backGround_->Draw(worldTransformBackGround_, viewProjection_, Vector4{ 1.0f,1.0f,1.0f,1.0f }, directionalLight_);
	MapManager::GetInstance()->Draw(viewProjection_);
	//player_->Draw(viewProjection_);
	unit_->Draw(viewProjection_);
#pragma endregion

#pragma region 前景スプライト描画
	CJEngine_->PreDraw2D();

	Fade::GetInstance()->Draw();
#pragma endregion
}

void GameTitleScene::Finalize() {
	
}