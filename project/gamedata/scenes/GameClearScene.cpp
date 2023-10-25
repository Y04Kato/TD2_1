#include"GameClearScene.h"
#include "../ScoreManager.h"
#include "gamedata/Fade.h"

void GameClearScene::Initialize() {
	//CJEngine
	CJEngine_ = CitrusJunosEngine::GetInstance();

	//Score
	ScoreManager::GetInstance()->Initialize();

	//Input
	input_ = Input::GetInstance();

	Fade::GetInstance()->Initialize();
	isSceneChange_ = false;
	inResult_ = false;
}

void GameClearScene::Update() {
	if (!inResult_) {
		if (!Fade::GetInstance()->IsFade()) {
			Fade::GetInstance()->FadeOut();
			inResult_ = true;
		}
	}

	if (inResult_ && !Fade::GetInstance()->IsFade()) {
		if (isSceneChange_ && !Fade::GetInstance()->IsFade()) {
			inResult_ = false;
			sceneNo = 0;
			isSceneChange_ = false;
		}
		if (input_->PressKey(DIK_SPACE) && !isSceneChange_) {
			if (!Fade::GetInstance()->IsFade() && Fade::GetInstance()->IsFadeEnd()) {
				Fade::GetInstance()->FadeIn();
				isSceneChange_ = true;
			}
		}
	}

	ScoreManager::GetInstance()->SetTransform(Transform{ {2.0f,2.0f,2.0f},{0.0f,0.0f,0.0f},{-1400.0f,218.0f,0.0f} });

	ScoreManager::GetInstance()->FrameStart();
	ScoreManager::GetInstance()->ScoreConfirm();
	Fade::GetInstance()->Update();
}

void GameClearScene::Draw() {
#pragma region 3Dオブジェクト描画
	CJEngine_->PreDraw3D();


#pragma endregion

#pragma region 前景スプライト描画
	CJEngine_->PreDraw2D();

	ScoreManager::GetInstance()->Draw();
	Fade::GetInstance()->Draw();
#pragma endregion
}

void GameClearScene::Finalize() {

}