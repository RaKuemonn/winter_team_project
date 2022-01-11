#pragma once

#include <memory>
#include "scene.h"
#include "sprite_batch.h"
#include "sound.h"
#include "sky_box.h"
#include "model.h"
#include "camera_controller.h" 
#include "input_manager.h"
#include "sprite.h"


class Scene_Select : public Scene
{
public:
    ~Scene_Select() override {}

	void initialize() override;
	void uninitialize() override;
	void update(float elapsed_time) ;
	void move(float elapsedTime, Input_Manager* input_manager);
	void render(float elapsed_time) override;


	// �f�o�b�O�pGUI�`��
	void DrawDebugGUI();


private:

	float position = 0;
	float left_max = 500;
	float light_max = 500;
	float move_left = 0; // ���ɓ�����
	float move_right = 0;  // �E�ɓ�����
	bool left_flag = false; // ���ɓ������̃t���O
	bool right_flag = false;  // ���ɓ������̃t���O

	std::unique_ptr<Sprite_Batch> select_back = nullptr; //�@�w�i
	std::unique_ptr<Sprite_Batch> select_stage1; //�@�w�i
	std::unique_ptr<Sprite> back = nullptr;
	std::unique_ptr<Sprite> stage1 = nullptr;
	std::unique_ptr<Sprite> stage2 = nullptr;
	std::unique_ptr<Sprite> stage3 = nullptr;
	std::unique_ptr<Sprite> stage4 = nullptr;
	std::unique_ptr<Sprite> stage5 = nullptr;
	std::unique_ptr<Sprite> choice = nullptr;
	std::unique_ptr<Scene_Manager> scene_manager = nullptr;

};