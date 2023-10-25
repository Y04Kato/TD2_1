#pragma once
#include "MapManager.h"
#include "VectorInt.h"
#include "components/3d/WorldTransform.h"
#include "components/3d/ViewProjection.h"
#include "components/3d/Model.h"
#include "components/audio/Audio.h"

#include <memory>

class Unit {
public:

	enum class Phase {
		Next,
		Move,
		Create,
	};

	void Initialize();
	//ロード時間短縮用再初期化
	void ShortInitialize();
	void Update();
	void Draw(const ViewProjection& viewProjection);

	void Next();
	void Move();
	void Create();

	void ApplyGlobalVariables();

private:
	std::unique_ptr<Model> model_;
	VectorInt2 mapPosition_;
	WorldTransform worldTransform_;
	WorldTransform targetWorldTransform_;
	WorldTransform nowWorldTransform_;

	VectorInt2 target_;

	Vector3 velocity_;
	int frameCount_ = 0;
	int moveEnd = 60;
	Phase phase_ = Phase::Next;
	static void (Unit::* phaseTable[])();

	DirectionalLight directionalLight_;

	//生きてるか
	bool isLive_;
	int respawnCoolTime = 0;
	int kRespawnTime = 300;

	//設置の向き
	MapManager::Direction direction_;

	Vector4 material;

	Audio* audio_;
	SoundData soundData1_;

	//生存時間
	int32_t liveTime_;
	//強化段階
	int32_t powerStep_;

	//強化フレーム
	int32_t kLevel2 = 0;
	int32_t kLevel3 = 0;

	int32_t kRespawnLev1;
	int32_t kRespawnLev2;
	int32_t kRespawnLev3;

	int32_t kMoveEndLev1;
	int32_t kMoveEndLev2;
	int32_t kMoveEndLev3;

	//動いてる最中に切り替わって挙動がおかしくなるのを防ぐ用
	int32_t nextMoveEnd;
};