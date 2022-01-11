#pragma once

#include <memory>
#include "scene.h"
#include "enemy_spawner.h"
#include "collision_manager.h"


class Scene_Game : public Scene
{
public:
	~Scene_Game() override = default;

	void initialize() override;
	void uninitialize() override;
	void update(float elapsed_time) override;
	void render(float elapsed_time) override;

private:
	std::unique_ptr<Enemy_Spawner>		enemy_spawner = nullptr;
	std::unique_ptr<Collision_Manager>	collision_manager = nullptr;

	std::unique_ptr<class Camera_Controller> camera_controller = nullptr;
};
