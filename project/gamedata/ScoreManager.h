#pragma once
#include <stdint.h>
#include <array>
#include <list>
#include "VectorInt.h"
#include <memory>
#include "components/manager/TextureManager.h"
#include "components/2d/CreateSprite.h"
//#include "components/3d/Model.h"

class ScoreManager {
public:

	static ScoreManager* GetInstance();
	void Initialize();
	//ロード時間短縮用
	//void ShortInitialize();

	void Draw();


	//ループの頭で呼ぶ
	void FrameStart();

	//ブロックを破壊したときによぶ
	void BreakBlock();

	void ExplodeBomb();

	//追加スコアを確定する
	void ScoreConfirm();

	//ブロック破壊時の基本増加スコア
	const int kBlockPoint = 10;

	//ボムを巻き込んだ時の増加倍率
	const float kBombRatio = 0.5f;

	int32_t GetScore() const { return score_; };
private:
	ScoreManager() = default;
	~ScoreManager() = default;
	ScoreManager(const ScoreManager&) = delete;
	ScoreManager& operator=(const ScoreManager&) = delete;

	int32_t score_;
	//フレームのスコア
	int32_t frameScore_;
	//フレームの壊したブロック数
	int32_t blockNum_;
	//フレームの爆発させた数
	int32_t bombNum_;

	//スコア桁数
	static const int32_t kScoreDigits = 5;


	//kaisiiti
	const int32_t kScoreLeft =500;
	const int32_t kScoreTop=100;

	//一桁ごとのサイズ
	const int32_t kScoreWidth = 64;
	const int32_t kScoreHeight = 64;
	//桁ごとの空間
	const int32_t kScoreSpace = 16;

	std::array<std::unique_ptr<CreateSprite>, kScoreDigits> scoreSprites_;
	//std::list<std::function<ScoreManager>> scoreCommandList_;
	uint32_t numberTextureHandle_ = 0;
};