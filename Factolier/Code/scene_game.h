#pragma once

#include <memory>
#include "scene.h"
#include "player.h"



class Scene_Game : public Scene
{
public:
	~Scene_Game() override = default;

	void initialize() override;
	void uninitialize() override;
	void update(float elapsed_time) override;
	void render(float elapsed_time) override;

private:

};
