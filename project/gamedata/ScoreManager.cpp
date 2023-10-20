#include "ScoreManager.h"

ScoreManager* ScoreManager::GetInstance()
{
	static ScoreManager instance;
	return &instance;
}

void ScoreManager::Initialize() {
	score_ = 0;
	frameScore_ = 0;
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
	frameScore_ = int(float(blockNum_) * ratio);
	score_ += frameScore_;
}