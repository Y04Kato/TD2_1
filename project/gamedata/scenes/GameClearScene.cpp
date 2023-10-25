#include"GameClearScene.h"
#include "../ScoreManager.h"
#include "gamedata/Fade.h"
#include "../MapManager.h"

void GameClearScene::Initialize() {
	//CJEngine
	CJEngine_ = CitrusJunosEngine::GetInstance();

	debugCamera_ = DebugCamera::GetInstance();
	debugCamera_->initialize();
	debugCamera_->SetCamera({ 18.0f,39.0f,-24.0f }, { -5.0f,0.0f,0.0f });
	viewProjection_.Initialize();
	viewProjection_.translation_ = debugCamera_->GetViewProjection()->translation_;
	viewProjection_.rotation_ = debugCamera_->GetViewProjection()->rotation_;
	viewProjection_.UpdateMatrix();

	//Score
	ScoreManager::GetInstance()->Initialize();

	//Input
	input_ = Input::GetInstance();

	Fade::GetInstance()->Initialize();
	isSceneChange_ = false;
	inResult_ = false;

	//ライト
	directionalLight_ = { {1.0f,1.0f,1.0f,1.0f},{0.0f,-1.0f,0.0f},1.0f };


	//BackGround
	backGround_.reset(Model::CreateModelFromObj("project/gamedata/resources/BG", "BG.obj"));
	worldTransformBackGround_.Initialize();
	worldTransformBackGround_.translation_ = { 17.3f,-25.8f,-4.6f };
	worldTransformBackGround_.scale_ = { 58.0f,1.0f,38.5f };


	RTTextureHandle_ = TextureManager::GetInstance()->Load("project/gamedata/resources/RT.png");
	spaceTextureHandle_ = TextureManager::GetInstance()->Load("project/gamedata/resources/space.png");
	pressTextureHandle_ = TextureManager::GetInstance()->Load("project/gamedata/resources/pressTitle.png");

	Vector4 leftTop = { float(-kRTWidth / 2),float(-kHeight / 2),0.0f,1.0f };
	Vector4 rightBottom = { float(kRTWidth / 2),float(kHeight / 2),0.0f,1.0f };
	RTSprite_.reset(new CreateSprite);
	RTSprite_->Initialize(leftTop, rightBottom);

	leftTop = { float(-kpressWidth / 2),float(-kHeight / 2),0.0f,1.0f };
	rightBottom = { float(kpressWidth / 2),float(kHeight / 2),0.0f,1.0f };
	pressSprite_.reset(new CreateSprite);
	pressSprite_->Initialize(leftTop, rightBottom);

	XINPUT_STATE joyState;
	isDrawController_ = Input::GetInstance()->GetJoystickState(0, joyState);

	inputAlpha_ = 1.0f;
}

void GameClearScene::Update() {
	XINPUT_STATE joyState;
	bool isConnect = Input::GetInstance()->GetJoystickState(0, joyState);
	if (!inResult_) {
		if (!Fade::GetInstance()->IsFade()) {
			Fade::GetInstance()->FadeOut();
			inResult_ = true;
		}
	}

	float t = (float(flick_) / float(flickLength) - 0.5f) * 2.0f;
	inputAlpha_ = (t * t);

	flick_++;
	if (flick_ > flickLength) {
		flick_ = 0;
	}

	if (inResult_ && !Fade::GetInstance()->IsFade()) {
		if (isSceneChange_ && !Fade::GetInstance()->IsFade()) {
			inResult_ = false;
			sceneNo = 0;
			isSceneChange_ = false;
		}
		if ((input_->PressKey(DIK_SPACE) || (isConnect && joyState.Gamepad.bRightTrigger != 0)) && !isSceneChange_) {
			if (!Fade::GetInstance()->IsFade() && Fade::GetInstance()->IsFadeEnd()) {
				Fade::GetInstance()->FadeIn();
				isSceneChange_ = true;
			}
		}
	}

	ScoreManager::GetInstance()->SetTransform(Transform{ {2.0f,2.0f,2.0f},{0.0f,0.0f,0.0f},{-1400.0f,218.0f,0.0f} });

	worldTransformBackGround_.UpdateMatrix();
	ScoreManager::GetInstance()->FrameStart();
	ScoreManager::GetInstance()->ScoreConfirm();
	Fade::GetInstance()->Update();
}

void GameClearScene::Draw() {
#pragma region 3Dオブジェクト描画
	CJEngine_->PreDraw3D();
	backGround_->Draw(worldTransformBackGround_, viewProjection_, Vector4{ 1.0f,1.0f,1.0f,1.0f }, directionalLight_);
	MapManager::GetInstance()->Draw(viewProjection_);
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


	ScoreManager::GetInstance()->Draw();
	Fade::GetInstance()->Draw();
#pragma endregion
}

void GameClearScene::Finalize() {

}