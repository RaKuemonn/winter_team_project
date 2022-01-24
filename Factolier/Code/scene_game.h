#pragma once

#include <memory>
#include "scene.h"
#include "enemy_spawner.h"
#include "stage_spawner.h"
#include "collision_manager.h"
#include "sky_box.h"
#include "decotator_supporter.h"
#include "clear_judge.h"
#include "sound.h"


class Scene_Game : public Scene
{
public:
	~Scene_Game() override = default;

	void initialize(Scene_Manager* parent_) override;
	void uninitialize() override;
	void update(float elapsed_time) override;
	void render(float elapsed_time) override;

private:
	void init_player_position(const Stage_Select stage_, std::weak_ptr<Entity> player_);
	void init_stage(const Stage_Select stage_);
    short* init_enemy(const Stage_Select stage_,const DirectX::XMFLOAT3& target_position);
	bool judge_clear();

private:
	std::unique_ptr<Sky_Box> sky_box = nullptr;

	std::unique_ptr<Collision_Manager>	collision_manager = nullptr;

	std::unique_ptr<Clear_Judge> clear_judge		= nullptr;			// ステージのゴール判定とボスを倒したかを判定している


	std::unique_ptr<Decotator_Supporter> debug_decorator_supporter = nullptr;
	std::unique_ptr<class Camera_Controller> camera_controller = nullptr;

	std::unique_ptr<Sprite> clear_back = nullptr;

	std::unique_ptr<Sound> bgm_stage1 = nullptr;
	std::unique_ptr<Sound> bgm_stage2 = nullptr;
	std::unique_ptr<Sound> bgm_stage3 = nullptr;
	std::unique_ptr<Sound> bgm_stage4 = nullptr;
	std::unique_ptr<Sound> bgm_boss = nullptr;
	bool is_option = false;
	bool clear_end = false;
};
