#pragma once
#include "components/manager/Iscene.h"
#include "components/audio/Audio.h"
#include "components/input/Input.h"
#include "components/3d/WorldTransform.h"
#include "components/3d/ViewProjection.h"
#include "components/manager/TextureManager.h"
#include "components/2d/CreateTriangle.h"
#include "components/2d/CreateSprite.h"
#include "components/3d/CreateSphere.h"
#include "components/3d/Model.h"
#include "components/debugcamera/DebugCamera.h"
#include "components/utilities/collisionManager/CollisionManager.h"
#include "components/utilities/collisionManager/CollisionConfig.h"

#include "gamedata/Player.h"
#include "gamedata/Unit.h"
#include "gamedata/Timer.h"
#include "gamedata/Target.h"

class GamePlayScene :public Iscene {
public:
	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	void ApplyGlobalVariables();

private:
	CitrusJunosEngine* CJEngine_;
	DirectXCommon* dxCommon_;
	ViewProjection viewProjection_;
	TextureManager* textureManager_;

	int blendCount_;

	CreateTriangle* triangle_[2];
	WorldTransform worldTransformTriangle_[2];
	Vector4 triangleMaterial_[2];

	CreateSprite* sprite_[2];
	SpriteData spriteData_;
	Transform spriteTransform_;
	Transform SpriteuvTransform_;

	CreateSphere* sphere_;
	WorldTransform worldTransformSphere_;
	Vector4 sphereMaterial_;

	Model* model_;
	WorldTransform worldTransformModel_;
	Vector4 modelMaterial_;

	DirectionalLight directionalLight_;

	uint32_t uvResourceNum_;
	uint32_t monsterBallResourceNum_;

	Audio* audio_;
	SoundData soundData1_;

	Input* input_;

	DebugCamera* debugCamera_;

	CollisionManager* collisionManager_;

	int texture_;

	bool isTriangleDraw1_;
	bool isTriangleDraw2_;
	int isSphereDraw_;
	int isSpriteDraw_;
	int isModelDraw_;

	std::unique_ptr<Player> player_;

	std::unique_ptr<Unit> unit_;

	std::unique_ptr<Model> backGround_;
	WorldTransform worldTransformBackGround_;

	//タイマー描画
	std::unique_ptr<Timer> timer_;

	//unitのターゲット
	std::unique_ptr<Target> target_;


	//プレイ中か
	bool inGame_ = false;

	//プレイ時間初期値(フレーム)
	int kPlayTime = 60 * 60;
	//終了タイマー
	int gameEndTimer_;

	//シーンチェンジフラグ
	bool isSceneChange_ = false;

	bool isDrawController_;

	const int32_t kHeight = 128;

	std::unique_ptr<CreateSprite> stickSprite_;
	Transform stickTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	const int32_t kstickWidth = 128;
	//std::unique_ptr<CreateSprite> arrowSprite_;
	//Transform arrowTransform_;
	//const int32_t karrowWidth = 128;
	std::unique_ptr<CreateSprite> moveSprite_;
	Transform moveTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	const int32_t kmoveWidth = 300;
	
	std::unique_ptr<CreateSprite> RTSprite_;
	Transform RTTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	const int32_t kRTWidth = 128;
	//std::unique_ptr<CreateSprite> spaceSprite_;
	//Transform spaceTransform_;
	//const int32_t kspaceWidth = 128;
	std::unique_ptr<CreateSprite> breakSprite_;
	Transform breakTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	const int32_t kbreakWidth = 300;

	uint32_t stickTextureHandle_;
	uint32_t arrowTextureHandle_;
	uint32_t moveTextureHandle_;
	uint32_t RTTextureHandle_;
	uint32_t spaceTextureHandle_;
	uint32_t breakTextureHandle_;

	bool isDrawtutorial_;

	const int32_t kSlidewidth = 600;
	const int32_t kSlideHeight = 800;

	static const int kSlideNum = 2;

	int32_t slideNum_;
	Transform sliderTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{620.310f,0.0f,0.0f} };
	std::unique_ptr<CreateSprite> slideSprite_;
	std::array<uint32_t,size_t(kSlideNum)> slideTextureHandle_;

	XINPUT_STATE preJoyState;
};
