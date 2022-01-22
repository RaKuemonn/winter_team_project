#pragma once

#include <d3d11.h>
#include "memory"
#include "sprite.h"
<<<<<<< HEAD
#include "binary_loader.h"
=======
#include "input_manager.h"
>>>>>>> 667e84f148c8f6b717b3b9629e885ba4895596ab



enum class Stage_Select
{
    STAGE_1 = 0,
    STAGE_2,
    STAGE_3,
    STAGE_4,
    STAGE_BOSS
};


class Option_Manager
{
public:
    Option_Manager(ID3D11Device* device, ID3D11DeviceContext* context);
    ~Option_Manager() { File_IO::write("save.dat", binary_data); }

    void update(float elapsedTime, Input_Manager* input_manager);
    void setvolume(float elapsedTime, Input_Manager* input_manager);
    // デバッグ用GUI描画
    void DrawDebugGUI();
    void render();

<<<<<<< HEAD
    void set_next_stage(Stage_Select next_stage) { now_stage = next_stage; }
    Stage_Select get_now_stage() { return now_stage; }
    Binary_Data& get_binary() { return binary_data; }
=======

public:
    float bgm_vo = 0;         // BGMのボリューム 1~0の間で替ええってくる
    float bgm_bar = 126;      // BGMのボリュームバーの中身をどれくらい表示させるか
    float bgm_move = 0;       // BGMのボリュームバーの中身の描画が全体の何パーセントか
    
    float se_vo = 0;          // SEのボリューム 1~0の間で替ええってくる
    float se_bar = 126;       // SEのボリュームバーの中身をどれくらい表示させるか
    float se_move = 0;        // SEのボリュームバーの中身の描画が全体の何パーセントか
    
    float camera_bar = 252;     // カメラ感度バーの中身をどれくらい表示させるか
    float camera_move = 0;    // カメラ感度バーの中身の描画が全体の何パーセントか

    float icon_eas = 0;     // アイコンの移動距離
    float icon_pos = 0;     //　アイコンの位置修正

    float time = 0; // 経過時間

    float arrow_x = 0;
    float arrow_y = 0;

    bool up_flag = false;
    bool down_flag = false;

    int icon_select = 0;

    
>>>>>>> 667e84f148c8f6b717b3b9629e885ba4895596ab

private:
    ID3D11DeviceContext* immediate_context;

    std::unique_ptr<Sprite> back = nullptr;
<<<<<<< HEAD
    //std::unique_ptr<Sprite> option_1 = nullptr;

    Stage_Select now_stage = Stage_Select::STAGE_1;
    Binary_Data binary_data {};
=======
    std::unique_ptr<Sprite> bar_back = nullptr;
    std::unique_ptr<Sprite> bgm = nullptr;
    std::unique_ptr<Sprite> se = nullptr;
    std::unique_ptr<Sprite> camera = nullptr;
    std::unique_ptr<Sprite> option = nullptr;
    std::unique_ptr<Sprite> bar = nullptr;
    std::unique_ptr<Sprite> icon = nullptr;
    std::unique_ptr<Sprite> arrow = nullptr;
>>>>>>> 667e84f148c8f6b717b3b9629e885ba4895596ab
};
