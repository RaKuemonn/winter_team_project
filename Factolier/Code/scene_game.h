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
	void update(float elapsedTime) override;
	void render(float elapsedTime) override;

private:

};
