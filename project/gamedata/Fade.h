#pragma once
#include "components/manager/TextureManager.h"
#include "components/2d/CreateSprite.h"
class Fade {
public:
	static Fade* GetInstance();
	void Initialize();
	void Update();
	void Draw();

	//フェード開始
	void FadeIn();
	//フェード終了
	void FadeOut();
	//フェードが終了しているか
	bool IsFadeEnd() { return !isFade_; };

	//フェード再生中か
	bool IsFade() { return isFadePlay_; };

private:
	Fade() = default;
	~Fade() = default;
	Fade(const Fade&) = delete;
	Fade& operator=(const Fade&) = delete;

	//フェード中か
	bool isFade_;
	bool isFadePlay_;
	uint32_t textureHandle_;
	std::unique_ptr<CreateSprite> sprite_;
	Vector4 material_;
	float alpha_;

	//イン:1,アウト:-1
	int phase_;

	//フェードの長さ(フレーム)
	const float kFadeLength = 60.0f;
};
