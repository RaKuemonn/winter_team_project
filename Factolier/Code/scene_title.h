#pragma once

#include <memory>
#include "scene.h"
#include "sprite_batch.h"
#include "sound.h"
#include "sky_box.h"


class Scene_Title : public Scene
{
public:
	~Scene_Title() override {}

	void initialize() override;
	void uninitialize() override;
	void update(float elapsedTime) override;
	void render(float elapsedTime) override;

private:
	std::unique_ptr<Sprite_Batch> title_back = nullptr;
	std::unique_ptr<Sound> sound = nullptr;
	std::unique_ptr<Sky_Box> sky_box = nullptr;

};