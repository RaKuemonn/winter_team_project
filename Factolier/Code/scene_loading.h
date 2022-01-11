#pragma once

#include "scene.h"


//���[�f�B���O�V�[��
class Scene_Loading : public Scene
{
public:
	Scene_Loading(Scene* nextScene) : next_scene(nextScene) {}

	void initialize(Scene_Manager* parent_) override;
	void uninitialize() override;
	void update(float elapsed_time) override;
	void render(float elapsed_time) override;

private:
	//���[�f�B���O�X���b�h
	static void loading_thread(Scene_Loading* scene);

private:
	Scene* next_scene = nullptr;
};
