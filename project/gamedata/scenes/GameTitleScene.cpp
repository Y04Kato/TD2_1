#include"GameTitleScene.h"

void GameTitleScene::Initialize(){
	//CJEngine
	CJEngine_ = CitrusJunosEngine::GetInstance();
	
	//Input
	input_ = Input::GetInstance();
}

void GameTitleScene::Update(){
	if (input_->PressKey(DIK_N)) {
		sceneNo = 1;
	}
#ifdef _DEBUG
	ImGui::Begin("debug");
	ImGui::Text("GameTitleScene");
	ImGui::Text("nextScene:pressKey N");
	ImGui::End();
#endif
}

void GameTitleScene::Draw(){

}

void GameTitleScene::Finalize() {
	
}