#pragma once

#include "scene.h"


//ローディングシーン
class Scene_Loading : public Scene
{
public:
	Scene_Loading(Scene* nextScene) : next_scene(nextScene) {}

	void initialize() override;
	void uninitialize() override;
	void update(float elapsedTime) override;
	void render(float elapsedTime) override;

private:
	//ローディングスレッド
	static void loading_thread(Scene_Loading* scene);

private:
	Scene* next_scene = nullptr;
};
