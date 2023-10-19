#include "Player.h"
#include <algorithm>

void (Player::* Player::phaseTable[])() = { &Player::Idle, &Player::Move, &Player::Break };

void Player::Initialize() {
	input_ = Input::GetInstance();
	model_.reset(Model::CreateModelFromObj("project/gamedata/resources/Cursor", "Cursor.obj"));
	worldTransform_.Initialize();
	worldTransformBreak_.Initialize();
	targetWorldTransform_.Initialize();
	nowWorldTransform_.Initialize();
	mapPosition_ = MapManager::GetInstance()->GetCorePosition();
	mapPosition_.y --;
	worldTransform_.translation_ = MapManager::GetInstance()->GetworldPosition(mapPosition_);
	worldTransform_.translation_.num[1] = 2.0f;
	worldTransform_.UpdateMatrix();
	phase_ = Phase::Idle;
	//ライト
	directionalLight_ = { {1.0f,1.0f,1.0f,1.0f},{-0.2f,-1.5f,0.4f},1.0f };

	//Explosion
	explosion_ = new Explosion();
	explosion_->Initialize();
	explosionTimer_ = 10;
	isExplosion_ = false;

	debugCamera_ = DebugCamera::GetInstance();
}

void Player::ShortInitialize() {
	explosionTimer_ = 10;
	isExplosion_ = false;
	mapPosition_ = MapManager::GetInstance()->GetCorePosition();
	mapPosition_.y--;
	frameCount_ = 0;
	isMove_ = false;
	worldTransform_.translation_ = MapManager::GetInstance()->GetworldPosition(mapPosition_);
	worldTransform_.translation_.num[1] = 2.0f;
	worldTransform_.UpdateMatrix();
	phase_ = Phase::Idle;
}

void Player::Update() {

	explosion_->Update(worldTransformBreak_);

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

	(this->*phaseTable[static_cast<size_t>(phase_)])();
	worldTransform_.translation_.num[1] = 2.0f;
	worldTransform_.UpdateMatrix();
	worldTransformBreak_.UpdateMatrix();
#ifdef _DEBUG
	ImGui::Begin("player");
	ImGui::SliderInt("move", &moveEnd, 1, 120);
	ImGui::End();
#endif
}

void Player::Idle() {
	//moveTarget_ = {0,0};
	if (input_->TriggerKey(DIK_LEFT)) {
		moveTarget_.x = -1;
		moveTarget_.y = 0;
		isMove_ = true;
	}
	else if (input_->TriggerKey(DIK_RIGHT)) {
		moveTarget_.x = 1;
		moveTarget_.y = 0;
		isMove_ = true;
	}
	else if (input_->TriggerKey(DIK_UP)) {
		moveTarget_.y = -1;
		moveTarget_.x = 0;
		isMove_ = true;
	}
	else if (input_->TriggerKey(DIK_DOWN)) {
		moveTarget_.y = 1;
		moveTarget_.x = 0;
		isMove_ = true;
	}
	if (isMove_)
	{
		isMove_ = false;
		frameCount_ = 0;
		phase_ = Phase::Move;
		moveTarget_.x = moveTarget_.x + mapPosition_.x;
		moveTarget_.y = moveTarget_.y + mapPosition_.y;
		moveTarget_.x =
			std::clamp(moveTarget_.x, 0, int(MapManager::GetInstance()->kMapWidth - 1));
		moveTarget_.y =
			std::clamp(moveTarget_.y, 0, int(MapManager::GetInstance()->kMapHeight - 1));

		targetWorldTransform_.translation_ = MapManager::GetInstance()->GetworldPosition(moveTarget_);
		nowWorldTransform_.translation_ = MapManager::GetInstance()->GetworldPosition(mapPosition_);
		nextPosition_ = moveTarget_;
		moveTarget_ = { 0,0 };
	}
	if (input_->TriggerKey(DIK_SPACE)) {
		worldTransformBreak_.translation_ = worldTransform_.translation_;
		phase_ = Phase::Break;
	}
}

void Player::Move() {
	if (input_->TriggerKey(DIK_LEFT)) {
		moveTarget_.x = -1;
		moveTarget_.y = 0;
		isMove_ = true;
	}
	else if (input_->TriggerKey(DIK_RIGHT)) {
		moveTarget_.x = 1;
		moveTarget_.y = 0;
		isMove_ = true;
	}
	else if (input_->TriggerKey(DIK_UP)) {
		moveTarget_.y = -1;
		moveTarget_.x = 0;
		isMove_ = true;
	}
	else if (input_->TriggerKey(DIK_DOWN)) {
		moveTarget_.y = 1;
		moveTarget_.x = 0;
		isMove_ = true;
	}

	float t = float(frameCount_) / float(moveEnd);
	worldTransform_.translation_.num[0] =
		(1.0f - t) * nowWorldTransform_.translation_.num[0] + t * targetWorldTransform_.translation_.num[0];
	worldTransform_.translation_.num[2] =
		(1.0f - t) * nowWorldTransform_.translation_.num[2] + t * targetWorldTransform_.translation_.num[2];
	if (frameCount_ >= moveEnd) {
		mapPosition_ = nextPosition_;

		phase_ = Phase::Idle;
	}
	frameCount_++;
}

void Player::Break() {
	if (MapManager::GetInstance()->GetState(mapPosition_) == MapManager::MapState::Block) {
		isExplosion_ = true;
		explosion_->ExplosionFlagTrue();
		explosionTimer_ = 10;
	}
	MapManager::GetInstance()->BreakBlock(mapPosition_);
	phase_ = Phase::Idle;
}


void Player::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, Vector4{ 1.0f,1.0f,1.0f,1.0f }, directionalLight_);
	explosion_->Draw(viewProjection);
}