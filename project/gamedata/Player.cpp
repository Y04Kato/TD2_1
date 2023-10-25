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

	//矢印
	modelUp_.reset(Model::CreateModelFromObj("project/gamedata/resources/PlayerUp", "PlayerUp.obj"));
	worldTransformUp_.Initialize();
	worldTransformUp_.parent_ = &worldTransform_;

	worldTransformCursor_.Initialize();
	worldTransformCursor_.parent_ = &worldTransform_;
}

void Player::ShortInitialize() {
	explosionTimer_ = 10;
	isExplosion_ = false;
	mapPosition_ = MapManager::GetInstance()->GetCorePosition();
	mapPosition_.y--;
	frameCount_ = 0;
	isMove_ = false;
	isBreak_ = false;
	worldTransform_.translation_ = MapManager::GetInstance()->GetworldPosition(mapPosition_);
	worldTransform_.translation_.num[1] = 2.0f;
	worldTransform_.UpdateMatrix();
	phase_ = Phase::Idle;
}

void Player::Update() {
	XINPUT_STATE joyState;
	Input::GetInstance()->GetJoystickState(0, joyState);

	explosion_->Update(worldTransformBreak_);

	if (isExplosion_ == true) {
		explosionTimer_--;
		debugCamera_->ShakeCamera(2);
	}

	if (explosionTimer_ <= 0 && isExplosion_ == true) {
		isExplosion_ = false;
		debugCamera_->SetCamera({ 18.0f,39.0f,-24.0f }, { -5.0f,0.0f,0.0f });
	}

#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_X)) {
		MapManager::GetInstance()->CreateBlock(mapPosition_);
	}
#endif // _DEBUG
	//InPutJoyStick();
	(this->*phaseTable[static_cast<size_t>(phase_)])();
	worldTransform_.translation_.num[1] = 2.0f;
	worldTransform_.UpdateMatrix();
	worldTransformBreak_.UpdateMatrix();
	worldTransformUp_.UpdateMatrix();
	worldTransformCursor_.UpdateMatrix();
#ifdef _DEBUG
	ImGui::Begin("player");
	ImGui::SliderInt("move", &moveEnd, 1, 120);
	ImGui::SliderInt("break", &breakEnd, 1, 120);
	ImGui::End();
#endif
	preJoyState = joyState;
}

