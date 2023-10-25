#include "GamePlayScene.h"
#include "components/utilities/globalVariables/GlobalVariables.h"
#include "../MapManager.h"
#include "../ScoreManager.h"
#include "gamedata/Fade.h"

void GamePlayScene::Initialize() {
	CJEngine_ = CitrusJunosEngine::GetInstance();
	dxCommon_ = DirectXCommon::GetInstance();
	textureManager_ = TextureManager::GetInstance();

#ifdef _DEBUG
	//三角形
	for (int i = 0; i < 2; i++) {
		triangle_[i] = new CreateTriangle();
		triangle_[i]->Initialize();
		worldTransformTriangle_[i].Initialize();
		triangleMaterial_[i] = { 1.0f,1.0f,1.0f,1.0f };
	}

	worldTransformTriangle_[1].rotation_.num[1] = 0.7f;

	isTriangleDraw1_ = false;
	isTriangleDraw2_ = false;

	//スプライト
	spriteData_.positionLeftTop[0] = { 0.0f,0.0f,0.0f,1.0f };
	spriteData_.positionRightDown[0] = { 640.0f,360.0f,0.0f,1.0f };
	spriteData_.positionLeftTop[1] = { 0.0f,0.0f,0.0f,1.0f };
	spriteData_.positionRightDown[1] = { 640.0f,360.0f,0.0f,1.0f };
	spriteData_.material = { 1.0f,1.0f,1.0f,1.0f };
	spriteTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	SpriteuvTransform_ = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};

	for (int i = 0; i < 2; i++) {
		sprite_[i] = new CreateSprite();
		sprite_[i]->Initialize(spriteData_.positionLeftTop[i], spriteData_.positionRightDown[i]);
	}

	isSpriteDraw_ = false;

	//球体
	sphere_ = new CreateSphere();
	sphere_->Initialize();
	worldTransformSphere_.Initialize();
	sphereMaterial_ = { 1.0f,1.0f,1.0f,1.0f };

	isSphereDraw_ = false;

	//objモデル
	model_ = new Model();
	model_->Initialize("project/gamedata/resources/block", "block.obj");
	worldTransformModel_.Initialize();
	modelMaterial_ = { 1.0f,1.0f,1.0f,1.0f };
#endif

	//BackGround
	backGround_.reset(Model::CreateModelFromObj("project/gamedata/resources/BG", "BG.obj"));
	worldTransformBackGround_.Initialize();
	worldTransformBackGround_.translation_ = { 17.3f,-25.8f,-4.6f };
	worldTransformBackGround_.scale_ = { 58.0f,1.0f,38.5f };

	//ライト
	directionalLight_ = { {1.0f,1.0f,1.0f,1.0f},{0.0f,-1.0f,0.0f},1.0f };

	//テクスチャ
	texture_ = 1;
	uvResourceNum_ = textureManager_->Load("project/gamedata/resources/uvChecker.png");

	monsterBallResourceNum_ = textureManager_->Load("project/gamedata/resources/monsterBall.png");

	//Input
	input_ = Input::GetInstance();

	//Audio
	audio_ = Audio::GetInstance();

	// デバッグカメラの初期化
	debugCamera_ = DebugCamera::GetInstance();
	debugCamera_->initialize();
	debugCamera_->SetCamera({ 18.0f,39.0f,-24.0f }, { -5.0f,0.0f,0.0f });

	viewProjection_.Initialize();

	//CollisionManager
	collisionManager_ = CollisionManager::GetInstance();

#ifdef _DEBUG
	GlobalVariables* globalVariables{};
	globalVariables = GlobalVariables::GetInstance();

	const char* groupName = "GamePlayScene";
	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "Test", 90);
	globalVariables->AddItem(groupName, "stick", stickTransform_.translate);
	globalVariables->AddItem(groupName, "move", moveTransform_.translate);
	globalVariables->AddItem(groupName, "RT", RTTransform_.translate);
	globalVariables->AddItem(groupName, "break", breakTransform_.translate);

	XINPUT_STATE joyState;
	isDrawController_ = Input::GetInstance()->GetJoystickState(0, joyState);
