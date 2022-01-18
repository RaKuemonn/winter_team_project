#pragma once

#include <memory>
#include "scene.h"
#include "sprite_batch.h"
#include "sound.h"
#include "sky_box.h"
#include "model.h"
#include "camera_controller.h" 



class Scene_Manager;

class Scene_Select : public Scene
{
public:
    ~Scene_Select() override {}

	void initialize(Scene_Manager* parent_) override;
	void uninitialize() override;
	void update(float elapsed_time) ;
	void move(float elapsedTime, Input_Manager* input_manager);
	void render(float elapsed_time) override;


	// デバッグ用GUI描画
	void DrawDebugGUI();


private:

	float position = 0;
	float left_max = -500;
	float light_max = 500;
	float move_left = 0; // 左に動く分
	float move_right = 0;  // 右に動く分
	bool left_flag = false; // 左に動く式のフラグ
	bool right_flag = false;  // 左に動く式のフラグ

	float time = 0.0f;
	float eas = 0.0;


	std::unique_ptr<Sprite> back = nullptr; // 背景
	std::unique_ptr<Sprite> stage1 = nullptr; 
	std::unique_ptr<Sprite> stage2 = nullptr;
	std::unique_ptr<Sprite> stage3 = nullptr;
	std::unique_ptr<Sprite> stage4 = nullptr;
	std::unique_ptr<Sprite> stage5 = nullptr;
	std::unique_ptr<Sprite> choice = nullptr; // 枠　今は黒い四角
	std::unique_ptr<Scene_Manager> scene_manager = nullptr;

};