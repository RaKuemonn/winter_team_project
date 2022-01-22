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
	void move(float elapsedTime, Input_Manager* input_manager); // アイコンを動かす関数
	void slideshow(float elapsedTime);  // 背景がスライドショーする関数
	void option(float elapsedTime, Input_Manager* input_manager);
	void render(float elapsed_time) override;


	// デバッグ用GUI描画
	void DrawDebugGUI();


private:

	float slidetimer = 0.0f; // スライドショー用のタイマー
	int slide = 0; //スライドショーの切り替わるタイミングの合計
	float integer = 0;  // slidetimerの整数部分
	float decimals = 0;// slidetimerの小数部分

	float time = 0.0f;       // 時間
	float icon_eas = 0.0f;   //　移動量
	float icon_pos = 0.0;    // 位置修正
	int timer = 0;

	bool icon_up_flag = false;// 上に上がるときのフラグ
	bool icon_down_flag = false; // 下に下がるときのフラグ
	bool icon_flag = false;      // enterボタンが押されてるかのフラグ

	bool move_flag = false;     // start画面に行くとき
	bool option_flag = false;   // option画面に行くとき

	float option_vo = 70; //　音量設定
	float gauge = 0;
	float gauge_pos = 0;

	float back_stage1_alpha = 1; // stage1の背景のアルファ値
	float back_stage2_alpha = 1; // stage2の背景のアルファ値
	float back_stage3_alpha = 1; // stage3の背景のアルファ値
	float back_stage4_alpha = 0; // stage4の背景のアルファ値

	
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