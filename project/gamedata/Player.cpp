#include "Player.h"
//#include "Input.h"
#include <algorithm>
void Player::Initialize() {
	input_ = Input::GetInstance();
	model_.reset(Model::CreateModelFromObj("project/gamedata/resources/Cursor", "Cursor.obj"));
	worldTransform_.Initialize();
	//ライト
	directionalLight_ = { {1.0f,1.0f,1.0f,1.0f},{0.0f,-1.0f,0.0f},1.0f };

	//Explosion
	explosion_ = new Explosion();
	explosion_->Initialize();
	explosionTimer_ = 10;
	isExplosion_ = false;

	debugCamera_ = DebugCamera::GetInstance();
}

void Player::Update() {
	if (input_->TriggerKey(DIK_LEFT)) {
		mapPosition_.x--;
	}
	if (input_->TriggerKey(DIK_RIGHT)) {
		mapPosition_.x++;
	}
	if (input_->TriggerKey(DIK_UP)) {
		mapPosition_.y--;
	}
	if (input_->TriggerKey(DIK_DOWN)) {
		mapPosition_.y++;
	}
	mapPosition_.x = std::clamp(mapPosition_.x, 0, int(MapManager::GetInstance()->kMapWidth - 1));
	mapPosition_.y = std::clamp(mapPosition_.y, 0, int(MapManager::GetInstance()->kMapHeight - 1));

	Vector3 worldPos = MapManager::GetworldPosition(mapPosition_);
	worldTransform_.translation_.num[0] = worldPos.num[0];
	worldTransform_.translation_.num[2] = worldPos.num[2];
	worldTransform_.translation_.num[1] = 2.0f;
	worldTransform_.UpdateMatrix();

	explosion_->Update(worldTransform_);
	if (input_->TriggerKey(DIK_SPACE))
	{
		MapManager::GetInstance()->BreakBlock(mapPosition_);
		isExplosion_ = true;
		explosionTimer_ = 10;
	}

	if (isExplosion_ == true) {
		explosionTimer_--;
		debugCamera_->ShakeCamera(2);
	}

	if (explosionTimer_ <= 0 && isExplosion_ == true) {
		isExplosion_ = false;
		debugCamera_->SetCamera({ 14.0f,43.0f,-20.0f }, { -5.0f,0.0f,0.0f });
	}

#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_X)) {
		MapManager::GetInstance()->CreateBlock(mapPosition_);
	}
#endif // _DEBUG
}


void Player::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, Vector4{ 1.0f,1.0f,1.0f,1.0f }, directionalLight_);
	explosion_->Draw(viewProjection);
}