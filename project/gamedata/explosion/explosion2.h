#pragma once
#include "components/input/Input.h"
#include "components/3d/WorldTransform.h"
#include "components/3d/ViewProjection.h"
#include "components/3d/Model.h"
#include "components/audio/Audio.h"

class Explosion2 {
public:
	void Initialize();
	void Update();
	void Draw(const ViewProjection& viewProjection);

	void ExplosionFlagTrue();

	void SetExplosion(const WorldTransform& worldTransform);

	WorldTransform GetworldTransform() { return worldTransform_; }

private:
	Input* input_;
	Model* model_;
	Transform transform_;
	WorldTransform worldTransform_;
	Vector4 modelMaterial_;
	DirectionalLight directionalLight_;

	bool isExplosion = false;

	int explosionTimer = 0;

	Audio* audio_;
	SoundData soundData1_;
};