#endif
	//Map
	MapManager::GetInstance()->Initialize();

	//Player
	player_.reset(new Player());
	player_->Initialize();

	//Unit
	unit_.reset(new Unit());
	unit_->Initialize();

	//Score
	ScoreManager::GetInstance()->Initialize();
	inGame_ = false;

	//終了タイマー
	gameEndTimer_ = kPlayTime;

	isSceneChange_ = false;
	timer_.reset(new Timer);
	timer_->Initialize();
	timer_->SetNowTime(gameEndTimer_ / 60);
	timer_->SetInitialTime(kPlayTime / 60);

	target_.reset(new Target);
	target_->Initialize();

	stickTextureHandle_=TextureManager::GetInstance()->Load("project/gamedata/resources/botan.png");
	arrowTextureHandle_ = TextureManager::GetInstance()->Load("project/gamedata/resources/arrow.png");
	moveTextureHandle_ = TextureManager::GetInstance()->Load("project/gamedata/resources/idou.png");
	RTTextureHandle_ = TextureManager::GetInstance()->Load("project/gamedata/resources/RT.png");
	spaceTextureHandle_ = TextureManager::GetInstance()->Load("project/gamedata/resources/space.png");
	breakTextureHandle_ = TextureManager::GetInstance()->Load("project/gamedata/resources/break.png");

	Vector4 leftTop = { float(-kstickWidth / 2),float(-kHeight/2),0.0f,1.0f };
	Vector4 rightBottom = { float(kstickWidth / 2),float(kHeight/2),0.0f,1.0f };
	stickSprite_.reset(new CreateSprite);
	stickSprite_->Initialize(leftTop, rightBottom);
	
	leftTop = { float(-kmoveWidth / 2),float(-kHeight / 2),0.0f,1.0f };
	rightBottom = { float(kmoveWidth / 2),float(kHeight / 2),0.0f,1.0f };
	moveSprite_.reset(new CreateSprite);
	moveSprite_->Initialize(leftTop, rightBottom);

	leftTop = { float(-kRTWidth / 2),float(-kHeight / 2),0.0f,1.0f };
	rightBottom = { float(kRTWidth / 2),float(kHeight / 2),0.0f,1.0f };
	RTSprite_.reset(new CreateSprite);
	RTSprite_->Initialize(leftTop, rightBottom);

	leftTop = { float(-kbreakWidth / 2),float(-kHeight / 2),0.0f,1.0f };
	rightBottom = { float(kbreakWidth / 2),float(kHeight / 2),0.0f,1.0f };
	breakSprite_.reset(new CreateSprite);
	breakSprite_->Initialize(leftTop, rightBottom);
	

}

