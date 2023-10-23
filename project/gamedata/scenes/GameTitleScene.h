#pragma once
#include "components/manager/Iscene.h"
#include "CJEngine.h"
#include "components/input/Input.h"
#include "components/3d/ViewProjection.h"
#include "components/debugcamera/DebugCamera.h"
#include "gamedata/Unit.h"
class GameTitleScene :public Iscene{
public:
	void Initialize();
	void Update();
	void Draw();
	void Finalize();

private:
	CitrusJunosEngine* CJEngine_;

	Input* input_ = nullptr;
	ViewProjection viewProjection_;
	DebugCamera* debugCamera_;

	std::unique_ptr<Unit> unit_;
};
