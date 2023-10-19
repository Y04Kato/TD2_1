#pragma once
#include "MapManager.h"
#include "VectorInt.h"
#include "components/3d/WorldTransform.h"
#include "components/3d/ViewProjection.h"
#include "components/3d/Model.h"

#include <memory>

class Unit {
public:

	enum class Phase {
		Next,
		Move,
		Create,
	};

	void Initialize();
	void Update();
	void Draw(const ViewProjection& viewProjection);

	void Next();
	void Move();
	void Create();

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
};