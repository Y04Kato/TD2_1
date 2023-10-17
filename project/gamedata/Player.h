#pragma once
#include "MapManager.h"
#include "components/3d/WorldTransform.h"
#include "components/3d/ViewProjection.h"
//#include "components/manager/TextureManager.h"
#include "components/3d/Model.h"
//#include "Input.h"
#include "components/input/Input.h"
#include "VectorInt.h"

#include <memory>

class Player {
public:
	void Initialize();
	void Update();
	void Draw(const ViewProjection& viewProjection);

private:
	Input* input_ = nullptr;
	std::unique_ptr<Model> model_;
	VectorInt2 mapPosition_;
	WorldTransform worldTransform_;

	DirectionalLight directionalLight_;
};