void Player::Idle() {
	InPutJoyStick();
	frameCount_ = 0;
	//moveTarget_ = {0,0};
	XINPUT_STATE joyState;
	Input::GetInstance()->GetJoystickState(0, joyState);
	if (input_->TriggerKey(DIK_LEFT) ||
		((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) &&
		!(preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT))) {
		moveTarget_.x = -1;
		moveTarget_.y = 0;
		isMove_ = true;
		isBreak_ = false;
	}
	else if (input_->TriggerKey(DIK_RIGHT) ||
		((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) &&
			!(preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT))) {
		moveTarget_.x = 1;
		moveTarget_.y = 0;
		isMove_ = true;
		isBreak_ = false;
	}
	else if (input_->TriggerKey(DIK_UP) || 
		((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) &&
		!(preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP))) {
		moveTarget_.y = -1;
		moveTarget_.x = 0;
		isMove_ = true;
		isBreak_ = false;
	}
	else if (input_->TriggerKey(DIK_DOWN) ||
		((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) &&
			!(preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN))) {
		moveTarget_.y = 1;
		moveTarget_.x = 0;
		isMove_ = true;
		isBreak_ = false;
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
	if (input_->TriggerKey(DIK_SPACE) || isBreak_) {
		worldTransformBreak_.translation_ = worldTransform_.translation_;
		phase_ = Phase::Break;
		isBreak_ = false;
	}
}

void Player::Move() {
	XINPUT_STATE joyState;
	Input::GetInstance()->GetJoystickState(0, joyState);
	if (input_->TriggerKey(DIK_LEFT) ||
		((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) &&
			!(preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT))) {
		moveTarget_.x = -1;
		moveTarget_.y = 0;
		isMove_ = true;
		isBreak_ = false;
	}
	else if (input_->TriggerKey(DIK_RIGHT) ||
		((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) &&
			!(preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT))) {
		moveTarget_.x = 1;
		moveTarget_.y = 0;
		isMove_ = true;
		isBreak_ = false;
	}
	else if (input_->TriggerKey(DIK_UP) ||
		((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) &&
			!(preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP))) {
		moveTarget_.y = -1;
		moveTarget_.x = 0;
		isMove_ = true;
		isBreak_ = false;
	}
	else if (input_->TriggerKey(DIK_DOWN) ||
		((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) &&
			!(preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN))) {
		moveTarget_.y = 1;
		moveTarget_.x = 0;
		isMove_ = true;
		isBreak_ = false;
	}
	if (input_->TriggerKey(DIK_SPACE)) {
		isBreak_ = true;
		isMove_ = false;
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
	//先行入力受け付け
	XINPUT_STATE joyState;
	Input::GetInstance()->GetJoystickState(0, joyState);
	if (input_->TriggerKey(DIK_LEFT) ||
		((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) &&
			!(preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT))) {
		moveTarget_.x = -1;
		moveTarget_.y = 0;
		isMove_ = true;
		isBreak_ = false;
	}
	else if (input_->TriggerKey(DIK_RIGHT) ||
		((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) &&
			!(preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT))) {
		moveTarget_.x = 1;
		moveTarget_.y = 0;
		isMove_ = true;
		isBreak_ = false;
	}
	else if (input_->TriggerKey(DIK_UP) ||
		((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) &&
			!(preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP))) {
		moveTarget_.y = -1;
		moveTarget_.x = 0;
		isMove_ = true;
		isBreak_ = false;
	}
	else if (input_->TriggerKey(DIK_DOWN) ||
		((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) &&
			!(preJoyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN))) {
		moveTarget_.y = 1;
		moveTarget_.x = 0;
		isMove_ = true;
		isBreak_ = false;
	}
	if (input_->TriggerKey(DIK_SPACE)) {
		isBreak_ = true;
		isMove_ = false;
	}
	if (frameCount_==0) {
		isBreakBlock_ = false;
		if (MapManager::GetInstance()->GetState(mapPosition_) == MapManager::MapState::Block) {
			isExplosion_ = true;
			explosion_->ExplosionFlagTrue();
			explosionTimer_ = 10;
			isBreakBlock_ = true;
		}
		MapManager::GetInstance()->BreakBlock(mapPosition_);	
	}
	if (frameCount_ == breakEnd) {
		phase_ = Phase::Idle;
	}
	float t = (float(frameCount_) / float(breakEnd) -0.5f) * 2.0f;
	if (isBreakBlock_) {
		worldTransformUp_.translation_.num[1] = -2.0f + 3.0f*(t * t);
		worldTransformUp_.scale_.num[0] = 0.5f + (t * t *0.5f);
		worldTransformUp_.scale_.num[2] = 0.5f + (t * t *0.5f);

		worldTransformCursor_.scale_.num[0] = 1.5f + -(t * t*0.5f);
		worldTransformCursor_.scale_.num[2] = 1.5f + -(t * t*0.5f);
	}
	else {
		worldTransformUp_.translation_.num[1] = 0.5f + (t * t*0.5f);
	}
	frameCount_++;
}


void Player::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransformCursor_, viewProjection, Vector4{ 1.0f,1.0f,1.0f,1.0f }, directionalLight_);
	modelUp_->Draw(worldTransformUp_, viewProjection, Vector4{ 1.0f,1.0f,1.0f,1.0f }, directionalLight_);
	explosion_->Draw(viewProjection);
}

void Player::InPutJoyStick() {
	XINPUT_STATE joyState;
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}
	Vector2 move = {
			float(joyState.Gamepad.sThumbLX) / SHRT_MAX,
			float(joyState.Gamepad.sThumbLY) / SHRT_MAX };
	/*
	Vector2 move = {
			float(joyState.Gamepad.sThumbLX) ,
			float(joyState.Gamepad.sThumbLY)  };*/
	if (std::abs( move.num[0]) < 0.8f) {
		move.num[0] = 0;
	}
	if (std::abs(move.num[1]) < 0.8f) {
		move.num[1] = 0;
	}
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B || 
		joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A || 
		joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER || 
		joyState.Gamepad.bRightTrigger != 0) {
		if (isButtonAble_) {
			isBreak_ = true;
			isMove_ = false;
		}
		isButtonAble_ = false;
	}
	else {
		isButtonAble_ = true;
	}
	if (move.num[0] == 0 && move.num[1] == 0) {
		joyStickAble_ = true;
		//moveTarget_.x = 0;
		//moveTarget_.y = 0;
		return;
	}
	if (joyStickAble_ || 1) {
		if ((move.num[1] * move.num[1]) <= (move.num[0] * move.num[0])) {
			move.num[1] = 0.0f;
		}
		else {
			move.num[0] = 0.0f;
		}

		move.num[0] *= 100.0f;
		move.num[1] *= 100.0f;

		move.num[0] = std::clamp(move.num[0], -1.1f, 1.1f);
		move.num[1] = std::clamp(move.num[1], -1.1f, 1.1f);
		moveTarget_.x = int(move.num[0]);
		moveTarget_.y = -int(move.num[1]);
		isMove_ = true;
		isBreak_ = false;

		joyStickAble_ = false;
	}
	
}