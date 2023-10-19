#pragma once
#include "components/3d/ViewProjection.h"
#include "components/3d/WorldTransform.h"
#include <stdint.h>
#include <array>
#include <list>
#include "VectorInt.h"
#include <memory>
#include "components/manager/TextureManager.h"
#include "components/3d/Model.h"

#include "gamedata/explosion/explosion2.h"

class MapManager {
public: 
	enum class MapState {
		None,
		Block,
		Core,
		Bomb,
		MaskBlock,
		ChaindBomb,
		UnChaindBomb,
	};

	struct Map {
		MapState mapstate;
		WorldTransform worldTransform;
	};

	struct Bomb {
		VectorInt2 position;
		int chaind;
	};

	static const uint32_t kMapWidth = 15;
	static const uint32_t kMapHeight = 10;

	const float kBlockWidth = 1.0f;
	const float kBlockHeight = 1.0f;

	static MapManager* GetInstance();
	void Initialize();
	void Update();
	void MapRead();
	void FindChain();
	void Protect(int x, int y);

	static Vector3 GetworldPosition(VectorInt2 vector);
	static Vector3 GetCenterworldPosition();

	void BreakBlock(const VectorInt2& position);
	void CreateBlock(const VectorInt2& position);

	MapState GetState(const VectorInt2& positiom) { return map[positiom.y][positiom.x].mapstate; };

	VectorInt2 GetCorePosition();

	VectorInt2 GetPriority();

	void Draw(const ViewProjection& );

	static const int kBombMax = 15;

private:
	MapManager() = default;
	~MapManager() = default;
	MapManager(const MapManager&) = delete;
	MapManager& operator=(const MapManager&) = delete;

	std::unique_ptr<Model> modelBlock;
	std::unique_ptr<Model> modelCore;
	std::unique_ptr<Model> modelBomb;
	std::unique_ptr<Model> modelUnChaindBomb;

	uint32_t blockTextureHandle_=0;
	uint32_t coreTextureHandle_=0;
	uint32_t BombTextureHandle_ = 0;
	uint32_t unChaindTextureHandle_ = 0;


	Map map[kMapHeight][kMapWidth];
	
	std::list<Bomb> bombs_;

	DirectionalLight directionalLight_;

	Explosion2* explosion_[kBombMax];
	int BombCount = 0;
};
