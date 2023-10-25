#include"GameTitleScene.h"
#include "../MapManager.h"
#include "gamedata/Fade.h"
#include "components/utilities/globalVariables/GlobalVariables.h"

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
	//BackGround
	backGround_.reset(Model::CreateModelFromObj("project/gamedata/resources/BG", "BG.obj"));
	worldTransformBackGround_.Initialize();
	worldTransformBackGround_.translation_ = { 17.3f,-25.8f,-4.6f };
	worldTransformBackGround_.scale_ = { 58.0f,1.0f,38.5f };

	//ライト
	directionalLight_ = { {1.0f,1.0f,1.0f,1.0f},{0.0f,-1.0f,0.0f},1.0f };

	RTTextureHandle_ = TextureManager::GetInstance()->Load("project/gamedata/resources/RT.png");
	spaceTextureHandle_ = TextureManager::GetInstance()->Load("project/gamedata/resources/space.png");
	pressTextureHandle_ = TextureManager::GetInstance()->Load("project/gamedata/resources/pre.png");
	titleTextureHandle_ = TextureManager::GetInstance()->Load("project/gamedata/resources/title.png");
	Vector4 leftTop = { float(-kRTWidth / 2),float(-kHeight / 2),0.0f,1.0f };
	Vector4 rightBottom = { float(kRTWidth / 2),float(kHeight / 2),0.0f,1.0f };
	RTSprite_.reset(new CreateSprite);
	RTSprite_->Initialize(leftTop, rightBottom);

	leftTop = { float(-kpressWidth / 2),float(-kHeight / 2),0.0f,1.0f };
	rightBottom = { float(kpressWidth / 2),float(kHeight / 2),0.0f,1.0f };
	pressSprite_.reset(new CreateSprite);
	pressSprite_->Initialize(leftTop, rightBottom);

	leftTop = { float(-ktitleWidth / 2),float(-kHeight / 2),0.0f,1.0f };
	rightBottom = { float(ktitleWidth / 2),float(kHeight / 2),0.0f,1.0f };
	titleSprite_.reset(new CreateSprite);
	titleSprite_->Initialize(leftTop, rightBottom);

	XINPUT_STATE joyState;
	isDrawController_ = Input::GetInstance()->GetJoystickState(0, joyState);

	inputAlpha_ = 1.0f;
	GlobalVariables* globalVariables{};
	globalVariables = GlobalVariables::GetInstance();

	const char* groupName = "GameTitleScene";
	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "RT", RTTransform_.translate);
	globalVariables->AddItem(groupName, "press", pressTransform_.translate);
	globalVariables->AddItem(groupName, "title", titleTransform_.translate);

}

void GameTitleScene::Update(){
	ApplyGlobalVariables();
	XINPUT_STATE joyState;
	bool isConnect = Input::GetInstance()->GetJoystickState(0, joyState);
	if ((input_->PressKey(DIK_SPACE)|| (isConnect && joyState.Gamepad.bRightTrigger != 0)) && !isSceneChange_) {
		//sceneNo = 1;
		if (!Fade::GetInstance()->IsFade()) {
			Fade::GetInstance()->FadeIn();
			isSceneChange_ = true;
		}
	}

	float t = (float(flick_) / float(flickLength) - 0.5f)*2.0f;
	inputAlpha_ = (t*t);

	flick_++;
	if (flick_>flickLength) {
		flick_ = 0;
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
	ImGui::Text("nextScene:pressKey SPACE");
	ImGui::End();
#endif

	MapManager::GetInstance()->Update();
	unit_->Update();
	Fade::GetInstance()->Update();
	worldTransformBackGround_.UpdateMatrix();
	preJoyState = joyState;
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
	uint32_t breakth = spaceTextureHandle_;

	Transform uv = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	Vector4 color = { 1.0f,1.0f,1.0f,1.0f };
	color.num[3] = inputAlpha_;
	if (isDrawController_) {
		//moveth = stickTextureHandle_;
		breakth = RTTextureHandle_;
	}
	RTSprite_->Draw(RTTransform_, uv, color, breakth);
	pressSprite_->Draw(pressTransform_, uv, color, pressTextureHandle_);
	titleSprite_->Draw(titleTransform_, uv, {1.0f,1.0f,1.0f,1.0f}, titleTextureHandle_);
	Fade::GetInstance()->Draw();
#pragma endregion
}

void GameTitleScene::Finalize() {
	
}

void GameTitleScene::ApplyGlobalVariables() {
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "GameTitleScene";
	RTTransform_.translate = globalVariables->GetVector3Value(groupName, "RT");
	pressTransform_.translate = globalVariables->GetVector3Value(groupName, "press");
	titleTransform_.translate = globalVariables->GetVector3Value(groupName, "title");

}
