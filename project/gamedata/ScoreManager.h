#pragma once
#include <stdint.h>
#include <array>
#include <list>
#include "VectorInt.h"
#include <memory>
#include "components/manager/TextureManager.h"
#include "components/3d/Model.h"

class ScoreManager {
public:

	static ScoreManager* GetInstance();
	void Initialize();
	//ロード時間短縮用
	//void ShortInitialize();

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

	int GetScore() { return score_; };
private:
	ScoreManager() = default;
	~ScoreManager() = default;
	ScoreManager(const ScoreManager&) = delete;
	ScoreManager& operator=(const ScoreManager&) = delete;

	int score_;
	//フレームのスコア
	int frameScore_;
	//フレームの壊したブロック数
	int blockNum_;
	//フレームの爆発させた数
	int bombNum_;

	//std::list<std::function<ScoreManager>> scoreCommandList_;
};