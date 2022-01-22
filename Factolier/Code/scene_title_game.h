#pragma once

#include <memory>
#include "scene.h"
#include "sprite_batch.h"
#include "sound.h"
#include "sky_box.h"
#include "model.h"
#include "camera_controller.h" 




class Scene_Title_Game : public Scene
{
public:
	~Scene_Title_Game() override {}

	void initialize(Scene_Manager* parent_) override;
	void uninitialize() override;
	void update(float elapsed_time);
	void move(float elapsedTime, Input_Manager* input_manager); // �A�C�R���𓮂����֐�
	void slideshow(float elapsedTime);  // �w�i���X���C�h�V���[����֐�
	void option(float elapsedTime, Input_Manager* input_manager);
	void render(float elapsed_time) override;


	// �f�o�b�O�pGUI�`��
	void DrawDebugGUI();


private:

	float slidetimer = 0.0f; // �X���C�h�V���[�p�̃^�C�}�[
	int slide = 0; //�X���C�h�V���[�̐؂�ւ��^�C�~���O�̍��v
	float integer = 0;  // slidetimer�̐�������
	float decimals = 0;// slidetimer�̏�������

	float time = 0.0f;       // ����
	float icon_eas = 0.0f;   //�@�ړ���
	float icon_pos = 0.0;    // �ʒu�C��
	int timer = 0;

	bool icon_up_flag = false;// ��ɏオ��Ƃ��̃t���O
	bool icon_down_flag = false; // ���ɉ�����Ƃ��̃t���O
	bool icon_flag = false;      // enter�{�^����������Ă邩�̃t���O

	bool move_flag = false;     // start��ʂɍs���Ƃ�
	bool option_flag = false;   // option��ʂɍs���Ƃ�

	float option_vo = 70; //�@���ʐݒ�
	float gauge = 0;
	float gauge_pos = 0;

	float back_stage1_alpha = 1; // stage1�̔w�i�̃A���t�@�l
	float back_stage2_alpha = 1; // stage2�̔w�i�̃A���t�@�l
	float back_stage3_alpha = 1; // stage3�̔w�i�̃A���t�@�l
	float back_stage4_alpha = 0; // stage4�̔w�i�̃A���t�@�l

	
	std::unique_ptr<Sprite> title = nullptr;

	std::unique_ptr<Sprite> back_stage1 = nullptr;
	std::unique_ptr<Sprite> back_stage2 = nullptr;
	std::unique_ptr<Sprite> back_stage3 = nullptr;
	std::unique_ptr<Sprite> back_stage4 = nullptr;
	std::unique_ptr<Sprite> back_stage0 = nullptr;


	std::unique_ptr<Sprite> start = nullptr;
	std::unique_ptr<Sprite> icon = nullptr;
	std::unique_ptr<Sprite> option_start = nullptr;

	std::unique_ptr<Sound> sound = nullptr;
	std::unique_ptr<Sound> se = nullptr;


};