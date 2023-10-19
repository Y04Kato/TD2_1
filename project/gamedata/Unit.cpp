#include "Unit.h"
#include <algorithm>

void (Unit::* Unit::phaseTable[])() = { &Unit::Next,&Unit::Move, &Unit::Create };

void Unit::Initialize() {
	model_.reset(Model::CreateModelFromObj("project/gamedata/resources/block", "block.obj"));
	worldTransform_.Initialize();
	targetWorldTransform_.Initialize();
	nowWorldTransform_.Initialize();
	frameCount_ = 0;
	mapPosition_.x = 7;
	mapPosition_.y = 4;
	mapPosition_.x = std::clamp(mapPosition_.x, 0, int(MapManager::GetInstance()->kMapWidth - 1));
	mapPosition_.y = std::clamp(mapPosition_.y, 0, int(MapManager::GetInstance()->kMapHeight - 1));
	mapPosition_ = MapManager::GetInstance()->GetCorePosition();
	Vector3 worldPos = MapManager::GetworldPosition(mapPosition_);
	worldTransform_.translation_.num[0] = worldPos.num[0];
	worldTransform_.translation_.num[2] = worldPos.num[2];
	worldTransform_.translation_.num[1] = 2.0f;

	//ライト
	directionalLight_ = { {1.0f,1.0f,1.0f,1.0f},{0.0f,-1.0f,0.0f},1.0f };

	worldTransform_.UpdateMatrix();
}

void Unit::Update() {

	if (!isLive_)
	{
		if (respawnCoolTime <= 0)
		{
			isLive_ = true;
			mapPosition_ = MapManager::GetInstance()->GetCorePosition();
			phase_ = Phase::Next;
			Vector3 worldPos = MapManager::GetworldPosition(mapPosition_);
			worldTransform_.translation_.num[0] = worldPos.num[0];
			worldTransform_.translation_.num[2] = worldPos.num[2];
			worldTransform_.translation_.num[1] = 2.0f;
			worldTransform_.UpdateMatrix();
		}
		respawnCoolTime--;
	}
	else {
		(this->*phaseTable[static_cast<size_t>(phase_)])();

		if (MapManager::GetInstance()->GetState(mapPosition_) == MapManager::MapState::None ||
			MapManager::GetInstance()->GetState(mapPosition_) == MapManager::MapState::UnChaindBomb) {
			isLive_ = false;
			respawnCoolTime = kRespawnTime;
		}
	}
	worldTransform_.UpdateMatrix();

#ifdef _DEBUG
	ImGui::Begin("unit");
	ImGui::SliderInt("move", &moveEnd, 1, 120);
	ImGui::SliderInt("cooltime", &kRespawnTime, 0, 1800);
	ImGui::End();
#endif
}
void Unit::Next() {
	VectorInt2 targetBomb = MapManager::GetInstance()->GetPriority();
	targetBomb.x -= mapPosition_.x;
	targetBomb.y -= mapPosition_.y;
	if (std::abs(targetBomb.x) < std::abs(targetBomb.y)) {
		targetBomb.x = 0;
		direction_ = MapManager::Direction::Top;
		if (targetBomb.y > 0) {
			direction_ = MapManager::Direction::Down;
		}
	}
	else {
		targetBomb.y = 0;
		direction_ = MapManager::Direction::Left;
		if (targetBomb.x > 0) {
			direction_ = MapManager::Direction::Right;
		}
	}

	targetBomb.x = std::clamp(targetBomb.x, -1, 1);
	targetBomb.y = std::clamp(targetBomb.y, -1, 1);
	target_.x = targetBomb.x + mapPosition_.x;
	target_.y = targetBomb.y + mapPosition_.y;

	targetWorldTransform_.translation_ = MapManager::GetInstance()->GetworldPosition(target_);
	nowWorldTransform_.translation_ = MapManager::GetInstance()->GetworldPosition(mapPosition_);
	if (MapManager::GetInstance()->GetState(target_) ==
		MapManager::MapState::None)
	{
		phase_ = Phase::Create;
	}
	else {
		phase_ = Phase::Move;
		frameCount_ = 0;
	}
}

void Unit::Move() {
	float t = float(frameCount_) / float(moveEnd);
	worldTransform_.translation_.num[0] =
		(1.0f - t) * nowWorldTransform_.translation_.num[0] + t * targetWorldTransform_.translation_.num[0];
	worldTransform_.translation_.num[2] =
		(1.0f - t) * nowWorldTransform_.translation_.num[2] + t * targetWorldTransform_.translation_.num[2];
	if (frameCount_ >= moveEnd) {
		mapPosition_ = target_;
		phase_ = Phase::Next;
	}
	frameCount_++;
}

void Unit::Create()
{
	MapManager::GetInstance()->CreateBlock(target_, direction_);
	phase_ = Phase::Next;
}

void Unit::Draw(const ViewProjection& viewProjection) {
	if (isLive_) {
		model_->Draw(worldTransform_, viewProjection, Vector4{ 1.0f,0.0f,0.0f,1.0f }, directionalLight_);
	}

}