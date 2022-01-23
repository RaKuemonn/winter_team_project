#pragma once

#include <memory>
#include "scene.h"
#include "enemy_spawner.h"
#include "stage_spawner.h"
#include "collision_manager.h"
#include "sky_box.h"
#include "decotator_supporter.h"
#include "clear_judge.h"


class Scene_Game : public Scene
{
public:
	~Scene_Game() override = default;

	void initialize(Scene_Manager* parent_) override;
	void uninitialize() override;
	void update(float elapsed_time) override;
	void render(float elapsed_time) override;

private:
	std::unique_ptr<Sky_Box> sky_box = nullptr;

	std::unique_ptr<Enemy_Spawner>		enemy_spawner = nullptr;
	std::unique_ptr<Stage_Spawner>		stage_spawner = nullptr;
	std::unique_ptr<Collision_Manager>	collision_manager = nullptr;

	std::unique_ptr<Clear_Judge> clear_judge		= nullptr;			// �X�e�[�W�̃S�[������ƃ{�X��|�������𔻒肵�Ă���


	std::unique_ptr<Decotator_Supporter> debug_decorator_supporter = nullptr;
	std::unique_ptr<class Camera_Controller> camera_controller = nullptr;
};
