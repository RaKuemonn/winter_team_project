#pragma once

#include "scene.h"
#include "sprite.h"

//ローディングシーン
class Scene_Loading : public Scene
{
public:
	Scene_Loading(Scene* nextScene) : next_scene(nextScene) {}

	void initialize(Scene_Manager* parent_) override;
	void uninitialize() override;
	void update(float elapsed_time) override;
	void render(float elapsed_time) override;

private:
	//ローディングスレッド
	static void loading_thread(Scene_Loading* scene);

private:
	float  angle = 0;

	Scene* next_scene = nullptr;
	std::unique_ptr<Sprite> icon = nullptr;
	std::unique_ptr<Sprite> loding = nullptr;
};
