#pragma once
#include <stdint.h>
#include <array>
#include <list>
#include "VectorInt.h"
#include <memory>
#include "components/manager/TextureManager.h"
#include "components/2d/CreateSprite.h"
class Target {
public:
	void Initialize();
	void Update();
	void SetWorldTransform(const WorldTransform& worldTransform,const ViewProjection& viewProjection);
	void Draw();
private:
	//画面上の座標
	VectorInt2 position_;
	VectorInt2 offset_;
	VectorInt2 translate_;
	//画像幅
	const int32_t kWidth = 64;
	const int32_t kHeight = 64;

	std::unique_ptr<CreateSprite> sprite_;
	uint32_t textureHandle_ = 0;

	float floatanime = 0;
	float floatmax_=30.0f;
};