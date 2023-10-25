#include "ScoreManager.h"

ScoreManager* ScoreManager::GetInstance()
{
	static ScoreManager instance;
	return &instance;
}

void ScoreManager::Initialize() {
	score_ = 0;
	frameScore_ = 0;
	numberTextureHandle_ = TextureManager::GetInstance()->Load("project/gamedata/resources/verticalNumbers.png");
	for (int index = 0; index < kScoreDigits; index++) {
		scoreSprites_[index].reset(new CreateSprite());

		Vector4 leftTop = { float(kScoreLeft + (index * (kScoreSpace + kScoreWidth))),float(kScoreTop),0.0f,1.0f };
		Vector4 rightBottom = { float(kScoreLeft + kScoreWidth + (index * (kScoreSpace + kScoreWidth))),float(kScoreTop + kScoreHeight),0.0f,1.0f };
		scoreSprites_[index]->Initialize(leftTop, rightBottom);
	}

	transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
}

void ScoreManager::Draw() {
#ifdef _DEBUG
	ImGui::Begin("Score");
	ImGui::DragFloat3("scale", transform_.scale.num, 1.0f);
	ImGui::DragFloat3("rotate", transform_.rotate.num, 1.0f);
	ImGui::DragFloat3("translate", transform_.translate.num, 1.0f);
	ImGui::End();
#endif // DEBUG

	int32_t drawScore;
	Transform uv = { {1.0f,1.0f / 10.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	for (int index = 0; index < kScoreDigits; index++) {
		drawScore = score_ / int(std::pow(10, kScoreDigits - index - 1));
		drawScore %= 10;
		uv.translate.num[1] = 0.1f * float(drawScore);
		scoreSprites_[index]->Draw(transform_, uv, { 1.0f,1.0f,1.0f,1.0f }, numberTextureHandle_);
	}
}

void ScoreManager::FrameStart() {
	frameScore_ = 0;
	blockNum_ = 0;
	bombNum_ = 0;
}

void ScoreManager::BreakBlock() {
	blockNum_++;
}

void ScoreManager::ExplodeBomb() {
	bombNum_++;
}

void ScoreManager::ScoreConfirm() {
	float ratio = 1.0f + float(bombNum_) * kBombRatio;
	frameScore_ = int(float(blockNum_)*10 * ratio);
	score_ += frameScore_;
	score_ = std::clamp(score_, 0, 99999);
}