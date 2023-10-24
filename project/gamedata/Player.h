#pragma once
#include "MapManager.h"
#include "components/3d/WorldTransform.h"
#include "components/3d/ViewProjection.h"
#include "components/3d/Model.h"
#include "components/input/Input.h"
#include "VectorInt.h"

#include "gamedata/explosion/explosion.h"
#include <memory>
#include "components/debugcamera/DebugCamera.h"
#include "MapManager.h"

class Player {
public:
	enum class Phase {
		Idle,
		Move,
		Break,
	};

	void Initialize();
	//ロード時間短縮用
	void ShortInitialize();
	void Update();
	void Draw(const ViewProjection& viewProjection);

	void Idle();
	void Move();
	void Break();

private:
	Input* input_ = nullptr;
	std::unique_ptr<Model> model_;
	std::unique_ptr<Model> modelUp_;

	VectorInt2 mapPosition_;
	VectorInt2 moveTarget_;
	VectorInt2 nextPosition_;
	WorldTransform worldTransform_;
	WorldTransform worldTransformBreak_;

	DirectionalLight directionalLight_;

	DebugCamera* debugCamera_;

	Explosion* explosion_;
	float explosionTimer_;
	bool isExplosion_;

	int frameCount_ = 0;
	int moveEnd = 3;
	Phase phase_ = Phase::Idle;
	static void (Player::* phaseTable[])();

	WorldTransform targetWorldTransform_;
	WorldTransform nowWorldTransform_;

	//矢印
	WorldTransform worldTransformUp_;
	WorldTransform worldTransformCursor_;

	//先行入力用
	bool isMove_ = false;
	bool isBreak_ = false;
};
