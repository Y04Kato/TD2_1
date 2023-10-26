#include "Unit.h"
#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>
#include "components/utilities/globalVariables/GlobalVariables.h"
void (Unit::* Unit::phaseTable[])() = { &Unit::Next,&Unit::Move, &Unit::Create };

void Unit::Initialize() {
	GlobalVariables* grovalVariables = GlobalVariables::GetInstance();
	const char* groupName = "UnitLevel1Parameta";
	grovalVariables->CreateGroup(groupName);
	grovalVariables->AddItem(groupName, "MoveEnd", kMoveEndLev1);
	grovalVariables->AddItem(groupName, "RespawnTime", kRespawnLev1);

	const char* groupName2 = "UnitLevel2Parameta";
	grovalVariables->CreateGroup(groupName2);
	grovalVariables->AddItem(groupName2, "LevelStart", kLevel2);
	grovalVariables->AddItem(groupName2, "MoveEnd", kMoveEndLev2);
	grovalVariables->AddItem(groupName2, "RespawnTime", kRespawnLev2);

	const char* groupName3 = "UnitLevel3Parameta";
	grovalVariables->CreateGroup(groupName3);
	grovalVariables->AddItem(groupName3, "LevelStart", kLevel3);
	grovalVariables->AddItem(groupName3, "MoveEnd", kMoveEndLev3);
	grovalVariables->AddItem(groupName3, "RespawnTime", kRespawnLev3);


	model_.reset(Model::CreateModelFromObj("project/gamedata/resources/Unit", "Unit.obj"));
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

	material = { 1.0f,0.0f,0.0f,1.0f };

	explosion_->Initialize();

	//ライト
	directionalLight_ = { {1.0f,1.0f,1.0f,1.0f},{-0.2f,-1.5f,0.4f},1.0f };

	worldTransform_.UpdateMatrix();

	audio_ = Audio::GetInstance();

	soundData1_ = audio_->SoundLoadWave("project/gamedata/resources/block.wav");

	liveTime_ = 0;
	powerStep_ = 1;

	spriteNeedle_.reset(new CreateSprite());
	spriteTimer_.reset(new CreateSprite());
	Vector4 leftTop = { float(-kWidth / 2),float(-kHeight/2),0.0f,1.0f };
	Vector4 rightBottom = { float(kWidth / 2),float(kHeight/2),0.0f,1.0f };
	spriteNeedle_->Initialize(leftTop, rightBottom);
	spriteTimer_->Initialize(leftTop, rightBottom);

	respawnTimerTextureHandle_[0] = TextureManager::GetInstance()->Load("project/gamedata/resources/revivaltime.png");
	respawnTimerTextureHandle_[1] = TextureManager::GetInstance()->Load("project/gamedata/resources/revivaltime1.png");
	respawnTimerTextureHandle_[2] = TextureManager::GetInstance()->Load("project/gamedata/resources/revivaltime2.png");
	respawnTimerTextureHandle_[3] = TextureManager::GetInstance()->Load("project/gamedata/resources/revivaltime3.png");
	respawnTimerTextureHandle_[4] = TextureManager::GetInstance()->Load("project/gamedata/resources/revivaltime.png");

	needleTextureHandle_ = TextureManager::GetInstance()->Load("project/gamedata/resources/revival.png");

}

void Unit::ShortInitialize() {
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
	worldTransform_.UpdateMatrix();
	isLive_ = true;
	respawnCoolTime = 0;
	phase_ = Phase::Next;
	material = {0.3f,0.9f,0.5f,1.0f};
	liveTime_ = 0;
	powerStep_ = 1;
}

