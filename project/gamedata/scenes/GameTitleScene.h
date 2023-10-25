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

	void ApplyGlobalVariables();
private:
	CitrusJunosEngine* CJEngine_;

	Input* input_ = nullptr;
	ViewProjection viewProjection_;
	DebugCamera* debugCamera_;

	std::unique_ptr<Unit> unit_;

	std::unique_ptr<Model> backGround_;
	WorldTransform worldTransformBackGround_;

	DirectionalLight directionalLight_;

	bool isSceneChange_ = false;

	const int32_t kHeight = 128;
	std::unique_ptr<CreateSprite> RTSprite_;
	Vector3 RTOffset_;
	Transform RTTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	const int32_t kRTWidth = 128;

	std::unique_ptr<CreateSprite> pressSprite_;
	Vector3 pressOffset_;
	Transform pressTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	const int32_t kpressWidth = 640;

	uint32_t RTTextureHandle_;
	uint32_t spaceTextureHandle_;
	uint32_t pressTextureHandle_;

	bool isDrawController_;

	float inputAlpha_;

	int32_t flick_=0;
	int32_t flickLength=120;
	XINPUT_STATE preJoyState;
};
