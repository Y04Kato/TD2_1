#include "GamePlayScene.h"
#include "components/utilities/globalVariables/GlobalVariables.h"

void GamePlayScene::Initialize() {
	CJEngine_ = CitrusJunosEngine::GetInstance();
	dxCommon_ = DirectXCommon::GetInstance();
	textureManager_ = TextureManager::GetInstance();

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
	model_->Initialize("project/gamedata/resources/fence", "fence.obj");
	worldTransformModel_.Initialize();
	modelMaterial_ = { 1.0f,1.0f,1.0f,1.0f };

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
	soundData1_ = audio_->SoundLoadWave("project/gamedata/resources/fanfare.wav");
	//音声再生
	audio_->SoundPlayWave(soundData1_);

	// デバッグカメラの初期化
	debugCamera_ = DebugCamera::GetInstance();
	debugCamera_->initialize();

	viewProjection_.Initialize();

	//CollisionManager
	collisionManager_ = CollisionManager::GetInstance();

	GlobalVariables* globalVariables{};
	globalVariables = GlobalVariables::GetInstance();

	const char* groupName = "GamePlayScene";
	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "Test", 90);
}

void GamePlayScene::Update() {

	ApplyGlobalVariables();

	collisionManager_->ClearColliders();
	collisionManager_->CheckAllCollision();

	input_->Update();
	debugCamera_->Update();

	viewProjection_.translation_ = debugCamera_->GetViewProjection()->translation_;
	viewProjection_.rotation_ = debugCamera_->GetViewProjection()->rotation_;
	viewProjection_.UpdateMatrix();

	if (input_->PressKey(DIK_A)) {
		OutputDebugStringA("Hit A\n");
	}

	directionalLight_.direction = Normalise(directionalLight_.direction);

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
		ImGui::TreePop();
	}

	ImGui::Text("%f", ImGui::GetIO().Framerate);

	ImGui::End();
}

void GamePlayScene::Draw() {
#pragma region 3Dオブジェクト描画
	CJEngine_->PreDraw3D();

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
		for (int i = 0; i < 400; i++) {
			model_->Draw(worldTransformModel_,viewProjection_,modelMaterial_, directionalLight_);
		}
	}
#pragma endregion

#pragma region 前景スプライト描画
	CJEngine_->PreDraw2D();

	if (isSpriteDraw_) {
		for (int i = 0; i < 1; i++) {//Sprite描画
			sprite_[i]->Draw(spriteTransform_, SpriteuvTransform_, spriteData_.material, uvResourceNum_);
		}
	}
#pragma endregion
}

void GamePlayScene::Finalize() {
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
}

void GamePlayScene::ApplyGlobalVariables() {
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "GamePlayScene";
}