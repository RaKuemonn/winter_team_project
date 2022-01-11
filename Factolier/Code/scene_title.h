#pragma once

#include <memory>
#include "scene.h"
#include "sprite_batch.h"
#include "sound.h"
#include "sky_box.h"
#include "model.h"
#include "camera_controller.h" 




class Scene_Title : public Scene
{
public:
	~Scene_Title() override {}

	void initialize() override;
	void uninitialize() override;
	void update(float elapsed_time) override;
	void render(float elapsed_time) override;

private:
	std::unique_ptr<Sprite_Batch> title_back = nullptr;
	std::unique_ptr<Sound> sound = nullptr;
	std::unique_ptr<Sky_Box> sky_box = nullptr;
	std::unique_ptr<Model> test_model = nullptr;
	std::unique_ptr<Model> stage_model = nullptr;


	std::unique_ptr<Camera_Controller> camera_controller = nullptr;

};