void Unit::Update() {
	ApplyGlobalVariables();
	if (!isLive_)
	{
		if (respawnCoolTime <= 0)
		{
			liveTime_ = 0;
			isLive_ = true;
			mapPosition_ = MapManager::GetInstance()->GetCorePosition();
			phase_ = Phase::Next;
			Vector3 worldPos = MapManager::GetworldPosition(mapPosition_);
			worldTransform_.translation_.num[0] = worldPos.num[0];
			worldTransform_.translation_.num[2] = worldPos.num[2];
			worldTransform_.translation_.num[1] = 2.0f;
			material = { 0.3f,0.9f,0.5f,1.0f };
			worldTransform_.UpdateMatrix();
		}
		respawnCoolTime--;
		worldTransform_.translation_.num[1] -= 0.2f;
		material.num[3] -= 0.01f;
	}
	else {
		if (MapManager::GetInstance()->GetState(mapPosition_) == MapManager::MapState::None ||
			MapManager::GetInstance()->GetState(mapPosition_) == MapManager::MapState::UnChaindBomb) {
			isLive_ = false;
			respawnCoolTime = kRespawnTime;
		}
		if (isLive_) {
			powerStep_ = 1;
			if (liveTime_>= kLevel2) {
				powerStep_ = 2;
			}
			if (liveTime_ >= kLevel3) {
				powerStep_ = 3;
			}

			if (powerStep_ == 1) {
				nextMoveEnd = kMoveEndLev1;
				kRespawnTime = kRespawnLev1;
			}
			if (powerStep_ == 2) {
				nextMoveEnd = kMoveEndLev2;
				kRespawnTime = kRespawnLev2;
			}
			if (powerStep_ == 3) {
				nextMoveEnd = kMoveEndLev3;
				kRespawnTime = kRespawnLev3;
			}

			(this->*phaseTable[static_cast<size_t>(phase_)])();
			//向きを進行方向に変える
			if (direction_ == MapManager::Direction::Top) {
				worldTransform_.rotation_.num[1] = 0.0f;
			}
			else if (direction_ == MapManager::Direction::Down) {
				worldTransform_.rotation_.num[1] = float(M_PI);
			}
			else if (direction_ == MapManager::Direction::Left) {
				worldTransform_.rotation_.num[1] = float(M_PI) / 2.0f;
			}
			else if (direction_ == MapManager::Direction::Right) {
				worldTransform_.rotation_.num[1] = -float(M_PI) / 2.0f;
			}
			liveTime_++;
		}
	}

	explosion_->Update(worldTransform_);
	worldTransform_.UpdateMatrix();

#ifdef _DEBUG
	ImGui::Begin("unit");
	ImGui::SliderInt("move", &moveEnd, 1, 120);
	ImGui::SliderInt("cooltime", &kRespawnTime, 0, 1800);
	ImGui::End();
#endif
}
void Unit::Next() {
	moveEnd = nextMoveEnd;
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
	audio_->SoundPlayWave(soundData1_, 1.0f, false);
	explosion_->ExplosionFlagTrue();
	MapManager::GetInstance()->CreateBlock(target_, direction_);
	phase_ = Phase::Next;
}

void Unit::Draw(const ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection, material, directionalLight_);
	explosion_->Draw(viewProjection);
}

void Unit::ApplyGlobalVariables() {
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "UnitLevel1Parameta";
	kMoveEndLev1 = globalVariables->GetIntValue(groupName, "MoveEnd");
	kRespawnLev1 = globalVariables->GetIntValue(groupName, "RespawnTime");

	const char* groupName2 = "UnitLevel2Parameta";
	kLevel2 = globalVariables->GetIntValue(groupName2,"LevelStart");
	kMoveEndLev2 = globalVariables->GetIntValue(groupName2, "MoveEnd");
	kRespawnLev2 = globalVariables->GetIntValue(groupName2, "RespawnTime");

	const char* groupName3 = "UnitLevel3Parameta";
	kLevel3 = globalVariables->GetIntValue(groupName3, "LevelStart");
	kMoveEndLev3 = globalVariables->GetIntValue(groupName3, "MoveEnd");
	kRespawnLev3 = globalVariables->GetIntValue(groupName3, "RespawnTime");

}

void Unit::DrawUI() {
	if (!isLive_) {
		uint32_t textureHandle_;
		float persent = float(kRespawnTime - respawnCoolTime) / float(kRespawnTime);
		if (persent < 1.0f / 4.0f) {
			textureHandle_ = respawnTimerTextureHandle_[0];
		}
		else if (persent < 1.0f / 2.0f) {
			textureHandle_ = respawnTimerTextureHandle_[1];
		}
		else if (persent < 3.0f / 4.0f) {
			textureHandle_ = respawnTimerTextureHandle_[2];
		}
		else {
			textureHandle_ = respawnTimerTextureHandle_[3];
		}
		Transform transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{float(timerPosotion_.x),float(timerPosotion_.y),0.0f} };
		Transform transformNeelde = { {1.0f,1.0f,1.0f},{0.0f,0.0f,persent * 2 * float(M_PI)},{float(timerPosotion_.x),float(timerPosotion_.y),0.0f} };
		Transform uv = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
		Vector4 color{ 1.0f,1.0f,1.0f,1.0f };
		spriteTimer_->Draw(transform, uv, { 1.0f,1.0f,1.0f,1.0f }, textureHandle_);
		spriteNeedle_->Draw(transformNeelde, uv, { 1.0f,1.0f,1.0f,1.0f }, needleTextureHandle_);
	}
}

void Unit::SetCorePosition(const ViewProjection& viewProjection) {
	WorldTransform& worldTransform = MapManager::GetWorldTransform(MapManager::GetInstance()->GetCorePosition());
	Matrix4x4 matViewport =
		MakeViewportMatrix(0, 0, WinApp::kClientWidth, WinApp::kClientHeight, 0, 1.0f);
	worldTransform.translation_;
	Vector3 screen = TransformN(worldTransform.translation_, Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), matViewport));
	timerOffset_.x = int(screen.num[0]);
	timerOffset_.y = int(screen.num[1]);
	timerPosotion_ = timerOffset_;
	timerPosotion_.y -= 64;
}

