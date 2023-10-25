#pragma once
#include "components/manager/Iscene.h"
#include "CJEngine.h"
#include "components/input/Input.h"
#include "components/3d/ViewProjection.h"
#include "components/debugcamera/DebugCamera.h"

class GameClearScene :public Iscene {
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

	bool isSceneChange_ = false;
	bool inResult_ = false;
};