void GamePlayScene::Update() {

	ApplyGlobalVariables();

	collisionManager_->ClearColliders();
	collisionManager_->CheckAllCollision();

	debugCamera_->Update();

	viewProjection_.translation_ = debugCamera_->GetViewProjection()->translation_;
	viewProjection_.rotation_ = debugCamera_->GetViewProjection()->rotation_;
	viewProjection_.UpdateMatrix();

	directionalLight_.direction = Normalise(directionalLight_.direction);

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_A)) {
		OutputDebugStringA("Hit A\n");
		//Initialize();
		MapManager::GetInstance()->ShortInitialize();
		player_->ShortInitialize();
		unit_->ShortInitialize();
		ScoreManager::GetInstance()->Initialize();
	}

	for (int i = 0; i < 2; i++) {
		worldTransformTriangle_[i].UpdateMatrix();
	}
	worldTransformSphere_.UpdateMatrix();
	worldTransformModel_.UpdateMatrix();

	ImGui::Begin("debug");
	ImGui::Text("GamePlayScene");
	if (ImGui::TreeNode("Triangle")) {//三角形
		if (ImGui::Button("DrawTriangle1")) {
			if (isTriangleDraw1_ == false) {
				isTriangleDraw1_ = true;
			}
			else {
				isTriangleDraw1_ = false;
			}
		}
		if (ImGui::Button("DrawTriangle2")) {
			if (isTriangleDraw2_ == false) {
				isTriangleDraw2_ = true;
			}
			else {
				isTriangleDraw2_ = false;
			}
		}
		if (isTriangleDraw1_ == true) {
			if (ImGui::TreeNode("Triangle1")) {
				ImGui::DragFloat3("Translate", worldTransformTriangle_[0].translation_.num, 0.05f);
				ImGui::DragFloat3("Rotate", worldTransformTriangle_[0].rotation_.num, 0.05f);
				ImGui::DragFloat3("Scale", worldTransformTriangle_[0].scale_.num, 0.05f);
				ImGui::ColorEdit4("", triangleMaterial_[0].num, 0);
				ImGui::TreePop();
			}
		}
		if (isTriangleDraw2_ == true) {
			if (ImGui::TreeNode("Triangle2")) {
				ImGui::DragFloat3("Translate", worldTransformTriangle_[1].translation_.num, 0.05f);
				ImGui::DragFloat3("Rotate", worldTransformTriangle_[1].rotation_.num, 0.05f);
				ImGui::DragFloat3("Scale", worldTransformTriangle_[1].scale_.num, 0.05f);
				ImGui::ColorEdit4("", triangleMaterial_[1].num, 0);
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Sphere")) {//球体
		if (ImGui::Button("DrawSphere")) {
			if (isSphereDraw_ == false) {
				isSphereDraw_ = true;
			}
			else {
				isSphereDraw_ = false;
			}
		}
		ImGui::DragFloat3("Translate", worldTransformSphere_.translation_.num, 0.05f);
		ImGui::DragFloat3("Rotate", worldTransformSphere_.rotation_.num, 0.05f);
		ImGui::DragFloat3("Scale", worldTransformSphere_.scale_.num, 0.05f);
		ImGui::ColorEdit4("", sphereMaterial_.num, 0);
		ImGui::SliderInt("ChangeTexture", &texture_, 1, 2);
		ImGui::DragFloat4("LightColor", directionalLight_.color.num, 1.0f);
		ImGui::DragFloat3("lightDirection", directionalLight_.direction.num, 0.1f);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Sprite")) {//スプライト
		if (ImGui::Button("DrawSprite")) {
			if (isSpriteDraw_ == false) {
				isSpriteDraw_ = true;
			}
			else {
				isSpriteDraw_ = false;
			}
		}
		ImGui::DragFloat3("Translate", spriteTransform_.translate.num, 0.05f);
		ImGui::DragFloat3("Rotate", spriteTransform_.rotate.num, 0.05f);
		ImGui::DragFloat3("Scale", spriteTransform_.scale.num, 0.05f);
		ImGui::ColorEdit4("", spriteData_.material.num, 0);
		ImGui::DragFloat2("uvScale", SpriteuvTransform_.scale.num, 0.1f);
		ImGui::DragFloat3("uvTranslate", SpriteuvTransform_.translate.num, 0.1f);
		ImGui::DragFloat("uvRotate", &SpriteuvTransform_.rotate.num[2], 0.1f);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Model")) {//objモデル
		if (ImGui::Button("DrawModel")) {
			if (isModelDraw_ == false) {
				isModelDraw_ = true;
			}
			else {
				isModelDraw_ = false;
			}
		}
		ImGui::DragFloat3("Translate", worldTransformModel_.translation_.num, 0.05f);
		ImGui::DragFloat3("Rotate", worldTransformModel_.rotation_.num, 0.05f);
		ImGui::DragFloat3("Scale", worldTransformModel_.scale_.num, 0.05f);
		ImGui::ColorEdit4("", modelMaterial_.num, 0);
		ImGui::TreePop();
	}

	ImGui::Text("%f", ImGui::GetIO().Framerate);

	ImGui::End();
#endif
	ScoreManager::GetInstance()->SetTransform(Transform{ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{100.0f,0.0f,0.0f} });

	if (!inGame_) {
		//inGame_ = true;
		MapManager::GetInstance()->ShortInitialize();
		player_->ShortInitialize();
		unit_->ShortInitialize();
		ScoreManager::GetInstance()->Initialize();
		//仮
		gameEndTimer_ = kPlayTime;
		timer_->SetNowTime(gameEndTimer_ / 60);
		if (!Fade::GetInstance()->IsFade()) {
			Fade::GetInstance()->FadeOut();
			inGame_ = true;
		}
	}
	if (inGame_ && !Fade::GetInstance()->IsFade()) {
		ScoreManager::GetInstance()->FrameStart();
		MapManager::GetInstance()->Update();
		unit_->Update();
		player_->Update();
		ScoreManager::GetInstance()->ScoreConfirm();
		gameEndTimer_--;
		timer_->SetNowTime(gameEndTimer_ / 60);
		if (gameEndTimer_ <= 0) {
			if (isSceneChange_ && !Fade::GetInstance()->IsFade()) {
				inGame_ = false;
				sceneNo = 2;
				isSceneChange_ = false;
			}
			if (!Fade::GetInstance()->IsFade() && Fade::GetInstance()->IsFadeEnd()) {
				Fade::GetInstance()->FadeIn();
				isSceneChange_ = true;
			}
		}
	}
	unit_->SetCorePosition(viewProjection_);
	target_->SetWorldTransform(MapManager::GetWorldTransform(MapManager::GetInstance()->GetPriority()), viewProjection_);
	target_->Update();
	worldTransformBackGround_.UpdateMatrix();
	Fade::GetInstance()->Update();

#ifdef _DEBUG
	ImGui::Begin("Score");
	ImGui::Text("Score : %d",ScoreManager::GetInstance()->GetScore());
	ImGui::End();
#endif // _DEBUG
}

void GamePlayScene::Draw() {
#pragma region 3Dオブジェクト描画
	CJEngine_->PreDraw3D();

#ifdef _DEBUG
	if (isTriangleDraw1_) {//Triangle描画
		triangle_[0]->Draw(worldTransformTriangle_[0],viewProjection_,triangleMaterial_[0], uvResourceNum_, directionalLight_);
	}
	if (isTriangleDraw2_) {//Triangle描画
		triangle_[1]->Draw(worldTransformTriangle_[1], viewProjection_, triangleMaterial_[1], uvResourceNum_, directionalLight_);
	}

	if (isSphereDraw_) {
		sphere_->Draw(worldTransformSphere_,viewProjection_, sphereMaterial_, texture_, directionalLight_);
	}

	if (isModelDraw_) {
		model_->Draw(worldTransformModel_,viewProjection_,modelMaterial_, directionalLight_);
	}
#endif

	backGround_->Draw(worldTransformBackGround_,viewProjection_,Vector4{ 1.0f,1.0f,1.0f,1.0f }, directionalLight_);
	MapManager::GetInstance()->Draw(viewProjection_);
	player_->Draw(viewProjection_);
	unit_->Draw(viewProjection_);
#pragma endregion

#pragma region 前景スプライト描画
	CJEngine_->PreDraw2D();
	if (inGame_) {
		target_->Draw();
		unit_->DrawUI();
	}
	Transform uv = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	Vector4 color = {1.0f,1.0f,1.0f,1.0f};
	
	uint32_t moveth = arrowTextureHandle_;
	uint32_t breakth = spaceTextureHandle_;

	if (isDrawController_) {
		moveth = stickTextureHandle_;
		breakth = RTTextureHandle_;
	}

	stickSprite_->Draw(stickTransform_,uv,color,moveth);
	moveSprite_->Draw(moveTransform_, uv, color, moveTextureHandle_);
	RTSprite_->Draw(RTTransform_,uv,color,breakth);
	breakSprite_->Draw(breakTransform_,uv,color,breakTextureHandle_);

	ScoreManager::GetInstance()->Draw();
	timer_->Draw();
	Fade::GetInstance()->Draw();
#ifdef _DEBUG
	if (isSpriteDraw_) {
		for (int i = 0; i < 1; i++) {//Sprite描画
			sprite_[i]->Draw(spriteTransform_, SpriteuvTransform_, spriteData_.material, uvResourceNum_);
		}
	}
#endif
#pragma endregion
}

void GamePlayScene::Finalize() {

#ifdef _DEBUG
	for (int i = 0; i < 2; i++) {
		triangle_[i]->Finalize();
		delete triangle_[i];
	}

	for (int i = 0; i < 2; i++) {
		sprite_[i]->Finalize();
		delete sprite_[i];
	}

	sphere_->Finalize();
	delete sphere_;

	model_->Finalize();
	delete model_;

	audio_->SoundUnload(&soundData1_);
#endif
}

void GamePlayScene::ApplyGlobalVariables() {
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "GamePlayScene";
	stickTransform_.translate = globalVariables->GetVector3Value(groupName, "stick");
	moveTransform_.translate = globalVariables->GetVector3Value(groupName, "move");
	RTTransform_.translate = globalVariables->GetVector3Value(groupName, "RT");
	breakTransform_.translate = globalVariables->GetVector3Value(groupName, "break");
}